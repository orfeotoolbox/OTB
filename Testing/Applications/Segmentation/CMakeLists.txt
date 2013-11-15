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
set(cc_parameters "-filter.cc.expr distance<25")
set(edison_parameters "-filter.edison.spatialr 5 -filter.edison.ranger 50 -filter.edison.minsize 10")
set(meanshift_parameters "-filter.meanshift.spatialr 5 -filter.meanshift.ranger 50 -filter.meanshift.minsize 10")
set(vector_parameters "-mode.vector.tilesize 500 -mode.vector.minsize 50 -mode.vector.simplify 0.1 -mode.vector.outmode ovw")

# validation option
set(meanshift_option "--without-threads")

# Output type dependning on mode
set(raster_output_format ".tif")
set(raster_output_option " uint16")
set(vector_output_format ".sqlite")

# Validation depending on mode
set(vector_comparison --compare-ogr ${NOTOL})
set(raster_comparison --compare-image ${NOTOL})

# Reference ffiles depending on modes
set(vector_ref_path ${BASELINE_FILES})
set(raster_ref_path ${BASELINE})

# Loop on modes
foreach(mode "Raster" "Vector")
string(TOLOWER ${mode} lmode)

# Loop on filters
foreach(filter "Edison" "Meanshift" "CC" "Watershed")
string(TOLOWER ${filter} lfilter)

# Derive output file name
set(OUTFILE apTvSeSegmentation${filter}${mode}${${lmode}_output_format})

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

endforeach()
endforeach()

set(filter "CC")
set(mode "Vector")

string(TOLOWER ${filter} lfilter)

# Add a test for ULOVW
set(vector_parameters "-mode.vector.tilesize 500 -mode.vector.minsize 50 -mode.vector.simplify 0.1 -mode.vector.outmode ulovw")

set(OUTFILE apTvSeSegmentation${filter}${mode}${${lmode}_output_format})

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

set_tests_properties(apTvSeSegmentation${filter}${mode}_ULOVW
                     PROPERTIES DEPENDS apTvSeSegmentation${filter}${mode}
                                RESOURCE_LOCK ${OUTFILE})

# Add a test for ULU
set(vector_parameters "-mode.vector.tilesize 500 -mode.vector.minsize 50 -mode.vector.simplify 0.1 -mode.vector.outmode ulu")

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

set_tests_properties(apTvSeSegmentation${filter}${mode}_ULU
                     PROPERTIES DEPENDS apTvSeSegmentation${filter}${mode}
                                RESOURCE_LOCK ${OUTFILE})

# Add a test for ULCO
set(vector_parameters "-mode.vector.tilesize 500 -mode.vector.minsize 50 -mode.vector.simplify 0.1 -mode.vector.outmode ulco")

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

set_tests_properties(apTvSeSegmentation${filter}${mode}_ULCO
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
set_tests_properties(apTvLSMS2Segmentation PROPERTIES DEPENDS apTvLSMS1MeanShiftSmoothingNoModeSearch)

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
set_tests_properties(apTvLSMS2Segmentation_NoSmall PROPERTIES DEPENDS apTvLSMS1MeanShiftSmoothingNoModeSearch)

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
set_tests_properties(apTvLSMS3SmallRegionsMerging PROPERTIES DEPENDS apTvLSMS2Segmentation)

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
set_tests_properties(apTvLSMS4Vectorization_SmallMerged PROPERTIES DEPENDS apTvLSMS3SmallRegionsMerging)

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
set_tests_properties(apTvLSMS4Vectorization_NoSmall PROPERTIES DEPENDS apTvLSMS2Segmentation_NoSmall)
