
#--- EstimateImageStatistics ---#

add_test(NAME apTvClEstimateImagesStatisticsQB1
         COMMAND otbTestDriver
                --compare-ascii ${NOTOL} 
                  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB1.xml
                  ${TEMP}/apTvClEstimateImageStatisticsQB1.xml
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 EstimateImagesStatistics
                 --modulePath $<TARGET_FILE_DIR:otbapp_EstimateImagesStatistics>
                 --in ${INPUTDATA}/Classification/QB_1_ortho.tif
                 --out ${TEMP}/apTvClEstimateImageStatisticsQB1.xml )

add_test(NAME apTvClEstimateImagesStatisticsQB123
         COMMAND otbTestDriver
                --compare-ascii ${NOTOL} 
                  ${OTBAPP_BASELINE_FILES}/clImageStatisticsQB123.xml
                  ${TEMP}/apTvClEstimateImageStatisticsQB123.xml
                 Execute $<TARGET_FILE:otbApplicationLauncherCommandLine>
                 EstimateImagesStatistics
                 --modulePath $<TARGET_FILE_DIR:otbapp_EstimateImagesStatistics>
                 --in ${INPUTDATA}/Classification/QB_1_ortho.tif
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
                 --in ${INPUTDATA}/Classification/QB_4_ortho.tif
                      ${INPUTDATA}/Classification/QB_5_ortho.tif
                      ${INPUTDATA}/Classification/QB_6_ortho.tif
                 --out ${TEMP}/apTvClEstimateImageStatisticsQB456.xml )



                 