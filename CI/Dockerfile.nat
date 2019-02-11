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

ENV PATH /usr/lib/ccache:$PATH
ENV OTB_USE_SHARK OFF
