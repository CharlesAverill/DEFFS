# DEFFS
A custom FUSE-based filesystem that distributes shards of data across machines
on a local network, allowing those files to be accessible from any machine.

# Installation
- Install FUSE with `sudo apt-get install libfuse-dev`

- Uncomment `user_allow_other` from the last line of `/etc/fuse.conf`

- Build and mount DEFFS

```bash
mkdir bin
cmake --build ./ --target DEFFS -- -j 6

mkdir deffs_test
./builds/DEFFS -d -s -f /tmp/deffs_test -o allow_other

# Filesystem now mounted at /tmp/deffs_test
```
