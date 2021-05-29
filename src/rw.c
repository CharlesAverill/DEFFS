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
        printf("Could not open file %s for encrypting, %s\n", nonconst_path, path);
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
            printf("Could not open file %s for decrypting\n", nonconst_path);
            exit(1);
        }

        // Get header size
        fseek(header_pointer, 0, SEEK_END);
        int header_size = ftell(header_pointer);
        fseek(header_pointer, 0, SEEK_SET);
        if (header_size <= 0) {
            printf("Cannot trace corresponding file shard for file %s", nonconst_path);
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
        shard_pointer = fopen(shard_path, "rw");

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

        // Decrypt shard
        struct EncryptionData *plain = get_plaintext(ciphertext_buf, key);

        // Only read the requested data
        char *sub_buf = malloc(size);
        strncpy(sub_buf, plain->plaintext + offset, size);
        strcpy(buf, sub_buf);

        res = strlen(buf);
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
    header_pointer = fopen(nonconst_path, "r+");
    if (header_pointer == NULL) {
        printf("Could not open file %s for encrypting, %s\n", nonconst_path, path);
        exit(1);
    }

    // Get header size
    fseek(header_pointer, 0, SEEK_END);
    int header_size = ftell(header_pointer);
    fseek(header_pointer, 0, SEEK_SET);

    if (FLAG_OPENED_EMPTY_FILE == 0) { // Not empty
        // Read hash from header file
        char hash_buf[SHARD_FN_LEN];
        fread(hash_buf, SHARD_FN_LEN, 1, header_pointer);

        // Construct shard filepath from hash
        char shard_path[strlen(shardpoint) + SHARD_FN_LEN + 7];
        strcpy(shard_path, shardpoint);
        strcat(shard_path, hash_buf);
        strcat(shard_path, ".shard");

        // Open shard file
        FILE *shard_pointer;
        shard_pointer = fopen(shard_path, "r+");

        // Get shard size
        fseek(shard_pointer, 0, SEEK_END);
        int shard_size = ftell(shard_pointer);
        fseek(shard_pointer, 0, SEEK_SET);

        // Read shard metadata
        char key_buf[SHARD_KEY_LEN];
        fread(key_buf, SHARD_KEY_LEN, 1, shard_pointer);
        key_buf[SHARD_KEY_LEN] = '\0';

        // Read shard data
        char ciphertext_buf[shard_size - SHARD_KEY_LEN - 1];
        fread(ciphertext_buf, shard_size - SHARD_KEY_LEN, 1, shard_pointer);
        ciphertext_buf[shard_size - SHARD_KEY_LEN - 1] = '\0';

        // Erase shard data
        fseek(shard_pointer, 0, SEEK_SET);
        fflush(shard_pointer);
        ftruncate(fileno(shard_pointer), 0);

        // Decrypt shard
        char *plaintext_buf          = malloc(strlen(ciphertext_buf));
        struct EncryptionData *plain = get_plaintext(ciphertext_buf, key_buf);
        strcpy(plaintext_buf, plain->plaintext);

        // Modify data
        if (offset == 0) {
            plaintext_buf = buf;
        } else {
            if (offset + strlen(buf) > strlen(plaintext_buf)) {
                int len       = offset + strlen(buf) - strlen(plaintext_buf);
                plaintext_buf = (char *)realloc(plaintext_buf, offset + strlen(buf));
            }
            for (int i = offset; i < offset + strlen(buf); i++) {
                plaintext_buf[i] = buf[i - offset];
            }
        }

        // Re-encrypt modified data
        struct EncryptionData *reencrypted = get_ciphertext(plaintext_buf);

        fseek(shard_pointer, 0, SEEK_END);
        shard_size = ftell(shard_pointer);
        fseek(shard_pointer, 0, SEEK_SET);

        // Write metadata
        int n = fwrite(reencrypted->key, 1, sizeof(reencrypted->key), shard_pointer);
        if (n != SHARD_KEY_LEN) {
            printf("Error writing key metadata, %d bytes written, %d expected\n", n, SHARD_KEY_LEN);
            exit(1);
        }

        // Write encrypted data to shard
        int ciphertext_len = 16 * ((strlen(plaintext_buf) + 15) / 16) + 1;
        n                  = fwrite(reencrypted->ciphertext, 1, ciphertext_len, shard_pointer);

        fclose(shard_pointer);
    } else { // Empty
        // Encrypt buffer
        struct EncryptionData *encrypted = get_ciphertext(strdup(buf));

        // Generate hash
        unsigned char *hash_buf = malloc(65);
        get_sha256_hash(encrypted->ciphertext, hash_buf);

        // Construct shard filepath from hash
        char shard_path[strlen(shardpoint) + SHARD_FN_LEN + 7];
        strcpy(shard_path, shardpoint);
        strcat(shard_path, hash_buf);
        strcat(shard_path, ".shard");

        // Open shard for writing
        FILE *shard_pointer;
        shard_pointer = fopen(shard_path, "w");

        // Write metadata
        int n = fwrite(encrypted->key, 1, sizeof(encrypted->key), shard_pointer);
        if (n != SHARD_KEY_LEN) {
            printf("Error writing key metadata\n");
            exit(1);
        }

        // Write encrypted data to shard
        fprintf(shard_pointer, "%s", encrypted->ciphertext);

        fclose(shard_pointer);

        // Write hash to buf
        pwrite(fi->fh, hash_buf, strlen(hash_buf), 0);
    }

    if (res == -1)
        return -errno;

    FLAG_OPENED_EMPTY_FILE = 0;
    FLAG_TRUNCATE          = -1;

    fclose(header_pointer);

    // FUSE expects res to be the number of characters written, but DEFFS
    // writes the ciphertext for the entire file. So just fudge the numbers.
    return strlen(buf);
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

    return size;
    /*

    // Copy path to non-constant copy for fopen
    char nonconst_path[strlen(storepoint) + strlen(path) + 1];
    strcpy(nonconst_path, path);
    strcpy(nonconst_path, deffs_path_prepend(nonconst_path, storepoint));

    if(size == 0){
        // Open header file
        FILE *header_pointer;
        header_pointer = fopen(nonconst_path, "rw");
        if(header_pointer == NULL){
            printf("Could not open file %s for truncating, %s\n", nonconst_path, path);
            exit(1);
        }

        // Read hash from header file
        char hash_buf[SHARD_FN_LEN + 1];
        fread(hash_buf, SHARD_FN_LEN, 1, header_pointer);
        hash_buf[SHARD_FN_LEN] = '\0';

        // Construct shard filepath from hash
        char shard_path[strlen(shardpoint) + SHARD_FN_LEN + 7];
        strcpy(shard_path, shardpoint);
        strcat(shard_path, hash_buf);
        strcat(shard_path, ".shard");

        res = truncate(shard_path, size);
        if (res == -1)
            return -errno;
    } else {
        printf("Trying to truncate %s to size %zu", nonconst_path, size);
        exit(1);
    }

    return 0;
    */
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
