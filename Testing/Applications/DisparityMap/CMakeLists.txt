SET(EPSILON_10 0.0000000001)

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

