#echo "Calling 'sudo umount' to reset mount state"
#sudo umount -l /tmp/deffs_test
mkdir /tmp/deffs_test
sudo ./bin/DEFFS -d -s -f /tmp/deffs_test -o allow_other
