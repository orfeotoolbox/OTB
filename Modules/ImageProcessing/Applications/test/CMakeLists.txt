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

#----------- Smoothing TESTS ----------------

otb_test_application(NAME  apTvUtSmoothingTest_OutXML
                     APP  Smoothing
                     OPTIONS -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTest_OutXML.tif
                             -type mean
                             -outxml ${TEMP}/apTvUtSmoothingTest_OutXML.xml
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTest.tif
                             ${TEMP}/apTvUtSmoothingTest_OutXML.tif)

otb_test_application(NAME  apTvUtSmoothingTest_InXML
                     APP  Smoothing
                     OPTIONS -inxml ${TEMP}/apTvUtSmoothingTest_OutXML.xml
               	             -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTest_InXML.tif
                             -type mean
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTest.tif
                             ${TEMP}/apTvUtSmoothingTest_InXML.tif)

set_tests_properties( apTvUtSmoothingTest_InXML
    PROPERTIES DEPENDS apTvUtSmoothingTest_OutXML)

otb_test_application(NAME  apTvUtSmoothingTestGaussian
                     APP  Smoothing
                     OPTIONS -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTestGaussian.tif
                             -type gaussian
                             -type.gaussian.stdev 2.0
                             -type.gaussian.maxerror 0.1
                             -type.gaussian.maxwidth 64
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTestGaussian.tif
                             ${TEMP}/apTvUtSmoothingTestGaussian.tif)

otb_test_application(NAME  apTvUtSmoothingTestAnisotropic
                     APP  Smoothing
                     OPTIONS -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTestAnisotropic.tif
                             -type anidif
                             -type.anidif.timestep 0.125
                             -type.anidif.nbiter 10
                             -type.anidif.conductance 1.
                     VALID   --compare-image ${EPSILON_6}
                             ${BASELINE}/apTvUtSmoothingTestAnisotropic.tif
                             ${TEMP}/apTvUtSmoothingTestAnisotropic.tif)

#----------- Contrast TESTS ----------------

otb_test_application(NAME  apTvUtContrastTest_base
                     APP  ContrastEnhancement
                     OPTIONS -in ${INPUTDATA}/QB_Suburb.png
                             -out ${TEMP}/apTvUtContrastTest_base.tif int16
                             -bins 256
                             -spatial.local.h 51
                             -spatial.local.w 67
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtContrastTest_base.tif
                             ${TEMP}/apTvUtContrastTest_base.tif)

otb_test_application(NAME  apTvUtContrastTest_base_glob
                     APP  ContrastEnhancement
                     OPTIONS -in ${INPUTDATA}/QB_Suburb.png
                             -out ${TEMP}/apTvUtContrastTest_base_glob.tif int16
                             -bins 256
                             -spatial global
                             -minmax manual
                             -minmax.manual.min 0 
                             -minmax.manual.max 255
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtContrastTest_base_glob.tif
                             ${TEMP}/apTvUtContrastTest_base_glob.tif)

otb_test_application(NAME  apTvUtContrastTest_lum_glob
                     APP  ContrastEnhancement
                     OPTIONS -in ${INPUTDATA}/anaglyphInput1.tif
                             -out ${TEMP}/apTvUtContrastTest_lum_glob.tif int16
                             -bins 256
                             -spatial global
                             -hfact 2.7
                             -nodata 0
                             -mode lum
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtContrastTest_lum_glob.tif
                             ${TEMP}/apTvUtContrastTest_lum_glob.tif)

otb_test_application(NAME  apTvUtContrastTest_lum
                     APP  ContrastEnhancement
                     OPTIONS -in ${INPUTDATA}/anaglyphInput1.tif
                             -out ${TEMP}/apTvUtContrastTest_lum.tif int16
                             -bins 256
                             -spatial.local.h 33
                             -spatial.local.w 47
                             -hfact 2.1
                             -nodata 0
                             -mode lum
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtContrastTest_lum.tif
                             ${TEMP}/apTvUtContrastTest_lum.tif)
                             
#----------- NL Means TESTS ----------------

otb_test_application(NAME  nlMeansTest_base
                     APP  FastNLMeans
                     OPTIONS -in ${INPUTDATA}/GomaAvant.tif
                             -out ${TEMP}/GomaAvant_NLMeans.tif
                             -patchradius 2
                             -searchradius 11
                             -thresh 30
                     VALID   --compare-image ${EPSILON_7}
                             ${BASELINE}/GomaAvant_NLMeans.tif
                             ${TEMP}/GomaAvant_NLMeans.tif)


#----------- BandMath TESTS ----------------
otb_test_application(NAME apTvUtBandMath
                     APP  BandMath
                     OPTIONS -il ${INPUTDATA}/poupees_sub_c1.png
                                  ${INPUTDATA}/poupees_sub_c2.png
                                  ${INPUTDATA}/poupees_sub.png
                             -out ${TEMP}/apTvUtBandMathOutput.tif
                             -exp "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3,im3b4)"
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/apTvUtBandMathOutput.tif
                             ${TEMP}/apTvUtBandMathOutput.tif)

#----------- BandMathX TESTS ----------------                             
set(OTBBandMathXAppTest
otbBandMathXAppTests.cxx
)

add_executable(OTBBandMathXAppTest ${OTBBandMathXAppTest})
target_link_libraries(OTBBandMathXAppTest ${OTBAppMathParserX-Test_LIBRARIES})
otb_module_target_label(OTBBandMathXAppTest)

otb_add_test(NAME apTvUtBandMathX COMMAND OTBBandMathXAppTest
$<TARGET_FILE_DIR:otbapp_BandMathX>
${INPUTDATA}/apTvUtExportBandMathX.txt
)

#----------- LineSegmentDetection TESTS ----------------
otb_test_application(NAME   apTvFeLineSegmentDetectionNoRescale
                     APP  LineSegmentDetection
                     OPTIONS -in ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             -out ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp
                             -norescale true
                     VALID   --compare-ogr ${EPSILON_9}
                             ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI_NoRescale.shp
                             ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp)

otb_test_application(NAME   apTvFeLineSegmentDetection
                     APP  LineSegmentDetection
                     OPTIONS -in ${INPUTDATA}/QB_MUL_ROI_1000_100.tif
                             -out ${TEMP}/feTvLineSegmentDetection_LI.shp
                     VALID   --compare-ogr ${EPSILON_9}
                             ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI.shp
                             ${TEMP}/feTvLineSegmentDetection_LI.shp)


#----------- EdgeExtraction TESTS ----------------
otb_test_application(NAME  apTvFEEdgeExtraction
                     APP  EdgeExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -filter sobel
                             -out ${TEMP}/apTvFEEdgeExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEEdgeExtraction.tif
                 		     ${TEMP}/apTvFEEdgeExtraction.tif)

#----------- BinaryMorphologicalOperation TESTS ----------------
otb_test_application(NAME  apTvFEBinaryMorphologicalOperation
APP  BinaryMorphologicalOperation
OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask.tif
        -channel 1
        -structype ball
        -xradius 10
        -filter opening
        -foreval 1.0
        -backval 0.0
        -out ${TEMP}/apTvFEBinaryMorphologicalOperation.tif
VALID   --compare-image ${NOTOL}
                       ${BASELINE}/apTvFEBinaryMorphologicalOperation.tif
                 ${TEMP}/apTvFEBinaryMorphologicalOperation.tif)

#----------- HaralickTextureExtraction TESTS ----------------
otb_test_application(NAME  apTvFEHaralickTextureExtraction
                     APP  HaralickTextureExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -texture simple
                             -out ${TEMP}/apTvFEHaralickTextureExtraction.tif
                             -parameters.min 127
                             -parameters.max 1578
                     VALID   --compare-image ${EPSILON_15}
                   			 ${BASELINE}/apTvFEHaralickTextureExtraction.tif
                 		     ${TEMP}/apTvFEHaralickTextureExtraction.tif)


#----------- SFSTextureExtraction TESTS ----------------
otb_test_application(NAME  apTvFESFSTextureExtraction
                     APP  SFSTextureExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -out ${TEMP}/apTvFESFSTextureExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFESFSTextureExtraction.tif
                 		     ${TEMP}/apTvFESFSTextureExtraction.tif)


#----------- PantexTextureExtraction TESTS ----------------
otb_test_application(NAME  apTvFEPantexTextureExtraction
                     APP  PantexTextureExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -out ${TEMP}/apTvFEPantexTextureExtraction.tif
                             -nbin 8
                             -sradx 4
                             -srady 4
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEPantexTextureExtraction.tif
                 		     ${TEMP}/apTvFEPantexTextureExtraction.tif)

#----------- MultivariateAlterationDetector TESTS ----------------
otb_test_application(NAME   apTvChMultivariateAlterationDetector
                     APP  MultivariateAlterationDetector
                     OPTIONS -in1 ${INPUTDATA}/Spot5-Gloucester-before.tif
                             -in2 ${INPUTDATA}/Spot5-Gloucester-after.tif
                             -out ${TEMP}/apTvChMultivariateAlterationDetectorSameNbBands.tif
                     VALID   --compare-image 0.025
                             ${BASELINE}/cdTvMultivariateAlterationDetectorImageFilterOutputSameNbBands.tif
                  			 ${TEMP}/apTvChMultivariateAlterationDetectorSameNbBands.tif)

#----------- HomologousPointsExtraction TESTS ----------------
otb_test_application(NAME apTvHomologousPointsExtractionGeoBinsAnisotropic
APP  HomologousPointsExtraction
OPTIONS -in1 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
        -in2 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
        -algorithm surf
        -mode geobins
        -mode.geobins.binsize 100
        -mode.geobins.binsizey 150
        -mode.geobins.binstep 25
        -mode.geobins.binstepy 10
        -mode.geobins.margin 12
        -mfilter 1
        -precision 10
        -2wgs84 1
        -elev.dem ${INPUTDATA}/DEM/srtm_directory
        -elev.geoid ${INPUTDATA}/DEM/egm96.grd
        -out ${TEMP}/apTvHomologousPointsExtractionGeoBinsAnisotropic.txt
VALID   --compare-ascii ${EPSILON_9}
        ${BASELINE_FILES}/apTvHomologousPointsExtractionGeoBinsAnisotropic.txt
        ${TEMP}/apTvHomologousPointsExtractionGeoBinsAnisotropic.txt)

otb_test_application(NAME apTvHomologousPointsExtractionFull
APP  HomologousPointsExtraction
OPTIONS -in1 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
        -in2 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
        -algorithm surf
        -out ${TEMP}/apTvHomologousPointsExtractionFull.txt
VALID   --compare-ascii ${EPSILON_9}
        ${BASELINE_FILES}/apTvHomologousPointsExtractionFull.txt
        ${TEMP}/apTvHomologousPointsExtractionFull.txt)

otb_test_application(NAME apTvHomologousPointsExtractionGeoBins
APP  HomologousPointsExtraction
OPTIONS -in1 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
        -in2 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
        -algorithm surf
        -mode geobins
        -mode.geobins.binsize 100
        -mode.geobins.binstep 25
        -mode.geobins.margin 12
        -mfilter 1
        -precision 10
        -2wgs84 1
        -elev.dem ${INPUTDATA}/DEM/srtm_directory
        -elev.geoid ${INPUTDATA}/DEM/egm96.grd
        -out ${TEMP}/apTvHomologousPointsExtractionGeoBins.txt
VALID   --compare-ascii ${EPSILON_9}
        ${BASELINE_FILES}/apTvHomologousPointsExtractionGeoBins.txt
        ${TEMP}/apTvHomologousPointsExtractionGeoBins.txt)

#----------- DimensionalityReduction TESTS ----------------
otb_test_application(NAME   apTvFEDimensionalityReductionMAF
                     APP  DimensionalityReduction
                     OPTIONS -in ${INPUTDATA}/cupriteSubHsi.tif
                             -out ${TEMP}/apTvFEDimensionalityReductionMAF.tif
                             -method maf
                     VALID   --compare-image 0.025
                             ${BASELINE}/bfTvMaximumAutocorrelationFactorImageFilterOutput.tif
                             ${TEMP}/apTvFEDimensionalityReductionMAF.tif)

otb_test_application(NAME   apTvFEDimensionalityReductionPCA
                     APP  DimensionalityReduction
                     OPTIONS -in ${INPUTDATA}/cupriteSubHsi.tif
                             -out ${TEMP}/apTvChDimensionalityReductionPCA.tif
                             -method pca
                     VALID   --compare-image 0.025
                             ${BASELINE}/apTvChDimensionalityReductionPCA.tif
                             ${TEMP}/apTvChDimensionalityReductionPCA.tif)