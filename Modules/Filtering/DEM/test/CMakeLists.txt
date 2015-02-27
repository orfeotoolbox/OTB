otb_module_test()
set(OTBDEMTests
  otbDEMToImageGeneratorFromImageTest.cxx
  otbDEMToImageGeneratorNew.cxx
  otbDEMToImageGeneratorTest.cxx
  otbDEMCaracteristicsExtractor.cxx
  otbDEMCaracteristicsExtractorNew.cxx
  otbDEMTestDriver.cxx  )

add_executable(otbDEMTestDriver ${OTBDEMTests})
target_link_libraries(otbDEMTestDriver ${OTBDEM-Test_LIBRARIES})
otb_module_target_label(otbDEMTestDriver)
otb_add_test(NAME ioTvDEMToImageGeneratorFromImageTest_SensorModel COMMAND otbDEMTestDriver
  --compare-n-images ${EPSILON_5} 2
  ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest_SM.tif
  ${TEMP}/ioTvDEMToImageGeneratorFromImage_Extract_DEM.tif
  ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest_SM.tif
  ${TEMP}/ioTvDEMToImageGeneratorFromImage_DEM_Extract.tif
  otbDEMToImageGeneratorFromImageTest
  LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/ioTvDEMToImageGeneratorFromImage_Extract_DEM.tif
  ${TEMP}/ioTvDEMToImageGeneratorFromImage_DEM_Extract.tif
  2600 4300 512 128
  )
otb_add_test(NAME ioTvDEMToImageGeneratorFromImageTest COMMAND otbDEMTestDriver
  --compare-image ${EPSILON_5}
  ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest.tif
  ${TEMP}/ioTvDEMToImageGeneratorFromImage.tif
  otbDEMToImageGeneratorFromImageTest
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/ioTvDEMToImageGeneratorFromImage.tif
  )
otb_add_test(NAME ioTvDEMToImageGeneratorFromImageTest2 COMMAND otbDEMTestDriver
  --compare-n-images ${EPSILON_5} 2
  ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest_Ortho.tif
  ${TEMP}/ioTvDEMToImageGeneratorFromImage_Extract_DEM2.tif
  ${BASELINE}/ioTvDEMToImageGeneratorFromImageTest_Ortho.tif
  ${TEMP}/ioTvDEMToImageGeneratorFromImage_DEM_Extract2.tif
  otbDEMToImageGeneratorFromImageTest
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/ioTvDEMToImageGeneratorFromImage_Extract_DEM2.tif
  ${TEMP}/ioTvDEMToImageGeneratorFromImage_DEM_Extract2.tif
  10 15 16 64
  )
otb_add_test(NAME ioTuDEMToImageGeneratorNew COMMAND otbDEMTestDriver otbDEMToImageGeneratorNew )
otb_add_test(NAME ioTvDEMToImageGeneratorTest2 COMMAND otbDEMTestDriver
  --compare-image ${EPSILON_5}  ${BASELINE}/ioTvDEMToImageGenerator2.tif
  ${TEMP}/ioTvDEMToImageGenerator2.tif
  otbDEMToImageGeneratorTest
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/ioTvDEMToImageGenerator2.tif
  -1.8 52 232 422  0.02 -0.018
  )
otb_add_test(NAME ioTvDEMToImageGeneratorTest1 COMMAND otbDEMTestDriver
  --compare-image ${EPSILON_5}
  ${BASELINE}/ioTvDEMToImageGenerator1.tif
  ${TEMP}/ioTvDEMToImageGenerator1.tif
  otbDEMToImageGeneratorTest
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/ioTvDEMToImageGenerator1.tif
  1.437371 43.559045 512 128 0.000032 -0.000024
  )
otb_add_test(NAME ioTvDEMToImageGeneratorTest COMMAND otbDEMTestDriver
  --compare-image ${EPSILON_5}  ${BASELINE}/ioTvDEMToImageGenerator.tif
  ${TEMP}/ioTvDEMToImageGenerator.tif
  otbDEMToImageGeneratorTest
  ${INPUTDATA}/DEM/srtm_directory
  ${TEMP}/ioTvDEMToImageGenerator.tif
  6.5
  45.5
  500
  500
  0.002
  -0.002
  )
otb_add_test(NAME raTvDEMCaracteristicsExtractor COMMAND otbDEMTestDriver
  --compare-n-images ${EPSILON_12} 4
  ${BASELINE}/raTvDEMCaracteristicsExtractorSlop.tif
  ${TEMP}/raTvDEMCaracteristicsExtractorSlop.tif
  ${BASELINE}/raTvDEMCaracteristicsExtractorAspect.tif
  ${TEMP}/raTvDEMCaracteristicsExtractorAspect.tif
  ${BASELINE}/raTvDEMCaracteristicsExtractorIncidence.tif
  ${TEMP}/raTvDEMCaracteristicsExtractorIncidence.tif
  ${BASELINE}/raTvDEMCaracteristicsExtractorExitance.tif
  ${TEMP}/raTvDEMCaracteristicsExtractorExitance.tif
  otbDEMCaracteristicsExtractor
  ${INPUTDATA}/pyramide.tif
  20 # view azimut
  40 # view angle
  10 # solar azimut
  30 # solar angle
  ${TEMP}/raTvDEMCaracteristicsExtractorSlop.tif
  ${TEMP}/raTvDEMCaracteristicsExtractorAspect.tif
  ${TEMP}/raTvDEMCaracteristicsExtractorIncidence.tif
  ${TEMP}/raTvDEMCaracteristicsExtractorExitance.tif
  )
otb_add_test(NAME raTuDEMCaracteristicsExtractorNew COMMAND otbDEMTestDriver
  otbDEMCaracteristicsExtractorNew
  )
