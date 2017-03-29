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

# - Find LibKML
# Find the native LibKML includes and library
#
#   LIBKML_FOUND       - True if LibKML found.
#   LIBKML_INCLUDE_DIR - where to find kml/dom.h, etc.
#   LIBKML_LIBRARIES   - List of libraries when using LibKML.
#

if( LIBKML_INCLUDE_DIR )
    # Already in cache, be silent
    set( LibKML_FIND_QUIETLY TRUE )
endif()

find_path( LIBKML_INCLUDE_DIR kml/dom.h )

find_library( LIBKML_BASE_LIBRARY
              NAMES kmlbase )

# TODO : add HINTS derived from path of LIBKML_BASE_LIBRARY
find_library( LIBKML_CONVENIENCE_LIBRARY
              NAMES kmlconvenience )
find_library( LIBKML_DOM_LIBRARY
              NAMES kmldom )
find_library( LIBKML_ENGINE_LIBRARY
              NAMES kmlengine )
find_library( LIBKML_REGIONATOR_LIBRARY
              NAMES kmlregionator )
find_library( LIBKML_XSD_LIBRARY
              NAMES kmlxsd )
find_library( LIBKML_MINIZIP_LIBRARY
              NAMES minizip )

mark_as_advanced(   LIBKML_INCLUDE_DIR
                    LIBKML_BASE_LIBRARY
                    LIBKML_CONVENIENCE_LIBRARY
                    LIBKML_DOM_LIBRARY
                    LIBKML_ENGINE_LIBRARY
                    LIBKML_REGIONATOR_LIBRARY
                    LIBKML_XSD_LIBRARY
                    LIBKML_MINIZIP_LIBRARY )

# handle the QUIETLY and REQUIRED arguments and set LIBKML_FOUND to TRUE if
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( LibKML DEFAULT_MSG
                                    LIBKML_BASE_LIBRARY
                                    LIBKML_CONVENIENCE_LIBRARY
                                    LIBKML_DOM_LIBRARY
                                    LIBKML_ENGINE_LIBRARY
                                    LIBKML_REGIONATOR_LIBRARY
                                    LIBKML_XSD_LIBRARY
                                    LIBKML_MINIZIP_LIBRARY
                                    LIBKML_INCLUDE_DIR
                                     )

if(LIBKML_FOUND)
  set(LIBKML_INCLUDE_DIRS ${LIBKML_INCLUDE_DIR})
  set(LIBKML_LIBRARIES ${LIBKML_BASE_LIBRARY}
                       ${LIBKML_CONVENIENCE_LIBRARY}
                       ${LIBKML_DOM_LIBRARY}
                       ${LIBKML_ENGINE_LIBRARY}
                       ${LIBKML_REGIONATOR_LIBRARY}
                       ${LIBKML_XSD_LIBRARY}
                       ${LIBKML_MINIZIP_LIBRARY})
endif()
