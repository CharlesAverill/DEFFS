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
