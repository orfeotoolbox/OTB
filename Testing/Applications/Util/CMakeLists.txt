
add_test(NAME exTvRescaleTest
         COMMAND otbApplicationLauncherCommandLine
                 Rescale
                 --modulePath $<TARGET_FILE_DIR:otbapp_Rescale>
                 --in ${INPUTDATA}/poupees.tif
                 --out ${TEMP}/owTvRescaleTest.tif
                 --outmin 20
                 --outmax 150 )

add_test(NAME exTvSmoothingTest
         COMMAND otbApplicationLauncherCommandLine
                 Smoothing
                 --modulePath $<TARGET_FILE_DIR:otbapp_Smoothing>
                 --in ${INPUTDATA}/poupees.tif
                 --out ${TEMP}/owTvSmoothingTest2.tif
                 --type mean)

add_test(NAME exTvReadImageInfoTest
         COMMAND otbApplicationLauncherCommandLine
                 ReadImageInfo
                 --modulePath $<TARGET_FILE_DIR:otbapp_ReadImageInfo>
                 --in ${INPUTDATA}/poupees.tif )
