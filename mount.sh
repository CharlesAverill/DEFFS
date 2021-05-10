echo "Calling 'sudo umount' to reset mount state"
sudo umount -l ~/deffs_mountpoint

echo "Calling 'sudo rm' to remove storepoint"
sudo rm -rf ~/deffs_storepoint
rm -rf ~/deffs_mountpoint

mkdir ~/deffs_mountpoint
mkdir ~/deffs_storepoint

sudo ./bin/DEFFS ~/deffs_mountpoint ~/deffs_storepoint
