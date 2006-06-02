# This is the root ITK CMakeLists file.

PROJECT(ITK)

#-----------------------------------------------------------------------------
# ITK version number.  An even minor number corresponds to releases.
SET(ITK_VERSION_MAJOR "2")
SET(ITK_VERSION_MINOR "6")
SET(ITK_VERSION_PATCH "0")

# Version string should not include patch level.  The major.minor is
# enough to distinguish available features of the toolkit.
SET(ITK_VERSION_STRING "${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}")

#-----------------------------------------------------------------------------
# Load some macros.
INCLUDE(${ITK_SOURCE_DIR}/Utilities/itkThirdParty.cmake)


#-----------------------------------------------------------------------------
# Configure files with settings for use by the build.
CONFIGURE_FILE(${ITK_SOURCE_DIR}/itkConfigure.h.in 
               ${ITK_BINARY_DIR}/itkConfigure.h)


#-----------------------------------------------------------------------------
# Configure Dart testing support.  This should be done before any
# MESSAGE(FATAL_ERROR ...) commands are invoked.
# INCLUDE(${CMAKE_ROOT}/Modules/Dart.cmake)
# MARK_AS_ADVANCED(TCL_TCLSH DART_ROOT)
# IF(BUILD_TESTING)
#   ENABLE_TESTING()
#   CONFIGURE_FILE(${ITK_SOURCE_DIR}/InsightLogo.gif
#                  ${ITK_BINARY_DIR}/Testing/HTML/TestingResults/Icons/Logo.gif
#                  COPYONLY IMMEDIATE)
#   SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
#   MARK_AS_ADVANCED(BUILDNAME)
# ENDIF(BUILD_TESTING)

# Deal with FFT v3 Options
MARK_AS_ADVANCED(USE_FFTWD)
OPTION(USE_FFTWD "Use double precision FFTW if found" OFF)
MARK_AS_ADVANCED(USE_FFTWF)
OPTION(USE_FFTWF "Use single precision FFTW if found" OFF)
IF( USE_FFTWD OR USE_FFTWF )
    FIND_PACKAGE( FFTW )
ENDIF( USE_FFTWD OR USE_FFTWF )

# Deal with SGI SCSL Options
OPTION(USE_SCSL "Use the SGI SCSL High Performance Computing math library" OFF)
MARK_AS_ADVANCED(USE_SCSL)
IF( USE_SCSL )
    FIND_PACKAGE( SCSL )
ENDIF( USE_SCSL )

# Put "." in the rpath of all ITK shared libraries.
LINK_DIRECTORIES(.)

#-----------------------------------------------------------------------------
# ITK build configuration options.
OPTION(BUILD_SHARED_LIBS "Build ITK with shared libraries." OFF)
SET(ITK_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

OPTION(ITK_CSWIG_TCL "Build cswig Tcl wrapper support (requires CableSwig)." OFF)
OPTION(ITK_CSWIG_PYTHON "Build cswig Python wrapper support (requires CableSwig)." OFF)
OPTION(ITK_CSWIG_JAVA "Build cswig Java wrapper support " OFF)

# Options for building some of the most common core templates explicitly only
# once.  This has only been tested on gcc compilers.
OPTION(ITK_EXPLICIT_INSTANTIATION "BETA-Try to explicitly build some of the core templates explictitly rather than over and over again." OFF)
MARK_AS_ADVANCED(ITK_EXPLICIT_INSTANTIATION)
IF( ITK_EXPLICIT_INSTANTIATION )
 ADD_DEFINITIONS(-DITK_EXPLICIT_INSTANTIATION)
ENDIF( ITK_EXPLICIT_INSTANTIATION )
# perl support does not work, contact bill hoffman at kitware
# if you are interested in perl wrapping.  It is close, but
# not there yet.
#OPTION(ITK_CSWIG_PERL "Build cswig Perl wrapper support " OFF)

#-----------------------------------------------------------------------------
# ITK build patented algorithms
OPTION(ITK_USE_PATENTED "Build algorithms that are covered with Patents. It is YOUR responsibility to obtain a license from the patent holder in order to use these methods." OFF)
MARK_AS_ADVANCED(ITK_USE_PATENTED)

#-----------------------------------------------------------------------------
# ITK build classes that are in the review process
OPTION(ITK_USE_REVIEW "Build algorithms that are candidates to be included in the Toolkit. This classes are not covered by the Backward Compatibility Policy, nor the Copyright Policy of ITK." OFF)
MARK_AS_ADVANCED(ITK_USE_REVIEW)


#-----------------------------------------------------------------------------
# Do we need CableSwig?
SET(ITK_NEED_CableSwig 0)

IF(ITK_CSWIG_TCL)
  SET(ITK_NEED_CableSwig 1)
ENDIF(ITK_CSWIG_TCL)

IF(ITK_CSWIG_PYTHON)
  SET(ITK_NEED_CableSwig 1)
ENDIF(ITK_CSWIG_PYTHON)

IF(ITK_CSWIG_JAVA)
  SET(ITK_NEED_CableSwig 1)
ENDIF(ITK_CSWIG_JAVA)

IF(ITK_CSWIG_PERL)
  SET(ITK_NEED_CableSwig 1)
ENDIF(ITK_CSWIG_PERL)

IF(ITK_NEED_CableSwig)
  IF(NOT BUILD_SHARED_LIBS)
    MESSAGE(FATAL_ERROR "Wrapping requires a shared build, change BUILD_SHARED_LIBS to ON")
  ENDIF(NOT BUILD_SHARED_LIBS)
ENDIF(ITK_NEED_CableSwig)

#-----------------------------------------------------------------------------
# Find CableSwig if necessary.
IF(ITK_NEED_CableSwig)
  IF(EXISTS ${ITK_SOURCE_DIR}/Utilities/CableSwig)
    SET(CMAKE_MODULE_PATH ${ITK_SOURCE_DIR}/Utilities/CableSwig/SWIG/CMake)

    # CableSwig is included in the source distribution.
    SET(ITK_BUILD_CABLESWIG 1)
    SET(CableSwig_DIR ${ITK_BINARY_DIR}/Utilities/CableSwig)
    SET(CableSwig_FOUND 1)
    SET(CableSwig_INSTALL_ROOT /lib/InsightToolkit/CSwig)
    INCLUDE(${CableSwig_DIR}/CableSwigConfig.cmake OPTIONAL) 
  ELSE(EXISTS ${ITK_SOURCE_DIR}/Utilities/CableSwig)
    # CableSwig is not included in the source distribution.  Find it.
    FIND_PACKAGE(CableSwig)
    SET(CMAKE_MODULE_PATH ${CableSwig_DIR}/SWIG/CMake)

    # Hack for CMake 1.6 compatability.
    IF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 1.7)
      SET(CableSwig_FOUND ${CABLESWIG_FOUND})
      SET(CableSwig_DIR ${CABLESWIG_DIR})
    ENDIF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} LESS 1.7)
  ENDIF(EXISTS ${ITK_SOURCE_DIR}/Utilities/CableSwig)
  
  IF(NOT CableSwig_FOUND)
    # We have not found CableSwig.  Complain.
    MESSAGE(FATAL_ERROR "CableSwig is required for CSwig Wrapping.")
  ENDIF(NOT CableSwig_FOUND)
ENDIF(ITK_NEED_CableSwig)

#-----------------------------------------------------------------------------


#-----------------------------------------------------------------------------
# Find wrapping language API libraries.
IF(ITK_CSWIG_TCL)
  FIND_PACKAGE(TCL)
  # Hide useless settings provided by FindTCL.
  FOREACH(entry TCL_LIBRARY_DEBUG
                TK_LIBRARY_DEBUG
                TCL_STUB_LIBRARY
                TCL_STUB_LIBRARY_DEBUG
                TK_STUB_LIBRARY
                TK_STUB_LIBRARY_DEBUG
                TK_WISH)
    SET(${entry} "${${entry}}" CACHE INTERNAL "This value is not used by ITK.")
  ENDFOREACH(entry)
ENDIF(ITK_CSWIG_TCL)

IF(ITK_CSWIG_PYTHON)
  INCLUDE(${CMAKE_ROOT}/Modules/FindPythonLibs.cmake)
  FIND_PROGRAM(PYTHON_EXECUTABLE
    NAMES python python2.3 python2.2 python2.1 python2.0 python1.6 python1.5 
    PATHS
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.3\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.2\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.1\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\2.0\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\1.6\\InstallPath]
    [HKEY_LOCAL_MACHINE\\SOFTWARE\\Python\\PythonCore\\1.5\\InstallPath]
    )
  MARK_AS_ADVANCED(PYTHON_EXECUTABLE)
ENDIF(ITK_CSWIG_PYTHON)

IF(ITK_CSWIG_JAVA)
  FIND_PACKAGE(Java)
  FIND_PACKAGE(JNI)
ENDIF(ITK_CSWIG_JAVA)
IF(ITK_CSWIG_PERL)
  FIND_PACKAGE(Perl)
  FIND_PACKAGE(PerlLibs)
ENDIF(ITK_CSWIG_PERL)

IF(WIN32)
   IF(BUILD_SHARED_LIBS)
     SET(ITK_COMMON_BUILD_TYPE "SHARED")
   ELSE(BUILD_SHARED_LIBS)
     SET(ITK_COMMON_BUILD_TYPE "STATIC")
   ENDIF(BUILD_SHARED_LIBS)
   SET(BUILD_SHARED_LIBS OFF)
ENDIF(WIN32)

OPTION(BUILD_EXAMPLES "Build the Examples directory." ON)

OPTION(ITK_USE_SYSTEM_VXL "Use an outside build of VXL." OFF)
MARK_AS_ADVANCED(ITK_USE_SYSTEM_VXL)
IF(ITK_USE_SYSTEM_VXL)
  FIND_PACKAGE(VXL)
  IF(VXL_FOUND)
    INCLUDE(${VXL_CMAKE_DIR}/UseVXL.cmake)
    SET(VXL_NUMERICS_LIBRARIES vnl vnl_algo netlib vcl)
  ELSE(VXL_FOUND)
    MESSAGE(FATAL_ERROR "Must set VXL_DIR for ITK_USE_SYSTEM_VXL.")
  ENDIF(VXL_FOUND)
ELSE(ITK_USE_SYSTEM_VXL)
  SET(VXL_NETLIB_INCLUDE_DIR ${ITK_SOURCE_DIR}/Utilities/vxl/v3p/netlib)
  SET(VXL_NUMERICS_LIBRARIES itkvnl itkvnl_algo itknetlib)
ENDIF(ITK_USE_SYSTEM_VXL)

# Provide options to use system versions of third-party libraries.
ITK_THIRD_PARTY_OPTION(ZLIB zlib)
ITK_THIRD_PARTY_OPTION(PNG  png)
ITK_THIRD_PARTY_OPTION(TIFF tiff)

MARK_AS_ADVANCED(ITK_CSWIG_PYTHON)
MARK_AS_ADVANCED(ITK_CSWIG_TCL)
MARK_AS_ADVANCED(ITK_CSWIG_JAVA)
MARK_AS_ADVANCED(ITK_CSWIG_PERL)
MARK_AS_ADVANCED(CABLE_INDEX)
MARK_AS_ADVANCED(CSWIG)
MARK_AS_ADVANCED(GCCXML)

#-----------------------------------------------------------------------------
# Dispatch the build into the proper subdirectories.
SUBDIRS(Utilities Code)

# set a variable to determine if 
# the Wrapping/CSwig directory should be used
SET(ITK_CSWIG_DIR 0)
IF(ITK_CSWIG_TCL)
  SET(ITK_CSWIG_DIR 1)
ENDIF(ITK_CSWIG_TCL)
IF(ITK_CSWIG_PYTHON)
  SET(ITK_CSWIG_DIR 1)
ENDIF(ITK_CSWIG_PYTHON)
IF(ITK_CSWIG_JAVA)
  SET(ITK_CSWIG_DIR 1)
ENDIF(ITK_CSWIG_JAVA)

IF(ITK_CSWIG_DIR)
  SUBDIRS(Wrapping/CSwig)
ENDIF(ITK_CSWIG_DIR)



#-----------------------------------------------------------------------------
# The entire ITK tree should use the same include path, except for the
# Wrapping directory.

# Create the list of include directories needed for ITK header files.
#INCLUDE(${ITK_SOURCE_DIR}/itkIncludeDirectories.cmake)
#SET(OTB_INCLUDE_DIRS_BUILD_TREE ${OTB_INCLUDE_DIRS_BUILD_TREE}
#	  		${ITK_INCLUDE_DIRS_BUILD_TREE})


# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
INCLUDE_DIRECTORIES(
  ${ITK_INCLUDE_DIRS_BUILD_TREE}
  ${ITK_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${ITK_INCLUDE_DIRS_SYSTEM}
)
