/*
* FILENAME: attr.c
*
* DESCRIPTION: FUSE callbacks relating to file attributes
*
* AUTHOR: Charles Averill
*/

#include "attr.h"

int deffs_getattr(const char *path, struct stat *stbuf)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = lstat(path, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

int deffs_fgetattr(const char *path, struct stat *stbuf,
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

#ifdef HAVE_SETXATTR
/* xattr operations are optional and can safely be left unimplemented */
#ifdef __APPLE__
static int deffs_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags, uint32_t position)
#else
static int deffs_setxattr(const char *path, const char *name, const char *value,
			size_t size, int flags)
#endif
{
#ifdef __APPLE__
	int res;
	if (!strncmp(name, XATTR_APPLE_PREFIX, sizeof(XATTR_APPLE_PREFIX) - 1)) {
		flags &= ~(XATTR_NOSECURITY);
	}
	if (!strcmp(name, A_KAUTH_FILESEC_XATTR)) {
		char new_name[MAXPATHLEN];
		memcpy(new_name, A_KAUTH_FILESEC_XATTR, sizeof(A_KAUTH_FILESEC_XATTR));
		memcpy(new_name, G_PREFIX, sizeof(G_PREFIX) - 1);
		res = setxattr(path, new_name, value, size, position, flags);
	} else {
		res = setxattr(path, name, value, size, position, flags);
	}
#else
	int res = lsetxattr(path, name, value, size, flags);
#endif
	if (res == -1)
		return -errno;
	return 0;
}

#ifdef __APPLE__
static int deffs_getxattr(const char *path, const char *name, char *value,
			size_t size, uint32_t position)
#else
static int deffs_getxattr(const char *path, const char *name, char *value,
			size_t size)
#endif
{
#ifdef __APPLE__
	int res;
	if (strcmp(name, A_KAUTH_FILESEC_XATTR) == 0) {
		char new_name[MAXPATHLEN];
		memcpy(new_name, A_KAUTH_FILESEC_XATTR, sizeof(A_KAUTH_FILESEC_XATTR));
		memcpy(new_name, G_PREFIX, sizeof(G_PREFIX) - 1);
		res = getxattr(path, new_name, value, size, position, XATTR_NOFOLLOW);
	} else {
		res = getxattr(path, name, value, size, position, XATTR_NOFOLLOW);
	}
#else
	int res = lgetxattr(path, name, value, size);
#endif
	if (res == -1)
		return -errno;
	return res;
}

static int deffs_listxattr(const char *path, char *list, size_t size)
{
#ifdef __APPLE__
	ssize_t res = listxattr(path, list, size, XATTR_NOFOLLOW);
	if (res > 0) {
		if (list) {
			size_t len = 0;
			char *curr = list;
			do {
				size_t thislen = strlen(curr) + 1;
				if (strcmp(curr, G_KAUTH_FILESEC_XATTR) == 0) {
					memmove(curr, curr + thislen, res - len - thislen);
					res -= thislen;
					break;
				}
				curr += thislen;
				len += thislen;
			} while (len < res);
		} else {
			/*
			ssize_t res2 = getxattr(path, G_KAUTH_FILESEC_XATTR, NULL, 0, 0,
						XATTR_NOFOLLOW);
			if (res2 >= 0) {
				res -= sizeof(G_KAUTH_FILESEC_XATTR);
			}
			*/
		}
	}
#else
	int res = llistxattr(path, list, size);
#endif
	if (res == -1)
		return -errno;
	return res;
}

static int deffs_removexattr(const char *path, const char *name)
{
#ifdef __APPLE__
	int res;
	if (strcmp(name, A_KAUTH_FILESEC_XATTR) == 0) {
		char new_name[MAXPATHLEN];
		memcpy(new_name, A_KAUTH_FILESEC_XATTR, sizeof(A_KAUTH_FILESEC_XATTR));
		memcpy(new_name, G_PREFIX, sizeof(G_PREFIX) - 1);
		res = removexattr(path, new_name, XATTR_NOFOLLOW);
	} else {
		res = removexattr(path, name, XATTR_NOFOLLOW);
	}
#else
	int res = lremovexattr(path, name);
#endif
	if (res == -1)
		return -errno;
	return 0;
}
#endif /* HAVE_SETXATTR */
