
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

                 