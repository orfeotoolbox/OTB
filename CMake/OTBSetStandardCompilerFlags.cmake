#
# Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# trict-null-sentinel Check the set of common warning flags supported by C and C++ compilers
# check_compiler_warning_flags(<c_flags_var> <cxx_flags_var>)
#  <c_flags_var> - variable to store valid C warning flags
#  <cxx_flags_var> - variable to store valid CXX warning flags
# This internally calls the check_c_compiler_flag and check_cxx_compiler_flag macros.


# To create a portable build system, it is best to not
# test for platforms, but to test for features.
#
# Instead of testing "if Windows then do this", test for
# "if the -Wno-invalid-offsetof flag works then use it".
#
# Typical use of this module is:
#
#  include(CheckCompilerWarningFlags)
#  check_compiler_warning_flags(C_WARNING_FLAGS CXX_WARNING_FLAGS)
#  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_WARNING_FLAGS}")
#  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNING_FLAGS}")


include(OTB_CheckCCompilerFlag)
include(OTB_CheckCXXCompilerFlag)

macro( set_debug_flags )
  string( TOUPPER "${CMAKE_BUILD_TYPE}" MODE )

  if( "${MODE}" STREQUAL "DEBUG" )
    message( STATUS "Adding -DOTB_DEBUG" )

    set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DOTB_DEBUG" )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DOTB_DEBUG" )
  endif()
endmacro()

function(check_c_compiler_warning_flags c_flag_var)
  set(local_c_flags "")
  set(flag_list "${ARGN}")
  foreach(flag IN LISTS flag_list)
    OTB_CHECK_C_COMPILER_FLAG(${flag} C_HAS_WARNING${flag})
    if(${C_HAS_WARNING${flag}})
      set(local_c_flags "${local_c_flags} ${flag}")
    endif()
  endforeach()
  set(${c_flag_var} "${local_c_flags}" PARENT_SCOPE)
endfunction()


function(check_cxx_compiler_warning_flags cxx_flag_var)
  set(local_cxx_flags "")
  set(flag_list "${ARGN}")
  foreach(flag IN LISTS flag_list)
    OTB_CHECK_CXX_COMPILER_FLAG(${flag} CXX_HAS_WARNING${flag})
    if(${CXX_HAS_WARNING${flag}})
      set(local_cxx_flags "${local_cxx_flags} ${flag}")
    endif()
  endforeach()
  set(${cxx_flag_var} "${local_cxx_flags}" PARENT_SCOPE)
endfunction()


function(check_compiler_warning_flags c_warning_flags_var cxx_warning_flags_var)
  set(${c_warning_flags_var} "" PARENT_SCOPE)
  set(${cxx_warning_flags_var} "" PARENT_SCOPE)

  # Check this list on C compiler only
  set(c_flags
    -Wno-uninitialized
    -Wno-unused-parameter
  )

  ## On windows, the most verbose compiler options
  ## is reporting 1000's of wanings in windows
  ## header files, for now, limit the number of
  ## warnings to level 3
  if( MSVC )
    set(VerboseWarningsFlag -W3 )
    ## A better solution would be to use -Wall,
    ## and then disable warnings one by one
    ## set(VerboseWarningsFlag -Wall -wd4820 -wd4682 )
  else()
    ## with Intel compiler, the -Wall compiler options
    ## is reporting 1000's of remarks of trivial items
    ## that will only slow day-to-day operations
    ## specify -w2 to restrict to only warnings and errors
    if (${CMAKE_C_COMPILER} MATCHES "icc.*$")
      set(USING_INTEL_ICC_COMPILER TRUE)
    endif()
    if (${CMAKE_CXX_COMPILER} MATCHES "icpc.*$")
      set(USING_INTEL_ICC_COMPILER TRUE)
    endif()
    if(USING_INTEL_ICC_COMPILER)
      # NOTE -w2 is close to gcc's -Wall warning level, -w5 is intels -Wall warning level, and it is too verbose.
      set(VerboseWarningsFlag -w2 -wd1268 -wd981 -wd383 -wd1418 -wd1419 -wd2259 -wd1572 -wd424 )
      #-wd424  #Needed for Intel compilers with remarki  #424: extra ";" ignored
      #-wd383  #Needed for Intel compilers with remark   #383: value copied to temporary, reference to temporary used
      #-wd981  #Needed for Intel compilers with remark   #981: operands are evaluated in unspecified order
      #-wd1418 #Needed for Intel compilers with remark  #1418: external function definition with no prior declaration
      #-wd1419 #Needed for Intel compilers with remark  #1419: external declaration in primary source file
      #-wd1572 #Needed for Intel compilers with remark  #1572: floating-point equality and inequality comparisons are unreliable
      #-wd2259 #Needed for Intel compilers with remark  #2259: non-pointer conversion from "otb::SizeValueType={unsigned long}" to "double" may lose significant bits
      #-wd1268 #Needed for Intel compilers with warning #1268: support for exported templates is disabled
    else()
      set(VerboseWarningsFlag -Wall )
    endif ()
  endif()

  # Check this list on both C and C++ compilers
  set(c_and_cxx_flags
    ${VerboseWarningsFlag}
    -Wno-long-double        #Needed on APPLE
    -Wcast-align
    -Wdisabled-optimization
    -Wextra
    -Wformat=2
    -Winvalid-pch
    -Wno-format-nonliteral
    -Wpointer-arith
    -Wshadow
    -Wunused
    -Wwrite-strings
    -funit-at-a-time
    -Wno-strict-overflow
  )

  # Check this list on C++ compiler only
  set(cxx_flags
    -Wno-deprecated
    -Wno-invalid-offsetof
    -Woverloaded-virtual
    -Wstrict-null-sentinel
  )
##-Wno-c++0x-static-nonintegral-init
    ## Clang compiler likes to warn about this feature that is technically only in
    ## c++0x, but works on many compilers, and if it fails, then alternate methods are used

  check_c_compiler_warning_flags(CMAKE_C_WARNING_FLAGS ${c_flags} ${c_and_cxx_flags})
  check_cxx_compiler_warning_flags(CMAKE_CXX_WARNING_FLAGS ${c_and_cxx_flags} ${cxx_flags})

  set(${c_warning_flags_var} "${CMAKE_C_WARNING_FLAGS}" PARENT_SCOPE)
  set(${cxx_warning_flags_var} "${CMAKE_CXX_WARNING_FLAGS}" PARENT_SCOPE)
endfunction()


macro(check_compiler_platform_flags)
  # On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
  # warnings
  if(WIN32)
       if(NOT MINGW)
         if(NOT OTB_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
           add_definitions(
             -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
             -D_CRT_IS_WCTYPE_NO_DEPRECATE
             -D_CRT_MANAGED_FP_NO_DEPRECATE
             -D_CRT_NONSTDC_NO_DEPRECATE
             -D_CRT_SECURE_NO_DEPRECATE
             -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
             -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
             -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
             -D_CRT_VCCLRIT_NO_DEPRECATE
             -D_SCL_SECURE_NO_DEPRECATE
             )
         endif()
         # With MS compilers on Win64, we need the /bigobj switch, else generated
         # code results in objects with number of sections exceeding object file
         # format.
         # see http://msdn.microsoft.com/en-us/library/ms173499.aspx
         if(MSVC_VERSION GREATER 1310)
           set(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} /bigobj")
         endif()
       endif()
  endif()

  if(WIN32)
    # Some libraries (e.g. vxl libs) have no dllexport markup, so we can
    # build full shared libraries only with the GNU toolchain. For non
    # gnu compilers on windows, only Common is shared.  This allows for
    # plugin type applications to use a dll for OTBCommon which will contain
    # the static for Modified time.
    if(CMAKE_COMPILER_IS_GNUCXX)
      # CMake adds --enable-all-exports on Cygwin (since Cygwin is
      # supposed to be UNIX-like), but we need to add it explicitly for
      # a native windows build with the MinGW tools.
      if(MINGW)
        set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS
          "-shared -Wl,--export-all-symbols -Wl,--enable-auto-import")
        set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS
          "-shared -Wl,--export-all-symbols -Wl,--enable-auto-import")
        set(CMAKE_EXE_LINKER_FLAGS "-Wl,--enable-auto-import")
      endif()
    else()
      if(MSVC)
        set(OTB_REQUIRED_LINK_FLAGS "${OTB_REQUIRED_LINK_FLAGS} /MANIFEST:NO")
      endif()
      # if CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS is on, then
      # BUILD_SHARED_LIBS works as it would on other systems
      if(NOT CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS)
        if(BUILD_SHARED_LIBS)
          set(OTB_LIBRARY_BUILD_TYPE "SHARED")
        else()
          set(OTB_LIBRARY_BUILD_TYPE "STATIC")
        endif()
        # turn off BUILD_SHARED_LIBS as OTB_LIBRARY_BUILD_TYPE
        # is used on the libraries that have markup.
        set(BUILD_SHARED_LIBS OFF)
      endif()
    endif()
  endif()
  #-----------------------------------------------------------------------------
  #OTB requires special compiler flags on some platforms.
  if(CMAKE_COMPILER_IS_GNUCXX)
    # GCC's -Warray-bounds has been shown to throw false positives with -O3 on 4.8.
    if(UNIX AND (
      ("${CMAKE_CXX_COMPILER_VERSION}" VERSION_EQUAL "4.8") OR
      ("${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER "4.8" AND "${CMAKE_CXX_COMPILER_VERSION}" VERSION_LESS "4.9") ))
      set(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -Wno-array-bounds")
    endif()
  endif()

  if(APPLE)
    option(OTB_USE_64BITS_APPLE_TRUNCATION_WARNING "Turn on warnings on 64bits to 32bits truncations." OFF)
    mark_as_advanced(OTB_USE_64BITS_APPLE_TRUNCATION_WARNING)
  endif()

  #-----------------------------------------------------------------------------

  # for the gnu compiler a -D_PTHREADS is needed on sun
  # for the native compiler a -mt flag is needed on the sun
  if(CMAKE_SYSTEM MATCHES "SunOS.*")
    if(CMAKE_COMPILER_IS_GNUCXX)
      set(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -D_PTHREADS")
      set(OTB_REQUIRED_LINK_FLAGS "${OTB_REQUIRED_LINK_FLAGS} -lrt")
    else()
      set(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -mt")
      set(OTB_REQUIRED_C_FLAGS "${OTB_REQUIRED_C_FLAGS} -mt")
    endif()
    # Add flags for the SUN compiler to provide all the methods for std::allocator.
    #
    CHECK_CXX_SOURCE_COMPILES("-features=no%anachronisms" SUN_COMPILER)
    if(SUN_COMPILER)
      CHECK_CXX_SOURCE_COMPILES("-library=stlport4" SUN_COMPILER_HAS_STL_PORT_4)
      if(SUN_COMPILER_HAS_STL_PORT_4)
        set(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -library=stlport4")
      endif()
     endif()
  endif()

  # mingw thread support
  if(MINGW)
    set(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} -mthreads")
    set(OTB_REQUIRED_C_FLAGS "${OTB_REQUIRED_C_FLAGS} -mthreads")
    set(OTB_REQUIRED_LINK_FLAGS "${OTB_REQUIRED_LINK_FLAGS} -mthreads")
  endif()

  # check for OpenMP
  if(OTB_USE_OPENMP)
    find_package(OpenMP QUIET)
  endif()
  if(OPENMP_FOUND)
    message(STATUS "Enabling OpenMP support")
    set(OTB_REQUIRED_C_FLAGS "${OTB_REQUIRED_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(OTB_REQUIRED_LINK_FLAGS "${OTB_REQUIRED_LINK_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  else()
    message(STATUS "Disabling OpenMP support")
  endif()

  set(OTB_SSE_FLAGS)
 if(OTB_USE_SSE_FLAGS)
    include(OTBCheckSSEFeatures)
    check_sse_features(OTB_SSE_FLAGS)
    if(OTB_SSE_FLAGS)
      message(STATUS "Adding OTB_SSE_FLAGS(${OTB_SSE_FLAGS}) to OTB_REQUIRED_CXX_FLAGS")
      set(OTB_REQUIRED_CXX_FLAGS "${OTB_REQUIRED_CXX_FLAGS} ${OTB_SSE_FLAGS}")
    endif()
  endif()

  #-----------------------------------------------------------------------------
  # The frename-registers option does not work due to a bug in the gnu compiler.
  # It must be removed or data errors will be produced and incorrect results
  # will be produced.  This is first documented in the gcc4 man page.
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(ALL_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS} ${CMAKE_MODULE_LINKER_FLAGS}" )
    #TODO: list(REMOVE_DUPLICATES ALL_FLAGS)
    separate_arguments(ALL_FLAGS)
    foreach(COMP_OPTION ${ALL_FLAGS})
      if("${COMP_OPTION}" STREQUAL "-frename-registers")
        message(FATAL_ERROR "-frename-registers causes runtime bugs.  It must be removed from your compilation options.")
      endif()
    endforeach()
  endif()

  #-----------------------------------------------------------------------------
  # Set the compiler-specific flag for disabling optimization.
  if(MSVC)
    set(OTB_CXX_DISABLE_OPTIMIZATION_FLAG "/Od")
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(GNU|Intel)$")
    set(OTB_CXX_DISABLE_OPTIMIZATION_FLAG "-O0")
  endif()
  if(DEFINED OTB_CXX_DISABLE_OPTIMIZATION_FLAG)
    CHECK_CXX_SOURCE_COMPILES(${OTB_CXX_DISABLE_OPTIMIZATION_FLAG} CXX_HAS_DISABLE_OPTIMIZATION_FLAG)
  endif()
endmacro()#End the platform check function

#-----------------------------------------------------------------------------
#Check the set of warning flags the compiler supports
check_compiler_warning_flags(C_WARNING_FLAGS CXX_WARNING_FLAGS)

# Append OTB warnings to the CMake flags.
# We do not set them in OTB_REQUIRED FLAGS because all project which
# use OTB don't require these flags .
set(CMAKE_C_FLAGS "${C_WARNING_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CXX_WARNING_FLAGS} ${CMAKE_CXX_FLAGS}")

set_debug_flags()

#-----------------------------------------------------------------------------
#Check the set of platform flags the compiler supports
check_compiler_platform_flags()


# Usage: set_linker_stack_size_flag(otbApplicationLauncherCommandLine 10000000)
# The above macro call will set LINK_FLAGS executable target named
# otbApplicationLauncherCommandLine to 10Mbytes
macro(set_linker_stack_size_flag exe_target requested_stack_size)
  # Since CMake 2.8.11, the size of the stack is not modified by CMake on
  # windows platform, it uses the default size: with visual compiler it is 1Mbyte
  # which is to lower for us (thanks to 6S code).
  if(MSVC)
    set_target_properties(${exe_target} PROPERTIES LINK_FLAGS "/STACK:${requested_stack_size}")
  elseif(MINGW)
    set_target_properties(${exe_target} PROPERTIES LINK_FLAGS "-Wl,--stack,${requested_stack_size}")
  endif()
endmacro()
