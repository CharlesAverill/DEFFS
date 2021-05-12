/*
* FILENAME: perms.c
*
* DESCRIPTION: FUSE callbacks relating to file permissions
*
* AUTHOR: Charles Averill
*/

#include "perms.h"

int deffs_access(const char *path, int mask)
{
    int res;

    path = deffs_path_prepend(path, storepoint);

    res = access(path, mask);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_chmod(const char *path, mode_t mode)
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

int deffs_chown(const char *path, uid_t uid, gid_t gid)
{
    int res;

    path = deffs_path_prepend(path, storepoint);

    res = lchown(path, uid, gid);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_statfs(const char *path, struct statvfs *stbuf)
{
    int res;

    path = deffs_path_prepend(path, storepoint);

    res = statvfs(path, stbuf);
    if (res == -1)
        return -errno;

    return 0;
}
