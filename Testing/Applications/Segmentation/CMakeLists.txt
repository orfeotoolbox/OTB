#--- MeanShift ---#

OTB_TEST_APPLICATION(NAME apTvSeMeanShift
                     APP  MeanShiftSegmentation
                     OPTIONS --in ${INPUTDATA}/qb_RoadExtract2sub200x200.tif
                 			 --fout ${TEMP}/apTvSeMeanShift_FilterOutput.tif
                 			 --cout ${TEMP}/apTvSeMeanShift_ClusteredOutput.tif
                 			 --lout ${TEMP}/apTvSeMeanShift_LabeledClusteredOutput.tif
                 			 --cbout ${TEMP}/apTvSeMeanShift_ClusterBoundariesOutput.tif
                 			 --spatialr 16
                 			 --ranger 16
                 			 --minsize 10
                 			 --scale 1.0
                     VALID   --compare-n-images ${EPSILON_7} 4
                               ${BASELINE}/bfMeanShiftVectorImageFilterOutput.tif
                    		   ${TEMP}/apTvSeMeanShift_FilterOutput.tif
                    		   ${BASELINE}/bfMeanShiftVectorImageFilterClusteredOutput.tif
                    		   ${TEMP}/apTvSeMeanShift_ClusteredOutput.tif
                    		   ${BASELINE}/bfMeanShiftVectorImageFilterLabeledClusteredOutput.tif
                    		   ${TEMP}/apTvSeMeanShift_LabeledClusteredOutput.tif
                    		   ${BASELINE}/bfMeanShiftVectorImageFilterClusterBoundariesOutput.tif
                    		   ${TEMP}/apTvSeMeanShift_ClusterBoundariesOutput.tif)
                    		   