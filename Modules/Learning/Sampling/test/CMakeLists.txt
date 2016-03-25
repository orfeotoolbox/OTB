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
  
  
#otb_add_test(NAME leTuSamplingRateCalculatorUI COMMAND otbSamplingTestDriver
            #otbSamplingRateCalculatorUI 
            #110
            #${TEMP}/leTuSamplingRateCalculatorUI.txt
   
   #)
   
   
#otb_add_test(NAME leTuSamplingRateCalculatorS COMMAND otbSamplingTestDriver
            #otbSamplingRateCalculatorS 
            #"110"
            #${TEMP}/leTuSamplingRateCalculatorS.txt
   
   #)   
   
   
#otb_add_test(NAME leTuSamplingRateCalculatorSmallest COMMAND otbSamplingTestDriver
            #otbSamplingRateCalculatorSmallest 
            #${TEMP}/leTuSamplingRateCalculatorSmallest.txt
   
   #)   


#otb_add_test(NAME leTuSamplingRateCalculatorSmallest2 COMMAND otbSamplingTestDriver
            #otbSamplingRateCalculatorSmallest2 
            #${TEMP}/leTuSamplingRateCalculatorSmallest.txt
   
   #) 


otb_add_test(NAME leTuOGRDataToResampledOGRDataNew COMMAND otbSamplingTestDriver
  otbOGRDataToResampledOGRDataNew )

otb_add_test(NAME leTvOGRDataToResampledOGRDataPoly COMMAND otbSamplingTestDriver
  otbOGRDataToResampledOGRData
  ${INPUTDATA}/variousVectors.sqlite
  ${INPUTDATA}/leTvOGRDataToResampledOGRDataResamplingVectors_Poly.txt
  0
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToResampledOGRDataXMLPoly COMMAND otbSamplingTestDriver
  otbOGRDataToResampledOGRDataXML
  ${INPUTDATA}/variousVectors.sqlite
  ${INPUTDATA}/leTvOGRDataToResampledOGRDataResamplingVectors_Poly.txt
  0
  ${INPUTDATA}/leTvOGRDataToResampledOGRDataXMLPoly.xml
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToResampledOGRDataLines COMMAND otbSamplingTestDriver
  otbOGRDataToResampledOGRData
  ${INPUTDATA}/variousVectors.sqlite
  ${INPUTDATA}/leTvOGRDataToResampledOGRDataResamplingVectors_Lines.txt
  1
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_Lines.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Lines.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToResampledOGRDataPoints COMMAND otbSamplingTestDriver
  otbOGRDataToResampledOGRData
  ${INPUTDATA}/variousVectors.sqlite
  ${INPUTDATA}/leTvOGRDataToResampledOGRDataResamplingVectors_Points.txt
  2
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_Points.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Points.sqlite
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

