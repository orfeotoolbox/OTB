otb_module_test()
#----------- RadiometricIndices TESTS ----------------
otb_test_application(NAME  apTvFERadiometricIndices_veg
                     APP  RadiometricIndices
                     OPTIONS -in ${INPUTDATA}/veryverySmallFSATSW.tif
                     		 -channels.red 3
                     		 -channels.green 2
                     		 -channels.nir 4
                             -list Vegetation:NDVI Vegetation:RVI Vegetation:IPVI
                             -out ${TEMP}/apTvFERadiometricIndices_veg.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvRIRadiometricVegetationIndices.tif
                 		     ${TEMP}/apTvFERadiometricIndices_veg.tif)

otb_test_application(NAME  apTvFERadiometricIndices_wat
                     APP  RadiometricIndices
                     OPTIONS -in ${INPUTDATA}/veryverySmallFSATSW.tif
                             -list Water:NDWI2
                             -channels.red 3
                     		 -channels.green 2
                     		 -channels.nir 4
                             -out ${TEMP}/apTvFERadiometricIndices_wat.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvRIRadiometricWaterIndices_.tif
                 		     ${TEMP}/apTvFERadiometricIndices_wat.tif)

