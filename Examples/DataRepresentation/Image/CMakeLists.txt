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

project(ImageExamples)

add_executable(Image1 Image1.cxx )
target_link_libraries(Image1 ${OTB_LIBRARIES})

add_executable(Image2 Image2.cxx )
target_link_libraries(Image2 ${OTB_LIBRARIES})

add_executable(Image3 Image3.cxx )
target_link_libraries(Image3 ${OTB_LIBRARIES})

add_executable(Image4 Image4.cxx )
target_link_libraries(Image4 ${OTB_LIBRARIES})

add_executable(Image5 Image5.cxx )
target_link_libraries(Image5 ${OTB_LIBRARIES})

add_executable(Image6 Image6.cxx )
target_link_libraries(Image6 ${OTB_LIBRARIES} )

add_executable(RGBImage RGBImage.cxx )
target_link_libraries(RGBImage ${OTB_LIBRARIES})

add_executable(VectorImage VectorImage.cxx )
target_link_libraries(VectorImage ${OTB_LIBRARIES})

add_executable(ImageListExample ImageListExample.cxx )
target_link_libraries(ImageListExample ${OTB_LIBRARIES} )

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

