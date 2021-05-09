# DEFFS
A custom FUSE-based filesystem that distributes shards of data across machines
on a local network, allowing those files to be accessible from any machine.

# Installation
- Install `libfuse-dev`:

`sudo apt-get install libfuse-dev`

- Build and mount DEFFS

```bash
mkdir builds
cmake --build ./ --target DEFFS -- -j 6

./builds/DEFFS -d -s -f ./builds/testmount
```
