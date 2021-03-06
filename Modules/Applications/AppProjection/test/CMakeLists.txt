#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()
#----------- OrthoRectification TESTS ----------------
otb_test_application(NAME  apTvPrOrthorectification_UTM_OutXML1
                     APP  OrthoRectification
                     OPTIONS -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                 			 -io.out ${TEMP}/apTvPrOrthorectifTest_UTM_OutXML1.tif
                 			 -elev.dem ${INPUTDATA}/DEM/srtm_directory/
                 		     -outputs.ulx  374100.8
                 			 -outputs.uly  4829184.8
                 			 -outputs.sizex 500
                 			 -outputs.sizey 500
                 			 -outputs.spacingx  0.5
                 			 -outputs.spacingy  -0.5
                 			 -map utm
                 			 -opt.gridspacing 4 # Spacing of the deformation field equal to 4 meters
                       -outxml  ${TEMP}/apTvPrOrthorectifTest_UTM_OutXML1.xml
                       -interpolator linear
                     VALID   --compare-image ${EPSILON_4}
                              ${BASELINE}/owTvOrthorectifTest_UTM.tif
                 			  ${TEMP}/apTvPrOrthorectifTest_UTM_OutXML1.tif)

otb_test_application(NAME  apTvPrOrthorectification_UTM_InXML1
                     APP  OrthoRectification
                     OPTIONS
                       -inxml  ${INPUTDATA}/apTvPrOrthorectifTest_UTM_OutXML1.xml
                 			 -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                 			 -io.out ${TEMP}/apTvPrOrthorectifTest_UTM_InXML1.tif
                 			 -elev.dem ${INPUTDATA}/DEM/srtm_directory/
                        VALID   --compare-image ${EPSILON_4}
                              ${BASELINE}/owTvOrthorectifTest_UTM.tif
                 			  ${TEMP}/apTvPrOrthorectifTest_UTM_InXML1.tif)

#otb_test_application(NAME  apTvPrOrthorectification_DEMTIF_UTM_InXML1
                     #APP  OrthoRectification
                     #OPTIONS
                       #-inxml  ${INPUTDATA}/apTvPrOrthorectifTest_DEMTIF_UTM_OutXML1.xml
                       #-io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                       #-io.out ${TEMP}/apTvPrOrthorectifTest_DEMTIF_UTM_InXML1.tif
                       #-elev.dem ${INPUTDATA}/DEM/tif_directory/
                        #VALID   --compare-image ${EPSILON_4}
                              #${BASELINE}/owTvOrthorectifTest_UTM.tif
                        #${TEMP}/apTvPrOrthorectifTest_DEMTIF_UTM_InXML1.tif)

otb_test_application(NAME  apTvPrOrthorectification_WGS84
                     APP  OrthoRectification
                     OPTIONS -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                       -io.out ${TEMP}/apTvPrOrthorectifTest_WGS84.tif
                       -elev.dem ${INPUTDATA}/DEM/srtm_directory/
                       -outputs.ulx  1.35404
                       -outputs.uly  43.65414
                       -outputs.sizex 500
                       -outputs.sizey 500
                       -outputs.spacingx  0.00000621314
                       -outputs.spacingy  -0.00000621314
                       -map wgs
                       -opt.gridspacing 0.00001242628 # Spacing of the deformation field equal to |2*outputs.spacingx| degrees
                       -interpolator linear
                     VALID   --compare-image ${EPSILON_4}
                        ${BASELINE}/owTvOrthorectifTest_WGS84.tif
                        ${TEMP}/apTvPrOrthorectifTest_WGS84.tif
                     )

otb_test_application(NAME  apTvPrOrthorectification_UTM
                     APP  OrthoRectification
                     OPTIONS -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                 			 -io.out ${TEMP}/apTvPrOrthorectifTest_UTM.tif
                 			 -elev.dem ${INPUTDATA}/DEM/srtm_directory/
                 		     -outputs.ulx  374100.8
                 			 -outputs.uly  4829184.8
                 			 -outputs.sizex 500
                 			 -outputs.sizey 500
                 			 -outputs.spacingx  0.5
                 			 -outputs.spacingy  -0.5
                 			 -map utm
                 			 -opt.gridspacing 4 # Spacing of the displacement field equal to 4 meters
                       -interpolator linear
                     VALID   --compare-image ${EPSILON_4}
                              ${BASELINE}/owTvOrthorectifTest_UTM.tif
                 			  ${TEMP}/apTvPrOrthorectifTest_UTM.tif)

#otb_test_application(NAME  apTvPrOrthorectification_DEMTIF_UTM_OutXML1
                     #APP  OrthoRectification
                     #OPTIONS -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                       #-io.out ${TEMP}/apTvPrOrthorectifTest_DEMTIF_UTM_OutXML1.tif
                       #-elev.dem ${INPUTDATA}/DEM/tif_directory/
                       #-outputs.ulx  374100.8
                       #-outputs.uly  4829184.8
                       #-outputs.sizex 500
                       #-outputs.sizey 500
                       #-outputs.spacingx  0.5
                       #-outputs.spacingy  -0.5
                       #-map utm
                       #-opt.gridspacing 4 # Spacing of the deformation field equal to 4 meters
                       #-outxml  ${TEMP}/apTvPrOrthorectifTest_DEMTIF_UTM_OutXML1.xml
                       #-interpolator linear
                     #VALID   --compare-image ${EPSILON_4}
                              #${BASELINE}/owTvOrthorectifTest_UTM.tif
                        #${TEMP}/apTvPrOrthorectifTest_DEMTIF_UTM_OutXML1.tif)

#otb_test_application(NAME  apTvPrOrthorectification_DEMTIF_UTM
                     #APP  OrthoRectification
                     #OPTIONS -io.in LARGEINPUT{QUICKBIRD/TOULOUSE/000000128955_01_P001_PAN/02APR01105228-P1BS-000000128955_01_P001.TIF}
                       #-io.out ${TEMP}/apTvPrOrthorectifTest_DEMTIF_UTM.tif
                       #-elev.dem ${INPUTDATA}/DEM/tif_directory/
                       #-outputs.ulx  374100.8
                       #-outputs.uly  4829184.8
                       #-outputs.sizex 500
                       #-outputs.sizey 500
                       #-outputs.spacingx  0.5
                       #-outputs.spacingy  -0.5
                       #-map utm
                       #-opt.gridspacing 4 # Spacing of the deformation field equal to 4 meters
                       #-interpolator linear
                     #VALID   --compare-image ${EPSILON_4}
                              #${BASELINE}/owTvOrthorectifTest_UTM.tif
                        #${TEMP}/apTvPrOrthorectifTest_DEMTIF_UTM.tif)


#----------- ConvertCartoToGeoPoint TESTS ----------------
otb_test_application(NAME  apTvPrConvertCartoToGeoPoint
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


#----------- GenerateRPCSensorModel TESTS ----------------
# Remove baseline for OSSIM >= 2.2, as ossimRPCProjection.optimizeFit() is broken
set(VALID_CONDITION)
if(OTB_OSSIM_VERSION LESS 20200)
set(VALID_CONDITION
  VALID --compare-ascii ${EPSILON_3}
        ${BASELINE_FILES}/apTvPrGenerateRPCSensorModelGeomOutput.geom
        ${TEMP}/apTvPrGenerateRPCSensorModelGeomOutput.geom)
endif()
otb_test_application(NAME  apTvPrGenerateRPCSensorModel
                     APP  GenerateRPCSensorModel
                     OPTIONS -inpoints  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500_in_tie_points.txt
                 	     -outgeom   ${TEMP}/apTvPrGenerateRPCSensorModelGeomOutput.geom
                             -outstat   ${TEMP}/apTvPrGenerateRPCSensorModelStatsOutput.txt
                             -outvector ${TEMP}/apTvPrGenerateRPCSensorModelVectorOutput.shp
                             -map utm -map.utm.zone 31 -map.utm.northhem 1
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -elev.geoid ${INPUTDATA}/DEM/egm96.grd
                     ${VALID_CONDITION})


#----------- GridBasedImageResampling TESTS ----------------

otb_test_application(NAME apTuDmGridBasedImageResamplingLeftTest
                     APP  GridBasedImageResampling
                     OPTIONS -io.in ${INPUTDATA}/sensor_stereo_left.tif
                             -io.out ${TEMP}/apTvDmGridBasedImageResamplingLeftTest.tif
                             -grid.in ${INPUTDATA}/sensor_stereo_rectif_left.tif
                             -out.sizex 158
                             -out.sizey 158
                     )


otb_test_application(NAME apTuDmGridBasedImageResamplingRightTest
                     APP  GridBasedImageResampling
                     OPTIONS -io.in ${INPUTDATA}/sensor_stereo_right.tif
                             -io.out ${TEMP}/apTvDmGridBasedImageResamplingRightTest.tif
                             -grid.in ${INPUTDATA}/sensor_stereo_rectif_right.tif
                             -out.sizex 158
                             -out.sizey 158
                     )

#----------- VectorDataReprojection TESTS ----------------
otb_test_application(NAME  apTuPrVectorDataReprojectionFromImageToMap
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/ToulousePointsUTM31N.shp
                     		     -in.kwl ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromImageToMap.shp
                             -out.proj user
                             -out.proj.user.map utm
                             -out.proj.user.map.utm.northhem true
                 	           -out.proj.user.map.utm.zone 31)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToMap
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/toulousepoints_examples.shp
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToMap.sqlite
                             -out.proj user
                             -out.proj.user.map  lambert93
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToMap.sqlite
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToMap.sqlite)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToImage
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/toulousepoints_examples.shp
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToImage.shp
                             -out.proj image
                             -out.proj.image.in  ${INPUTDATA}/QB_Toulouse_Ortho_XS.tif
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/apTvPrVectorDataReprojectionFromMapToImage.shp
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToImage.shp)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToSensor
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/toulousepoints_examples.shp
                 			 -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToSensor.shp
                             -out.proj image
                             -out.proj.image.in ${INPUTDATA}/QB_PAN_ROI_1000_100.tif
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/apTvPrVectorDataReprojectionFromMapToSensor.shp
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToSensor.shp)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToMap2
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToMap.sqlite
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToMap2.sqlite
                             -out.proj user
                             -out.proj.user.map  utm
                             -out.proj.user.map.utm.northhem true
                 	           -out.proj.user.map.utm.zone 31
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/apTvPrVectorDataReprojectionFromMapToMap2.sqlite
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToMap2.sqlite)

otb_test_application(NAME  apTvPrVectorDataReprojectionFromMapToGeo
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/toulousepoints_examples.shp
                 			       -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromMapToGeo.shp
                             -out.proj user
                             -out.proj.user.map  wgs
                     VALID   --compare-ogr ${EPSILON_9}
                              ${BASELINE_FILES}/prTvVectorDataProjectionFilterFromMapToGeo.shp
                 			  ${TEMP}/apTvPrVectorDataReprojectionFromMapToGeo.shp)

otb_test_application(NAME  apTuPrVectorDataReprojectionFromGeoToMap
                     APP  VectorDataReprojection
                     OPTIONS -in.vd  ${INPUTDATA}/ToulousePointsWGS.sqlite
                 			 -out.vd ${TEMP}/apTvPrVectorDataReprojectionFromGeoToMap.shp
                             -out.proj image
                             -out.proj.image.in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif)


#----------- ObtainUTMZoneFromGeoPoint TESTS ----------------
otb_test_application(NAME apTvPrObtainUTMZoneFromGeoPoint
                     APP ObtainUTMZoneFromGeoPoint
                     OPTIONS -lat 10.0
                             -lon 124.0
                     TESTENVOPTIONS ${TEMP}/apTvPrObtainUTMZoneFromGeoPoint.txt
                     VALID  --compare-ascii ${EPSILON_7}
                        ${BASELINE_FILES}/apTvPrObtainUTMZoneFromGeoPoint.txt
                        ${TEMP}/apTvPrObtainUTMZoneFromGeoPoint.txt)


#----------- RefineSensorModel TESTS ----------------
otb_test_application(NAME  apTvPrRefineSensorModel
                     APP  RefineSensorModel
                     OPTIONS -ingeom    ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.geom
                             -inpoints  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500_in_tie_points.txt
                 	         -outgeom   ${TEMP}/apTvPrRefineSensorModelGeomOutput.geom
                             -outstat   ${TEMP}/apTvPrRefineSensorModelStatsOutput.txt
                             -outvector ${TEMP}/apTvPrRefineSensorModelVectorOutput.shp
                             -map utm -map.utm.zone 31 -map.utm.northhem 1
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -elev.geoid ${INPUTDATA}/DEM/egm96.grd
                     VALID   --compare-ascii 0.1
                             ${BASELINE_FILES}/apTvPrRefineSensorModelGeomOutput.geom
                             ${TEMP}/apTvPrRefineSensorModelGeomOutput.geom
                             --ignore-lines-with 1 support_data.)

# Remove baseline for OSSIM >= 2, as ossimRPCProjection.optimizeFit() is broken
set(VALID_CONDITION)
if(OTB_OSSIM_VERSION LESS 20200)
set(VALID_CONDITION
  VALID --compare-ascii 0.1
        ${BASELINE_FILES}/apTvPrRefineSensorModelGeomOutput2.geom
        ${TEMP}/apTvPrRefineSensorModelGeomOutput2.geom)
endif()

otb_test_application(NAME  apTvPrRefineSensorModel2
                     APP  RefineSensorModel
                     OPTIONS -ingeom    ${INPUTDATA}/s1a-iw-grd-vh-20150210t060009-20150210t060034-004557-0059a3-002.geom 
                             -inpoints  ${INPUTDATA}/pts_idV5.txt
                 	         -outgeom   ${TEMP}/apTvPrRefineSensorModelGeomOutput2.geom
                     ${VALID_CONDITION})
                             


#----------- RigidTransformResample TESTS ----------------
otb_test_application(NAME apTvPrRigidTransformResample
                     APP  RigidTransformResample
                     OPTIONS -in ${INPUTDATA}/poupees.tif
                 			       -out ${TEMP}/apTvPrRigidTransformResampleTest.tif
                             -transform.type translation
                             -transform.type.translation.tx -5
                             -transform.type.translation.ty 5
                             -interpolator nn
                     VALID   --compare-image ${NOTOL}
                              ${BASELINE}/owTvRigidTransformResampleTest.tif
                			  ${TEMP}/apTvPrRigidTransformResampleTest.tif)
                			  



#----------- ImageEnvelope TESTS ----------------
otb_test_application(NAME apTvPrImageEnvelopeTest
                     APP ImageEnvelope
                     OPTIONS -in ${INPUTDATA}/sensor_stereo_left.tif
                             -out ${TEMP}/apTvPrImageEnvelopeTest.shp
                     VALID   --compare-ogr ${EPSILON_9}
                        ${BASELINE_FILES}/apTvPrImageEnvelopeTest.shp
                        ${TEMP}/apTvPrImageEnvelopeTest.shp)


#----------- ConvertSensorToGeoPoint TESTS ----------------
otb_test_application(NAME apTvPrConvertSensorToGeoPoint
                     APP ConvertSensorToGeoPoint
                     OPTIONS -in  ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -input.idx 500
                             -input.idy 500
                     TESTENVOPTIONS ${TEMP}/apTvPrConvertSensorToGeoPoint.txt
                     VALID  --compare-ascii ${EPSILON_7}
                        ${BASELINE_FILES}/apTvPrConvertSensorToGeoPoint.txt
                        ${TEMP}/apTvPrConvertSensorToGeoPoint.txt
                        --ignore-lines-with 2 town country)


#----------- Superimpose TESTS ----------------
otb_test_application(NAME apTvPrSuperimpose
                     APP Superimpose
                     OPTIONS -inr  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -inm ${INPUTDATA}/QB_Toulouse_Ortho_XS_ROI_170x230.tif
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -out ${TEMP}/apTvPrSuperimpose.tif int16
                             -lms 4.0
                     VALID  --compare-image ${EPSILON_2}
                        ${BASELINE}/apTvPrSuperimpose.tif
                        ${TEMP}/apTvPrSuperimpose.tif)

otb_test_application(NAME apTvPrSuperimpose_phr
                     APP Superimpose
                     OPTIONS -inr  ${INPUTDATA}/phr_pan.tif
                             -inm ${INPUTDATA}/phr_xs.tif
                             -out ${TEMP}/apTvPrSuperimposePHR.tif int16
                     VALID  --compare-image ${EPSILON_7}
                        ${BASELINE}/apTvPrSuperimposePHR.tif
                        ${TEMP}/apTvPrSuperimposePHR.tif)

otb_test_application(NAME apTvPrSuperimpose_phr_nn
                     APP Superimpose
                     OPTIONS -inr  ${INPUTDATA}/phr_pan.tif
                             -inm ${INPUTDATA}/phr_xs.tif
                             -interpolator nn
                             -out ${TEMP}/apTvPrSuperimposePHR_nn.tif int16
                     VALID  --compare-image ${EPSILON_7}
                        ${BASELINE}/apTvPrSuperimposePHR_nn.tif
                        ${TEMP}/apTvPrSuperimposePHR_nn.tif)
