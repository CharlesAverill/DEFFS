#ifndef ATTR_H
#define ATTR_H

#include <fuse.h>
#include <errno.h>

#include "utils.h"
#include "deffs.h"

int deffs_getattr(const char *path, struct stat *stbuf);
int deffs_fgetattr(const char *path, struct stat *stbuf,
			struct fuse_file_info *fi);

#ifdef __APPLE__
static int deffs_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags, uint32_t position);
static int deffs_getxattr(const char *path, const char *name, char *value,
			size_t size, uint32_t position);
#else
static int deffs_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags);
static int deffs_getxattr(const char *path, const char *name, char *value,
            size_t size);
#endif

static int deffs_listxattr(const char *path, char *list, size_t size);
static int deffs_removexattr(const char *path, const char *name);

#endif
