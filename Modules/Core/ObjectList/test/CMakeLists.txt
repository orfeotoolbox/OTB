otb_module_test()
set(OTBObjectListTests
  otbObjectList.cxx
  otbObjectListNew.cxx
  otbImageList.cxx
  otbImageListNew.cxx
  otbImageListSourceNew.cxx
  otbImageListToImageFilterNew.cxx
  otbImageListToImageListApplyFilter.cxx
  otbImageListToImageListApplyFilterNew.cxx
  otbImageListToImageListFilterNew.cxx
  otbImageListToVectorImageFilter2.cxx
  otbImageListToVectorImageFilter.cxx
  otbImageListToVectorImageFilterNew.cxx
  otbObjectList2.cxx
  otbObjectListToObjectListFilterNew.cxx
  otbImageToImageListFilterNew.cxx
  otbVectorImageToImageListFilter.cxx
  otbVectorImageToImageListFilterNew.cxx
  otbObjectListTestDriver.cxx  )

add_executable(otbObjectListTestDriver ${OTBObjectListTests})
target_link_libraries(otbObjectListTestDriver ${OTBObjectList-Test_LIBRARIES})
otb_module_target_label(otbObjectListTestDriver)
otb_add_test(NAME coTvObjectList COMMAND otbObjectListTestDriver
  otbObjectList
  ${INPUTDATA}/couleurs.jpg
  ${INPUTDATA}/poupees.png
  ${INPUTDATA}/couleurs_extrait.png
  )
otb_add_test(NAME coTuObjectListNew COMMAND otbObjectListTestDriver
  otbObjectListNew)
otb_add_test(NAME coTvImageList COMMAND otbObjectListTestDriver
  --compare-image ${NOTOL}   ${INPUTDATA}/amst.png
  ${TEMP}/amst.png
  otbImageList
  ${INPUTDATA}/amst.png
  ${TEMP}/amst.png
  )
otb_add_test(NAME coTuImageListNew COMMAND otbObjectListTestDriver
  otbImageListNew)
otb_add_test(NAME coTuImageListSourceNew COMMAND otbObjectListTestDriver
  otbImageListSourceNew)
otb_add_test(NAME coTuImageListToImageFilterNew COMMAND otbObjectListTestDriver
  otbImageListToImageFilterNew)
otb_add_test(NAME bfTvImageListToImageListApplyFilter COMMAND otbObjectListTestDriver
  --compare-n-images ${EPSILON_7} 3
  ${BASELINE}/bfTvImageListToImageListApplyFilterBand1.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand1.png
  ${BASELINE}/bfTvImageListToImageListApplyFilterBand2.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand2.png
  ${BASELINE}/bfTvImageListToImageListApplyFilterBand3.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand3.png
  otbImageListToImageListApplyFilter
  ${INPUTDATA}/poupees_c1.hdr
  ${INPUTDATA}/poupees_c2.hdr
  ${INPUTDATA}/poupees_c3.hdr
  ${TEMP}/bfTvImageListToImageListApplyFilterBand1.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand2.png
  ${TEMP}/bfTvImageListToImageListApplyFilterBand3.png
  )
otb_add_test(NAME bfTuImageListToImageListApplyFilterNew COMMAND otbObjectListTestDriver
  otbImageListToImageListApplyFilterNew)
otb_add_test(NAME coTvImageListToImageListFilterNew COMMAND otbObjectListTestDriver
  otbImageListToImageListFilterNew)
otb_add_test(NAME coTvImageListToVectorImageFilter2 COMMAND otbObjectListTestDriver
  otbImageListToVectorImageFilter2
  ${INPUTDATA}/poupees_c1.hdr
  ${INPUTDATA}/poupees_c2.hdr
  ${INPUTDATA}/poupees_c3.hdr
  )
otb_add_test(NAME coTvImageListToVectorImageFilter COMMAND otbObjectListTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvImageListToVectorImageFilter.png
  ${TEMP}/coTvImageListToVectorImageFilter.png
  otbImageListToVectorImageFilter
  ${INPUTDATA}/poupees_c1.hdr
  ${INPUTDATA}/poupees_c2.hdr
  ${INPUTDATA}/poupees_c3.hdr
  ${TEMP}/coTvImageListToVectorImageFilter.png
  )
otb_add_test(NAME coTuImageListToVectorImageFilterNew COMMAND otbObjectListTestDriver
  otbImageListToVectorImageFilterNew)
otb_add_test(NAME coTvObjectListTestNotValid COMMAND otbObjectListTestDriver
  otbObjectList2
  )
otb_add_test(NAME coTuObjectListToObjectListFilterNew COMMAND otbObjectListTestDriver
  otbObjectListToObjectListFilterNew)
otb_add_test(NAME coTuImageToImageListFilterNew COMMAND otbObjectListTestDriver
  otbImageToImageListFilterNew)
otb_add_test(NAME bfTvVectorImageToImageListFilter COMMAND otbObjectListTestDriver
  --compare-n-images ${EPSILON_7} 3
  ${BASELINE}/bfTvVectorImageToImageListFilterBand1.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand1.png
  ${BASELINE}/bfTvVectorImageToImageListFilterBand2.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand2.png
  ${BASELINE}/bfTvVectorImageToImageListFilterBand3.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand3.png
  otbVectorImageToImageListFilter
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand1.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand2.png
  ${TEMP}/bfTvVectorImageToImageListFilterBand3.png
  )
otb_add_test(NAME bfTvVectorImageToImageListFilterIt COMMAND otbObjectListTestDriver
  --compare-n-images ${EPSILON_7} 3
  ${BASELINE}/bfTvVectorImageToImageListFilterBand1.png
  ${TEMP}/bfTvVectorImageToImageListFilterItBand1.png
  ${BASELINE}/bfTvVectorImageToImageListFilterBand2.png
  ${TEMP}/bfTvVectorImageToImageListFilterItBand2.png
  ${BASELINE}/bfTvVectorImageToImageListFilterBand3.png
  ${TEMP}/bfTvVectorImageToImageListFilterItBand3.png
  otbVectorImageToImageListFilterIterator
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt
  png
  )
otb_add_test(NAME bfTvVectorImageToImageListFilterIt2 COMMAND otbObjectListTestDriver
  --compare-n-images ${EPSILON_7} 3
  ${BASELINE}/bfTvVectorImageToImageListFilterBand1.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt2Band1.tif
  ${BASELINE}/bfTvVectorImageToImageListFilterBand2.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt2Band2.tif
  ${BASELINE}/bfTvVectorImageToImageListFilterBand3.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt2Band3.tif
  otbVectorImageToImageListFilterIterator
  ${INPUTDATA}/poupees.png
  ${TEMP}/bfTvVectorImageToImageListFilterIt2
  tif
  )
otb_add_test(NAME bfTuVectorImageToImageListFilterNew COMMAND otbObjectListTestDriver
  otbVectorImageToImageListFilterNew)
