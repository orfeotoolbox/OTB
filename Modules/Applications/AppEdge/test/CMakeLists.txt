otb_module_test()
#----------- LineSegmentDetection TESTS ----------------
otb_test_application(NAME   apTvFeLineSegmentDetectionNoRescale
                     APP  LineSegmentDetection
                     OPTIONS -in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF
                             -out ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp
                             -norescale true
                     VALID   --compare-ogr ${EPSILON_9}
                             ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI_NoRescale.shp
                             ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp)

otb_test_application(NAME   apTvFeLineSegmentDetection
                     APP  LineSegmentDetection
                     OPTIONS -in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF
                             -out ${TEMP}/feTvLineSegmentDetection_LI.shp
                     VALID   --compare-ogr ${EPSILON_9}
                             ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI.shp
                             ${TEMP}/feTvLineSegmentDetection_LI.shp)


#----------- EdgeExtraction TESTS ----------------
otb_test_application(NAME  apTvFEEdgeExtraction
                     APP  EdgeExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
                             -channel 1
                             -filter sobel
                             -out ${TEMP}/apTvFEEdgeExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEEdgeExtraction.tif
                 		     ${TEMP}/apTvFEEdgeExtraction.tif)

