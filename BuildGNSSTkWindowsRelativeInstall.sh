# run in projects/gnsstk
cd build
cmake -DBUILD_EXT=true -DCMAKE_INSTALL_PREFIX=../install -G"Visual Studio 16 2019" -A x64 ../
cmake --build . --config release --target install
cd ..

