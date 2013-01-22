
#--- EstimateImageStatistics ---#
# QB1 chain

OTB_TEST_APPLICATION(NAME apTvClComputeImagesStatisticsQB1
                     APP  ComputeImagesStatistics
                     OPTIONS -il ${INPUTDATA}/Classification/QB_1_ortho.tif
                			 -out ${TEMP}/apTvClEstimateImageStatisticsQB1.xml 
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
                             ${TEMP}/apTvClEstimateImageStatisticsQB1.xml )
                                                     
                        
OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB1   
                     APP  TrainSVMImagesClassifier
                     OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
  							 -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
  							 -io.imstat ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
  						     ##-sample.b 2
					         -svm.opt true
    						 -io.out ${TEMP}/clsvmModelQB1.svm
    						 -rand 121212
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
   					 		 ${TEMP}/clsvmModelQB1.svm)
       
SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB1 PROPERTIES DEPENDS apTvClComputeImagesStatisticsQB1)

OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB1_allOpt      
                     APP  TrainSVMImagesClassifier
                     OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
  						     -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
 							 -io.imstat ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
  						     ##-sample.b 2
  						     -sample.mv 100
   							 -sample.mt 100
    					     -sample.vtr 0.5
    						 -svm.opt true
    						 -rand 121212
    					     -io.out ${TEMP}/clsvmModelQB1_allOpt.svm
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
   					 		 ${TEMP}/clsvmModelQB1_allOpt.svm)
  
SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB1_allOpt PROPERTIES DEPENDS apTvClComputeImagesStatisticsQB1)


OTB_TEST_APPLICATION(NAME apTvClValidateSVMImagesClassifierQB1     
                     APP  ValidateSVMImagesClassifier
                     OPTIONS -il     ${INPUTDATA}/Classification/QB_1_ortho.tif
                             -vd     ${INPUTDATA}/Classification/VectorData_QB1_bis.shp
     						 -imstat ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
   							 -svm    ${TEMP}/clsvmModelQB1.svm
   							 -out    ${TEMP}/clsvmPerformanceEstimationQB1.txt
   							 -rand   121212
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clsvmPerformanceEstimationQB1.txt
    						 ${TEMP}/clsvmPerformanceEstimationQB1.txt)
 
SET_TESTS_PROPERTIES(apTvClValidateSVMImagesClassifierQB1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)


OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB1      
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_1_ortho.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
   							 -svm     ${TEMP}/clsvmModelQB1.svm
  							 -out     ${TEMP}/clLabeledImageQB1.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB1.tif
                             ${TEMP}/clLabeledImageQB1.tif)
 
SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)

OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB2      
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_2_ortho.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
   							 -svm     ${TEMP}/clsvmModelQB1.svm
  							 -out     ${TEMP}/clLabeledImageQB2.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB2.tif
                             ${TEMP}/clLabeledImageQB2.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB2 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)

OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB3   
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_3_ortho.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
   							 -svm     ${TEMP}/clsvmModelQB1.svm
  							 -out     ${TEMP}/clLabeledImageQB3.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB3.tif
                             ${TEMP}/clLabeledImageQB3.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB3 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)

#--------
# QB multi chain

OTB_TEST_APPLICATION(NAME apTvClComputeImagesStatisticsQB123
                     APP  ComputeImagesStatistics
                     OPTIONS -il ${INPUTDATA}/Classification/QB_1_ortho.tif
                    			  ${INPUTDATA}/Classification/QB_2_ortho.tif
                   			      ${INPUTDATA}/Classification/QB_3_ortho.tif
                			 -out ${TEMP}/apTvClEstimateImageStatisticsQB123.xml 
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
                             ${TEMP}/apTvClEstimateImageStatisticsQB123.xml)
   

OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB123   
                     APP  TrainSVMImagesClassifier
                     OPTIONS -io.il ${INPUTDATA}/Classification/QB_1_ortho.tif
                    			  ${INPUTDATA}/Classification/QB_2_ortho.tif
                   			      ${INPUTDATA}/Classification/QB_3_ortho.tif              	
							 -io.vd ${INPUTDATA}/Classification/VectorData_QB1.shp
        						  ${INPUTDATA}/Classification/VectorData_QB2.shp
         						  ${INPUTDATA}/Classification/VectorData_QB3.shp
    						 -io.imstat ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
    						 #-sample.b 2
   							 -svm.opt true
    						 -io.out ${TEMP}/clsvmModelQB123.svm
    						 -rand 121212
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clsvmModelQB123.svm
   					 		 ${TEMP}/clsvmModelQB123.svm)
     

SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB123 PROPERTIES DEPENDS apTvClComputeImagesStatisticsQB123)


OTB_TEST_APPLICATION(NAME apTvClValidateSVMImagesClassifierQB123     
                     APP  ValidateSVMImagesClassifier
                     OPTIONS -il     ${INPUTDATA}/Classification/QB_1_ortho.tif
                             -vd     ${INPUTDATA}/Classification/VectorData_QB1_bis.shp
     						 -imstat ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
   							 -svm    ${TEMP}/clsvmModelQB123.svm
   							 -out    ${TEMP}/clsvmPerformanceEstimationQB123.txt
   							 -rand   121212
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clsvmPerformanceEstimationQB123.txt
    						 ${TEMP}/clsvmPerformanceEstimationQB123.txt)
 

SET_TESTS_PROPERTIES(apTvClValidateSVMImagesClassifierQB123 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)

OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB123_1  
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_1_ortho.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
   							 -svm     ${TEMP}/clsvmModelQB123.svm
  							 -out     ${TEMP}/clLabeledImageQB123_1.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                             ${TEMP}/clLabeledImageQB123_1.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)


OTB_TEST_APPLICATION(NAME apTvClLabeledImageColorMappingQB123_1   
                     APP   ColorMapping
                     OPTIONS -in      ${TEMP}/clLabeledImageQB123_1.tif
                             -method  custom
                             -method.custom.lut ${INPUTDATA}/Classification/ColorTable.txt
                             -out     ${TEMP}/clLabeledFancyImageQB123_1.tif
   		             VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledFancyImageQB123_1.tif
   							 ${TEMP}/clLabeledFancyImageQB123_1.tif)
  

SET_TESTS_PROPERTIES(apTvClLabeledImageColorMappingQB123_1 PROPERTIES DEPENDS apTvClImageSVMClassifierQB123_1)

OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB123_2  
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_2_ortho.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
   							 -svm     ${TEMP}/clsvmModelQB123.svm
  							 -out     ${TEMP}/clLabeledImageQB123_2.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB123_2.tif
                             ${TEMP}/clLabeledImageQB123_2.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_2 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)

OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB123_3  
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_3_ortho.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
   							 -svm     ${TEMP}/clsvmModelQB123.svm
  							 -out     ${TEMP}/clLabeledImageQB123_3.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB123_3.tif
                             ${TEMP}/clLabeledImageQB123_3.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_3 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)

OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB123_6 
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_6_extract.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
   							 -svm     ${TEMP}/clsvmModelQB123.svm
  							 -out     ${TEMP}/clLabeledImageQB123_6.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB123_6.tif
   							 ${TEMP}/clLabeledImageQB123_6.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_6 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)

OTB_TEST_APPLICATION(NAME apTvClComputeImagesStatisticsQB456
                     APP  ComputeImagesStatistics
                     OPTIONS -il ${INPUTDATA}/Classification/QB_4_extract.tif
                    			  ${INPUTDATA}/Classification/QB_5_extract.tif
                   			      ${INPUTDATA}/Classification/QB_6_extract.tif
                			 -out ${TEMP}/apTvClEstimateImageStatisticsQB456.xml 
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
                             ${TEMP}/apTvClEstimateImageStatisticsQB456.xml)
   
OTB_TEST_APPLICATION(NAME apTvClTrainSVMImagesClassifierQB456   
                     APP  TrainSVMImagesClassifier
                     OPTIONS -io.il ${INPUTDATA}/Classification/QB_4_extract.tif
                    			  ${INPUTDATA}/Classification/QB_5_extract.tif
                   			      ${INPUTDATA}/Classification/QB_6_extract.tif              	
							 -io.vd ${INPUTDATA}/Classification/VectorData_QB4.shp
        						  ${INPUTDATA}/Classification/VectorData_QB5.shp
         						  ${INPUTDATA}/Classification/VectorData_QB6.shp
    						 -io.imstat ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
    						 #-sample.b 2
   							 -svm.opt true
    						 -io.out ${TEMP}/clsvmModelQB456.svm
    						 -rand 121212
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clsvmModelQB456.svm
   					 		 ${TEMP}/clsvmModelQB456.svm)
     


SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB456 PROPERTIES DEPENDS apTvClComputeImagesStatisticsQB456)

OTB_TEST_APPLICATION(NAME apTvClValidateSVMImagesClassifierQB456    
                     APP  ValidateSVMImagesClassifier
                     OPTIONS -il     ${INPUTDATA}/Classification/QB_5_extract.tif
                             -vd     ${INPUTDATA}/Classification/VectorData_QB5_bis.shp
     						 -imstat ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
   							 -svm    ${TEMP}/clsvmModelQB456.svm
   							 -rand   121212
   							 -out    ${TEMP}/clsvmPerformanceEstimationQB456.txt
                     VALID   --compare-ascii ${NOTOL}
                             ${OTBAPP_BASELINE_FILES}/clsvmPerformanceEstimationQB456.txt
    						 ${TEMP}/clsvmPerformanceEstimationQB456.txt)
 

SET_TESTS_PROPERTIES(apTvClValidateSVMImagesClassifierQB456 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)


OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB456_4  
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_4_extract.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
   							 -svm     ${TEMP}/clsvmModelQB456.svm
  							 -out     ${TEMP}/clLabeledImageQB456_4.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB456_4.tif
                             ${TEMP}/clLabeledImageQB456_4.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_4 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)

OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB456_5  
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_5_extract.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
   							 -svm     ${TEMP}/clsvmModelQB456.svm
  							 -out     ${TEMP}/clLabeledImageQB456_5.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB456_5.tif
                             ${TEMP}/clLabeledImageQB456_5.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_5 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)


OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB456_6  
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_6_extract.tif
                             -mask   ${INPUTDATA}/Classification/QB_6_mask.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
   							 -svm     ${TEMP}/clsvmModelQB456.svm
  							 -out     ${TEMP}/clLabeledImageQB456_6.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB456_6.tif
                             ${TEMP}/clLabeledImageQB456_6.tif)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_6 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)


OTB_TEST_APPLICATION(NAME apTvClImageSVMClassifierQB456_1
                     APP  ImageSVMClassifier
                     OPTIONS -in      ${INPUTDATA}/Classification/QB_1_ortho.tif
   							 -imstat  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
   							 -svm     ${TEMP}/clsvmModelQB456.svm
  							 -out     ${TEMP}/clLabeledImageQB456_1.tif   
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
                             ${TEMP}/clLabeledImageQB456_1.tif)


SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)

OTB_TEST_APPLICATION(NAME apTvClKMeansImageClassification
                     APP  KMeansClassification
                     OPTIONS -in ${INPUTDATA}/qb_RoadExtract.img
                 	         -vm ${INPUTDATA}/qb_RoadExtract_mask.png
                 	         -ts 30000 
                             -nc 5  
                             -maxit 10000 
                             -ct 0.0000001
                             -rand 121212
                             -out ${TEMP}/apTvClKMeansImageClassificationFilterOuptut.tif 
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/apTvClKMeansImageClassificationFilterOuptut.tif
                             ${TEMP}/apTvClKMeansImageClassificationFilterOuptut.tif )

# --- SOMClassification ---
OTB_TEST_APPLICATION(NAME   apTvClSOMClassificationSmall
                     APP    SOMClassification
                     OPTIONS -in ${INPUTDATA}/poupees_sub.png
                             -rand 121212
                             -out ${TEMP}/apTvClSOMClassificationSmall.tif uint16
                     VALID   --compare-image ${NOTOL}
                             ${BASELINE}/apTvClSOMClassificationSmall.tif
                             ${TEMP}/apTvClSOMClassificationSmall.tif)

OTB_TEST_APPLICATION(NAME   apTvClSOMClassificationFull
                     APP    SOMClassification
                     OPTIONS -in  ${INPUTDATA}/poupees_sub.png
                             -out ${TEMP}/apTvClSOMClassificationFull.tif uint16
                             -vm  ${INPUTDATA}/poupees_sub_c1.png
                             -tp  0.8
                             -ts  13000
                             -sl  32
                             -som ${TEMP}/apTvClSOMClassificationMap.hdr
                             -sx  30
                             -sy  30
                             -nx  9
                             -ny  9
                             -ni  5
                             -bi  1.0
                             -bf  0.1
                             -iv  0
                             -rand 121212
                     VALID   --compare-n-images ${NOTOL} 2
                             ${BASELINE}/apTvClSOMClassificationFull.tif
                             ${TEMP}/apTvClSOMClassificationFull.tif
                             ${BASELINE}/apTvClSOMClassificationMap.hdr
                             ${TEMP}/apTvClSOMClassificationMap.hdr)

# --- Fusion of classifications ---
OTB_TEST_APPLICATION(NAME   apTvFusionOfClassifications2Inputs
                     APP    FusionOfClassifications
                     OPTIONS -il ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                                 ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
                             -undecided 100
                             -out ${TEMP}/apTvFusionOfClassifications2InputsOutput.tif uint16
                     VALID   --compare-image ${NOTOL}
                     ${OTBAPP_BASELINE}/apTvFusionOfClassifications2InputsOutput.tif
                     ${TEMP}/apTvFusionOfClassifications2InputsOutput.tif
)

OTB_TEST_APPLICATION(NAME   apTvFusionOfClassifications3Inputs
                     APP    FusionOfClassifications
                     OPTIONS -il ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                                 ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
                                 ${OTBAPP_BASELINE}/clLabeledImageQB1.tif
                             -undecided 100
                             -out ${TEMP}/apTvFusionOfClassifications3InputsOutput.tif uint16
                     VALID   --compare-image ${NOTOL}
                     ${OTBAPP_BASELINE}/apTvFusionOfClassifications3InputsOutput.tif
                     ${TEMP}/apTvFusionOfClassifications3InputsOutput.tif
)


# --- Compute confusion matrix ---
OTB_TEST_APPLICATION(NAME   apTvComputeConfusionMatrixV
                     APP    ComputeConfusionMatrix
                     OPTIONS -in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                             -ref vector
                             -ref.vector.in ${INPUTDATA}/Classification/VectorData_QB1_ter.shp
                             -ref.vector.field Class
                             -labels 4
                             -out ${TEMP}/apTvComputeConfusionMatrixTconfusionVOut.txt
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixTconfusionVOut.txt
                     ${TEMP}/apTvComputeConfusionMatrixTconfusionVOut.txt
)

OTB_TEST_APPLICATION(NAME   apTvComputeConfusionMatrixR
                     APP    ComputeConfusionMatrix
                     OPTIONS -in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                             -ref raster
                             -ref.raster.in ${INPUTDATA}/Classification/clLabeledImageQB456_1-tmp.tif
                             -labels 4
                             -nodata 255
                             -out ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.txt
                     VALID   --compare-ascii ${NOTOL}
                     ${OTBAPP_BASELINE_FILES}/apTvComputeConfusionMatrixTconfusionROut.txt
                     ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.txt
)

#OTB_TEST_APPLICATION(NAME   apTvComputeConfusionMatrixI
#                     APP    ComputeConfusionMatrix
#                     OPTIONS -in ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
#                             -ref raster
#                             -ref.raster.in ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
#                             -labels 4
#                             -nodata 255
#                             -out ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.txt
#                     VALID   --compare-ascii ${NOTOL}
#                     ${OTBAPP_BASELINE}/apTvComputeConfusionMatrixTconfusionROut.txt
#                     ${TEMP}/apTvComputeConfusionMatrixTconfusionROut.txt
#)

OTB_TEST_APPLICATION(NAME   apTvClassificationMapRegularization
                     APP    ClassificationMapRegularization
                     OPTIONS -io.in  ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
                             -io.out ${TEMP}/clLabeledImageQB123_1_CMR_r2_nodl_10_undl_7.tif
                             -ip.radius 2
                             -ip.suvbool true
                             -ip.nodatalabel 10
                             -ip.undecidedlabel 7
                     VALID   --compare-image ${NOTOL}
                             ${OTBAPP_BASELINE}/clLabeledImageQB123_1_CMR_r2_nodl_10_undl_7.tif
                             ${TEMP}/clLabeledImageQB123_1_CMR_r2_nodl_10_undl_7.tif
)
