otb_module_test()

set(OTBIndicesTests
otbIndicesTestDriver.cxx
otbRAndGAndNIRVegetationIndexImageFilterNew.cxx
otbAVIMultiChannelRAndGAndNIRVegetationIndexImageFilter.cxx
otbNDVIDataNodeFeatureFunction.cxx
otbRAndNIRVegetationIndexImageFilterNew.cxx
otbMultiChannelRAndBAndNIRVegetationIndexImageFilterNew.cxx
otbLandsatTMIndexNDSITest.cxx
otbLandsatTMIndexBIOTest.cxx
otbLAIFromReflectancesLinearFunctorTest.cxx
otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter.cxx
otbWDVIRAndNIRVegetationIndexImageFilter.cxx
otbGAndRIndexImageFilter.cxx
otbRAndBAndNIRVegetationIndexImageFilter.cxx
otbIBMultiChannelGAndRAndNIRIndexImageFilter.cxx
otbLandsatTMIndexMIR2Test.cxx
otbLandsatTMIndexNDVITest.cxx
otbISURAndNIRIndexImageFilter.cxx
otbLAIFromNDVILogarithmicFunctorTest.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilter.cxx
otbLandsatTMIndexVisTest.cxx
otbWaterSqrtSpectralAngleImageFilter.cxx
otbMultiChannelRAndGAndNIRIndexImageFilterNew.cxx
otbLandsatTMIndexBrightTest.cxx
otbMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx
otbMultiChannelRAndNIRVegetationIndexImageFilterNew.cxx
otbLandsatTMBrightTest.cxx
otbLandsatTMThinCloudTest.cxx
otbISUMultiChannelRAndNIRIndexImageFilter.cxx
otbAVIRAndGAndNIRVegetationIndexImageFilter.cxx
otbMultiChannelGAndRIndexImageFilter.cxx
otbLandsatTMIndexMIRTIRTest.cxx
otbLandsatTMIndexMIR1Test.cxx
otbRAndNIRVegetationIndexImageFilter.cxx
otbLandsatTMVegetationTest.cxx
otbEVIRAndBAndNIRVegetationIndexImageFilter.cxx
otbNDBITM4AndTM5IndexImageFilter.cxx
otbLandsatTMIndexNDBBBITest.cxx
otbIBGAndRAndNIRIndexImageFilter.cxx
otbGAndRIndexImageFilterNew.cxx
otbEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx
otbNDBIMultiChannelTM4AndTM5IndexImageFilter.cxx
otbMultiChannelGAndRIndexImageFilterNew.cxx
otbNDWIWaterIndexImageFilter.cxx
otbLandsatTMIndexTIRTest.cxx
otbTSARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter.cxx
otbLandsatTMLinguisticLabelsTest.cxx
otbNDWIMultiChannelWaterIndexImageFilter.cxx
otbWDVIMultiChannelRAndNIRVegetationIndexImageFilter.cxx
otbLandsatTMIndexNDSIVisTest.cxx
otbRAndBAndNIRVegetationIndexImageFilterNew.cxx
otbLandsatTMIndexNIRTest.cxx
otbSetASetBRAndNIRVegetationIndexImageFilter.cxx
otbLandsatTMKernelSpectralRules.cxx
otbLandsatTMIndexNDBSITest.cxx
otbTSARVIRAndBAndNIRVegetationIndexImageFilter.cxx
otbLandsatTMThickCloudTest.cxx
)

add_executable(otbIndicesTestDriver ${OTBIndicesTests})
target_link_libraries(otbIndicesTestDriver ${OTBIndices-Test_LIBRARIES})
otb_module_target_label(otbIndicesTestDriver)

# Tests Declaration

otb_add_test(NAME raTuRAndGAndNIRVegetationIndexImageFilterNew COMMAND otbIndicesTestDriver
  otbRAndGAndNIRVegetationIndexImageFilterNew )

otb_add_test(NAME raTvAVI_MultiChannelRAndGAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raMultiChannelRAndGAndNIRVegetationIndex_AVI_qb_RoadExtract.tif
  ${TEMP}/raMultiChannelRAndGAndNIRVegetationIndex_AVI_qb_RoadExtract.tif
  otbAVIMultiChannelRAndGAndNIRVegetationIndexImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raMultiChannelRAndGAndNIRVegetationIndex_AVI_qb_RoadExtract.tif
  3   # red
  2   # green
  4   # nir
  660.  # lambda r
  560.  # lambda g
  830.  # lambda nir
  )

otb_add_test(NAME bfTvNDVIDataNodeFeatureFunction_Polygon COMMAND otbIndicesTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  otbNDVIDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_BUILDINGS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Polygon.shp
  0
  )

otb_add_test(NAME bfTvNDVIDataNodeFeatureFunction_Line COMMAND otbIndicesTestDriver
  --compare-ogr ${NOTOL}
  ${BASELINE_FILES}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  otbNDVIDataNodeFeatureFunction
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE_ROADS.shp
  ${INPUTDATA}/Dempster-Shafer/ROI_QB_TOULOUSE.TIF
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/bfTvNDVIDataNodeFeatureFunctionOutput_Line.shp
  0
  )

otb_add_test(NAME bfTuNDVIDataNodeFeatureFunctionNew COMMAND otbIndicesTestDriver
  otbNDVIDataNodeFeatureFunctionNew
  )

otb_add_test(NAME raTuRAndNIRVegetationIndexImageFilterNew COMMAND otbIndicesTestDriver
  otbRAndNIRVegetationIndexImageFilterNew )

otb_add_test(NAME raTuMultiChannelRAndBAndNIRVegetationIndexImageFilterNew COMMAND otbIndicesTestDriver
  otbMultiChannelRAndBAndNIRVegetationIndexImageFilterNew )

otb_add_test(NAME raTvLandsatTMIndexNDSITest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDSI
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMIndexBIOTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexBIO
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLAIFromReflectancesLinearFunctorTest COMMAND otbIndicesTestDriver
  otbLAIFromReflectancesLinear
  3   # red
  4   # nir
  -18   # red coef
  13   # nir coef
  )

otb_add_test(NAME raTvPVI_MultiChannelRAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raMultiChannelRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raMultiChannelRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
  otbSetASetBMultiChannelRAndNIRVegetationIndexImageFilter
  PVI
  0.7
  0.9
  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raMultiChannelRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
  3 4
  )

otb_add_test(NAME raTvWDVI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_WDVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_WDVI_verySmallFSATSWcRcNir.tif
  otbWDVIRAndNIRVegetationIndexImageFilter
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_WDVI_verySmallFSATSWcRcNir.tif
  2.0   # g : slope of soil line
  )

otb_add_test(NAME raTvIR_GAndRIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raGAndRIndexImageFilter_IR_verySmallFSATSW.tif
  ${TEMP}/raGAndRIndexImageFilter_IR_verySmallFSATSW.tif
  otbGAndRIndexImageFilter
  IR
  ${INPUTDATA}/verySmallFSATSW_g.tif
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${TEMP}/raGAndRIndexImageFilter_IR_verySmallFSATSW.tif
  )

otb_add_test(NAME raTvIB_GAndRIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raGAndRIndexImageFilter_IB_verySmallFSATSW.tif
  ${TEMP}/raGAndRIndexImageFilter_IB_verySmallFSATSW.tif
  otbGAndRIndexImageFilter
  IB
  ${INPUTDATA}/verySmallFSATSW_g.tif
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${TEMP}/raGAndRIndexImageFilter_IB_verySmallFSATSW.tif
  )

otb_add_test(NAME raTvIC_GAndRIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raGAndRIndexImageFilter_IC_verySmallFSATSW.tif
  ${TEMP}/raGAndRIndexImageFilter_IC_verySmallFSATSW.tif
  otbGAndRIndexImageFilter
  IC
  ${INPUTDATA}/verySmallFSATSW_g.tif
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${TEMP}/raGAndRIndexImageFilter_IC_verySmallFSATSW.tif
  )

otb_add_test(NAME raTvARVI_RAndBAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
  otbRAndBAndNIRVegetationIndexImageFilter
  ARVI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_b.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
  0.6 # Gamma parameter
  )

otb_add_test(NAME raTvIB2_MultiChannelGAndRIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_6}   ${BASELINE}/raMultiChannelGAndRAndNIRIndex_IB2_Romania_Extract.tif
  ${TEMP}/raMultiChannelGAndRAndNIRIndex_IB2_Romania_Extract.tif
  otbIBMultiChannelGAndRAndNIRIndexImageFilter
  ${INPUTDATA}/Romania_Extract.tif
  ${TEMP}/raMultiChannelGAndRAndNIRIndex_IB2_Romania_Extract.tif
  2 1 3
  )

otb_add_test(NAME raTvLandsatTMIndexMIR2Test COMMAND otbIndicesTestDriver
  otbLandsatTMIndexMIR2
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMIndexNDVITest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDVI
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvISU_RAndNIRBuiltUpIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRBuiltUpIndex_ISU_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRBuiltUpIndex_ISU_verySmallFSATSWcRcNir.tif
  otbISURAndNIRIndexImageFilter
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRBuiltUpIndex_ISU_verySmallFSATSWcRcNir.tif
  100.
  25.
  )

otb_add_test(NAME raTvLAIFromNDVILogarithmicFunctorTest COMMAND otbIndicesTestDriver
  otbLAIFromNDVILogarithmic
  3   # red
  4   # nir
  0.12   # ndvi soil
  0.91   # ndvi infinity
  0.70   # extinction coefficient
  )

otb_add_test(NAME raTvLAIFromNDVIFormosat2_MultiChannelRAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_8}   ${BASELINE}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
  ${TEMP}/raMultiChannelRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
  otbMultiChannelRAndNIRVegetationIndexImageFilter
  LAIFormosat2
  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raMultiChannelRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
  1
  4
  )

otb_add_test(NAME raTvLandsatTMIndexVisTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexVis
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvWaterSqrtSpectralAngleImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raWater_SqrtSpectralAngle.tif
  ${TEMP}/raWater_SqrtSpectralAngle.tif
  otbWaterSqrtSpectralAngleImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raWater_SqrtSpectralAngle.tif
  0 1 2 3
  )

otb_add_test(NAME raTuMultiChannelRAndGAndNIRIndexImageFilterNew COMMAND otbIndicesTestDriver
  otbMultiChannelRAndGAndNIRIndexImageFilterNew )

otb_add_test(NAME raTvLandsatTMIndexBrightTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexBright
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvARVI_MultiChannelRAndBAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
  ${TEMP}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
  otbMultiChannelRAndBAndNIRVegetationIndexImageFilter
  ARVI
  ${INPUTDATA}/verySmallFSATSW.tif
  ${TEMP}/raMultiChannelRAndBAndNIRVegetationIndex_ARVI_verySmallFSATSWcRcBcNir.tif
  3 1 4
  0.6 # Gamma parameter
  )

otb_add_test(NAME raTvLandsatTMBrightTest COMMAND otbIndicesTestDriver
  otbLandsatTMBrightTest
  ${INPUTDATA}/L5Strip.tif
  ${TEMP}/raTvLandsatTMBrightTest_cloudImage.tif
  )

otb_add_test(NAME raTvLandsatTMThinCloudTest COMMAND otbIndicesTestDriver
  otbLandsatTMThinCloudTest
  ${INPUTDATA}/L5Strip.tif
  ${TEMP}/raTvLandsatTMThinCloudTest_cloudImage.tif
  )

otb_add_test(NAME raTvISU_MultiChannelRAndNIRBuiltUpIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRBuiltUpIndex_ISU_qb_RoadExtract.tif
  ${TEMP}/raRAndNIRBuiltUpIndex_ISU_qb_RoadExtract.tif
  otbISUMultiChannelRAndNIRIndexImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raRAndNIRBuiltUpIndex_ISU_qb_RoadExtract.tif
  3 4 # red nir channels
  100.
  25.
  )

otb_add_test(NAME raTvAVI_RAndGAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndGAndNIRVegetationIndex_AVI_verySmallFSATSWcRcGcNir.tif
  ${TEMP}/raRAndGAndNIRVegetationIndex_AVI_verySmallFSATSWcRcGcNir.tif
  otbAVIRAndGAndNIRVegetationIndexImageFilter
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_g.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndGAndNIRVegetationIndex_AVI_verySmallFSATSWcRcGcNir.tif
  660.  # lambda r
  560.  # lambda g
  830.  # lambda nir
  )

otb_add_test(NAME raTvIB_MultiChannelGAndRIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_10}   ${BASELINE}/raMultiChannelGAndRIndex_IB_Romania_Extract.tif
  ${TEMP}/raMultiChannelGAndRIndex_IB_Romania_Extract.tif
  otbMultiChannelGAndRIndexImageFilter
  IB
  ${INPUTDATA}/Romania_Extract.tif
  ${TEMP}/raMultiChannelGAndRIndex_IB_Romania_Extract.tif
  1 2
  )

otb_add_test(NAME raTvIR_MultiChannelGAndRIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_10}   ${BASELINE}/raMultiChannelGAndRIndex_IR_Romania_Extract.tif
  ${TEMP}/raMultiChannelGAndRIndex_IR_Romania_Extract.tif
  otbMultiChannelGAndRIndexImageFilter
  IR
  ${INPUTDATA}/Romania_Extract.tif
  ${TEMP}/raMultiChannelGAndRIndex_IR_Romania_Extract.tif
  1 2
  )

otb_add_test(NAME raTvIC_MultiChannelGAndRIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/raMultiChannelGAndRIndex_IC_Romania_Extract.tif
  ${TEMP}/raMultiChannelGAndRIndex_IC_Romania_Extract.tif
  otbMultiChannelGAndRIndexImageFilter
  IC
  ${INPUTDATA}/Romania_Extract.tif
  ${TEMP}/raMultiChannelGAndRIndex_IC_Romania_Extract.tif
  1 2
  )

otb_add_test(NAME raTvLandsatTMIndexMIRTIRTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexMIRTIR
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMIndexMIR1Test COMMAND otbIndicesTestDriver
  otbLandsatTMIndexMIR1
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvMSAVI2_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_8}   ${BASELINE}/raRAndNIRVegetationIndex_MSAVI2_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_MSAVI2_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  MSAVI2
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_MSAVI2_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvRVI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/raRAndNIRVegetationIndex_RVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_RVI_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  RVI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_RVI_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvMSAVI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_8}   ${BASELINE}/raRAndNIRVegetationIndex_MSAVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_MSAVI_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  MSAVI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_MSAVI_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvNDVI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/raRAndNIRVegetationIndex_NDVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_NDVI_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  NDVI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_NDVI_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvTSAVI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_TSAVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_TSAVI_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  TSAVI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_TSAVI_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvIPVI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_IPVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_IPVI_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  IPVI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_IPVI_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvTNDVI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_TNDVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_TNDVI_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  TNDVI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_TNDVI_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvSAVI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${NOTOL}   ${BASELINE}/raRAndNIRVegetationIndex_SAVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_SAVI_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  SAVI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_SAVI_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvLAIFromNDVIFormosat2_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  LAIFormosat2
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_LAIFromNDVIFormosat2_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvGEMI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_GEMI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_GEMI_verySmallFSATSWcRcNir.tif
  otbRAndNIRVegetationIndexImageFilter
  GEMI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_GEMI_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvLandsatTMVegetationTest COMMAND otbIndicesTestDriver
  otbLandsatTMVegetationTest
  ${INPUTDATA}/L5Strip.tif
  ${TEMP}/raTvLandsatTMVegetationTest_Image.tif
  )

otb_add_test(NAME raTvEVI_RAndBAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_EVI_verySmallFSATSWcRcBcNir.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_verySmallFSATSWcRcBcNir.tif
  otbEVIRAndBAndNIRVegetationIndexImageFilter
  EVI
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_b.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_verySmallFSATSWcRcBcNir.tif
  2.5
  6.0
  7.5
  1.0
  )

otb_add_test(NAME raTvNDBI_TM4AndTM5BuiltUpIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raTM4AndTM5BuiltUpIndex_NDBI_verySmallFSATSW.tif
  ${TEMP}/raTM4AndTM5BuiltUpIndex_NDBI_verySmallFSATSW.tif
  otbNDBITM4AndTM5IndexImageFilter
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raTM4AndTM5BuiltUpIndex_NDBI_verySmallFSATSW.tif
  )

otb_add_test(NAME raTvLandsatTMIndexNDBBBITest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDBBBI
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvIB2_GAndRAndNIRIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raGAndRAndNIRIndexImageFilter_IB2_verySmallFSATSW.tif
  ${TEMP}/raGAndRAndNIRIndexImageFilter_IB2_verySmallFSATSW.tif
  otbIBGAndRAndNIRIndexImageFilter
  ${INPUTDATA}/verySmallFSATSW_g.tif
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raGAndRAndNIRIndexImageFilter_IB2_verySmallFSATSW.tif
  )

otb_add_test(NAME raTuGAndRIndexImageFilterNew COMMAND otbIndicesTestDriver
  otbGAndRIndexImageFilterNew )

otb_add_test(NAME raTvEVI_MultiChannelRAndBAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_EVI_qb_RoadExtract.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_qb_RoadExtract.tif
  otbEVIMultiChannelRAndBAndNIRVegetationIndexImageFilter
  EVI
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_EVI_qb_RoadExtract.tif
  3   # red
  1   # blue
  4   # nir
  10.0    # gain factor
  30.0    # coefficient of the aerosol resistance term
  7.5     # coefficient of the aerosol resistance term
  400.0   # canopy  background adjustment
  )

otb_add_test(NAME raTvNDBI_MultiChannelTM4AndTM5BuiltUpIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raTM4AndTM5BuiltUpIndex_NDBI_qb_RoadExtract.tif
  ${TEMP}/raTM4AndTM5BuiltUpIndex_NDBI_qb_RoadExtract.tif
  otbNDBIMultiChannelTM4AndTM5IndexImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raTM4AndTM5BuiltUpIndex_NDBI_qb_RoadExtract.tif
  3   # red
  4   # nir
  )

otb_add_test(NAME raTuMultiChannelGAndRIndexImageFilterNew COMMAND otbIndicesTestDriver
  otbMultiChannelGAndRIndexImageFilterNew )

otb_add_test(NAME raTvNDWI_WaterIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raWaterIndex_NDWI_verySmallFSATSW.tif
  ${TEMP}/raWaterIndex_NDWI_verySmallFSATSW.tif
  otbNDWIWaterIndexImageFilter
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raWaterIndex_NDWI_verySmallFSATSW.tif
  )

otb_add_test(NAME raTvLandsatTMIndexTIRTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexTIR
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvTSARVI_MultiChannelRAndBAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_TSARVI_qb_RoadExtract.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_qb_RoadExtract.tif
  otbTSARVIMultiChannelRAndBAndNIRVegetationIndexImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_qb_RoadExtract.tif
  3   # red
  1   # blue
  4   # nir
  0.7   # a ( pente de la droite des sols nus dans l'espace RB/PIR )
  0.9   # b ( ordonnee a l'origine de la droite des sols nus dans l'espace RB/PIR )
  0.08  # x coeff a priori constant
  0.5   # gamma
  )

otb_add_test(NAME raTvLandsatTMLinguisticLabelsTest COMMAND otbIndicesTestDriver
  otbLandsatTMLinguisticLabels
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvLandsatTMLinguisticLabelsTest2 COMMAND otbIndicesTestDriver
  otbLandsatTMLinguisticLabels
  3   #TM1
  4   #TM2
  80   #TM3
  100  #TM4
  33  #TM5
  1  #TM61
  8   #TM62
  51  #TM7
  )

otb_add_test(NAME raTvNDWI_MultiChannelWaterIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raMultiChannelWaterIndex_NDWI_qb_RoadExtract.tif
  ${TEMP}/raMultiChannelWaterIndex_NDWI_qb_RoadExtract.tif
  otbNDWIMultiChannelWaterIndexImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raMultiChannelWaterIndex_NDWI_qb_RoadExtract.tif
  3 4 # mir nir channels
  )

otb_add_test(NAME raTvWDVI_MultiChannelRAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_WDVI_qb_RoadExtract.tif
  ${TEMP}/raRAndNIRVegetationIndex_WDVI_qb_RoadExtract.tif
  otbWDVIMultiChannelRAndNIRVegetationIndexImageFilter
  ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
  ${TEMP}/raRAndNIRVegetationIndex_WDVI_qb_RoadExtract.tif
  3   # red
  4   # nir
  2.0   # g : slope of soil line
  )

otb_add_test(NAME raTvLandsatTMIndexNDSIVisTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDSIVis
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTuRAndBAndNIRVegetationIndexImageFilterNew COMMAND otbIndicesTestDriver
  otbRAndBAndNIRVegetationIndexImageFilterNew )

otb_add_test(NAME raTvLandsatTMIndexNIRTest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNIR
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvPVI_RAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
  ${TEMP}/raRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
  otbSetASetBRAndNIRVegetationIndexImageFilter
  PVI
  0.7
  0.9
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndNIRVegetationIndex_PVI_verySmallFSATSWcRcNir.tif
  )

otb_add_test(NAME raTvLandsatTMKernelSpectralRulesTest2 COMMAND otbIndicesTestDriver
  otbLandsatTMKernelSpectralRules
  49   #TM1
  50   #TM2
  70   #TM3
  100  #TM4
  69  #TM5
  0  #TM61
  0   #TM62
  0  #TM7
  )

otb_add_test(NAME raTvLandsatTMKernelSpectralRulesTest COMMAND otbIndicesTestDriver
  otbLandsatTMKernelSpectralRules
  63   #TM1
  64   #TM2
  57   #TM3
  150  #TM4
  33  #TM5
  1  #TM61
  8   #TM62
  51  #TM7
  )

otb_add_test(NAME raTvLandsatTMKernelSpectralRulesWithImageTest COMMAND otbIndicesTestDriver
  otbLandsatTMKernelSpectralRulesWithImage
  ${INPUTDATA}/L5Strip.tif
  )

otb_add_test(NAME raTvLandsatTMIndexNDBSITest COMMAND otbIndicesTestDriver
  otbLandsatTMIndexNDBSI
  3   #TM1
  4   #TM2
  8   #TM3
  21  #TM4
  33  #TM5
  41  #TM61
  8   #TM62
  21  #TM7
  )

otb_add_test(NAME raTvTSARVI_RAndBAndNIRVegetationIndexImageFilter COMMAND otbIndicesTestDriver
  --compare-image ${EPSILON_12}   ${BASELINE}/raRAndBAndNIRVegetationIndex_TSARVI_verySmallFSATSWcRcBcNir.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_verySmallFSATSWcRcBcNir.tif
  otbTSARVIRAndBAndNIRVegetationIndexImageFilter
  ${INPUTDATA}/verySmallFSATSW_r.tif
  ${INPUTDATA}/verySmallFSATSW_b.tif
  ${INPUTDATA}/verySmallFSATSW_nir.tif
  ${TEMP}/raRAndBAndNIRVegetationIndex_TSARVI_verySmallFSATSWcRcBcNir.tif
  0.7   # a ( pente de la droite des sols nus dans l'espace RB/PIR )
  0.9   # b ( ordonnee a l'origine de la droite des sols nus dans l'espace RB/PIR )
  0.08  # x coeff a priori constant
  0.5   # gamma
  )

otb_add_test(NAME raTvLandsatTMThickCloudTest COMMAND otbIndicesTestDriver
  otbLandsatTMThickCloudTest
  ${INPUTDATA}/L5Strip.tif
  ${TEMP}/raTvLandsatTMThickCloudTest_cloudImage.tif
  )

