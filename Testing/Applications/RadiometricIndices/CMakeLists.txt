set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_ReadImageInfo> )

add_test(NAME apTvRIRadiometricVegetationIndices
         COMMAND otbTestDriver --compare-image ${NOTOL}
                    ${BASELINE}/apTvRIRadiometricVegetationIndices.tif
                    ${TEMP}/apTvRIRadiometricVegetationIndices.tif
         
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                    RadiometricVegetationIndices
                    $<TARGET_FILE_DIR:otbapp_RadiometricVegetationIndices>
                    --in ${INPUTDATA}/veryverySmallFSATSW.tif
                    --index.ndvi 1 
                    --index.rvi 1 
                    --index.ipvi 1
                    --out ${TEMP}/apTvRIRadiometricVegetationIndices.tif
                    )

