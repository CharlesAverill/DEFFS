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

int read_shard_data(FILE *header_pointer, size_t size, off_t offset, struct deffs_shard_data *sd)
{
    if (fread(sd->hash_buf, SHARD_HASH_LEN, 1, header_pointer) != 1) {
        fprintf(stderr, "Hash buf read error\n");
        return 1;
    }

    // Read chunk size from header file
    if (fread(&sd->chunk_size, sizeof(int), 1, header_pointer) != 1) {
        fprintf(stderr, "Chunk size read error");
        return 1;
    }

    // Determine size of entire buf
    int total_size = sd->chunk_size * n_machines;
    if ((offset != -1 && size != -1) && offset + size > total_size) {
        // This is a check used when modifying files
        // If not using read_shard_data during a rewrite, size and offset should be -1
        total_size += offset + size - total_size;
    }
    sd->total_size = total_size;

    return 0;
}

int combine_shards(struct deffs_shard_data *sd, char *combined, int size)
{
    // Recombine all shards
    // TODO: Not efficient, only need to rewrite the shard(s) where data is being modified
    combined[0] = '\0';
    for (int i = 0; i < n_machines; i++) {
        // Construct shard filepath from hash
        char shard_path[strlen(shardpoint) + SHARD_HASH_LEN +
                        SHARD_SUFF_LEN]; // Allocates a few extra characters, not a big deal
        strcpy(shard_path, shardpoint);
        strcat(shard_path, sd->hash_buf);
        sprintf(shard_path + strlen(shardpoint) + SHARD_HASH_LEN, "-%04d.shard", i);

        // Open shard file
        FILE *shard_pointer;
        shard_pointer = fopen(shard_path, "r");
        if (shard_pointer == NULL) {
            fprintf(stderr, "Error opening shard %s\n", shard_path);
            return 1;
        }

        // Read chunk from shard
        char temp_chunk[sd->chunk_size];
        if (fread(temp_chunk, sd->chunk_size, 1, shard_pointer) != 1) {
            fprintf(stderr, "Mismatched lengths while reading shard data for shard %s\n",
                    shard_path);
            return 1;
        }
        temp_chunk[sd->chunk_size] = '\0';

        printf("Chunk: %s\n", temp_chunk);

        // Close shard
        fclose(shard_pointer);

        // Append chunk to combined
        strcat(combined, temp_chunk);
    }
    combined[size] = '\0';

    return 0;
}

int deffs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    int fd;

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

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

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

    fd = open(nonconst_path, fi->flags);
    if (fd == -1)
        return -errno;

    fi->fh = fd;
    return 0;
}

int deffs_readlink(const char *path, char *buf, size_t size)
{
    int res;

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

    res = readlink(nonconst_path, buf, size - 1);
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

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

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

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

    if (S_ISFIFO(mode))
        res = mkfifo(nonconst_path, mode);
    else
        res = mknod(nonconst_path, mode, rdev);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_mkdir(const char *path, mode_t mode)
{
    int res;

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

    res = mkdir(nonconst_path, mode);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_unlink(const char *path)
{
    int res;

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

    // Open header file
    FILE *header_pointer;
    header_pointer = fopen(nonconst_path, "r+");
    if (header_pointer == NULL) {
        fprintf(stderr, "Could not open file %s for encrypting, %s\n", nonconst_path, path);
        return 1;
    }

    // Get header size
    fseek(header_pointer, 0, SEEK_END);
    int header_size = ftell(header_pointer);
    fseek(header_pointer, 0, SEEK_SET);

    // Read hash from header file
    char hash_buf[SHARD_HASH_LEN];
    fread(hash_buf, SHARD_HASH_LEN, 1, header_pointer);
    hash_buf[SHARD_HASH_LEN] = '\0';

    // Construct shard filepath from hash
    char shard_path[strlen(shardpoint) + SHARD_HASH_LEN + 7];
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

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

    res = rmdir(nonconst_path);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_symlink(const char *from, const char *to)
{
    int res;

    // Copy path to non-constant copy
    int path_len_from = strlen(from) + strlen(storepoint) + 1;
    char nonconst_from[path_len_from];
    get_full_path(from, nonconst_from, path_len_from);

    // Copy path to non-constant copy
    int path_len_to = strlen(to) + strlen(storepoint) + 1;
    char nonconst_to[path_len_to];
    get_full_path(to, nonconst_to, path_len_to);

    res = symlink(nonconst_from, nonconst_to);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_rename(const char *from, const char *to)
{
    int res;

    // Copy path to non-constant copy
    int path_len_from = strlen(from) + strlen(storepoint) + 1;
    char nonconst_from[path_len_from];
    get_full_path(from, nonconst_from, path_len_from);

    // Copy path to non-constant copy
    int path_len_to = strlen(to) + strlen(storepoint) + 1;
    char nonconst_to[path_len_to];
    get_full_path(to, nonconst_to, path_len_to);

    res = rename(nonconst_from, nonconst_to);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_link(const char *from, const char *to)
{
    int res;

    // Copy path to non-constant copy
    int path_len_from = strlen(from) + strlen(storepoint) + 1;
    char nonconst_from[path_len_from];
    get_full_path(from, nonconst_from, path_len_from);

    // Copy path to non-constant copy
    int path_len_to = strlen(to) + strlen(storepoint) + 1;
    char nonconst_to[path_len_to];
    get_full_path(to, nonconst_to, path_len_to);

    res = link(nonconst_from, nonconst_to);
    if (res == -1)
        return -errno;

    return 0;
}

int deffs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int res;

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

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
            return 1;
        }

        // Allocate and fill shard data
        struct deffs_shard_data *sd = malloc(sizeof(struct deffs_shard_data));
        read_shard_data(header_pointer, -1, -1, sd);

        // Recombine all shards
        printf("Allocating\n");
        char *combined = malloc(sd->total_size + 1);
        printf("Combining\n");
        combine_shards(sd, combined, sd->total_size + 1);
        printf("Combined: %s\n", combined);

        free(sd);

        strcpy(buf, combined);
        printf("After copy: %s\n", buf);

        res = size;
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

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

    // umask so that only the creator can rwx their shards
    umask(007);

    // Open header file
    FILE *header_pointer;
    header_pointer = fopen(nonconst_path, "r");
    if (header_pointer == NULL) {
        fprintf(stderr, "Could not open file %s for encrypting, %s\n", nonconst_path, path);
        return 1;
    }

    // Get header size
    fseek(header_pointer, 0, SEEK_END);
    int header_size = ftell(header_pointer);
    fseek(header_pointer, 0, SEEK_SET);

    // Open connection to a single client
    //connection *client_connection = cconnect("169.254.222.50", port, 5);

    if (FLAG_TRUNCATE != 0) { // Not empty
        // Allocate and fill shard data
        struct deffs_shard_data *sd = malloc(sizeof(struct deffs_shard_data));
        read_shard_data(header_pointer, size, offset, sd);

        // Recombine all shards
        char *combined = malloc(sd->total_size + 1);
        combine_shards(sd, combined, sd->total_size + 1);
        printf("Recombined: %s\n", combined);

        // Modify
        memcpy(combined + offset, buf, size);
        printf("Modified: %s\n", combined);

        // Split buf into chunks
        int new_chunk_size = (sd->total_size / n_machines) + 1;
        char *message_chunks[new_chunk_size];
        split_into_shards(combined, message_chunks, n_machines);

        for (int i = 0; i < n_machines; i++) {
            // Construct shard suffix string
            char *shard_suffix = malloc(SHARD_SUFF_LEN);
            sprintf(shard_suffix, "-%04d.shard", i);
            shard_suffix[SHARD_SUFF_LEN] = '\0';

            // Construct shard filepath from hash
            char shard_path[strlen(shardpoint) + SHARD_HASH_LEN +
                            SHARD_SUFF_LEN]; // Allocates a few extra characters, not a big deal
            strcpy(shard_path, shardpoint);
            strcat(shard_path, sd->hash_buf);
            strcat(shard_path, shard_suffix);
            free(shard_suffix);

            // Open shard file
            FILE *shard_pointer;
            shard_pointer = fopen(shard_path, "w");
            if (shard_pointer == NULL) {
                fprintf(stderr, "Error opening shard %s\n", shard_path);
                return 1;
            }

            printf("Message Chunk %d: %s\n", i, message_chunks[i]);
            int xn = fwrite(message_chunks[i], 1, new_chunk_size, shard_pointer);
            printf("Bytes written: %d\n", xn);

            fclose(shard_pointer);
        }

        // Write hash and chunk size to buf
        ftruncate(fi->fh, 0);
        pwrite(fi->fh, sd->hash_buf, SHARD_HASH_LEN, 0);
        pwrite(fi->fh, (const void *)&new_chunk_size, sizeof(int), SHARD_HASH_LEN);

        free(sd);

    } else if (FLAG_TRUNCATE < 1 && offset == 0) { // Empty
        // Generate hash
        unsigned char hash_buf[SHARD_HASH_LEN];
        get_sha256_hash(buf, hash_buf);

        // Split buf into chunks
        int chunk_size = (strlen(buf) / n_machines);
        char *message_chunks[chunk_size + 1];
        split_into_shards(buf, message_chunks, n_machines);

        for (int i = 0; i < n_machines; i++) {
            // Construct shard filepath from hash
            char shard_path[strlen(shardpoint) + SHARD_HASH_LEN +
                            SHARD_SUFF_LEN]; // Allocates a few extra characters, not a big deal
            strcpy(shard_path, shardpoint);
            strcat(shard_path, hash_buf);
            sprintf(shard_path + strlen(shardpoint) + SHARD_HASH_LEN, "-%04d.shard", i);

            // Open shard file
            FILE *shard_pointer;
            shard_pointer = fopen(shard_path, "w");
            if (shard_pointer == NULL) {
                fprintf(stderr, "Error opening shard %s\n", shard_path);
                return 1;
            }

            int xw = pwrite(fileno(shard_pointer), message_chunks[i], chunk_size, 0);

            fclose(shard_pointer);
        }

        // Write hash and chunk size to buf
        pwrite(fi->fh, hash_buf, SHARD_HASH_LEN, 0);
        pwrite(fi->fh, (const void *)&chunk_size, sizeof(int), SHARD_HASH_LEN);
    }

    FLAG_TRUNCATE = -1;

    fclose(header_pointer);
    //close_conn(client_connection);

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

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

    if (size == 0) {
        // Open header file
        FILE *header_pointer;
        header_pointer = fopen(nonconst_path, "r");
        if (header_pointer == NULL) {
            fprintf(stderr, "Could not open file %s for truncating, %s\n", nonconst_path, path);
            return 1;
        }

        // Read hash from header file
        char hash_buf[SHARD_HASH_LEN];
        fread(hash_buf, SHARD_HASH_LEN, 1, header_pointer);
        hash_buf[SHARD_HASH_LEN] = '\0';

        fclose(header_pointer);

        for (int i = 0; i < n_machines; i++) {
            // Construct shard filepath from hash
            char shard_path[strlen(shardpoint) + SHARD_HASH_LEN +
                            SHARD_SUFF_LEN]; // Allocates a few extra characters, not a big deal
            strcpy(shard_path, shardpoint);
            strcat(shard_path, hash_buf);
            sprintf(shard_path + strlen(shardpoint) + SHARD_HASH_LEN, "-%04d.shard", i);

            if (unlink(shard_path) == -1) {
                return -errno;
            }
        }
    } else {
        fprintf(stderr, "Trying to truncate %s to size %zu", nonconst_path, size);
        return 1;
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

    // Copy path to non-constant copy
    int path_len = strlen(path) + strlen(storepoint) + 1;
    char nonconst_path[path_len];
    get_full_path(path, nonconst_path, path_len);

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
