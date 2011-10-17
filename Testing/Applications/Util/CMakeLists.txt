
set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_ReadImageInfo> )

#--- Rescale ---#
add_test(NAME apTvUtRescaleTest
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 Rescale
                 --modulePath $<TARGET_FILE_DIR:otbapp_Rescale>
                 --in ${INPUTDATA}/poupees.tif
                 --out ${TEMP}/owTvRescaleTest.tif
                 --outmin 20
                 --outmax 150 )

#--- Smoothing ---#
add_test(NAME apTvUtSmoothingTest
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 Smoothing
                 --modulePath $<TARGET_FILE_DIR:otbapp_Smoothing>
                 --in ${INPUTDATA}/poupees.tif
                 --out ${TEMP}/owTvSmoothingTest2.tif
                 --type mean)

#--- ReadImageInfo ---#

add_test(NAME apTvUtReadImageInfoTest
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ReadImageInfo
                 --modulePath $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${INPUTDATA}/poupees.tif )

IF(OTB_DATA_USE_LARGEINPUT)

add_test(NAME apTvUtReadImageInfoSpot5
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ReadImageInfo
                 --modulePath $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${LARGEDATA}/SPOT5/TEHERAN/IMAGERY.TIF )

add_test(NAME apTvUtReadImageInfoERS
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ReadImageInfo
                 --modulePath $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${LARGEDATA}/SAR_ERS2_SLCI_SCENE1/DAT_01.001 )

add_test(NAME apTvUtReadImageInfoRADARSAT1
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ReadImageInfo
                 --modulePath $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${LARGEDATA}/RADARSAT1/GOMA/SCENE01/DAT_01.001 )

ENDIF(OTB_DATA_USE_LARGEINPUT)

#--- ExtractROI ---#
add_test(NAME apTvUtExtractROIRightInputFile 
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/couleurs_extrait.png
                   ${TEMP}/utExtractROIRightInputFile.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ExtractROI
                 --modulePath $<TARGET_FILE_DIR:otbapp_ExtractROI>
                 --in ${INPUTDATA}/couleurs_extrait.png
                 --out ${TEMP}/utExtractROIRightInputFile.tif )

add_test(NAME apTvUtExtractROISizeNULL
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/couleurs_extrait.png
                   ${TEMP}/utExtractROISizeNULL.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ExtractROI
                 --modulePath $<TARGET_FILE_DIR:otbapp_ExtractROI>
                 --in ${INPUTDATA}/couleurs_extrait.png
                 --out ${TEMP}/utExtractROISizeNULL.tif
                 --sizex 0 )

add_test(NAME apTvUtExtractROISizeTooBig
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/couleurs_extrait.png
                   ${TEMP}/utExtractROISizeNULL.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ExtractROI
                 --modulePath $<TARGET_FILE_DIR:otbapp_ExtractROI>
                 --in ${INPUTDATA}/couleurs_extrait.png
                 --out ${TEMP}/utExtractROISizeNULL.tif
                 --sizex 1000000 )

add_test(NAME apTvUtConcatenateImages_1Image
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/poupees_c1
                   ${TEMP}/apTvUtConcatenateImages_1Image.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ConcatenateImages
                 --modulePath $<TARGET_FILE_DIR:otbapp_ExtractROI>
                 --il ${INPUTDATA}/poupees_c1
                 --out ${TEMP}/apTvUtConcatenateImages_1Image.tif)

add_test(NAME apTvUtConcatenateImages
         COMMAND otbTestDriver --compare-image ${NOTOL}
                   ${INPUTDATA}/poupees.lum
                   ${TEMP}/apTvUtConcatenateImages.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ConcatenateImages
                 --modulePath $<TARGET_FILE_DIR:otbapp_ExtractROI>
                 --il ${INPUTDATA}/poupees_I1.lum ${INPUTDATA}/poupees_I2.lun ${INPUTDATA}/poupees_I3.lum ${INPUTDATA}/poupees_I4.lum
                 --out ${TEMP}/apTvUtConcatenateImages.tif )
