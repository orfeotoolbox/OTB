IF(OTB_DATA_USE_LARGEINPUT)

OTB_TEST_APPLICATION(NAME apTuCdComputePolylineFeatureFromImage_LI_NONDVI_gt
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/NDVI.TIF
                			 --vd ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/roads_ground_truth.shp
                			 --expr  "(b1 > 0.4)"
  						     --field NONDVI
                			 --out ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_NONDVI_gt.shp)
                   
OTB_TEST_APPLICATION(NAME apTuCdComputePolylineFeatureFromImage_LI_ROADSA_gt
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/SpectralAngleRoad.TIF
                			 --vd ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_NONDVI_gt.shp
                			 --expr  "(b1 > 0.25)"
  						     --field "ROADSA"
                			 --out ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_ROADSA_gt.shp)
                                   
OTB_TEST_APPLICATION(NAME apTvCdComputePolylineFeatureFromImage_LI_NOBUIL_gt
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/BUILDINGS.TIF
                			 --vd ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_ROADSA_gt.shp
                			 --expr  "(b1 == 0)"
  						     --field "NOBUIL"
                			 --out ${TEMP}/cdTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                     VALID	 --compare-ogr 0.0
                     		 ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                   			 ${TEMP}/cdTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp)

SET_TESTS_PROPERTIES(apTuCdComputePolylineFeatureFromImage_LI_ROADSA_gt PROPERTIES DEPENDS apTuCdComputePolylineFeatureFromImage_LI_NONDVI_gt)
SET_TESTS_PROPERTIES(apTvCdComputePolylineFeatureFromImage_LI_NOBUIL_gt PROPERTIES DEPENDS apTuCdComputePolylineFeatureFromImage_LI_ROADSA_gt)
				     																	       
OTB_TEST_APPLICATION(NAME apTuCdComputePolylineFeatureFromImage_LI_NONDVI_wr
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/NDVI.TIF
                			 --vd ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/wrongroads.shp
                			 --expr  "(b1 > 0.4)"
  						     --field "NONDVI"
                			 --out ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_NONDVI_wr.shp)
                   
OTB_TEST_APPLICATION(NAME apTuCdComputePolylineFeatureFromImage_LI_ROADSA_wr
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/SpectralAngleRoad.TIF
                			 --vd ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_NONDVI_wr.shp
                			 --expr  "(b1 > 0.25)"
  						     --field "ROADSA"
                			 --out ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_ROADSA_wr.shp)

OTB_TEST_APPLICATION(NAME apTvCdComputePolylineFeatureFromImage_LI_NOBUIL_wr
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/BUILDINGS.TIF
                			 --vd ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_ROADSA_wr.shp
                			 --expr  "(b1 == 0)"
  						     --field "NOBUIL"
                			 --out ${TEMP}/cdTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                     VALID	 --compare-ogr 0.0
                     		 ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                   			 ${TEMP}/cdTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp)

SET_TESTS_PROPERTIES(apTuCdComputePolylineFeatureFromImage_LI_ROADSA_wr PROPERTIES DEPENDS apTuCdComputePolylineFeatureFromImage_LI_NONDVI_wr)
SET_TESTS_PROPERTIES(apTvCdComputePolylineFeatureFromImage_LI_NOBUIL_wr PROPERTIES DEPENDS apTuCdComputePolylineFeatureFromImage_LI_ROADSA_wr)
        


OTB_TEST_APPLICATION(NAME apTuCdComputePolylineFeatureFromImage_LI_NONDVI_RoadExtractionApplication
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/NDVI.TIF
                			 --vd ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/road_extraction.shp
                			 --expr  "(b1 > 0.4)"
  						     --field "NONDVI"
                			 --out ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_NONDVI_RoadExtractionApplication.shp)
                   
OTB_TEST_APPLICATION(NAME apTuCdComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/SpectralAngleRoad.TIF
                			 --vd ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_NONDVI_RoadExtractionApplication.shp
                			 --expr  "(b1 > 0.25)"
  						     --field "ROADSA"
                			 --out ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication.shp)

OTB_TEST_APPLICATION(NAME apTvCdComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication
                     APP  ComputePolylineFeatureFromImage
                     OPTIONS --in ${OTB_DATA_LARGEINPUT_ROOT}/DEMPSTER-SHAFER/BUILDINGS.TIF
                			 --vd ${TEMP}/cdTuComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication.shp
                			 --expr  "(b1 == 0)"
  						     --field "NOBUIL"
                			 --out ${TEMP}/cdTvComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication.shp
                     VALID	 --compare-ogr 0.0
                     		 ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication.shp
                   			 ${TEMP}/cdTvComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication.shp)

SET_TESTS_PROPERTIES(apTuCdComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication PROPERTIES DEPENDS apTuCdComputePolylineFeatureFromImage_LI_NONDVI_RoadExtractionApplication)
SET_TESTS_PROPERTIES(apTvCdComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication PROPERTIES DEPENDS apTuCdComputePolylineFeatureFromImage_LI_ROADSA_RoadExtractionApplication)
        
ENDIF(OTB_DATA_USE_LARGEINPUT)



OTB_TEST_APPLICATION(NAME cdbTuDSFuzzyModelEstimation_LI
                     APP  DSFuzzyModelEstimation
                     OPTIONS --psin ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                		     --nsin ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                		     --belsup "ROADSA" 
                		     --plasup "NONDVI" "ROADSA" "NOBUIL" 
                		     --initmod ${OTB_DATA_ROOT}/Input/Dempster-Shafer/DSFuzzyModel_Init.xml 
                		     --maxnbit 4 
                		     --optobs true
                		     --out ${TEMP}/cdbTuDSFuzzyModelEstimatorOutput_LI.xml)

OTB_TEST_APPLICATION(NAME cdbTuDSFuzzyModelEstimation_LI_autoInit
                     APP  DSFuzzyModelEstimation
                     OPTIONS --psin ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                		     --nsin ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                		     --belsup "ROADSA" 
                		     --plasup "NONDVI" "ROADSA" "NOBUIL" 
                		     --desclist "NONDVI" "ROADSA" "NOBUIL"
                		     --maxnbit 4 
                		     --optobs true
                		     --out ${TEMP}/cdbTuDSFuzzyModelEstimatorOutput_LI_autoInit.xml)



OTB_TEST_APPLICATION(NAME cdbTvVectorDataDSValidationGroundTruth_LI
                     APP  VectorDataDSValidation
                     OPTIONS --in ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_gt.shp
                		     --belsup "ROADSA" 
                		     --plasup "NONDVI" "ROADSA" "NOBUIL" 
                		     --descmod ${OTB_DATA_ROOT}/Input/Dempster-Shafer/DSFuzzyModel.xml 
                		     --out ${TEMP}/cdbTvVectorDataDSValidationOutpout_LI_gt.shp
					VALID	 --compare-ogr 0.0
							 ${OTBAPP_BASELINE_FILES}/cdbTvVectorDataDSValidationOutpout_LI_gt.shp
							 ${TEMP}/cdbTvVectorDataDSValidationOutpout_LI_gt.shp)
							 

OTB_TEST_APPLICATION(NAME cdbTvVectorDataDSValidationWrongRoads_LI
                     APP  VectorDataDSValidation
                     OPTIONS --in ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_wr.shp
                		     --belsup "ROADSA" 
                		     --plasup "NONDVI" "ROADSA" "NOBUIL" 
                		     --descmod ${OTB_DATA_ROOT}/Input/Dempster-Shafer/DSFuzzyModel.xml 
                		     --out ${TEMP}/cdbTvVectorDataDSValidationOutpout_LI_wr.shp
					VALID	 --compare-ogr 0.0
							 ${OTBAPP_BASELINE_FILES}/cdbTvVectorDataDSValidationOutpout_LI_wr.shp
							 ${TEMP}/cdbTvVectorDataDSValidationOutpout_LI_wr.shp)
							 

OTB_TEST_APPLICATION(NAME cdbTuVectorDataDSValidationRoadExtractionApplication_LI
                     APP  VectorDataDSValidation
                     OPTIONS --in ${OTBAPP_BASELINE_FILES}/cdbTvComputePolylineFeatureFromImage_LI_NOBUIL_RoadExtractionApplication.shp
                		     --belsup "ROADSA" 
                		     --plasup "NONDVI" "ROADSA" "NOBUIL" 
                		     --descmod ${OTB_DATA_ROOT}/Input/Dempster-Shafer/DSFuzzyModel.xml 
                		     --out ${TEMP}/cdbTuVectorDataDSValidationOutpout_LI_RoadExtractionApplication.shp)
					
					
							

