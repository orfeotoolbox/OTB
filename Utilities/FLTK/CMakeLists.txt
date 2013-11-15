# Main CMakeLists.txt to build the FLTK project using CMake (www.cmake.org)
# Written by Andy Cedilnik and Julien Jomier

project(FLTK)
cmake_minimum_required(VERSION 2.0)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif()

# The FLTK version
set(FLTK_VERSION_MAJOR "1")
set(FLTK_VERSION_MINOR "1")
set(FLTK_VERSION_PATCH "9")
set(FLTK_VERSION "${FLTK_VERSION_MAJOR}.${FLTK_VERSION_MINOR}")
set(FLTK_VERSION_FULL "${FLTK_VERSION}.${FLTK_VERSION_PATCH}")

set(FLTK_LIBRARIES "fltk_images;fltk;fltk_gl;fltk_forms" CACHE INTERNAL "Variable used by OTB to link against FLTK")

# Allow building shared libraries
option(BUILD_SHARED_LIBS "Build FLTK as a shared library" OFF)

# Search for modules in the FLTK source dir first
set(CMAKE_MODULE_PATH "${FLTK_SOURCE_DIR}/CMake")

#-----------------------------------------------------------------------------
# Test for some required system information.
find_package(Threads)
set(CMAKE_USE_PTHREADS
  "${CMAKE_USE_PTHREADS_INIT}" CACHE BOOL "Use the pthreads library.")

# We need ansi c-flags, especially on HP
set(CMAKE_C_FLAGS "${CMAKE_ANSI_CFLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_REQUIRED_FLAGS ${CMAKE_ANSI_CFLAGS})

if(WIN32)
  if(NOT CYGWIN)
    if(BORLAND)
      set( FLTK_PLATFORM_DEPENDENT_LIBS import32 )
    else()
      set( FLTK_PLATFORM_DEPENDENT_LIBS wsock32 comctl32 )
    endif()
  endif()
endif()

set(FLTK_X11 1)
set(FLTK_APPLE 0)
if(APPLE)
  option(FLTK_APPLE_X11 "Use X11 on Mac instead of Carbon" OFF)
  mark_as_advanced(FLTK_APPLE_X11)
  if(NOT FLTK_APPLE_X11)
    set(FLTK_APPLE 1)
    set(FLTK_X11 0)
    option(FLTK_QUARTZ "Use Quartz instead of Quickdraw" OFF)
  endif()
endif()

if(UNIX)
  find_package(X11)
  set( FLTK_PLATFORM_DEPENDENT_LIBS ${X11_LIBRARIES} -lm)
endif()

if(APPLE AND NOT FLTK_APPLE_X11)
  set( FLTK_PLATFORM_DEPENDENT_LIBS
    "-framework Carbon -framework Cocoa -framework ApplicationServices -lz")
endif()

if(CYGWIN)
  add_definitions(-DWIN32)
  set( FLTK_PLATFORM_DEPENDENT_LIBS ole32 uuid comctl32 wsock32 supc++ -lm -lgdi32)
endif()

if(MINGW)
  add_definitions(-DWIN32)
  set( FLTK_PLATFORM_DEPENDENT_LIBS ole32 uuid wsock32 gdi32 comdlg32)
endif()

include(CheckIncludeFiles)
# Check if header file exists and add it to the list.
macro(CHECK_INCLUDE_FILE_CONCAT FILE VARIABLE)
  CHECK_INCLUDE_FILES("${PROJECT_INCLUDES};${FILE}" ${VARIABLE})
  if(${VARIABLE})
    set(PROJECT_INCLUDES ${PROJECT_INCLUDES} ${FILE})
  endif()
endmacro()
CHECK_INCLUDE_FILE_CONCAT("GL/glu.h"       HAVE_GL_GLU_H)
CHECK_INCLUDE_FILE_CONCAT("OpenGL/glu.h"   HAVE_OPENGL_GLU_H)
CHECK_INCLUDE_FILE_CONCAT("dirent.h"       HAVE_DIRENT_H)
CHECK_INCLUDE_FILE_CONCAT("stdio.h"        HAVE_STDIO_H)
CHECK_INCLUDE_FILE_CONCAT("strings.h"      HAVE_STRINGS_H)
CHECK_INCLUDE_FILE_CONCAT("sys/dir.h"      HAVE_SYS_DIR_H)
CHECK_INCLUDE_FILE_CONCAT("sys/ndir.h"     HAVE_SYS_NDIR_H)
CHECK_INCLUDE_FILE_CONCAT("sys/select.h"   HAVE_SYS_SELECT_H)
CHECK_INCLUDE_FILE_CONCAT("sys/stdtypes.h" HAVE_SYS_STDTYPES_H)
CHECK_INCLUDE_FILE("pthread.h"      CMAKE_HAVE_PTHREAD_H)

find_package(ZLIB)
find_package(PNG)
find_package(JPEG)

include(CheckSymbolExists)
include(CheckFunctionWithHeaderExists)

CHECK_FUNCTIONWITHHEADER_EXISTS("int strcasecmp()"  "${PROJECT_INCLUDES}" HAVE_STRCASECMP)

CHECK_SYMBOL_EXISTS(strlcat       "${PROJECT_INCLUDES}" HAVE_STRLCAT)
CHECK_SYMBOL_EXISTS(strlcpy       "${PROJECT_INCLUDES}" HAVE_STRLCPY)
CHECK_SYMBOL_EXISTS(vsnprintf     "${PROJECT_INCLUDES}" HAVE_VSNPRINTF)
CHECK_SYMBOL_EXISTS(snprintf      "${PROJECT_INCLUDES}" HAVE_SNPRINTF)
CHECK_SYMBOL_EXISTS(scandir       "${PROJECT_INCLUDES}" HAVE_SCANDIR)

include(CheckTypeSize)

CHECK_TYPE_SIZE(short SIZEOF_SHORT)
CHECK_TYPE_SIZE(int   SIZEOF_INT)
CHECK_TYPE_SIZE(long  SIZEOF_LONG)

if(${SIZEOF_SHORT} MATCHES "^2$")
  set(U16 "unsigned short")
endif()

if(${SIZEOF_INT} MATCHES "^4$")
  set(U32 "unsigned")
else()
  if(${SIZEOF_LONG} MATCHES "^4$")
    set(U32 "unsigned long")
  endif()
endif()

if(${SIZEOF_INT} MATCHES "^8$")
  set(U64 "unsigned")
else()
  if(${SIZEOF_LONG} MATCHES "^8$")
    set(U64 "unsigned long")
  endif()
endif()

# Set an option to build FLTK with OpenGL support
set(HAVE_GL 0)
option(USE_OPENGL "OpenGL Support" ON)
if(USE_OPENGL)
  find_package(OpenGL)
  if(OPENGL_FOUND)
    set(HAVE_GL 1)
    include_directories(${OPENGL_INCLUDE_DIR})
  endif()
endif()

#
# Perform the FLTK specific test with status output
#
macro(PERFORM_CMAKE_TEST FILE TEST)
  if("${TEST}" MATCHES "^${TEST}$")
    # Perform test
    set(MACRO_CHECK_FUNCTION_DEFINITIONS
      "-D${TEST} ${CMAKE_REQUIRED_FLAGS}")
    if(CMAKE_REQUIRED_LIBRARIES)
      set(TEST_ADD_LIBRARIES
        "-DLINK_LIBRARIES:STRING=${CMAKE_REQUIRED_LIBRARIES}")
    endif()
    message(STATUS "Performing Test ${TEST}")

    try_compile(${TEST}
      ${CMAKE_BINARY_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
      CMAKE_FLAGS -DCOMPILE_DEFINITIONS:STRING=${MACRO_CHECK_FUNCTION_DEFINITIONS}
      -DLINK_LIBRARIES:STRING=${CMAKE_TEST_SPECIAL_LIBRARIES}
      "${TEST_ADD_LIBRARIES}"
      OUTPUT_VARIABLE OUTPUT)
    if(${TEST})
      set(${TEST} 1 CACHE INTERNAL "CMake test ${FUNCTION}")
      message(STATUS "Performing Test ${TEST} - Success")
    else()
      message(STATUS "Performing Test ${TEST} - Failed")
      set(${TEST} 0 CACHE INTERNAL "Test ${FUNCTION}")
      write_file(${CMAKE_BINARY_DIR}/CMakeError.log
        "Performing Test ${TEST} failed with the following output:\n"
        "${OUTPUT}\n" APPEND)
    endif()
  endif()
endmacro()

# Set an option to build the zlib library or not
# option(FLTK_USE_SYSTEM_ZLIB "Use's system zlib" OFF)
if(ZLIB_FOUND)
	set(CMAKE_TEST_SPECIAL_LIBRARIES ${ZLIB_LIBRARIES})
	set(FLTK_ZLIB_LIBRARIES ${ZLIB_LIBRARIES})
	PERFORM_CMAKE_TEST(CMake/PlatformTests.cxx HAVE_LIBZ)
else()
	mark_as_advanced(ZLIB_INCLUDE_DIR)
	mark_as_advanced(ZLIB_LIBRARY)
	if(WIN32)
		if(NOT CYGWIN)
			subdirs(zlib)
			set(HAVE_LIBZ 1)
			set(FLTK_ZLIB_LIBRARIES fltk_zlib)
			set(FLTK_LIBRARIES "${FLTK_LIBRARIES};fltk_zlib")
			include_directories("${CMAKE_CURRENT_SOURCE_DIR}/zlib")
		endif()
	else()
		message(FATAL_ERROR "Cannot find Z library.")
	endif()
endif()

# Set an option to build the jpeg library or not
#option(FLTK_USE_SYSTEM_JPEG "Use's system jpeg" OFF)
if(JPEG_FOUND)
    set(CMAKE_TEST_SPECIAL_LIBRARIES ${JPEG_LIBRARIES})
    set(FLTK_JPEG_LIBRARIES ${JPEG_LIBRARIES})
    PERFORM_CMAKE_TEST(CMake/PlatformTests.cxx HAVE_LIBJPEG)
else()
  	mark_as_advanced(JPEG_INCLUDE_DIR)
	mark_as_advanced(JPEG_LIBRARY)
	if(WIN32)
		if(NOT CYGWIN)
			subdirs(jpeg)
			set(HAVE_LIBJPEG 1)
			set(FLTK_JPEG_LIBRARIES fltk_jpeg)
			set(FLTK_LIBRARIES "${FLTK_LIBRARIES};fltk_jpeg")
			include_directories("${CMAKE_CURRENT_SOURCE_DIR}/jpeg")
		endif()
	else()
		message(FATAL_ERROR "Cannot find JPEG library.")
	endif()
endif()

# Set an option to build the png library or not
# option(FLTK_USE_SYSTEM_PNG "Use's system png" OFF)
if(PNG_FOUND)
    set(CMAKE_TEST_SPECIAL_LIBRARIES ${PNG_LIBRARIES})
    set(FLTK_PNG_LIBRARIES ${PNG_LIBRARIES})
    PERFORM_CMAKE_TEST(CMake/PlatformTests.cxx HAVE_LIBPNG)
    PERFORM_CMAKE_TEST(CMake/PlatformTests.cxx HAVE_PNG_GET_VALID)
    PERFORM_CMAKE_TEST(CMake/PlatformTests.cxx HAVE_PNG_SET_TRNS_TO_ALPHA)
    set(HAVE_PNG_H 1)
else()
	mark_as_advanced(PNG_INCLUDE_DIR)
	mark_as_advanced(PNG_LIBRARY)
  	if(WIN32)
		if(NOT CYGWIN)
			subdirs(png)
			set(HAVE_LIBPNG 1)
			set(HAVE_PNG_H 1)
			set(FLTK_PNG_LIBRARIES fltk_png)
			set(FLTK_LIBRARIES "${FLTK_LIBRARIES};fltk_png")
			include_directories("${CMAKE_CURRENT_SOURCE_DIR}/png")
		endif()
	else()
		message(FATAL_ERROR "Cannot find PNG library.")
	endif()
endif()

set(FLTK_DATADIR "${CMAKE_INSTALL_PREFIX}/share/FLTK")
set(FLTK_DOCDIR  "${CMAKE_INSTALL_PREFIX}/share/doc/FLTK")

# Write out configuration header file
# OTB MOdifications : conflict name file with the OTB/Utilities/ITK/Utilities/nifti/znzlib/config.h
configure_file(${FLTK_SOURCE_DIR}/configh.cmake.in
#  ${FLTK_BINARY_DIR}/config.h)
  ${FLTK_BINARY_DIR}/fltk-config.h)

# On unix create symlinks for backward compatibility
set(FLTK_CREATE_SYMLINKS 1)
if(WIN32)
  if(NOT UNIX)
    set(FLTK_CREATE_SYMLINKS 0)
  endif()
endif()

macro(SAFE_CREATE_SYMLINK SOURCE DESTINATION)
  if(EXISTS "${DESTINATION}")
  else()
    message(STATUS "Create symlink from: \"${SOURCE}\" to \"${DESTINATION}\"")
    # The quoting here does seems unnatural, but this is to prevent bug in CMake
    exec_program(ln ARGS
      "-s \"${SOURCE}\" \"${DESTINATION}\"" OUTPUT_VARIABLE ln_output
      RETURN_VALUE ln_retval)
    if("${ln_retval}" GREATER 0)
      message(FATAL_ERROR "Problem creatin symlink from \"${SOURCE}\" to \"${DESTINATION}\":\n${ln_output}")
    endif()
  endif()
endmacro()

# If this is out-of-source build, then copy FL directory
file(GLOB FLTK_HEADER_FILES "${FLTK_SOURCE_DIR}/FL/*.[hHr]")
foreach(file ${FLTK_HEADER_FILES})
  get_filename_component(ext "${file}" EXT)
  get_filename_component(namewe "${file}" NAME_WE)
  get_filename_component(name "${file}" NAME)
  string(COMPARE EQUAL "${ext}" ".h" lower_case_h)
  string(COMPARE EQUAL "${ext}" ".H" upper_case_h)
  if(lower_case_h OR upper_case_h)
    set(outfile_h "${FLTK_BINARY_DIR}/FL/${namewe}.h")
    set(outfile_H "${FLTK_BINARY_DIR}/FL/${namewe}.H")
    configure_file("${file}" "${outfile_H}" COPYONLY IMMEDIATE)
    configure_file("${file}" "${outfile_h}" COPYONLY IMMEDIATE)
#    if(FLTK_CREATE_SYMLINKS)
#      SAFE_CREATE_SYMLINK("${outfile_H}" "${outfile_h}")
#    endif()
  else()
    string(COMPARE EQUAL "${ext}" ".r" mac_resource_file)
    if(mac_resource_file)
      set(outfile "${FLTK_BINARY_DIR}/FL/${name}")
      configure_file("${file}" "${outfile}" COPYONLY IMMEDIATE)
    endif()
  endif()
endforeach()

if(FLTK_CREATE_SYMLINKS)
  SAFE_CREATE_SYMLINK(
    "${FLTK_BINARY_DIR}/FL"
    "${FLTK_BINARY_DIR}/Fl")

  SAFE_CREATE_SYMLINK(
    "${FLTK_BINARY_DIR}/FL/gl.H"
    "${FLTK_BINARY_DIR}/FL/gl.h")

  # Create the symlinks
  file(READ ${FLTK_SOURCE_DIR}/fltk.list.in SYMLINKSFILE)
  string(REGEX MATCHALL "(l 0000 root sys .includedir/)([^(\n)])+"
  SYMLINKS ${SYMLINKSFILE})
  foreach(var ${SYMLINKS} )
    if("${var}" MATCHES ".H")
      string(REGEX MATCH "(/F)([^(\n)])+" tmp ${var})
      string(REGEX MATCH "(/F)([^( )])+" in ${tmp})
      string(REGEX MATCH "( )([^(\n)])+" out ${tmp})
      string(REGEX REPLACE "( )" "" out ${out})
      SAFE_CREATE_SYMLINK("${FLTK_BINARY_DIR}/FL/${out}" "${FLTK_BINARY_DIR}/${in}")
    endif()
  endforeach()
endif()

# Set the fluid executable path
utility_source(FLUID_COMMAND fluid fluid fluid.cxx)
set(FLUID_COMMAND "${FLUID_COMMAND}" CACHE INTERNAL "" FORCE)

# Include header files in fltk binary tree
include_directories(BEFORE ${FLTK_BINARY_DIR})

# Do the build of fltk libraries and fluid
subdirs(src)
subdirs(fluid)


# Set an option to build the examples and testing
#option(BUILD_EXAMPLES "Build the tests" ON)
#if(BUILD_EXAMPLES)
#  subdirs(test)
#endif()

#option(BUILD_TESTING "Build testing of FLTK" ON)
#if(BUILD_TESTING)
#  enable_testing()
#  include(Dart)
#endif()

#-----------------------------------------------------------------------------
# Help outside projects build FLTK projects.
include(CMakeExportBuildSettings)
export_library_dependencies(${FLTK_BINARY_DIR}/FLTKLibraryDepends.cmake)
CMAKE_EXPORT_BUILD_SETTINGS(${FLTK_BINARY_DIR}/FLTKBuildSettings.cmake)
set(FL_MAJOR_VERSION "${FLTK_VERSION_MAJOR}")
set(FL_MINOR_VERSION "${FLTK_VERSION_MINOR}")
set(FL_PATCH_VERSION "${FLTK_VERSION_PATCH}")

set(CFLAGS "${CMAKE_C_FLAGS}")
set(CXXFLAGS "${CMAKE_CXX_FLAGS}")
set(CC "${CMAKE_C_COMPILER}")
set(CXX "${CMAKE_CXX_COMPILER}")

# For build tree usage
set(FLTK_FLUID_COMMAND "${FLUID_COMMAND}")
set(FLTK_LIBRARY_DEPENDS_FILE ${FLTK_BINARY_DIR}/FLTKLibraryDepends.cmake)
set(FLTK_EXECUTABLE_DIRS ${EXECUTABLE_OUTPUT_PATH})
set(FLTK_LIBRARY_DIRS ${LIBRARY_OUTPUT_PATH})
set(FLTK_USE_FILE ${FLTK_SOURCE_DIR}/CMake/FLTKUse.cmake)
set(FLTK_INCLUDE_DIRS "${FLTK_BINARY_DIR}/")
set(FLTK_BUILD_SETTINGS_FILE ${FLTK_BINARY_DIR}/FLTKBuildSettings.cmake)
set(prefix "${FLTK_BINARY_DIR}")
set(exec_prefix "${prefix}")
set(exec_prefix_set "no")
set(bindir "${prefix}/bin")
set(includedir "${prefix}")
set(libdir "${prefix}/bin")
set(srcdir "${FLTK_SOURCE_DIR}")

configure_file("${FLTK_SOURCE_DIR}/CMake/FLTKConfig.cmake.in"
  "${FLTK_BINARY_DIR}/FLTKConfig.cmake" @ONLY IMMEDIATE)
configure_file("${FLTK_SOURCE_DIR}/fltk-config.in"
  "${FLTK_BINARY_DIR}/fltk-config" @ONLY IMMEDIATE)

# For installed tree usage
set(FLTK_FLUID_COMMAND "${CMAKE_INSTALL_PREFIX}/bin/fluid")
set(FLTK_LIBRARY_DEPENDS_FILE ${CMAKE_INSTALL_PREFIX}/lib/FLTK-${FLTK_VERSION}/FLTKLibraryDepends.cmake)
set(FLTK_EXECUTABLE_DIRS "${CMAKE_INSTALL_PREFIX}/bin")
set(FLTK_LIBRARY_DIRS "${CMAKE_INSTALL_PREFIX}/lib")
set(FLTK_USE_FILE "${CMAKE_INSTALL_PREFIX}/lib/FLTK-${FLTK_VERSION}/FLTKUse.cmake")
set(FLTK_INCLUDE_DIRS "${CMAKE_INSTALL_PREFIX}/")
set(FLTK_BUILD_SETTINGS_FILE ${CMAKE_INSTALL_PREFIX}/lib/FLTK-${FLTK_VERSION}/FLTKBuildSettings.cmake)
set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "${prefix}")
set(exec_prefix_set "no")
set(bindir "${prefix}/bin")
set(includedir "${prefix}")
set(libdir "${prefix}/lib")
set(srcdir ".")

configure_file("${FLTK_SOURCE_DIR}/CMake/FLTKConfig.cmake.in"
  "${FLTK_BINARY_DIR}/CMake/FLTKConfig.cmake" @ONLY IMMEDIATE)
configure_file("${FLTK_SOURCE_DIR}/fltk-config.in"
  "${FLTK_BINARY_DIR}/CMake/fltk-config" @ONLY IMMEDIATE)

# OTB Modifications
#install_files(/include/FL FILES ${FLTK_HEADER_FILES})
#install_files(/include/otb/Utilities/FLTK/FL FILES ${FLTK_HEADER_FILES})
install(FILES ${FLTK_HEADER_FILES}
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/FLTK/FL
    COMPONENT Development)

#install_files(/lib/FLTK-${FLTK_VERSION} FILES "${FLTK_SOURCE_DIR}/CMake/FLTKUse.cmake")
#install_files(/lib/FLTK-${FLTK_VERSION} FILES "${FLTK_BINARY_DIR}/FLTKBuildSettings.cmake")
#install_files(/lib/FLTK-${FLTK_VERSION} FILES "${FLTK_BINARY_DIR}/FLTKLibraryDepends.cmake")
#install_files(/lib/FLTK-${FLTK_VERSION} FILES "${FLTK_BINARY_DIR}/CMake/FLTKConfig.cmake")
