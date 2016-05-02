otb_module_test()

set(OTBSamplingTests
otbSamplingTestDriver.cxx
otbOGRDataToSamplePositionFilterTest.cxx
otbSamplingRateCalculatorTest.cxx
otbOGRDataToClassStatisticsFilterTest.cxx
)

add_executable(otbSamplingTestDriver ${OTBSamplingTests})
target_link_libraries(otbSamplingTestDriver ${OTBSampling-Test_LIBRARIES})
otb_module_target_label(otbSamplingTestDriver)

# Tests Declaration
# ---------------- SamplingRateCalculator ------------------------------------
otb_add_test(NAME leTuSamplingRateCalculatorNew COMMAND otbSamplingTestDriver
            otbSamplingRateCalculatorNew 
  
  )

otb_add_test(NAME leTvSamplingRateCalculator COMMAND otbSamplingTestDriver
  --compare-ascii ${NOTOL}
            ${BASELINE_FILES}/leTuSamplingRateCalculator.txt
            ${TEMP}/leTuSamplingRateCalculator.txt
            otbSamplingRateCalculator 
            ${INPUTDATA}/leTvOGRDataToResampledOGRDataXMLPoly.xml
            83
            ${TEMP}/leTuSamplingRateCalculator.txt)
  
# ----------------- OGRDataToSamplePositionFilter ----------------------------
otb_add_test(NAME leTuOGRDataToSamplePositionFilterNew COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilterNew )

otb_add_test(NAME leTvOGRDataToSamplePositionFilterPoly COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilter
  ${INPUTDATA}/variousVectors.sqlite
  0
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToSamplePositionFilterPolyPattern COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilterPattern
  ${INPUTDATA}/variousVectors.sqlite
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_XMLPoly.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Poly.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToSamplePositionFilterLines COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilter
  ${INPUTDATA}/variousVectors.sqlite
  1
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_Lines.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Lines.sqlite
  )
  
otb_add_test(NAME leTvOGRDataToSamplePositionFilterPoints COMMAND otbSamplingTestDriver
  otbOGRDataToSamplePositionFilter
  ${INPUTDATA}/variousVectors.sqlite
  2
  ${TEMP}/leTvOGRDataToResampledOGRDataOutput_Points.sqlite
  ${BASELINE_FILES}/leTvOGRDataToResampledOGRDataOutput_Points.sqlite
  )

# --------------- OGRDataToClassStatisticsFilter -----------------------------
otb_add_test(NAME leTuOGRDataToClassStatisticsFilterNew COMMAND otbSamplingTestDriver
  otbOGRDataToClassStatisticsFilterNew )

otb_add_test(NAME leTvOGRDataToClassStatisticsFilter COMMAND otbSamplingTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvOGRDataToClassStatisticsFilterOutput.txt
  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt
  otbOGRDataToClassStatisticsFilter
  ${INPUTDATA}/variousVectors.sqlite
  ${TEMP}/leTvOGRDataToClassStatisticsFilterOutput.txt)

