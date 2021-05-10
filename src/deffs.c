#define FUSE_USE_VERSION 29

#include <fuse.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "utils.h"
#include "attr.h"
#include "arguments.h"
#include "crypto.h"
#include "deffs.h"

char *mountpoint;
char *storepoint;

struct deffs_dirp {
	DIR *dp;
	struct dirent *entry;
	off_t offset;
};

static inline struct deffs_dirp *get_dirp(struct fuse_file_info *fi)
{
	return (struct deffs_dirp *) (uintptr_t) fi->fh;
}

static struct fuse_operations deffs_oper = {
	.init	   	= deffs_init,
	.destroy	= NULL, //deffs_destroy,
	.getattr	= deffs_getattr,
	.fgetattr	= deffs_fgetattr,
#ifndef __APPLE__
	.access		= deffs_access,
#endif
	.readlink	= NULL, //deffs_readlink,
	.opendir	= deffs_opendir,
	.readdir	= deffs_readdir,
	.releasedir	= deffs_releasedir,
	.mknod		= NULL, //deffs_mknod,
	.mkdir		= deffs_mkdir,
	.symlink	= NULL, //deffs_symlink,
	.unlink		= NULL, //deffs_unlink,
	.rmdir		= NULL, //deffs_rmdir,
	.rename		= NULL, //deffs_rename,
	.link		= NULL, //deffs_link,
	.chmod		= deffs_chmod,
	.chown		= deffs_chown,
	.truncate	= deffs_truncate,
	.ftruncate	= deffs_ftruncate,
#ifdef HAVE_UTIMENSAT
	.utimens	= deffs_utimens,
#endif
	.create		= deffs_create,
	.open		= deffs_open,
	.read		= deffs_read,
	.read_buf	= NULL, //deffs_read_buf,
	.write		= deffs_write,
	.write_buf	= deffs_write_buf,
	.statfs		= deffs_statfs,
	.flush		= deffs_flush,
	.release	= deffs_release,
	.fsync		= NULL, //deffs_fsync,
#if defined(HAVE_POSIX_FALLOCATE) || defined(__APPLE__)
	.fallocate	= deffs_fallocate,
#endif
#ifdef HAVE_SETXATTR
	.setxattr	= deffs_setxattr,
	.getxattr	= deffs_getxattr,
	.listxattr	= deffs_listxattr,
	.removexattr	= deffs_removexattr,
#endif
#ifndef __APPLE__
	.lock		= NULL, //deffs_lock,
	.flock		= NULL, //deffs_flock,
#endif
#ifdef __APPLE__
	.setvolname	= NULL, //deffs_setvolname,
	.exchange	= NULL, //deffs_exchange,
	.getxtimes	= NULL, //deffs_getxtimes,
	.setbkuptime	= NULL, //deffs_setbkuptime,
	.setchgtime	= NULL, //deffs_setchgtime,
	.setcrtime	= NULL, //deffs_setcrtime,
	.chflags	= NULL, //deffs_chflags,
	.setattr_x	= NULL, //deffs_setattr_x,
	.fsetattr_x	= NULL, //deffs_fsetattr_x,
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
	return NULL;
}

static int deffs_getattr(const char *path, struct stat *stbuf)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_fgetattr(const char *path, struct stat *stbuf,
			struct fuse_file_info *fi)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	(void) path;

	res = fstat(fi->fh, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_access(const char *path, int mask)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = access(path, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_opendir(const char *path, struct fuse_file_info *fi)
{
	int res;
	struct deffs_dirp *d = malloc(sizeof(struct deffs_dirp));
	if (d == NULL)
		return -ENOMEM;

	path = deffs_path_prepend(path, storepoint);

	d->dp = opendir(path);
	if (d->dp == NULL) {
		res = -errno;
		free(d);
		return res;
	}
	d->offset = 0;
	d->entry = NULL;

	fi->fh = (unsigned long) d;
	return 0;
}

static int deffs_mkdir(const char *path, mode_t mode)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_chmod(const char *path, mode_t mode)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

#ifdef __APPLE__
	res = lchmod(path, mode);
#else
	res = chmod(path, mode);
#endif
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = lchown(path, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_truncate(const char *path, off_t size)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = truncate(path, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_ftruncate(const char *path, off_t size,
			struct fuse_file_info *fi)
{
	int res;

	(void) path;

	res = ftruncate(fi->fh, size);
	if (res == -1)
		return -errno;

	return 0;
}

#ifdef HAVE_UTIMENSAT
static int deffs_utimens(const char *path, const struct timespec ts[2])
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	/* don't use utime/utimes since they follow symlinks */
	res = utimensat(0, path, ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
		return -errno;

	return 0;
}
#endif

static int deffs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	int fd;

	path = deffs_path_prepend(path, storepoint);

	fd = open(path, fi->flags, mode);
	if (fd == -1)
		return -errno;

	fi->fh = fd;
	return 0;
}

static int deffs_open(const char *path, struct fuse_file_info *fi)
{
	int fd;

	path = deffs_path_prepend(path, storepoint);

	fd = open(path, fi->flags);
	if (fd == -1)
		return -errno;

	fi->fh = fd;
	return 0;
}

static int deffs_read(const char *path, char *buf, size_t size, off_t offset,
			struct fuse_file_info *fi)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	(void) path;
	res = pread(fi->fh, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
}

static int deffs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			off_t offset, struct fuse_file_info *fi)
{
	struct deffs_dirp *d = get_dirp(fi);

	path = deffs_path_prepend(path, storepoint);

	(void) path;
	if (offset != d->offset) {
		seekdir(d->dp, offset);
		d->entry = NULL;
		d->offset = offset;
	}

	while (1) {
		struct stat st;
		off_t nextoff;

		if (!d->entry) {
			d->entry = readdir(d->dp);
			if (!d->entry)
				break;
		}

		memset(&st, 0, sizeof(st));
		st.st_ino = d->entry->d_ino;
		st.st_mode = d->entry->d_type << 12;
		nextoff = telldir(d->dp);
		if (filler(buf, d->entry->d_name, &st, nextoff))
			break;

		d->entry = NULL;
		d->offset = nextoff;
	}

	return 0;
}

static int deffs_releasedir(const char *path, struct fuse_file_info *fi)
{
	struct deffs_dirp *d = get_dirp(fi);
	path = deffs_path_prepend(path, storepoint);
	(void) path;
	closedir(d->dp);
	free(d);
	return 0;
}

static int deffs_write(const char *path, const char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	(void) path;
	res = pwrite(fi->fh, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
}

static int deffs_write_buf(const char *path, struct fuse_bufvec *buf,
			off_t offset, struct fuse_file_info *fi)
{
	struct fuse_bufvec dst = FUSE_BUFVEC_INIT(fuse_buf_size(buf));

	path = deffs_path_prepend(path, storepoint);

	(void) path;

	dst.buf[0].flags = FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK;
	dst.buf[0].fd = fi->fh;
	dst.buf[0].pos = offset;

	return fuse_buf_copy(&dst, buf, FUSE_BUF_SPLICE_NONBLOCK);
}

static int deffs_statfs(const char *path, struct statvfs *stbuf)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = statvfs(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_flush(const char *path, struct fuse_file_info *fi)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	(void) path;
	/* This is called from every close on an open file, so call the
	   close on the underlying filesystem.	But since flush may be
	   called multiple times for an open file, this must not really
	   close the file.  This is important if used on a network
	   filesystem like NFS which flush the data/metadata on close() */
	res = close(dup(fi->fh));
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_release(const char *path, struct fuse_file_info *fi)
{
	path = deffs_path_prepend(path, storepoint);

	(void) path;
	close(fi->fh);

	return 0;
}

const char *argp_program_version = "DEFFS 0.0.1";
const char *argp_program_bug_address = "charlesaverill20@gmail.com";
static char doc[] = "Distributed, Encrypted, Fractured File System";
static char args_doc[] = "MOUNTPOINT STOREPOINT";

static struct argp_option options[] = {
    /*
    {"mountpoint", 'm', "STRING", 0, "The path to the folder in which DEFFS will be mounted"},
    {"storepoint", 's', "STRING", 0, "The path to the folder in which DEFFS file headers will be stored persistently"},
    */
    { 0 }
};

static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

int main(int argc, char *argv[])
{
	struct arguments arguments;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

	if(arguments.points[0] == NULL){
		printf("ERROR: Mountpoint required\n");
		exit(1);
	}
	if(arguments.points[1] == NULL){
		printf("ERROR: Storepoint required\n");
		exit(1);
	}

	mountpoint = arguments.points[0];
	storepoint = arguments.points[1];

	/*
	char *plaintext = "Hello world";

	struct EncryptionData *cipher = get_ciphertext(plaintext);
	printf("Cipher text: --%s--\n", cipher->ciphertext);

	struct EncryptionData *plain = get_plaintext(cipher->ciphertext, cipher->key);
	printf("Cipher text: --%s--\n", plain->plaintext);
	*/

	char *static_argv[] = {argv[0], mountpoint, "-o", "allow_other", "-o", "nonempty", "-d", "-s", "-f"};
	int static_argc = 9;

	return fuse_main(static_argc, static_argv, &deffs_oper, NULL);
}
