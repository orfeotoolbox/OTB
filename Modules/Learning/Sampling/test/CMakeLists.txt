otb_module_test()

set(OTBSamplingTests
otbSamplingTestDriver.cxx
#otbOGRDataToClassStatisticsFilterTest.cxx
otbSamplingRateCalculatorTest.cxx
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




#otb_add_test(NAME leTvOGRDataToClassStatisticsFilter COMMAND otbSamplingTestDriver
#  --compare-ascii ${NOTOL}
#  ${BASELINE_FILES}/leTvOGRDataToClassStatisticsFilterOutput.txt
#  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt
#  otbOGRDataToClassStatisticsFilter
#  ${INPUTDATA}/variousVectors.sqlite
#  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt)
