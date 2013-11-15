if(WIN32)
    cmake_minimum_required(VERSION 2.4.6 FATAL_ERROR)
else(WIN32)
    if(APPLE)
        cmake_minimum_required(VERSION 2.4.6 FATAL_ERROR)
    else(APPLE)
        cmake_minimum_required(VERSION 2.4.0 FATAL_ERROR)
    endif(APPLE)
endif(WIN32)


project(OpenThreads)

# We have some custom .cmake scripts not in the official distribution.
# Maybe this can be used override existing behavior if needed?
set(CMAKE_MODULE_PATH "${OpenThreads_SOURCE_DIR}/CMakeModules;${CMAKE_MODULE_PATH}")

################################################################################
# Installation stuff

# We want to build SONAMES shared librariess
set(OPENTHREADS_SONAMES TRUE)

#set(CMAKE_DEBUG_POSTFIX  "d")
#set(INSTALL_BINDIR OpenThreads/bin)
#set(INSTALL_INCDIR OpenThreads/include)
#set(INSTALL_LIBDIR OpenThreads/lib)
#set(INSTALL_DOCDIR OpenThreads/docs)


#set(CMAKE_DEBUG_POSTFIX  "d")

set(LIB_POSTFIX "")
if(UNIX AND NOT WIN32 AND NOT APPLE)
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
      set(LIB_POSTFIX "64")
  endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
endif(UNIX AND NOT WIN32 AND NOT APPLE)

################################################################################


# Add a source group for the include headers so they are seen in IDEs.
# Hmmm, this isn't working for me in Xcode.
# source_group(
#     "Header Files"
#     FILES ${OpenThreads_PUBLIC_HEADERS}
# )

# Make the headers visible to everything
include_directories(
    ${OpenThreads_SOURCE_DIR}/include
)

# This is mainly for Windows declspec, but other platforms know
# what to do with it.
add_definitions(-DOPENTHREADS_EXPORTS)

# User Options
# OTB modifications
#option(DYNAMIC_OPENTHREADS "Set to ON to build OpenThreads for dynamic linking.  Use OFF for static." ON)
#if(DYNAMIC_OPENTHREADS)
#    set(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
#else(DYNAMIC_OPENTHREADS)
#    set(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
#endif(DYNAMIC_OPENTHREADS)
if( OTB_BUILD_SHARED_LIBS )
    set(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
else( OTB_BUILD_SHARED_LIBS )
    set(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
endif( OTB_BUILD_SHARED_LIBS )



# Use our modified version of FindThreads.cmake which has Sproc hacks.
find_package(Threads)

# OTB Modifications
#subdirs(src/OpenThreads examples)
subdirs(src/OpenThreads)

# FIXME: Make optional install test programs
# FIXME: /bin directory not correct for Windows
# install_targets(/bin OpenThreads)

#set(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
# OTB modifications
# Alls bin or lib in OTB bin directory
#set(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
set(OUTPUT_BINDIR ${OTB_BINARY_DIR}/bin)
make_directory(${OUTPUT_BINDIR})
set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_BINDIR})

#set(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME})
#set(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
set(OUTPUT_LIBDIR ${OTB_BINARY_DIR}/bin)
make_directory(${OUTPUT_LIBDIR})
set(LIBRARY_OUTPUT_PATH ${OUTPUT_LIBDIR})

################################################################################
# Create bin and lib directories if required

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin ${CMAKE_BINARY_DIR}/lib)
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")




###############################################################################
# This is for an advanced option to give aggressive warnings
# under different compilers. If yours is not implemented, this option
# will not be made available.
if(CMAKE_COMPILER_IS_GNUCXX)
    # To be complete, we might also do GNUCC flags,
    # but everything here is C++ code.
    # FYI, if we do implement GNUCC, then -Wmissing-prototypes in another
    # interesting C-specific flag.
    # Also, there is a bug in gcc 4.0. Under C++, -pedantic will create
    # errors instead of warnings for certain issues, including superfluous
    # semicolons and commas, and the use of long long. -fpermissive seems
    # to be the workaround.
#    set(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS "-Wall -Wparentheses -Wformat=2 -Wno-long-long -Wno-import -pedantic -Wnewline-eof -Wreturn-type -Wmissing-braces -Wunknown-pragmas -Wunused -Wshadow -Woverloaded-virtual -fpermissive")
    set(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS "-w -Wparentheses -Wformat=2 -Wno-long-long -Wno-import -pedantic -Wnewline-eof -Wreturn-type -Wmissing-braces -Wunknown-pragmas -Wunused -Wshadow -Woverloaded-virtual -fpermissive")
else(CMAKE_COMPILER_IS_GNUCXX)
    if(MSVC)
        # FIXME: What are good aggressive warning flags for Visual Studio?
        # And do we need to further subcase this for different versions of VS?
        # CMake variables: MSVC60, MSVC70, MSVC71, MSVC80, CMAKE_COMPILER_2005
#        set(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS "/Wall /W4")
        set(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS "/W0")
    else(MSVC)
        # CMake lacks an elseif, so other non-gcc, non-VS compilers need
        # to be listed below. If unhandled, OPENTHREADS_AGGRESSIVE_WARNING_FLAGS should
        # remain unset.
    endif(MSVC)
endif(CMAKE_COMPILER_IS_GNUCXX)

# This part is for the CMake menu option to toggle the warnings on/off.
# This will only be made available if we set values for OPENTHREADS_AGGRESSIVE_WARNING_FLAGS.
if(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS)
    option(OPENTHREADS_USE_AGGRESSIVE_WARNINGS "Enable to activate aggressive warnings" OFF)
    mark_as_advanced(OPENTHREADS_USE_AGGRESSIVE_WARNINGS)

    if(OPENTHREADS_USE_AGGRESSIVE_WARNINGS)
        if(NOT "${OLD_CMAKE_CXX_FLAGS_WAS_SET}")
            set(OLD_CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE INTERNAL "Old CXX flags")
            set(OLD_CMAKE_CXX_FLAGS_WAS_SET 1 CACHE INTERNAL "Old CXX flags was set")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENTHREADS_AGGRESSIVE_WARNING_FLAGS}" CACHE STRING "Flags used by the compiler during all build types." FORCE)
        endif(NOT "${OLD_CMAKE_CXX_FLAGS_WAS_SET}")
    else(OPENTHREADS_USE_AGGRESSIVE_WARNINGS)
        # FIXME: This will lose any changes made after OLD_CMAKE_CXX_FLAGS was
        # set. The better way would be to parse the string and remove each
        # option explicitly.
        if("${OLD_CMAKE_CXX_FLAGS_WAS_SET}")
            set(CMAKE_CXX_FLAGS "${OLD_CMAKE_CXX_FLAGS}" CACHE STRING "Flags used by the compiler during all build types." FORCE)
            set(OLD_CMAKE_CXX_FLAGS_WAS_SET 0 CACHE INTERNAL "Old CXX flags was set")
        endif("${OLD_CMAKE_CXX_FLAGS_WAS_SET}")
    endif(OPENTHREADS_USE_AGGRESSIVE_WARNINGS)
endif(OPENTHREADS_AGGRESSIVE_WARNING_FLAGS)


###############################################################################
# Set defaults for Universal Binaries. We want 32-bit Intel/PPC on 10.4
# and 32/64-bit Intel/PPC on >= 10.5. Anything <= 10.3 doesn't support.
if(APPLE)
    # These are just defaults/recommendations, but how we want to build
    # out of the box. But the user needs to be able to change these options.
    # So we must only set the values the first time CMake is run, or we
    # will overwrite any changes the user sets.
    # FORCE is used because the options are not reflected in the UI otherwise.
    if(NOT OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE)
        # This is really fragile, but CMake doesn't provide the OS system
        # version information we need. (Darwin versions can be changed
        # independently of OS X versions.)
        # It does look like CMake handles the CMAKE_OSX_SYSROOT automatically.
        if(EXISTS /Developer/SDKs/10.5.sdk)
            set(CMAKE_OSX_ARCHITECTURES "ppc;i386;ppc64;x86_64" CACHE STRING "Build architectures for OSX" FORCE)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden" CACHE STRING "Flags used by the compiler during all build types." FORCE)
        else(EXISTS /Developer/SDKs/10.5.sdk)
            if(EXISTS /Developer/SDKs/MacOSX10.4u.sdk)
                # Unix layer on Tiger is 64-bit clean/ready so I think
                # it is okay to build 64-bit here. This presumes we
                # keep non-64-bit ready APIs (e.g. Carbon) out
                # of OpenThreads.
                set(CMAKE_OSX_ARCHITECTURES "ppc;i386;ppc64;x86_64" CACHE STRING "Build architectures for OSX" FORCE)
                #set(CMAKE_OSX_ARCHITECTURES "ppc;i386" CACHE STRING "Build architectures for OSX" FORCE)
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility-inlines-hidden" CACHE STRING "Flags used by the compiler during all build types." FORCE)
            else(EXISTS /Developer/SDKs/MacOSX10.4u.sdk)
                # No Universal Binary support
            endif(EXISTS /Developer/SDKs/MacOSX10.4u.sdk)
        endif(EXISTS /Developer/SDKs/10.5.sdk)
    endif(NOT OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE)
endif(APPLE)



################################################################################
# For Doxygen
include(${CMAKE_ROOT}/Modules/Documentation.cmake OPTIONAL)

# To build the documention, you will have to enable it
# and then do the equivalent of "make DoxygenDoc".
if(BUILD_DOCUMENTATION)
    if(DOT)
        set(HAVE_DOT YES)
    else(DOT)
        set(HAVE_DOT NO)
    endif(DOT)
    # This processes our Doxyfile.in and substitutes paths to generate
    # a final Doxyfile
    configure_file(${PROJECT_SOURCE_DIR}/docs/doxyfile.cmake
        ${PROJECT_BINARY_DIR}/docs/doxyfile
        )
    # This creates a new target to build documentation.
    # It runs ${DOXYGEN} which is the full path and executable to
    # Doxygen on your system, set by the FindDoxygen.cmake module
    # (called by FindDocumentation.cmake).
    # It runs the final generated Doxyfile against it.
    # The DOT_PATH is substituted into the Doxyfile.
    add_custom_target(DoxygenDoc ${DOXYGEN}
        ${PROJECT_BINARY_DIR}/docs/doxyfile
    )
endif(BUILD_DOCUMENTATION)


# This needs to be run very last so other parts of the scripts can take
# advantage of this.
if(NOT OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE)
    set(OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE 1 CACHE INTERNAL "Flag to track whether this is the first time running CMake or if CMake has been configured before")
endif(NOT OPENTHREADS_CONFIG_HAS_BEEN_RUN_BEFORE)


