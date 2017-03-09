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

cmake_minimum_required(VERSION 2.8.3)

project(tiff_install)

file(GLOB ${PROJECT_NAME}_HEADERS "${TIFF_BUILD_DIR}/tif*.h" )
file(GLOB ${PROJECT_NAME}_LIBS "${TIFF_BUILD_DIR}/libtiff*.lib" )
file(GLOB ${PROJECT_NAME}_DLLS "${TIFF_BUILD_DIR}/libtiff*.dll" )

foreach(${PROJECT_NAME}_HEADER ${${PROJECT_NAME}_HEADERS})
install(FILES ${${PROJECT_NAME}_HEADER}
  DESTINATION include
  COMPONENT Development)
endforeach()
  
foreach(${PROJECT_NAME}_LIB ${${PROJECT_NAME}_LIBS})
install(FILES ${${PROJECT_NAME}_LIB}
  DESTINATION lib
  COMPONENT Development)
endforeach()

foreach(${PROJECT_NAME}_DLL ${${PROJECT_NAME}_DLLS})
install(FILES ${${PROJECT_NAME}_DLL}
  DESTINATION bin
  COMPONENT Development)
endforeach()