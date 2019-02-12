FROM ubuntu:18.04 as shark-provider
MAINTAINER OrfeoToolbox Core Team

ENV DEBIAN_FRONTEND noninteractive

# ----------------------------------------------------------------------------
# First stage : install tools 
# (based on https://github.com/Shark-ML/Shark/blob/master/.travis.yml)
# ----------------------------------------------------------------------------

RUN apt-get update -y \
  && apt-get upgrade -y \
  && apt-get install -y --no-install-recommends \
        ca-certificates \
        g++ \
        make \
        cmake \
        libboost-date-time-dev \
        libboost-filesystem-dev \
        libboost-graph-dev \
        libboost-program-options-dev \
        libboost-system-dev \
        libboost-thread-dev \
        libcurl4-gnutls-dev \
        curl \
  && rm -rf /var/lib/apt/lists/*

RUN cd \tmp \
  && curl -o shark.tar.gz https://codeload.github.com/Shark-ML/Shark/tar.gz/v3.1.4 \
  && tar xzf shark.tar.gz \
  && cd Shark-3.1.4\
  && mkdir build \
  && cd build \
  && cmake  -DBUILD_EXAMPLES:BOOL=OFF \
            -DBUILD_TESTING:BOOL=OFF \
            -DENABLE_HDF5:BOOL=OFF \
            -DBUILD_SHARED_LIBS=ON \
            -DENABLE_CBLAS:BOOL=OFF \
            -DENABLE_OPENMP:BOOL=OFF \
            ../. \
  && make install

FROM ubuntu:18.04
MAINTAINER Sebastien Dinot <sebastien.dinot@c-s.fr>

ENV DEBIAN_FRONTEND noninteractive

RUN echo "Europe/Paris" > /etc/timezone

# ----------------------------------------------------------------------------
# First stage : install tools (they rarely evolve)
# ----------------------------------------------------------------------------
RUN apt-get update -y \
 && apt-get upgrade -y \
 && apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        make \
        cmake \
        ninja-build \
        g++ \
        gcc \
        clang \
        clang-tidy \
        clang-format \
        ccache \
        git \
        libtool \
        swig \
        xvfb \
 && rm -rf /var/lib/apt/lists/*

# ----------------------------------------------------------------------------
# Second stage : dependencies (they evolve more often)
# ----------------------------------------------------------------------------

RUN apt-get update -y \
 && apt-get upgrade -y \
 && apt-get install -y --no-install-recommends \
        freeglut3-dev \
        libboost-date-time-dev \
        libboost-filesystem-dev \
        libboost-graph-dev \
        libboost-program-options-dev \
        libboost-system-dev \
        libboost-thread-dev \
        libcurl4-gnutls-dev \
        libexpat1-dev \
        libfftw3-dev \
        libgdal-dev \
        libgeotiff-dev \
        libglew-dev \
        libglfw3-dev \
        libgsl-dev \
        libinsighttoolkit4-dev \
        libkml-dev \
        libmuparser-dev \
        libmuparserx-dev \
        libopencv-core-dev \
        libopencv-ml-dev \
        libopenmpi-dev \
        libopenthreads-dev \
        libossim-dev \
        libpng-dev \
        libqt5opengl5-dev \
        libqwt-qt5-dev \
        libsvm-dev \
        libtinyxml-dev \
        qtbase5-dev \
        qttools5-dev \
        default-jdk \
        python-dev \
        python-numpy \
        python-gdal \
        python3-dev \
        python3-numpy \
        python3-gdal \
 && rm -rf /var/lib/apt/lists/*

COPY --from=shark-provider /usr/local/include/shark/* /usr/include/shark/
COPY --from=shark-provider /usr/local/lib/libshark* /usr/lib/
COPY --from=shark-provider /usr/local/bin/SharkVersion /usr/bin/
COPY --from=shark-provider /usr/local/lib/cmake/ /usr/lib/cmake/

ENV PATH /usr/lib/ccache:$PATH
ENV OTB_USE_SHARK ON
