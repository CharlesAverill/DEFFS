# DEFFS
A custom FUSE-based filesystem that distributes encrypted shards of data across
machines on a local network, allowing those files to be accessible from any
machine.

## Installation
- Install dependencies - `sudo apt-get install libfuse-dev libssl-dev`

- Uncomment `user_allow_other` from the last line of `/etc/fuse.conf`

- Build

```bash
cmake .
mkdir bin
cmake --build ./ --target DEFFS -- -j 6
```

- Mount

```bash
mkdir ~/deffs_mountpoint
mkdir ~/deffs_storepoint

./bin/DEFFS ~/deffs_mountpoint ~/deffs_storepoint

# DEFFS now mounted at ~/deffs_mountpoint
# Files are interacted with within the mountpoint, but are stored at the storepoint
# It is not advised to modify any files in your storepoint
```

## Usage
Right now, DEFFS can be used like a typical EXT4 filesystem. It will store
encrypted data shards in `<storepoint>/.shards/`, so modifying files in that
folder or the folder itself will break DEFFS.

This filesystem's encryption is based on the AES_encrypt method, meaning:
- It is NOT (yet) resistant to attackers
- Making files with unencrypted content longer than 15 characters is undefined (soon to be fixed)

Currently, DEFFS only encrypts files when the `write` syscall is called. Soon,
`write_buf` will be supported as well. Files are decrypted upon `read`.

Proper usage of DEFFS looks like this:

```bash
cd mountpoint

mkdir test
touch hello
echo "hello world" > test/helloworld.txt

cat test/helloworld.txt
```

## Progress
- [ ] Distributed
- [x] Encrypted
- [ ] Fractured
- [x] File System
