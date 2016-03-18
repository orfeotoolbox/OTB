otb_module_test()

set(OTBConversionTests
otbConversionTestDriver.cxx
otbVectorDataToLabelMapFilter.cxx
otbOGRDataSourceToLabelImageFilter.cxx
otbVectorDataToLabelMapFilterNew.cxx
otbLabelImageToVectorDataFilterNew.cxx
otbLabelImageToVectorDataFilter.cxx
otbLabelImageToOGRDataSourceFilter.cxx
otbVectorDataToLabelImageFilter.cxx
otbPolygonizationRasterizationTest.cxx
otbVectorDataRasterizeFilter.cxx
otbLabelImageRegionPruningFilter.cxx
otbLabelImageRegionMergingFilter.cxx
otbLabelMapToVectorDataFilter.cxx
otbLabelMapToVectorDataFilterNew.cxx
)

add_executable(otbConversionTestDriver ${OTBConversionTests})
target_link_libraries(otbConversionTestDriver ${OTBConversion-Test_LIBRARIES})
otb_module_target_label(otbConversionTestDriver)

# Tests Declaration

otb_add_test(NAME obTvVectorDataToLabelMapFilter COMMAND otbConversionTestDriver
  otbVectorDataToLabelMapFilter
  ${INPUTDATA}/vectorIOexample_gis_to_vec.shp
  ${TEMP}/vectordataToLabelMap.png)

otb_add_test(NAME coTuOGRDataSourceToLabelImageFilterNew COMMAND otbConversionTestDriver
  otbOGRDataSourceToLabelImageFilterNew
  )

otb_add_test(NAME coTvOGRDataSourceToLabelImageFilterSHP COMMAND otbConversionTestDriver
  --compare-image 0.0
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output.tif
  otbOGRDataSourceToLabelImageFilter
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${INPUTDATA}/QB_Toulouse_ortho.shp
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output.tif
  1 0 255
  )

otb_add_test(NAME bfTvOGRDataSourceToLabelImageFilterSHPForegroundMode COMMAND otbConversionTestDriver
  --compare-image 0.0
  ${BASELINE}/bfTvOGRDataSourceToLabelImageFilter_Output_ForegroundMode.tif
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output_ForegroundMode.tif
  otbOGRDataSourceToLabelImageFilter
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${INPUTDATA}/QB_Toulouse_ortho_partial.shp
  ${TEMP}/bfTvOGRDataSourceToLabelImageFilter_Output_ForegroundMode.tif
  0 0 255
  )

otb_add_test(NAME obTuVectorDataToLabelMapFilterNew COMMAND otbConversionTestDriver
  otbVectorDataToLabelMapFilterNew)

otb_add_test(NAME obTuLabelImageToVectorDataFilterNew COMMAND otbConversionTestDriver
  otbLabelImageToVectorDataFilterNew)

otb_add_test(NAME obTvLabelImageToVectorDataFilter COMMAND otbConversionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/obTuLabelImageToVectorDataFilter.shp
  ${TEMP}/obTuLabelImageToVectorDataFilter.shp
  otbLabelImageToVectorDataFilter
  ${INPUTDATA}/labelImage_UnsignedChar.tif
  ${TEMP}/obTuLabelImageToVectorDataFilter.shp
  )

otb_add_test(NAME obTuLabelImageToOGRDataSourceFilterNew COMMAND otbConversionTestDriver
  otbLabelImageToOGRDataSourceFilterNew)

otb_add_test(NAME obTvLabelImageToOGRDataSourceFilter COMMAND otbConversionTestDriver
  otbLabelImageToOGRDataSourceFilter
  ${INPUTDATA}/labelImage_UnsignedChar.tif
  )

otb_add_test(NAME bfTuVectorDataToLabelImageFilterNew COMMAND otbConversionTestDriver
  otbVectorDataToLabelImageFilterNew
  )

otb_add_test(NAME bfTvVectorDataToLabelImageFilterSHP COMMAND otbConversionTestDriver
  --compare-image 0.0
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${TEMP}/bfTvVectorDataToLabelImageFilter_Output.tif
  otbVectorDataToLabelImageFilter
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  ${INPUTDATA}/QB_Toulouse_ortho.shp
  ${TEMP}/bfTvVectorDataToLabelImageFilter_Output.tif
  )

otb_add_test(NAME bfTvPolygonizationRasterization_WGS84 COMMAND otbConversionTestDriver
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage_WGS84.tif
  )

otb_add_test(NAME bfTvPolygonizationRasterization_UTM COMMAND otbConversionTestDriver
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_ortho_labelImage.tif
  )

otb_add_test(NAME bfTvPolygonizationRasterization_Index COMMAND otbConversionTestDriver
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_labelImage_Index.tif
  )

otb_add_test(NAME bfTvPolygonizationRasterization_SensorModel COMMAND otbConversionTestDriver
  otbPolygonizationRasterizationTest
  ${INPUTDATA}/QB_Toulouse_SensorModel_labelImage.tif
  )

otb_add_test(NAME bfTvVectorDataRasterizeFilterKML COMMAND otbConversionTestDriver
  --compare-image 0.0001
  ${BASELINE}/bfTvVectorDataRasterizeFilter_OutputKML.tif
  ${TEMP}/bfTvVectorDataRasterizeFilter_OutputKML.tif
  otbVectorDataRasterizeFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${INPUTDATA}/Capitole-Shadows.kml
  ${TEMP}/bfTvVectorDataRasterizeFilter_OutputKML.tif
  )

otb_add_test(NAME bfTuVectorDataRasterizeFilterNew COMMAND otbConversionTestDriver
  otbVectorDataRasterizeFilterNew
  )

otb_add_test(NAME bfTvVectorDataRasterizeFilterSHP COMMAND otbConversionTestDriver
  --compare-image 0.0001
  ${BASELINE}/bfTvVectorDataRasterizeFilter_OutputSHP.tif
  ${TEMP}/bfTvVectorDataRasterizeFilter_OutputSHP.tif
  otbVectorDataRasterizeFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
  ${INPUTDATA}/Capitole-Shadows.shp
  ${TEMP}/bfTvVectorDataRasterizeFilter_OutputSHP.tif
  )

#otb_add_test(NAME bfTvLabelImageRegionPruningFilterQB_Suburb COMMAND otbConversionTestDriver
  #--compare-n-images ${EPSILON_7} 2
  #${BASELINE}/bfLabelImageRegionPruningFilterMergeOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionPruningFilterMergeOutput_QB_Suburb.tif
  #${BASELINE}/bfLabelImageRegionPruningFilterClusteredOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionPruningFilterClusteredOutput_QB_Suburb.tif
  #otbLabelImageRegionPruningFilter
  #${INPUTDATA}/QB_Suburb.png
  #${TEMP}/bfLabelImageRegionPruningFilterSpectralOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionPruningFilterLabelOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionPruningFilterMergeOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionPruningFilterClusteredOutput_QB_Suburb.tif
  #4 25 0.1 100 10
  #)

#otb_add_test(NAME bfTvLabelImageRegionPruningFilterSPOT5 COMMAND otbConversionTestDriver
  #--compare-n-images ${EPSILON_7} 2
  #${BASELINE}/bfLabelImageRegionPruningFilterMergeOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionPruningFilterMergeOutput_SPOT5.tif
  #${BASELINE}/bfLabelImageRegionPruningFilterClusteredOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionPruningFilterClusteredOutput_SPOT5.tif
  #otbLabelImageRegionPruningFilter
  #${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
  #${TEMP}/bfLabelImageRegionPruningFilterSpectralOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionPruningFilterLabelOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionPruningFilterMergeOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionPruningFilterClusteredOutput_SPOT5.tif
  #4 25 0.1 100 20
  #)

#otb_add_test(NAME bfTvLabelImageRegionMergingFilterQB_Suburb COMMAND otbConversionTestDriver
  #--compare-n-images ${EPSILON_7} 2
  #${BASELINE}/bfLabelImageRegionMergingFilterMergeOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionMergingFilterMergeOutput_QB_Suburb.tif
  #${BASELINE}/bfLabelImageRegionMergingFilterClusteredOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionMergingFilterClusteredOutput_QB_Suburb.tif
  #otbLabelImageRegionMergingFilter
  #${INPUTDATA}/QB_Suburb.png
  #${TEMP}/bfLabelImageRegionMergingFilterSpectralOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionMergingFilterLabelOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionMergingFilterMergeOutput_QB_Suburb.tif
  #${TEMP}/bfLabelImageRegionMergingFilterClusteredOutput_QB_Suburb.tif
  #4 25 0.1 100
  #)

#otb_add_test(NAME bfTvLabelImageRegionMergingFilterSPOT5 COMMAND otbConversionTestDriver
  #--compare-n-images ${EPSILON_7} 2
  #${BASELINE}/bfLabelImageRegionMergingFilterMergeOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionMergingFilterMergeOutput_SPOT5.tif
  #${BASELINE}/bfLabelImageRegionMergingFilterClusteredOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionMergingFilterClusteredOutput_SPOT5.tif
  #otbLabelImageRegionMergingFilter
  #${INPUTDATA}/SPOT5_EXTRACTS/Arcachon/Arcachon_extrait_3852_3319_546_542.tif
  #${TEMP}/bfLabelImageRegionMergingFilterSpectralOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionMergingFilterLabelOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionMergingFilterMergeOutput_SPOT5.tif
  #${TEMP}/bfLabelImageRegionMergingFilterClusteredOutput_SPOT5.tif
  #4 25 0.1 100
  #)

otb_add_test(NAME obTvLabelMapToVectorDataFilter2 COMMAND otbConversionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/obTvLabelMapToVectorDataFilter.shp
  ${TEMP}/obTvLabelMapToVectorDataFilter2.shp
  otbLabelMapToVectorDataFilter
  ${INPUTDATA}/rcc8_mire5.png
  ${TEMP}/obTvLabelMapToVectorDataFilter2.shp)

otb_add_test(NAME obTvLabelMapToVectorDataFilter COMMAND otbConversionTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/obTvLabelMapToVectorDataFilter.shp
  ${TEMP}/obTvLabelMapToVectorDataFilter.shp
  otbLabelMapToVectorDataFilter
  ${INPUTDATA}/rcc8_mire1.png
  ${TEMP}/obTvLabelMapToVectorDataFilter.shp)

otb_add_test(NAME obTuLabelMapToVectorDataFilterNew COMMAND otbConversionTestDriver
  otbLabelMapToVectorDataFilterNew)

