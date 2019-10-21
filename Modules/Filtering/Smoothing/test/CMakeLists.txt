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

otb_module_test()

set(OTBSmoothingTests
otbSmoothingTestDriver.cxx
otbMeanShiftSmoothingImageFilter.cxx
otbMeanShiftSmoothingImageFilterSpatialStability.cxx
otbMeanShiftSmoothingImageFilterThreading.cxx
)

add_executable(otbSmoothingTestDriver ${OTBSmoothingTests})
target_link_libraries(otbSmoothingTestDriver ${OTBSmoothing-Test_LIBRARIES})
otb_module_target_label(otbSmoothingTestDriver)

# Tests Declaration

otb_add_test(NAME bfTvMeanShiftSmoothingImageFilterOptim COMMAND otbSmoothingTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvMeanShiftFilterSpectralOutputOptim.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpectralOutputOptim.tif
  otbMeanShiftSmoothingImageFilter
  ${INPUTDATA}/MeanShiftTest.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpatialOutputOptim.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpectralOutputOptim.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterIterationOutputOptim.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterLabelOutputOptim.tif
  2 10 0.1 10 1
  )

set_tests_properties(bfTvMeanShiftSmoothingImageFilterOptim PROPERTIES
  ENVIRONMENT ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS=4)


otb_add_test(NAME bfTuMeanShiftSmoothingImageFilterSPOT5 COMMAND otbSmoothingTestDriver
  otbMeanShiftSmoothingImageFilter
  ${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_SPOT5.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_SPOT5.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_SPOT5.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_SPOT5.tif
  4 10 0.1 100
  )

otb_add_test(NAME bfTvMeanShiftSmoothingImageFilterNonOptim COMMAND otbSmoothingTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvMeanShiftFilterSpectralOutputNonOptim.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpectralOutputNonOptim.tif
  otbMeanShiftSmoothingImageFilter
  ${INPUTDATA}/MeanShiftTest.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpatialOutputNonOptim.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterSpectralOutputNonOptim.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterIterationOutputNonOptim.tif
  ${TEMP}/bfTvMeanShiftSmoothingImageFilterLabelOutputNonOptim.tif
  2 10 0.1 10 0
  )

otb_add_test(NAME bfTuMeanShiftSmoothingImageFilterROIQB COMMAND otbSmoothingTestDriver
  otbMeanShiftSmoothingImageFilter
  ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_ROIQB.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_ROIQB.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_ROIQB.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_ROIQB.tif
  4 50 0.1 100
  )

otb_add_test(NAME bfTvMeanShiftSmoothingImageFilterQBSuburbNonOptim COMMAND otbSmoothingTestDriver
  --compare-n-images ${EPSILON_7} 3
  ${BASELINE}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBSuburbNonOptim.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBSuburbNonOptim.tif
  ${BASELINE}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbNonOptim.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbNonOptim.tif
  ${BASELINE}/bfMeanShiftSmoothingImageFilterIterationOutput_QBSuburbNonOptim.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBSuburbNonOptim.tif
  otbMeanShiftSmoothingImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBSuburbNonOptim.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbNonOptim.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBSuburbNonOptim.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_QBSuburbNonOptim.tif
  4 25 0.1 100 0
  )

otb_add_test(NAME bfTuMeanShiftSmoothingImageFilterQBSuburbOptim COMMAND otbSmoothingTestDriver
  otbMeanShiftSmoothingImageFilter
  ${INPUTDATA}/QB_Suburb.png
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBSuburbOptim.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbOptim.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBSuburbOptim.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_QBSuburbOptim.tif
  4 25 0.1 100 1
  )

otb_add_test(NAME bfTvMeanShiftSmoothingImageFilterQBPAN COMMAND otbSmoothingTestDriver
  otbMeanShiftSmoothingImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_PAN.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_PAN.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_PAN.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_PAN.tif
  5 15 1.0 100
  )

otb_add_test(NAME bfTuMeanShiftSmoothingImageFilterQBRoad COMMAND otbSmoothingTestDriver
  otbMeanShiftSmoothingImageFilter
  ${INPUTDATA}/ctest/qb_RoadExtract_200x200.img
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpatialOutput_QBRoad.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBRoad.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterIterationOutput_QBRoad.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterLabelOutput_QBRoad.tif
  4 30 0.1 100
  )

otb_add_test(NAME bfTuMeanShiftSmoothingImageFilterSpatialStability COMMAND otbSmoothingTestDriver
  otbMeanShiftSmoothingImageFilterSpatialStability
  ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
  2 50 0.1 10
  10 10 80 80
  )


otb_add_test(NAME bfTvMeanShiftSmoothingImageFilterThreadingNonOpt COMMAND otbSmoothingTestDriver
  --compare-image ${EPSILON_7}
  ${TEMP}/bfMeanShiftSmoothingImageFilterSingleThread.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterMultiThread.tif
  otbMeanShiftSmoothingImageFilterThreading
  ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterSingleThread.tif
  ${TEMP}/bfMeanShiftSmoothingImageFilterMultiThread.tif
  4 50 0
  )
