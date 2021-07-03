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

#include "utils.h"
#include "arguments.h"
#include "attr.h"
#include "crypto.h"
#include "perms.h"
#include "rw.h"

#include "net/client.h"
#include "net/server.h"

#include "c-sss/shamir.h"

extern char *mountpoint;
extern char *storepoint;
extern char *shardpoint;

extern int n_machines;
extern int port;

extern struct connection *host_connection;

// These include the null terminator
#define SHARD_HASH_LEN SHA256_DIGEST_LENGTH
#define SHARD_SUFF_LEN 12 // -XXXX.shard
#define SHARD_KEY_LEN 17

#define DEFAULT_PORT 13035 // Conway's Constant

typedef struct deffs_dirp {
    DIR *dp;
    struct dirent *entry;
    off_t offset;
} deffs_dirp;

static inline struct deffs_dirp *get_dirp(struct fuse_file_info *fi)
{
    return (struct deffs_dirp *) (uintptr_t) fi->fh;
}

void *deffs_init(struct fuse_conn_info *conn);
void deffs_destroy(void *private_data);

#endif
