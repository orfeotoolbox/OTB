otb_module_test()

set(OTBSARCalibrationTests
otbSARCalibrationTestDriver.cxx
otbSarBrightnessFunction.cxx
otbSarBrightnessToImageFilterTestWithoutNoise.cxx
##>otbTerraSarBrightnessImageFilterTest.cxx
##>otbSarRadiometricCalibrationToImageFilterWithRealPixelTest.cxx
otbTerraSarBrightnessImageFilterNew.cxx
otbSarParametricMapFunctionToImageFilter.cxx
otbSarRadiometricCalibrationToImageFilterWithExtractROIBeforeTest.cxx
otbSarRadiometricCalibrationToImageFilterWithComplexPixelTestWithoutNoise.cxx
otbSarBrightnessFunctorWithoutNoise.cxx
otbTerraSarBrightnessFunctor.cxx
otbSarBrightnessToImageWithComplexPixelFilterTest.cxx
otbSarParametricMapFunctionTest.cxx
otbSarRadiometricCalibrationToImageFilterCompareTest.cxx
otbSarBrightnessFunctor.cxx
otbSarBrightnessFunctionWithoutNoise.cxx
otbSarRadiometricCalibrationFunction.cxx
otbSarRadiometricCalibrationFunctionWithoutNoise.cxx
##>otbTerraSarBrightnessImageComplexFilterTest.cxx
##>otbSarRadiometricCalibrationToImageFilterWithComplexPixelTest.cxx
otbSarBrightnessToImageFilterTest.cxx
)

add_executable(otbSARCalibrationTestDriver ${OTBSARCalibrationTests})
target_link_libraries(otbSARCalibrationTestDriver ${OTBSARCalibration-Test_LIBRARIES})
otb_module_target_label(otbSARCalibrationTestDriver)

# Tests Declaration

otb_add_test(NAME raTvSarBrightnessFunction COMMAND otbSARCalibrationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/raTvSarBrightnessFunctionOutputAscii.txt
  ${TEMP}/raTvSarBrightnessFunctionOutputAsciiWithNoise.txt
  otbSarBrightnessFunction
  ${INPUTDATA}/scene.png
  ${TEMP}/raTvSarBrightnessFunctionOutputAsciiWithNoise.txt
  )

otb_add_test(NAME raTvSarBrightnessToImageFilterWithoutNoise COMMAND  otbSARCalibrationTestDriver
  --compare-image ${EPSILON_12}
  ${BASELINE}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
  ${TEMP}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
  otbSarBrightnessToImageFilterTestWithoutNoise
  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
  ${TEMP}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
  1000 1000 250 250 # Extract
  )

##>otb_add_test(NAME raTvTerraSarBrightnessImageFilterUPSALA COMMAND otbSARCalibrationTestDriver
##>  --compare-image ${EPSILON_12}
##>  ${BASELINE}/raTvTerraSarBrightnessImageFilter_UPSALA.tif
##>  ${TEMP}/raTvTerraSarBrightnessImageFilter_UPSALA.tif
##>  otbTerraSarBrightnessImageFilterTest
##>  LARGEINPUT{TERRASARX/UPSALA_GLACIER/TSX1_SAR__MGD/IMAGEDATA/IMAGE_HH_SRA_strip_012.tif}
##>  ${TEMP}/raTvTerraSarBrightnessImageFilter_UPSALA.tif
##>  1 # use metadata
##>  0 # results in linear scale
##>  2000 2000 250 250 # Extract
##>  )

##>otb_add_test(NAME raTvTerraSarBrightnessImageFilterTest COMMAND otbSARCalibrationTestDriver
##>  --compare-image ${EPSILON_12}
##>  ${BASELINE}/raTvTerraSarBrightnessImageFilterTest.tif
##>  ${TEMP}/raTvTerraSarBrightnessImageFilterTest.tif
##>  otbTerraSarBrightnessImageFilterTest
##>  ${INPUTDATA}/ExtractIMAGE_HH_SRA_strip_012.tif
##>  ${TEMP}/raTvTerraSarBrightnessImageFilterTest.tif
##>  0 # don't use metadata
##>  0 # results in linear scale
##>  )

##>otb_add_test(NAME raTvSarRadiometricCalibrationToImageWithRealPixelFilter_TSX_PANGKALANBUUN COMMAND  otbSARCalibrationTestDriver
##>  --compare-image ${EPSILON_12}
##>  ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH.tif
##>  ${TEMP}/raTvSarRadiometricCalibrationToImageFilterRealPixel_TSX_PANGKALANBUUN_HH.tif
##>  otbSarRadiometricCalibrationToImageFilterWithRealPixelTest
##>  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
##>  ${TEMP}/raTvSarRadiometricCalibrationToImageFilterRealPixel_TSX_PANGKALANBUUN_HH.tif
##>  1000 1000 250 250 # Extract
##>  )

##>otb_add_test(NAME raTvSarRadiometricCalibrationToImageWithRealPixelFilter_TSX_UPSALA COMMAND otbSARCalibrationTestDriver
##>  --compare-image ${EPSILON_12}
##>  ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_UPSALA.tif
##>  ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_UPSALA.tif
##>  otbSarRadiometricCalibrationToImageFilterWithRealPixelTest
##>  LARGEINPUT{TERRASARX/UPSALA_GLACIER/TSX1_SAR__MGD/IMAGEDATA/IMAGE_HH_SRA_strip_012.tif}
##>  ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_UPSALA.tif
##>  2000 2000 250 250 # Extract
##>  )

otb_add_test(NAME raTuTerraSarBrightnessImageFilterNew COMMAND otbSARCalibrationTestDriver
  otbTerraSarBrightnessImageFilterNew
  )

otb_add_test(NAME raTvSarParametricMapFunctionToImageFilter COMMAND  otbSARCalibrationTestDriver
  --compare-image ${EPSILON_12}
  ${BASELINE}/raTvSarParametricMapFunctionToImageFilter.tif
  ${TEMP}/raTvSarParametricMapFunctionToImageFilter.tif
  otbSarParametricMapFunctionToImageFilter
  ${TEMP}/raTvSarParametricMapFunctionToImageFilter.tif
  100 200 # image size
  )

otb_add_test(NAME raTvSarRadiometricCalibrationToImagePixelFilterWithExtractROIBefore_TSX_PANGKALANBUUN COMMAND  otbSARCalibrationTestDriver
  --compare-image ${EPSILON_12}
  ${BASELINE}/raTvSarRadiometricCalibrationToImageFilterWithExtractROIBefore_TSX_PANGKALANBUUN_HH.tif
  ${TEMP}/raTvSarRadiometricCalibrationToImageFilterWithExtractROIBefore_TSX_PANGKALANBUUN_HH.tif
  otbSarRadiometricCalibrationToImageFilterWithExtractROIBeforeTest
  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
  ${TEMP}/raTvSarRadiometricCalibrationToImageFilterWithExtractROIBefore_TSX_PANGKALANBUUN_HH.tif
  1000 1000 250 250 # Extract
  )

otb_add_test(NAME raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_TSX_PANGKALANBUUN COMMAND  otbSARCalibrationTestDriver
  --compare-image ${EPSILON_12}
  ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
  ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
  otbSarRadiometricCalibrationToImageFilterWithComplexPixelTestWithoutNoise
  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
  ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH_WN.tif
  1000 1000 250 250 # Extract
  )

#Sentinel-1 L1, SLC
otb_add_test(NAME raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_SENTINEL1 COMMAND  otbSARCalibrationTestDriver
  --compare-image ${EPSILON_12}
  ${BASELINE}/raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_SENTINEL1_VV.tif
  ${TEMP}/raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_SENTINEL1_VV.tif
  otbSarRadiometricCalibrationToImageFilterWithComplexPixelTestWithoutNoise
  LARGEINPUT{SENTINEL1/S1A_S6_SLC__1SSV_20150619T195043/measurement/s1a-s6-slc-vv-20150619t195043-20150619t195101-006447-00887d-001.tiff}
  ${TEMP}/raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_SENTINEL1_VV.tif
  1100 1900 450 450 # Extract
  )

#Radarsat2
otb_add_test(NAME raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_RADARSAT2 COMMAND  otbSARCalibrationTestDriver
  --compare-image ${EPSILON_12}
  ${BASELINE}/raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_RADARSAT2_HV.tif
  ${TEMP}/raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_RADARSAT2_HV.tif
  otbSarRadiometricCalibrationToImageFilterWithComplexPixelTestWithoutNoise
  LARGEINPUT{RADARSAT2/ALTONA/Fine_Quad-Pol_Dataset/PK6621_DK406_FQ9_20080405_124900_HH_VV_HV_VH_SLC_Altona/imagery_HV.tif}
  ${TEMP}/raTvSarRadiometricCalibrationToImageWithComplexPixelFilterWithoutNoise_RADARSAT2_HV.tif
  11 11 650 750 # Extract
  )

otb_add_test(NAME raTuSarBrightnessFunctorWithoutNoise COMMAND otbSARCalibrationTestDriver
  otbSarBrightnessFunctorWithoutNoise
  )

otb_add_test(NAME raTvTerraSarBrightnessFunctor COMMAND otbSARCalibrationTestDriver
  --compare-ascii ${EPSILON_12}
  ${BASELINE_FILES}/raTvTerraSarBrightnessImageFunctor.txt
  ${TEMP}/raTvTerraSarBrightnessImageFunctor.txt
  otbTerraSarBrightnessFunctor
  12 # real pixel
  12 # complex real part pixel
  0  # complex imaginary part pixel
  1  # calFactor
  ${TEMP}/raTvTerraSarBrightnessImageFunctor.txt
  )

otb_add_test(NAME raTvSarBrightnessToImageWithComplexPixelFilter COMMAND  otbSARCalibrationTestDriver
  --compare-image ${EPSILON_12}
  ${BASELINE}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH.tif
  ${TEMP}/raTvSarBrightnessToImageFilterComplexPixel_TSX_PANGKALANBUUN_HH.tif
  otbSarBrightnessToImageWithComplexPixelFilterTest
  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
  ${TEMP}/raTvSarBrightnessToImageFilterComplexPixel_TSX_PANGKALANBUUN_HH.tif
  1000 1000 250 250 # Extract
  )

otb_add_test(NAME raTvSarParametricMapFunction COMMAND otbSARCalibrationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/raTvSarParametricMapFunctionOutputAscii.txt
  ${TEMP}/raTvSarParametricMapFunctionOutputAscii.txt
  otbSarParametricMapFunctionTest
  ${INPUTDATA}/scene.png
  ${TEMP}/raTvSarParametricMapFunctionOutputAscii.txt
  )

otb_add_test(NAME raTvSarRadiometricCalibrationToImageCompareFilter_TSX_PANGKALANBUUN COMMAND  otbSARCalibrationTestDriver
  otbSarRadiometricCalibrationToImageFilterCompareTest
  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
  1000 1000 250 250 # Extract
  )

otb_add_test(NAME raTuSarBrightnessFunctor COMMAND otbSARCalibrationTestDriver
  otbSarBrightnessFunctor
  )

otb_add_test(NAME raTvSarBrightnessFunctionWithoutNoise COMMAND otbSARCalibrationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/raTvSarBrightnessFunctionOutputAscii.txt
  ${TEMP}/raTvSarBrightnessFunctionOutputAsciiWithoutNoise.txt
  otbSarBrightnessFunctionWithoutNoise
  ${INPUTDATA}/scene.png
  ${TEMP}/raTvSarBrightnessFunctionOutputAsciiWithoutNoise.txt
  )

otb_add_test(NAME raTvSarRadiometricCalibrationFunction COMMAND otbSARCalibrationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/raTvSarRadiometricCalibrationFunctionOutputAscii.txt
  ${TEMP}/raTvSarRadiometricCalibrationFunctionOutputAsciiWithNoise.txt
  otbSarRadiometricCalibrationFunction
  ${INPUTDATA}/scene.png
  ${TEMP}/raTvSarRadiometricCalibrationFunctionOutputAsciiWithNoise.txt
  )


otb_add_test(NAME raTvSarRadiometricCalibrationFunctionWithoutNoise COMMAND otbSARCalibrationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/raTvSarRadiometricCalibrationFunctionOutputAscii.txt
  ${TEMP}/raTvSarRadiometricCalibrationFunctionOutputAsciiWithoutNoise.txt
  otbSarRadiometricCalibrationFunctionWithoutNoise
  ${INPUTDATA}/scene.png
  ${TEMP}/raTvSarRadiometricCalibrationFunctionOutputAsciiWithoutNoise.txt
  )

##>otb_add_test(NAME raTvTerraSarBrightnessImageComplexFilterTORONTO COMMAND otbSARCalibrationTestDriver
##>  --compare-image ${EPSILON_12}
##>  ${BASELINE}/raTvTerraSarBrightnessComplexImageFilter_TORONTO.tif
##>  ${TEMP}/raTvTerraSarBrightnessImageComplexFilter_TORONTO.tif
##>  otbTerraSarBrightnessImageComplexFilterTest
##>  LARGEINPUT{TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos}
##>  ${TEMP}/raTvTerraSarBrightnessImageComplexFilter_TORONTO.tif
##>  1 # use metadata
##>  0 # results in linear scale
##>  2000 2000 250 250 # Extract
##>  )

##>otb_add_test(NAME raTvTerraSarBrightnessImageComplexFilterTest COMMAND otbSARCalibrationTestDriver
##>  --compare-image ${EPSILON_12}
##>  ${BASELINE}/raTvTerraSarBrightnessImageComplexFilterTest.tif
##>  ${TEMP}/raTvTerraSarBrightnessImageComplexFilterTest.tif
##>  otbTerraSarBrightnessImageComplexFilterTest
##>  ${INPUTDATA}/ExtractIMAGE_HH_SRA_spot_074.tif
##>  ${TEMP}/raTvTerraSarBrightnessImageComplexFilterTest.tif
##>  0 # don't use metadata
##>  0 # results in linear scale
##>  )

##>otb_add_test(NAME raTvSarRadiometricCalibrationToImageWithComplexPixelFilter_TSX_PANGKALANBUUN COMMAND  otbSARCalibrationTestDriver
##>  --compare-image ${EPSILON_12}
##>  ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH.tif
##>  ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH.tif
##>  otbSarRadiometricCalibrationToImageFilterWithComplexPixelTest
##>  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
##>  ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_PANGKALANBUUN_HH.tif
##>  1000 1000 250 250 # Extract
##>  )

##>otb_add_test(NAME raTvSarRadiometricCalibrationToImageWithComplexPixelFilter_TSX_TORONTO COMMAND otbSARCalibrationTestDriver
##>  --compare-image ${EPSILON_12}
##>  ${BASELINE}/raTvSarRadiometricCalibrationToImageFilter_TSX_TORONTO.tif
##>  ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_TORONTO.tif
##>  otbSarRadiometricCalibrationToImageFilterWithComplexPixelTest
##>  LARGEINPUT{TERRASARX/TORONTO/TSX1_SAR__SSC/IMAGEDATA/IMAGE_HH_SRA_spot_074.cos}
##>  ${TEMP}/raTvSarRadiometricCalibrationToImageFilter_TSX_TORONTO.tif
##>  2000 2000 250 250 # Extract
##>  )

otb_add_test(NAME raTvSarBrightnessToImageFilter COMMAND  otbSARCalibrationTestDriver
  --compare-image ${EPSILON_12}
  ${BASELINE}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH.tif
  ${TEMP}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH.tif
  otbSarBrightnessToImageFilterTest
  LARGEINPUT{TERRASARX/PANGKALANBUUN/IMAGEDATA/IMAGE_HH_SRA_stripFar_008.cos}
  ${TEMP}/raTvSarBrightnessToImageFilter_TSX_PANGKALANBUUN_HH.tif
  1000 1000 250 250 # Extract
  )
