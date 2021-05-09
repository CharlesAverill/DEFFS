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

#include "deffs.h"

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
	//.destroy	= deffs_destroy,
	.getattr	= deffs_getattr,
	/*
	.fgetattr	= deffs_fgetattr,
	.readlink	= deffs_readlink,
	.opendir	= deffs_opendir,
	*/
	.readdir	= deffs_readdir,
	/*
	.releasedir	= deffs_releasedir,
	.mknod		= deffs_mknod,
	*/
	.mkdir		= deffs_mkdir,
	/*
	.symlink	= deffs_symlink,
	.unlink		= deffs_unlink,
	.rmdir		= deffs_rmdir,
	.rename		= deffs_rename,
	.link		= deffs_link,
	.chmod		= deffs_chmod,
	.chown		= deffs_chown,
	.truncate	= deffs_truncate,
	.ftruncate	= deffs_ftruncate,
	*/
	.create		= deffs_create,
	.open		= deffs_open,
	.read		= deffs_read,
	/*
	.read_buf	= deffs_read_buf,
	*/
	.write		= deffs_write,
	.write_buf	= deffs_write_buf,
	/*
	.statfs		= deffs_statfs,
	.flush		= deffs_flush,
	.release	= deffs_release,
	.fsync		= deffs_fsync,
	*/
};

static int deffs_getattr(const char *path, struct stat *stbuf)
{
	int res;

	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_mkdir(const char *path, mode_t mode)
{
	int res;

	printf("Making directory");

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int deffs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	int fd;

	fd = open(path, fi->flags, mode);
	if (fd == -1)
		return -errno;

	fi->fh = fd;
	return 0;
}

static int deffs_open(const char *path, struct fuse_file_info *fi)
{
	int fd;

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

static int deffs_write(const char *path, const char *buf, size_t size,
							off_t offset, struct fuse_file_info *fi)
{
	int res;

	printf("Writing at %s", path);

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

	(void) path;

	dst.buf[0].flags = FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK;
	dst.buf[0].fd = fi->fh;
	dst.buf[0].pos = offset;

	return fuse_buf_copy(&dst, buf, FUSE_BUF_SPLICE_NONBLOCK);
}

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &deffs_oper, NULL);
}
