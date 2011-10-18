
#--- LineSegmentDetection ---#

IF(OTB_DATA_USE_LARGEINPUT)

add_test(NAME apTvFeLineSegmentDetection
         COMMAND otbTestDriver
                 --compare-ogr ${EPSILON_9}
                   ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI.shp
                   ${TEMP}/feTvLineSegmentDetection_LI.shp
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 LineSegmentDetection
                 $<TARGET_FILE_DIR:otbapp_LineSegmentDetection>
                 --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF
                 --out ${TEMP}/feTvLineSegmentDetection_LI.shp)

add_test(NAME apTvFeLineSegmentDetectionNoRescale
         COMMAND otbTestDriver
                 --compare-ogr ${EPSILON_9}
                   ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI_NoRescale.shp
                   ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 LineSegmentDetection
                 $<TARGET_FILE_DIR:otbapp_LineSegmentDetection>
                 --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF
                 --out ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp)

ENDIF(OTB_DATA_USE_LARGEINPUT)