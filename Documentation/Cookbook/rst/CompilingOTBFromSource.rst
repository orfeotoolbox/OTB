.. _compilingfromsource:

Compiling OTB from source
=========================

This section covers the compilation of OTB from source code using
`CMake <http://www.cmake.org>`_. If you just need to install OTB,
follow instructions from the :doc:`Installation` section.

OTB is known to work on:

* Visual Studio 2019 on Windows 10

* GCC 5 or higher, Clang 3.4 or higher on GNU/Linux

* AppleClang on macOS 10.8 or higher (intel, no ARM like the M1)

The C++14 standard is required since version 6.2.0.

OTB depends on a number of external libraries. Some are mandatory,
meaning that OTB cannot be compiled without them, while others (the
majority) are optional and can be activated or not during the build
process:

.. table:: External libraries used in OTB

    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | **Library**                                                      | **Mandatory**         | **Minimum version**        | **Superbuild Version**   |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `Boost <http://www.boost.org>`_                                  | Yes                   |                            | 1.72.0                   |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `Expat <https://sourceforge.net/projects/expat/>`_               | Yes                   |                            | 2.4.8                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `Geos <https://libgeos.org/>`_                                   | Yes                   |                            | 3.9.3 / 3.6.5 (windows)  |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `ITK <http://www.itk.org>`_                                      | Yes                   | 4.6.0                      | 4.13.3                   |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `GDAL <http://www.gdal.org>`_                                    | Yes                   | 2.4.1                      | 3.4.2 / 3.2.2 (windows)  |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `HDF4 <https://www.hdfgroup.org/solutions/hdf4/>`_               | Yes                   |                            | 4.2.13                   |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `HDF5 <https://www.hdfgroup.org/solutions/hdf5/>`_               | Yes                   |                            | 1.10.1                   |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libgeotiff <http://trac.osgeo.org/geotiff/>`_                   | Yes                   |                            | 1.7.1                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libjpeg-turbo <http://sourceforge.net/projects/libjpeg-turbo>`_ | Yes                   |                            | 1.5.3                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libpng <https://downloads.sourceforge.net/project/libpng>`_     | Yes                   |                            | 1.6.37                   |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libtiff <http://www.libtiff.org/>`_                             | Yes                   |                            | 4.4.0                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `NetCDF <https://github.com/Unidata/netcdf-c>`_                  | Yes                   |                            | 4.7.4                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `OpenJPEG <https://github.com/uclouvain/openjpeg>`_              | Yes                   |                            | 2.3.1                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `PROJ <https://proj.org/>`_                                      | Yes                   |                            | 8.2.0 / 6.2.1 (windows)  |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `SQLITE <https://www.sqlite.org>`_                               | Yes                   |                            | 3.36.0 / 3.8.8 (windows) |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `tinyXML <http://www.grinninglizard.com/tinyxml>`_               | Yes                   |                            | 2.6.2                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `ZLIB <https://zlib.net>`_                                       | Yes                   |                            | 1.2.12                   |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `6S <http://6s.ltdri.org>`_                                      | No                    |                            |                          |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `Curl <http://www.curl.haxx.se>`_                                | No                    |                            | 7.54.1                   |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `FFTW <http://www.fftw.org>`_                                    | No                    |                            | 3.3.10                   |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `libSVM <http://www.csie.ntu.edu.tw/~cjlin/libsvm>`_             | No                    | 2.0                        | 3.25                     |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `MPI <https://www.open-mpi.org/>`_                               | No                    |                            |                          |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `MuParser <http://www.muparser.sourceforge.net>`_                | No                    |                            | 2.3.2                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `MuParserX <http://muparserx.beltoforion.de>`_                   | No                    | 4.0.7                      | 4.0.8                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `OpenCV <http://opencv.org>`_                                    | No                    | 3.0.0                      | 4.6.0                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `Shark <http://image.diku.dk/shark/>`_                           | No                    | 4                          | 4.0                      |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `SiftFast <http://libsift.sourceforge.net>`_                     | No                    |                            |                          |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `SPTW <https://github.com/remicres/sptw.git>`_                   | No                    |                            |                          |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+
    | `SWIG <https://www.swig.org/>`_                                  | No                    |                            | 4.0.2                    |
    +------------------------------------------------------------------+-----------------------+----------------------------+--------------------------+

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
    apt install -y texlive-latex-base texlive-latex-recommended texlive-latex-extra texlive-fonts-recommended doxygen graphviz gnuplot dvipng python3-sphinx sphinx-rtd-theme-common
    pip install sphinx_rtd_theme


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

* **Normal build**: OTB dependencies must already be compiled and available on
  your system. This method requires more work but provides more flexibility.

If you do not know which method to use and just want to compile OTB with
all its modules, use the SuperBuild.

Important CMake configuration variables:

* ``CMAKE_INSTALL_PREFIX``: Installation directory, target for ``make install``
* ``BUILD_EXAMPLES``: Activate compilation of OTB examples
* ``BUILD_TESTING``: Activate compilation of the tests
* ``OTB_BUILD_DEFAULT_MODULES``: Activate all usual modules, required to build the examples
* ``OTB_USE_XXX``: Activate module *XXX*
* ``OTBGroup_XXX``: Enable modules in the group *XXX*
* ``OTB_DATA_ROOT``: otb-data repository
* ``OTB_WRAP_PYTHON``: Enable Python wrapper

SuperBuild only:

* ``DOWNLOAD_LOCATION``: Location to download dependencies
* ``USE_SYSTEM_XXX``: Use the system’s *XXX* library

SuperBuild: Build OTB and all dependencies
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

OTB’s compilation is customized by specifying configuration variables.
The most important configuration variables are shown in the
table above. The simplest way to provide
configuration variables is via the command line ``-D`` option:

::

    $ cd ~/OTB/build
    $ cmake -D CMAKE_INSTALL_PREFIX=~/OTB/install ../otb/SuperBuild

You can also set variables manually with ``cmake-gui`` or ``ccmake``.

Please note that the ``CMAKE_INSTALL_PREFIX`` variable is important
because the SuperBuild will install some targets during the compilation
step. Therefore this directory will be used even if you don’t use make
install target. In fact there is no *make install* target for the
SuperBuild. Also note that if not specified to cmake, a default install
dir will be used, located in ``../superbuild_install``.

By default, SuperBuild will not use any of libraries installed on
system. All ``USE_SYSTEM_XXX`` are set to `OFF`. This is our recommended
way of using SuperBuild. You are however free to use a system library if
you want! You must be very much aware of dependencies of those
libraries you use from system. For example, if libjpeg is not used from
superbuild then you should not use zlib from superbuild because zlib is
a dependency of libjpeg. Here SuperBuild will NOT set
``USE_SYSTEM_ZLIB=FALSE``. One must re-run cmake with
``-DUSE_SYSTEM_ZLIB=FALSE``. Above example of libjpeg-zlib dependency is
so simple. Imagine the case for GDAL which depends on zlib, libjpeg,
libtiff (with big tiff support), geotiff, sqlite, curl, geos, libkml,
openjpeg. This is one of the reasons we recommend to use SuperBuild
exclusively.

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

::

    $ cd ~/OTB/build
    $ make

Applications will be located in the ``CMAKE_INSTALL_PREFIX/bin/`` directory:

::

    ~/OTB/install/bin/otbcli_ExtractROI

will launch the command line version of the **ExtractROI** application,
while:

::

In order to ensure access to your OTB build from anywhere within your
system, we recommend setting the following environment variables.
First, add ``bin/`` directory to your PATH for easy access:

::

    export PATH=$PATH:~/OTB/install/bin

Second, add the ``lib/`` directory to your ``LD_LIBRARY_PATH``:

::

    export LD_LIBRARY_PATH=~/OTB/install/lib:$LD_LIBRARY_PATH

Normal build: Build only OTB
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Once all OTB dependencies are availables on your system, use CMake to
generate a Makefile:

::

    $ cd ~/OTB/build
    $ cmake -C configuration.cmake ../otb

The script ``configuration.cmake`` needs to contain dependencies
location if CMake cannot find them automatically. This can be done with
the ``XXX_DIR`` variables containing the directories which contain the
FindXXX.cmake scripts, or with the ``XXX_INCLUDEDIR`` and
``XXX_LIBRARY`` variables.

Additionally, decide which module you wish to enable, together with tests and
examples. Refer to table above for the list of CMake variables.

OTB is modular. It is possible to only build some modules
instead of the whole set. To deactivate a module (and the ones that
depend on it) switch off the CMake variable
``OTB_BUILD_DEFAULT_MODULES``, configure, and then switch off each
``Module_module_name`` variable.

Some of the OTB capabilities are considered as optional, and you can
deactivate the related modules thanks to a set of CMake variables
starting with ``OTB_USE_XXX``. The table below shows which modules
are associated to these variables. It is very important to notice that
these variable override the variable ``OTB_BUILD_DEFAULT_MODULES``.

You are now ready to compile OTB! Simply use the make command (other
targets can be generated with CMake’s ``-G`` option):

::

    $ make

The installation target will copy the binaries and libraries to the
installation location:

::

    $ make install

+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **CMake variable**        | **3rd party module**   | **Modules depending on it**                                                                                                                                               |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_CURL**        | OTBCurl                |                                                                                                                                                                           |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_MUPARSER**    | OTBMuParser            | OTBMathParser OTBDempsterShafer OTBAppClassification OTBAppMathParser OTBAppStereo OTBAppProjection OTBAppSegmentation OTBRoadExtraction OTBRCC8 OTBCCOBIA OTBMeanShift   |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_MUPARSERX**   | OTBMuParserX           | OTBMathParserX OTBAppMathParserX                                                                                                                                          |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_LIBSVM**      | OTBLibSVM              | optional for OTBSupervised OTBAppClassification                                                                                                                           |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_OPENCV**      | OTBOpenCV              | optional for OTBSupervised OTBAppClassification                                                                                                                           |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_SHARK**       | OTBShark               | optional for OTBSupervised OTBAppClassification                                                                                                                           |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_6S**          | OTB6S                  | OTBOpticalCalibration OTBAppOpticalCalibration OTBSimulation                                                                                                              |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_SIFTFAST**    | OTBSiftFast            |                                                                                                                                                                           |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

Table: Third parties and related modules.

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

