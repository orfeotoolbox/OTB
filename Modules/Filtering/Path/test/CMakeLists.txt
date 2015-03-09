otb_module_test()

set(OTBPathTests
otbPathTestDriver.cxx
otbCompacityPathRectangle.cxx
otbVectorizationPathListFilterNew.cxx
otbRegionImageToRectangularPathListFilterNew.cxx
otbImageToEdgePathFilter.cxx
otbCompacityPathCircle.cxx
otbDrawPathListFilterWithValue.cxx
otbImageFittingPolygonListFilterNew.cxx
otbDrawPath.cxx
otbCompacityPathSquare.cxx
otbPathLengthFunctor.cxx
otbPathListToHistogramGenerator.cxx
otbImageFittingPolygonListFilter.cxx
otbImageToPathFilterNew.cxx
otbImageToEdgePathFilterNew.cxx
otbDrawPathListFilter.cxx
otbDrawPathListFilterNew.cxx
otbDrawPathFilter.cxx
otbOrientationPath.cxx
otbOrientationPathNew.cxx
otbDrawPathFilterNew.cxx
otbPolyLineImageConstIterator.cxx
otbRegionImageToRectangularPathListFilter.cxx
otbVectorizationPathListFilter.cxx
otbPathListToPathListFilterNew.cxx
otbPathListToHistogramGeneratorNew.cxx
otbCompacityPathNew.cxx
otbClosePathFunctor.cxx
otbPolyLineImageIterator.cxx
)

add_executable(otbPathTestDriver ${OTBPathTests})
target_link_libraries(otbPathTestDriver ${OTBPath-Test_LIBRARIES})
otb_module_target_label(otbPathTestDriver)

# Tests Declaration

otb_add_test(NAME feTuCompacityPathRectangle COMMAND otbPathTestDriver
  otbCompacityPathRectangle 10.0  20.0)

otb_add_test(NAME feTuVectorizationPathListFilterNew COMMAND otbPathTestDriver
  otbVectorizationPathListFilterNew)

otb_add_test(NAME feTuRegionImageToRectangularPathListFilterNew COMMAND otbPathTestDriver
  otbRegionImageToRectangularPathListFilterNew)

otb_add_test(NAME feTvImageToEdgePathFilterBis COMMAND otbPathTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feImageToEdgePathFilter_maskrom.png
  ${TEMP}/feImageToEdgePathFilter_maskrom.png
  otbImageToEdgePathFilter
  ${INPUTDATA}/maskrom.png
  ${TEMP}/feImageToEdgePathFilter_maskrom.png
  0
  )

otb_add_test(NAME feTvImageToEdgePathFilter COMMAND otbPathTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feImageToEdgePathFilter_rcc8_mire4.png
  ${TEMP}/feImageToEdgePathFilter_rcc8_mire4.png
  otbImageToEdgePathFilter
  ${INPUTDATA}/rcc8_mire4.png
  ${TEMP}/feImageToEdgePathFilter_rcc8_mire4.png
  255
  )

otb_add_test(NAME feTuCompacityPathCircle COMMAND otbPathTestDriver
  otbCompacityPathCircle 1000)

otb_add_test(NAME coTvDrawPathListFilterWithValue COMMAND otbPathTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvDrawPathListFilterWithValueOutput.png
  ${TEMP}/coTvDrawPathListFilterWithValueOutput.png
  otbDrawPathListFilterWithValue
  512 512 20
  ${TEMP}/coTvDrawPathListFilterWithValueOutput.png
  )

otb_add_test(NAME feTuImageFittingPolygonListFilterNew COMMAND otbPathTestDriver
  otbImageFittingPolygonListFilterNew)

otb_add_test(NAME feTvDrawPathTestCarre COMMAND otbPathTestDriver
  --compare-image ${NOTOL}  ${BASELINE}/feDrawPathDessinCarre.png
  ${TEMP}/feDrawPathDessinCarre.png
  otbDrawPathDessinCarre
  ${INPUTDATA}/DeuxTraits.png
  ${TEMP}/feDrawPathDessinCarre.png)

otb_add_test(NAME feTuCompacityPathSquare COMMAND otbPathTestDriver
  otbCompacityPathSquare 10.0)

otb_add_test(NAME bfTvPathLengthFunctor COMMAND otbPathTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfPathLengthFunctorTest.txt
  ${TEMP}/bfPathLengthFunctorTest.txt
  otbPathLengthFunctor
  ${INPUTDATA}/polygon.png
  ${TEMP}/bfPathLengthFunctorTest.txt
  )

otb_add_test(NAME coTuPathListToHistogramGenerator COMMAND otbPathTestDriver
  otbPathListToHistogramGenerator 100 2)

otb_add_test(NAME feTvImageFittingPolygonListFilter COMMAND otbPathTestDriver
  --compare-ogr ${EPSILON_3}
  ${BASELINE_FILES}/feTvImageFittingPolygonListFilter_Output.shp
  ${TEMP}/feTvImageFittingPolygonListFilter_Output.shp
  otbImageFittingPolygonListFilter
  ${INPUTDATA}/polygon.png
  ${INPUTDATA}/polygon-start.shp
  ${TEMP}/feTvImageFittingPolygonListFilter_Output.shp
  5 10
  )

otb_add_test(NAME coTuImageToPathFilterNew COMMAND otbPathTestDriver
  otbImageToPathFilterNew)

otb_add_test(NAME feTuImageToEdgePathFilterNew COMMAND otbPathTestDriver
  otbImageToEdgePathFilterNew)

otb_add_test(NAME coTvDrawPathListFilter COMMAND otbPathTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvDrawPathListFilterOutput.png
  ${TEMP}/coTvDrawPathListFilterOutput.png
  otbDrawPathListFilter
  512 512 20
  ${TEMP}/coTvDrawPathListFilterOutput.png
  )

otb_add_test(NAME coTuDrawPathListFilterNew COMMAND otbPathTestDriver
  otbDrawPathListFilterNew)

otb_add_test(NAME coTvDrawPatFilter COMMAND otbPathTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvDrawPathFilterOutput.png
  ${TEMP}/coTvDrawPathFilterOutput.png
  otbDrawPathFilter
  512 512 20
  ${TEMP}/coTvDrawPathFilterOutput.png
  )


otb_add_test(NAME feTuOrientationPath_090 COMMAND otbPathTestDriver
  otbOrientationPath 90.0)

otb_add_test(NAME feTuOrientationPath_045 COMMAND otbPathTestDriver
  otbOrientationPath 45.0)

otb_add_test(NAME feTuOrientationPath_000 COMMAND otbPathTestDriver
  otbOrientationPath 0.0)

otb_add_test(NAME feTuOrientationPath1_80 COMMAND otbPathTestDriver
  otbOrientationPath 180.0)

otb_add_test(NAME feTuOrientationPathNew COMMAND otbPathTestDriver
  otbOrientationPathNew)

otb_add_test(NAME coTuDrawPatFilterNew COMMAND otbPathTestDriver
  otbDrawPathFilterNew
  )

otb_add_test(NAME coTvPolyLineImageConstIterator COMMAND otbPathTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/coTvPolyLineImageConstIterator.txt
  ${TEMP}/coTvPolyLineImageConstIterator.txt
  otbPolyLineImageConstIterator
  512 512 20
  ${TEMP}/coTvPolyLineImageConstIterator.txt
  )

otb_add_test(NAME feTvRegionImageToRectangularPathListFilter COMMAND otbPathTestDriver
  --compare-ascii ${EPSILON_3}     ${BASELINE_FILES}/feRectangle.txt
  ${TEMP}/feRectangle.txt
  otbRegionImageToRectangularPathListFilter
  ${INPUTDATA}/rectangle.png
  ${TEMP}/feRectangle.txt
  0.9 #fit score
  10)  #minimum size

otb_add_test(NAME feTvVectorizationPathListFilterImageOutput COMMAND otbPathTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvVectorizationPathListOutput.png
  ${TEMP}/feTvVectorizationPathListOutput.png
  otbVectorizationPathListFilter
  ${INPUTDATA}/InputForRoadDetection_NonMaxRem.hdr
  ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
  ${TEMP}/feTvVectorizationPathListOutputIgnored.txt
  ${TEMP}/feTvVectorizationPathListOutput.png
  0.0001
  )

otb_add_test(NAME feTvVectorizationPathListFilterAsciiOutput COMMAND otbPathTestDriver
  --compare-ascii ${EPSILON_3}
  ${BASELINE_FILES}/feTvVectorizationPathListOutput.txt
  ${TEMP}/feTvVectorizationPathListOutput.txt
  otbVectorizationPathListFilter
  ${INPUTDATA}/InputForRoadDetection_NonMaxRem.hdr
  ${INPUTDATA}/InputForRoadDetectionScalarProductDir.hdr
  ${TEMP}/feTvVectorizationPathListOutput.txt
  ${TEMP}/feTvVectorizationPathListOutputIgnored.png
  0.0005
  )

otb_add_test(NAME coTuPathListToPathListFilterNew COMMAND otbPathTestDriver
  otbPathListToPathListFilterNew)

otb_add_test(NAME coTuPathListToHistogramGeneratorNew COMMAND otbPathTestDriver
  otbPathListToHistogramGeneratorNew)


otb_add_test(NAME feTuCompacityPathNew COMMAND otbPathTestDriver
  otbCompacityPathNew)

otb_add_test(NAME bfTvClosePathFunctor COMMAND otbPathTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfClosePathFunctorTest.txt
  ${TEMP}/bfClosePathFunctorTest.txt
  otbClosePathFunctor
  ${INPUTDATA}/polygon.png
  ${TEMP}/bfClosePathFunctorTest.txt
  )

otb_add_test(NAME coTvPolyLineImageIterator COMMAND otbPathTestDriver
  --compare-image ${NOTOL}
  ${BASELINE}/coTvPolyLineImageIterator.png
  ${TEMP}/coTvPolyLineImageIterator.png
  otbPolyLineImageIterator
  512 512 20
  ${TEMP}/coTvPolyLineImageIterator.png
  )

