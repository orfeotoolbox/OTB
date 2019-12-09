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

set(OTBAppImageUtilsTests
otbAppImageUtilsTestDriver.cxx
otbExtractROIAppTests.cxx
)

add_executable(otbAppImageUtilsTestDriver ${OTBAppImageUtilsTests})
target_link_libraries(otbAppImageUtilsTestDriver ${OTBAppImageUtils-Test_LIBRARIES})
otb_module_target_label(otbAppImageUtilsTestDriver)

#----------- DownloadSRTMTiles TESTS ----------------
otb_test_application(NAME apTuUtDownloadSRTMTiles
                     APP  DownloadSRTMTiles
                     OPTIONS -il LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
                             -mode list
                             -tiledir ${INPUTDATA}/DEM/srtm_directory
                     )

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
                     VALID   --compare-ascii ${EPSILON_7}
                             ${INPUTDATA}/ToulouseExtract_ModifiedGeom.geom
                             ${TEMP}/apTvUtGeomExtendedFilename.geom)

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
                             ${INPUTDATA}/Scene.png
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
                     -outkwl ${TEMP}/apTvUtReadImageInfoTest_${current_testname}.geom
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
                             ${BASELINE}/apTvUtQuicklookSpot5.tif
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
