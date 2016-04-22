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

otb_add_test(NAME leTuSamplingRateCalculator COMMAND otbSamplingTestDriver
  --compare-ascii ${NOTOL}
            ${BASELINE_FILES}/leTuSamplingRateCalculator.txt
            ${TEMP}/leTuSamplingRateCalculator.txt
            otbSamplingRateCalculator 
            ${INPUTDATA}/leTvOGRDataToResampledOGRDataXMLPoly.xml
            83
            ${TEMP}/leTuSamplingRateCalculator.txt)
  

otb_add_test(NAME leTuOGRDataToSamplePositionFilterNew COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilterNew )

otb_add_test(NAME leTvOGRDataToSamplePositionFilterPoly COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilter
  ${INPUTDATA}/variousVectors.sqlite
  ${INPUTDATA}/leTvOGRDataToResampledOGRDataResamplingVectors_Poly.txt
  0
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToSamplePositionFilterXMLPoly COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilterXML
  ${INPUTDATA}/variousVectors.sqlite
  ${INPUTDATA}/leTvOGRDataToResampledOGRDataResamplingVectors_Poly.txt
  0
  ${INPUTDATA}/leTvOGRDataToResampledOGRDataXMLPoly.xml
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_XMLPoly.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToSamplePositionFilterLines COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilter
  ${INPUTDATA}/variousVectors.sqlite
  ${INPUTDATA}/leTvOGRDataToResampledOGRDataResamplingVectors_Lines.txt
  1
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_Lines.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Lines.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToSamplePositionFilterPoints COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilter
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

