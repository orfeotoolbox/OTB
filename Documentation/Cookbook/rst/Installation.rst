Installation
============

We provide different standalone binary packages for OTB-Applications:

-  for 64 bits Linux distribution

-  for Windows platform (10 or higher)

Other binaries can be available as packages (OSGeo packages,
Debian/Ubuntu packages), however be advised that they
may not be up-to-date nor delivered with full features. If you want to
build from source or if we don’t provide packages for your system,
information is available in the :ref:`compilingfromsource` section.

You can get latest binary packages from our `Download page <https://www.orfeo-toolbox.org/download>`__.

Linux
-----

.. include:: Installation_Linux.rst

Windows
-------

.. include:: Installation_Windows.rst

Docker image
------------

A Docker image is available on `DockerHub <https://hub.docker.com/r/orfeotoolbox/otb>`__. It
can be used on any device running Docker, and provides python 3.8. 

**It is the only way to run OTB for MacOS since the Intel architecture is no more in the Mac ecosystem.**

::

    docker pull orfeotoolbox/otb

To run CLI tools, you can launch the docker image with:

::
   
    docker run -it orfeotoolbox/otb

If you are planning on using the GUI applications, you have to call
docker with the following arguments:

::
   
    docker run -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix/:/tmp/.X11-unix/ --device=/dev/dri:/dev/dri orfeotoolbox/otb monteverdi


Conda package
-------------

A conda package is available for Orfeo Toolbox on Linux. It can be found at https://anaconda.org/orfeotoolbox/otb 
and can be installed with the command. 

Note : it is a 7.1 version that is old. The conda package is hard to maintain as the conda base evolves often
which breaks the build sometimes. 

::

    conda install -c orfeotoolbox otb 


Other packages
--------------

Warning! These packages may not be up-to-date with latest OTB
releases. In addition, some features of the library may not be available
on every platform. Some of these are not maintained by the OTB-team. If you
want to get involved in the packaging of OTB for your favourite
platform, please contact us through the developer’s mailing list:
otb-developers@googlegroups.com.

Debian and Ubuntu
~~~~~~~~~~~~~~~~~

There are OTB packages for Debian (unstable) since version 5.2.0. OTB
Applications packages may be available as Debian packages through APT
repositories:

-  otb-bin for command line applications

-  python-otb for python applications

Due to license issues, the OTB package built in Debian doesn’t contain
6S. As a consequence, the package does not contain the
OpticalCalibration application.

**This is why it is recommended to use the official binary package.**

Archlinux
~~~~~~~~~~

Package is in AUR (Arch User Repository).
You will then need to run (if you use yaourt, you could use trizen instead):

::

    yaourt -S orfeo-toolbox

