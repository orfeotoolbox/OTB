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

set(OTBMomentsTests
otbMomentsTestDriver.cxx
otbComplexMomentPathFloat.cxx
otbRadiometricMomentsImageFunction.cxx
otbHuMomentsImageFunction.cxx
otbRealMomentsImageFunction.cxx
otbComplexMomentsImageFunction.cxx
otbFlusserMomentsImageFunction.cxx
otbHuPath.cxx
otbFlusserPath.cxx
otbComplexMomentPath.cxx
otbRadiometricMomentsImageFilter.cxx
)

add_executable(otbMomentsTestDriver ${OTBMomentsTests})
target_link_libraries(otbMomentsTestDriver ${OTBMoments-Test_LIBRARIES})
otb_module_target_label(otbMomentsTestDriver)

# Tests Declaration

otb_add_test(NAME feTvComplexMomentPathFloat COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feComplexMomentPathFloat.txt
  ${TEMP}/feComplexMomentPathFloat.txt
  otbComplexMomentPathFloat
  1
  1
  ${TEMP}/feComplexMomentPathFloat.txt)


otb_add_test(NAME feTvRadiometricMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvRadiometricMomentsImageFunction.txt
  ${TEMP}/feTvRadiometricMomentsImageFunction.txt
  otbRadiometricMomentsImageFunction
  ${INPUTDATA}/poupees.png
  ${TEMP}/feTvRadiometricMomentsImageFunction.txt)



otb_add_test(NAME feTvHuMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvHuMomentsImageFunction.txt
  ${TEMP}/feTvHuMomentsImageFunction.txt
  otbHuMomentsImageFunction
  ${INPUTDATA}/poupees.png
  ${TEMP}/feTvHuMomentsImageFunction.txt
  )

otb_add_test(NAME feTvHuMomentsImageFunctionScaleInvariant COMMAND otbMomentsTestDriver
  otbHuMomentsImageFunctionScaleInvariant
  ${INPUTDATA}/poupees.png
  )


otb_add_test(NAME feTvHuMomentsImageFunctionRotationInvariant COMMAND otbMomentsTestDriver
  otbHuMomentsImageFunctionRotationInvariant
  ${INPUTDATA}/poupees.png
  90
  )


otb_add_test(NAME feTvRealMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvRealMomentsImageFunction.txt
  ${TEMP}/feTvRealMomentsImageFunction.txt
  otbRealMomentsImageFunction
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvRealMomentsImageFunction.txt)



otb_add_test(NAME feTvComplexMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feTvComplexMomentsImageFunction.txt
  ${TEMP}/feTvComplexMomentsImageFunction.txt
  otbComplexMomentsImageFunction
  ${INPUTDATA}/poupees.png
  4
  4
  ${TEMP}/feTvComplexMomentsImageFunction.txt
  )


otb_add_test(NAME feTvComplexMomentsImageFunctionScaleInvariant COMMAND otbMomentsTestDriver
  otbComplexMomentsImageFunctionScaleInvariant
  ${INPUTDATA}/poupees.png
  4
  4
  )






otb_add_test(NAME feTvFlusserMomentsImageFunctionScaleInvariant COMMAND otbMomentsTestDriver
  otbFlusserMomentsImageFunctionScaleInvariant
  ${INPUTDATA}/poupees.png
  )

otb_add_test(NAME feTvFlusserMomentsImageFunction COMMAND otbMomentsTestDriver
  --compare-ascii ${EPSILON_8}     ${BASELINE_FILES}/feTvFlusserMomentsImageFunction.txt
  ${TEMP}/feTvFlusserMomentsImageFunction.txt
  otbFlusserMomentsImageFunction
  ${INPUTDATA}/poupees.png
  ${TEMP}/feTvFlusserMomentsImageFunction.txt)

otb_add_test(NAME feTvFlusserMomentsImageFunctionRotationInvariant COMMAND otbMomentsTestDriver
  otbFlusserMomentsImageFunctionRotationInvariant
  ${INPUTDATA}/poupees.png
  90
  )



otb_add_test(NAME feTvHuPath COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feHuPath.txt
  ${TEMP}/feHuPath.txt
  otbHuPath
  ${TEMP}/feHuPath.txt)



otb_add_test(NAME feTvFlusserPath COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feFlusserPath.txt
  ${TEMP}/feFlusserPath.txt
  otbFlusserPath
  ${TEMP}/feFlusserPath.txt)

otb_add_test(NAME feTvComplexMomentPath COMMAND otbMomentsTestDriver
  --compare-ascii ${NOTOL}     ${BASELINE_FILES}/feComplexMomentPath.txt
  ${TEMP}/feComplexMomentPath.txt
  otbComplexMomentPath
  1
  1
  ${TEMP}/feComplexMomentPath.txt)


otb_add_test(NAME feTvRadiometricMomentsImageFilter COMMAND otbMomentsTestDriver
  --compare-image ${EPSILON_8}
  ${BASELINE}/feTvRadiometricMomentsImageFilter.tif
  ${TEMP}/feTvRadiometricMomentsImageFilter.tif
  otbRadiometricMomentsImageFilter
  ${INPUTDATA}/QB_Toulouse_Ortho_PAN.tif
  ${TEMP}/feTvRadiometricMomentsImageFilter.tif
  3 #radius
  )

