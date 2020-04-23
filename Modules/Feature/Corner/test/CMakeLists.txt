#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

otb_module_test()

set(OTBCornerTests
otbCornerTestDriver.cxx
otbVectorDataToRightAngleVectorDataFilter.cxx
otbHarrisImage.cxx
otbHarrisToPointSet.cxx
)

add_executable(otbCornerTestDriver ${OTBCornerTests})
target_link_libraries(otbCornerTestDriver ${OTBCorner-Test_LIBRARIES})
otb_module_target_label(otbCornerTestDriver)

# Tests Declaration

otb_add_test(NAME feTvVectorDataToRightAngleVectorDataFilter COMMAND otbCornerTestDriver
  --compare-ogr ${EPSILON_8}
  ${BASELINE_FILES}/feTvVectorDataToRightAngleVectorDataFilterOutput.shp
  ${TEMP}/feTvVectorDataToRightAngleVectorDataFilterOutput.shp
  otbVectorDataToRightAngleVectorDataFilter
  ${BASELINE_FILES}/feTvLineSegmentDetectorOutput.shp     #input vectordata
  ${TEMP}/feTvVectorDataToRightAngleVectorDataFilterOutput.shp  #output text file
  )

otb_add_test(NAME feTvHarrisImage COMMAND otbCornerTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feHarrisImage.png
  ${TEMP}/feHarrisImage.png
  otbHarrisImage
  ${INPUTDATA}/TeteAToto.png
  ${TEMP}/feHarrisImage.png
  1.0 2.0 0.0)

otb_add_test(NAME feTvHarrisPoupee COMMAND otbCornerTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feHarris_poupees.png
  ${TEMP}/feHarris_poupees.png
  otbHarrisImage
  ${INPUTDATA}/poupees_1canal.c1.hdr
  ${TEMP}/feHarris_poupees.png
  1.0 1.0 1.0)

otb_add_test(NAME feTvHarrisProduceImageToValidHarrisToPointSet COMMAND otbCornerTestDriver
  otbHarrisImage
  ${INPUTDATA}/small_points.raw.hdr
  ${TEMP}/small_points_harris.tif
  1.0 1.0 1.0)

otb_add_test(NAME feTvHarrisImageQB COMMAND otbCornerTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feHarrisImage_QB.png
  ${TEMP}/feHarrisImage_QB.png
  otbHarrisImage
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/feHarrisImage_QB.png
  1.0 1.0 1.0)

otb_add_test(NAME feTvHarrisToPointSet2 COMMAND otbCornerTestDriver
  --compare-ascii ${EPSILON_3}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_20To90.txt
  ${TEMP}/feHarrisToPointSet_Threshold_20To90.txt
  otbHarrisToPointSet
  ${INPUTDATA}/small_points.raw.hdr
  ${TEMP}/feHarrisToPointSet_Threshold_20To90.txt
  1.0 1.0 1.0 20 90)

otb_add_test(NAME feTvHarrisToPointSet3 COMMAND otbCornerTestDriver
  --compare-ascii ${EPSILON_3}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_100To160.txt
  ${TEMP}/feHarrisToPointSet_Threshold_100To160.txt
  otbHarrisToPointSet
  ${INPUTDATA}/small_points.raw.hdr
  ${TEMP}/feHarrisToPointSet_Threshold_100To160.txt
  1.0 1.0 1.0 100 160)

otb_add_test(NAME feTvHarrisToPointSet1 COMMAND otbCornerTestDriver
  --compare-ascii ${EPSILON_3}        ${BASELINE_FILES}/feHarrisToPointSet_Threshold_2To255.txt
  ${TEMP}/feHarrisToPointSet_Threshold_2To255.txt
  otbHarrisToPointSet
  ${INPUTDATA}/small_points.raw.hdr
  ${TEMP}/feHarrisToPointSet_Threshold_2To255.txt
  1.0 1.0 1.0 2.0 255.0)
