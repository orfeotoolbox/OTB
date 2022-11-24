#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
#----------- BundleToPerfectSensor TESTS ----------------
otb_test_application(NAME apTvPrBundleToPerfectSensor
                     APP BundleToPerfectSensor
                     OPTIONS -inp  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS_ROI_170x230.tif
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -out ${TEMP}/apTvPrBundleToPerfectSensor.tif int16
                     VALID  --compare-image ${EPSILON_2}
                        ${BASELINE}/apTvPrBundleToPerfectSensor.tif
                        ${TEMP}/apTvPrBundleToPerfectSensor.tif)
                      
otb_test_application(NAME apTvPrBundleToPerfectSensor_phr
                     APP BundleToPerfectSensor
                     OPTIONS -inp  ${INPUTDATA}/phr_pan.tif
                             -inxs ${INPUTDATA}/phr_xs.tif
                             -out ${TEMP}/apTvPrBundleToPerfectSensorPHR.tif int16
                     VALID  --compare-image ${EPSILON_2}
                        ${BASELINE}/apTvPrBundleToPerfectSensorPHR.tif
                        ${TEMP}/apTvPrBundleToPerfectSensorPHR.tif)

#----------- Pansharpening TESTS ----------------
otb_test_application(NAME  apTvFuPansharpening_LMVM
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                       -out ${TEMP}/apTvFuPanSharpeningLmvm.tif double
                       -method lmvm
                             -method.lmvm.radiusx 5
                             -method.lmvm.radiusy 5
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/fuTvLmvmPanSharpeningFusion.tif
                            ${TEMP}/apTvFuPanSharpeningLmvm.tif
                     )

otb_test_application(NAME  apTvFuPansharpening_Bayes
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/panchro.tif
                             -inxs ${INPUTDATA}/multiSpectInterp.tif
                       -out ${TEMP}/apTvFuPanSharpeningBayes.tif double
                       -method bayes
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/apTvFuPanSharpeningBayes.tif
                            ${TEMP}/apTvFuPanSharpeningBayes.tif
                     )

otb_test_application(NAME  apTvFuPansharpening_RCS
                     APP  Pansharpening
                     OPTIONS -inp ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inxs ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                       -out ${TEMP}/apTvFuPanSharpeningRcs.tif double
                       -method rcs
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/fuTvRcsPanSharpeningFusion.tif
                            ${TEMP}/apTvFuPanSharpeningRcs.tif
                     )


set(OTBAppImageUtilsTests
otbAppImageUtilsTestDriver.cxx
otbExtractROIAppTests.cxx
)

add_executable(otbAppImageUtilsTestDriver ${OTBAppImageUtilsTests})
target_link_libraries(otbAppImageUtilsTestDriver ${OTBAppCore-Test_LIBRARIES})
otb_module_target_label(otbAppImageUtilsTestDriver)

#----------- DynamicConvert TESTS ------------
otb_test_application(NAME apTvUtDynamicConvertBasic
                        APP DynamicConvert
                        OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                                -out ${TEMP}/apTvUtDynamicConvertOutput.tif
                                -type linear
                                -channels rgb
                                -channels.rgb.red 2
                                -channels.rgb.green 3
                                -channels.rgb.blue 1
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/apTvUtConvertSelectChannelsRgbOutput.tif
                                ${TEMP}/apTvUtDynamicConvertOutput.tif)

otb_test_application(NAME apTuUtDynamicConvertExtendedFilename_writer
                        APP  DynamicConvert
                        OPTIONS -in ${INPUTDATA}/ToulouseExtract_WithGeom.tif
                                -out ${TEMP}/apTvUtNoGeomExtendedFilename.tif?&gdal:co:TILED=YES&writegeom=false
                        )

otb_test_application(NAME apTvUtDynamicConvertExtendedFilename_readerGEOM
                        APP  DynamicConvert
                        OPTIONS -in ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&geom=${INPUTDATA}/ToulouseExtract_ModifiedGeom.geom
                                -out ${TEMP}/apTvUtGeomExtendedFilename.tif
                        VALID   --compare-metadata ${NOTOL}
                                ${INPUTDATA}/ToulouseExtract_ModifiedMetadata.tif
                                ${TEMP}/apTvUtGeomExtendedFilename.tif)

otb_test_application(NAME apTvUtDynamicConvertLog2
                        APP DynamicConvert
                        OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                                -out ${TEMP}/apTvUtDynamicConvertLog2Output.tif
                                -type log2
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/apTvUtDynamicConvertLog2Output.tif
                                ${TEMP}/apTvUtDynamicConvertLog2Output.tif)

otb_test_application(NAME apTvUtDynamicConvertFloat
                        APP DynamicConvert
                        OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                                -out ${TEMP}/apTvUtDynamicConvertFloatOutput.tif float
                                -type linear
                                -type.linear.gamma 2.2
                                -outmin 0.0
                                -outmax 1.0
                                -quantile.low 0
                                -quantile.high 4
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/apTvUtDynamicConvertFloatOutput.tif
                                ${TEMP}/apTvUtDynamicConvertFloatOutput.tif)

otb_test_application(NAME apTvUtDynamicConvertMask
                        APP DynamicConvert
                        OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                                -out ${TEMP}/apTvUtDynamicConvertMaskOutput.tif
                                -outmin 0
                                -outmax 255
                                -quantile.low 4
                                -quantile.high 4
                                -mask ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask.tif
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/apTvUtDynamicConvertMaskOutput.tif
                                ${TEMP}/apTvUtDynamicConvertMaskOutput.tif)


#----------- Extract ROI tests  ----------------

otb_add_test(NAME apTvUtExtractROI COMMAND otbAppImageUtilsTestDriver
otbExtractROIAppTests
$<TARGET_FILE_DIR:otbapp_ExtractROI>
10 3 2 3 4
)

otb_test_application(NAME apTvUtExtractROIExtentFitVect
                        APP  ExtractROI
                        OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                                -out ${TEMP}/apTvUtExtractROIExtentFitVect.tif
                                -mode fit
                                -mode.fit.vect ${INPUTDATA}/apTvUtExtractROIExtentFitVect.shp
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/apTvUtExtractROI.tif
                                ${TEMP}/apTvUtExtractROIExtentFitVect.tif)

#----------- Clamp ROI tests  ----------------
otb_test_application(NAME apTvUtResetMargin_Threshold
                        APP  ResetMargin
                        OPTIONS -in ${INPUTDATA}/ResetMarginInput100x100.tiff
                                -out ${TEMP}/apTvUtResetMargin.tif
                                -mode threshold
                                -threshold.x       10
                                -threshold.y.start 12
                                -threshold.y.end   25
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/ResetMarginBaseline100x100.tiff
                                ${TEMP}/apTvUtResetMargin.tif)

otb_test_application(NAME apTvUtResetMargin_ROI
                        APP  ResetMargin
                        OPTIONS -in ${INPUTDATA}/ResetMarginInput100x100.tiff
                                -out ${TEMP}/apTvUtResetMargin_ROI.tif
                                -mode roi
                                -roi.startx 10
                                -roi.starty 12
                                -roi.sizex 80
                                -roi.sizey 63
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/ResetMarginBaseline100x100.tiff
                                ${TEMP}/apTvUtResetMargin_ROI.tif)

otb_test_application(NAME apTvUtResetMargin_Margin
                        APP  ResetMargin
                        OPTIONS -in ${INPUTDATA}/ResetMarginInput100x100.tiff
                                -out ${TEMP}/apTvUtResetMargin_Mar.tif
                                -mode margin
                                -margin.top 12
                                -margin.left 10
                                -margin.right 10
                                -margin.bottom 25
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/ResetMarginBaseline100x100.tiff
                                ${TEMP}/apTvUtResetMargin_Mar.tif)

#----------- ReadImageInfo TESTS ----------------
otb_test_application(NAME apTuUtReadImageInfoExtendedFilename_reader
                        APP  ReadImageInfo
                        OPTIONS -in ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&skipgeom=true&skipcarto=true
                        )

otb_test_application(NAME apTuUtReadComplexImageInfoFilename_reader
                        APP  ReadImageInfo
                        OPTIONS -in ${INPUTDATA}/complexInputCfloat.tif
                        )


set(TESTNAME
"gd-pleiades-1" #LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2}
"gd-wv2-1"           #LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
"gd-spot6-1"       #LARGEINPUT{SPOT6/600143101-Primary-Bundle-JP2-LOSSLESS/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201212071020271_SEN_600143101_R1C1.JP2}
"gd-qb-1"             #LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
"gd-ikonos-1"     #LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_nir_0000000.tif}
"gd-rapideye-1" #LARGEINPUT{RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf}
)

set(IMG
LARGEINPUT{PLEIADES/TLSE_JP2_DIMAPv2_PRIMARY_PMS_lossless_12bits/IMGPHR_201222215194743808/IMG_PHR1A_PMS_201201151100183_SEN_IPU_20120222_0901-001_R1C1.JP2}
LARGEINPUT{WORLDVIEW2/ROME/WV-2_standard_8band_bundle_16bit/052298844010_01_P001_PAN/09DEC10103019-P2AS-052298844010_01_P001.TIF}
LARGEINPUT{SPOT6/600143101-Primary-Bundle-JP2-LOSSLESS/PROD_SPOT6_001/VOL_SPOT6_001_A/IMG_SPOT6_MS_001_A/IMG_SPOT6_MS_201212071020271_SEN_600143101_R1C1.JP2}
LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
LARGEINPUT{IKONOS/BLOSSEVILLE/po_2619900_nir_0000000.tif}
LARGEINPUT{RAPIDEYE/level1B/2008-12-25T005918_RE3_1B-NAC_397971_12345_band3.ntf}
)

set( GEOM_TESTNB 0)
foreach( file ${IMG} )
list(GET TESTNAME   ${GEOM_TESTNB} current_testname   )
math(EXPR GEOM_TESTNB "${GEOM_TESTNB} + 1")

otb_test_application(NAME  apTuReadImageInfoTest_${current_testname}
                        APP  ReadImageInfo
                        OPTIONS -in ${file}
                        #No baseline, just check that the process finished well
                        )
endforeach()



#----------- ConcatenateImages TESTS ----------------
otb_test_application(NAME apTvUtConcatenateImages
                        APP  ConcatenateImages
                        OPTIONS -il ${INPUTDATA}/poupees_sub_c1.png
                                ${INPUTDATA}/poupees_sub_c2.png
                                ${INPUTDATA}/poupees_sub_c3.png
                                        -out ${TEMP}/apTvUtConcatenateImages.png
                        VALID   --compare-image ${NOTOL}
                                ${INPUTDATA}/poupees_sub_3c.png
                                ${TEMP}/apTvUtConcatenateImages.png)

otb_test_application(NAME apTvUtConcatenateImages_1Image
                        APP  ConcatenateImages
                        OPTIONS -il ${INPUTDATA}/poupees_c1.raw
                                -out ${TEMP}/apTvUtConcatenateImages_1Image.tif
                        VALID   --compare-image ${NOTOL}
                                ${INPUTDATA}/poupees_c1.raw
                                ${TEMP}/apTvUtConcatenateImages_1Image.tif)



#----------- CompareImages TESTS ----------------
otb_test_application(NAME apTvUtCompareImagesNoROI
                        APP CompareImages
                        OPTIONS -ref.in ${INPUTDATA}/GomaAvant.png
                                -meas.in ${INPUTDATA}/GomaApres.png
                        TESTENVOPTIONS ${TEMP}/apTvUtCompareImagesNoROI.txt
                        VALID   --compare-ascii ${EPSILON_7}
                                ${BASELINE_FILES}/apTvUtCompareImagesNoROI.txt
                                ${TEMP}/apTvUtCompareImagesNoROI.txt)

otb_test_application(NAME apTvUtCompareImages
                        APP CompareImages
                        OPTIONS -ref.in ${INPUTDATA}/poupees.tif
                                -ref.channel 2
                                -meas.in ${INPUTDATA}/brain.png
                                -meas.channel 1
                                -roi.startx 20
                                -roi.starty 15
                                -roi.sizex 150
                                -roi.sizey 200
                        TESTENVOPTIONS ${TEMP}/apTvUtCompareImages.txt
                        VALID   --compare-ascii ${EPSILON_7}
                                ${BASELINE_FILES}/apTvUtCompareImages.txt
                                ${TEMP}/apTvUtCompareImages.txt)


#----------- Mosaic TESTS ----------------
otb_test_application(NAME MosaicTestLargeFeathering
                        APP  Mosaic
                        OPTIONS -il ${INPUTDATA}/SP67_FR_subset_1.tif ${INPUTDATA}/SP67_FR_subset_2.tif
                                -out ${TEMP}/apTvMosaicTestLargeFeathering.tif uint8
                                -comp.feather large
                        VALID   --compare-image ${EPSILON_8}
                                ${BASELINE}/apTvMosaicTestLargeFeathering.tif
                                ${TEMP}/apTvMosaicTestLargeFeathering.tif)


otb_test_application(NAME MosaicTestSlimFeathering
                        APP  Mosaic
                        OPTIONS -il ${INPUTDATA}/SP67_FR_subset_1.tif ${INPUTDATA}/SP67_FR_subset_2.tif
                                -out ${TEMP}/apTvMosaicTestSlimFeathering.tif uint8
                                -comp.feather slim
                                -comp.feather.slim.length 100
                        VALID   --compare-image ${EPSILON_8}
                                ${BASELINE}/apTvMosaicTestSlimFeathering.tif
                                ${TEMP}/apTvMosaicTestSlimFeathering.tif)


otb_test_application(NAME MosaicTestSimpleWithHarmoBandRmse
                        APP  Mosaic
                        OPTIONS -il ${INPUTDATA}/SP67_FR_subset_1.tif ${INPUTDATA}/SP67_FR_subset_2.tif
                                -out ${TEMP}/apTvMosaicTestSimpleWithHarmoBandRmse.tif uint8
                                -harmo.method band
                                -harmo.cost rmse
                        VALID   --compare-image ${EPSILON_8}
                                ${BASELINE}/apTvMosaicTestSimpleWithHarmoBandRmse.tif
                                ${TEMP}/apTvMosaicTestSimpleWithHarmoBandRmse.tif)

otb_test_application(NAME MosaicTestSimpleWithHarmoRgbRmse
                        APP  Mosaic
                        OPTIONS -il ${INPUTDATA}/SP67_FR_subset_1.tif ${INPUTDATA}/SP67_FR_subset_2.tif
                                -out ${TEMP}/apTvMosaicTestSimpleWithHarmoRgbRmse.tif uint8
                                -harmo.method rgb
                                -harmo.cost rmse
                        VALID   --compare-image ${EPSILON_8}
                                ${BASELINE}/apTvMosaicTestSimpleWithHarmoRgbRmse.tif
                                ${TEMP}/apTvMosaicTestSimpleWithHarmoRgbRmse.tif)

otb_test_application(NAME MosaicTestSimpleWithCutline
                        APP  Mosaic
                        OPTIONS -il ${INPUTDATA}/SP67_FR_subset_1.tif ${INPUTDATA}/SP67_FR_subset_2.tif
                                -out ${TEMP}/apTvMosaicTestSimpleWithCutline.tif uint8
                                -vdcut ${INPUTDATA}/SP67_FR_subset_1_cutline.shp ${INPUTDATA}/SP67_FR_subset_2_cutline.shp
                        VALID   --compare-image ${EPSILON_8}
                                ${BASELINE}/apTvMosaicTestSimpleWithCutline.tif
                                ${TEMP}/apTvMosaicTestSimpleWithCutline.tif)

otb_test_application(NAME MosaicTestSimpleWithVdstats
                        APP  Mosaic
                        OPTIONS -il ${INPUTDATA}/SP67_FR_subset_1.tif ${INPUTDATA}/SP67_FR_subset_2.tif
                                -out ${TEMP}/apTvMosaicTestSimpleWithVdstats.tif uint8
                                -vdstats ${INPUTDATA}/SP67_FR_subset_1_cutline.shp ${INPUTDATA}/SP67_FR_subset_2_cutline.shp
                        VALID   --compare-image ${EPSILON_8}
                                ${BASELINE}/apTvMosaicTestSimpleWithVdstats.tif
                                ${TEMP}/apTvMosaicTestSimpleWithVdstats.tif)
                     
#----------- OpticalCalibration TESTS ----------------
otb_test_application(NAME apTvRaOpticalCalibration_QuickbirdXS
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             -level toa
                             -clamp false
                             -out ${TEMP}/apTvRaOpticalCalibration_QuickbirdXS.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif
                             ${TEMP}/apTvRaOpticalCalibration_QuickbirdXS.tif )

otb_test_application(NAME apTvRaOpticalCalibration_SolarDistance
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             -level toa
                             -clamp false
                             -acqui.solardistance 0.9987593
                             -out ${TEMP}/apTvRaOpticalCalibration_SolarDistance.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif
                             ${TEMP}/apTvRaOpticalCalibration_SolarDistance.tif )

otb_test_application(NAME apTvRaOpticalCalibration_WV2PAN
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/WV2_PAN_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_WV2PAN.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoWV2PAN.tif
                             ${TEMP}/apTvRaOpticalCalibration_WV2PAN.tif )

otb_test_application(NAME apTvRaOpticalCalibration_WV2Multi
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/WV2_MUL_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_WV2Multi.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoWV2Multi.tif
                             ${TEMP}/apTvRaOpticalCalibration_WV2Multi.tif )

otb_test_application(NAME apTvRaOpticalCalibration_Formosat
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/FORMOSAT_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_Formosat.img
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoFormosat.tif
                             ${TEMP}/apTvRaOpticalCalibration_Formosat.img )

otb_test_application(NAME apTvRaOpticalCalibration_QuickbirdPAN
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/QB_PAN_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_QuickbirdPAN.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdPAN.tif
                             ${TEMP}/apTvRaOpticalCalibration_QuickbirdPAN.tif )

otb_test_application(NAME apTvRaOpticalCalibration_Spot5
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/SPOT5_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_Spot5.img
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoSpot5.tif
                             ${TEMP}/apTvRaOpticalCalibration_Spot5.img )

otb_test_application(NAME apTvRaOpticalCalibration_UnknownSensor
                     APP  OpticalCalibration
                     OPTIONS 
           -in ${INPUTDATA}/Romania_Extract.tif
           -out ${TEMP}/apTvRaOpticalCalibration_Spot4_UnknownSensor_test.tif
           -level toc
           -acqui.gainbias ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorGainsBiases2.txt
           -acqui.day 4
           -acqui.month 12
           -acqui.sun.elev 62.7
           -acqui.sun.azim 152.7
           -acqui.view.elev 87.5
           -acqui.view.azim 283
           -acqui.solarilluminations ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorSolarIllumations2.txt
           -atmo.rsr ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorRSR.txt
           -atmo.pressure 1013.0
           -atmo.wa 2.48134
           -atmo.oz 0.34400
           -atmo.aerosol continental
           -atmo.opt 0.199854
           -atmo.radius 2
           -atmo.pixsize 0.02
           -milli false
           -clamp false
                     VALID   --compare-image ${EPSILON_6}
                             ${OTB_DATA_ROOT}/Baseline/Examples/Radiometry/Example_RomaniaAtmosphericCorrectionSequencement.tif
                             ${TEMP}/apTvRaOpticalCalibration_Spot4_UnknownSensor_test.tif )

otb_test_application(NAME apTvRaOpticalCalibration_Reverse_UnknownSensor
                     APP  OpticalCalibration
                     OPTIONS 
			     -in ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoQuickbirdXS.tif
			     -out ${TEMP}/apTvRaOpticalCalibration_Rev_QB-XS_UnknownSensor_test.tif
			     -level toatoim 
			     -acqui.gainbias ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorGainsBiases.txt
			     -acqui.day 1
			     -acqui.month 4
			     -acqui.sun.elev 48.6
			     -acqui.solarilluminations ${INPUTDATA}/apTvRaOpticalCalibrationUnknownSensorSolarIllumations.txt
		       -milli false
			     -clamp false
                     VALID   --compare-image ${EPSILON_6}
                             ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             ${TEMP}/apTvRaOpticalCalibration_Rev_QB-XS_UnknownSensor_test.tif )

otb_test_application(NAME apTvRaOpticalCalibration_Ikonos
                     APP  OpticalCalibration
                     OPTIONS -in ${INPUTDATA}/IKONOS_BLOSSEVILLE_ROI_1000_100.tif
                             -level toa
                             -out ${TEMP}/apTvRaOpticalCalibration_Ikonos.tif
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/raTvRadianceToReflectanceImageFilterAutoIkonos.tif
                             ${TEMP}/apTvRaOpticalCalibration_Ikonos.tif )

#----------- VectorDataTransform TESTS ----------------
otb_test_application(NAME apTvUtVectorDataTransform_Rotation
                     APP VectorDataTransform
                     OPTIONS -in ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             -vd ${INPUTDATA}/ToulouseRoad-examples.shp
                             -out ${TEMP}/apTvUtVectorDataTransform_rotation.shp
                             -transform.ro 15
                     VALID   --compare-ogr ${EPSILON_9}
                             ${OTBAPP_BASELINE_FILES}/utTvVectorDataTransformFilter_Rotation.shp
                             ${TEMP}/apTvUtVectorDataTransform_rotation.shp)

otb_test_application(NAME apTvUtVectorDataTransform_Translation_Rotation
                     APP VectorDataTransform
                     OPTIONS -in ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             -vd ${INPUTDATA}/ToulouseRoad-examples.shp
                             -out ${TEMP}/apTvUtVectorDataTransform_Translation_rotation.shp
                             -transform.ro 15
                             -transform.tx  5
                             -transform.ty  6
                     VALID   --compare-ogr ${EPSILON_9}
                             ${OTBAPP_BASELINE_FILES}/utTvVectorDataTransformFilter_Translation_rotation.shp
                             ${TEMP}/apTvUtVectorDataTransform_Translation_rotation.shp)


#----------- VectorDataExtractROI TESTS ----------------
otb_test_application(NAME apTvUtVectorDataExtractROI
                     APP VectorDataExtractROI
                     OPTIONS -io.in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -io.vd LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
                             -io.out ${TEMP}/apTvUtVectorDataExtractROIApplicationTest.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvUtVectorDataExtractROIApplicationTest.shp
                             ${TEMP}/apTvUtVectorDataExtractROIApplicationTest.shp
                     )


#----------- VectorDataSetField TESTS ----------------
otb_test_application(NAME apTvUtVectorDataSetField
                     APP VectorDataSetField
                     OPTIONS -in ${INPUTDATA}/toulousepoints_examples.shp
                             -out ${TEMP}/apTvUtVectorDataSetFieldTest.shp
                             -fn MyField
                             -fv MyValue
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvUtVectorDataSetFieldTest.shp
                             ${TEMP}/apTvUtVectorDataSetFieldTest.shp
                     )

#----------- ConcatenateVectorData TESTS ----------------
otb_test_application(NAME apTvUtConcatenateVectorData
                     APP ConcatenateVectorData
                     OPTIONS -vd   ${INPUTDATA}/waterways.shp
                                    ${INPUTDATA}/france_coastline.shp
                             -out ${TEMP}/apTvUtConcatenateVectorDataOutputTest.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/utTvConcatenateVectorData.shp
                             ${TEMP}/apTvUtConcatenateVectorDataOutputTest.shp)

#----------- Rasterization TESTS ----------------
otb_test_application(NAME apTvRsRasterizationLinesAttributeMode
APP  Rasterization
OPTIONS -in   LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
        -im   ${BASELINE}/apTvRsRasterizationLines.tif
        -out  ${TEMP}/apTvRsRasterizationLinesAttributeMode.tif uint32
        -background 0
        -mode attribute
        -mode.attribute.field osm_id
VALID   --compare-image ${NOTOL}
         ${BASELINE}/apTvRsRasterizationLinesAttributeMode.tif
         ${TEMP}/apTvRsRasterizationLinesAttributeMode.tif)

otb_test_application(NAME apTvRsRasterizationPolygons2LayersWith2SRS
APP  Rasterization
OPTIONS -in  ${INPUTDATA}/ToulouseRoad-examples_2layers_2srs.sqlite
        -spx 1.
        -spy -1.
        -out ${TEMP}/apTvRsRasterizationPolygons2LayersWith2SRS.tif uint8
        -background 255
        -mode.binary.foreground 0
VALID   --compare-image ${NOTOL}
         ${BASELINE}/apTvRsRasterizationPolygons2LayersWith2SRS.tif
         ${TEMP}/apTvRsRasterizationPolygons2LayersWith2SRS.tif)

otb_test_application(NAME apTvRsRasterizationLines
APP  Rasterization
OPTIONS -in   LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
        -im   ${BASELINE}/apTvRsRasterizationLines.tif
        -out  ${TEMP}/apTvRsRasterizationLines.tif uint8
        -background 255
        -mode.binary.foreground 0
VALID   --compare-image ${NOTOL}
         ${BASELINE}/apTvRsRasterizationLines.tif
                     ${TEMP}/apTvRsRasterizationLines.tif)

otb_test_application(NAME apTvRsRasterizationLinesStreaming
APP  Rasterization
OPTIONS -in   LARGEINPUT{VECTOR/MidiPyrenees/roads.shp}
        -im   ${BASELINE}/apTvRsRasterizationLines.tif
        -out  ${TEMP}/apTvRsRasterizationLinesStreaming.tif uint8
         -ram 1
        -background 255
        -mode.binary.foreground 0
VALID   --compare-image ${NOTOL}
        ${BASELINE}/apTvRsRasterizationLines.tif
        ${TEMP}/apTvRsRasterizationLinesStreaming.tif)

otb_test_application(NAME apTvRsRasterizationPolygons
APP  Rasterization
OPTIONS -in  ${INPUTDATA}/Rasterization/polgons.shp
        -spx 1.
        -spy 1.
        -out ${TEMP}/apTvRsRasterizationPolygons.tif uint8
        -background 255
        -mode.binary.foreground 0
VALID   --compare-image ${NOTOL}
         ${BASELINE}/apTvRsRasterizationPolygons.tif
         ${TEMP}/apTvRsRasterizationPolygons.tif)

otb_test_application(NAME apTvRsRasterizationPoints
APP  Rasterization
OPTIONS -in   LARGEINPUT{VECTOR/MidiPyrenees/points.shp}
        -im   ${BASELINE}/apTvRsRasterizationPoints.tif
        -out  ${TEMP}/apTvRsRasterizationPoints.tif uint8
        -background 255
        -mode.binary.foreground 0
VALID   --compare-image ${NOTOL}
         ${BASELINE}/apTvRsRasterizationPoints.tif
         ${TEMP}/apTvRsRasterizationPoints.tif)

otb_test_application(NAME apTvRsRasterizationShadowsSHP
APP  Rasterization
OPTIONS -in   ${INPUTDATA}/Capitole-Shadows.shp
        -im   ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
        -out  ${TEMP}/apTvRsRasterizationShadowsSHP.tif
        -background 255
        -mode.binary.foreground 0
VALID   --compare-image ${NOTOL}
         ${BASELINE}/apTvRsRasterizationShadowsSHP.tif
               ${TEMP}/apTvRsRasterizationShadowsSHP.tif)

otb_test_application(NAME apTvRsRasterizationShadowsKML
APP  Rasterization
OPTIONS -in   ${INPUTDATA}/Capitole-Shadows.kml
        -im   ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
        -out  ${TEMP}/apTvRsRasterizationShadowsKML.tif
        -background 255
        -mode.binary.foreground 0
VALID   --compare-image ${NOTOL}
         ${BASELINE}/apTvRsRasterizationShadowsKML.tif
               ${TEMP}/apTvRsRasterizationShadowsKML.tif)

#----------- VectorDataReprojection TESTS ----------------
otb_test_application(NAME  apTuPrVectorDataReprojectionFromImageToMap
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/ToulousePointsUTM31N.shp
                     		     -in.kwl ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromImageToMap.shp
                             -out.proj user
                             -out.proj.user.map utm
                             -out.proj.user.map.utm.northhem true
                 	           -out.proj.user.map.utm.zone 31)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToMap
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/toulousepoints_examples.shp
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToMap.sqlite
                             -out.proj user
                             -out.proj.user.map  lambert93
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToMap.sqlite
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToMap.sqlite)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToImage
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/toulousepoints_examples.shp
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToImage.shp
                             -out.proj image
                             -out.proj.image.in  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/apTvPrVectorDataReprojectionFromMapToImage.shp
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToImage.shp)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToSensor
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/toulousepoints_examples.shp
                 			 -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToSensor.shp
                             -out.proj image
                             -out.proj.image.in ${INPUTDATA}/QB_PAN_ROI_1000_100.tif
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/apTvPrVectorDataReprojectionFromMapToSensor.shp
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToSensor.shp)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToMap2
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToMap.sqlite
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToMap2.sqlite
                             -out.proj user
                             -out.proj.user.map  utm
                             -out.proj.user.map.utm.northhem true
                 	           -out.proj.user.map.utm.zone 31
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/apTvPrVectorDataReprojectionFromMapToMap2.sqlite
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToMap2.sqlite)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToGeo
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/toulousepoints_examples.shp
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToGeo.shp
                             -out.proj user
                             -out.proj.user.map  wgs
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToGeo.shp
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToGeo.shp)

otb_test_application(NAME  apTuPrVectorDataReprojectionFromGeoToMap
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/ToulousePointsWGS.sqlite
                 			 -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromGeoToMap.shp
                             -out.proj image
                             -out.proj.image.in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif)

#----------- OrthoRectification TESTS ----------------
otb_test_application(NAME  apTvPrOrthorectification_UTM_OutXML1
                     APP  OrthoRectification
                     OPTIONS -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                 			 -io.out ${TEMP}/apTvPrOrthorectifTest_UTM_OutXML1.tif
                 			 -elev.dem ${INPUTDATA}/DEM/srtm_directory/
                 		     -outputs.ulx  374100.8
                 			 -outputs.uly  4829184.8
                 			 -outputs.sizex 500
                 			 -outputs.sizey 500
                 			 -outputs.spacingx  0.5
                 			 -outputs.spacingy  -0.5
                 			 -map utm
                 			 -opt.gridspacing 4 # Spacing of the deformation field equal to 4 meters
                       -outxml  ${TEMP}/apTvPrOrthorectifTest_UTM_OutXML1.xml
                       -interpolator linear
                     VALID   --compare-image ${EPSILON_4}
                              ${BASELINE}/owTvOrthorectifTest_UTM.tif
                 			  ${TEMP}/apTvPrOrthorectifTest_UTM_OutXML1.tif)

otb_test_application(NAME  apTvPrOrthorectification_UTM_InXML1
                     APP  OrthoRectification
                     OPTIONS
                       -inxml  ${INPUTDATA}/apTvPrOrthorectifTest_UTM_OutXML1.xml
                 			 -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                 			 -io.out ${TEMP}/apTvPrOrthorectifTest_UTM_InXML1.tif
                 			 -elev.dem ${INPUTDATA}/DEM/srtm_directory/
                        VALID   --compare-image ${EPSILON_4}
                              ${BASELINE}/owTvOrthorectifTest_UTM.tif
                 			  ${TEMP}/apTvPrOrthorectifTest_UTM_InXML1.tif)

#otb_test_application(NAME  apTvPrOrthorectification_DEMTIF_UTM_InXML1
                     #APP  OrthoRectification
                     #OPTIONS
                       #-inxml  ${INPUTDATA}/apTvPrOrthorectifTest_DEMTIF_UTM_OutXML1.xml
                       #-io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                       #-io.out ${TEMP}/apTvPrOrthorectifTest_DEMTIF_UTM_InXML1.tif
                       #-elev.dem ${INPUTDATA}/DEM/tif_directory/
                        #VALID   --compare-image ${EPSILON_4}
                              #${BASELINE}/owTvOrthorectifTest_UTM.tif
                        #${TEMP}/apTvPrOrthorectifTest_DEMTIF_UTM_InXML1.tif)

otb_test_application(NAME  apTvPrOrthorectification_WGS84
                     APP  OrthoRectification
                     OPTIONS -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                       -io.out ${TEMP}/apTvPrOrthorectifTest_WGS84.tif
                       -elev.dem ${INPUTDATA}/DEM/srtm_directory/
                       -outputs.ulx  1.35404
                       -outputs.uly  43.65414
                       -outputs.sizex 500
                       -outputs.sizey 500
                       -outputs.spacingx  0.00000621314
                       -outputs.spacingy  -0.00000621314
                       -map wgs
                       -opt.gridspacing 0.00001242628 # Spacing of the deformation field equal to |2*outputs.spacingx| degrees
                       -interpolator linear
                     VALID   --compare-image ${EPSILON_4}
                        ${BASELINE}/owTvOrthorectifTest_WGS84.tif
                        ${TEMP}/apTvPrOrthorectifTest_WGS84.tif
                     )

otb_test_application(NAME  apTvPrOrthorectification_UTM
                     APP  OrthoRectification
                     OPTIONS -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                 			 -io.out ${TEMP}/apTvPrOrthorectifTest_UTM.tif
                 			 -elev.dem ${INPUTDATA}/DEM/srtm_directory/
                 		     -outputs.ulx  374100.8
                 			 -outputs.uly  4829184.8
                 			 -outputs.sizex 500
                 			 -outputs.sizey 500
                 			 -outputs.spacingx  0.5
                 			 -outputs.spacingy  -0.5
                 			 -map utm
                 			 -opt.gridspacing 4 # Spacing of the displacement field equal to 4 meters
                       -interpolator linear
                     VALID   --compare-image ${EPSILON_4}
                              ${BASELINE}/owTvOrthorectifTest_UTM.tif
                 			  ${TEMP}/apTvPrOrthorectifTest_UTM.tif)

otb_test_application(NAME apTvPrOrthorectification_OTB_Metadata
                     APP OrthoRectification
                     OPTIONS -io.in ${INPUTDATA}/QB_TOULOUSE_11_11.tif
                             -io.out ${TEMP}/apTvPrOrthorectification_OTB_Metadata.tif
                     VALID --compare-image ${EPSILON_4}
                           ${BASELINE}/apTvPrOrthorectification_OTB_Metadata.tif
                           ${TEMP}/apTvPrOrthorectification_OTB_Metadata.tif)

#----------- Superimpose TESTS ----------------
otb_test_application(NAME apTvPrSuperimpose
                     APP Superimpose
                     OPTIONS -inr  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inm ${INPUTDATA}/QB_Toulouse_Ortho_XS_ROI_170x230.tif
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -out ${TEMP}/apTvPrSuperimpose.tif int16
                             -lms 4.0
                     VALID  --compare-image ${EPSILON_2}
                        ${BASELINE}/apTvPrSuperimpose.tif
                        ${TEMP}/apTvPrSuperimpose.tif)

otb_test_application(NAME apTvPrSuperimpose_phr
                     APP Superimpose
                     OPTIONS -inr  ${INPUTDATA}/phr_pan.tif
                             -inm ${INPUTDATA}/phr_xs.tif
                             -out ${TEMP}/apTvPrSuperimposePHR.tif int16
                     VALID  --compare-image ${EPSILON_7}
                        ${BASELINE}/apTvPrSuperimposePHR.tif
                        ${TEMP}/apTvPrSuperimposePHR.tif)

otb_test_application(NAME apTvPrSuperimpose_phr_nn
                     APP Superimpose
                     OPTIONS -inr  ${INPUTDATA}/phr_pan.tif
                             -inm ${INPUTDATA}/phr_xs.tif
                             -interpolator nn
                             -out ${TEMP}/apTvPrSuperimposePHR_nn.tif int16
                     VALID  --compare-image ${EPSILON_7}
                        ${BASELINE}/apTvPrSuperimposePHR_nn.tif
                        ${TEMP}/apTvPrSuperimposePHR_nn.tif)



#----------- ImageEnvelope TESTS ----------------
otb_test_application(NAME apTvPrImageEnvelopeTest
                     APP ImageEnvelope
                     OPTIONS -in ${INPUTDATA}/sensor_stereo_left.tif
                             -out ${TEMP}/apTvPrImageEnvelopeTest.shp
                     VALID   --compare-ogr ${EPSILON_9}
                        ${BASELINE_FILES}/apTvPrImageEnvelopeTest.shp
                        ${TEMP}/apTvPrImageEnvelopeTest.shp)



#----------- RigidTransformResample TESTS ----------------
otb_test_application(NAME apTvPrRigidTransformResample
                     APP  RigidTransformResample
                     OPTIONS -in ${INPUTDATA}/poupees.tif
                 			       -out ${TEMP}/apTvPrRigidTransformResampleTest.tif
                             -transform.type translation
                             -transform.type.translation.tx -5
                             -transform.type.translation.ty 5
                             -interpolator nn
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/owTvRigidTransformResampleTest.tif
                			  ${TEMP}/apTvPrRigidTransformResampleTest.tif)
                			  
                        
#----------- RefineSensorModel TESTS ----------------
#otb_test_application(NAME  apTvPrRefineSensorModel
#                     APP  RefineSensorModel
#                     OPTIONS -ingeom    ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.geom
#                             -inpoints  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500_in_tie_points.txt
#                 	         -outgeom   ${TEMP}/apTvPrRefineSensorModelGeomOutput.geom
#                             -outstat   ${TEMP}/apTvPrRefineSensorModelStatsOutput.txt
#                             -outvector ${TEMP}/apTvPrRefineSensorModelVectorOutput.shp
#                             -map utm -map.utm.zone 31 -map.utm.northhem 1
#                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
#                             -elev.geoid ${INPUTDATA}/DEM/egm96.grd
#                     VALID   --compare-ascii 0.1
#                             ${BASELINE_FILES}/apTvPrRefineSensorModelGeomOutput.geom
#                             ${TEMP}/apTvPrRefineSensorModelGeomOutput.geom
#                             --ignore-lines-with 1 support_data.)

# Remove baseline for OSSIM >= 2, as ossimRPCProjection.optimizeFit() is broken
# set(VALID_CONDITION)
# if(OTB_OSSIM_VERSION LESS 20200)
# set(VALID_CONDITION
#   VALID --compare-ascii 0.1
#         ${BASELINE_FILES}/apTvPrRefineSensorModelGeomOutput2.geom
#         ${TEMP}/apTvPrRefineSensorModelGeomOutput2.geom)
#endif()

#otb_test_application(NAME  apTvPrRefineSensorModel2
#                     APP  RefineSensorModel
#                     OPTIONS -ingeom    ${INPUTDATA}/s1a-iw-grd-vh-20150210t060009-20150210t060034-004557-0059a3-002.geom 
#                             -inpoints  ${INPUTDATA}/pts_idV5.txt
#                 	         -outgeom   ${TEMP}/apTvPrRefineSensorModelGeomOutput2.geom
#                     ${VALID_CONDITION})