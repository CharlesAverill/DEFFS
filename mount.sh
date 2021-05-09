echo "Calling 'sudo umount' to reset mount state"
sudo umount -l /tmp/deffs_test
mkdir /tmp/deffs_test
sudo ./bin/DEFFS /tmp/deffs_test -o allow_other -o nonempty -d -s -f
