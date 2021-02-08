Remote Modules
===============

What is a Remote Module
-----------------------

Remote modules allow anyone to **extend the functionalities of OTB** without being part of the core project repository. 
They can have different licenses than the main OTB repository.
Those modules are just like regular modules, except they are not distributed inside OTB source code. Under some conditions (dependencies, official acceptance
process, etc.), we are also able to distribute your remote module in the official standalone binaries.


List of available modules
--------------------------

**Official OTB modules**

"Official" remote modules are just regular modules that we consider particularly useful for users. Some of those are shipped in the binary packages so you don't need to build them yourself. 

  - **DiapOTB** : Potential events analyze by highlighting differences between SAR images

        + Repository: https://gitlab.orfeo-toolbox.org/remote_modules/diapotb
        + License: Apache License 2.0
        + Description: The differential SAR interferometry (DInSAR) technique relies on the processing of two SAR images of the same portion of the Earth’s surface taken at different time. The aim is to analyze potential events (earthquake, destruction, …) by highlighting differences between SAR images. DInSAR involves a set of tools such as creation of deformation grids , coregistration or building of interferograms. The Orfeo Toolbox remote module DiapOTB contains all necessary steps and allows to launch a complete DInSAR chain. This module has been used with Sentinel-1 data with satisfactory results. This module is a port from DIAPASON tool, integrated in the `ESA GeoHazards TEP <http://terradue.github.io/doc-tep-geohazards/tutorials/diapason-sm.html>`__.

  - **otb-mosaic** : Image mosaiking

        + Repository : https://github.com/remicres/otb-mosaic
        + Author: Rémi Cresson
        + License: CeCILL-B
        + Description: This module provides one application dedicated to images mosaicking with several compositing methods available. This module is now part of OTB since version 7.0.0

  - **otb-bv** : Estimation of biophysical variables

        + Repository:  https://gitlab.orfeo-toolbox.org/jinglada/otb-bv
        + Author: Jordi Inglada 
        + License: GNU Affero General Public License v3.0
        + Description: The otb-bv project allows for estimation of biophysical variables (LAI, fAPAR, fCover) from remote sensing images using machine learning nonlinear regressions for the inversion of PROSPECT+SAIL model.

  - **Phenotb** : Extract phenological information from time profiles

        + Repository:  https://gitlab.orfeo-toolbox.org/jinglada/phenotb/ 
        + Author: Jordi Inglada 
        + License: GNU Affero General Public License v3.0
        + Description: This module implements several algorithms allowing to extract phenological information from time profiles. These time profiles should represent vegetation status as for instance NDVI, LAI, etc.

  - **otbFFSforGMM** : Large-scale feature selection with Gaussian mixture models

        + Repository:  https://github.com/Laadr/otbFFSforGMM
        + Author: Adrien Lagrange
        + License: Apache
        + Description: This module implements a method to perform a fast forward feature selection using a Gaussian Mixture Model for the classification of high dimensional remote sensing images. The algorithm is describes in the following paper https://hal.archives-ouvertes.fr/hal-01382500.

  - **GRM** : Generic region merging segmentation

        + Repository:  http://tully.ups-tlse.fr/lassallep/grm/tree/master
        + Author: Pierre Lassalle
        + License: GPL v3
        + Description: This module provides the GRM OTB application to perform multi-scale region-merging segmentation on satellite images. Three local homogeneity criteria are available: the Baatz & Schäpe criterion, the Full Lambda Schedule criterion and the simple Euclidean Distance criterion. This module was contributed by Pierre Lassalle who also provides a tutorial to learn how to use the library.

  - **SertitObject** : Object oriented image analysis

        + Repository:  https://github.com/sertit/SertitObject
        + Author: SERTIT - University of Strasbourg
        + License: CeCILL-B
        + Description: This module provides two OTB applications dedicated to object-oriented image analysis.

  - **Temporal gap-filling** : perform temporal gap-filling in image time series

        + Repository:  http://tully.ups-tlse.fr/jordi/temporalgapfilling.git 
        + Author: Jordi Inglada
        + License: GNU Affero General Public License v3.0
        + Description: This module provides classes and one application to perform for temporal gap-filling in image time series (linear and spline interpolators are provided).

**Community modules**

  - **Feature selection** : 

        + Repository:  https://github.com/boussaffawalid/FeatureSelection 
        + Author: Walid Boussafa and Nesrine Chehata 
        + License: All rights reserverd (without license granting more rights, copyright fully applies, this component can not be used without the explicit and prior authorization of the copyright owner).
        + Description: This module contains a FeatureSelection application based on the `FST3Lib <http://fst.utia.cz/>`__.

  - **OTBTensorflow (otbtf)** : generic, multi purpose deep learning framework, targeting remote sensing images processing

        + Repository:  https://gitlab.irstea.fr/remi.cresson/otbtf 
        + Author: Rémi Cresson 
        + License: Apache License 2.0
        + Description: This remote module of the Orfeo ToolBox provides a generic, multi purpose deep learning framework, targeting remote sensing images processing. It contains a set of new process objects that internally invoke Tensorflow, and a bunch of user-oriented applications to perform deep learning with real-world remote sensing images. Applications can be used to build OTB pipelines from Python or C++ APIs.


Installation and usage
----------------------
**Build possibilities**

Your remote module can be built inside the OTB source tree or outside as
an external CMake project with an existing OTB installation.

* **Building against a build tree**

  In this case you have `compiled OTB from source <https://www.orfeo-toolbox.org/CookBook-develop/CompilingOTBFromSource.html>`__ 
  , the cmake configuration will be done inside OTB build directory.

  Note that there are two ways of compiling:

  - *Build as a module inside OTB*, in which case build files will be written to the OTB build tree as other modules. Main benefit is that this will enrich the current OTB build with your new module, but you need to have write access to the build directory. For this type of build, the cmake configuration is easy, see below the *compilation* chapter

  - *Build as a standalone CMake project*, in which case build files will remain in the module build folder. This build is fully independent from the build (or install) directory, but the module will not be recognized as an OTB module (still you will be able to use its binaries and libraries).

  This behaviour is controlled by the cmake option ``OTB_BUILD_MODULE_AS_STANDALONE``, which is
  OFF by default (hence first behaviour). There are other cmake options to set in order to build it as standalone, this is described below.

* **Building against an installed OTB**

  In this case, only the second behaviour (build as standalone) is available. this requires to specify cmake options for the build : 
    - Set the module to build as standalone with ``OTB_BUILD_MODULE_AS_STANDALONE=ON``
    - Set the OTB install directory with the variable ``OTB_DIR``
    - Set an install folder for your libraries ``CMAKE_INSTALL_PREFIX=/theModulePath/install``
    - Set the runtime path *RPATH* of your libraries to your install/lib folder ``DCMAKE_INSTALL_RPATH=/theModulePath/install/lib``
    - Tell cmake to set runtime path using link path : ``CMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE`` (permits to avoid modifying your LD_LIBRARY_PATH)


**Compilation and Installation**

* If you choose the *inside OTB build*, your module will be built with the rest of the OTB project. To add the module to the compilation process you have two options:
  
  * use OTB to retrieve automaticaly the official remote module (not applicable for community/self remote modules) that you want to build. 
    All you have to do is to call cmake configuration in OTB build dir to activate ``Module_TheModuleName``
  
  * clone the module yourself (necessary if you use a community module, or your own) and copy the folder to *OTBSource/Modules/Remote*, this will trigger a new option in CMake config
    named ``Module_TheModuleName`` which is *OFF* by default.

  Open a terminal and run :

.. code-block:: bash

    cd /PathToOTB/build
    cmake -DModule_TheModuleName=ON
    make install

the applications of your module will be installed in the same folder as OTB applications

* If you choose the *OTB install build* :

.. code-block:: bash

  mkdir /Path/to/Module/build && cd /Path/to/Module/build
  cmake -DOTB_DIR=/PathTo/OTB/install -DOTB_BUILD_MODULE_AS_STANDALONE=ON
  -DCMAKE_INSTALL_PREFIX=/theModulePath/install -DCMAKE_INSTALL_RPATH=/theModulePath/install/lib 
  -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE ../
  make install

the applications will be installed in ``/theModuleInstallFolder/lib`` and the binaries will be available in ``/theModuleInstallFolder/bin`` 

**Usage**

* For an *inside OTB build*, verify that you have the OTB environment variables set, and call your application :

.. code-block:: bash

  otbcli_MyModuleApp arg1 ... argX

* For an *OTB install build*, you have to add your ``/theModuleInstallFolder/lib`` to the variable OTB_APPLICATION_PATH, and ``theModuleInstallFolder/bin`` to the PATH  :

.. code-block:: bash

  export OTB_APPLICATION_PATH=/theModuleInstallFolder/lib:$OTB_APPLICATION_PATH
  export PATH=/theModuleInstallFolder/bin:$PATH 

We strongly recommend **adding these exports in your .bashrc** in order to make your applications available system wise


Writing your own remote module
------------------------------

This section will guide you step by step to create your own remote module.
To start, you can fork our remote module template project:
`Remote Module Template <https://gitlab.orfeo-toolbox.org/remote_modules/remote-module-template>`__.
Each module is made of different components, described in the following sections.

**The otb-module.cmake file**

This file is mandatory. It follows the CMake syntax, and has two
purposes:

-  Declare dependencies to other modules,

-  Provide a short description of the module purpose.

These purposes are fulfilled by a single CMake Macro call:

.. code-block:: cmake

    otb_module(TheModuleName DEPENDS OTBModule1 OTBModule2 ... OTBModuleN DESCRIPTION "A description string")

**Note**: You can use the keyword ``TESTDEPENDS`` to declare module
dependencies that only applies to the tests.

**The CMakeLists.txt file**

The ``CMakeLists.txt`` file is mandatory. It contains only a few things.
First, it declares a new CMake project with the name of the module:

.. code-block:: cmake

    project(OTBTheModuleName)

Second, if the module contain a library (see src folder section below),
it initializes the TheModuleNameLIBRARIES CMake variable (if your module
only contains headers or template code, skip this line):

.. code-block:: cmake

    set(OTBTheModuleName_LIBRARIES OTBTheModuleName)

You can build your remote modules inside the OTB source tree by copying
your source inside the directory ``Module/Remote`` or against an existing
OTB build tree (note that it does not work with an install version of
OTB).

The configuration below will handle both cases and take care of all the
CMake plumbing of the module:

.. code-block:: cmake

    if(NOT OTB_SOURCE_DIR)
      find_package(OTB REQUIRED)
      list(APPEND CMAKE_MODULE_PATH ${OTB_CMAKE_DIR})
      # The Python interpreter is needed for Python tests
      set(Python_ADDITIONAL_VERSIONS "3")
      find_package( PythonInterp REQUIRED)
      include(UseOTB)
      include(OTBModuleExternal)
    else()
      otb_module_impl()
    endif()

The overall file should look like this:

.. code-block:: cmake

    cmake_minimum_required(VERSION 3.10.0)
    project(OTBTheModuleName)
    set(OTBTheModuleName_LIBRARIES OTBTheModuleName)

    if(NOT OTB_SOURCE_DIR)
      find_package(OTB REQUIRED)
      list(APPEND CMAKE_MODULE_PATH ${OTB_CMAKE_DIR})
      # The Python interpreter is needed for Python tests
      set(Python_ADDITIONAL_VERSIONS "3")
      find_package( PythonInterp REQUIRED)
      include(UseOTB)
      include(OTBModuleExternal)
    else()
      otb_module_impl()
    endif()

**The include folder**

The include folder will contain all your headers (``*.h`` files) and
template method files (``*.hxx`` or ``*.hxx``). It does not require any
additional file (in particular, no CMakeLists.txt file is required).

**The src folder**

The src folder contains the internal implementation of your module:

-  It typically contains cxx source files that will be compiled into a
   library.

-  It can contain header files for classes used only within the
   implementation files of your module. Any header file present in the
   src folder will not be installed, and will not be available to other
   modules depending on your module.

If your modules is made of template only code, you do not need a src
folder at all.

If present, the src folder requires a CMakeLists.txt file.

The first part of the CMakeLists.txt file is classical, as it builds the
library and links it:

.. code-block:: cmake

    set(OTBTheModuleName_SRC
        sourceFile1.cxx
        sourceFile2.cxx
        sourceFile3.cxx
        ...
        sourceFileN.cxx)

    add_library(OTBTheModuleName ${OTBTheModuleName_SRC})

    target_link_libraries(OTBTheModuleName ${OTBModule1_LIBRARIES} ${OTBModule2_LIBRARIES} ... ${OTBModuleN_LIBRARIES})

*Notes*:

-  Library name should match the one declared in the root CMakeLists.txt
   when setting CMake variable TheModuleName_LIBRARIES,

-  Linked libraries should match the dependencies of your module
   declared in the root otb-module.cmake file.

The last line of CMake code takes care of installation instructions:

.. code-block:: cmake

    otb_module_target(OTBTheModuleName)

The overall CMakeLists.txt file should look like:

.. code-block:: cmake

    set(OTBTheModuleName_SRC
        sourceFile1.cxx
        sourceFile2.cxx
        sourceFile3.cxx
        ...
        sourceFileN.cxx)

    add_library(OTBTheModuleName ${OTBTheModuleName_SRC})

    target_link_libraries(OTBTheModuleName ${OTBModule1_LIBRARIES} ${OTBModule2_LIBRARIES} ... ${OTBModuleN_LIBRARIES})

    otb_module_target(OTBTheModuleName)

**The app folder**

The app folder contains the code of applications shipped with your module. If your module has no application, you do not need the app folder.

*Notes*: If your module contains application (and an app folder), do not forget to add the ApplicationEngine in the dependencies listed in
the otb-module.cmake file.

In addition to the applications source code, the app folder should
contain a CMakeLists.txt file as follows.

For each application, a single call otbcreateapplication is required:

.. code-block:: cmake

    otb_create_application(
      NAME           TheModuleApplication1
      SOURCES        TheModuleApplication1.cxx
      LINK_LIBRARIES ${OTBModule1_LIBRARIES} ${OTBModule2_LIBRARIES} ... ${OTBModuleN_LIBRARIES})

**The test folder**

This folder contains tests of the module. If your module has no test in
it (which is not recommended, you do not need it).

The test folder should contain the source files of tests, as well as a
CMakeLists.txt file. This file will contain the following.

First, indicate that this folder contains tests.

.. code-block:: cmake

    otb_module_test()

Then, build the test driver:

.. code-block:: cmake

    set(OTBTheModuleNameTests
        testFile1.cxx
        testFile2.cxx
        ...
        testFileN.cxx)

    add_executable(otbTheModuleNameTestDriver ${OTBTheModuleNameTests})

    target_link_libraries(otbTheModuleNameTestDriver ${OTBTheModuleName-Test_LIBRARIES})

    otb_module_target_label(otbTheModuleNameTestDriver)

Finally, you can add your tests:

.. code-block:: cmake

    otb_add_test(NAME nameOfTheTest COMMAND otbTheModuleNameTestDriver
                 --compare-image ${EPSILON_8} ... # baseline comparison if needed
                 nameOfTheTestFunction
                 testParameters)

If your module contains one or more application in the app folder, you
should also write tests for them, in the test folder. Running an
application test is easily done with the helper macro
otbtestapplication:

.. code-block:: cmake

    otb_test_application(NAME   nameofApplication1Test1
                          APP  TheModuleApplication1
                          OPTIONS -in1 ${INPUTDATA}/input1.tif
                                  -in2 ${INPUTDATA}/input2.tif
                                  -out ${TEMP}/nameofApplication1Test1_result.tif
                          VALID   --compare-image ${EPSILON_8}
                                  ${BASELINE}/nameofApplication1Test1_result.tif
                                  ${TEMP}/nameofApplication1Test1_result.tif)

To add a **test executed by a Python script** using OTB Applications bindings:

.. code-block:: cmake

    set(TEST_DRIVER otbTestDriver
        --add-before-env OTB_APPLICATION_PATH $<TARGET_FILE_DIR:otbapp_EmptyApp> )

    otb_add_test(NAME otbEmptyScriptTest
      COMMAND ${TEST_DRIVER} Execute ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/EmptyScript.py)

Overall CMakeLists.txt should look like:

.. code-block:: cmake

    otb_module_test()

    set(OTBTheModuleNameTests
        testFile1.cxx
        testFile2.cxx
        ...
        testFileN.cxx)

    add_executable(otbTheModuleNameTestDriver ${OTBTheModuleNameTests})

    target_link_libraries(otbTheModuleNameTestDriver ${OTBTheModuleName-Test_LIBRARIES})

    otb_module_target_label(otbTheModuleNameTestDriver)

    otb_add_test(NAME nameOfTheTest COMMAND otbTheModuleNameTestDriver
                 --compare-image ${EPSILON_8} ... # baseline comparison if needed
                 nameOfTheTestFunction
                 testParameters)

    otb_test_application(NAME otbEmptyAppTest
                     APP  EmptyApp
                     )

Sharing your module
-------------------

To make your remote module available to others, you should first host the module code on a
publicly available git repository. If you do not have access to a git server, bitbucket or github can provide this service for you.
Then you should provide a CMake file named **TheModuleName.remote.cmake** for inclusion in the Modules/Remote folder in OTB source tree.

This file should contain the following:

.. code-block:: cmake

    # Contact: Author name <author email address>

    otb_fetch_module(TheModuleName
      "A description of the module, to appear during CMake configuration step"

      GIT_REPOSITORY http_link_to_a_git_repository_hosting_the_module
      GIT TAG the git revision_to_checkout
    )

This file should be provided along with your remote module inclusion
proposal email to the otb community list : acceptance of remote module is submitted to vote on otb-developers.
If accepted, your CMake file will be placed into the Modules/Remote folder inside OTB source tree.

**Important Note**: a remote module bringing in new third party dependencies will not be included in binary packages. 

During the OTB release process, all modules complying with the remote module release policy will be packaged along with standard modules.

A remote module can be removed from Modules/Remote (this only requires to remove the CMake file describing it), if:

  - It does no longer comply with the remote module acceptance policy (in which case the decision is submitted to vote on `OTB's forum <https://forum.orfeo-toolbox.org/>`_),
  - The author of the remote module ask to remove it.

* **Remote module acceptance policy**

So as to get your module accepted as an official remote module, you should comply with the following:

  - Remote module source code should be hosted on a publicly available Git repository
  - Author of the remote module should be identified and registered to `OTB's forum <https://forum.orfeo-toolbox.org/>`_
  - Author of the remote module accepts to be contacted by developers or users regarding issues with his module (on a best effort basis),
  - Remote module source code should comply with OTB style as much as possible,
  - Remote module source code should be documented using doxygen tags,
  - Remote module should provide a minimal set of tests to ensure build of template code and basic non-regression testing,
  - Remote module should come with a form of documentation (website, publication, readme file ...)
  - Remote module should not embed code from any third party software (unless strong arguments are given by the author, in which case an exception can be made),
  - Remote module should avoid depending on new third parties if possible,
  - Remote module author should be the copyright owner and comply with licences of any third party, which in turn should comply with terms of OTB licence (to be reviewed by PSC)
  - Author of remote module should provide a small description of the remote module to be added on OTB website.

An internal module should never depend on a remote module whatsoever.

* **Remote module release policy**

During the OTB release process, a remote module will be included in source and binary packages if dashboard submission exist and show that the remote module:
  
  - Builds on all plateform
  - Passes all tests on the reference platform
  - Does not have any test crashing (i.e. failing with core dump or memory issues) on remaining platform
  - The remote module complies with the remote module acceptance policy at the time of the Release Candidate

Developers will notify remote modules authors of existing issues at Release Candidate. If by 3 day to the final release dates, some issues listed above still exist, the remote module will be removed from the release source and binary packages.

Use Continuous Integration
--------------------------

We encourage you to use a CI platform during the development of your module.

In the remote module template, we provide two files which permits to put your module on a CI
  * ci.cmake => used to build your module
  * travis.yml => used by Travis-CI to call cmake using ci.cmake script and launch your tests

Those files enable your module to use Travis-CI, which is the github continuous integration platform.
if your repository is on Gitlab, you can make a `mirror of your repo to github <https://docs.gitlab.com/ee/user/project/repository/repository_mirroring.html#setting-up-a-push-mirror-from-gitlab-to-github>`__

To modify the travis.yml: use the `manual <https://docs.travis-ci.com/user/customizing-the-build/>`__

You have two options to use OTB in your CI :
* Get an installer from the website and install it in the ``install:`` part of travis.yml. You will have to build your module as standalone

.. code-block:: bash
  env:
    global:
      - OTB_URL=https://www.orfeo-toolbox.org/packages/archives/OTB
      - OTB_VER=7.2.0
      - OTB_OS=Linux64
      - OTB_PKG_EXT=run

  install:
    - export OTB_PKG="OTB-${OTB_VER}-${OTB_OS}.${OTB_PKG_EXT}"
    - wget ${OTB_URL}/${OTB_PKG}
    - chmod +x ${OTB_PKG}
    - ./${OTB_PKG} --target xdk

  script:
    - command to build and test your module here

* Build a docker image containing an otb build tree. Run this docker image in travis and build you module against this build tree

.. code-block:: bash

  before_script:
    - docker pull YourOTBImage

  script:
    - docker run -it YourOTBImage /bin/bash -c "ctest -VV -S ci.cmake"


In the before script section, you have to set the environment variables. For this you can create a script called activate_env.sh in your main module folder which contains:

.. code-block:: bash

  export OTB_RESULT_DIR=/home/travis/build/yourName/yourModule/data/OutputTest
  export OTB_DATA_DIR=/home/travis/build/yourName/yourModule/data
  export PYTHONPATH=/OTB_InstallDir/lib/otb/python
  export OTB_APPLICATION_PATH=/home/travis/build/yourName/yourModule/install/lib:/OTB_InstallDir/lib/otb/applications

and call it in travis.yml:

.. code-block:: bash

  before-script: 
     - source activate_env.sh

You can test your module with one line in the travis.yml:

.. code-block:: bash

  script:
     - ctest -VV -S ci.cmake

This command builds your project and launches the tests.
