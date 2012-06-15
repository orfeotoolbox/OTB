SET(EPSILON_10 0.0000000001)
SET(BASELINE_EXAMPLE ${OTB_DATA_ROOT}/Baseline/Examples/DisparityMap)
#--- StereoSensorModelToElevationMap ---#

OTB_TEST_APPLICATION(NAME  apTvDmStereoSensorModelToElevationMapTest
                     APP  StereoSensorModelToElevationMap
                     OPTIONS -ref ${INPUTDATA}/sensor_stereo_left.tif
                             -sec ${INPUTDATA}/sensor_stereo_right.tif
                             -out ${TEMP}/apTvDmStereoSensorModelToElevationMapTest.tif
                             -r 3
                             -ct 0.7
                             -vt 4.0
                             -minh -20.0
                             -maxh 20.0
                             -step 1.0
                             -elev dem
                             -elev.dem.path ${INPUTDATA}/DEM/srtm_directory/
                             -elev.dem.geoid ${INPUTDATA}/DEM/egm96.grd
                     VALID   --compare-image ${EPSILON_10}
                             ${BASELINE}/apTvDmStereoSensorModelToElevationMapTest.tif
                             ${TEMP}/apTvDmStereoSensorModelToElevationMapTest.tif)

OTB_TEST_APPLICATION(NAME  apTvDmStereoSensorModelToElevationMapWithSmoothing
                     APP  StereoSensorModelToElevationMap
                     OPTIONS -ref ${INPUTDATA}/sensor_stereo_left.tif
                             -sec ${INPUTDATA}/sensor_stereo_right.tif
                             -out ${TEMP}/apTvDmStereoSensorModelToElevationMapWithSmoothing.tif
                             -r 3
                             -ct 0.7
                             -vt 4.0
                             -minh -20.0
                             -maxh 20.0
                             -step 1.0
                             -elev dem
                             -elev.dem.path ${INPUTDATA}/DEM/srtm_directory/
                             -elev.dem.geoid ${INPUTDATA}/DEM/egm96.grd
                             -rgs 2.0
                             -sgs 2.0
                     VALID   --compare-image ${EPSILON_10}
                             ${BASELINE}/apTvDmStereoSensorModelToElevationMapWithSmoothing.tif
                             ${TEMP}/apTvDmStereoSensorModelToElevationMapWithSmoothing.tif)

#--- FineRegistration ---#
OTB_TEST_APPLICATION(NAME apTvDmFineRegistrationTest
                     APP  FineRegistration
                     OPTIONS -ref ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
                             -sec ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub_warped_centered_rigid.tif
                             -out ${TEMP}/apTvDmFineRegistrationTest.tif
                             -erx 5
                             -ery 5
                             -mrx 3
                             -mry 3
                             -ssrx 8
                             -ssry 8
                             -spa 0.1
                             -cox -2
                             -vmlt 0.999
                     VALID   --compare-image ${EPSILON_10}
                             ${BASELINE}/apTvDmFineRegistrationTest.tif
                             ${TEMP}/apTvDmFineRegistrationTest.tif
                     )

#stereo reconstruction of images pair
#--- StereoRectificationGridGenerator ---#
OTB_TEST_APPLICATION(NAME apTvDmStereoRectificationGridGeneratorTest
                     APP  StereoRectificationGridGenerator
                     OPTIONS -io.inleft ${INPUTDATA}/sensor_stereo_left.tif
                             -io.inright ${INPUTDATA}/sensor_stereo_right.tif
                             -io.outleft ${TEMP}/apTvDmStereoRectificationGridGeneratorLeftTest.tif
                             -io.outright ${TEMP}/apTvDmStereoRectificationGridGeneratorRightTest.tif
                             -epi.elevation.avg.value 140
                             -epi.scale 1.0
                             -epi.step 4
                     )

#--- GridBasedImageResampling ---#
OTB_TEST_APPLICATION(NAME apTvDmGridBasedImageResamplingLeftTest
                     APP  GridBasedImageResampling
                     OPTIONS -io.in ${INPUTDATA}/sensor_stereo_left.tif
                             -io.out ${TEMP}/apTvDmGridBasedImageResamplingLeftTest.tif
                             -grid.in ${TEMP}/apTvDmStereoRectificationGridGeneratorLeftTest.tif
                             -out.sizex 158
                             -out.sizey 158
                     )

SET_TESTS_PROPERTIES(apTvDmGridBasedImageResamplingLeftTest PROPERTIES DEPENDS apTvDmStereoRectificationGridGeneratorTest)

OTB_TEST_APPLICATION(NAME apTvDmGridBasedImageResamplingRightTest
                     APP  GridBasedImageResampling
                     OPTIONS -io.in ${INPUTDATA}/sensor_stereo_right.tif
                             -io.out ${TEMP}/apTvDmGridBasedImageResamplingRightTest.tif
                             -grid.in ${TEMP}/apTvDmStereoRectificationGridGeneratorRightTest.tif
                             -out.sizex 158
                             -out.sizey 158
                     )
SET_TESTS_PROPERTIES(apTvDmGridBasedImageResamplingRightTest PROPERTIES DEPENDS apTvDmStereoRectificationGridGeneratorTest)

#--- BlockMatching ---#
OTB_TEST_APPLICATION(NAME apTvDmBlockMatchingTest
                     APP  BlockMatching
                     OPTIONS -io.inleft ${TEMP}/apTvDmGridBasedImageResamplingLeftTest.tif
                             -io.inright ${TEMP}/apTvDmGridBasedImageResamplingRightTest.tif
                             -io.out ${TEMP}/apTvDmBlockMatchingTest.tif
                             -bm.minhd -24
                             -bm.maxhd 0
                             -bm.minvd 0
                             -bm.maxvd 0
                             -mask.nodata 0
                             -bm.metric ncc
                             -bm.subpixel dichotomy
                             -bm.medianfilter.radius 2
                             -bm.medianfilter.incoherence 2.0
                     )
set_property(TEST apTvDmBlockMatchingTest APPEND PROPERTY DEPENDS apTvDmGridBasedImageResamplingRightTest)
set_property(TEST apTvDmBlockMatchingTest APPEND PROPERTY DEPENDS apTvDmGridBasedImageResamplingLeftTest)

#--- DisparityMapToElevationMap ---#
OTB_TEST_APPLICATION(NAME apTvDmDisparityMapToElevationMap
                     APP  DisparityMapToElevationMap
                     OPTIONS -io.in ${TEMP}/apTvDmBlockMatchingTest.tif
                             -io.left ${INPUTDATA}/sensor_stereo_left.tif
                             -io.right ${INPUTDATA}/sensor_stereo_right.tif
                             -io.lgrid ${TEMP}/apTvDmStereoRectificationGridGeneratorLeftTest.tif
                             -io.rgrid ${TEMP}/apTvDmStereoRectificationGridGeneratorRightTest.tif
                             -io.out ${TEMP}/apTvDmDisparityMapToElevationMapTest.tif
                             -hmin 130
                             -hmax 220
                             -elev.average.value 140
                             -step 2.5
                     VALID   --compare-image ${EPSILON_10}
                             ${BASELINE_EXAMPLE}/elevationOutput.tif
                             ${TEMP}/apTvDmDisparityMapToElevationMapTest.tif
                     )
SET_TESTS_PROPERTIES(apTvDmDisparityMapToElevationMap PROPERTIES DEPENDS apTvDmBlockMatchingTest)
