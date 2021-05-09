void *deffs_init(struct fuse_conn_info *conn);
static int deffs_getattr(const char *path, struct stat *stbuf);
static int deffs_fgetattr(const char *path, struct stat *stbuf,
			struct fuse_file_info *fi);
static int deffs_access(const char *path, int mask);
static int deffs_opendir(const char *path, struct fuse_file_info *fi);

static int deffs_chmod(const char *path, mode_t mode);
static int deffs_chown(const char *path, uid_t uid, gid_t gid);

static int deffs_truncate(const char *path, off_t size);
static int deffs_ftruncate(const char *path, off_t size,
			struct fuse_file_info *fi);

static int deffs_utimens(const char *path, const struct timespec ts[2]);
static int deffs_create(const char *path, mode_t mode, struct fuse_file_info *fi);
static int deffs_open(const char *path, struct fuse_file_info *fi);

static int deffs_mkdir(const char *path, mode_t mode);

static int deffs_read(const char *path, char *buf, size_t size, off_t offset,
			struct fuse_file_info *fi);
static int deffs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			off_t offset, struct fuse_file_info *fi);
static int deffs_releasedir(const char *path, struct fuse_file_info *fi);

static int deffs_write(const char *path, const char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi);
static int deffs_write_buf(const char *path, struct fuse_bufvec *buf,
			off_t offset, struct fuse_file_info *fi);
static int deffs_statfs(const char *path, struct statvfs *stbuf);

static int deffs_flush(const char *path, struct fuse_file_info *fi);
static int deffs_release(const char *path, struct fuse_file_info *fi);
