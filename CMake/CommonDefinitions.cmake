# This file contains the common build definitions and flags used by default when building all targets

if(WIN32)
  # do not build shared lib under windows
  set(BUILD_SHARED_LIBS OFF)
  message(STATUS "Disabling build shared lib option on windows like OS.")
  
  # Since CMake 2.8.11, the size of the stack is not modified by CMake on windows platform, it uses the default size: with visual compiler it is 1Mbyte which is to lower for us (thanks to 6S code).

  if (${CMAKE_VERSION} VERSION_GREATER "2.8.10.2")
    if("${CMAKE_EXE_LINKER_FLAGS}" MATCHES "/STACK:[0-9]+")
        message(STATUS "The size of the stack is already defined, so we dont't modified it.")
    else()
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:10000000" CACHE STRING "Flags used by the linker" FORCE)
        message(STATUS "Increase the size of the stack to 10Mbytes.")
    endif()
  endif()
 
else()
  # on other systems, leave the choice to the user
  option(BUILD_SHARED_LIBS "Build OTB with shared libraries." ON)
endif()

set(OTB_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings
if(WIN32)
    if(NOT CYGWIN)
      if(NOT MINGW)
        if(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
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

        if(MSVC)
          add_definitions(-D_CRT_SECURE_NO_WARNINGS)
          add_definitions(-D_CRT_NONSTDC_NO_WARNING)
        endif()

      endif()
    endif()
endif()

# __CYGWIN__ must be define for ossim (ex: ossimCommon.h, isnan definition, .. )
#if(CYGWIN)
#  add_definitions(-D__CYGWIN__)
#endif()


# Large File Support (> 4Go)
include(TestLargeFiles)
OTB_TEST_LARGE_FILES(OTB_HAS_LARGEFILES)
if (_FILE_OFFSET_BITS)
  add_definitions(-D_FILE_OFFSET_BITS=${_FILE_OFFSET_BITS})
endif()
if (_LARGEFILE_SOURCE)
  add_definitions(-D_LARGEFILE_SOURCE)
endif()
if (_LARGEFILE64_SOURCE)
  add_definitions(-D_LARGEFILE64_SOURCE)
endif()
if (_LARGE_FILES)
  add_definitions(-D_LARGE_FILES)
endif()

# Pedantic mode
set(OTB_BUILD_PEDANTIC OFF CACHE BOOL
    "Choose compilation in pedantic or relaxed mode")
mark_as_advanced(OTB_BUILD_PEDANTIC)

if(OTB_BUILD_PEDANTIC)
    if(WIN32)
      if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
      endif()
    else()
      if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
      endif()
    endif()
endif()


# Full compilation warning mode
option(OTB_COMPILE_WITH_FULL_WARNING "OTB developers : Compilation with FULL warning (use only for otb developers)." OFF)
mark_as_advanced(OTB_COMPILE_WITH_FULL_WARNING)
if(OTB_COMPILE_WITH_FULL_WARNING)
  if(CMAKE_COMPILER_IS_GNUCXX)
    set(OTB_FULL_WARNING_LIST_PARAMETERS "-Wall -Wunused  -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value -Wunused-variable -Wuninitialized -Wsign-compare -Wparentheses -Wunknown-pragmas -Wswitch" CACHE STRING "List of compilation parameters.")
    mark_as_advanced(OTB_FULL_WARNING_LIST_PARAMETERS)
  endif()
endif()


