.. _compilingfromsource:

Compiling OTB from source
=========================

This section covers the compilation of OTB from source code using
`CMake <http://www.cmake.org>`_. If you just need to install OTB,
follow instructions from the :doc:`Installation` section.

OTB is known to work on:

* Visual Studio 2019 on Windows 10

* GCC 5 or higher, Clang 3.4 or higher on GNU/Linux

The C++14 standard is required since version 6.2.0.

OTB depends on a number of external libraries. Some are mandatory,
meaning that OTB cannot be compiled without them, while others (the
majority) are optional and can be activated or not during the build
process:

.. table:: External libraries used in OTB

    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | **Library**                                                       | **Mandatory**         | **Minimum version**        | **Superbuild Version**   |
    +===================================================================+=======================+============================+==========================+
    | `Boost <http://www.boost.org>`_                                   | Yes                   | 1.73.0                     | 1.82.0                   |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `Expat <https://sourceforge.net/projects/expat/>`_                | Yes                   |                            | 2.5.0                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `Geos <https://libgeos.org/>`_                                    | Yes                   |                            | 3.12.1                   |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `ITK <http://www.itk.org>`_                                       | Yes                   | 4.6.0                      | 4.13.3                   |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `GDAL <http://www.gdal.org>`_                                     | Yes                   | 2.4.1                      | 3.8.3                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `HDF4 <https://www.hdfgroup.org/solutions/hdf4/>`_                | Yes                   |                            | 4.2.13                   |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `HDF5 <https://www.hdfgroup.org/solutions/hdf5/>`_                | Yes                   |                            | 1.12.3                   |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libgeotiff <http://trac.osgeo.org/geotiff/>`_                    | Yes                   |                            | 1.7.1                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libjpeg-turbo <https://github.com/libjpeg-turbo/libjpeg-turbo>`_ | Yes                   |                            | 3.0.3                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libpng <https://downloads.sourceforge.net/project/libpng>`_      | Yes                   |                            | 1.6.37                   |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libtiff <http://www.libtiff.org/>`_                              | Yes                   |                            | 4.6.0                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `NetCDF <https://github.com/Unidata/netcdf-c>`_                   | Yes                   |                            | 4.9.2                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `OpenJPEG <https://github.com/uclouvain/openjpeg>`_               | Yes                   |                            | 2.5.0                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `PROJ <https://proj.org/>`_                                       | Yes                   |                            | 9.3.1                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `SQLITE <https://www.sqlite.org>`_                                | Yes                   |                            | 3.42.0                   |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `tinyXML <http://www.grinninglizard.com/tinyxml>`_                | Yes                   |                            | 2.6.2                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `ZLIB <https://zlib.net>`_                                        | Yes                   |                            | 1.3.1                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `6S <http://6s.ltdri.org>`_                                       | No                    |                            |                          |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `Curl <http://www.curl.haxx.se>`_                                 | No                    |                            | 8.2.1                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `FFTW <http://www.fftw.org>`_                                     | No                    |                            | 3.3.10                   |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libSVM <http://www.csie.ntu.edu.tw/~cjlin/libsvm>`_              | No                    | 2.0                        | 3.25                     |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `MPI <https://www.open-mpi.org/>`_                                | No                    |                            |                          |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `MuParser <http://www.muparser.sourceforge.net>`_                 | No                    |                            | 2.3.2                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `MuParserX <http://muparserx.beltoforion.de>`_                    | No                    | 4.0.7                      | 4.0.8                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `OpenCV <http://opencv.org>`_                                     | No                    | 3.0.0                      | 4.6.0                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `Shark <http://image.diku.dk/shark/>`_                            | No                    | 4                          | 4.0                      |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `SiftFast <http://libsift.sourceforge.net>`_                      | No                    |                            |                          |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `SPTW <https://github.com/remicres/sptw.git>`_                    | No                    |                            |                          |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `SWIG <https://www.swig.org/>`_                                   | No                    |                            | 4.1.1                    |
    +-------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+

GNU/Linux
---------

System dependencies to build from source
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You have two choices to build from source :

  - **Native build**: you need to install the OTB dependencies packaged in your OS.
  - **Superbuild build**: you need to install the packages required to build all the OTB dependencies

Common dependencies
+++++++++++++++++++

Debian / Ubuntu
~~~~~~~~~~~~~~~

  .. code-block:: bash

    apt update -y && apt install -y --no-install-recommends ca-certificates curl make cmake g++ gcc git git-lfs libtool swig python3 python3-dev python3-pip python3-numpy pkg-config patch

    # Additional dependencies if you need to build the documentation
    apt install -y texlive-latex-base texlive-latex-recommended texlive-latex-extra texlive-fonts-recommended doxygen graphviz gnuplot dvipng python3-sphinx sphinx-rtd-theme-common python3-sphinx-rtd-theme


Additional dependencies for a native build
++++++++++++++++++++++++++++++++++++++++++

Debian / Ubuntu
~~~~~~~~~~~~~~~

  .. code-block:: bash

    # Install mandatory dependencies
    apt install -y --no-install-recommends libboost-filesystem-dev libboost-serialization-dev libboost-system-dev libboost-thread-dev libcurl4-gnutls-dev libgdal-dev python3-gdal libexpat1-dev libfftw3-dev libgeotiff-dev libgsl-dev libinsighttoolkit4-dev libgeotiff-dev libpng-dev libtinyxml-dev

    # Install optional dependencies
    apt install -y --no-install-recommends libmuparser-dev libmuparserx-dev libkml-dev libopencv-core-dev libopencv-ml-dev libopenmpi-dev libsvm-dev


Setting up the build environment
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The first thing to do is to create a directory for working with OTB.
This guide will use ``~/OTB`` but you are free to choose something
else. In this directory, there will be three locations:

*  ``~/OTB/otb`` for the source git repository

*  ``~/OTB/build`` for the intermediate build objects, CMake specific
   files, libraries and binaries.

*  ``~/OTB/install``, the installation directory for OTB once it is
   built. A system location (``/usr/local`` for example) can also be
   used, but installing locally is more flexible and does not require
   root access.

::

    $ mkdir ~/OTB
    $ cd ~/OTB
    $ git clone https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb.git
    $ mkdir build
    $ mkdir install

The OTB project uses a git branching model where ``develop`` is the current
development version. It contains the latest patches and represents the work in
progress towards the next release. ``master`` is the latest stable release.

Checkout the branch you want to build now:

::

    $ cd ~/OTB/otb
    $ git checkout develop

Now, there are two ways of compiling OTB from source, depending on how you want
to manage dependencies. Both methods rely on CMake.

* **SuperBuild**: All OTB dependencies are automatically downloaded and
  compiled.  This method is the easiest to use and provides a complete OTB with
  minimal effort.

* **Native build**: OTB dependencies must already be installed/compiled on
  your system. This method requires more work but provides more flexibility.

If you do not know which method to use and just want to compile OTB with
all its modules, use the SuperBuild.

Important CMake configuration variables:

* ``CMAKE_INSTALL_PREFIX``: Installation directory, target for ``make install``
* ``BUILD_EXAMPLES``: Activate compilation of OTB examples
* ``BUILD_TESTING``: Activate compilation of the tests
* ``OTB_USE_XXX``: Activate dependency *XXX* such as MUPARSER, OPENCV...
* ``OTB_BUILD_ModuleName``: Enable building of optional modules (SAR,FeaturesExtraction...) used in the superbuild
* ``OTBGroup_XXX``: Enable modules in the group *XXX* used in a native build
* ``OTB_DATA_ROOT``: otb-data repository
* ``OTB_WRAP_PYTHON``: Enable Python wrapper
* ``XDK_INSTALL_PATH``: You can choose to build the dependencies in another folder than the otb install prefix. In order to package the dependencies you build, you have to position that variable where you installed the dependencies

SuperBuild only:

* ``DOWNLOAD_LOCATION``: Location to download dependencies

**IMPORTANT NOTE**

By default the Toolbox is only building the CORE modules, if you want to build the whole toolbox you have to activate the ``OTB_BUILD_ModuleName`` options for each additional module you want to build

SuperBuild: build all OTB dependencies before building the toolbox against these dependencies
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

OTB’s compilation is customized by specifying configuration variables.
The most important configuration variables are shown in the
table above. The simplest way to provide
configuration variables is via the command line ``-D`` option:

::

    $ cd ~/OTB/build
    # this example command will build the dependencies only for the CORE module of OTB
    # see below for building the dependencies for the whole toolbox
    $ cmake -DCMAKE_INSTALL_PREFIX=~/OTB/install -DXDK_INSTALL_PATH=~/OTB/install ../otb/SuperBuild

You can also set variables manually with ``cmake-gui`` or ``ccmake``.

Please note that the ``CMAKE_INSTALL_PREFIX`` variable is important
because the SuperBuild will install some targets during the compilation
step. Therefore this directory will be used even if you don’t use make
install target. In fact there is no *make install* target for the
SuperBuild. Also note that if not specified to cmake, a default install
dir will be used, located in ``../superbuild_install``.

All dependencies are configured and built in a way that help us to get
an efficient OTB build. So we enable geotiff (with proj4 support),
openjpeg, geos in GDAL build.

SuperBuild downloads dependencies into the ``DOWNLOAD_LOCATION`` directory,
which will be ``~/OTB/build/Downloads`` in our example.  Dependencies can be
downloaded manually into this directory before the compilation step. This can be
useful if you wish to bypass a proxy, intend to compile OTB without an internet
connection, or other network constraints. You can find an archive with sources
of all our dependencies on `the Orfeo ToolBox website
<https://www.orfeo-toolbox.org/packages>`_ (pick the ’SuperBuild-archives’
corresponding to the OTB version you want to build).

You are now ready to compile OTB! Simply use the make command (other
targets can be generated with CMake’s ``-G`` option):

You have to choice for building : have the dependencies installed in another folder than otb or install everything in the same folder.
The first method is recommended for clarity.

Build the dependencies in another folder than otb install path
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    $ mkdir ~/OTB/buildxdk && cd ~/OTB/buildxdk
    # here use the OTB_BUILD var that will take care to build all dependencies needed for them
    $ cmake ../otb/Superbuild -DCMAKE_INSTALL_PREFIX=$PWD/../xdk -DOTB_BUILD_FeaturesExtraction=ON -DOTB_BUILD_Hyperspectral=ON -DOTB_BUILD_Learning=ON -DOTB_BUILD_Miscellaneous=ON -DOTB_BUILD_SAR=ON -DOTB_BUILD_Segmentation=ON -DOTB_BUILD_StereoProcessing=ON
    $ make OTB_DEPENDS
    # now build OTB
    $ cd .. && mkdir otb_build && cd otb_build
    $ cmake ../otb -DXDK_INSTALL_PATH=/Path/To/xdk -DCMAKE_PREFIX_PATH=~/Workspace/xdk -DCMAKE_INSTALL_PREFIX=~/OTB/install
    $ make -j8

Build the dependencies in the same folder as otb install
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    $ mkdir ~/OTB/build && cd ~/OTB/build
    $ cmake ../otb/SuperBuild -DCMAKE_INSTALL_PREFIX=~/OTB/install -DOTB_BUILD_FeaturesExtraction=ON -DOTB_BUILD_Hyperspectral=ON -DOTB_BUILD_Learning=ON -DOTB_BUILD_Miscellaneous=ON -DOTB_BUILD_SAR=ON -DOTB_BUILD_Segmentation=ON -DOTB_BUILD_StereoProcessing=ON
    $ make -j8

Applications will be located in the ``CMAKE_INSTALL_PREFIX/bin/`` directory:

::

    ~/OTB/install/bin/otbcli_ExtractROI

will launch the command line version of the **ExtractROI** application,
while:

In order to ensure access to your OTB build from anywhere within your
system, we recommend calling `source ~/OTB/install/otbenv.profile`

Native build with system dependencies
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once all OTB dependencies are available on your system, use CMake to
generate a Makefile:

::

    $ cd ~/OTB/build
    # if you want to build only the Core module use this command
    $ cmake ../otb -DCMAKE_INSTALL_PREFIX=/Path/To/OTB_install
    # if you want to build the whole toolbox not only the Core of OTB, use this command instead
    $ cmake ../otb -DCMAKE_INSTALL_PREFIX=/Path/To/OTB_install -DOTB_BUILD_FeaturesExtraction=ON -DOTB_BUILD_Hyperspectral=ON -DOTB_BUILD_Learning=ON -DOTB_BUILD_Miscellaneous=ON -DOTB_BUILD_SAR=ON -DOTB_BUILD_Segmentation=ON -DOTB_BUILD_StereoProcessing=ON

Additionally, decide which module you wish to enable, together with tests and
examples. Refer to table above for the list of CMake variables.

OTB is modular. It is possible to only build some modules
instead of the whole set. To activate an optional module (and the ones that
depend on it) you can pass ``OTB_BUILD_ModuleName`` to cmake.
The activation or deactivation of these variables will automatically switch ON or OFF
the variable ``OTB_USE_XXX``.

You are now ready to compile OTB! Simply use the make command (other
targets can be generated with CMake’s ``-G`` option):

::

    $ make -j8

The installation target will copy the binaries and libraries to the
installation location:

::

    $ make install

+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **CMake variable**        | **3rd party module**   | **Modules depending on it**                                                                                                                                               |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_CURL**        | OTBCurl                |                                                                                                                                                                           |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_MUPARSER**    | OTBMuParser            | FeaturesExtraction                                                                                                                                                        |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_MUPARSERX**   | OTBMuParserX           | FeaturesExtraction                                                                                                                                                        |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_LIBSVM**      | OTBLibSVM              | Learning                                                                                                                                                                  |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_OPENCV**      | OTBOpenCV              | Learning                                                                                                                                                                  |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_SHARK**       | OTBShark               | Learning                                                                                                                                                                  |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_6S**          | OTB6S                  | OTBOpticalCalibration OTBAppOpticalCalibration OTBSimulation                                                                                                              |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_SIFTFAST**    | OTBSiftFast            |                                                                                                                                                                           |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

Table: Third parties and related modules.

Packaging
---------

Before OTB 9, the packaging was done using makeself which delivers a .run self extractable file. The main problem of this method was the huge number of
steps required to have a single package (a successive call to 12 cmake files). In OTB9 we decided to do the packaging with CPack which is included in CMake, making it very simple to package OTB.
The packaging is done via the "install" routines in the CMake Code.
The file describing the packaging is Package_OTB.cmake that you can find in the CMake folder.

To make the packages for OTB, you should simply call :

::

    $ cd ~/OTB/build
    $ make package

By default the generated package contains all the modules and will be delivered in the subfolder build_packages.
If you want to package OTB by module, you can set the variable ``CPACK_ARCHIVE_COMPONENT_INSTALL`` to *ON* :

::

    $ cd ~/OTB/build
    $ cmake . -DCPACK_ARCHIVE_COMPONENT_INSTALL=ON
    $ make package

Known issues
------------

Please check `our gitlab tracker <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues?label_name%5B%5D=bug>`_ for a list of open bugs.

Tests
-----

There are more than 2100 tests for OTB. It can take from 20 minutes to 3
hours to run them all, depending on compilation options
(release mode does make a difference) and hardware.

To run the tests, first make sure to set the option
``BUILD_TESTING`` to ``ON`` before building the library. If you want to run the tests for the
python API, you will also need to install the python module `pytest`.

For some of the tests, you also need the test data and the baselines (~1GB). These files are stored
using `git-lfs` in the `Data` folder at the root of otb sources. To download them, you have to make
sure `git-lfs` is installed before cloning otb (binaries for `git lfs` are available for different
OS `here <https://github.com/git-lfs/git-lfs/releases>`_).

After downloading, add the binary to $PATH and run `git lfs install`. You can then clone otb sources :

::

    git clone https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb.git

Once OTB is built with the tests, you just have to go to the binary
directory where you built OTB and run ``ctest -N`` to have a list of all
the tests. Just using ``ctest`` will run all the tests. To select a
subset, you can do ``ctest -R Kml`` to run all tests related to kml
files or ``ctest -I 1,10`` to run tests from 1 to 10.

Compiling documentation
-----------------------

Enable Python bindings and set ``BUILD_COOKBOOK`` option to ON:

::

    cmake -DOTB_WRAP_PYTHON=ON -DBUILD_COOKBOOK=ON .

Then, build the target:

::

    make CookbookHTML

