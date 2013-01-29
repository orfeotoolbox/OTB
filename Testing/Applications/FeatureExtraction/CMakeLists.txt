
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
                             -norescale true
                     VALID   --compare-ogr ${EPSILON_9}
                             ${OTBAPP_BASELINE_FILES}/feTvLineSegmentDetection_LI_NoRescale.shp
                             ${TEMP}/feTvLineSegmentDetection_LI_NoRescale.shp)
  
ENDIF(OTB_DATA_USE_LARGEINPUT)

OTB_TEST_APPLICATION(NAME  apTvFERadiometricIndices_veg
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

OTB_TEST_APPLICATION(NAME  apTvFERadiometricIndices_wat
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
                 		     
OTB_TEST_APPLICATION(NAME  apTvFEBinaryMorphologicalOperation
                     APP  BinaryMorphologicalOperation
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN_Mask.tif                             
                             -channel 1 
                             -structype ball
                             -structype.ball.xradius 10
                             -filter opening
                             -filter.opening.foreval 1.0
                             -filter.opening.backval 0.0                      
                             -out ${TEMP}/apTvFEBinaryMorphologicalOperation.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEBinaryMorphologicalOperation.tif
                 		     ${TEMP}/apTvFEBinaryMorphologicalOperation.tif)	     

OTB_TEST_APPLICATION(NAME  apTvFEGrayScaleMorphologicalOperation
                     APP  GrayScaleMorphologicalOperation
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif                             
                             -channel 1 
                             -structype ball
                             -structype.ball.xradius 10
                             -filter opening                     
                             -out ${TEMP}/apTvFEGrayScaleMorphologicalOperation.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEGrayScaleMorphologicalOperation.tif
                 		     ${TEMP}/apTvFEGrayScaleMorphologicalOperation.tif)	  
                 		     
OTB_TEST_APPLICATION(NAME  apTvFELocalStatisticExtraction
                     APP  LocalStatisticExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif                             
                             -channel 1 
                             -radius 3
                             -out ${TEMP}/apTvFELocalStatisticExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/feTvRadiometricMomentsImageFilter.tif
                 		     ${TEMP}/apTvFELocalStatisticExtraction.tif)      		        
                 		     
OTB_TEST_APPLICATION(NAME  apTvFEEdgeExtraction
                     APP  EdgeExtraction
                     OPTIONS -in ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif                             
                             -channel 1 
                             -filter sobel
                             -out ${TEMP}/apTvFEEdgeExtraction.tif
                     VALID   --compare-image ${NOTOL}
                   			 ${BASELINE}/apTvFEEdgeExtraction.tif
                 		     ${TEMP}/apTvFEEdgeExtraction.tif)  

OTB_TEST_APPLICATION(NAME apTvHomologousPointsExtractionFull
                     APP  HomologousPointsExtraction
                     OPTIONS -in1 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -in2 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -algorithm surf
                             -out ${TEMP}/apTvHomologousPointsExtractionFull.txt
                     VALID   --compare-ascii ${EPSILON_9}
                             ${BASELINE_FILES}/apTvHomologousPointsExtractionFull.txt
                             ${TEMP}/apTvHomologousPointsExtractionFull.txt)

OTB_TEST_APPLICATION(NAME apTvHomologousPointsExtractionGeoBins
                     APP  HomologousPointsExtraction
                     OPTIONS -in1 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -in2 ${INPUTDATA}/QB_TOULOUSE_MUL_Extract_500_500.tif
                             -algorithm surf
                             -mode geobins
                             -mode.geobins.binsize 100
                             -mode.geobins.binstep 25
                             -mfilter 1
                             -precision 10
                             -2wgs84 1
                             -elev.dem ${INPUTDATA}/DEM/srtm_directory
                             -elev.geoid ${INPUTDATA}/DEM/egm96.grd
                             -out ${TEMP}/apTvHomologousPointsExtractionGeoBins.txt
                     VALID   --compare-ascii ${EPSILON_9}
                             ${BASELINE_FILES}/apTvHomologousPointsExtractionGeoBins.txt
                             ${TEMP}/apTvHomologousPointsExtractionGeoBins.txt)
