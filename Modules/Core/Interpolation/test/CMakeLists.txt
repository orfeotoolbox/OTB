#
# Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

otb_module_test()

set(OTBInterpolationTests
otbInterpolationTestDriver.cxx
otbWindowedSincInterpolateImageLanczosFunction.cxx
otbWindowedSincInterpolateImageBlackmanFunction.cxx
otbBSplineDecompositionImageFilter.cxx
otbWindowedSincInterpolateImageGaussianFunction.cxx
otbWindowedSincInterpolateImageCosineFunction.cxx
otbWindowedSincInterpolateImageHammingFunction.cxx
otbWindowedSincInterpolateImageWelchFunction.cxx
otbBSplineInterpolateImageFunction.cxx
otbStreamingTraits.cxx
otbBCOInterpolateImageFunction.cxx
otbProlateInterpolateImageFunction.cxx
otbProlateValidationTest.cxx
)

add_executable(otbInterpolationTestDriver ${OTBInterpolationTests})
target_link_libraries(otbInterpolationTestDriver ${OTBInterpolation-Test_LIBRARIES})
otb_module_target_label(otbInterpolationTestDriver)

# Tests Declaration

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

otb_add_test(NAME bfTuStreamingTraitsVectorImage COMMAND otbInterpolationTestDriver
  otbStreamingTraitsVectorImage
  )

otb_add_test(NAME bfTuStreamingTraitsImage COMMAND otbInterpolationTestDriver
  otbStreamingTraitsImage
  )

otb_add_test(NAME bfTvBCOInterpolateImageFunctionVectorImageTest COMMAND otbInterpolationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvBCOInterpolateImageFunctionVectorImageTest.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionVectorImageTest.tif
  otbBCOInterpolateImageFunctionVectorImageTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionVectorImageTest.tif
  4 # radius
  -0.5 # optimised bicubic
  )
otb_add_test(NAME bfTvBCOInterpolateImageFunctionTest COMMAND otbInterpolationTestDriver
  --compare-image ${EPSILON_7}
  ${BASELINE}/bfTvBCOInterpolateImageFunctionTest.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionTest.tif
  otbBCOInterpolateImageFunctionTest
  ${INPUTDATA}/poupees.tif
  ${TEMP}/bfTvBCOInterpolateImageFunctionTest.tif
  3 # radius
  -0.5 # optimised bicubic
  )
otb_add_test(NAME bfTvBCOInterpolateImageFunctionOverVectorImage COMMAND otbInterpolationTestDriver
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
otb_add_test(NAME bfTvBCOInterpolateImageFunction2 COMMAND otbInterpolationTestDriver
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
otb_add_test(NAME bfTvBCOInterpolateImageFunction COMMAND otbInterpolationTestDriver
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
otb_add_test(NAME bfTvProlateInterpolateImageFunction COMMAND otbInterpolationTestDriver
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
otb_add_test(NAME bfTvProlateValidationTest COMMAND otbInterpolationTestDriver
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
