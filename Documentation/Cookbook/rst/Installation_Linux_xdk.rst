Synopsis
--------

This archive provides a development kit for the ORFEO ToolBox library which
allows to easily compile OTB and facilitate the creation of programs based on
OTB.

It contains shared libraries of OTB dependencies and helper scripts for the
configuration and the compilation of the library.

Note that OTB sources are not included in the archive (see instructions below).

Prerequisites
-------------

-  cmake (>= 3.2)
-  gcc (>= 4.9)
-  git

Extract XDK
-----------

::

    cd /opt (You can use another a directory of your own choice)
    chmod +x OTB-|release|-xdk-Linux64.run
    ./OTB-|release|-xdk-Linux64.run

**You now will have a directory named OTB-|release|-xdk-Linux64 in \'/opt\'**

Setup environment
-----------------
There is a helper script in package called **xdkenv.profile**. This script export
required environments to configure and build OTB.

::

    cd /opt/OTB-|release|-xdk-Linux64
    . ./xdkenv.profile

Test environment setup by checking value of CMAKE_PREFIX_PATH

::

    echo $CMAKE_PREFIX_PATH

The above command will output:
 /opt/OTB-|release|-xdk-Linux64
 

Download, Configure and build OTB
---------------------------------

::

    mkdir -p /opt/OTB/build && cd /opt/OTB
    git clone --depth=1 --branch=develop https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb.git source
    cd build && cmake ../source 
    make -j2


Notes
~~~~~

* By default, all OTB\_USE\_\* variables in OTB are set to FALSE except for 6S and SiftFast. You can check/activate them in your cmake-gui/ccmake.

* If you need to activate Qt4 modules (Monteverdi, Ice, QtWidget, ApplicationEngine ..), you must set *QT_BINARY_DIR*.

::

    cd /opt/OTB/build && cmake -DOTB_USE_QT4=TRUE -DQT_BINARY_DIR=$CMAKE_PREFIX_PATH/bin

This is because Qt tools ( moc, rcc, et al) are reported as not found by cmake due to incorrect path in "qmake -query"


