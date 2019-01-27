#!/bin/bash

export TOP_DIR=$(pwd)

curl -L -o otb-data-master.tar.gz \
     https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-data/-/archive/master/otb-data-master.tar.gz
tar xzf otb-data-master.tar.gz
mv otb-data-master otb-data
rm -f otb-data-master.tar.gz

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX:PATH=/usr/local \
      -DCMAKE_C_FLAGS:STRING=-fPIC -Wall -Wextra \
      -DCMAKE_CXX_FLAGS:STRING=-fPIC -Wall -Wextra -Wno-cpp \
      -DBUILD_SHARED_LIBS:BOOL=ON \
      -DBUILD_TESTING:BOOL=ON \
      -DBUILD_EXAMPLES:BOOL=ON \
      -DOTB_USE_CURL:BOOL=ON \
      -DOTB_USE_GLEW:BOOL=ON \
      -DOTB_USE_GLFW:BOOL=ON \
      -DOTB_USE_GLUT:BOOL=ON \
      -DOTB_USE_LIBKML:BOOL=ON \
      -DOTB_USE_LIBSVM:BOOL=ON \
      -DOTB_USE_MPI:BOOL=OFF \
      -DOTB_USE_MUPARSER:BOOL=ON \
      -DOTB_USE_MUPARSERX:BOOL=ON \
      -DOTB_USE_OPENCV:BOOL=ON \
      -DOTB_USE_OPENGL:BOOL=ON \
      -DOTB_USE_OPENMP:BOOL=ON \
      -DOTB_USE_QT:BOOL=ON \
      -DOTB_USE_QWT:BOOL=ON \
      -DOTB_USE_SHARK:BOOL=ON \
      -DOTB_USE_SPTW:BOOL=OFF \
      -DOTB_DATA_ROOT:PATH=${TOP_DIR}/otb-data \
      -DOTB_DATA_USE_LARGEINPUT:BOOL=OFF \
      -DOTB_WRAP_PYTHON:BOOL=ON \
      -DOTB_WRAP_PYTHON3:BOOL=ON \
      -Dopencv_INCLUDE_DIR:PATH=/usr/include \
      ..

make -j $(grep -c processor /proc/cpuinfo 2>/dev/null || echo 8)

xvfb-run -a -n 1 -s "-screen 0 1024x768x24 -dpi 96" ctest -T test
