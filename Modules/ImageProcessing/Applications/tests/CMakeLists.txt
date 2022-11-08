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