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
                              -outgt ${TEMP}/apTvSeHooverCompareSegmentationTestOutgt.tif
                              -outms ${TEMP}/apTvSeHooverCompareSegmentationTestOutms.tif
                     TESTENVOPTIONS ${TEMP}/apTvSeHooverCompareSegmentationTest.txt
                     VALID    --compare-ascii ${EPSILON_7}
                             ${BASELINE_FILES}/apTvSeHooverCompareSegmentationTest.txt
                	         ${TEMP}/apTvSeHooverCompareSegmentationTest.txt
                     )
                     
#--- Segmentation ---#
SET(cc_parameters "-filter.cc.expr distance<50")
SET(edison_parameters "-filter.edison.spatialr 5 -filter.edison.ranger 50 -filter.edison.minsize 10")
SET(meanshift_parameters "-filter.meanshift.spatialr 5 -filter.meanshift.ranger 50 -filter.meanshift.minsize 10")
SET(vector_parameters "-mode.vector.tilesize 500 -mode.vector.minsize 50 -mode.vector.ogroptions OVERWRITE=YES")
SET(raster_output_format ".tif uint16")
SET(vector_output_format ".sqlite")

# Loop on modes
FOREACH(mode "Raster" "Vector")
STRING(TOLOWER ${mode} lmode)
# Loop on filters
FOREACH(filter "Edison" "Meanshift" "CC" "Watershed")

STRING(TOLOWER ${filter} lfilter)
SET(OUTFILE ${TEMP}/apTvSeSegmentation${filter}${mode}${${lmode}_output_format})

#Ensure outfile is deleted
file(REMOVE ${OUTFILE})

OTB_TEST_APPLICATION(NAME     apTvSeSegmentation${filter}${mode}
                     APP      Segmentation
                     OPTIONS  -in ${EXAMPLEDATA}/qb_RoadExtract2.tif
                              -filter ${lfilter}
                              ${${lfilter}_parameters}
                              -mode ${lmode}
                              -mode.${lmode}.out ${OUTFILE}
                              ${${lmode}_parameters}
                     )

ENDFOREACH(filter)
ENDFOREACH(mode)
                     
