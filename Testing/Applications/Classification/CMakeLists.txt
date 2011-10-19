
#--- EstimateImageStatistics ---#
# QB1 chain

add_test(NAME apTvClEstimateImagesStatisticsQB1
         COMMAND otbTestDriver
                --compare-ascii ${NOTOL} 
                  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
                  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 EstimateImagesStatistics
                 $<TARGET_FILE_DIR:otbapp_EstimateImagesStatistics>
                 --il ${INPUTDATA}/Classification/QB_1_ortho.tif
                 --out ${TEMP}/apTvClEstimateImageStatisticsQB1.xml )

ADD_TEST(NAME apTvClTrainSVMImagesClassifierQB1   
  COMMAND otbTestDriver
  --compare-ascii ${NOTOL} 
    ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
    ${TEMP}/clsvmModelQB1.svm
     Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
     TrainSVMImagesClassifier
      $<TARGET_FILE_DIR:otbapp_TrainSVMImagesClassifier>
    --il ${INPUTDATA}/Classification/QB_1_ortho.tif
    --vd ${INPUTDATA}/Classification/VectorData_QB1.shp
    --imstat ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
    --b 2
    --opt true
    --out ${TEMP}/clsvmModelQB1.svm
)
SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB1 PROPERTIES DEPENDS apTvClEstimateImagesStatisticsQB1)

ADD_TEST(NAME apTvClTrainSVMImagesClassifierQB1_allOpt   
   COMMAND otbTestDriver
  --compare-ascii ${NOTOL} 
    ${OTBAPP_BASELINE_FILES}/clsvmModelQB1.svm
    ${TEMP}/clsvmModelQB1.svm
     Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
     TrainSVMImagesClassifier
     $<TARGET_FILE_DIR:otbapp_TrainSVMImagesClassifier>
    --il ${INPUTDATA}/Classification/QB_1_ortho.tif
    --vd ${INPUTDATA}/Classification/VectorData_QB1.shp
    --imstat ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
    --b 2
    --mv 100
    --mt 100
    --vtr 0.5
    --opt true
    --out ${TEMP}/clsvmModelQB1_allOpt.svm.svm
)


SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB1_allOpt PROPERTIES DEPENDS apTvClEstimateImagesStatisticsQB1)



ADD_TEST(NAME apTvClImageSVMClassifierQB1  
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB1.tif
    ${TEMP}/clLabeledImageQB1.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_1_ortho.tif
   --imstat  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
   --svm     ${TEMP}/clsvmModelQB1.svm
   --out     ${TEMP}/clLabeledImageQB1.tif
)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)

ADD_TEST(NAME apTvClImageSVMClassifierQB2   
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB2.tif
    ${TEMP}/clLabeledImageQB2.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in     ${INPUTDATA}/Classification/QB_2_ortho.tif
   --imstat ${TEMP}/apTvClEstimateImageStatisticsQB1.xml 
   --svm    ${TEMP}/clsvmModelQB1.svm
   --out    ${TEMP}/clLabeledImageQB2.tif
)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB2 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)


ADD_TEST(NAME apTvClImageSVMClassifierQB3  
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB3.tif
    ${TEMP}/clLabeledImageQB3.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_3_ortho.tif
   --imstat  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
   --svm     ${TEMP}/clsvmModelQB1.svm
   --out     ${TEMP}/clLabeledImageQB3.tif
)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB3 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB1)


#--------
# QB multi chain
add_test(NAME apTvClEstimateImagesStatisticsQB123
         COMMAND otbTestDriver
                --compare-ascii ${NOTOL} 
                  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
                  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 EstimateImagesStatistics
                 $<TARGET_FILE_DIR:otbapp_EstimateImagesStatistics>
                 --il ${INPUTDATA}/Classification/QB_1_ortho.tif
                      ${INPUTDATA}/Classification/QB_2_ortho.tif
                      ${INPUTDATA}/Classification/QB_3_ortho.tif
                 --out ${TEMP}/apTvClEstimateImageStatisticsQB123.xml )


ADD_TEST(NAME apTvClTrainSVMImagesClassifierQB123   
  COMMAND otbTestDriver
  --compare-ascii ${NOTOL} 
    ${OTBAPP_BASELINE_FILES}/clsvmModelQB123.svm
    ${TEMP}/clsvmModelQB123.svm
     Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
     TrainSVMImagesClassifier
      $<TARGET_FILE_DIR:otbapp_TrainSVMImagesClassifier>
    --il ${INPUTDATA}/Classification/QB_1_ortho.tif
         ${INPUTDATA}/Classification/QB_2_ortho.tif
         ${INPUTDATA}/Classification/QB_3_ortho.tif
    --vd ${INPUTDATA}/Classification/VectorData_QB1.shp
         ${INPUTDATA}/Classification/VectorData_QB2.shp
         ${INPUTDATA}/Classification/VectorData_QB3.shp
    --imstat ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
    --b 2
    --opt true
    --out ${TEMP}/clsvmModelQB123.svm
)

SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB123 PROPERTIES DEPENDS apTvClEstimateImagesStatisticsQB123)


ADD_TEST(NAME apTvClImageSVMClassifierQB123_1  
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB123_1.tif
    ${TEMP}/clLabeledImageQB123_1.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_1_ortho.tif
   --imstat  ${TEMP}/clImageStatisticsQB123.xml
   --svm     ${TEMP}/clsvmModelQB123.svm
   --out     ${TEMP}/clLabeledImageQB123_1.tif
)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)


ADD_TEST(NAME apTvClImageSVMClassifierQB123_2 
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB123_2.tif
    ${TEMP}/clLabeledImageQB123_2.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_2_ortho.tif
   --imstat  ${TEMP}/clImageStatisticsQB123.xml
   --svm     ${TEMP}/clsvmModelQB123.svm
   --out     ${TEMP}/clLabeledImageQB123_2.tif
)


SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_2 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)


ADD_TEST(NAME apTvClImageSVMClassifierQB123_3
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB123_3.tif
    ${TEMP}/clLabeledImageQB123_3.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_3_ortho.tif
   --imstat  ${TEMP}/clImageStatisticsQB123.xml
   --svm     ${TEMP}/clsvmModelQB123.svm
   --out     ${TEMP}/clLabeledImageQB123_3.tif
)


SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_3 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)


ADD_TEST(NAME apTvClImageSVMClassifierQB123_6
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB123_6.tif
    ${TEMP}/clLabeledImageQB123_6.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_6_extract.tif
   --imstat  ${TEMP}/clImageStatisticsQB123.xml
   --svm     ${TEMP}/clsvmModelQB123.svm
   --out     ${TEMP}/clLabeledImageQB123_6.tif
)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB123_6 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB123)

add_test(NAME apTvClEstimateImagesStatisticsQB456
         COMMAND otbTestDriver
                --compare-ascii ${NOTOL} 
                  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB456.xml
                  ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 EstimateImagesStatistics
                 $<TARGET_FILE_DIR:otbapp_EstimateImagesStatistics>
                 --il ${INPUTDATA}/Classification/QB_4_extract.tif 
                      ${INPUTDATA}/Classification/QB_5_extract.tif 
                      ${INPUTDATA}/Classification/QB_6_extract.tif
                 --out ${TEMP}/apTvClEstimateImageStatisticsQB456.xml )



ADD_TEST(NAME apTvClTrainSVMImagesClassifierQB456  
  COMMAND otbTestDriver
  --compare-ascii ${NOTOL} 
    ${OTBAPP_BASELINE_FILES}/clsvmModelQB456.svm
    ${TEMP}/clsvmModelQB456.svm
     Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
     TrainSVMImagesClassifier
      $<TARGET_FILE_DIR:otbapp_TrainSVMImagesClassifier>
    --il ${INPUTDATA}/Classification/QB_4_extract.tif
         ${INPUTDATA}/Classification/QB_5_extract.tif
         ${INPUTDATA}/Classification/QB_6_extract.tif
    --vd ${INPUTDATA}/Classification/VectorData_QB4.shp
         ${INPUTDATA}/Classification/VectorData_QB5.shp
         ${INPUTDATA}/Classification/VectorData_QB6.shp
    --imstat ${TEMP}/apTvClEstimateImageStatisticsQB456.xml
    --b 2
    --opt true
    --out ${TEMP}/clsvmModelQB456.svm
)

SET_TESTS_PROPERTIES(apTvClTrainSVMImagesClassifierQB456 PROPERTIES DEPENDS apTvClEstimateImagesStatisticsQB456)



ADD_TEST(NAME apTvClImageSVMClassifierQB456_4
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB456_4.tif
    ${TEMP}/clLabeledImageQB456_4.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_4_extract.tif
   --imstat  ${TEMP}/clImageStatisticsQB456.xml
   --svm     ${TEMP}/clsvmModelQB456.svm
   --out     ${TEMP}/clLabeledImageQB456_4.tif
)

SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_4 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)

ADD_TEST(NAME apTvClImageSVMClassifierQB456_5
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB456_5.tif
    ${TEMP}/clLabeledImageQB456_5.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_5_extract.tif
   --imstat  ${TEMP}/clImageStatisticsQB456.xml
   --svm     ${TEMP}/clsvmModelQB456.svm
   --out     ${TEMP}/clLabeledImageQB456_5.tif
)


SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_5 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)

ADD_TEST(NAME apTvClImageSVMClassifierQB456_6
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB456_6.tif
    ${TEMP}/clLabeledImageQB456_6.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_6_extract.tif
   --mask    ${INPUTDATA}/Classification/QB_6_mask.tif
   --imstat  ${TEMP}/clImageStatisticsQB456.xml
   --svm     ${TEMP}/clsvmModelQB456.svm
   --out     ${TEMP}/clLabeledImageQB456_6.tif
)


SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_6 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)


ADD_TEST(NAME apTvClImageSVMClassifierQB456_1
 COMMAND otbTestDriver
  --compare-image ${NOTOL} 
    ${OTBAPP_BASELINE}/clLabeledImageQB456_1.tif
    ${TEMP}/clLabeledImageQB456_1.tif
   Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
   ImageSVMClassifier
   $<TARGET_FILE_DIR:otbapp_ImageSVMClassifier>
   --in      ${INPUTDATA}/Classification/QB_1_ortho.tif
   --imstat  ${TEMP}/clImageStatisticsQB456.xml
   --svm     ${TEMP}/clsvmModelQB456.svm
   --out     ${TEMP}/clLabeledImageQB456_1.tif
)


SET_TESTS_PROPERTIES(apTvClImageSVMClassifierQB456_1 PROPERTIES DEPENDS apTvClTrainSVMImagesClassifierQB456)


add_test(NAME apTvClKMeansImageClassification
         COMMAND otbTestDriver
                --compare-image ${NOTOL} 
                  ${OTBAPP_BASELINE}/apTvClKMeansImageClassificationFilterOuptut.tif
                  ${TEMP}/apTvClKMeansImageClassificationFilterOuptut.tif
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 KMeansClassification
                 $<TARGET_FILE_DIR:otbapp_KMeansClassification>
                 --in ${INPUTDATA}/poupees_sub.png
                 --vm ${INPUTDATA}/mask_KMeans.png 
                 --ts 100 
                 --tp 0.6 
                 --nc 5 
                 --cp 0.9 
                 --sl 100
                 --out ${TEMP}/apTvClKMeansImageClassificationFilterOuptut.tif )





