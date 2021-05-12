/*
* FILENAME: deffs.h
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

#include "utils.h"
#include "deffs.h"

#include "attr.h"
#include "arguments.h"
#include "crypto.h"
#include "perms.h"
#include "rw.h"

char *mountpoint;
char *storepoint;
char *shardpoint;

static struct fuse_operations deffs_oper = {
	.init           = deffs_init,
	.destroy        = NULL, //deffs_destroy,
	.getattr        = deffs_getattr,
	.fgetattr       = deffs_fgetattr,
	#ifndef __APPLE__
	.access         = deffs_access,
	#endif
	.readlink       = NULL, //deffs_readlink,
	.opendir        = deffs_opendir,
	.readdir        = deffs_readdir,
	.releasedir     = deffs_releasedir,
	.mknod          = NULL, //deffs_mknod,
	.mkdir          = deffs_mkdir,
	.symlink        = NULL, //deffs_symlink,
	.unlink         = deffs_unlink,
	.rmdir          = NULL, //deffs_rmdir,
	.rename         = NULL, //deffs_rename,
	.link           = NULL, //deffs_link,
	.chmod          = deffs_chmod,
	.chown          = deffs_chown,
	.truncate       = deffs_truncate,
	.ftruncate      = deffs_ftruncate,
	#ifdef HAVE_UTIMENSAT
	.utimens        = deffs_utimens,
	#endif
	.create         = deffs_create,
	.open           = deffs_open,
	.read           = deffs_read,
	.read_buf       = NULL, //deffs_read_buf,
	.write          = deffs_write,
	.write_buf      = NULL, //deffs_write_buf, // This one could be implemented but it's different enough from write that I don't want to yet
	.statfs         = deffs_statfs,
	.flush          = deffs_flush,
	.release        = deffs_release,
	.fsync          = NULL, //deffs_fsync,
	#if defined(HAVE_POSIX_FALLOCATE) || defined(__APPLE__)
	.fallocate      = deffs_fallocate,
	#endif
	#ifdef HAVE_SETXATTR
	.setxattr       = deffs_setxattr,
	.getxattr       = deffs_getxattr,
	.listxattr      = deffs_listxattr,
	.removexattr    = deffs_removexattr,
	#endif
	#ifndef __APPLE__
	.lock           = NULL, //deffs_lock,
	.flock          = NULL, //deffs_flock,
	#endif
	#ifdef __APPLE__
	.setvolname     = NULL, //deffs_setvolname,
	.exchange       = NULL, //deffs_exchange,
	.getxtimes      = NULL, //deffs_getxtimes,
	.setbkuptime    = NULL, //deffs_setbkuptime,
	.setchgtime     = NULL, //deffs_setchgtime,
	.setcrtime      = NULL, //deffs_setcrtime,
	.chflags        = NULL, //deffs_chflags,
	.setattr_x      = NULL, //deffs_setattr_x,
	.fsetattr_x     = NULL, //deffs_fsetattr_x,
	#endif
	.flag_nullpath_ok   = 1,
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
	if(mkdir_if_not_exists(shardpoint, 0700) != 0){
		printf("%s", "Shards directory detected. When mounting DEFFS, use a fresh mountpoint and storepoint");
		exit(1);
	}

	return NULL;
}

const char *argp_program_version = "DEFFS 0.0.1";
const char *argp_program_bug_address = "charlesaverill20@gmail.com";
const char doc[] = "Distributed, Encrypted, Fractured File System";
static char args_doc[] = "MOUNTPOINT STOREPOINT";

static struct argp_option options[] = {
	{ 0 }
};

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char *argv[])
{
	// Argument parsing
	struct arguments arguments;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	// Setup mount, store, and shardpoints
	mountpoint = arguments.points[0];
	storepoint = arguments.points[1];

	shardpoint = malloc(strlen(storepoint) + strlen("/.shards/") + 1);
	strcpy(shardpoint, storepoint);
	strcat(shardpoint, "/.shards/");
	shardpoint[strlen(storepoint) + strlen("/.shards/") + 1] = '\0';

	// Start FUSE
	char *static_argv[] = {argv[0], mountpoint, "-o", "allow_other", "-d", "-s", "-f"};
	int static_argc = sizeof(static_argv) / sizeof(static_argv[0]);

	return fuse_main(static_argc, static_argv, &deffs_oper, NULL);
}
