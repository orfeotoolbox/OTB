# exact C++ implementation of lowe's sift program
# author: zerofrog(@gmail.com), Sep 2008
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# Lesser GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
cmake_minimum_required (VERSION 2.4.7)
project (otbsiftfast)
set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}")

# Differences between CMake 2.4 and 2.6
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" GREATER 2.4)
   message(STATUS "Using cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" )
   # some developers may be using an cmake cvs version which didn't have set_property() yet
   # Tell them that a more recent version is required.
   if(NOT COMMAND SET_PROPERTY)
      message(FATAL_ERROR "You are using an old version of CMake from cvs, please update to CMake >= 2.6.0 or cvs at least from Feb 20th, 2008")
   endif(NOT COMMAND SET_PROPERTY)

   # CMP0003: add the link paths to the link command as with cmake 2.4
   cmake_policy(SET CMP0003 OLD)
endif()

# Add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

include(CheckCXXSourceRuns)

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  # TRY  COMPILE libsiftfast.cpp (depend of gcc version)
    SET(IS_SIFTFAST_COMPILE 0)
    TRY_COMPILE(IS_SIFTFAST_COMPILE
              ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/test_try_compile_libsiftfast.cpp
              OUTPUT_VARIABLE OUTPUT)
    IF(IS_SIFTFAST_COMPILE)
        MESSAGE(STATUS "Try to compile libsiftfast.cpp -- yes.")
    ELSE(IS_SIFTFAST_COMPILE)
        MESSAGE(STATUS "Try to compile libsiftfast.cpp -- no.")
        TRY_COMPILE(IS_SIFTFAST_COMPILE2
              ${CMAKE_CURRENT_BINARY_DIR}
              ${CMAKE_CURRENT_SOURCE_DIR}/test_try_compile_libsiftfast.cpp
              COMPILE_DEFINITIONS -DOTB_DISABLE_FAST_FUNCTIONS
              OUTPUT_VARIABLE OUTPUT)
        IF(IS_SIFTFAST_COMPILE2)
            MESSAGE(STATUS "Try to compile libsiftfast.cpp with disabling fast functions (add CFLAGS -DOTB_DISABLE_FAST_FUNCTIONS) -- yes.")
        ELSE(IS_SIFTFAST_COMPILE2)
            MESSAGE(STATUS "Try to compile libsiftfast.cpp with disabling fast functions (add CFLAGS -DOTB_DISABLE_FAST_FUNCTIONS) -- no.")
            MESSAGE(FATAL_ERROR "Impossible to compile libsiftfast. Contact OTB development team")
        ENDIF(IS_SIFTFAST_COMPILE2)
        SET(OTB_DISABLE_FAST_FUNCTIONS_VALUE 1)
    ELSE(NOT IS_SIFTFAST_COMPILE)
        MESSAGE(STATUS "Try to compile libsiftfast.cpp done.")
    ENDIF(IS_SIFTFAST_COMPILE)

  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g")
  add_definitions(" -Wall -fPIC ")

  # check for x86-64 system
  check_cxx_source_runs("
  int main()
  {
    int a = 0;
    int*pa = &a;
    asm(\".intel_syntax\\\\n\"
	\"mov %%rax, %0\\\\n\"
    \"mov %%eax, [%%rax]\\\\n\"
    \".att_syntax\\\\n\"
    : : \"r\"(pa) : \"%rax\");
    return 0;
  }"
  IS_X86_64)

  if( IS_X86_64 )
    add_definitions("-D__x86_64__")
  endif()
else()
  set(IS_X86_64 0)
endif()

IF(OTB_DISABLE_FAST_FUNCTIONS_VALUE)
    ADD_DEFINITIONS(-DOTB_DISABLE_FAST_FUNCTIONS)
ENDIF(OTB_DISABLE_FAST_FUNCTIONS_VALUE)


include(CheckIncludeFile)
include(CheckLibraryExists)
include(CheckCXXSourceRuns)
include(CheckCXXCompilerFlag)

if( UNIX OR CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  set(STDC_LIBRARY stdc++)
else()
  set(STDC_LIBRARY)
endif()

# check for SSE extensions
if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  set(SSE_FLAGS)

  set(CMAKE_REQUIRED_FLAGS "-msse2")
  check_cxx_source_runs("
    #include <emmintrin.h>

    int main()
    {
        __m128d a, b;
        double vals[2] = {0};
        a = _mm_loadu_pd(vals);
        b = _mm_add_pd(a,a);
        _mm_storeu_pd(vals,b);
        return 0;
     }"
     HAS_SSE2_EXTENSIONS)

  set(CMAKE_REQUIRED_FLAGS "-msse")
  check_cxx_source_runs("
    #include <xmmintrin.h>
    int main()
    {
        __m128 a, b;
        float vals[4] = {0};
        a = _mm_loadu_ps(vals);
        b = a;
        b = _mm_add_ps(a,b);
        _mm_storeu_ps(vals,b);
        return 0;
    }"
    HAS_SSE_EXTENSIONS)

  set(CMAKE_REQUIRED_FLAGS)

  if(HAS_SSE2_EXTENSIONS)
    message(STATUS "Using SSE2 extensions")
    set(SSE_FLAGS "-msse2 -mfpmath=sse")
  elseif(HAS_SSE_EXTENSIONS)
    message(STATUS "Using SSE extensions")
    set(SSE_FLAGS "-msse -mfpmath=sse")
  endif()

  add_definitions(${SSE_FLAGS})
elseif(MSVC)
  check_cxx_source_runs("
    #include <emmintrin.h>

    int main()
    {
        __m128d a, b;
        double vals[2] = {0};
        a = _mm_loadu_pd(vals);
        b = _mm_add_pd(a,a);
        _mm_storeu_pd(vals,b);
        return 0;
     }"
     HAS_SSE2_EXTENSIONS)
  if( HAS_SSE2_EXTENSIONS )
    message(STATUS "Using SSE2 extensions")
    add_definitions( "/arch:SSE2 /fp:fast -D__SSE__ -D__SSE2__" )
  endif()
endif()

#set(Boost_ADDITIONAL_VERSIONS "1.40" "1.39" "1.38" "1.37.0" "1.37" "1.35.0" "1.34.1" "1.34.0" "1.34" "1.33.1" "1.33.0" "1.33")
#if( NOT $ENV{BOOST_INCLUDEDIR} STREQUAL "" )
#  set(Boost_INCLUDE_DIR $ENV{BOOST_INCLUDEDIR})
#endif()
#if( NOT $ENV{BOOST_LIBRARYDIR} STREQUAL "" )
#  set(Boost_LIBRARY_DIRS $ENV{BOOST_LIBRARYDIR})
#endif()
#find_package(Boost COMPONENTS python)

#if( Boost_FOUND )
#  message(STATUS "found boost version: ${Boost_VERSION}")
#else()
#  message(STATUS "Could not find boost libraries!")
#endif()

add_library(otbsiftfast libsiftfast.cpp)

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
  target_link_libraries(otbsiftfast m stdc++)
endif()

set_target_properties(otbsiftfast PROPERTIES OUTPUT_NAME  siftfast CLEAN_DIRECT_OUTPUT 1)

# compile without depending on libsiftfast
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#set_target_properties(siftfast PROPERTIES COMPILE_FLAGS "-DDVPROFILE")
#target_link_libraries(siftfast libsiftfast)

# check for OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
  message(STATUS "Enabling OpenMP support")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
else()
  message(STATUS "Disabling OpenMP support")
endif()

#Install TARGET & FILES for otb-lib
IF(OTB_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(otbsiftfast PROPERTIES ${OTB_LIBRARY_PROPERTIES})
ENDIF(OTB_LIBRARY_PROPERTIES)

IF(NOT OTB_INSTALL_NO_LIBRARIES)
  INSTALL(TARGETS otbsiftfast
    RUNTIME DESTINATION ${OTB_INSTALL_BIN_DIR} COMPONENT RuntimeLibraries
    LIBRARY DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT RuntimeLibraries
    ARCHIVE DESTINATION ${OTB_INSTALL_LIB_DIR} COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_LIBRARIES)

IF(NOT OTB_INSTALL_NO_DEVELOPMENT)
  INSTALL(FILES siftfast.h 
    DESTINATION ${OTB_INSTALL_INCLUDE_DIR}/Utilities/otbsiftfast
    COMPONENT Development)
ENDIF(NOT OTB_INSTALL_NO_DEVELOPMENT)