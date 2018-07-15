mkdir build/lib

cd build/lib

cmake \
  -D CMAKE_INSTALL_PREFIX=/home/student/.local \
  ../../simple-project/library

make

cmake -D COMPONENT=developer -P cmake_install.cmake

mkdir ../bin
cd ../bin

cmake ../../simple-project/application

make