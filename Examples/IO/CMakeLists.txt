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

project(IOExamples)

add_executable(ComplexImageReadWrite ComplexImageReadWrite.cxx)
target_link_libraries(ComplexImageReadWrite ${OTB_LIBRARIES})

add_executable(DEMHandlerExample DEMHandlerExample.cxx)
target_link_libraries(DEMHandlerExample ${OTB_LIBRARIES})

add_executable(DEMToImageGenerator DEMToImageGenerator.cxx)
target_link_libraries(DEMToImageGenerator ${OTB_LIBRARIES})

add_executable(ExplicitStreamingExample ExplicitStreamingExample.cxx)
target_link_libraries(ExplicitStreamingExample ${OTB_LIBRARIES})

add_executable(ExtractROI ExtractROI.cxx)
target_link_libraries(ExtractROI ${OTB_LIBRARIES})

add_executable(HDFReaderExample HDFReaderExample.cxx)
target_link_libraries(HDFReaderExample ${OTB_LIBRARIES})

add_executable(ImageReadCastWrite ImageReadCastWrite.cxx)
target_link_libraries(ImageReadCastWrite ${OTB_LIBRARIES})

add_executable(ImageReadRegionOfInterestWrite ImageReadRegionOfInterestWrite.cxx)
target_link_libraries(ImageReadRegionOfInterestWrite ${OTB_LIBRARIES})

add_executable(ImageReadWrite ImageReadWrite.cxx)
target_link_libraries(ImageReadWrite ${OTB_LIBRARIES})

add_executable(ImageSeriesIOExample ImageSeriesIOExample.cxx)
target_link_libraries(ImageSeriesIOExample ${OTB_LIBRARIES})

if(OTBlibkml_LOADED)
add_executable(ImageToKmzAndMapFileProductExample ImageToKmzAndMapFileProductExample.cxx)
target_link_libraries(ImageToKmzAndMapFileProductExample ${OTB_LIBRARIES})
endif()

add_executable(MetadataExample MetadataExample.cxx)
target_link_libraries(MetadataExample ${OTB_LIBRARIES})

add_executable(MultibandImageReadWrite MultibandImageReadWrite.cxx)
target_link_libraries(MultibandImageReadWrite ${OTB_LIBRARIES})

add_executable(OGRWrappersExample OGRWrappersExample.cxx)
target_link_libraries(OGRWrappersExample ${OTB_LIBRARIES})

add_executable(RGBImageReadWrite RGBImageReadWrite.cxx)
target_link_libraries(RGBImageReadWrite ${OTB_LIBRARIES})

add_executable(StreamingImageReadWrite StreamingImageReadWrite.cxx)
target_link_libraries(StreamingImageReadWrite ${OTB_LIBRARIES})

add_executable(VectorDataIOExample VectorDataIOExample.cxx)
target_link_libraries(VectorDataIOExample ${OTB_LIBRARIES})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()
