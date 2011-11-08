set(TEST_DRIVER otbTestDriver
    --add-before-env ITK_AUTOLOAD_PATH $<TARGET_FILE_DIR:otbapp_ReadImageInfo> )


OTB_TEST_APPLICATION(NAME  apTvRIRadiometricVegetationIndices
                     APP  RadiometricVegetationIndices
                     OPTIONS --in ${INPUTDATA}/veryverySmallFSATSW.tif
                             --index.ndvi 1 
                             --index.rvi 1 
                             --index.ipvi 1
                             --out ${TEMP}/apTvRIRadiometricVegetationIndices.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvRIRadiometricVegetationIndices.tif
                 		     ${TEMP}/apTvRIRadiometricVegetationIndices.tif)
