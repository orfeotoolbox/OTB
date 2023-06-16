In order to run the OTB applications, you have to make sure you installed some necessary dependencies on your system if you plan to use the Python interface
or the graphical version of the apps

System dependencies to run the standalone package
`````````````````````````````````````````````````

OTB 8 has both command line and graphical interface, the latter needs some system dependencies to run properly.
Here are provided provide examples of package installations on popular distributions, using package managers to install the required dependencies.

The following commands can be executed separatly, depending on your needs

Ubuntu 20.04 and 22.04
++++++++++++++++++++++

.. code-block:: bash

  # Required packages to extract OTB from the archive
  apt update && apt install -y --no-install-recommends file python3 python3-dev python3-numpy

  # Required tools to recompile the python bindings
  apt install -y --no-install-recommends g++ swig cmake make

  # Required tools to run OTB apps with graphical interface
  apt install -y --no-install-recommends libgl-dev libgl1 libxcb1 '^libxcb.*' libx11-xcb1 libglu1-mesa libxrender1 libxi6 libxkbcommon0 libxkbcommon-x11-0 libxinerama1

RedHat 8 / Fedora 37+
+++++++++++++++++++++

.. code-block:: bash

  # Required packages to extract OTB from the archive    
  dnf update && dnf install -y python38 python38-devel python38-numpy

  # Required tools to recompile the python bindings
  dnf install gcc gcc-c++ cmake3 swig make which

  # Required tools to run OTB apps with graphical interface
  dnf install -y mesa-libGL mesa-libGL-devel mesa-libGLU libXcursor libXi libXinerama libXrandr libxcb-devel libxkbcommon-devel libxkbcommon-x11-devel xcb-util-devel xcb-util-image-devel xcb-util-keysyms-devel xcb-util-renderutil-devel xcb-util-wm-devel


System dependencies to run the docker image
```````````````````````````````````````````

In order to run the docker image, you only need docker installed on your machine.
The docker image is known to work on MacOS (Intel and Silicon), any Linux, and Windows in the WSL Linux Environment

Please follow this tutorial to install docker depending on your OS : https://docs.docker.com/engine/install/

System dependencies to build from source
`````````````````````````````````````````

You have two choices to build from source, they are detailed in the "Compile OTB from source" section.

  - **Native build**: you need to install the OTB dependencies packaged in your OS.
  - **Superbuild build**: you need to install the packages required to build all the OTB dependencies

Common dependencies
~~~~~~~~~~~~~~~~~~~~

Ubuntu 20.04 and 22.04
++++++++++++++++++++++

  .. code-block:: bash

    apt update -y && apt install -y --no-install-recommends ca-certificates curl make cmake g++ gcc git git-lfs libtool swig python3 python3-dev python3-pip python3-numpy pkg-config patch

    # Additional dependencies if you need to build the documentation
    apt install -y texlive-latex-base texlive-latex-recommended texlive-latex-extra texlive-fonts-recommended doxygen graphviz gnuplot dvipng python3-sphinx sphinx-rtd-theme-common
    pip install sphinx_rtd_theme

RedHat 8 / Fedora 37+
+++++++++++++++++++++

  .. code-block:: bash

    dnf -y update && dnf install -y git git-lfs cmake3 which procps swig gcc gcc-c++ make glibc-static patch patchelf pcre-devel python38 python38-devel python38-numpy python38-pip openssl-devel perl-devel zlib-devel curl-devel
    
    # Additional dependencies if you need to build the documentation
    dnf install -y texlive texlive-dvipng doxygen graphviz gnuplot python38-sphinx
    pip install sphinx_rtd_theme

Dependencies for a native build
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ubuntu 20.04 and 22.04
++++++++++++++++++++++

  .. code-block:: bash

    # Install mandatory dependencies
    apt install -y --no-install-recommends libboost-filesystem-dev libboost-serialization-dev libboost-system-dev libboost-thread-dev libcurl4-gnutls-dev libgdal-dev python3-gdal libexpat1-dev libfftw3-dev libgeotiff-dev libgsl-dev libinsighttoolkit4-dev libgeotiff-dev libpng-dev libtinyxml-dev
    
    # Install optional dependencies
    apt install -y --no-install-recommends libmuparser-dev libmuparserx-dev libkml-dev libopencv-core-dev libopencv-ml-dev libopenmpi-dev libsvm-dev

    # Install graphical dependencies
    apt install -y --no-install-recommends libegl1-mesa-dev libglvnd-dev libglu1-mesa-dev libglx-dev libgles2-mesa-dev libglew-dev libglfw3-dev freeglut3-dev qtbase5-dev qttools5-dev libqt5opengl5-dev libqwt-qt5-dev
         

RedHat 8 / Fedora 37+
+++++++++++++++++++++

  .. code-block:: bash

    # Install mandatory dependencies
    dnf install -y boost-devel gdal-devel python3-gdal expat-devel libgeotiff-devel fftw-devel InsightToolkit-devel gsl-devel libpng-devel tinyxml-devel

    # Install optional dependencies (for muparserX, you have to manually add a third party repository)
    dnf install -y muParser-devel opencv-devel libsvm-devel libkml-devel

    # Install graphical dependencies
    dnf install -y mesa-libGL mesa-libGLU libXcursor libXi libXinerama libXrandr libpnglibjpeg libxcb libxkbcommon libxkbcommon-x11 xcb-util xcb-util-image xcb-util-keysyms xcb-util-renderutil xcb-util-wm

Dependencies for a SuperBuild build
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Ubuntu 20.04 and 22.04
++++++++++++++++++++++

  .. code-block:: bash

    # Install graphical dependencies
    apt install '^libxcb.*-dev' libglvnd-dev libglu1-mesa-dev libegl1-mesa-dev mesa-utils libgles2-mesa-dev libperl-dev libwayland-dev libxi-dev libxrandr-dev libxinerama-dev libxcursor-dev libxkbcommon-x11-dev libxkbcommon-dev libxcb-xinerama0-dev libx11-xcb-dev

RedHat 8 / Fedora 37+
+++++++++++++++++++++

  .. code-block:: bash

    dnf -y install mesa-libGL-devel mesa-libGLU-devel libXcursor-devel libXi-devel libXinerama-devel libXrandr-devel libpng-devel	libjpeg-devel libxcb-devel libxkbcommon-devel libxkbcommon-x11-devel xcb-util-devel xcb-util-image-devel xcb-util-keysyms-devel xcb-util-renderutil-devel xcb-util-wm-devel