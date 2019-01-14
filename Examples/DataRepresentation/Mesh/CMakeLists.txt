#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

project(MeshExamples)

add_executable(Mesh1 Mesh1.cxx )
target_link_libraries(Mesh1 ${ITK_LIBRARIES})

add_executable(Mesh2 Mesh2.cxx )
target_link_libraries(Mesh2 ${ITK_LIBRARIES})

add_executable(Mesh3 Mesh3.cxx )
target_link_libraries(Mesh3 ${ITK_LIBRARIES})

add_executable(PointSet1 PointSet1.cxx )
target_link_libraries(PointSet1 ${ITK_LIBRARIES})

add_executable(PointSet2 PointSet2.cxx )
target_link_libraries(PointSet2 ${ITK_LIBRARIES})

add_executable(PointSet3 PointSet3.cxx )
target_link_libraries(PointSet3 ${ITK_LIBRARIES})

add_executable(PointSetWithVectors PointSetWithVectors.cxx )
target_link_libraries(PointSetWithVectors ${ITK_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
