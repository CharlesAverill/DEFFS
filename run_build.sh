rm builds/DEFFS

cmake --build ./ --target DEFFS -- -j 6

./builds/DEFFS -d -s -f ./builds/testmount
