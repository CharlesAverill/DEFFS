static int deffs_getattr(const char *path, struct stat *stbuf);
static int deffs_create(const char *path, mode_t mode, struct fuse_file_info *fi);
static int deffs_open(const char *path, struct fuse_file_info *fi);

static int deffs_mkdir(const char *path, mode_t mode);

static int deffs_read(const char *path, char *buf, size_t size, off_t offset,
							struct fuse_file_info *fi);
static int deffs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
							off_t offset, struct fuse_file_info *fi);

static int deffs_write(const char *path, const char *buf, size_t size,
							off_t offset, struct fuse_file_info *fi);
static int deffs_write_buf(const char *path, struct fuse_bufvec *buf,
							off_t offset, struct fuse_file_info *fi);
