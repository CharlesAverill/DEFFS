/*
* FILENAME: rw.c
*
* DESCRIPTION: FUSE callbacks relating to read, write, open, and close operations for files and folders
*
* AUTHOR: Charles Averill
*/

#include "rw.h"

int FLAG_OPENED_EMPTY_FILE;
int FLAG_TRUNCATE;

// TODO: Replace all exit calls with error returns

int deffs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    int fd;

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    fd = open(nonconst_path, fi->flags, mode);
    if (fd == -1)
        return -errno;

    fi->fh = fd;

    FLAG_OPENED_EMPTY_FILE = 1;

    return 0;
}

int deffs_open(const char *path, struct fuse_file_info *fi)
{
    int fd;

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    fd = open(nonconst_path, fi->flags);
    if (fd == -1)
        return -errno;

    fi->fh = fd;
    return 0;
}

int deffs_readlink(const char *path, char *buf, size_t size)
{
    int res;

    res = readlink(path, buf, size - 1);
    if (res == -1)
        return -errno;

    buf[res] = '\0';
    return 0;
}

int deffs_opendir(const char *path, struct fuse_file_info *fi)
{
    int res;
    struct deffs_dirp *d = malloc(sizeof(struct deffs_dirp));
    if (d == NULL)
        return -ENOMEM;

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    d->dp = opendir(nonconst_path);
    if (d->dp == NULL) {
        res = -errno;
        free(d);
        return res;
    }
    d->offset = 0;
    d->entry  = NULL;

    fi->fh = (unsigned long)d;
    return 0;
}

int deffs_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res;

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    if (S_ISFIFO(mode))
        res = mkfifo(path, mode);
    else
        res = mknod(path, mode, rdev);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_mkdir(const char *path, mode_t mode)
{
    int res;

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    res = mkdir(nonconst_path, mode);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_unlink(const char *path)
{
    int res;

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    // Open header file
    FILE *header_pointer;
    header_pointer = fopen(nonconst_path, "r+");
    if (header_pointer == NULL) {
        fprintf(stderr, "Could not open file %s for encrypting, %s\n", nonconst_path, path);
        exit(1);
    }

    // Get header size
    fseek(header_pointer, 0, SEEK_END);
    int header_size = ftell(header_pointer);
    fseek(header_pointer, 0, SEEK_SET);

    // Read hash from header file
    char hash_buf[SHARD_FN_LEN];
    fread(hash_buf, SHARD_FN_LEN, 1, header_pointer);
    hash_buf[SHARD_FN_LEN] = '\0';

    // Construct shard filepath from hash
    char shard_path[strlen(shardpoint) + SHARD_FN_LEN + 7];
    strcpy(shard_path, shardpoint);
    strcat(shard_path, hash_buf);
    strcat(shard_path, ".shard");

    // Close header and unlink header and shard
    fclose(header_pointer);
    res = unlink(nonconst_path) + unlink(shard_path);
    if (res < 0)
        return -errno;

    return 0;
}

int deffs_rmdir(const char *path)
{
    int res;

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    res = rmdir(path);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_symlink(const char *from, const char *to)
{
    int res;

    // Copy from to non-constant copy for fopen
    char nonconst_from[strlen(storepoint) + strlen(from) + 1];
    strcpy(nonconst_from, nonconst_from);
    strcpy(nonconst_from, deffs_path_prepend(nonconst_from, storepoint));

    // Copy to to non-constant copy for fopen
    char nonconst_to[strlen(storepoint) + strlen(to) + 1];
    strcpy(nonconst_to, to);
    strcpy(nonconst_to, deffs_path_prepend(nonconst_to, storepoint));

    res = symlink(nonconst_from, nonconst_to);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_rename(const char *from, const char *to)
{
    int res;

    // Copy from to non-constant copy for fopen
    char nonconst_from[strlen(storepoint) + strlen(from) + 1];
    strcpy(nonconst_from, nonconst_from);
    strcpy(nonconst_from, deffs_path_prepend(nonconst_from, storepoint));

    // Copy to to non-constant copy for fopen
    char nonconst_to[strlen(storepoint) + strlen(to) + 1];
    strcpy(nonconst_to, to);
    strcpy(nonconst_to, deffs_path_prepend(nonconst_to, storepoint));

    res = rename(nonconst_from, nonconst_to);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_link(const char *from, const char *to)
{
    int res;

    // Copy from to non-constant copy for fopen
    char nonconst_from[strlen(storepoint) + strlen(from) + 1];
    strcpy(nonconst_from, nonconst_from);
    strcpy(nonconst_from, deffs_path_prepend(nonconst_from, storepoint));

    // Copy to to non-constant copy for fopen
    char nonconst_to[strlen(storepoint) + strlen(to) + 1];
    strcpy(nonconst_to, to);
    strcpy(nonconst_to, deffs_path_prepend(nonconst_to, storepoint));

    res = link(nonconst_from, nonconst_to);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int res;

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    if (starts_with(nonconst_path, shardpoint) == 1 && ends_with(nonconst_path, ".shard") == 1) {
        res = pread(fi->fh, buf, size, offset);
        if (res == -1)
            res = -errno;
    } else {
        // Open header file
        FILE *header_pointer;
        header_pointer = fopen(nonconst_path, "r");
        if (header_pointer == NULL) {
            fprintf(stderr, "Could not open file %s for decrypting\n", nonconst_path);
            exit(1);
        }

        // Get header size
        fseek(header_pointer, 0, SEEK_END);
        int header_size = ftell(header_pointer);
        fseek(header_pointer, 0, SEEK_SET);
        if (header_size <= 0) {
            fprintf(stderr, "Cannot trace corresponding file shard for file %s", nonconst_path);
            exit(1);
        }

        // Read hash from header file
        char hash_buf[SHARD_FN_LEN + 1];
        fread(hash_buf, SHARD_FN_LEN, 1, header_pointer);
        hash_buf[SHARD_FN_LEN] = '\0';

        fclose(header_pointer);

        // Construct shard filepath from hash
        char shard_path[strlen(shardpoint) + SHARD_FN_LEN + 7];
        strcpy(shard_path, shardpoint);
        strcat(shard_path, hash_buf);
        strcat(shard_path, ".shard");

        // Open shard file
        FILE *shard_pointer;
        shard_pointer = fopen(shard_path, "r");

        res = pread(fileno(shard_pointer), buf, size, offset);

        /*
        // Get shard size
        fseek(shard_pointer, 0, SEEK_END);
        int shard_size = ftell(shard_pointer);
        fseek(shard_pointer, 0, SEEK_SET);
        
        // Read shard metadata
        char key[SHARD_KEY_LEN];
        fread(key, SHARD_KEY_LEN, 1, shard_pointer);

        // Read shard data
        char ciphertext_buf[shard_size - SHARD_KEY_LEN];
        fread(ciphertext_buf, shard_size - SHARD_KEY_LEN, 1, shard_pointer);

        fclose(shard_pointer);

        fprintf(stderr, "Ciphertext: %s\nKey: %s\n", ciphertext_buf, key);

        // Decrypt shard
        struct EncryptionData *plain = get_plaintext(ciphertext_buf, key);

        // Only read the requested data
        char *sub_buf = malloc(size);
        strncpy(sub_buf, plain->plaintext + offset, size);

        strcpy(buf, sub_buf);

        res = strlen(buf);
        */
    }

    return res;
}

int deffs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset,
                  struct fuse_file_info *fi)
{
    struct deffs_dirp *d = get_dirp(fi);

    (void)path;
    if (offset != d->offset) {
        seekdir(d->dp, offset);
        d->entry  = NULL;
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
        st.st_ino  = d->entry->d_ino;
        st.st_mode = d->entry->d_type << 12;
        nextoff    = telldir(d->dp);
        if (filler(buf, d->entry->d_name, &st, nextoff))
            break;

        d->entry  = NULL;
        d->offset = nextoff;
    }

    return 0;
}

int deffs_write(const char *path, const char *buf, size_t size, off_t offset,
                struct fuse_file_info *fi)
{
    int res;

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    // umask so that only the creator can rwx their shards
    umask(007);

    // Open header file
    FILE *header_pointer;
    header_pointer = fopen(nonconst_path, "a+");
    if (header_pointer == NULL) {
        fprintf(stderr, "Could not open file %s for encrypting, %s\n", nonconst_path, path);
        exit(1);
    }

    // Get header size
    fseek(header_pointer, 0, SEEK_END);
    int header_size = ftell(header_pointer);
    fseek(header_pointer, 0, SEEK_SET);

    if (header_size > 0) { // Not empty
        // Read hash from header file
        char hash_buf[SHARD_FN_LEN];
        fread(hash_buf, SHARD_FN_LEN, 1, header_pointer);
        hash_buf[SHARD_FN_LEN] = '\0';

        fclose(header_pointer);

        // Construct shard filepath from hash
        char shard_path[strlen(shardpoint) + SHARD_FN_LEN + 7];
        strcpy(shard_path, shardpoint);
        strcat(shard_path, hash_buf);
        strcat(shard_path, ".shard");

        // Open shard file
        FILE *shard_pointer;
        shard_pointer = fopen(shard_path, "a+");

        pwrite(fileno(shard_pointer), buf, size, offset);

        fclose(shard_pointer);
    } else { // Empty
        fclose(header_pointer);

        // Generate hash
        unsigned char hash_buf[SHARD_FN_LEN];
        get_sha256_hash(buf, hash_buf);

        // Construct shard filepath from hash
        char shard_path[strlen(shardpoint) + SHARD_FN_LEN + 7];
        strcpy(shard_path, shardpoint);
        strcat(shard_path, hash_buf);
        strcat(shard_path, ".shard");

        // Open shard for writing
        FILE *shard_pointer;
        shard_pointer = fopen(shard_path, "w");

        fwrite(buf, 1, size, shard_pointer);

        // Write hash to buf
        pwrite(fi->fh, hash_buf, strlen(hash_buf), 0);

        fclose(shard_pointer);
    }

    return size;
}

int deffs_write_buf(const char *path, struct fuse_bufvec *buf, off_t offset,
                    struct fuse_file_info *fi)
{
    struct fuse_bufvec dst = FUSE_BUFVEC_INIT(fuse_buf_size(buf));

    (void)path;

    dst.buf[0].flags = FUSE_BUF_IS_FD | FUSE_BUF_FD_SEEK;
    dst.buf[0].fd    = fi->fh;
    dst.buf[0].pos   = offset;

    return fuse_buf_copy(&dst, buf, FUSE_BUF_SPLICE_NONBLOCK);
}

int deffs_flush(const char *path, struct fuse_file_info *fi)
{
    int res;

    (void)path;
    /* This is called from every close on an open file, so call the
       close on the underlying filesystem.    But since flush may be
       called multiple times for an open file, this must not really
       close the file.  This is important if used on a network
       filesystem like NFS which flush the data/metadata on close() */
    res = close(dup(fi->fh));
    if (res == -1)
        return -errno;

    return res;
}

int deffs_truncate(const char *path, off_t size)
{
    int res;

    FLAG_TRUNCATE = size;

    printf("TRUNCATING\n");

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    if (size == 0) {
        // Open header file
        FILE *header_pointer;
        header_pointer = fopen(nonconst_path, "r+");
        if (header_pointer == NULL) {
            fprintf(stderr, "Could not open file %s for truncating, %s\n", nonconst_path, path);
            exit(1);
        }

        // Read hash from header file
        char hash_buf[SHARD_FN_LEN];
        fread(hash_buf, SHARD_FN_LEN, 1, header_pointer);
        hash_buf[SHARD_FN_LEN] = '\0';

        // Construct shard filepath from hash
        char shard_path[strlen(shardpoint) + SHARD_FN_LEN + 7];
        strcpy(shard_path, shardpoint);
        strcat(shard_path, hash_buf);
        strcat(shard_path, ".shard");

        res = unlink(shard_path);
        if (res == -1)
            return -errno;
    } else {
        fprintf(stderr, "Trying to truncate %s to size %zu", nonconst_path, size);
        exit(1);
    }

    return 0;
}

int deffs_ftruncate(const char *path, off_t size, struct fuse_file_info *fi)
{
    int res;
    (void)path;

    res = ftruncate(fi->fh, size);
    if (res == -1)
        return -errno;

    return 0;
}

#ifdef HAVE_UTIMENSAT
int deffs_utimens(const char *path, const struct timespec ts[2])
{
    int res;
    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    /* don't use utime/utimes since they follow symlinks */
    res = utimensat(0, nonconst_path, ts, AT_SYMLINK_NOFOLLOW);
    if (res == -1)
        return -errno;

    return 0;
}
#endif

int deffs_release(const char *path, struct fuse_file_info *fi)
{
    (void)path;
    close(fi->fh);

    return 0;
}

int deffs_releasedir(const char *path, struct fuse_file_info *fi)
{
    struct deffs_dirp *d = get_dirp(fi);
    (void)path;
    closedir(d->dp);
    free(d);
    return 0;
}
