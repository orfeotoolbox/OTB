#--- MeanShift ---#

OTB_TEST_APPLICATION(NAME apTvSeMeanShift
                     APP  MeanShiftSegmentation
                     OPTIONS -in ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
                 			 -fout ${TEMP}/apTvSeMeanShift_FilterOutput.tif
                 			 -cout ${TEMP}/apTvSeMeanShift_ClusteredOutput.tif
                 			 -lout ${TEMP}/apTvSeMeanShift_LabeledClusteredOutput.tif
                 			 -cbout ${TEMP}/apTvSeMeanShift_ClusterBoundariesOutput.tif
                 			 -spatialr 16
                 			 -ranger 16
                 			 -minsize 10
                 			 -scale 1.0
                     VALID   --compare-n-images ${EPSILON_7} 4
                               ${BASELINE}/bfMeanShiftVectorImageFilterOutput.tif
                    		   ${TEMP}/apTvSeMeanShift_FilterOutput.tif
                    		   ${BASELINE}/bfMeanShiftVectorImageFilterClusteredOutput.tif
                    		   ${TEMP}/apTvSeMeanShift_ClusteredOutput.tif
                    		   ${BASELINE}/bfMeanShiftVectorImageFilterLabeledClusteredOutput.tif
                    		   ${TEMP}/apTvSeMeanShift_LabeledClusteredOutput.tif
                    		   ${BASELINE}/bfMeanShiftVectorImageFilterClusterBoundariesOutput.tif
                    		   ${TEMP}/apTvSeMeanShift_ClusterBoundariesOutput.tif)

#--- ConnectedComponentSegmentation ---#

OTB_TEST_APPLICATION(NAME  apTvCcConnectedComponentSegmentationMaskMuParserShp
                     APP  ConnectedComponentSegmentation
                     OPTIONS -in ${INPUTDATA}/ROI_QB_MUL_4.tif
                             -out ${TEMP}/apTvCcConnectedComponentSegmentationMaskMuParser.shp
                             -mask "((b1>80)*intensity>95)"
                             -expr "distance<10"
                             -minsize 15
                             -obia "SHAPE_Elongation>8"
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvCcConnectedComponentSegmentationMaskMuParser.shp
                             ${TEMP}/apTvCcConnectedComponentSegmentationMaskMuParser.shp)

OTB_TEST_APPLICATION(NAME  apTvCcConnectedComponentSegmentatioNoMask
                     APP  ConnectedComponentSegmentation
                     OPTIONS -in ${INPUTDATA}/ROI_QB_MUL_4.tif
                             -out ${TEMP}/apTvCcConnectedComponentSegmentationNoMask.shp
                             -expr "distance<10"
                             -minsize 15
                             -obia "SHAPE_Elongation>8"
                     VALID   --compare-ogr ${NOTOL}
                             ${BASELINE_FILES}/apTvCcConnectedComponentSegmentationNoMask.shp
                             ${TEMP}/apTvCcConnectedComponentSegmentationNoMask.shp)                    		   

#--- HooverCompareSegmentation ---#
OTB_TEST_APPLICATION(NAME     apTvSeHooverCompareSegmentationTest
                     APP      HooverCompareSegmentation
                     OPTIONS  -ingt ${INPUTDATA}/maur_GT.tif
                              -inms ${INPUTDATA}/maur_labelled.tif
                     TESTENVOPTIONS ${TEMP}/apTvSeHooverCompareSegmentationTest.txt
                     VALID    --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/apTvSeHooverCompareSegmentationTest.txt
                	         ${TEMP}/apTvSeHooverCompareSegmentationTest.txt
                     )
