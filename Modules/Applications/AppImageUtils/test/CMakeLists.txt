otb_module_test()
#----------- DownloadSRTMTiles TESTS ----------------
otb_test_application(NAME apTuUtDownloadSRTMTiles
                     APP  DownloadSRTMTiles
                     OPTIONS -il LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF}
                             -mode list
                             -mode.list.indir ${INPUTDATA}/DEM/srtm_directory
                     )




#----------- Convert TESTS ----------------
otb_test_application(NAME apTvUtConvertBasic
                     APP Convert
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -out ${TEMP}/apTvUtConvertBasicOutput.tif
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/apTvUtConvertBasicOutput.tif
                	     ${TEMP}/apTvUtConvertBasicOutput.tif
)

otb_test_application(NAME apTuUtConvertExtendedFilename_writer
                     APP  Convert
                     OPTIONS -in ${INPUTDATA}/ToulouseExtract_WithGeom.tif
                             -out ${TEMP}/apTvUtGeomExtendedFilename.tif?&gdal:co:TILED=YES&writegeom=false
                     )

otb_test_application(NAME apTvUtConvertWithScaling
                     APP Convert
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -out ${TEMP}/apTvUtConvertWithScalingOutput.tif uint8
                             -type linear
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/apTvUtConvertWithScalingOutput.tif
                	     ${TEMP}/apTvUtConvertWithScalingOutput.tif
)

otb_test_application(NAME apTvUtConvertExtendedFilename_readerGEOM
                     APP  Convert
                     OPTIONS -in ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&geom=${INPUTDATA}/ToulouseExtract_ModifiedGeom.geom
                             -out ${TEMP}/apTvUtGeomExtendedFilename.tif
                     VALID   --compare-ascii ${EPSILON_7}
                             ${INPUTDATA}/ToulouseExtract_ModifiedGeom.geom
                             ${TEMP}/apTvUtGeomExtendedFilename.geom)


#----------- PixelInfo TESTS ----------------

#----------- ExtractROI TESTS ----------------
otb_test_application(NAME apTvUtExtractROISizeTooBig
                     APP  ExtractROI
                     OPTIONS -in ${INPUTDATA}/couleurs_extrait.png
                 			 -out ${TEMP}/apTvUtExtractROISizeTooBig.tif
                 			 -sizex 1000000
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/couleurs_extrait.png
                             ${TEMP}/apTvUtExtractROISizeTooBig.tif)

otb_test_application(NAME apTvUtExtractROISizeNULL
                     APP  ExtractROI
                     OPTIONS -in ${INPUTDATA}/couleurs_extrait.png
                 			 -out ${TEMP}/apTvUtExtractROISizeNULL.tif
                 			 -sizex 0
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/couleurs_extrait.png
                             ${TEMP}/apTvUtExtractROISizeNULL.tif)

otb_test_application(NAME apTvUtExtractROIRightInputFile
                     APP  ExtractROI
                     OPTIONS -in ${INPUTDATA}/couleurs_extrait.png
                             -out ${TEMP}/apTvUtExtractROIRightInputFile.tif
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/couleurs_extrait.png
                             ${TEMP}/apTvUtExtractROIRightInputFile.tif)


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
                     OPTIONS -il ${EXAMPLEDATA}/Scene_R1C1.png
                                 ${EXAMPLEDATA}/Scene_R1C2.png
                                 ${EXAMPLEDATA}/Scene_R2C1.png
                                 ${EXAMPLEDATA}/Scene_R2C2.png
                             -cols 2
                             -rows 2
                             -out ${TEMP}/apTvUtTileFusion.png uint8
                     VALID   --compare-image ${NOTOL}
                             ${EXAMPLEDATA}/Scene.png
                           ${TEMP}/apTvUtTileFusion.png)


#----------- ReadImageInfo TESTS ----------------
otb_test_application(NAME apTuUtReadImageInfoExtendedFilename_reader
                     APP  ReadImageInfo
                     OPTIONS -in ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&skipgeom=true&skipcarto=true
                     )

##>otb_test_application(NAME  apTvUtReadImageInfoRpcTag
                     ##>APP  ReadImageInfo
                     ##>OPTIONS -in ${INPUTDATA}/QB_TOULOUSE_RpcTag_100_100.tif
                             ##>-outkwl ${TEMP}/apTvUtReadImageInfoRpcTag.geom
                     ##>VALID   --compare-ascii ${EPSILON_7}
                             ##>${BASELINE_FILES}/apTvUtReadImageInfoRpcTag.geom
                       ##>${TEMP}/apTvUtReadImageInfoRpcTag.geom)

##>otb_test_application(NAME  apTvUtReadImageInfoTest
                     ##>APP  ReadImageInfo
                     ##>OPTIONS -in ${INPUTDATA}/poupees.tif
                     ##>TESTENVOPTIONS ${TEMP}/apTvUtReadImageInfoTest.txt
                     ##>VALID   --compare-ascii ${EPSILON_7}
                             ##>${BASELINE_FILES}/apTvUtReadImageInfoTest.txt
                	     ##>${TEMP}/apTvUtReadImageInfoTest.txt)

##>otb_test_application(NAME  apTvUtReadImageInfoSpot5
                     ##>APP  ReadImageInfo
                     ##>OPTIONS -in LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
                     ##>TESTENVOPTIONS ${TEMP}/apTvUtReadImageInfoSpot5.txt
                     ##>VALID   --compare-ascii ${EPSILON_7}
                             ##>${BASELINE_FILES}/apTvUtReadImageInfoSpot5.txt
                	     ##>${TEMP}/apTvUtReadImageInfoSpot5.txt)

##>otb_test_application(NAME  apTvUtReadImageInfoERS
                     ##>APP  ReadImageInfo
                     ##>OPTIONS -in LARGEINPUT{SAR_ERS2_SLCI_SCENE1/DAT_01.001}
                     ##>TESTENVOPTIONS ${TEMP}/apTvUtReadImageInfoERS.txt
                     ##>VALID   --compare-ascii ${EPSILON_7}
                             ##>${BASELINE_FILES}/apTvUtReadImageInfoERS.txt
                	     ##>${TEMP}/apTvUtReadImageInfoERS.txt)

##>otb_test_application(NAME  apTvUtReadImageInfoRADARSAT1
                     ##>APP  ReadImageInfo
                     ##>OPTIONS -in LARGEINPUT{RADARSAT1/GOMA/SCENE01/DAT_01.001}
                     ##>TESTENVOPTIONS ${TEMP}/apTvUtReadImageInfoRADARSAT1.txt
                     ##>VALID   --compare-ascii ${EPSILON_7}
                             ##>${BASELINE_FILES}/apTvUtReadImageInfoRADARSAT1.txt
                	     ##>${TEMP}/apTvUtReadImageInfoRADARSAT1.txt)


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

otb_test_application(NAME apTvUtQuicklookSpot5
                     APP Quicklook
                     OPTIONS -in LARGEINPUT{SPOT5/TEHERAN/IMAGERY.TIF}
                             -out ${TEMP}/apTvUtQuicklookSpot5.tif
                             -cl Channel1 Channel2 Channel3
                             -rox 10
                             -roy 10
                             -rsx 100
                             -rsy 200
                             -sr 2
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtQuicklookSpot5.tif
                             ${TEMP}/apTvUtQuicklookSpot5.tif
                     )


#----------- DEMConvert TESTS ----------------



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
                     OPTIONS -il ${INPUTDATA}/poupees_c1
                             -out ${TEMP}/apTvUtConcatenateImages_1Image.tif
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/poupees_c1
                             ${TEMP}/apTvUtConcatenateImages_1Image.tif)


#----------- MultiResolutionPyramid TESTS ----------------

#----------- PixelValue TESTS ----------------
OTB_TEST_APPLICATION(NAME apTvUtPixelValue
                     APP PixelValue
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -coordx 30
                             -coordy 30
                             -cl Channel1 Channel3 Channel4
                       TESTENVOPTIONS ${TEMP}/apTvUtPixelValue.txt
                       VALID --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/apTvUtPixelValue.txt
                	     ${TEMP}/apTvUtPixelValue.txt
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
                     OPTIONS -in ${EXAMPLEDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
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
                     OPTIONS -in ${EXAMPLEDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
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
                	         ${EXAMPLEDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
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
                     OPTIONS -in ${EXAMPLEDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
                             -method image
                             -method.image.in ${EXAMPLEDATA}/ROI_QB_MUL_1.tif
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

