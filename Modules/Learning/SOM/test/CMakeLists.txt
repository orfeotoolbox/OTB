otb_module_test()

set(OTBSOMTests
otbSOMTestDriver.cxx
otbSOMbasedImageFilterNew.cxx
otbSOM.cxx
otbSOMImageClassificationFilter.cxx
otbSOMActivationBuilder.cxx
otbSOMActivationBuilderNew.cxx
otbSOMClassifierNew.cxx
otbSOMImageClassificationFilterNew.cxx
otbSOMWithMissingValue.cxx
otbSOMNew.cxx
otbSOMMap.cxx
otbSOMWithMissingValueNew.cxx
otbSOMMapNew.cxx
otbPeriodicSOM.cxx
otbPeriodicSOMNew.cxx
otbSOMClassifier.cxx
otbSOMbasedImageFilter.cxx
)

add_executable(otbSOMTestDriver ${OTBSOMTests})
target_link_libraries(otbSOMTestDriver ${OTBSOM-Test_LIBRARIES})
otb_module_target_label(otbSOMTestDriver)

# Tests Declaration

otb_add_test(NAME leTuSOMbasedImageFilterNew COMMAND otbSOMTestDriver
  otbSOMbasedImageFilterNew )

otb_add_test(NAME leTvSOM COMMAND otbSOMTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMPoupeesSubOutputMap1.hdr
  otbSOM
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/leSOMPoupeesSubOutputMap1.hdr
  32 32 10 10 5 1.0 0.1 0)

otb_add_test(NAME leTvSOMImageClassificationFilter COMMAND otbSOMTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSOMPoupeesClassified.hdr
  ${TEMP}/leSOMImageClassificationFilterOuptut.hdr
  otbSOMImageClassificationFilter
  ${INPUTDATA}/poupees_sub.png
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMImageClassificationFilterOuptut.hdr
  )

otb_add_test(NAME leTvSOMActivationBuilder COMMAND otbSOMTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSOMPoupeesSubActivationMap1.hdr
  ${TEMP}/leSOMPoupeesSubActivationMap1.hdr
  otbSOMActivationBuilder
  ${INPUTDATA}/poupees_sub.png
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMPoupeesSubActivationMap1.hdr
  32 32 10 10 5 1.0 0.1 128)

otb_add_test(NAME leTuSOMActivationBuilderNew COMMAND otbSOMTestDriver
  otbSOMActivationBuilderNew)

otb_add_test(NAME leTuSOMClassifierNew COMMAND otbSOMTestDriver
  otbSOMClassifierNew)

otb_add_test(NAME leTuSOMImageClassificationFilterNew COMMAND otbSOMTestDriver
  otbSOMImageClassificationFilterNew)

otb_add_test(NAME leTvSOMWithMissingValue COMMAND otbSOMTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/leSOMWithMissingValue.hdr
  ${TEMP}/leSOMWithMissingValue.hdr
  otbSOMWithMissingValueTest
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/leSOMWithMissingValue.hdr
  32 32 10 10 5 1.0 0.1 0)

otb_add_test(NAME leTuSOMNew COMMAND otbSOMTestDriver
  otbSOMNew)

otb_add_test(NAME leTvSOMMap COMMAND otbSOMTestDriver
  otbSOMMap)

otb_add_test(NAME leTuSOMWithMissingValueNew COMMAND otbSOMTestDriver
  otbSOMWithMissingValueNew )

otb_add_test(NAME leTuSOMMapNew COMMAND otbSOMTestDriver
  otbSOMMapNew)

otb_add_test(NAME leTvPeriodicSOM COMMAND otbSOMTestDriver
  --compare-image ${EPSILON_10}
  ${BASELINE}/lePeriodicSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
  otbPeriodicSOMTest
  ${INPUTDATA}/poupees_sub.png
  ${TEMP}/lePeriodicSOMPoupeesSubOutputMap1.hdr
  32 32 10 10 5 1.0 0.1 0)

otb_add_test(NAME leTuPeriodicSOMNew COMMAND otbSOMTestDriver
  otbPeriodicSOMNew)

otb_add_test(NAME leTvSOMClassifier COMMAND otbSOMTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSOMPoupeesClassified.hdr
  ${TEMP}/leSOMPoupeesClassified.hdr
  otbSOMClassifier
  ${INPUTDATA}/poupees_sub.png
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMPoupeesClassified.hdr
  )

otb_add_test(NAME leTvSOMbasedImageFilter COMMAND otbSOMTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/leSOMbasedImageFilterOuptut.hdr
  ${TEMP}/leSOMbasedImageFilterOuptut.hdr
  otbSOMbasedImageFilterTest
  ${INPUTDATA}/poupees_sub.png
  ${BASELINE}/leSOMPoupeesSubOutputMap1.hdr
  ${TEMP}/leSOMbasedImageFilterOuptut.hdr
  )

