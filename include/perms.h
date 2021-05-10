#ifndef PERMS_H
#define PERMS_H

#include "utils.h"
#include "deffs.h"

int deffs_access(const char *path, int mask);

int deffs_chmod(const char *path, mode_t mode);
int deffs_chown(const char *path, uid_t uid, gid_t gid);

int deffs_statfs(const char *path, struct statvfs *stbuf);

#endif
