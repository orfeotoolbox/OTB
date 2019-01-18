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

project(IteratorsExamples)

add_executable(ImageLinearIteratorWithIndex2 ImageLinearIteratorWithIndex2.cxx)
target_link_libraries(ImageLinearIteratorWithIndex2 ${OTB_LIBRARIES})

add_executable(ImageLinearIteratorWithIndex ImageLinearIteratorWithIndex.cxx)
target_link_libraries(ImageLinearIteratorWithIndex ${OTB_LIBRARIES})

add_executable(ImageRandomConstIteratorWithIndex ImageRandomConstIteratorWithIndex.cxx)
target_link_libraries(ImageRandomConstIteratorWithIndex ${OTB_LIBRARIES})

add_executable(ImageRegionIterator ImageRegionIterator.cxx)
target_link_libraries(ImageRegionIterator ${OTB_LIBRARIES})

add_executable(ImageRegionIteratorWithIndex ImageRegionIteratorWithIndex.cxx)
target_link_libraries(ImageRegionIteratorWithIndex ${OTB_LIBRARIES})

add_executable(ImageSliceIteratorWithIndex ImageSliceIteratorWithIndex.cxx)
target_link_libraries(ImageSliceIteratorWithIndex ${OTB_LIBRARIES})

add_executable(IteratorsExamples IteratorsExamples.cxx)
target_link_libraries(IteratorsExamples ${OTB_LIBRARIES})

add_executable(NeighborhoodIterators1 NeighborhoodIterators1.cxx)
target_link_libraries(NeighborhoodIterators1 ${OTB_LIBRARIES})

add_executable(NeighborhoodIterators2 NeighborhoodIterators2.cxx)
target_link_libraries(NeighborhoodIterators2 ${OTB_LIBRARIES})

add_executable(NeighborhoodIterators3 NeighborhoodIterators3.cxx)
target_link_libraries(NeighborhoodIterators3 ${OTB_LIBRARIES})

add_executable(NeighborhoodIterators4 NeighborhoodIterators4.cxx)
target_link_libraries(NeighborhoodIterators4 ${OTB_LIBRARIES})

add_executable(NeighborhoodIterators5 NeighborhoodIterators5.cxx)
target_link_libraries(NeighborhoodIterators5 ${OTB_LIBRARIES})

add_executable(NeighborhoodIterators6 NeighborhoodIterators6.cxx)
target_link_libraries(NeighborhoodIterators6 ${OTB_LIBRARIES})

add_executable(ShapedNeighborhoodIterators1 ShapedNeighborhoodIterators1.cxx)
target_link_libraries(ShapedNeighborhoodIterators1 ${OTB_LIBRARIES})

add_executable(ShapedNeighborhoodIterators2 ShapedNeighborhoodIterators2.cxx)
target_link_libraries(ShapedNeighborhoodIterators2 ${OTB_LIBRARIES})


if(BUILD_TESTING)
  add_subdirectory(test)
endif()
