#ifndef RW_H
#define RW_H

#include <fuse.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "deffs.h"
#include "crypto.h"

#include "c-sss/shamir.h"

#include "net/client.h"
#include "net/server.h"

int read_shard_data(FILE *header_pointer, size_t size, off_t offset, struct deffs_shard_data *sd);

int deffs_create(const char *path, mode_t mode, struct fuse_file_info *fi);
int deffs_open(const char *path, struct fuse_file_info *fi);

int deffs_readlink(const char *path, char *buf, size_t size);
int deffs_opendir(const char *path, struct fuse_file_info *fi);

int deffs_mknod(const char *path, mode_t mode, dev_t rdev);
int deffs_mkdir(const char *path, mode_t mode);
int deffs_unlink(const char *path);
int deffs_rmdir(const char *path);
int deffs_symlink(const char *from, const char *to);
int deffs_rename(const char *from, const char *to);
int deffs_link(const char *from, const char *to);

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
