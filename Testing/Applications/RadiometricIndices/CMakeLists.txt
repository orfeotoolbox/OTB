OTB_TEST_APPLICATION(NAME  apTvRIRadiometricVegetationIndices
                     APP  RadiometricVegetationIndices
                     OPTIONS -in ${INPUTDATA}/veryverySmallFSATSW.tif
                             -index.ndvi 1 
                             -index.rvi 1 
                             -index.ipvi 1
                             -out ${TEMP}/apTvRIRadiometricVegetationIndices.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvRIRadiometricVegetationIndices.tif
                 		     ${TEMP}/apTvRIRadiometricVegetationIndices.tif)

OTB_TEST_APPLICATION(NAME  apTvRIRadiometricWaterIndices
                     APP  RadiometricWaterIndices
                     OPTIONS -in ${INPUTDATA}/veryverySmallFSATSW.tif
                             -index.ndwi 0
                             -index.ndwi2 1
                             -index.srwi 1
                             -channels.rho860 1
                             -channels.rho1240 2
                             -out ${TEMP}/apTvRIRadiometricWaterIndices.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvRIRadiometricWaterIndices.tif
                 		     ${TEMP}/apTvRIRadiometricWaterIndices.tif)

OTB_TEST_APPLICATION(NAME  apTvRIRadiometricIndices_veg
                     APP  RadiometricIndices
                     OPTIONS -in ${INPUTDATA}/veryverySmallFSATSW.tif
                             -list Vegetation:NDVI Vegetation:RVI Vegetation:IPVI
                             -out ${TEMP}/apTvRIRadiometricIndices_veg.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvRIRadiometricVegetationIndices.tif
                 		     ${TEMP}/apTvRIRadiometricIndices_veg.tif)

OTB_TEST_APPLICATION(NAME  apTvRIRadiometricIndices_wat
                     APP  RadiometricIndices
                     OPTIONS -in ${INPUTDATA}/veryverySmallFSATSW.tif
                             -list Water:NDWI2
                             -out ${TEMP}/apTvRIRadiometricIndices_wat.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvRIRadiometricWaterIndices_.tif
                 		     ${TEMP}/apTvRIRadiometricIndices_wat.tif)