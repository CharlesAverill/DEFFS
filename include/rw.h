#ifndef RW_H
#define RW_H

#include <fuse.h>
#include <errno.h>
#include <unistd.h>

#include "utils.h"
#include "deffs.h"
#include "crypto.h"

int deffs_create(const char *path, mode_t mode, struct fuse_file_info *fi);
int deffs_open(const char *path, struct fuse_file_info *fi);

int deffs_opendir(const char *path, struct fuse_file_info *fi);
int deffs_mkdir(const char *path, mode_t mode);

int deffs_read(const char *path, char *buf, size_t size, off_t offset,
			struct fuse_file_info *fi);
int deffs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			off_t offset, struct fuse_file_info *fi);

int deffs_write(const char *path, const char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi);
int deffs_write_buf(const char *path, struct fuse_bufvec *buf,
			off_t offset, struct fuse_file_info *fi);

int deffs_flush(const char *path, struct fuse_file_info *fi);

int deffs_truncate(const char *path, off_t size);
int deffs_ftruncate(const char *path, off_t size,
			struct fuse_file_info *fi);

int deffs_utimens(const char *path, const struct timespec ts[2]);

int deffs_release(const char *path, struct fuse_file_info *fi);
int deffs_releasedir(const char *path, struct fuse_file_info *fi);

#endif
