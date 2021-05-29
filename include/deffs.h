#ifndef DEFFS_H
#define DEFFS_H

#include <fuse.h>
#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "crypto.h"

extern char *mountpoint;
extern char *storepoint;
extern char *shardpoint;

#define SHARD_FN_LEN 64
#define SHARD_KEY_LEN 17

struct deffs_dirp {
    DIR *dp;
    struct dirent *entry;
    off_t offset;
};

static inline struct deffs_dirp *get_dirp(struct fuse_file_info *fi)
{
    return (struct deffs_dirp *) (uintptr_t) fi->fh;
}

void *deffs_init(struct fuse_conn_info *conn);

#endif
