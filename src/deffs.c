/*
* FILENAME: deffs.c
*
* DESCRIPTION: Main DEFFS logic, including binding DEFFS methods to FUSE
*                callbacks. Compiles to executable that mounts DEFFS filesystem.
*
* USAGE: cmake --build ./ --target DEFFS -- -j 6
*        mkdir ~/deffs
*        mkdir ~/deffs_storage
*        ./bin/DEFFS ~/deffs ~/deffs_storage
*
* AUTHOR: Charles Averill
*/

#define FUSE_USE_VERSION 29

#include "deffs.h"

char *mountpoint;
char *storepoint;
char *shardpoint;

int n_machines;
int port;

struct connection *host_connection;

static struct fuse_operations deffs_oper = {
    .init     = deffs_init,
    .destroy  = deffs_destroy,
    .getattr  = deffs_getattr,
    .fgetattr = deffs_fgetattr,
#ifndef __APPLE__
    .access = deffs_access,
#endif
    .readlink   = deffs_readlink,
    .opendir    = deffs_opendir,
    .readdir    = deffs_readdir,
    .releasedir = deffs_releasedir,
    .mknod      = NULL, //deffs_mknod,
    .mkdir      = deffs_mkdir,
    .symlink    = deffs_symlink,
    .unlink     = deffs_unlink,
    .rmdir      = deffs_rmdir,
    .rename     = deffs_rename,
    .link       = deffs_link,
    .chmod      = deffs_chmod,
    .chown      = deffs_chown,
    .truncate   = deffs_truncate,
    .ftruncate  = deffs_ftruncate,
#ifdef HAVE_UTIMENSAT
    .utimens = deffs_utimens,
#endif
    .create   = deffs_create,
    .open     = deffs_open,
    .read     = deffs_read,
    .read_buf = NULL, //deffs_read_buf,
    .write    = deffs_write,
    .write_buf =
        NULL, //deffs_write_buf, // This one could be implemented but it's different enough from write that I don't want to yet
    .statfs  = deffs_statfs,
    .flush   = deffs_flush,
    .release = deffs_release,
    .fsync   = NULL, //deffs_fsync,
#if defined(HAVE_POSIX_FALLOCATE) || defined(__APPLE__)
    .fallocate = deffs_fallocate,
#endif
#ifdef HAVE_SETXATTR
    .setxattr    = deffs_setxattr,
    .getxattr    = deffs_getxattr,
    .listxattr   = deffs_listxattr,
    .removexattr = deffs_removexattr,
#endif
#ifndef __APPLE__
    .lock  = NULL, //deffs_lock,
    .flock = NULL, //deffs_flock,
#endif
#ifdef __APPLE__
    .setvolname  = NULL, //deffs_setvolname,
    .exchange    = NULL, //deffs_exchange,
    .getxtimes   = NULL, //deffs_getxtimes,
    .setbkuptime = NULL, //deffs_setbkuptime,
    .setchgtime  = NULL, //deffs_setchgtime,
    .setcrtime   = NULL, //deffs_setcrtime,
    .chflags     = NULL, //deffs_chflags,
    .setattr_x   = NULL, //deffs_setattr_x,
    .fsetattr_x  = NULL, //deffs_fsetattr_x,
#endif
    .flag_nullpath_ok = 1,
#if HAVE_UTIMENSAT
    .flag_utime_omit_ok = 1,
#endif
};

void *deffs_init(struct fuse_conn_info *conn)
{
#ifdef __APPLE__
    FUSE_ENABLE_SETVOLNAME(conn);
    FUSE_ENABLE_XTIMES(conn);
#endif
    // Create the shardpoint directory
    if (mkdir_if_not_exists(shardpoint, 0700) != 0) {
        printf("%s", "Shards directory detected. When mounting DEFFS, use a fresh mountpoint and "
                     "storepoint");
        exit(1);
    }

    host_connection = sconnect(port);

    return NULL;
}

void deffs_destroy(void *private_data)
{
    close_conn(host_connection);
    printf("\nThank you for using DEFFS!\n");
}

const char *argp_program_version     = "DEFFS 0.0.3";
const char *argp_program_bug_address = "charles.averill@utdallas.edu";
static char doc[]                    = "Distributed, Encrypted, Fractured File System";
static char args_doc[]               = "MOUNTPOINT STOREPOINT";

static struct argp_option options[] = {
    {"n_machines", 'n', "INTEGER", 0, "Number of machines to interact with, default is 1"},
    {"port", 'p', "INTEGER", 0, "Port on which DEFFS machines will interact"},
    {0}};

static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[])
{
    // Argument parsing
    struct arguments arguments;
    arguments.n_machines = 1;
    arguments.port       = DEFAULT_PORT;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // Setup mount, store, and shardpoints
    mountpoint = arguments.points[0];
    storepoint = arguments.points[1];

    shardpoint = malloc(strlen(storepoint) + strlen("/.shards/") + 1);
    strcpy(shardpoint, storepoint);
    strcat(shardpoint, "/.shards/");
    shardpoint[strlen(storepoint) + strlen("/.shards/") + 1] = '\0';

    char *static_argv[] = {argv[0], mountpoint, "-o", "allow_other", "-d", "-s", "-f"};
    int static_argc     = sizeof(static_argv) / sizeof(static_argv[0]);

    // Setup machines
    n_machines = arguments.n_machines;
    port       = arguments.port;

    printf("Searching for %d machines\n", n_machines);
    printf("Port: %d\n", arguments.port);

    // Start FUSE
    return fuse_main(static_argc, static_argv, &deffs_oper, NULL);
}
