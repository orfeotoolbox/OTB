otb_module_test()

set(OTBInterpolationTests
otbInterpolationTestDriver.cxx
otbWindowedSincInterpolateImageCosineFunctionNew.cxx
otbWindowedSincInterpolateImageLanczosFunction.cxx
otbWindowedSincInterpolateImageLanczosFunctionNew.cxx
otbWindowedSincInterpolateImageFunctionBaseNew.cxx
otbProlateInterpolateImageFunctionNew.cxx
otbBSplineDecompositionImageFilterNew.cxx
otbBSplineInterpolateImageFunctionNew.cxx
otbWindowedSincInterpolateImageBlackmanFunction.cxx
otbBSplineDecompositionImageFilter.cxx
otbWindowedSincInterpolateImageWelchFunctionNew.cxx
otbWindowedSincInterpolateImageGaussianFunction.cxx
otbGenericInterpolateImageFunctionNew.cxx
otbWindowedSincInterpolateImageBlackmanFunctionNew.cxx
otbWindowedSincInterpolateImageCosineFunction.cxx
otbWindowedSincInterpolateImageHammingFunction.cxx
otbWindowedSincInterpolateImageWelchFunction.cxx
otbBSplineInterpolateImageFunction.cxx
otbWindowedSincInterpolateImageGaussianFunctionNew.cxx
otbWindowedSincInterpolateImageHammingFunctionNew.cxx
otbStreamingTraits.cxx
otbBCOInterpolateImageFunction.cxx
otbProlateInterpolateImageFunction.cxx
otbProlateValidationTest.cxx
)

add_executable(otbInterpolationTestDriver ${OTBInterpolationTests})
target_link_libraries(otbInterpolationTestDriver ${OTBInterpolation-Test_LIBRARIES})
otb_module_target_label(otbInterpolationTestDriver)

# Tests Declaration

otb_add_test(NAME bfTuWindowedSincInterpolateImageCosineFunctionNew COMMAND otbInterpolationTestDriver
  otbWindowedSincInterpolateImageCosineFunctionNew)

otb_add_test(NAME bfTvWindowedSincInterpolateImageLanczosFunction COMMAND otbInterpolationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfWindowedSincInterpolateImageLanczosFunctionOutput.txt
  ${TEMP}/bfWindowedSincInterpolateImageLanczosFunctionOutput.txt
  otbWindowedSincInterpolateImageLanczosFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfWindowedSincInterpolateImageLanczosFunctionOutput.txt
  3 # radius
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )

otb_add_test(NAME bfTuWindowedSincInterpolateImageLanczosFunctionNew COMMAND otbInterpolationTestDriver
  otbWindowedSincInterpolateImageLanczosFunctionNew)

otb_add_test(NAME bfTuWindowedSincInterpolateImageFunctionBaseNew COMMAND otbInterpolationTestDriver
  otbWindowedSincInterpolateImageFunctionBaseNew)

otb_add_test(NAME bfTuProlateInterpolateImageFunctionNew COMMAND otbInterpolationTestDriver
  otbProlateInterpolateImageFunctionNew)

otb_add_test(NAME bfTuBSplineDecompositionImageFilterNew COMMAND otbInterpolationTestDriver
  otbBSplineDecompositionImageFilterNew)

otb_add_test(NAME bfTuBSplineInterpolateImageFunctionNew COMMAND otbInterpolationTestDriver
  otbBSplineInterpolateImageFunctionNew)

otb_add_test(NAME bfTvWindowedSincInterpolateImageBlackmanFunction COMMAND otbInterpolationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfWindowedSincInterpolateImageBlackmanFunctionOutput.txt
  ${TEMP}/bfWindowedSincInterpolateImageBlackmanFunctionOutput.txt
  otbWindowedSincInterpolateImageBlackmanFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfWindowedSincInterpolateImageBlackmanFunctionOutput.txt
  3 # radius
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )

otb_add_test(NAME bfTvBSplineDecompositionImageFilter COMMAND otbInterpolationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfBSplineDecompositionImageFilterOutput.tif
  ${TEMP}/bfBSplineDecompositionImageFilterOutput.tif
  otbBSplineDecompositionImageFilter
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfBSplineDecompositionImageFilterOutput.tif
  )

otb_add_test(NAME bfTuWindowedSincInterpolateImageWelchFunctionNew COMMAND otbInterpolationTestDriver
  otbWindowedSincInterpolateImageWelchFunctionNew)

otb_add_test(NAME bfTvWindowedSincInterpolateImageGaussianFunction COMMAND otbInterpolationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfWindowedSincInterpolateImageGaussianFunctionOutput.txt
  ${TEMP}/bfWindowedSincInterpolateImageGaussianFunctionOutput.txt
  otbWindowedSincInterpolateImageGaussianFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfWindowedSincInterpolateImageGaussianFunctionOutput.txt
  3 # radius
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )

otb_add_test(NAME coTuGenericInterpolateImageFunctionNew COMMAND otbInterpolationTestDriver
  otbGenericInterpolateImageFunctionNew
  )

otb_add_test(NAME bfTuWindowedSincInterpolateImageBlackmanFunctionNew COMMAND otbInterpolationTestDriver
  otbWindowedSincInterpolateImageBlackmanFunctionNew)

otb_add_test(NAME bfTvWindowedSincInterpolateImageCosineFunction COMMAND otbInterpolationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfWindowedSincInterpolateImageCosineFunctionOutput.txt
  ${TEMP}/bfWindowedSincInterpolateImageCosineFunctionOutput.txt
  otbWindowedSincInterpolateImageCosineFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfWindowedSincInterpolateImageCosineFunctionOutput.txt
  3 # radius
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )

otb_add_test(NAME bfTvWindowedSincInterpolateImageHammingFunction COMMAND otbInterpolationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfWindowedSincInterpolateImageHammingFunctionOutput.txt
  ${TEMP}/bfWindowedSincInterpolateImageHammingFunctionOutput.txt
  otbWindowedSincInterpolateImageHammingFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfWindowedSincInterpolateImageHammingFunctionOutput.txt
  3 # radius
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )

otb_add_test(NAME bfTvWindowedSincInterpolateImageWelchFunction COMMAND otbInterpolationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfWindowedSincInterpolateImageWelchFunctionOutput.txt
  ${TEMP}/bfWindowedSincInterpolateImageWelchFunctionOutput.txt
  otbWindowedSincInterpolateImageWelchFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfWindowedSincInterpolateImageWelchFunctionOutput.txt
  3 # radius
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )

otb_add_test(NAME bfTvBSplineInterpolateImageFunction COMMAND otbInterpolationTestDriver
  --compare-ascii ${EPSILON_7}
  ${BASELINE_FILES}/bfBSplineInterpolateImageFunctionOutput.txt
  ${TEMP}/bfBSplineInterpolateImageFunctionOutput.txt
  otbBSplineInterpolateImageFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfBSplineInterpolateImageFunctionOutput.txt
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )

otb_add_test(NAME bfTuWindowedSincInterpolateImageGaussianFunctionNew COMMAND otbInterpolationTestDriver
  otbWindowedSincInterpolateImageGaussianFunctionNew)

otb_add_test(NAME bfTuWindowedSincInterpolateImageHammingFunctionNew COMMAND otbInterpolationTestDriver
  otbWindowedSincInterpolateImageHammingFunctionNew)

otb_add_test(NAME bfTuStreamingTraitsVectorImage COMMAND otbInterpolationTestDriver
  otbStreamingTraitsVectorImage
  )

otb_add_test(NAME bfTuStreamingTraitsImage COMMAND otbInterpolationTestDriver
  otbStreamingTraitsImage
  )

otb_add_test(NAME bfTvBCOInterpolateImageFunctionVectorImageTest COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvBCOInterpolateImageFunctionVectorImageTest.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionVectorImageTest.tif
  otbBCOInterpolateImageFunctionVectorImageTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionVectorImageTest.tif
  4 # radius
  -0.5 # optimised bicubic
  )
otb_add_test(NAME bfTvBCOInterpolateImageFunctionTest COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvBCOInterpolateImageFunctionTest.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionTest.tif
  otbBCOInterpolateImageFunctionTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionTest.tif
  3 # radius
  -0.5 # optimised bicubic
  )
otb_add_test(NAME bfTuBCOInterpolateImageFunctionNew COMMAND otbImageManipulationTestDriver
  otbBCOInterpolateImageFunctionNew
  )
otb_add_test(NAME bfTvBCOInterpolateImageFunctionOverVectorImage COMMAND otbImageManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvBCOInterpolateImageFunctionOverVectorImageOutput.txt
  ${TEMP}/bfTvBCOInterpolateImageFunctionOverVectorImageOutput.txt
  otbBCOInterpolateImageFunctionOverVectorImage
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionOverVectorImageOutput.txt
  3 # radius
  -0.5 # optimised bicubic
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )
otb_add_test(NAME bfTuBCOInterpolateImageFunctionOverVectorImageNew COMMAND otbImageManipulationTestDriver
  otbBCOInterpolateImageFunctionOverVectorImageNew)
otb_add_test(NAME bfTvBCOInterpolateImageFunction2 COMMAND otbImageManipulationTestDriver
  otbBCOInterpolateImageFunction2
  3 # radius
  -0.5 # optimised bicubic
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )
otb_add_test(NAME bfTvBCOInterpolateImageFunction COMMAND otbImageManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfTvBCOInterpolateImageFunctionOutput.txt
  ${TEMP}/bfTvBCOInterpolateImageFunctionOutput.txt
  otbBCOInterpolateImageFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionOutput.txt
  3 # radius
  -0.5 # optimised bicubic
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )
otb_add_test(NAME bfTvProlateInterpolateImageFunction COMMAND otbImageManipulationTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/bfProlateInterpolateImageFunctionOutput.txt
  ${TEMP}/bfProlateInterpolateImageFunctionOutput.txt
  otbProlateInterpolateImageFunction
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfProlateInterpolateImageFunctionOutput.txt
  ${TEMP}/bfProlateInterpolateImageFunctionCosOutput.tif
  ${TEMP}/bfProlateInterpolateImageFunctionITKCosOutput.tif
  ${TEMP}/bfProlateInterpolateImageFunctionProOutput.tif
  3 # radius
  0.5 0.5
  127.33 44.9
  259.67 21.43
  12.13 61.79
  89.5 11
  128 128
  127.255 128.73
  -1 -1
  )
otb_add_test(NAME bfTvProlateValidationTest COMMAND otbImageManipulationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvProlateValidationTest.tif
  ${TEMP}/bfTvProlateValidationTest.tif
  otbProlateValidationTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvProlateValidationTest.tif
  3 # radius
  0.5 # factor
  512 # size
  ${TEMP}/defaultprolatevalidationtest.tif # nearest neighborhood interpolator : NOT GENERATE IN THE TEST
  )
