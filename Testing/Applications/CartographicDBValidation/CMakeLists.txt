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
