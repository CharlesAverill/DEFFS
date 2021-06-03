rm bin/DEFFS

clang-format style=file -i src/* src/net/*
cmake --build ./ --target DEFFS -- -j 6
