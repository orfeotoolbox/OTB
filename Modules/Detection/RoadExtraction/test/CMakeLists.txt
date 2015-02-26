otb_module_test()

set(OTBRoadExtractionTests
otbRoadExtractionTestDriver.cxx
otbLikelihoodPathListFilter.cxx
otbParallelLinePathListFilterNew.cxx
otbRemoveWrongDirectionFilter.cxx
otbSimplifyPathListFilter.cxx
otbRoadExtractionFilterNew.cxx
otbRemoveIsolatedByDirectionFilterNew.cxx
otbAlignImageToPath.cxx
otbNonMaxRemovalByDirectionFilterNew.cxx
otbNeighborhoodScalarProductFilterNew.cxx
otbRemoveTortuousPathListFilterNew.cxx
otbNonMaxRemovalByDirectionFilter.cxx
otbRemoveWrongDirectionFilterNew.cxx
otbRoadExtractionFilter.cxx
otbLinkPathListFilter.cxx
otbModulusAndDirectionImageFiltersNew.cxx
otbSimplifyPathListFilterNew.cxx
otbRemoveTortuousPathListFilter.cxx
otbLinkPathListFilterNew.cxx
otbLikelihoodPathListFilterNew.cxx
otbParallelLinePathListFilter.cxx
otbRemoveIsolatedByDirectionFilter.cxx
otbVectorDataToRoadDescriptionFilter.cxx
otbSimplifyManyPathListFilter.cxx
otbNeighborhoodScalarProductFilter.cxx
otbDrawPathAlign.cxx
otbBreakAngularPathListFilter.cxx
otbBreakAngularPathListFilterNew.cxx
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
  ${INPUTDATA}/InputForRoadDetection_NonMaxRem.hdr
  ${TEMP}/feTvLikelihoodPathOutput.txt
  157 335 204 376 |
  21 305 35 308 77 354 85 358 |
  73 160 126 173
  )

otb_add_test(NAME feTuParallelLinePathListFilterNew COMMAND otbRoadExtractionTestDriver
  otbParallelLinePathListFilterNew)

otb_add_test(NAME feTvRemoveWrongDirectionFilter COMMAND otbRoadExtractionTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvRemoveWrongDirectionOutput.hdr
  ${TEMP}/feTvRemoveWrongDirectionOutput.hdr
  otbRemoveWrongDirectionFilter
  ${INPUTDATA}/InputForRoadDetection_IsolatedRem.hdr
  ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
  ${TEMP}/feTvRemoveWrongDirectionOutput.hdr
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

otb_add_test(NAME feTuRoadExtractionFilterNew COMMAND otbRoadExtractionTestDriver
  otbRoadExtractionFilterNew)

otb_add_test(NAME feTuRemoveIsolatedByDirectionFilterNew COMMAND otbRoadExtractionTestDriver
  otbRemoveIsolatedByDirectionFilterNew)

otb_add_test(NAME feTvAlignMV2ITK COMMAND otbRoadExtractionTestDriver
  --compare-ascii ${EPSILON_3}     ${BASELINE_FILES}/feAlign.txt
  ${TEMP}/feAlign.txt
  otbAlignImageToPath
  ${INPUTDATA}/poupees.hdr
  ${TEMP}/feAlign.txt)

otb_add_test(NAME feTuNonMaxRemovalByDirectionFilterNew COMMAND otbRoadExtractionTestDriver
  otbNonMaxRemovalByDirectionFilterNew)

otb_add_test(NAME feTuNeighborhoodScalarProductFilterNew COMMAND otbRoadExtractionTestDriver
  otbNeighborhoodScalarProductFilterNew)

otb_add_test(NAME feTuRemoveTortuousPathListFilterNew COMMAND otbRoadExtractionTestDriver
  otbRemoveTortuousPathListFilterNew)

otb_add_test(NAME feTvNonMaxRemovalByDirectionFilter COMMAND otbRoadExtractionTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvNonMaxRemovalByDirectionOutput.hdr
  ${TEMP}/feTvNonMaxRemovalByDirectionOutput.hdr
  otbNonMaxRemovalByDirectionFilter
  ${INPUTDATA}/InputForRoadDetection_WrongRem.hdr
  ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
  ${TEMP}/feTvNonMaxRemovalByDirectionOutput.hdr
  )

otb_add_test(NAME feTuRemoveWrongDirectionFilterNew COMMAND otbRoadExtractionTestDriver
  otbRemoveWrongDirectionFilterNew)

otb_add_test(NAME feTvRoadExtractionFilter COMMAND otbRoadExtractionTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvOutputRoadDetection.hdr
  ${TEMP}/feTvOutputRoadDetection.hdr
  otbRoadExtractionFilter
  ${INPUTDATA}/qb_RoadExtract.img.hdr
  ${TEMP}/feTvOutputRoadDetection.hdr
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

otb_add_test(NAME feTuModulusAndDirectionImageFiltersNew COMMAND otbRoadExtractionTestDriver
  otbModulusAndDirectionImageFiltersNew)

otb_add_test(NAME feTuSimplifyPathListFilterNew COMMAND otbRoadExtractionTestDriver
  otbSimplifyPathListFilterNew)

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

otb_add_test(NAME feTuLinkPathListFilterNew COMMAND otbRoadExtractionTestDriver
  otbLinkPathListFilterNew)

otb_add_test(NAME feTuLikelihoodPathListFilterNew COMMAND otbRoadExtractionTestDriver
  otbLikelihoodPathListFilterNew)

otb_add_test(NAME feTvParallelLinePathListFilter COMMAND otbRoadExtractionTestDriver
  otbParallelLinePathListFilter)

otb_add_test(NAME feTvRemoveIsolatedByDirectionFilter COMMAND otbRoadExtractionTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvRemoveIsolatedByDirectionOutput.hdr
  ${TEMP}/feTvRemoveIsolatedByDirectionOutput.hdr
  otbRemoveIsolatedByDirectionFilter
  ${INPUTDATA}/InputForRoadDetectionScalarProductMod.hdr
  ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
  ${TEMP}/feTvRemoveIsolatedByDirectionOutput.hdr
  )

otb_add_test(NAME fzTuVectorDataToRoadDescriptionFilterNew COMMAND otbRoadExtractionTestDriver
  otbVectorDataToRoadDescriptionFilterNew)

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
  ${BASELINE}/feTvNeigborhoodScalarProductModulusOutput.hdr
  ${TEMP}/feTvNeigborhoodScalarProductModulusOutput.hdr
  ${BASELINE}/feTvNeigborhoodScalarProductDirectionOutput.hdr
  ${TEMP}/feTvNeigborhoodScalarProductDirectionOutput.hdr
  otbNeighborhoodScalarProductFilter
  ${INPUTDATA}/InputForRoadDetection_SpectralAngle.hdr
  ${TEMP}/feTvNeigborhoodScalarProductModulusOutput.hdr
  ${TEMP}/feTvNeigborhoodScalarProductDirectionOutput.hdr
  1.0
  )

otb_add_test(NAME feTvDrawPathTestAlign COMMAND otbRoadExtractionTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feDrawPathAlignDeuxTraits.png
  ${TEMP}/feDrawPathAlignDeuxTraits.png
  otbDrawPathAlign
  ${INPUTDATA}/DeuxTraits.png
  ${TEMP}/feDrawPathAlignDeuxTraits.png)

otb_add_test(NAME feTuBreakAngularPathListFilterNew COMMAND otbPathTestDriver
  otbBreakAngularPathListFilterNew)
