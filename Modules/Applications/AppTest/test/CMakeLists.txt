otb_module_test()

set(OTBAppTestTests
otbWrapperApplicationDocTests.cxx
otbAppTestTestDriver.cxx
)

add_executable(otbAppTestTestDriver ${OTBAppTestTests})
target_link_libraries(otbAppTestTestDriver ${OTBAppTest-Test_LIBRARIES})
otb_module_target_label(otbAppTestTestDriver)


otb_add_test(NAME apCheckDocumentation COMMAND otbAppTestTestDriver
  otbWrapperApplicationDocTest ${OTB_BINARY_DIR}/lib/otb/applications)

otb_test_application(NAME apTvTestApplicationOutputXML_All
  APP TestApplication
  OPTIONS -boolean 1
          -int 2
          -float -0.7
          -string foo
          -filename ${INPUTDATA}/letter.scale
          -outfilename ${TEMP}/foo.txt
          -directory ${INPUTDATA}/world_boundaries
          -choice.choice1.floatchoice1 6.4e5
          -ingroup.integer 4
          -outgroup.outputimage ${TEMP}/foo.tif double
          -il ${INPUTDATA}/anaglyphInput1.tif ${INPUTDATA}/anaglyphInput2.tif
          -cl Choice2
          -cin ${INPUTDATA}/RADARSAT2_ALTONA_300_300_VV.tif
          -cout ${TEMP}/fooCplx.tif
          -outxml ${TEMP}/apTvTestApplicationOutputXML_All.xml
  VALID --compare-ascii ${NOTOL}
    ${OTBAPP_BASELINE_FILES}/apTvTestApplicationOutputXML_All.xml
    ${TEMP}/apTvTestApplicationOutputXML_All.xml
    --ignore-lines-with 7 letter.scale foo.txt world_boundaries foo.tif anaglyphInput1.tif anaglyphInput2.tif RADARSAT2_ALTONA_300_300_VV.tif
  )
