# File from https://github.com/progschj/OpenGL-Examples/
#
# ----------------------------------------------------------------------------
# Copyright (c) 2012-2013 Jakob Progsch
# Released under ZLib license:
#
# This software is provided 'as-is', without any express or implied warranty.
# In no event will the authors be held liable for any damages arising from the
# use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software in
#    a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
#
# 3. This notice may not be removed or altered from any source distribution.
# ----------------------------------------------------------------------------
#
# NOTE: This file was removed from OpenGL-Examples project on 2013-11-01:
#       https://github.com/progschj/OpenGL-Examples/commit/a100ce1ec6f5f6acf6ba3a3279cd3b3b91775b20
#
# Try to find GLEW library and include path.
# Once done this will define
#rewritten for OTB by Rashad Kanavath
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
#

*# This whole file could be deleted. Find Packages GLEW can be used now
find_path(
  GLEW_INCLUDE_DIR GL/glew.h
  DOC "The directory where GL/glew.h resides"
  )

if(WIN32)
  set(GLEW_LIB_NAMES glew GLEW glew32 glew32s)
else()
  set(GLEW_LIB_NAMES libGLEW GLEW)
endif()

FIND_LIBRARY( GLEW_LIBRARY
  NAMES ${GLEW_LIB_NAMES}
  DOC "The GLEW library")

set(GLEW_FOUND FALSE)
if (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
  set(GLEW_LIBRARIES ${GLEW_LIBRARY})
  set(GLEW_FOUND TRUE)
endif()

INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW
  REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR
  VERSION_VAR GLEW_VERSION_STRING)
