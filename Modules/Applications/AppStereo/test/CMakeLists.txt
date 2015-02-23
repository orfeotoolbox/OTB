otb_module_test()
#----------- StereoFramework TESTS ----------------
 otb_test_application(NAME apTvDmStereoFramework2
                     APP  StereoFramework
                     OPTIONS -input.il LARGEINPUT{PLEIADES/tristereo_Haiti_Pan/master_pan.tif}
                     			       LARGEINPUT{PLEIADES/tristereo_Haiti_Pan/slave_pan_1.tif}
                     			       LARGEINPUT{PLEIADES/tristereo_Haiti_Pan/slave_pan_2.tif}
                     			       -input.co "0 1,0 2"
                                -elev.dem ${DEM_DATA}
                                -elev.geoid ${OTB_DATA_ROOT}/Input/DEM/egm96.grd
                                -stereorect.invgridssrate 15
                                -mask.variancet 100
                                -bm.maxhoffset 15
                                -bm.minhoffset -15
                                -bm.radius 2
                                -bm.metric ssdmean
                                -postproc.med 1
                                -postproc.bij 1
                                -output.res 1.
                                -output.out ${TEMP}/apTvStereoFrameworkHaiti2.tif
                      VALID   --compare-image ${EPSILON_10}
                            ${BASELINE}/apTvStereoFrameworkHaiti2.tif
                            ${TEMP}/apTvStereoFrameworkHaiti2.tif
                     )

otb_test_application(NAME apTvDmStereoFramework
                     APP  StereoFramework
                     OPTIONS -input.il LARGEINPUT{PLEIADES/tristereo_Haiti_Pan/phr_haiti_xt1.tif}
                     			       LARGEINPUT{PLEIADES/tristereo_Haiti_Pan/phr_haiti_xt2.tif}
                     			       LARGEINPUT{PLEIADES/tristereo_Haiti_Pan/phr_haiti_xt3.tif}
                     			       -input.co "0 1,0 2"
                                -elev.dem ${DEM_DATA}
                                -elev.geoid ${OTB_DATA_ROOT}/Input/DEM/egm96.grd
                                -stereorect.invgridssrate 15
                                -mask.variancet 100
                                -bm.maxhoffset 15
                                -bm.minhoffset -15
                                -bm.radius 2
                                -bm.metric ssdmean
                                -postproc.med 1
                                -postproc.bij 1
                                -output.res 1.
                                -output.out ${TEMP}/apTvStereoFrameworkHaiti.tif
                     VALID   --compare-image ${EPSILON_10}
                             ${BASELINE}/apTvStereoFrameworkHaiti.tif
                             ${TEMP}/apTvStereoFrameworkHaiti.tif
                     )


#----------- StereoRectificationGridGenerator TESTS ----------------
otb_test_application(NAME apTuDmStereoRectificationGridGeneratorTest
                     APP  StereoRectificationGridGenerator

                     OPTIONS -io.inleft ${EXAMPLEDATA}/sensor_stereo_left.tif
                             -io.inright ${EXAMPLEDATA}/sensor_stereo_right.tif
                             -io.outleft ${TEMP}/apTvDmStereoRectificationGridGeneratorLeftTest.tif
                             -io.outright ${TEMP}/apTvDmStereoRectificationGridGeneratorRightTest.tif
                             -inverse.outleft  ${TEMP}/apTvDmStereoRectificationGridGeneratorInverseLeftTest.tif
                             -inverse.outright ${TEMP}/apTvDmStereoRectificationGridGeneratorInverseRightTest.tif
                             -epi.elevation.default 140
                             -epi.scale 1.0
                             -epi.step 4
                     VALID   --compare-n-images ${EPSILON_10} 4
                             ${BASELINE}/apTvDmStereoRectificationGridGeneratorLeftTest.tif
                             ${TEMP}/apTvDmStereoRectificationGridGeneratorLeftTest.tif
                             ${BASELINE}/apTvDmStereoRectificationGridGeneratorRightTest.tif
                             ${TEMP}/apTvDmStereoRectificationGridGeneratorRightTest.tif
                             ${BASELINE}/apTvDmStereoRectificationGridGeneratorInverseLeftTest.tif
                             ${TEMP}/apTvDmStereoRectificationGridGeneratorInverseLeftTest.tif
                             ${BASELINE}/apTvDmStereoRectificationGridGeneratorInverseRightTest.tif
                             ${TEMP}/apTvDmStereoRectificationGridGeneratorInverseRightTest.tif
                     )


#----------- FineRegistration TESTS ----------------
otb_test_application(NAME apTvDmFineRegistrationTest
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


#----------- GeneratePlyFile TESTS ----------------

#----------- DisparityMapToElevationMap TESTS ----------------
otb_test_application(NAME apTvDmDisparityMapToElevationMap
                     APP  DisparityMapToElevationMap
                     OPTIONS -io.in ${INPUTDATA}/sensor_stereo_blockmatching_output.tif
                             -io.left ${EXAMPLEDATA}/sensor_stereo_left.tif
                             -io.right ${EXAMPLEDATA}/sensor_stereo_right.tif
                             -io.lgrid ${INPUTDATA}/sensor_stereo_rectif_left.tif
                             -io.rgrid ${INPUTDATA}/sensor_stereo_rectif_right.tif
                             -io.out ${TEMP}/apTvDmDisparityMapToElevationMapTest.tif
                             -hmin 130
                             -hmax 220
                             -elev.default 140
                             -step 2.5
                             -io.mask ${INPUTDATA}/sensor_stereo_dmap_mask.tif
                     VALID   --compare-image ${EPSILON_10}
                             ${BASELINE_EXAMPLE}/elevationOutput.tif
                             ${TEMP}/apTvDmDisparityMapToElevationMapTest.tif
                     )


#----------- BlockMatching TESTS ----------------
otb_test_application(NAME apTvDmBlockMatchingTest
                     APP  BlockMatching
                     OPTIONS -io.inleft ${INPUTDATA}/sensor_stereo_left_gridbasedresampling.tif
                             -io.inright ${INPUTDATA}/sensor_stereo_right_gridbasedresampling.tif
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
                     VALID   --compare-image ${EPSILON_10}
                         ${BASELINE}/apTvDmBlockMatchingTest.tif
                         ${TEMP}/apTvDmBlockMatchingTest.tif
                     )

