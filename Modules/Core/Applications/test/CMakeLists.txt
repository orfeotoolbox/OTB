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
target_link_libraries(otbAppImageUtilsTestDriver ${OTBAppImageUtils-Test_LIBRARIES})
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

#----------- Rescale TESTS ----------------
otb_test_application(NAME  apTvUtRescaleTest
                        APP  Rescale
                        OPTIONS -in ${INPUTDATA}/poupees.tif
                                -out ${TEMP}/apTvUtRescaleTest.png uint8
                                -outmin 20
                                -outmax 150
                        VALID   --compare-image ${NOTOL}
                                ${BASELINE}/apTvUtRescaleTest.png
                                ${TEMP}/apTvUtRescaleTest.png)


#----------- TileFusion TESTS ----------------
otb_test_application(NAME apTvUtTileFusion
                        APP TileFusion
                        OPTIONS -il ${INPUTDATA}/Scene_R1C1.png
                                ${INPUTDATA}/Scene_R1C2.png
                                ${INPUTDATA}/Scene_R2C1.png
                                ${INPUTDATA}/Scene_R2C2.png
                                -cols 2
                                -rows 2
                                -out ${TEMP}/apTvUtTileFusion.png uint8
                        VALID   --compare-image ${NOTOL}
                                ${INPUTDATA}/scene.png
                        ${TEMP}/apTvUtTileFusion.png)


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



#----------- Quicklook TESTS ----------------
otb_test_application(NAME apTvUtQuicklookROI1Channel
                        APP Quicklook
                        OPTIONS -in ${INPUTDATA}/couleurs_extrait.png
                                -out ${TEMP}/apTvUtQuicklookROI1Channel.tif
                                -cl Channel1
                        VALID   --compare-image ${NOTOL}
                                ${BASELINE}/apTvUtQuicklookROI1Channel.tif
                                ${TEMP}/apTvUtQuicklookROI1Channel.tif
                        )

# this tests has two baseline to take into account the bug described in
# https://github.com/opengeospatial/ogc_api_coverages/issues/92
# (half pixel shift applied in the wrong direction to GCPs with the PixelIsPoint convention)
# The first baseline corresponds to the gdal < 3.1.3 case,
# and the second baseline correspond to gdal >= 3.1.3 case.
otb_test_application(NAME apTvUtQuicklookWithGCP
                        APP Quicklook
                        OPTIONS -in ${INPUTDATA}/spot5SubWithGcps.tif
                                -out ${TEMP}/apTvUtQuicklookWithGCP.tif uint8
                                -sr 4
                                -rox 100
                                -roy 100
                                -rsx 200
                                -rsy 200
                        VALID   --compare-metadata ${NOTOL}
                                ${BASELINE}/apTvUtQuicklookWithGCP.tif
                                ${TEMP}/apTvUtQuicklookWithGCP.tif
                        )

otb_test_application(NAME apTvUtQuicklookSpot5
                        APP Quicklook
                        OPTIONS -in LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
                                -out ${TEMP}/apTvUtQuicklookSpot5.img
                                -cl Channel1 Channel2 Channel3
                                -rox 10
                                -roy 10
                                -rsx 100
                                -rsy 200
                                -sr 2
                        VALID   --compare-image ${NOTOL}
                                ${BASELINE}/apTvUtQuicklookSpot5.img
                                ${TEMP}/apTvUtQuicklookSpot5.img
                        )

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


#----------- Synthetize TESTS ----------------
otb_test_application(NAME apTvUtSynthetize
                        APP  Synthetize
                        OPTIONS -il ${INPUTDATA}/s1a_33NWB_vv_DES_007_20200108t044150_100x100.tif
                                ${INPUTDATA}/s1a_33NWB_vv_DES_007_20200108t044215_100x100.tif
                                -out ${TEMP}/apTvUtSynthetize.tif
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/s1a_33NWB_vv_DES_007_20200108txxxxxx_100x100.tif
                                ${TEMP}/apTvUtSynthetize.tif)

#----------- MultiResolutionPyramid TESTS ----------------

#----------- PixelValue TESTS ----------------
OTB_TEST_APPLICATION(NAME apTvUtPixelValueIndex
                        APP PixelValue
                        OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                                -coordx 30
                                -coordy 30
                                -mode index
                                -cl Channel1 Channel3 Channel4
                        TESTENVOPTIONS ${TEMP}/apTvUtPixelValueIndex.txt
                        VALID --compare-ascii ${EPSILON_7}
                                ${BASELINE_FILES}/apTvUtPixelValue.txt
                                ${TEMP}/apTvUtPixelValueIndex.txt
                                )
OTB_TEST_APPLICATION(NAME apTvUtPixelValuePhys
                        APP PixelValue
                        OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                                -coordx 374168
                                -coordy 4829165.5
                                -mode physical
                                -cl Channel1 Channel3 Channel4
                        TESTENVOPTIONS ${TEMP}/apTvUtPixelValuePhys.txt
                        VALID --compare-ascii ${EPSILON_7}
                                ${BASELINE_FILES}/apTvUtPixelValue.txt
                        ${TEMP}/apTvUtPixelValuePhys.txt
                                )

OTB_TEST_APPLICATION(NAME apTvUtPixelValueEpsg
                        APP PixelValue
                        OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                                -coordx 1.4408400058746337890625
                                -coordy 43.604839324951171875
                                -mode epsg
                                -mode.epsg.code 4326
                                -cl Channel1 Channel3 Channel4
                        TESTENVOPTIONS ${TEMP}/apTvUtPixelValueEpsg.txt
                        VALID --compare-ascii ${EPSILON_7}
                                ${BASELINE_FILES}/apTvUtPixelValue.txt
                        ${TEMP}/apTvUtPixelValueEpsg.txt
                                )

#----------- ColorMapping TESTS ----------------
otb_test_application(NAME apTvUtColorMappingLabelToColorCustomLUT
                        APP ColorMapping
                        OPTIONS -in ${INPUTDATA}/labelImage_UnsignedChar.tif
                                -method custom
                                -method.custom.lut ${INPUTDATA}/labelImage_UnsignedChar_lut.dat
                                -out ${TEMP}/apTvUtColorMappingLabelToColorCustomLUT.tif uint8
                        VALID   --compare-image ${NOTOL}
                                ${BASELINE}/apTvUtColorMappingLabelToColorCustomLUT.tif
                                ${TEMP}/apTvUtColorMappingLabelToColorCustomLUT.tif)

otb_test_application(NAME apTvUtColorMappingColorToLabelOptimalLUTQB
                        APP ColorMapping
                        OPTIONS -in ${BASELINE}/apTvUtColorMappingLabelToColorOptimalLUTQB.tif
                                -method optimal
                                -op colortolabel
                                        -op.colortolabel.notfound 0
                                -out ${TEMP}/apTvUtColorMappingColorToLabelOptimalLUTQB.tif uint8
                        VALID   --compare-image ${NOTOL}
                                ${BASELINE}/apTvUtColorMappingColorToLabelOptimalLUTQB.tif
                                ${TEMP}/apTvUtColorMappingColorToLabelOptimalLUTQB.tif)

otb_test_application(NAME apTvUtColorMappingLabelToColorCustomLUTQB
                        APP ColorMapping
                        OPTIONS -in ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
                                -method custom
                                -method.custom.lut ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI_PNG_ColorTable.txt
                                -out ${TEMP}/apTvUtColorMappingLabelToColorCustomLUTQB.tif uint8
                        VALID   --compare-image ${NOTOL}
                                ${BASELINE}/apTvUtColorMappingLabelToColorCustomLUTQB.tif
                                ${TEMP}/apTvUtColorMappingLabelToColorCustomLUTQB.tif)

otb_test_application(NAME apTvUtColorMappingColorToLabelOptimalLUT
                        APP ColorMapping
                        OPTIONS -in ${BASELINE}/apTvUtColorMappingLabelToColorOptimalLUT.tif
                                        -op colortolabel
                                        -op.colortolabel.notfound 255
                                -method optimal
                                -out ${TEMP}/apTvUtColorMappingColorToLabelOptimalLUT.tif uint8
                        VALID   --compare-image ${NOTOL}
                                ${INPUTDATA}/labelImage_UnsignedChar.tif
                                ${TEMP}/apTvUtColorMappingColorToLabelOptimalLUT.tif)

otb_test_application(NAME apTvUtColorMappingLabelToColorOptimalLUTQB
                        APP ColorMapping
                        OPTIONS -in ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
                                -method optimal
                                -out ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUTQB.tif uint8
                        VALID   --compare-image ${NOTOL}
                                ${BASELINE}/apTvUtColorMappingLabelToColorOptimalLUTQB.tif
                                ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUTQB.tif)

otb_test_application(NAME apTvUtColorMappingColorToLabelCustomLUTQB
                        APP ColorMapping
                        OPTIONS -in ${BASELINE}/apTvUtColorMappingLabelToColorCustomLUTQB.tif
                                -op colortolabel
                                        -op.colortolabel.notfound 0
                                -method custom
                                -method.custom.lut ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI_PNG_ColorTable.txt
                                -out ${TEMP}/apTvUtColorMappingColorToLabelCustomLUTQB.tif uint8
                        VALID   --compare-image ${NOTOL}
                                ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
                                ${TEMP}/apTvUtColorMappingColorToLabelCustomLUTQB.tif)

otb_test_application(NAME apTvUtColorMappingLabelToColorContinuousLUTJet
                        APP ColorMapping
                        OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                                -method continuous
                                -method.continuous.lut jet
                                -method.continuous.min 100
                                -method.continuous.max 500
                                -out ${TEMP}/QB_Toulouse_Ortho_PAN_LUTJet.tif uint8
                        VALID --compare-image ${EPSILON_7}
                                ${BASELINE}/apTvUtColorMappingContinuousLUTJet.tif
                                ${TEMP}/QB_Toulouse_Ortho_PAN_LUTJet.tif)

otb_test_application(NAME apTvUtColorMappingLabelToColorSupportImage
                        APP ColorMapping
                        OPTIONS -in ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
                                -method image
                                -method.image.in ${INPUTDATA}/ROI_QB_MUL_1.tif
                                -out ${TEMP}/apTvUtColorMappingLabelToColorSupportImage.tif uint8
                        VALID   --compare-image ${NOTOL}
                                ${BASELINE}/apTvUtColorMappingLabelToColorSupportImage.tif
                                ${TEMP}/apTvUtColorMappingLabelToColorSupportImage.tif)

otb_test_application(NAME apTvUtColorMappingLabelToColorOptimalLUT
                        APP ColorMapping
                        OPTIONS -in ${INPUTDATA}/labelImage_UnsignedChar.tif
                                -method optimal
                                -out ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUT.tif uint8
                        VALID   --compare-image ${NOTOL}
                                ${BASELINE}/apTvUtColorMappingLabelToColorOptimalLUT.tif
                                ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUT.tif)

OTB_TEST_APPLICATION(NAME apTvClLabeledImageColorMappingQB123_1
                        APP  ColorMapping
                        OPTIONS -in      ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                                -method  custom
                                -method.custom.lut ${INPUTDATA}/Classification/ColorTable.txt
                                -out     ${TEMP}/clLabeledFancyImageQB123_1.tif
                        VALID   --compare-image ${NOTOL}
                                ${OTBAPP_BASELINE}/clLabeledFancyImageQB123_1.tif
                                ${TEMP}/clLabeledFancyImageQB123_1.tif)

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


#----------- SplitImage TESTS ----------------
otb_test_application(NAME apTvUtSplitImage
                        APP SplitImage
                        OPTIONS -in  ${INPUTDATA}/poupees_sub.png
                                -out ${TEMP}/apTvUtSplitImageOutput.tif
                        VALID   --compare-n-images ${NOTOL} 3
                                ${INPUTDATA}/poupees_sub_c1.png
                                ${TEMP}/apTvUtSplitImageOutput_0.tif

                                ${INPUTDATA}/poupees_sub_c2.png
                                ${TEMP}/apTvUtSplitImageOutput_1.tif

                                ${INPUTDATA}/poupees_sub_c3.png
                                ${TEMP}/apTvUtSplitImageOutput_2.tif)

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


# ----------- OSMDownloader TESTS ----------------
#RK: JIRA-1078: the test is failing on once in every two days.
#it doesn't make much sense to keep this test which grabs input from openstreetmap
#server. So skipping them below. (2/sep/2016)
# otb_test_application(NAME apTvUtOSMDownloader
#                      APP OSMDownloader
#                      OPTIONS -support LARGEINPUT{DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF}
#                              -key highway
#                              -out ${TEMP}/apTvUtOSMDownloader.shp
#                      VALID   --compare-ogr ${NOTOL}
#                              ${OTBAPP_BASELINE_FILES}/otbOSMDownloaderOutput.shp
#                              ${TEMP}/apTvUtOSMDownloader.shp)


#----------- ConcatenateVectorData TESTS ----------------
otb_test_application(NAME apTvUtConcatenateVectorData
                     APP ConcatenateVectorData
                     OPTIONS -vd   ${INPUTDATA}/waterways.shp
                                    ${INPUTDATA}/france_coastline.shp
                             -out ${TEMP}/apTvUtConcatenateVectorDataOutputTest.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/utTvConcatenateVectorData.shp
                             ${TEMP}/apTvUtConcatenateVectorDataOutputTest.shp)
