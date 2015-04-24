# OpenThreads is a C++ based threading library. Its largest userbase
# seems to OpenSceneGraph so you might notice I accept OSGDIR as an
# environment path.
# I consider this part of the Findosg* suite used to find OpenSceneGraph
# components.
# Each component is separate and you must opt in to each module.
#
# Locate OpenThreads
# This module defines
# OPENTHREADS_LIBRARIES
# OPENTHREADS_FOUND, if false, do not try to link to OpenThreads
# OPENTHREADS_INCLUDE_DIRS, where to find the headers
# OPENTHREADS_VERSION        - version string
# OPENTHREADS_VERSION_MAJOR  - version major number
# OPENTHREADS_VERSION_MINOR  - version minor number
# OPENTHREADS_VERSION_PATCH  - version patch number
# OPENTHREADS_VERSION_NUMBER - version number ((major*100)+minor)*100+patch
#
# $OPENTHREADS_DIR is an environment variable that would
# correspond to the ./configure --prefix=$OPENTHREADS_DIR
# used in building osg.
#
# Created by Eric Wing.
# Modified by Guillaume Pasero.

#=============================================================================
# Copyright 2007-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
# See OTBCopyright.txt for details.
#
#
#     This software is distributed WITHOUT ANY WARRANTY; without even
#     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
#     PURPOSE.  See the above copyright notices for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# Header files are presumed to be included like
# #include <OpenThreads/Thread>

# To make it easier for one-step automated configuration/builds,
# we leverage environmental paths. This is preferable
# to the -DVAR=value switches because it insulates the
# users from changes we may make in this script.
# It also offers a little more flexibility than setting
# the CMAKE_*_PATH since we can target specific components.
# However, the default CMake behavior will search system paths
# before anything else. This is problematic in the cases
# where you have an older (stable) version installed, but
# are trying to build a newer version.
# CMake doesn't offer a nice way to globally control this behavior
# so we have to do a nasty "double FIND_" in this module.
# The first FIND disables the CMAKE_ search paths and only checks
# the environmental paths.
# If nothing is found, then the second find will search the
# standard install paths.
# Explicit -DVAR=value arguments should still be able to override everything.

find_path(OPENTHREADS_INCLUDE_DIR OpenThreads/Thread
    HINTS
        # enough environment variables?
        $ENV{OPENTHREADS_INCLUDE_DIR}
        $ENV{OPENTHREADS_DIR}
        $ENV{OSG_INCLUDE_DIR}
        $ENV{OSG_DIR}
        $ENV{OSGDIR}
        $ENV{OpenThreads_ROOT}
        $ENV{OSG_ROOT}
    PATHS
        /sw # Fink
        /opt/local # DarwinPorts
        /opt/csw # Blastwave
        /opt
        /usr/freeware
    PATH_SUFFIXES include
)
mark_as_advanced(OPENTHREADS_INCLUDE_DIR)

if(EXISTS "${OPENTHREADS_INCLUDE_DIR}/OpenThreads/Version")
  file(READ "${OPENTHREADS_INCLUDE_DIR}/OpenThreads/Version" _openthreads_version_CONTENTS)
  string(REGEX REPLACE ".*# *define +OPENTHREADS_MAJOR_VERSION +([0-9]+).*" "\\1" OPENTHREADS_VERSION_MAJOR "${_openthreads_version_CONTENTS}")
  string(REGEX REPLACE ".*# *define +OPENTHREADS_MINOR_VERSION +([0-9]+).*" "\\1" OPENTHREADS_VERSION_MINOR "${_openthreads_version_CONTENTS}")
  string(REGEX REPLACE ".*# *define +OPENTHREADS_PATCH_VERSION +([0-9]+).*" "\\1" OPENTHREADS_VERSION_PATCH "${_openthreads_version_CONTENTS}")
  
  set(OPENTHREADS_VERSION "${OPENTHREADS_VERSION_MAJOR}.${OPENTHREADS_VERSION_MINOR}.${OPENTHREADS_VERSION_PATCH}")
  math(EXPR OPENTHREADS_VERSION_NUMBER
    "((${OPENTHREADS_VERSION_MAJOR})*100+${OPENTHREADS_VERSION_MINOR})*100+${OPENTHREADS_VERSION_PATCH}")
else()
  if(NOT OPENTHREADS_FIND_QUIETLY)
  message(WARNING "OpenThreads/Version not found !")
  endif()
endif()

find_library(OPENTHREADS_LIBRARY
    NAMES OpenThreads OpenThreadsWin32
    HINTS
        $ENV{OPENTHREADS_LIBRARY_DIR}
        $ENV{OPENTHREADS_DIR}
        $ENV{OSG_LIBRARY_DIR}
        $ENV{OSG_DIR}
        $ENV{OSGDIR}
        $ENV{OpenThreads_ROOT}
        $ENV{OSG_ROOT}
    PATHS
        /sw
        /opt/local
        /opt/csw
        /opt
        /usr/freeware
    PATH_SUFFIXES lib64 lib
)
mark_as_advanced(OPENTHREADS_LIBRARY)

find_library(OPENTHREADS_LIBRARY_DEBUG
    NAMES OpenThreadsd OpenThreadsWin32d
    HINTS
        $ENV{OPENTHREADS_DEBUG_LIBRARY_DIR}
        $ENV{OPENTHREADS_LIBRARY_DIR}
        $ENV{OPENTHREADS_DIR}
        $ENV{OSG_LIBRARY_DIR}
        $ENV{OSG_DIR}
        $ENV{OSGDIR}
        $ENV{OpenThreads_ROOT}
        $ENV{OSG_ROOT}
    PATHS
        /sw
        /opt/local
        /opt/csw
        /opt
        /usr/freeware
    PATH_SUFFIXES lib64 lib
)
mark_as_advanced(OPENTHREADS_LIBRARY_DEBUG)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenThreads DEFAULT_MSG
    OPENTHREADS_LIBRARY OPENTHREADS_INCLUDE_DIR)

if(OPENTHREADS_LIBRARY_DEBUG)
    set(OPENTHREADS_LIBRARIES
        optimized ${OPENTHREADS_LIBRARY}
        debug ${OPENTHREADS_LIBRARY_DEBUG})
else()
    set(OPENTHREADS_LIBRARIES ${OPENTHREADS_LIBRARY})
endif()

set(OPENTHREADS_INCLUDE_DIRS ${OPENTHREADS_INCLUDE_DIR})
