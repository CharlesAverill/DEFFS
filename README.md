# DEFFS
A custom FUSE-based filesystem that distributes shards of data across machines
on a local network, allowing those files to be accessible from any machine.

# Installation
- Install FUSE with `sudo apt-get install libfuse-dev`

- Uncomment `user_allow_other` from the last line of `/etc/fuse.conf`

- Build

```bash
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

# Progress
- [ ] Distributed
- [ ] Encrypted
- [ ] Fractured
- [x] File System
