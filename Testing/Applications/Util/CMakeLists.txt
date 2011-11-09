
set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_ReadImageInfo> )

#--- Rescale ---#


OTB_TEST_APPLICATION(NAME  apTuUtRescaleTest
                     APP  Rescale
                     OPTIONS --in ${INPUTDATA}/poupees.tif
                             --out ${TEMP}/owTvRescaleTest.tif
                             --outmin 20
                             --outmax 150)

#--- Smoothing ---#
OTB_TEST_APPLICATION(NAME  apTuUtSmoothingTest
                     APP  Smoothing
                     OPTIONS --in ${INPUTDATA}/poupees.tif
               			     --out ${TEMP}/apTuUtSmoothingTest2.tif
                 			 --type mean)
#--- ReadImageInfo ---#

OTB_TEST_APPLICATION(NAME  apTuUtReadImageInfoTest
                     APP  ReadImageInfo
                     OPTIONS --in ${INPUTDATA}/poupees.tif)

IF(OTB_DATA_USE_LARGEINPUT)

OTB_TEST_APPLICATION(NAME  apTuUtReadImageInfoSpot5
                     APP  ReadImageInfo
                     OPTIONS --in ${LARGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF)
                     
OTB_TEST_APPLICATION(NAME  apTuUtReadImageInfoERS
                     APP  ReadImageInfo
                     OPTIONS --in ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001)

OTB_TEST_APPLICATION(NAME  apTuUtReadImageInfoRADARSAT1
                     APP  ReadImageInfo
                     OPTIONS --in ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/DAT_01.001)

ENDIF(OTB_DATA_USE_LARGEINPUT)

#--- ExtractROI ---#

OTB_TEST_APPLICATION(NAME apTvUtExtractROIRightInputFile
                     APP  ExtractROI
                     OPTIONS --in ${INPUTDATA}/couleurs_extrait.png
                             --out ${TEMP}/apTvUtExtractROIRightInputFile.tif
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/couleurs_extrait.png
                             ${TEMP}/apTvUtExtractROIRightInputFile.tif)

OTB_TEST_APPLICATION(NAME apTvUtExtractROISizeNULL
                     APP  ExtractROI
                     OPTIONS --in ${INPUTDATA}/couleurs_extrait.png
                 			 --out ${TEMP}/apTvUtExtractROISizeNULL.tif
                 			 --sizex 0
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/couleurs_extrait.png
                             ${TEMP}/apTvUtExtractROISizeNULL.tif)
                             
OTB_TEST_APPLICATION(NAME apTvUtExtractROISizeTooBig
                     APP  ExtractROI
                     OPTIONS --in ${INPUTDATA}/couleurs_extrait.png
                 			 --out ${TEMP}/apTvUtExtractROISizeTooBig.tif
                 			 --sizex 1000000 
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/couleurs_extrait.png
                             ${TEMP}/apTvUtExtractROISizeTooBig.tif)


#--- ConcatenateImages ---#
OTB_TEST_APPLICATION(NAME apTvUtConcatenateImages_1Image
                     APP  ConcatenateImages
                     OPTIONS --il ${INPUTDATA}/poupees_c1
                             --out ${TEMP}/apTvUtConcatenateImages_1Image.tif
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/poupees_c1
                             ${TEMP}/apTvUtConcatenateImages_1Image.tif)

OTB_TEST_APPLICATION(NAME apTvUtConcatenateImages
                     APP  ConcatenateImages
                     OPTIONS --il ${INPUTDATA}/poupees_sub_c1.png 
                                  ${INPUTDATA}/poupees_sub_c2.png 
                                  ${INPUTDATA}/poupees_sub_c3.png
                 			 --out ${TEMP}/apTvUtConcatenateImages.png
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/poupees_sub_3c.png
                             ${TEMP}/apTvUtConcatenateImages.png)

#--- BandMath application -----

OTB_TEST_APPLICATION(NAME apTvUtBandMath
                     APP  BandMath
                     OPTIONS --il ${INPUTDATA}/poupees_sub_c1.png 
                                  ${INPUTDATA}/poupees_sub_c2.png 
                                  ${INPUTDATA}/poupees_sub.png
                             --out ${TEMP}/apTvUtBandMathOutput.tif
                             --exp "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3,im3b4)"
                     VALID   --compare-image ${NOTOL}
                             ${INPUTDATA}/apTvUtBandMathOutput.tif
                		     ${TEMP}/apTvUtBandMathOutput.tif)


#--- Split Image application ----

OTB_TEST_APPLICATION(NAME apTvUtSplitImage
                     APP SplitImage
                     OPTIONS --in  ${INPUTDATA}/poupees_sub.png
                             --out ${TEMP}/apTvUtSplitImageOutput.tif
                     VALID   --compare-n-images ${NOTOL} 3
                             ${INPUTDATA}/poupees_sub_c1.png
                             ${TEMP}/apTvUtSplitImageOutput_0.tif

                             ${INPUTDATA}/poupees_sub_c2.png
                             ${TEMP}/apTvUtSplitImageOutput_1.tif

                             ${INPUTDATA}/poupees_sub_c3.png
                             ${TEMP}/apTvUtSplitImageOutput_2.tif)

#--- Convert ----
OTB_TEST_APPLICATION(NAME apTuUtConvertBasic
                     APP Convert
                     OPTIONS --in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             --out ${TEMP}/apTuUtConvertBasicOutput.tif)

OTB_TEST_APPLICATION(NAME apTuUtConvertWithScaling
                     APP Convert
                     OPTIONS --in ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                             --out ${TEMP}/apTuUtConvertWithScaling.tif
                             --type linear)

#--- KmzExport ----
OTB_TEST_APPLICATION(NAME apTuUtKmzExport
                     APP KmzExport
                     OPTIONS --in   ${INPUTDATA}/qb_RoadExtract.img
                             --out ${TEMP}/apTvUtKmzExport.kmz
                             --logo ${INPUTDATA}/cnes.png
                             --legend ${INPUTDATA}/otb_big.png
                             --dem ${INPUTDATA}/DEM/srtm_directory)

#--- OSMDownloader ---
IF(OTB_DATA_USE_LARGEINPUT)

OTB_TEST_APPLICATION(NAME apTvUtOSMDownloader
                     APP OSMDownloader
                     OPTIONS --support ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF
                             --key highway
                             --out ${TEMP}/apTvUtOSMDownloader.shp
                     VALID   --compare-ogr ${NOTOL}  
                             ${OTBAPP_BASELINE_FILES}/otbOSMDownloaderOutput.shp
                             ${TEMP}/apTvUtOSMDownloader.shp)

ENDIF(OTB_DATA_USE_LARGEINPUT)

#--- Concatenate VectorData ---
OTB_TEST_APPLICATION(NAME apTuUtConcatenateVectorData
                     APP ConcatenateVectorData
                     OPTIONS --vd   ${INPUTDATA}/waterways.shp
                                    ${INPUTDATA}/france_coastline.shp
                             --out ${TEMP}/apTuUtConcatenateVectorDataOutputTest.shp
                     VALID   --compare-ogr ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/utTvConcatenateVectorData.shp
                             ${TEMP}/apTuUtConcatenateVectorDataOutputTest.shp)

#--- CompareImages ---
OTB_TEST_APPLICATION(NAME apTvUtCompareImages
                     APP CompareImages
                     OPTIONS --ref.in ${INPUTDATA}/poupees.tif
                             --ref.channel 2
                             --meas.in ${INPUTDATA}/brain.png
                             --meas.channel 1
                             --roi.startx 20
                             --roi.starty 15
                             --roi.sizex 150
                             --roi.sizey 200 )