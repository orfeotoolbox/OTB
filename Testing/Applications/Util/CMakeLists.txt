
set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_Smoothing> )


add_test(NAME exTvRescaleTest
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 Rescale
                 --modulePath $<TARGET_FILE_DIR:otbapp_Rescale>
                 --in ${INPUTDATA}/poupees.tif
                 --out ${TEMP}/owTvRescaleTest.tif
                 --outmin 20
                 --outmax 150 )

add_test(NAME exTvSmoothingTest
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 Smoothing
                 --modulePath $<TARGET_FILE_DIR:otbapp_Smoothing>
                 --in ${INPUTDATA}/poupees.tif
                 --out ${TEMP}/owTvSmoothingTest2.tif
                 --type mean)

add_test(NAME exTvReadImageInfoTest
         COMMAND otbTestDriver
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ReadImageInfo
                 --modulePath $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${INPUTDATA}/poupees.tif )

add_test(NAME exTvExtractROIRightInputFile 
         COMMAND otbTestDriver --compare-image ${TOL}
                   ${INPUTDATA}/couleurs_extrait.png
                   ${TEMP}/utExtractROIRightInputFile.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 ExtractROI
                 --modulePath $<TARGET_FILE_DIR:otbapp_ExtractROI>
                 --in ${INPUTDATA}/couleurs_extrait.png
                 --out ${TEMP}/utExtractROIRightInputFile.tif )
