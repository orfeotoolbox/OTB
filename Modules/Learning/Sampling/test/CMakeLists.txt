otb_module_test()

set(OTBSamplingTests
otbSamplingTestDriver.cxx
otbOGRDataToResampledOGRDataTest.cxx
otbSamplingRateCalculatorTest.cxx
otbOGRDataToClassStatisticsFilterTest.cxx
)

add_executable(otbSamplingTestDriver ${OTBSamplingTests})
target_link_libraries(otbSamplingTestDriver ${OTBSampling-Test_LIBRARIES})
otb_module_target_label(otbSamplingTestDriver)

# Tests Declaration

otb_add_test(NAME leTuSamplingRateCalculatorNew COMMAND otbSamplingTestDriver
            otbSamplingRateCalculatorNew 
  
  )
  
  
otb_add_test(NAME leTuSamplingRateCalculatorUI COMMAND otbSamplingTestDriver
            otbSamplingRateCalculatorUI 
            110
            ${TEMP}/leTuSamplingRateCalculatorUI.txt
   
   )
   
   
otb_add_test(NAME leTuSamplingRateCalculatorS COMMAND otbSamplingTestDriver
            otbSamplingRateCalculatorS 
            "110"
            ${TEMP}/leTuSamplingRateCalculatorS.txt
   
   )   
   
   
otb_add_test(NAME leTuSamplingRateCalculatorSmallest COMMAND otbSamplingTestDriver
            otbSamplingRateCalculatorSmallest 
            ${TEMP}/leTuSamplingRateCalculatorSmallest.txt
   
   )   


otb_add_test(NAME leTuSamplingRateCalculatorSmallest2 COMMAND otbSamplingTestDriver
            otbSamplingRateCalculatorSmallest2 
            ${TEMP}/leTuSamplingRateCalculatorSmallest.txt
   
   ) 


otb_add_test(NAME leTuOGRDataToResampledOGRDataNew COMMAND otbSamplingTestDriver
  otbOGRDataToResampledOGRDataNew )

otb_add_test(NAME leTvOGRDataToResampledOGRData COMMAND otbSamplingTestDriver
  otbOGRDataToResampledOGRData
  ${INPUTDATA}/variousVectors.sqlite
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput.sqlite
  0
  ${TEMP}/leTvOGRDataToResampledOGRDataResamplingVectors.txt
  )


otb_add_test(NAME leTuOGRDataToClassStatisticsFilterNew COMMAND otbSamplingTestDriver
  otbOGRDataToClassStatisticsFilterNew )

otb_add_test(NAME leTvOGRDataToClassStatisticsFilter COMMAND otbSamplingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvOGRDataToClassStatisticsFilterOutput.txt
  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt
  otbOGRDataToClassStatisticsFilter
  ${INPUTDATA}/variousVectors.sqlite
  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt)

