#--- MeanShift ---#


# VALIDATION should be enabled (baseline is same same as bfTvMeanShiftSmoothingImageFilterQBSuburbNonOptim) 
# results should be the same if thres is stored as double
#OTB_TEST_APPLICATION(NAME apTvSeMeanShiftSmoothing
OTB_TEST_APPLICATION(NAME apTuSeMeanShiftSmoothing 
                     APP  MeanShiftSmoothing
                     OPTIONS -in  ${INPUTDATA}/QB_Suburb.png
                             -fout ${TEMP}/apTvSeMeanShiftSmoothingFilter_SpectralOutput.tif
                             -foutpos ${TEMP}/apTvSeMeanShiftSmoothingFilter_SpatialOutput.tif
                 			 -spatialr 4
                 			 -ranger 25
                 			 -maxiter 100
                 			 -thres 0.1
                 			 -modesearch 0)
#                     VALID   --compare-image ${EPSILON_7} 
#                               ${BASELINE}/bfMeanShiftSmoothingImageFilterSpectralOutput_QBSuburbNonOptim.tif
#                    		   ${TEMP}/apTvSeMeanShiftSmoothingFilter_SpectralOutput.tif)

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
                              --ignore-lines-with 2 outgt: outms:
                     )
                     
#--- Segmentation ---#

# Segmentation algorithms parameters
SET(cc_parameters "-filter.cc.expr distance<25")
SET(edison_parameters "-filter.edison.spatialr 5 -filter.edison.ranger 50 -filter.edison.minsize 10")
SET(meanshift_parameters "-filter.meanshift.spatialr 5 -filter.meanshift.ranger 50 -filter.meanshift.minsize 10")
SET(vector_parameters "-mode.vector.tilesize 500 -mode.vector.minsize 50 -mode.vector.simplify 0.1 -mode.vector.outmode ovw")

# validation option
SET(meanshift_option "--without-threads")

# Output type dependning on mode
SET(raster_output_format ".tif")
SET(raster_output_option " uint16")
SET(vector_output_format ".sqlite")

# Validation depending on mode 
SET(vector_comparison --compare-ogr ${NOTOL})
SET(raster_comparison --compare-image ${NOTOL})

# Reference ffiles depending on modes
SET(vector_ref_path ${BASELINE_FILES})
SET(raster_ref_path ${BASELINE})

# Loop on modes
FOREACH(mode "Raster" "Vector")
STRING(TOLOWER ${mode} lmode)

# Loop on filters
FOREACH(filter "Edison" "Meanshift" "CC" "Watershed")
STRING(TOLOWER ${filter} lfilter)

# Derive output file name
SET(OUTFILE apTvSeSegmentation${filter}${mode}${${lmode}_output_format})

OTB_TEST_APPLICATION(NAME     apTvSeSegmentation${filter}${mode}
                     APP      Segmentation
                     OPTIONS  -in ${EXAMPLEDATA}/qb_RoadExtract2.tif
                              -filter ${lfilter}
                              ${${lfilter}_parameters}
                              -mode ${lmode}
                              -mode.${lmode}.out ${TEMP}/${OUTFILE}${${lmode}_output_option}
                              ${${lmode}_parameters}
                     VALID    ${${lfilter}_option}
                              ${${lmode}_comparison}
                              ${${lmode}_ref_path}/${OUTFILE}
                              ${TEMP}/${OUTFILE}                           
                     )

ENDFOREACH(filter)
ENDFOREACH(mode)

SET(filter "CC")
SET(mode "Vector")

STRING(TOLOWER ${filter} lfilter)

# Add a test for ULOVW
SET(vector_parameters "-mode.vector.tilesize 500 -mode.vector.minsize 50 -mode.vector.simplify 0.1 -mode.vector.outmode ulovw")
                     
SET(OUTFILE apTvSeSegmentation${filter}${mode}${${lmode}_output_format})

OTB_TEST_APPLICATION(NAME     apTvSeSegmentation${filter}${mode}_ULOVW
                     APP      Segmentation
                     OPTIONS  -in ${EXAMPLEDATA}/qb_RoadExtract2.tif
                              -filter ${lfilter}
                              ${${lfilter}_parameters}
                              -mode ${lmode}
                              -mode.${lmode}.out ${TEMP}/${OUTFILE}${${lmode}_output_option}
                              ${${lmode}_parameters}
                     VALID    ${${lfilter}_option}
                              ${${lmode}_comparison}
                              ${${lmode}_ref_path}/${OUTFILE}
                              ${TEMP}/${OUTFILE}                           
                     )

SET_TESTS_PROPERTIES(apTvSeSegmentation${filter}${mode}_ULOVW
                     PROPERTIES DEPENDS apTvSeSegmentation${filter}${mode}
                                RESOURCE_LOCK ${OUTFILE})

# Add a test for ULU
SET(vector_parameters "-mode.vector.tilesize 500 -mode.vector.minsize 50 -mode.vector.simplify 0.1 -mode.vector.outmode ulu")

OTB_TEST_APPLICATION(NAME     apTvSeSegmentation${filter}${mode}_ULU
                     APP      Segmentation
                     OPTIONS  -in ${EXAMPLEDATA}/qb_RoadExtract2.tif
                              -filter ${lfilter}
                              ${${lfilter}_parameters}
                              -mode ${lmode}
                              -mode.${lmode}.out ${TEMP}/${OUTFILE}${${lmode}_output_option}
                              ${${lmode}_parameters}
                     VALID    ${${lfilter}_option}
                              ${${lmode}_comparison}
                              ${${lmode}_ref_path}/${OUTFILE}_ULU
                              ${TEMP}/${OUTFILE}                           
                     )

SET_TESTS_PROPERTIES(apTvSeSegmentation${filter}${mode}_ULU
                     PROPERTIES DEPENDS apTvSeSegmentation${filter}${mode}
                                RESOURCE_LOCK ${OUTFILE})

# Add a test for ULCO
SET(vector_parameters "-mode.vector.tilesize 500 -mode.vector.minsize 50 -mode.vector.simplify 0.1 -mode.vector.outmode ulco")

OTB_TEST_APPLICATION(NAME     apTvSeSegmentation${filter}${mode}_ULCO
                     APP      Segmentation
                     OPTIONS  -in ${EXAMPLEDATA}/qb_RoadExtract2.tif
                              -filter ${lfilter}
                              ${${lfilter}_parameters}
                              -mode ${lmode}
                              -mode.${lmode}.out ${TEMP}/${OUTFILE}${${lmode}_output_option}
                              ${${lmode}_parameters}
                     VALID    ${${lfilter}_option}
                              ${${lmode}_comparison}
                              ${${lmode}_ref_path}/${OUTFILE}
                              ${TEMP}/${OUTFILE}                           
                     )

SET_TESTS_PROPERTIES(apTvSeSegmentation${filter}${mode}_ULCO
                     PROPERTIES DEPENDS apTvSeSegmentation${filter}${mode}
                                WILL_FAIL TRUE
                                RESOURCE_LOCK ${OUTFILE})


# Test of LSMS workflow

OTB_TEST_APPLICATION(NAME     apTvLSMS1MeanShiftSmoothingNoModeSearch
                     APP      MeanShiftSmoothing
                     OPTIONS  -in ${EXAMPLEDATA}/QB_1_ortho.tif
                              -fout ${TEMP}/apTvLSMS1_filtered_range.tif
                              -foutpos ${TEMP}/apTvLSMS1_filtered_spatial.tif
                              -ranger 30
                              -spatialr  5
                              -maxiter 10
                              -modesearch 0
                     VALID    --compare-n-images ${EPSILON_7} 2
                              ${BASELINE}/apTvLSMS1_filtered_range.tif
                              ${TEMP}/apTvLSMS1_filtered_range.tif
                              ${BASELINE}/apTvLSMS1_filtered_spatial.tif
                              ${TEMP}/apTvLSMS1_filtered_spatial.tif
                     )

OTB_TEST_APPLICATION(NAME     apTvLSMS2Segmentation
                     APP      LSMSSegmentation
                     OPTIONS  -in ${TEMP}/apTvLSMS1_filtered_range.tif
                              -inpos ${TEMP}/apTvLSMS1_filtered_spatial.tif
                              -out ${TEMP}/apTvLSMS2_Segmentation.tif uint32
                              -ranger 30
                              -spatialr  5
                              -minsize 0
                              -tilesizex 100 
                              -tilesizey 100
                     VALID    --compare-image ${NOTOL}
                              ${BASELINE}/apTvLSMS2_Segmentation.tif
                              ${TEMP}/apTvLSMS2_Segmentation.tif
                     )
SET_TESTS_PROPERTIES(apTvLSMS2Segmentation PROPERTIES DEPENDS apTvLSMS1MeanShiftSmoothingNoModeSearch)

OTB_TEST_APPLICATION(NAME     apTvLSMS2Segmentation_NoSmall
                     APP      LSMSSegmentation
                     OPTIONS  -in ${TEMP}/apTvLSMS1_filtered_range.tif
                              -inpos ${TEMP}/apTvLSMS1_filtered_spatial.tif
                              -out ${TEMP}/apTvLSMS2_Segmentation_NoSmall.tif uint32
                              -ranger 30
                              -spatialr  5
                              -minsize 10
                              -tilesizex 100 
                              -tilesizey 100
                     VALID    --compare-image ${NOTOL}
                              ${BASELINE}/apTvLSMS2_Segmentation_NoSmall.tif
                              ${TEMP}/apTvLSMS2_Segmentation_NoSmall.tif
                     )
SET_TESTS_PROPERTIES(apTvLSMS2Segmentation_NoSmall PROPERTIES DEPENDS apTvLSMS1MeanShiftSmoothingNoModeSearch)

OTB_TEST_APPLICATION(NAME     apTvLSMS3SmallRegionsMerging
                     APP      LSMSSmallRegionsMerging
                     OPTIONS  -in ${TEMP}/apTvLSMS1_filtered_range.tif
                              -inseg  ${TEMP}/apTvLSMS2_Segmentation.tif
                              -out ${TEMP}/apTvLSMS3_Segmentation_SmallMerged.tif uint32
                              -minsize 10
                              -tilesizex 100 
                              -tilesizey 100
                     VALID    --compare-image ${NOTOL}
                              ${BASELINE}/apTvLSMS3_Segmentation_SmallMerged.tif
                              ${TEMP}/apTvLSMS3_Segmentation_SmallMerged.tif
                     )
SET_TESTS_PROPERTIES(apTvLSMS3SmallRegionsMerging PROPERTIES DEPENDS apTvLSMS2Segmentation)

OTB_TEST_APPLICATION(NAME     apTvLSMS4Vectorization_SmallMerged
                     APP      LSMSVectorization
                     OPTIONS  -in ${EXAMPLEDATA}/QB_1_ortho.tif
                              -inseg  ${TEMP}/apTvLSMS3_Segmentation_SmallMerged.tif
                              -out ${TEMP}/apTvLSMS4_Segmentation_SmallMerged.shp
                              -tilesizex 100 
                              -tilesizey 100
                     VALID    --compare-ogr ${NOTOL}
                              ${BASELINE_FILES}/apTvLSMS4_Segmentation_SmallMerged.shp
                              ${TEMP}/apTvLSMS4_Segmentation_SmallMerged.shp
                     )
SET_TESTS_PROPERTIES(apTvLSMS4Vectorization_SmallMerged PROPERTIES DEPENDS apTvLSMS3SmallRegionsMerging)

OTB_TEST_APPLICATION(NAME     apTvLSMS4Vectorization_NoSmall
                     APP      LSMSVectorization
                     OPTIONS  -in ${EXAMPLEDATA}/QB_1_ortho.tif
                              -inseg  ${TEMP}/apTvLSMS2_Segmentation_NoSmall.tif
                              -out ${TEMP}/apTvLSMS4_Segmentation_NoSmall.shp
                              -tilesizex 100 
                              -tilesizey 100
                     VALID    --compare-ogr ${NOTOL}
                              ${BASELINE_FILES}/apTvLSMS4_Segmentation_NoSmall.shp
                              ${TEMP}/apTvLSMS4_Segmentation_NoSmall.shp
                     )
SET_TESTS_PROPERTIES(apTvLSMS4Vectorization_NoSmall PROPERTIES DEPENDS apTvLSMS2Segmentation_NoSmall)
