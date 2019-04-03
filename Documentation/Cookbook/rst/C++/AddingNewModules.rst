Adding New Modules
==================

This chapter is concerned with the creation of new modules. The
following sections give precise instructions about:

-  the organization of directories

-  the included files

-  what they must contain

-  ...

How to Write a Module
---------------------

There is a template of OTB remote module which help you start developing
a remote module: `External Module
Template <https://gitlab.orfeo-toolbox.org/remote_modules/remote-module-template>`__.

Each module is made of different components, described in the
following sections.

The otb-module.cmake file
-------------------------

This file is mandatory. It follows the CMake syntax, and has two
purposes:

-  Declare dependencies to other modules,

-  Provide a short description of the module purpose.

These purposes are fulfilled by a single CMake Macro call:

.. code-block:: cmake

    otb_module(TheModuleName DEPENDS OTBModule1 OTBModule2 ... OTBModuleN DESCRIPTION "A description string")

**Note**: You can use the keyword ``TESTDEPENDS`` to declare module
dependencies that only applies to the tests.

The CMakeLists.txt file
-----------------------

The ``CMakeLists.txt`` file is mandatory. It contains only a few things.
First, it declares a new CMake project with the name of the module:

.. code-block:: cmake

    project(TheModuleName)

Second, if the module contain a library (see src folder section below),
it initializes the TheModuleNameLIBRARIES CMake variable (if your module
only contains headers or template code, skip this line):

.. code-block:: cmake

    set(TheModuleName_LIBRARIES OTBTheModuleName)

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
      include(OTBModuleExternal)
    else()
      otb_module_impl()
    endif()

The overall file should look like this:

.. code-block:: cmake

    cmake_minimum_required(VERSION 2.8.9)
    project(TheModuleName)
    set(ExternalTemplate_LIBRARIES OTBTheModuleName)

    if(NOT OTB_SOURCE_DIR)
      find_package(OTB REQUIRED)
      list(APPEND CMAKE_MODULE_PATH ${OTB_CMAKE_DIR})
      include(OTBModuleExternal)
    else()
      otb_module_impl()
    endif()

The include folder
------------------

The include folder will contain all your headers (``*.h`` files) and
template method files (``*.hxx`` or ``*.hxx``). It does not require any
additional file (in particular, no CMakeLists.txt file is required).

The src folder
--------------

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

**Notes**:

-  Library name should match the one declared in the root CMakeLists.txt
   when setting CMake variable TheModuleNameLIBRARIES,

-  Linked libraries should match the dependencies of your module
   declared in the root otb-module.cmake file.

The last line of CMake code takes care of installation instructions:

.. code-block:: cmake

    otb_module_target(TBTheModuleName)

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

    otb_module_target(TBTheModuleName)

The app folder
--------------

The app folder contains the code of applications shipped with your
module. If your module has no application, you do not need the app
folder.

**Notes**: If your module contains application (and an app folder), do
not forget to add the ApplicationEngine in the dependencies listed in
the otb-module.cmake file.

In addition to the applications source code, the app folder should
contain a CMakeLists.txt file as follows.

For each application, a single call otbcreateapplication is required:

.. code-block:: cmake

    otb_create_application(
      NAME           TheModuleApplication1
      SOURCES        TheModuleApplication1.cxx
      LINK_LIBRARIES ${OTBModule1_LIBRARIES} ${OTBModule2_LIBRARIES} ... ${OTBModuleN_LIBRARIES})

The test folder
---------------

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

Including a remote module in OTB
--------------------------------

*  Local build of a remote module

Your remote module can be build inside the OTB source tree or outside as
a external CMake project with an existing OTB. Please note in that case
that you’ll have to set OTBDIR CMake option.

If OTBDIR is an OTB build tree, there are two ways of compiling:

-  Build as a module, in which case build files will be written to the
   OTB build tree as other modules. Main benefit is that this will
   enrich the current OTB build with your new module, but you need to
   have write access to the build directory.

-  Build as a standalone CMake project, in which case build files will
   remain in the module build folder. This build is fully independent
   from the build (or install) directory, but the module will not be
   recognized as an OTB module (still you will be able to use its
   binaries and libraries).

This behaviour is controlled by the ``OTB_BUILD_MODULE_AS_STANDALONE``, which is
OFF by default (hence first behaviour).

Note that when dealing with an installed OTB, only the second behaviour
(build as standalone) is available.

Optionally, you can build your new remote module inside the OTB source
tree by simply copy the folder containing the module component to
Modules/Remote, then run CMake configuration. you should see a new CMake
option named MODULETheModuleName. Simply turn this option to ON, and
finish CMake configuration. Your module will be built with the rest of
the OTB project.

*  Sharing your remote module

To make your remote module available to others when building OTB, you
should provide a CMake file named TheModuleName.remote.cmake file for
inclusion in the Modules/Remote folder in OTB source tree.

This file should contain the following:

.. code-block:: cmake

    # Contact: Author name <author email address>

    otb_fetch_module(TheModuleName
      "A description of the module, to appear during CMake configuration step"

      GIT_REPOSITORY http_link_to_a_git_repository_hosting the module
      GIT TAG the git revision to checkout
    )

This file should be provided along with your remote module inclusion
proposal email to the otb community list. Please refer to the
contributors guidelines for more information.
