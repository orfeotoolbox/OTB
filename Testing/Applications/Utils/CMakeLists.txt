
#--- Rescale ---#
OTB_TEST_APPLICATION(NAME  apTvUtRescaleTest
                     APP  Rescale
                     OPTIONS -in ${INPUTDATA}/poupees.tif
                             -out ${TEMP}/apTvUtRescaleTest.png uint8
                             -outmin 20
                             -outmax 150
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtRescaleTest.png
                             ${TEMP}/apTvUtRescaleTest.png)

#--- Smoothing ---#
OTB_TEST_APPLICATION(NAME  apTvUtSmoothingTest
                     APP  Smoothing
                     OPTIONS -in ${INPUTDATA}/poupees.tif
               	             -out ${TEMP}/apTvUtSmoothingTest.tif
                             -type mean
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtSmoothingTest.tif
                             ${TEMP}/apTvUtSmoothingTest.tif)
#--- ReadImageInfo ---#

OTB_TEST_APPLICATION(NAME  apTvUtReadImageInfoTest
                     APP  ReadImageInfo
                     OPTIONS -in ${INPUTDATA}/poupees.tif
                     TESTENVOPTIONS ${TEMP}/apTvUtReadImageInfoTest.txt
                     VALID   --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/apTvUtReadImageInfoTest.txt
                	     ${TEMP}/apTvUtReadImageInfoTest.txt)

IF(OTB_DATA_USE_LARGEINPUT)

OTB_TEST_APPLICATION(NAME  apTvUtReadImageInfoSpot5
                     APP  ReadImageInfo
                     OPTIONS -in ${LARGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF
                     TESTENVOPTIONS ${TEMP}/apTvUtReadImageInfoSpot5.txt
                     VALID   --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/apTvUtReadImageInfoSpot5.txt
                	     ${TEMP}/apTvUtReadImageInfoSpot5.txt)

OTB_TEST_APPLICATION(NAME  apTvUtReadImageInfoERS
                     APP  ReadImageInfo
                     OPTIONS -in ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001
                     TESTENVOPTIONS ${TEMP}/apTvUtReadImageInfoERS.txt
                     VALID   --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/apTvUtReadImageInfoERS.txt
                	     ${TEMP}/apTvUtReadImageInfoERS.txt)

OTB_TEST_APPLICATION(NAME  apTvUtReadImageInfoRADARSAT1
                     APP  ReadImageInfo
                     OPTIONS -in ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/DAT_01.001
                     TESTENVOPTIONS ${TEMP}/apTvUtReadImageInfoRADARSAT1.txt
                     VALID   --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/apTvUtReadImageInfoRADARSAT1.txt
                	     ${TEMP}/apTvUtReadImageInfoRADARSAT1.txt)

ENDIF(OTB_DATA_USE_LARGEINPUT)

#--- ExtractROI ---#

OTB_TEST_APPLICATION(NAME apTvUtExtractROIRightInputFile
                     APP  ExtractROI
                     OPTIONS -in ${INPUTDATA}/couleurs_extrait.png
                             -out ${TEMP}/apTvUtExtractROIRightInputFile.tif
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/couleurs_extrait.png
                             ${TEMP}/apTvUtExtractROIRightInputFile.tif)

OTB_TEST_APPLICATION(NAME apTvUtExtractROISizeNULL
                     APP  ExtractROI
                     OPTIONS -in ${INPUTDATA}/couleurs_extrait.png
                 			 -out ${TEMP}/apTvUtExtractROISizeNULL.tif
                 			 -sizex 0
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/couleurs_extrait.png
                             ${TEMP}/apTvUtExtractROISizeNULL.tif)

OTB_TEST_APPLICATION(NAME apTvUtExtractROISizeTooBig
                     APP  ExtractROI
                     OPTIONS -in ${INPUTDATA}/couleurs_extrait.png
                 			 -out ${TEMP}/apTvUtExtractROISizeTooBig.tif
                 			 -sizex 1000000
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/couleurs_extrait.png
                             ${TEMP}/apTvUtExtractROISizeTooBig.tif)


#--- ConcatenateImages ---#
OTB_TEST_APPLICATION(NAME apTvUtConcatenateImages_1Image
                     APP  ConcatenateImages
                     OPTIONS -il ${INPUTDATA}/poupees_c1
                             -out ${TEMP}/apTvUtConcatenateImages_1Image.tif
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/poupees_c1
                             ${TEMP}/apTvUtConcatenateImages_1Image.tif)

OTB_TEST_APPLICATION(NAME apTvUtConcatenateImages
                     APP  ConcatenateImages
                     OPTIONS -il ${INPUTDATA}/poupees_sub_c1.png
                                  ${INPUTDATA}/poupees_sub_c2.png
                                  ${INPUTDATA}/poupees_sub_c3.png
                 			 -out ${TEMP}/apTvUtConcatenateImages.png
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/poupees_sub_3c.png
                             ${TEMP}/apTvUtConcatenateImages.png)

#--- BandMath application -----

OTB_TEST_APPLICATION(NAME apTvUtBandMath
                     APP  BandMath
                     OPTIONS -il ${INPUTDATA}/poupees_sub_c1.png
                                  ${INPUTDATA}/poupees_sub_c2.png
                                  ${INPUTDATA}/poupees_sub.png
                             -out ${TEMP}/apTvUtBandMathOutput.tif
                             -exp "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3,im3b4)"
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/apTvUtBandMathOutput.tif
                             ${TEMP}/apTvUtBandMathOutput.tif)


#--- Split Image application ----

OTB_TEST_APPLICATION(NAME apTvUtSplitImage
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

#--- Convert ----
OTB_TEST_APPLICATION(NAME apTvUtConvertBasic
                     APP Convert
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -out ${TEMP}/apTvUtConvertBasicOutput.tif
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/apTvUtConvertBasicOutput.tif
                	     ${TEMP}/apTvUtConvertBasicOutput.tif
)

OTB_TEST_APPLICATION(NAME apTvUtConvertWithScaling
                     APP Convert
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -out ${TEMP}/apTvUtConvertWithScalingOutput.tif
                             -type linear
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/apTvUtConvertWithScalingOutput.tif
                	     ${TEMP}/apTvUtConvertWithScalingOutput.tif
)

#--- KmzExport ----
OTB_TEST_APPLICATION(NAME apTvUtKmzExport
                     APP KmzExport
                     OPTIONS -in   ${INPUTDATA}/qb_RoadExtract.img
                             -out ${TEMP}/apTvUtKmzExport.kmz
                             -logo ${INPUTDATA}/cnes.png
                             -legend ${INPUTDATA}/otb_big.png
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory)


IF(OTB_DATA_USE_LARGEINPUT)
#--- OSMDownloader ---
OTB_TEST_APPLICATION(NAME apTvUtOSMDownloader
                     APP OSMDownloader
                     OPTIONS -support ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF
                             -key highway
                             -out ${TEMP}/apTvUtOSMDownloader.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/otbOSMDownloaderOutput.shp
                             ${TEMP}/apTvUtOSMDownloader.shp)

#--- VectorDataTransform ---
OTB_TEST_APPLICATION(NAME apTvUtVectorDataTransform_Rotation
                     APP VectorDataTransform
                     OPTIONS -in ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
                             -vd ${INPUTDATA}/ToulouseRoad-examples.shp
                             -out ${TEMP}/apTvUtVectorDataTransform_rotation.shp
                             -transform.ro 15
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/utTvVectorDataTransformFilter_Rotation.shp
                             ${TEMP}/apTvUtVectorDataTransform_rotation.shp)

OTB_TEST_APPLICATION(NAME apTvUtVectorDataTransform_Translation_Rotation
                     APP VectorDataTransform
                     OPTIONS -in ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
                             -vd ${INPUTDATA}/ToulouseRoad-examples.shp
                             -out ${TEMP}/apTvUtVectorDataTransform_Translation_rotation.shp
                             -transform.ro 15
                             -transform.tx  5
                             -transform.ty  6
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/utTvVectorDataTransformFilter_Translation_rotation.shp
                             ${TEMP}/apTvUtVectorDataTransform_Translation_rotation.shp)

#--- VectorDataExtractROIApplication ---
OTB_TEST_APPLICATION(NAME apTvUtVectorDataExtractROIApplication
                     APP VectorDataExtractROIApplication
                     OPTIONS -io.in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             -io.vd ${OTB_DATA_LARGEINPUT_ROOT}/VECTOR/MidiPyrenees/roads.shp
                             -io.out ${TEMP}/apTvUtVectorDataExtractROIApplicationTest.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvUtVectorDataExtractROIApplicationTest.shp
                             ${TEMP}/apTvUtVectorDataExtractROIApplicationTest.shp
                     )

ENDIF(OTB_DATA_USE_LARGEINPUT)

#--- Concatenate VectorData ---
OTB_TEST_APPLICATION(NAME apTvUtConcatenateVectorData
                     APP ConcatenateVectorData
                     OPTIONS -vd   ${INPUTDATA}/waterways.shp
                                    ${INPUTDATA}/france_coastline.shp
                             -out ${TEMP}/apTvUtConcatenateVectorDataOutputTest.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/utTvConcatenateVectorData.shp
                             ${TEMP}/apTvUtConcatenateVectorDataOutputTest.shp)

#--- CompareImages ---
OTB_TEST_APPLICATION(NAME apTvUtCompareImages
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

#--- VectorDataSetField ---
OTB_TEST_APPLICATION(NAME apTvUtVectorDataSetField
                     APP VectorDataSetField
                     OPTIONS -in ${INPUTDATA}/ToulousePoints-examples.shp
                             -out ${TEMP}/apTvUtVectorDataSetFieldTest.shp
                             -fn MyField
                             -fv MyValue
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvUtVectorDataSetFieldTest.shp
                             ${TEMP}/apTvUtVectorDataSetFieldTest.shp
                     )

#--- Quicklook ---
OTB_TEST_APPLICATION(NAME apTvUtQuicklookROI1Channel
                     APP Quicklook
                     OPTIONS -in ${INPUTDATA}/couleurs_extrait.png
                             -out ${TEMP}/apTvUtQuicklookROI1Channel.tif
                             -cl Channel1
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtQuicklookROI1Channel.tif
                             ${TEMP}/apTvUtQuicklookROI1Channel.tif
                     )

IF(OTB_DATA_USE_LARGEINPUT)
OTB_TEST_APPLICATION(NAME apTvUtQuicklookSpot5
                     APP Quicklook
                     OPTIONS -in ${LARGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF
                             -out ${TEMP}/apTvUtQuicklookSpot5.tif
                             -cl Channel1 Channel2 Channel3
                             -rox 10
                             -roy 10
                             -rsx 100
                             -rsy 200
                             -sr 2
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtQuicklookSpot5.png
                             ${TEMP}/apTvUtQuicklookSpot5.tif
                     )
ENDIF(OTB_DATA_USE_LARGEINPUT)

#--- Pixel Value ---
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
#--- ColorMapping ---
OTB_TEST_APPLICATION(NAME apTvUtColorMappingLabelToColorContinuousLUTJet
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


OTB_TEST_APPLICATION(NAME apTvUtColorMappingLabelToColorCustomLUTQB
                     APP ColorMapping
                     OPTIONS -in ${EXAMPLEDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
                             -method custom
                             -method.custom.lut ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI_PNG_ColorTable.txt
                             -out ${TEMP}/apTvUtColorMappingLabelToColorCustomLUTQB.tif uint8
                     VALID   --compare-image ${NOTOL}
                	         ${BASELINE}/apTvUtColorMappingLabelToColorCustomLUTQB.tif
                	         ${TEMP}/apTvUtColorMappingLabelToColorCustomLUTQB.tif)

OTB_TEST_APPLICATION(NAME apTvUtColorMappingColorToLabelCustomLUTQB
                     APP ColorMapping
                     OPTIONS -in ${TEMP}/apTvUtColorMappingLabelToColorCustomLUTQB.tif
                             -op colortolabel
                     		 -op.colortolabel.notfound 0
                             -method custom
                             -method.custom.lut ${INPUTDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI_PNG_ColorTable.txt
                             -out ${TEMP}/apTvUtColorMappingColorToLabelCustomLUTQB.tif uint8
                     VALID   --compare-image ${NOTOL}
                	         ${EXAMPLEDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
                	         ${TEMP}/apTvUtColorMappingColorToLabelCustomLUTQB.tif)

SET_TESTS_PROPERTIES(apTvUtColorMappingColorToLabelCustomLUTQB PROPERTIES DEPENDS apTvUtColorMappingLabelToColorCustomLUTQB)


OTB_TEST_APPLICATION(NAME apTvUtColorMappingLabelToColorCustomLUT
                     APP ColorMapping
                     OPTIONS -in ${INPUTDATA}/labelImage_UnsignedChar.tif
                             -method custom
                             -method.custom.lut ${INPUTDATA}/labelImage_UnsignedChar_lut.dat
                             -out ${TEMP}/apTvUtColorMappingLabelToColorCustomLUT.tif uint8
                     VALID   --compare-image ${NOTOL}
                	         ${BASELINE}/apTvUtColorMappingLabelToColorCustomLUT.tif
                	         ${TEMP}/apTvUtColorMappingLabelToColorCustomLUT.tif)

OTB_TEST_APPLICATION(NAME apTvUtColorMappingLabelToColorOptimalLUTQB
                     APP ColorMapping
                     OPTIONS -in ${EXAMPLEDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
                             -method optimal
                             -out ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUTQB.tif uint8
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtColorMappingLabelToColorOptimalLUTQB.tif
                	         ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUTQB.tif)

OTB_TEST_APPLICATION(NAME apTvUtColorMappingColorToLabelOptimalLUTQB
                     APP ColorMapping
                     OPTIONS -in ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUTQB.tif
                             -method optimal
                             -op colortolabel
                     		 -op.colortolabel.notfound 0
                             -out ${TEMP}/apTvUtColorMappingColorToLabelOptimalLUTQB.tif uint8
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtColorMappingColorToLabelOptimalLUTQB.tif
                             ${TEMP}/apTvUtColorMappingColorToLabelOptimalLUTQB.tif)

SET_TESTS_PROPERTIES(apTvUtColorMappingColorToLabelOptimalLUTQB PROPERTIES DEPENDS apTvUtColorMappingLabelToColorOptimalLUTQB)


OTB_TEST_APPLICATION(NAME apTvUtColorMappingLabelToColorOptimalLUT
                     APP ColorMapping
                     OPTIONS -in ${INPUTDATA}/labelImage_UnsignedChar.tif
                             -method optimal
                             -out ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUT.tif uint8
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtColorMappingLabelToColorOptimalLUT.tif
                	         ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUT.tif)

OTB_TEST_APPLICATION(NAME apTvUtColorMappingColorToLabelOptimalLUT
                     APP ColorMapping
                     OPTIONS -in ${TEMP}/apTvUtColorMappingLabelToColorOptimalLUT.tif
                     		 -op colortolabel
                     		 -op.colortolabel.notfound 255
                             -method optimal
                             -out ${TEMP}/apTvUtColorMappingColorToLabelOptimalLUT.tif uint8
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/labelImage_UnsignedChar.tif
                             ${TEMP}/apTvUtColorMappingColorToLabelOptimalLUT.tif)

SET_TESTS_PROPERTIES(apTvUtColorMappingColorToLabelOptimalLUT PROPERTIES DEPENDS apTvUtColorMappingLabelToLabelOptimalLUT)

OTB_TEST_APPLICATION(NAME apTvUtColorMappingLabelToColorSupportImage
                     APP ColorMapping
                     OPTIONS -in ${EXAMPLEDATA}/ROI_QB_MUL_1_SVN_CLASS_MULTI.png
                             -method image
                             -method.image.in ${EXAMPLEDATA}/ROI_QB_MUL_1.tif
                             -out ${TEMP}/apTvUtColorMappingLabelToColorSupportImage.tif uint8
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvUtColorMappingLabelToColorSupportImage.tif
                	         ${TEMP}/apTvUtColorMappingLabelToColorSupportImage.tif)

#--- Testing Extended Filename -----

OTB_TEST_APPLICATION(NAME apTvUtConvertExtendedFilename_readerGEOM
                     APP  Convert
                     OPTIONS -in ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&geom=${INPUTDATA}/ToulouseExtract_ModifiedGeom.geom
                             -out ${TEMP}/apTvUtGeomExtendedFilename.tif
                     VALID   --compare-ascii ${EPSILON_7}
                             ${INPUTDATA}/ToulouseExtract_ModifiedGeom.geom
                             ${TEMP}/apTvUtGeomExtendedFilename.geom)

OTB_TEST_APPLICATION(NAME apTuUtReadImageInfoExtendedFilename_reader
                     APP  ReadImageInfo
                     OPTIONS -in ${INPUTDATA}/ToulouseExtract_WithGeom.tif?&skipgeom=true&skipcarto=true
                     )

OTB_TEST_APPLICATION(NAME apTuUtConvertExtendedFilename_writer
                     APP  Convert
                     OPTIONS -in ${INPUTDATA}/ToulouseExtract_WithGeom.tif
                             -out ${TEMP}/apTvUtGeomExtendedFilename.tif?&gdal:co:TILED=YES&writegeom=false
                     )

OTB_TEST_APPLICATION(NAME apTvUtTileFusion
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

#--- Testing Download SRTM tiles application ---

IF(OTB_DATA_USE_LARGEINPUT)
OTB_TEST_APPLICATION(NAME apTuUtDownloadSRTMTiles
                     APP  DownloadSRTMTiles
                     OPTIONS -il ${LARGEDATA}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
                             -mode list
                             -mode.list.indir ${INPUTDATA}/DEM/srtm_directory
                     )

ENDIF(OTB_DATA_USE_LARGEINPUT)
