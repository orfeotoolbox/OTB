
#--- EstimateImageStatistics ---#

add_test(NAME apTvClEstimateImagesStatisticsQB1
         COMMAND otbTestDriver
                --compare-ascii ${NOTOL} 
                  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
                  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 EstimateImagesStatistics
                 --modulePath $<TARGET_FILE_DIR:otbapp_EstimateImagesStatistics>
                 --il ${INPUTDATA}/Classification/QB_1_ortho.tif
                 --out ${TEMP}/apTvClEstimateImageStatisticsQB1.xml )

add_test(NAME apTvClEstimateImagesStatisticsQB123
         COMMAND otbTestDriver
                --compare-ascii ${NOTOL} 
                  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
                  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 EstimateImagesStatistics
                 --modulePath $<TARGET_FILE_DIR:otbapp_EstimateImagesStatistics>
                 --il ${INPUTDATA}/Classification/QB_1_ortho.tif
                      ${INPUTDATA}/Classification/QB_2_ortho.tif
                      ${INPUTDATA}/Classification/QB_3_ortho.tif
                 --out ${TEMP}/apTvClEstimateImageStatisticsQB123.xml )

add_test(NAME apTvClEstimateImagesStatisticsQB456
         COMMAND otbTestDriver
                --compare-ascii ${NOTOL} 
                  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
                  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 EstimateImagesStatistics
                 --modulePath $<TARGET_FILE_DIR:otbapp_EstimateImagesStatistics>
                 --il ${INPUTDATA}/Classification/QB_4_extract.tif 
                      ${INPUTDATA}/Classification/QB_5_extract.tif 
                      ${INPUTDATA}/Classification/QB_6_extract.tif
                 --out ${TEMP}/apTvClEstimateImageStatisticsQB456.xml )


ADD_TEST(NAME apTvClImageSVMClassifierQB1  
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB1.tif
    ${TEMP}/clLabeledImageQB1.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_1_ortho.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
   --out ${TEMP}/clLabeledImageQB1.tif
)


ADD_TEST(NAME apTvClImageSVMClassifierQB2   
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB2.tif
    ${TEMP}/clLabeledImageQB2.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_2_ortho.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
   --out ${TEMP}/clLabeledImageQB2.tif
)

ADD_TEST(NAME apTvClImageSVMClassifierQB3  
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB3.tif
    ${TEMP}/clLabeledImageQB3.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_3_ortho.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
   --out ${TEMP}/clLabeledImageQB3.tif
)

ADD_TEST(NAME apTvClImageSVMClassifierQB123_1  
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
    ${TEMP}/clLabeledImageQB123_1.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_1_ortho.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB123.svm
   --out ${TEMP}/clLabeledImageQB123_1.tif
)


ADD_TEST(NAME apTvClImageSVMClassifierQB123_2 
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB123_2.tif
    ${TEMP}/clLabeledImageQB123_2.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_2_ortho.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB123.svm
   --out ${TEMP}/clLabeledImageQB123_2.tif
)


ADD_TEST(NAME apTvClImageSVMClassifierQB123_3
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB123_3.tif
    ${TEMP}/clLabeledImageQB123_3.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_3_ortho.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB123.svm
   --out ${TEMP}/clLabeledImageQB123_3.tif
)


ADD_TEST(NAME apTvClImageSVMClassifierQB123_6
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB123_6.tif
    ${TEMP}/clLabeledImageQB123_6.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_6_extract.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB123.svm
   --out ${TEMP}/clLabeledImageQB123_6.tif
)



ADD_TEST(NAME apTvClImageSVMClassifierQB456_4
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB456_4.tif
    ${TEMP}/clLabeledImageQB456_4.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_4_extract.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB456.svm
   --out ${TEMP}/clLabeledImageQB456_4.tif
)


ADD_TEST(NAME apTvClImageSVMClassifierQB456_5
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB456_5.tif
    ${TEMP}/clLabeledImageQB456_5.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_5_extract.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB456.svm
   --out ${TEMP}/clLabeledImageQB456_5.tif
)


ADD_TEST(NAME apTvClImageSVMClassifierQB456_6
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB456_6.tif
    ${TEMP}/clLabeledImageQB456_6.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_6_extract.tif
   --mask ${INPUTDATA}/Classification/QB_6_mask.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB456.svm
   --out ${TEMP}/clLabeledImageQB456_6.tif
)


ADD_TEST(NAME apTvClImageSVMClassifierQB456_1
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
    ${TEMP}/clLabeledImageQB456_1.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   --modulePath $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in  ${INPUTDATA}/Classification/QB_1_ortho.tif
   --imstat  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
   --svm ${OTBAPP_BASELINE_FILES}/clsvmModelQB456.svm
   --out ${TEMP}/clLabeledImageQB456_1.tif
)

add_test(NAME apTvClKMeansImageClassification
         COMMAND otbTestDriver
                --compare-image ${NOTOL} 
                  ${OTBAPP_BASELINE}/apTvClKMeansImageClassificationFilterOuptut.tif
                  ${TEMP}/apTvClKMeansImageClassificationFilterOuptut.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 KMeansClassification
                 --modulePath $<TARGET_FILE_DIR:otbapp_KMeansClassification>
                 --in ${INPUTDATA}/poupees_sub.png
                 --vm ${INPUTDATA}/mask_KMeans.png 
                 --ts 100 
                 --tp 0.6 
                 --nc 5 
                 --cp 0.9 
                 --sl 100
                 --out ${TEMP}/apTvClKMeansImageClassificationFilterOuptut.tif )






IF( NOT OTB-Applications_DISABLE_CXX_TESTING )

SET(INPUTDATA ${OTB_DATA_ROOT}/Input)
SET(TEMP ${OTB-ApplicationsTesting_BINARY_DIR}/Temporary)
SET(BASELINE_FILES ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Files)
SET(BASELINE_IMAGES ${OTB_DATA_ROOT}/Baseline/OTB-Applications/Images)

SET(NOTOL 0.0)


#--------
# QB multi chain from extract and with complex class aggregation
ADD_TEST(clTvEstimateImagesStatisticsQB456   ${OTB_TEST_DRIVER} 
  --compare-ascii ${NOTOL} 
    ${BASELINE_FILES}/clImageStatisticsQB456.xml
    ${TEMP}/clImageStatisticsQB456.xml
  Execute otbEstimateImagesStatistics-cli
    -in ${INPUTDATA}/Classification/QB_4_extract.tif 
        ${INPUTDATA}/Classification/QB_5_extract.tif 
        ${INPUTDATA}/Classification/QB_6_extract.tif
    -out ${TEMP}/clImageStatisticsQB456.xml
)

ADD_TEST(clTvTrainImagesClassifierQB456   ${OTB_TEST_DRIVER} 
  --compare-ascii ${NOTOL} 
    ${BASELINE_FILES}/clsvmModelQB456.svm
    ${TEMP}/clsvmModelQB456.svm
  Execute otbTrainImagesClassifier-cli
    -in ${INPUTDATA}/Classification/QB_4_extract.tif
        ${INPUTDATA}/Classification/QB_5_extract.tif
        ${INPUTDATA}/Classification/QB_6_extract.tif
    -vd ${INPUTDATA}/Classification/VectorData_QB4.shp
        ${INPUTDATA}/Classification/VectorData_QB5.shp
        ${INPUTDATA}/Classification/VectorData_QB6.shp
    -is ${TEMP}/clImageStatisticsQB456.xml
    -b 2
    -opt
    -out ${TEMP}/clsvmModelQB456.svm
)
SET_TESTS_PROPERTIES(clTvTrainImagesClassifierQB456 PROPERTIES DEPENDS clTvEstimateImagesStatisticsQB456)

ADD_TEST(clTvValidateImagesClassifierQB456   ${OTB_TEST_DRIVER} 
  --compare-ascii ${NOTOL}
    ${BASELINE_FILES}/clsvmPerformanceEstimationQB456.txt
    ${TEMP}/clsvmPerformanceEstimationQB456.txt
  Execute otbValidateImagesClassifier-cli
    -in ${INPUTDATA}/Classification/QB_5_extract.tif
    -vd ${INPUTDATA}/Classification/VectorData_QB5_bis.shp
    -is ${TEMP}/clImageStatisticsQB456.xml
    -svm ${TEMP}/clsvmModelQB456.svm
    -out ${TEMP}/clsvmPerformanceEstimationQB456.txt
)
SET_TESTS_PROPERTIES(clTvValidateImagesClassifierQB456 PROPERTIES DEPENDS clTvTrainImagesClassifierQB456)

ADD_TEST(clTvImageSVMClassifierQB456_4   ${OTB_TEST_DRIVER} 
  --compare-image ${NOTOL} 
    ${BASELINE_IMAGES}/clLabeledImageQB456_4.tif
    ${TEMP}/clLabeledImageQB456_4.tif
  Execute otbImageSVMClassifier-cli
    -in  ${INPUTDATA}/Classification/QB_4_extract.tif
    -is  ${TEMP}/clImageStatisticsQB456.xml
    -svm ${TEMP}/clsvmModelQB456.svm
    -out ${TEMP}/clLabeledImageQB456_4.tif
)
SET_TESTS_PROPERTIES(clTvImageSVMClassifierQB456_4  PROPERTIES DEPENDS clTvTrainImagesClassifierQB456)

ADD_TEST(clTvImageSVMClassifierQB456_5   ${OTB_TEST_DRIVER} 
  --compare-image ${NOTOL} 
    ${BASELINE_IMAGES}/clLabeledImageQB456_5.tif
    ${TEMP}/clLabeledImageQB456_5.tif
  Execute otbImageSVMClassifier-cli
    -in  ${INPUTDATA}/Classification/QB_5_extract.tif
    -is  ${TEMP}/clImageStatisticsQB456.xml
    -svm ${TEMP}/clsvmModelQB456.svm
    -out ${TEMP}/clLabeledImageQB456_5.tif
)
SET_TESTS_PROPERTIES(clTvImageSVMClassifierQB456_5  PROPERTIES DEPENDS clTvTrainImagesClassifierQB456)

ADD_TEST(clTvImageSVMClassifierQB456_6   ${OTB_TEST_DRIVER} 
  --compare-image ${NOTOL} 
    ${BASELINE_IMAGES}/clLabeledImageQB456_6.tif
    ${TEMP}/clLabeledImageQB456_6.tif
  Execute otbImageSVMClassifier-cli
    -in  ${INPUTDATA}/Classification/QB_6_extract.tif
    -inm ${INPUTDATA}/Classification/QB_6_mask.tif
    -is  ${TEMP}/clImageStatisticsQB456.xml
    -svm ${TEMP}/clsvmModelQB456.svm
    -out ${TEMP}/clLabeledImageQB456_6.tif
)
SET_TESTS_PROPERTIES(clTvImageSVMClassifierQB456_6  PROPERTIES DEPENDS clTvTrainImagesClassifierQB456)

ADD_TEST(clTvImageSVMClassifierQB456_1   ${OTB_TEST_DRIVER} 
  --compare-image ${NOTOL} 
    ${BASELINE_IMAGES}/clLabeledImageQB456_1.tif
    ${TEMP}/clLabeledImageQB456_1.tif
  Execute otbImageSVMClassifier-cli
    -in  ${INPUTDATA}/Classification/QB_1_ortho.tif
    -is  ${TEMP}/clImageStatisticsQB456.xml
    -svm ${TEMP}/clsvmModelQB456.svm
    -out ${TEMP}/clLabeledImageQB456_1.tif
)
SET_TESTS_PROPERTIES(clTvImageSVMClassifierQB456_1  PROPERTIES DEPENDS clTvTrainImagesClassifierQB456)

#IF(OTB_DATA_USE_LARGEINPUT ) 
# Functional but use only locally      
# ADD_TEST(clTvImageSVMClassifierQB456_all   ${OTB_TEST_DRIVER} 
#    Execute otbImageSVMClassifier-cli
#      -in  ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
#      -is  ${TEMP}/clImageStatisticsQB456.xml
#      -svm ${TEMP}/clsvmModelQB456.svm
#      -out ${TEMP}/labeledImageQB456_all.tif    
#)
#SET_TESTS_PROPERTIES(clTvImageSVMClassifierQB456_all  PROPERTIES DEPENDS clTvTrainImagesClassifierQB456)
#
# Test 
# ADD_TEST(clTvImageSVMClassifier_M_all   ${OTB_TEST_DRIVER} 
#    Execute otbImageSVMClassifier-cli
#      -in  ${OTB_DATA_LARGEINPUT_ROOT}/QUICKBIRD/TOULOUSE/000000128955_01_P001_MUL/02APR01105228-M1BS-000000128955_01_P001.TIF
#      -is  ${INPUTDATA}/Classification/ImageStatisticsM.xml
#      -svm ${INPUTDATA}/Classification/svmModelM.svm
#      -out ${TEMP}/labeledImageQB_M_all.tif    
#)
#
#ENDIF(OTB_DATA_USE_LARGEINPUT)

ENDIF( NOT OTB-Applications_DISABLE_CXX_TESTING )

                 