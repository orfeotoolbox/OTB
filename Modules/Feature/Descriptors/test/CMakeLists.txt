#
# Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

set(OTBDescriptorsTests
otbDescriptorsTestDriver.cxx
otbHistogramOfOrientedGradientCovariantImageFunction.cxx
otbFourierMellinImageFilter.cxx
otbImageToHessianDeterminantImageFilter.cxx
otbFourierMellinDescriptors.cxx
otbKeyPointsAlgorithmsTest.cxx
)

add_executable(otbDescriptorsTestDriver ${OTBDescriptorsTests})
target_link_libraries(otbDescriptorsTestDriver ${OTBDescriptors-Test_LIBRARIES})
otb_module_target_label(otbDescriptorsTestDriver)

# Tests Declaration

otb_add_test(NAME feTvHistogramOfOrientedGradientCovariantImageFunction COMMAND otbDescriptorsTestDriver
  --compare-ascii ${EPSILON_8}
  ${BASELINE_FILES}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  ${TEMP}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  otbHistogramOfOrientedGradientCovariantImageFunction
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles.tif
  ${TEMP}/feTvHistogramOfOrientedGradientCovariantImageFunction.txt
  5 273 64
  )



otb_add_test(NAME feTvForwardFourierMellinImageFilter COMMAND otbDescriptorsTestDriver
  --compare-n-images ${EPSILON_6} 2
  ${BASELINE}/feForwardFourierMellinImageFilterReal.tif
  ${TEMP}/feForwardFourierMellinImageFilterReal.tif
  ${BASELINE}/feForwardFourierMellinImageFilterImag.tif
  ${TEMP}/feForwardFourierMellinImageFilterImag.tif
  otbFourierMellinImageFilter
  ${INPUTDATA}/circle.hdr
  ${TEMP}/feForwardFourierMellinImageFilterReal.tif
  ${TEMP}/feForwardFourierMellinImageFilterImag.tif)

otb_add_test(NAME feTvImageToHessianDeterminantImageFilter COMMAND otbDescriptorsTestDriver
  --compare-image ${EPSILON_3}
  ${BASELINE}/feTvImageToHessianDeterminantImageFilterOutput.tif
  ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
  otbImageToHessianDeterminantImageFilter
  ${INPUTDATA}/ROI_IKO_PAN_LesHalles_sub.tif
  ${TEMP}/feTvImageToHessianDeterminantImageFilterOutput.tif
  1.5
  )

otb_add_test(NAME feTvFourierMellinDescriptorsRotationInvariant COMMAND otbDescriptorsTestDriver
  otbFourierMellinDescriptorsRotationInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  90
  )


otb_add_test(NAME feTvFourierMellinDescriptorsScaleInvariant COMMAND otbDescriptorsTestDriver
  otbFourierMellinDescriptorsScaleInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  )

otb_add_test(NAME feTvFourierMellinDescriptors COMMAND otbDescriptorsTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/feTvFourierMellinDescriptors.txt
  ${TEMP}/feTvFourierMellinDescriptors.txt
  otbFourierMellinDescriptors
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvFourierMellinDescriptors.txt
  )

otb_add_test(NAME feTvKeyPointsAlgorithmsTest COMMAND otbDescriptorsTestDriver
  otbKeyPointsAlgorithmsTest
  ${INPUTDATA}/QB_TOULOUSE_RpcTag_100_100.tif
)
