
IF(OTB_DATA_USE_LARGEINPUT)

OTB_TEST_APPLICATION(NAME  apTvPrOrthorectification_UTM
                     APP  OrthoRectification
                     OPTIONS -io.in ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF
                 			 -io.out ${TEMP}/apTvPrOrthorectifTest_UTM.tif
                                         -elev  dem
                 			 -elev.dem.path ${INPUTDATA}/DEM/srtm_directory/
                 		     -outputs.ulx  374100.8
                 			 -outputs.uly  4829184.8
                 			 -outputs.sizex 500
                 			 -outputs.sizey 500
                 			 -outputs.spacingx  0.5
                 			 -outputs.spacingy  -0.5
                 			 -map utm
                 			 -opt.gridspacing 4 # Spacing of the deformation field
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/owTvOrthorectifTest_UTM.tif
                 			  ${TEMP}/apTvPrOrthorectifTest_UTM.tif)

ENDIF(OTB_DATA_USE_LARGEINPUT)

OTB_TEST_APPLICATION(NAME apTvPrRigidTransformResample
                     APP  RigidTransformResample
                     OPTIONS -in ${INPUTDATA}/poupees.tif
                 			 -out ${TEMP}/apTvPrRigidTransformResampleTest.tif
		 					 -tx 5 
							 -ty -5
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/owTvRigidTransformResampleTest.tif
                			  ${TEMP}/apTvPrRigidTransformResampleTest.tif)


OTB_TEST_APPLICATION(NAME  apTvPrConvertCartoToGeoPoint
                     APP  ConvertCartoToGeoPoint
                     OPTIONS -carto.x 374100.828
                             -carto.y 4829184.806
                 	     -mapproj utm
                 	     -mapproj.utm.northhem true
                 	     -mapproj.utm.zone 31
                     TESTENVOPTIONS ${TEMP}/apTvPrConvertCartoToGeoPoint.txt
                     VALID   --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/apTvPrConvertCartoToGeoPoint.txt
                	     ${TEMP}/apTvPrConvertCartoToGeoPoint.txt)

OTB_TEST_APPLICATION(NAME apTvPrImageEnvelopeTest
                     APP ImageEnvelope
                     OPTIONS -in ${INPUTDATA}/sensor_stereo_left.tif
                             -out ${TEMP}/apTvPrImageEnvelopeTest.shp
                     VALID   --compare-ogr ${NOTOL}
                        ${BASELINE_FILES}/apTvPrImageEnvelopeTest.shp
                        ${TEMP}/apTvPrImageEnvelopeTest.shp)

OTB_TEST_APPLICATION(NAME apTvPrObtainUTMZoneFromGeoPoint
                     APP ObtainUTMZoneFromGeoPoint
                     OPTIONS -lat 10.0
                             -lon 124.0
                     TESTENVOPTIONS ${TEMP}/apTvPrObtainUTMZoneFromGeoPoint.txt
                     VALID  --compare-ascii ${EPSILON_7}
                        ${BASELINE_FILES}/apTvPrObtainUTMZoneFromGeoPoint.txt
                        ${TEMP}/apTvPrObtainUTMZoneFromGeoPoint.txt)


OTB_TEST_APPLICATION(NAME apTvPrConvertSensorToGeoPoint
                     APP ConvertSensorToGeoPoint
                     OPTIONS -in  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -input.idx 500
                             -input.idy 500
                     TESTENVOPTIONS ${TEMP}/apTvPrConvertSensorToGeoPoint.txt
                     VALID  --compare-ascii ${EPSILON_7}
                        ${BASELINE_FILES}/apTvPrConvertSensorToGeoPoint.txt
                        ${TEMP}/apTvPrConvertSensorToGeoPoint.txt)
