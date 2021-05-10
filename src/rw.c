/*
* FILENAME: rw.c
*
* DESCRIPTION: FUSE callbacks relating to read, write, open, and close operations for files and folders
*
* AUTHOR: Charles Averill
*/

#include "rw.h"

int deffs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	int fd;

	path = deffs_path_prepend(path, storepoint);

	fd = open(path, fi->flags, mode);
	if (fd == -1)
		return -errno;

	fi->fh = fd;
	return 0;
}

int deffs_open(const char *path, struct fuse_file_info *fi)
{
	int fd;

	path = deffs_path_prepend(path, storepoint);

	fd = open(path, fi->flags);
	if (fd == -1)
		return -errno;

	fi->fh = fd;
	return 0;
}

int deffs_opendir(const char *path, struct fuse_file_info *fi)
{
	int res;
	struct deffs_dirp *d = malloc(sizeof(struct deffs_dirp));
	if (d == NULL)
		return -ENOMEM;

	path = deffs_path_prepend(path, storepoint);

	d->dp = opendir(path);
	if (d->dp == NULL) {
		res = -errno;
		free(d);
		return res;
	}
	d->offset = 0;
	d->entry = NULL;

	fi->fh = (unsigned long) d;
	return 0;
}

int deffs_mkdir(const char *path, mode_t mode)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

int deffs_unlink(const char *path)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = unlink(path);
	if (res == -1)
		return -errno;

	return 0;
}

int deffs_read(const char *path, char *buf, size_t size, off_t offset,
			struct fuse_file_info *fi)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	(void) path;
	res = pread(fi->fh, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
}

int deffs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			off_t offset, struct fuse_file_info *fi)
{
	struct deffs_dirp *d = get_dirp(fi);

	path = deffs_path_prepend(path, storepoint);

	(void) path;
	if (offset != d->offset) {
		seekdir(d->dp, offset);
		d->entry = NULL;
		d->offset = offset;
	}

	while (1) {
		struct stat st;
		off_t nextoff;

		if (!d->entry) {
			d->entry = readdir(d->dp);
			if (!d->entry)
				break;
		}

		memset(&st, 0, sizeof(st));
		st.st_ino = d->entry->d_ino;
		st.st_mode = d->entry->d_type << 12;
		nextoff = telldir(d->dp);
		if (filler(buf, d->entry->d_name, &st, nextoff))
			break;

		d->entry = NULL;
		d->offset = nextoff;
	}

	return 0;
}

int deffs_write(const char *path, const char *buf, size_t size,
			off_t offset, struct fuse_file_info *fi)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	// Copy path to non-constant copy for fopen
	char nonconst_path[strlen(path) + 1];
	strcpy(nonconst_path, path);

    // Open plaintext file
    FILE *plain_pointer;
    plain_pointer = fopen(nonconst_path, "r");
    if(plain_pointer == NULL){
        printf("Could not open file %s for encrypting, %s\n", nonconst_path, path);
		exit(1);
    }

    // Find size of file
    fseek(plain_pointer, 0, SEEK_END);
    long sz = ftell(plain_pointer);
    fseek(plain_pointer, 0, SEEK_SET);

    // Read all file contents into plaintext
    char *plaintext = malloc(sz + 1);
    fread(plaintext, 1, sz, plain_pointer);
    fclose(plain_pointer);

    plaintext[sz] = 0;

	// Encrypt plaintext
	EncryptionData *shard = get_encrypted_shards(plaintext);

	// Generate shard filepath
	char filename[38];
	random_string(filename, 32);
	strcat(filename, ".shard");

	char filepath[strlen(shardpoint) + strlen(filename) + 1];
	strcpy(filepath, shardpoint);
	strcat(filepath, filename);

	// Write encrypted data to shard
	FILE *shard_file;
	shard_file = fopen(filepath, "w");
	fprintf(shard_file, "%s", shard->ciphertext);
	fclose(shard_file);

	(void) path;
	res = pwrite(fi->fh, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
}

int deffs_write_buf(const char *path, struct fuse_bufvec *buf,
			off_t offset, struct fuse_file_info *fi)
{
	struct fuse_bufvec dst = FUSE_BUFVEC_INIT(fuse_buf_size(buf));

	path = deffs_path_prepend(path, storepoint);

	(void) path;

	dst.buf[0].flags = FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK;
	dst.buf[0].fd = fi->fh;
	dst.buf[0].pos = offset;

	return fuse_buf_copy(&dst, buf, FUSE_BUF_SPLICE_NONBLOCK);
}

int deffs_flush(const char *path, struct fuse_file_info *fi)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	(void) path;
	/* This is called from every close on an open file, so call the
	   close on the underlying filesystem.	But since flush may be
	   called multiple times for an open file, this must not really
	   close the file.  This is important if used on a network
	   filesystem like NFS which flush the data/metadata on close() */
	res = close(dup(fi->fh));
	if (res == -1)
		return -errno;

	return 0;
}

int deffs_truncate(const char *path, off_t size)
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	res = truncate(path, size);
	if (res == -1)
		return -errno;

	return 0;
}

int deffs_ftruncate(const char *path, off_t size,
			struct fuse_file_info *fi)
{
	int res;

	(void) path;

	res = ftruncate(fi->fh, size);
	if (res == -1)
		return -errno;

	return 0;
}

#ifdef HAVE_UTIMENSAT
int deffs_utimens(const char *path, const struct timespec ts[2])
{
	int res;

	path = deffs_path_prepend(path, storepoint);

	/* don't use utime/utimes since they follow symlinks */
	res = utimensat(0, path, ts, AT_SYMLINK_NOFOLLOW);
	if (res == -1)
		return -errno;

	return 0;
}
#endif

int deffs_release(const char *path, struct fuse_file_info *fi)
{
	path = deffs_path_prepend(path, storepoint);

	(void) path;
	close(fi->fh);

	return 0;
}

int deffs_releasedir(const char *path, struct fuse_file_info *fi)
{
   struct deffs_dirp *d = get_dirp(fi);
   path = deffs_path_prepend(path, storepoint);
   (void) path;
   closedir(d->dp);
   free(d);
   return 0;
}
