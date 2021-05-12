rm bin/DEFFS

clang-format style=file -i src/*
cmake --build ./ --target DEFFS -- -j 6
