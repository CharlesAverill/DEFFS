echo "Calling 'sudo umount' to reset mount state"
sudo umount -l ~/deffs_mountpoint

rm -rf ~/deffs_storepoint
rm -rf ~/deffs_mountpoint

mkdir ~/deffs_mountpoint
mkdir ~/deffs_storepoint

./bin/DEFFS ~/deffs_mountpoint ~/deffs_storepoint -n 2
