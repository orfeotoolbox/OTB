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

set(OTBRoadExtractionTests
otbRoadExtractionTestDriver.cxx
otbLikelihoodPathListFilter.cxx
otbRemoveWrongDirectionFilter.cxx
otbSimplifyPathListFilter.cxx
otbAlignImageToPath.cxx
otbNonMaxRemovalByDirectionFilter.cxx
otbRoadExtractionFilter.cxx
otbLinkPathListFilter.cxx
otbRemoveTortuousPathListFilter.cxx
otbParallelLinePathListFilter.cxx
otbRemoveIsolatedByDirectionFilter.cxx
otbVectorDataToRoadDescriptionFilter.cxx
otbSimplifyManyPathListFilter.cxx
otbNeighborhoodScalarProductFilter.cxx
otbDrawPathAlign.cxx
otbBreakAngularPathListFilter.cxx
)

add_executable(otbRoadExtractionTestDriver ${OTBRoadExtractionTests})
target_link_libraries(otbRoadExtractionTestDriver ${OTBRoadExtraction-Test_LIBRARIES})
otb_module_target_label(otbRoadExtractionTestDriver)

# Tests Declaration

otb_add_test(NAME feTvLikelihoodPathListFilter COMMAND otbRoadExtractionTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvLikelihoodPathOutput.txt
  ${TEMP}/feTvLikelihoodPathOutput.txt
  otbLikelihoodPathListFilter
  ${INPUTDATA}/InputForRoadDetection_NonMaxRem.raw.hdr
  ${TEMP}/feTvLikelihoodPathOutput.txt
  157 335 204 376 |
  21 305 35 308 77 354 85 358 |
  73 160 126 173
  )

otb_add_test(NAME feTvRemoveWrongDirectionFilter COMMAND otbRoadExtractionTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvRemoveWrongDirectionOutput.tif
  ${TEMP}/feTvRemoveWrongDirectionOutput.tif
  otbRemoveWrongDirectionFilter
  ${INPUTDATA}/InputForRoadDetection_IsolatedRem.raw.hdr
  ${INPUTDATA}/InputForRoadDetectionScalarProductDir.raw.hdr
  ${TEMP}/feTvRemoveWrongDirectionOutput.tif
  )

otb_add_test(NAME feTvSimplifyPathListFilter COMMAND otbRoadExtractionTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvSimplifyPathOutput.txt
  ${TEMP}/feTvSimplifyPathOutput.txt
  otbSimplifyPathListFilter
  ${TEMP}/feTvSimplifyPathOutput.txt
  1.0
  1 1   5  1  7  1  11  1  21  1  31  1 |
  1 1   5  1  7  1  11  1  21 11  31 11 |
  1 1   5  5  7  7  11 11  21 21  31 31 41 41 |
  1 1   5  1  7  1  11  1  21 11  31 11 41 11 58 11 70 11
  )

otb_add_test(NAME feTvAlignMV2ITK COMMAND otbRoadExtractionTestDriver
  --compare-ascii ${EPSILON_3}     ${BASELINE_FILES}/feAlign.txt
  ${TEMP}/feAlign.txt
  otbAlignImageToPath
  ${INPUTDATA}/poupees.raw.hdr
  ${TEMP}/feAlign.txt)

otb_add_test(NAME feTvNonMaxRemovalByDirectionFilter COMMAND otbRoadExtractionTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvNonMaxRemovalByDirectionOutput.tif
  ${TEMP}/feTvNonMaxRemovalByDirectionOutput.tif
  otbNonMaxRemovalByDirectionFilter
  ${INPUTDATA}/InputForRoadDetection_WrongRem.raw.hdr
  ${INPUTDATA}/InputForRoadDetectionScalarProductDir.raw.hdr
  ${TEMP}/feTvNonMaxRemovalByDirectionOutput.tif
  )

otb_add_test(NAME feTvRoadExtractionFilter COMMAND otbRoadExtractionTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvOutputRoadDetection.tif
  ${TEMP}/feTvOutputRoadDetection.tif
  otbRoadExtractionFilter
  ${INPUTDATA}/qb_RoadExtract.img.hdr
  ${TEMP}/feTvOutputRoadDetection.tif
  337 557 432 859  # ReferencePixel
  1.0              # Alpha
  0.00005          # AmplitudeThreshold
  1.0              # Tolerance
  22.5             # (degre) MaxAngle (3.14159265358979323846/8.)
  1.0              # FirstMeanDistanceThreshold
  10.0             # SecondMeanDistanceThreshold
  25.           # DistanceThreshold for LinkPathFiler
  22.5           # AngularThreshold for LinkPathFiler
  )

otb_add_test(NAME feTvLinkPathListFilter COMMAND otbRoadExtractionTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvLinkPathOutput.txt
  ${TEMP}/feTvLinkPathOutput.txt
  otbLinkPathListFilter
  ${TEMP}/feTvLinkPathOutput.txt
  30  40
  1 1 51 1 |
  61 1 91 1 |
  111 11 211 11 |
  1 51 51 51 |
  71 71 71 91 |
  71 131 71 201 |
  141 111 101 111 |
  151 111 191 111 |
  111  71 141  71 |
  181  71 151  71 |
  11 131  11 101 |
  11 171  11 141
  )

otb_add_test(NAME feTvRemoveTortuousPathListFilter COMMAND otbRoadExtractionTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvRemoveTortuousPathOutput.txt
  ${TEMP}/feTvRemoveTortuousPathOutput.txt
  otbRemoveTortuousPathListFilter
  ${TEMP}/feTvRemoveTortuousPathOutput.txt
  10.0
  1 1   1 11  |
  1 1   5  1  7  1  11  1  |
  1 1   7  7 21 21  31 31 41 41 |
  1 1   5  1  7  1  11  1  21 11  31 11 41 11 58 11 70 11
  )

otb_add_test(NAME feTvParallelLinePathListFilter COMMAND otbRoadExtractionTestDriver
  otbParallelLinePathListFilter)

otb_add_test(NAME feTvRemoveIsolatedByDirectionFilter COMMAND otbRoadExtractionTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvRemoveIsolatedByDirectionOutput.tif
  ${TEMP}/feTvRemoveIsolatedByDirectionOutput.tif
  otbRemoveIsolatedByDirectionFilter
  ${INPUTDATA}/InputForRoadDetectionScalarProductMod.raw.hdr
  ${INPUTDATA}/InputForRoadDetectionScalarProductDir.raw.hdr
  ${TEMP}/feTvRemoveIsolatedByDirectionOutput.tif
  )

otb_add_test(NAME fzTvVectorDataToRoadDescriptionFilter COMMAND otbRoadExtractionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
  ${TEMP}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
  otbVectorDataToRoadDescriptionFilter
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
  ${TEMP}/fzTvVectorDataToRoadDescriptionFilterOutput.shp
  ${INPUTDATA}/DEM/srtm_directory
  0
  )

otb_add_test(NAME feTpSimplifyManyPathListFilter COMMAND otbRoadExtractionTestDriver
  otbSimplifyManyPathListFilter
  ${TEMP}/feTvSimplifyManyPathOutput.txt
  1.0
  )

otb_add_test(NAME feTvNeighborhoodScalarProductFilter COMMAND otbRoadExtractionTestDriver
  --compare-n-images ${EPSILON_6} 2
  ${BASELINE}/feTvNeighborhoodScalarProductModulusOutput.tif
  ${TEMP}/feTvNeighborhoodScalarProductModulusOutput.tif
  ${BASELINE}/feTvNeighborhoodScalarProductDirectionOutput.tif
  ${TEMP}/feTvNeighborhoodScalarProductDirectionOutput.tif
  otbNeighborhoodScalarProductFilter
  ${INPUTDATA}/InputForRoadDetection_SpectralAngle.raw.hdr
  ${TEMP}/feTvNeighborhoodScalarProductModulusOutput.tif
  ${TEMP}/feTvNeighborhoodScalarProductDirectionOutput.tif
  1.0
  )

otb_add_test(NAME feTvDrawPathTestAlign COMMAND otbRoadExtractionTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feDrawPathAlignDeuxTraits.png
  ${TEMP}/feDrawPathAlignDeuxTraits.png
  otbDrawPathAlign
  ${INPUTDATA}/DeuxTraits.png
  ${TEMP}/feDrawPathAlignDeuxTraits.png)

otb_add_test(NAME feTvBreakAngularPathListFilter COMMAND otbRoadExtractionTestDriver
  --compare-ascii ${NOTOL}  ${BASELINE_FILES}/feBreakAngularPathListOutput.txt
  ${TEMP}/feBreakAngularPathListOutput.txt
  otbBreakAngularPathListFilter
  ${TEMP}/feBreakAngularPathListOutput.txt
  10 20 30 50 90 140 180 |
  1 1  5 1  7 1  11 1  21 1  31 1 |
  1 1  5 1  7 1  11 1  21 11  31 11 |
  1 1  5 1  7 1  11 1  21 11  31 11 41 11  |
  1 1  5 1  7 1  11 1  21 11  31 11 41 11 58 11 70 11 |
  1 11  5 11  7 11  11 11  21 1  31 1 41 1 |
  1 1  5 1  7 1  11 1  21 11  31 21 41 31  |
  1 1  5 1  5 11  5 21 |
  1 1  31 1 41 1
  )
