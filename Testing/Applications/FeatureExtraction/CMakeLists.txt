
#--- LineSegmentDetection ---#

IF(OTB_DATA_USE_LARGEINPUT)

OTB_TEST_APPLICATION(NAME   apTvFeLineSegmentDetection
                     APP  LineSegmentDetection
                     OPTIONS -in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF
                             -out ${TEMP}/feTvLineSegmentDetection_LI.shp
                     VALID   --compare-ogr ${EPSILON_9}
                             ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI.shp
                             ${TEMP}/feTvLineSegmentDetection_LI.shp)
    
OTB_TEST_APPLICATION(NAME   apTvFeLineSegmentDetectionNoRescale
                     APP  LineSegmentDetection
                     OPTIONS -in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/ROI_QB_TOULOUSE.TIF
                             -out ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp
                     VALID   --compare-ogr ${EPSILON_9}
                             ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI_NoRescale.shp
                             ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp)
  
ENDIF(OTB_DATA_USE_LARGEINPUT)

OTB_TEST_APPLICATION(NAME  apTvFEHaralickTextureExtraction
                     APP  HaralickTextureExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif                             
                             -channel 1
                             -texture simple 
                             -out ${TEMP}/apTvFEHaralickTextureExtraction.tif
                             -parameters.min 127
                             -parameters.max 1578
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEHaralickTextureExtraction.tif
                 		     ${TEMP}/apTvFEHaralickTextureExtraction.tif)
                 		     
OTB_TEST_APPLICATION(NAME  apTvFESFSTextureExtraction
                     APP  SFSTextureExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif                             
                             -channel 1 
                             -out ${TEMP}/apTvFESFSTextureExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFESFSTextureExtraction.tif
                 		     ${TEMP}/apTvFESFSTextureExtraction.tif)
                 		     