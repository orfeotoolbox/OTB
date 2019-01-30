Compiling OTB from source
=========================

This section covers the compilation of OTB from source code using `CMake
<http://www.cmake.org>`_. If you just need to install OTB and Monteverdi, follow
instructions from the :doc:`Installation` section.

OTB is known to work on:

* Visual Studio 2015 on Windows

* GCC 4.x, 5.x or Clang 3.x on GNU/Linux

* AppleClang on macOS (10.8 or higher)

The C++14 standard is required since version 6.2.0.

OTB depends on a number of external libraries. Some are mandatory,
meaning that OTB cannot be compiled without them, while others (the
majority) are optional and can be activated or not during the build
process:

.. table:: External libraries used in OTB
    :widths: 50 20 30
    :align: center

    +------------------------------------------------------------------+-----------------------+--------------------------+
    | **Library**                                                      | **Mandatory**         | **Minimum version**      |
    +==================================================================+=======================+==========================+
    | `ITK <http://www.itk.org>`_                                      | Yes                   | 4.6.0                    |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `GDAL <http://www.gdal.org>`_                                    | Yes                   | 2.0                      |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `OSSIM <http://www.ossim.org>`_                                  | Yes                   | 1.8.20-3                 |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `libgeotiff <http://trac.osgeo.org/geotiff/>`_                   | Yes                   |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `boost <http://www.boost.org>`_                                  | Yes                   |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `openthreads <http://www.openscenegraph.org>`_                   | Yes                   |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `tinyXML <http://www.grinninglizard.com/tinyxml>`_               | Yes                   |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `6S <http://6s.ltdri.org>`_                                      | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `Curl <http://www.curl.haxx.se>`_                                | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `FFTW <http://www.fftw.org>`_                                    | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `GLEW <http://glew.sourceforge.net/>`_                           | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `GLFW <http://www.glfw.org/>`_                                   | No                    | 3                        |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `GLUT <https://www.opengl.org/resources/libraries/glut/>`_       | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `libKML <https://github.com/google/libkml>`_                     | No                    | 1.2                      |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `libSVM <http://www.csie.ntu.edu.tw/~cjlin/libsvm>`_             | No                    | 2.0                      |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `MPI <https://www.open-mpi.org/>`_                               | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `MuParser <http://www.muparser.sourceforge.net>`_                | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `MuParserX <http://muparserx.beltoforion.de>`_                   | No                    | 4.0.7                    |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `OpenCV <http://opencv.org>`_                                    | No                    | 2 (3.x also supported)   |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `OPENGL <https://www.opengl.org/>`_                              | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `Qt <https://www.qt.io/developers/>`_                            | No                    | 5                        |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `QWT <http://qwt.sourceforge.net>`_                              | No                    | 6                        |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `Shark <http://image.diku.dk/shark/>`_                           | No                    | 3.1                      |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `SiftFast <http://libsift.sourceforge.net>`_                     | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+
    | `SPTW <https://github.com/remicres/sptw.git>`_                   | No                    |                          |
    +------------------------------------------------------------------+-----------------------+--------------------------+

GNU/Linux and macOS
-------------------

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
table above. The simplest way to provide
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

**Notes about Qt:** Unlike other dependencies, building Qt5 on all platforms is
not a trivial task but OTB SuperBuild does its best to facilitate this for the
user. So there is still some additional package installation, one has to do as a
pre-requistie for SuperBuild On a GNU/Linux you must have Qt X11 dependencies
installed. See `Qt 5 documentation
<https://doc.qt.io/qt-5/linux-requirements.html>`_ for the list of packages that
need to be installed before starting SuperBuild.

For example for a Debian 8.1 system, all Qt5 dependencies can be installed with the
following ’apt-get install’ command:

::

    apt-get install libx11-dev libxext-dev libxt-dev libxi-dev libxrandr-dev libgl-dev libglu-dev libxinerama-dev libxcursor-dev

You can also deactivate Qt5 and skip this by passing
``-DOTB_USE_QT=OFF`` to CMake, but this will install OTB without
Monteverdi, Mapla and the GUI application launchers.

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

    ~/OTB/install/bin/otbgui_ExtractROI

will launch the graphical version.

In order to ensure access to your OTB build from anywhere within your
system, we recommend setting the following environment variables.
First, add ``bin/`` directory to your PATH for easy access:

::

    export PATH=$PATH:~/OTB/install/bin

Second, add the ``lib/`` directory to your ``LD_LIBRARY_PATH``:

::

    export LD_LIBRARY_PATH=~/OTB/install/lib:$LD_LIBRARY_PATH

Monteverdi is part of OTB module and is compiled by the SuperBuild if GLEW, GLUT, OPENGL, Qt and QWT
modules are activated.

To use OTB applications from within Monteverdi you will need to define
the ``OTB_APPLICATION_PATH`` environment variable:

::

    export OTB_APPLICATION_PATH=~/OTB/install/lib/otb/applications
    monteverdi

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
examples. Refer to table above for the list of CMake variables.

OTB is modular. It is possible to only build some modules
instead of the whole set. To deactivate a module (and the ones that
depend on it) switch off the CMake variable
``OTB_BUILD_DEFAULT_MODULES``, configure, and then switch off each
``Module_module_name`` variable.

Some of the OTB capabilities are considered as optional, and you can
deactivate the related modules thanks to a set of CMake variables
starting with ``OTB_USE_XXX``. The table below shows which modules
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
+===========================+========================+===========================================================================================================================================================================+
| **OTB\_USE\_LIBKML**      | OTBlibkml              | OTBKMZWriter OTBIOKML OTBAppKMZ                                                                                                                                           |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_QT**          | OTBQt                  | OTBQtWidget                                                                                                                                                               |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_QWT**         | OTBQwt                 | OTBMonteverdiGUI OTBMonteverdi                                                                                                                                            |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_GLEW**        | OTBGlew                | OTBIce OTBMonteverdiGUI OTBMonteverdi                                                                                                                                     |
+---------------------------+------------------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **OTB\_USE\_OPENGL**      | OTBOpenGL              | OTBIce OTBMonteverdiGUI OTBMonteverdi                                                                                                                                     |
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

Windows
-------

Everything that is needed for OTB development on Windows, including
compiling from source, is covered in details on the OTB wiki at:

http://wiki.orfeo-toolbox.org/index.php/OTB_development_on_Windows

Known issues
------------

Please check `our gitlab tracker <https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues?label_name%5B%5D=bug>`_ for a list of open bugs.
