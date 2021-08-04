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
set(OTBSupervisedTests
otbSupervisedTestDriver.cxx
otbConfusionMatrixCalculatorTest.cxx
otbConfusionMatrixMeasurementsTest.cxx
otbMachineLearningModelCanRead.cxx
otbTrainMachineLearningModel.cxx
otbImageClassificationFilter.cxx
otbMachineLearningRegressionTests.cxx
otbExhaustiveExponentialOptimizerTest.cxx
)

if(OTB_USE_SHARK)
set(OTBSupervisedTests ${OTBSupervisedTests} otbSharkImageClassificationFilter.cxx)
endif()

if(OTB_USE_LIBSVM)
    set(OTBSupervisedTests ${OTBSupervisedTests} otbLabelMapClassifier.cxx otbSVMMarginSampler.cxx)
endif()

add_executable(otbSupervisedTestDriver ${OTBSupervisedTests})
target_link_libraries(otbSupervisedTestDriver ${OTBSupervised-Test_LIBRARIES})
otb_module_target_label(otbSupervisedTestDriver)

# Tests Declaration

otb_add_test(NAME leTvConfusionMatrixCalculatorWrongSize COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorWrongSize 1000 4)
set_property(TEST leTvConfusionMatrixCalculatorWrongSize PROPERTY WILL_FAIL true)

otb_add_test(NAME leTvConfusionMatrixCalculatorUpdate COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorCompute 4 3)

otb_add_test(NAME leTvConfusionMatrixCalculatorUpdateWithBaseline COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorComputeWithBaseline)

otb_add_test(NAME leTvConfusionMatrixCalculatorSetListSamples COMMAND otbSupervisedTestDriver
  otbConfusionMatrixCalculatorSetListSamples 1000 4)

otb_add_test(NAME leTvConfusionMatrixMeasurementsTest COMMAND otbSupervisedTestDriver
  otbConfusionMatrixMeasurementsTest)

otb_add_test(NAME leTvConfusionMatrixConcatenateTest COMMAND otbSupervisedTestDriver
  otbConfusionMatrixConcatenateTest
  ${INPUTDATA}/Classification/QB_1_ortho_C1.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C2.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C3.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C4.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C5.csv
  ${INPUTDATA}/Classification/QB_1_ortho_C6.csv)

otb_add_test(NAME leTvExhaustiveExponentialOptimizerTest COMMAND otbSupervisedTestDriver
  --compare-ascii ${NOTOL}
  ${BASELINE_FILES}/leTvExhaustiveExponentialOptimizerOutput.txt
  ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt
  otbExhaustiveExponentialOptimizerTest
  ${TEMP}/leTvExhaustiveExponentialOptimizerTestOutput.txt)

if(OTB_USE_LIBSVM)
  include(tests-libsvm.cmake)
endif()

if(OTB_USE_OPENCV)
  include(tests-opencv.cmake)
endif()

if(OTB_USE_SHARK)
  include(tests-shark.cmake)
endif()
