IF(WIN32)
    CMAKE_MINIMUM_REQUIRED(VERSION 2.4.6 FATAL_ERROR)
ELSE(WIN32)
    IF(APPLE)
        CMAKE_MINIMUM_REQUIRED(VERSION 2.4.6 FATAL_ERROR)
    ELSE(APPLE)
        CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0 FATAL_ERROR)
    ENDIF(APPLE)
ENDIF(WIN32)


PROJECT(OpenThreads)

# We have some custom .cmake scripts not in the official distribution.
# Maybe this can be used override existing behavior if needed?
SET(CMAKE_MODULE_PATH "${OpenThreads_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

################################################################################
# Installation stuff

# We want to build SONAMES shared librariess
SET(OPENTHREADS_SONAMES TRUE)

SET(CMAKE_DEBUG_POSTFIX  "d")
#SET(INSTALL_BINDIR OpenThreads/bin)
#SET(INSTALL_INCDIR OpenThreads/include)
#SET(INSTALL_LIBDIR OpenThreads/lib)
#SET(INSTALL_DOCDIR OpenThreads/docs)


SET(CMAKE_DEBUG_POSTFIX  "d")

SET(LIB_POSTFIX "")
IF(UNIX AND NOT WIN32 AND NOT APPLE)
  IF(CMAKE_SIZEOF_VOID_P MATCHES "8")
      SET(LIB_POSTFIX "64")
  ENDIF(CMAKE_SIZEOF_VOID_P MATCHES "8")
ENDIF(UNIX AND NOT WIN32 AND NOT APPLE)

################################################################################


# Add a source group for the include headers so they are seen in IDEs.
# Hmmm, this isn't working for me in Xcode.
# SOURCE_GROUP(
#     "Header Files"
#     FILES ${OpenThreads_PUBLIC_HEADERS}
# )

# Make the headers visible to everything
INCLUDE_DIRECTORIES(
    ${OpenThreads_SOURCE_DIR}/include
)

# This is mainly for Windows declspec, but other platforms know
# what to do with it.
ADD_DEFINITIONS(-DOPENTHREADS_EXPORTS)

# User Options
# OTB modifications
#OPTION(DYNAMIC_OPENTHREADS "Set to ON to build OpenThreads for dynamic linking.  Use OFF for static." ON)
#IF   (DYNAMIC_OPENTHREADS)
#    SET(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
#ELSE (DYNAMIC_OPENTHREADS)
#    SET(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
#ENDIF(DYNAMIC_OPENTHREADS)
IF( OTB_BUILD_SHARED_LIBS )
    SET(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
ELSE( OTB_BUILD_SHARED_LIBS )
    SET(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
ENDIF( OTB_BUILD_SHARED_LIBS )



# Use our modified version of FindThreads.cmake which has Sproc hacks.
FIND_PACKAGE(Threads)

# OTB Modifications
#SUBDIRS(src/OpenThreads examples)
SUBDIRS(src/OpenThreads)

# FIXME: Make optional install test programs
# FIXME: /bin directory not correct for Windows
# INSTALL_TARGETS(/bin OpenThreads)
 
#SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
# OTB modifications
# Alls bin or lib in OTB bin directory
#SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
SET(OUTPUT_BINDIR ${OTB_BINARY_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_BINDIR})
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_BINDIR})

#SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME})
#SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
SET(OUTPUT_LIBDIR ${OTB_BINARY_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_LIBDIR})
SET(LIBRARY_OUTPUT_PATH ${OUTPUT_LIBDIR})

################################################################################
# Create bin and lib directories if required

IF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   FILE(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin ${CMAKE_BINARY_DIR}/lib)
ENDIF("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")




###############################################################################
# This is for an advanced option to give aggressive warnings 
# under different compilers. If yours is not implemented, this option
# will not be made available.
IF(CMAKE_COMPILER_IS_GNUCXX)
    # To be complete, we might also do GNUCC flags, 
    # but everything here is C++ code.
    # FYI, if we do implement GNUCC, then -Wmissing-prototypes in another 
    # interesting C-specific flag.
    # Also, there is a bug in gcc 4.0. Under C++, -pedantic will create
    # errors instead of warnings for certain issues, including superfluous
    # semicolons and commas, and the use of long long. -fpermissive seems
    # to be the workaround. 
    SET(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS "-Wall -Wparentheses -Wformat=2 -Wno-long-long -Wno-import -pedantic -Wnewline-eof -Wreturn-type -Wmissing-braces -Wunknown-pragmas -Wunused -Wshadow -Woverloaded-virtual -fpermissive")
ELSE(CMAKE_COMPILER_IS_GNUCXX)
    IF(MSVC)
        # FIXME: What are good aggressive warning flags for Visual Studio?
        # And do we need to further subcase this for different versions of VS?
        # CMake variables: MSVC60, MSVC70, MSVC71, MSVC80, CMAKE_COMPILER_2005
        SET(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS "/Wall /W4")
    ELSE(MSVC)
        # CMake lacks an elseif, so other non-gcc, non-VS compilers need
        # to be listed below. If unhandled, OPENTHREADS_AGGRESSIVE_WARNING_FLAGS should 
        # remain unset.
    ENDIF(MSVC)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

# This part is for the CMake menu option to toggle the warnings on/off.
# This will only be made available if we set values for OPENTHREADS_AGGRESSIVE_WARNING_FLAGS.
IF(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS)
    OPTION(OPENTHREADS_USE_AGGRESSIVE_WARNINGS "Enable to activate aggressive warnings" OFF)
    MARK_AS_ADVANCED(OPENTHREADS_USE_AGGRESSIVE_WARNINGS)

    IF(OPENTHREADS_USE_AGGRESSIVE_WARNINGS)
        IF(NOT "${OLD_CMAKE_CXX_FLAGS_WAS_SET}")
            SET(OLD_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE INTERNAL "Old CXX flags")
            SET(OLD_CMAKE_CXX_FLAGS_WAS_SET 1 CACHE INTERNAL "Old CXX flags was set")
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENTHREADS_AGGRESSIVE_WARNING_FLAGS}" CACHE STRING "Flags used by the compiler during all build types." FORCE)
        ENDIF(NOT "${OLD_CMAKE_CXX_FLAGS_WAS_SET}")
    ELSE(OPENTHREADS_USE_AGGRESSIVE_WARNINGS)
        # FIXME: This will lose any changes made after OLD_CMAKE_CXX_FLAGS was 
        # set. The better way would be to parse the string and remove each
        # option explicitly.
        IF("${OLD_CMAKE_CXX_FLAGS_WAS_SET}")
            SET(CMAKE_CXX_FLAGS "${OLD_CMAKE_CXX_FLAGS}" CACHE STRING "Flags used by the compiler during all build types." FORCE)
            SET(OLD_CMAKE_CXX_FLAGS_WAS_SET 0 CACHE INTERNAL "Old CXX flags was set")
        ENDIF("${OLD_CMAKE_CXX_FLAGS_WAS_SET}")
    ENDIF(OPENTHREADS_USE_AGGRESSIVE_WARNINGS)
ENDIF(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS)


###############################################################################
# Set defaults for Universal Binaries. We want 32-bit Intel/PPC on 10.4 
# and 32/64-bit Intel/PPC on >= 10.5. Anything <= 10.3 doesn't support.
IF(APPLE)
    # These are just defaults/recommendations, but how we want to build
    # out of the box. But the user needs to be able to change these options.
    # So we must only set the values the first time CMake is run, or we 
    # will overwrite any changes the user sets.
    # FORCE is used because the options are not reflected in the UI otherwise.
    IF(NOT OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE)
        # This is really fragile, but CMake doesn't provide the OS system 
        # version information we need. (Darwin versions can be changed 
        # independently of OS X versions.)
        # It does look like CMake handles the CMAKE_OSX_SYSROOT automatically.
        IF(EXISTS /Developer/SDKs/10.5.sdk)
            SET(CMAKE_OSX_ARCHITECTURES "ppc;i386;ppc64;x86_64" CACHE STRING "Build architectures for OSX" FORCE)
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden" CACHE STRING "Flags used by the compiler during all build types." FORCE)
        ELSE(EXISTS /Developer/SDKs/10.5.sdk)
            IF(EXISTS /Developer/SDKs/MacOSX10.4u.sdk)
                # Unix layer on Tiger is 64-bit clean/ready so I think 
                # it is okay to build 64-bit here. This presumes we 
                # keep non-64-bit ready APIs (e.g. Carbon) out
                # of OpenThreads.
                SET(CMAKE_OSX_ARCHITECTURES "ppc;i386;ppc64;x86_64" CACHE STRING "Build architectures for OSX" FORCE)
                #SET(CMAKE_OSX_ARCHITECTURES "ppc;i386" CACHE STRING "Build architectures for OSX" FORCE)
                SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden" CACHE STRING "Flags used by the compiler during all build types." FORCE)
            ELSE(EXISTS /Developer/SDKs/MacOSX10.4u.sdk)
                # No Universal Binary support
            ENDIF(EXISTS /Developer/SDKs/MacOSX10.4u.sdk)
        ENDIF(EXISTS /Developer/SDKs/10.5.sdk)
    ENDIF(NOT OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE)
ENDIF(APPLE)



################################################################################
# For Doxygen
INCLUDE(${CMAKE_ROOT}/Modules/Documentation.cmake OPTIONAL)

# To build the documention, you will have to enable it
# and then do the equivalent of "make DoxygenDoc".
IF(BUILD_DOCUMENTATION)
    IF(DOT)
        SET(HAVE_DOT YES)
    ELSE(DOT)
        SET(HAVE_DOT NO)
    ENDIF(DOT)
    # This processes our Doxyfile.in and substitutes paths to generate
    # a final Doxyfile
    CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/docs/doxyfile.cmake
        ${PROJECT_BINARY_DIR}/docs/doxyfile
        )
    # This creates a new target to build documentation.
    # It runs ${DOXYGEN} which is the full path and executable to
    # Doxygen on your system, set by the FindDoxygen.cmake module
    # (called by FindDocumentation.cmake).
    # It runs the final generated Doxyfile against it.
    # The DOT_PATH is substituted into the Doxyfile.
    ADD_CUSTOM_TARGET(DoxygenDoc ${DOXYGEN}
        ${PROJECT_BINARY_DIR}/docs/doxyfile
    )
ENDIF(BUILD_DOCUMENTATION)


# This needs to be run very last so other parts of the scripts can take
# advantage of this.
IF(NOT OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE)
    SET(OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE 1 CACHE INTERNAL "Flag to track whether this is the first time running CMake or if CMake has been configured before")
ENDIF(NOT OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE)


