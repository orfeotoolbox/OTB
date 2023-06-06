In order to run the OTB applications, you have to make sure you installed some necessary dependencies on you system if you plan to use the python interface
or the graphical version of the apps

Installation of system dependencies
+++++++++++++++++++++++++++++++++++

OTB 8 has both command line and graphical interface, the latter needs some system dependencies in order to run properly.
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
  apt install -y --no-install-recommends libgl1 libxcb1 '^libxcb.*' libx11-xcb1 libglu1-mesa libxrender1 libxi6 libxkbcommon0 libxkbcommon-x11-0 libxinerama1

RedHat8 / Fedora 37+
++++++++++++++++++++

.. code-block:: bash

  # Required packages to extract OTB from the archive    
  dnf update && dnf install -y python38 python38-devel python38-numpy

  # Required tools to recompile the python bindings
  dnf install gcc gcc-c++ cmake3 swig make which

  # Required tools to run OTB apps with graphical interface
  dnf install -y mesa-libGL mesa-libGLU libXcursor libXi libXinerama libXrandr libxcb-devel libxkbcommon-devel libxkbcommon-x11-devel xcb-util-devel xcb-util-image-devel xcb-util-keysyms-devel xcb-util-renderutil-devel xcb-util-wm-devel
