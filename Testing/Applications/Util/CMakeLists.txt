
set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_ReadImageInfo> )

#--- Rescale ---#
add_test(NAME apTvUtRescaleTest
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 Rescale
                  $<TARGET_FILE_DIR:otbapp_Rescale>
                 --in ${INPUTDATA}/poupees.tif
                 --out ${TEMP}/owTvRescaleTest.tif
                 --outmin 20
                 --outmax 150 )

#--- Smoothing ---#
add_test(NAME apTvUtSmoothingTest
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 Smoothing
                  $<TARGET_FILE_DIR:otbapp_Smoothing>
                 --in ${INPUTDATA}/poupees.tif
                 --out ${TEMP}/owTvSmoothingTest2.tif
                 --type mean)

#--- ReadImageInfo ---#

add_test(NAME apTvUtReadImageInfoTest
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ReadImageInfo
                  $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${INPUTDATA}/poupees.tif )

IF(OTB_DATA_USE_LARGEINPUT)

add_test(NAME apTvUtReadImageInfoSpot5
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ReadImageInfo
                  $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${LARGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF )

add_test(NAME apTvUtReadImageInfoERS
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ReadImageInfo
                  $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 )

add_test(NAME apTvUtReadImageInfoRADARSAT1
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ReadImageInfo
                  $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/DAT_01.001 )

ENDIF(OTB_DATA_USE_LARGEINPUT)

#--- ExtractROI ---#
add_test(NAME apTvUtExtractROIRightInputFile 
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/couleurs_extrait.png
                   ${TEMP}/utExtractROIRightInputFile.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ExtractROI
                  $<TARGET_FILE_DIR:otbapp_ExtractROI>
                 --in ${INPUTDATA}/couleurs_extrait.png
                 --out ${TEMP}/utExtractROIRightInputFile.tif )

add_test(NAME apTvUtExtractROISizeNULL
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/couleurs_extrait.png
                   ${TEMP}/utExtractROISizeNULL.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ExtractROI
                  $<TARGET_FILE_DIR:otbapp_ExtractROI>
                 --in ${INPUTDATA}/couleurs_extrait.png
                 --out ${TEMP}/utExtractROISizeNULL.tif
                 --sizex 0 )

add_test(NAME apTvUtExtractROISizeTooBig
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/couleurs_extrait.png
                   ${TEMP}/utExtractROISizeNULL.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ExtractROI
                  $<TARGET_FILE_DIR:otbapp_ExtractROI>
                 --in ${INPUTDATA}/couleurs_extrait.png
                 --out ${TEMP}/utExtractROISizeNULL.tif
                 --sizex 1000000 )

#--- ConcatenateImages ---#
add_test(NAME apTvUtConcatenateImages_1Image
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/poupees_c1
                   ${TEMP}/apTvUtConcatenateImages_1Image.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ConcatenateImages
                  $<TARGET_FILE_DIR:otbapp_ConcatenateImages>
                 --il ${INPUTDATA}/poupees_c1
                 --out ${TEMP}/apTvUtConcatenateImages_1Image.tif)

add_test(NAME apTvUtConcatenateImages
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/poupees_sub_3c.png
                   ${TEMP}/apTvUtConcatenateImages.png
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ConcatenateImages
                  $<TARGET_FILE_DIR:otbapp_ConcatenateImages>
                 --il ${INPUTDATA}/poupees_sub_c1.png ${INPUTDATA}/poupees_sub_c2.png ${INPUTDATA}/poupees_sub_c3.png
                 --out ${TEMP}/apTvUtConcatenateImages.png uchar )

#--- BandMath application -----
add_test(NAME apTvUtBandMath
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/apTvUtBandMathOutput.tif
                   ${TEMP}/apTvUtBandMathOutput.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 BandMath
                  $<TARGET_FILE_DIR:otbapp_BandMath>
                 --il   ${INPUTDATA}/poupees_sub_c1.png
                 ${INPUTDATA}/poupees_sub_c2.png
                 ${INPUTDATA}/poupees_sub.png
                 --out ${TEMP}/apTvUtBandMathOutput.tif
                 --exp "cos(im1b1)+im2b1*im3b1-im3b2+ndvi(im3b3,im3b4)"
                 )

# Split Image applciation 
add_test(NAME apTvUtSplitImage
         COMMAND otbTestDriver --compare-n-images ${NOTOL} 3
                   ${INPUTDATA}/poupees_sub_c1.png
                   ${TEMP}/apTvUtSplitImageOutput_0.tif

                   ${INPUTDATA}/poupees_sub_c2.png
                   ${TEMP}/apTvUtSplitImageOutput_1.tif

                   ${INPUTDATA}/poupees_sub_c3.png
                   ${TEMP}/apTvUtSplitImageOutput_2.tif
                   
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 SplitImage
                  $<TARGET_FILE_DIR:otbapp_BandMath>
                 --in   ${INPUTDATA}/poupees_sub.png
                 --out ${TEMP}/apTvUtSplitImageOutput.tif
                 )
