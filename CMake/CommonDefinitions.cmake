# This file contains the common build definitions and flags used by default when building all targets

IF(WIN32)
  # do not build shared lib under windows
  SET(BUILD_SHARED_LIBS OFF)
  MESSAGE(STATUS "Disabling build shared lib option on windows like OS.")
ELSE(WIN32)
  # on other systems, leave the choice to the user
  OPTION(BUILD_SHARED_LIBS "Build OTB with shared libraries." ON)
ENDIF(WIN32)

SET(OTB_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

# On Visual Studio 8 MS deprecated C. This removes all 1.276E1265 security
# warnings
IF(WIN32)
    IF(NOT CYGWIN)
      IF(NOT MINGW)
        IF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
          ADD_DEFINITIONS(
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
        ENDIF(NOT ITK_ENABLE_VISUAL_STUDIO_DEPRECATED_C_WARNINGS)
        
        IF (MSVC)
          ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
          ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_WARNING)
        ENDIF(MSVC)
        
      ENDIF(NOT MINGW)
    ENDIF(NOT CYGWIN)
ENDIF(WIN32)

# __CYGWIN__ must be define for ossim (ex: ossimCommon.h, isnan definition, .. )
#IF(CYGWIN)
#  ADD_DEFINITIONS(-D__CYGWIN__)
#ENDIF(CYGWIN)


# Large File Support (> 4Go)
ADD_DEFINITIONS(
    -D_LARGEFILE_SOURCE
    -D_FILE_OFFSET_BITS=64
    -D_LARGEFILE64_SOURCE
    -D_LARGE_FILES
    )

# Pedantic mode
SET(OTB_BUILD_PEDANTIC OFF CACHE BOOL
    "Choose compilation in pedantic or relaxed mode")
MARK_AS_ADVANCED(OTB_BUILD_PEDANTIC)

IF(OTB_BUILD_PEDANTIC)
    IF(WIN32)
      IF (MSVC)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
      ENDIF(MSVC)
    ELSE(WIN32)
      IF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
      ENDIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    ENDIF(WIN32)
ENDIF(OTB_BUILD_PEDANTIC)


# Full compilation warning mode
OPTION(OTB_COMPILE_WITH_FULL_WARNING "OTB developers : Compilation with FULL warning (use only for otb developers)." OFF)
MARK_AS_ADVANCED(OTB_COMPILE_WITH_FULL_WARNING)
IF(OTB_COMPILE_WITH_FULL_WARNING)
  IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(OTB_FULL_WARNING_LIST_PARAMETERS "-Wall -Wunused  -Wunused-function  -Wunused-label  -Wunused-parameter -Wunused-value -Wunused-variable -Wuninitialized -Wsign-compare -Wparentheses -Wunknown-pragmas -Wswitch" CACHE STRING "List of compilation parameters.")
    MARK_AS_ADVANCED(OTB_FULL_WARNING_LIST_PARAMETERS)
  ENDIF(CMAKE_COMPILER_IS_GNUCXX)
ENDIF(OTB_COMPILE_WITH_FULL_WARNING)


