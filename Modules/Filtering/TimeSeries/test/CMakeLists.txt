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
set(OTBTimeSeriesTests
  otbEnvelopeSavitzkyGolayInterpolationFunctorTest.cxx
  otbPolynomialTimeSeriesTest.cxx
  otbSavitzkyGolayInterpolationFunctorTest.cxx
  otbTimeSeriesLeastSquareFittingFunctorTest.cxx
  otbTimeSeriesLeastSquareFittingFunctorWeightsTest.cxx
  otbTimeSeriesTestDriver.cxx  )

add_executable(otbTimeSeriesTestDriver ${OTBTimeSeriesTests})
target_link_libraries(otbTimeSeriesTestDriver ${OTBTimeSeries-Test_LIBRARIES})
otb_module_target_label(otbTimeSeriesTestDriver)
otb_add_test(NAME mtTvEnvelopeSavitzkyGolayInterpolationFunctorTest COMMAND otbTimeSeriesTestDriver
  otbEnvelopeSavitzkyGolayInterpolationFunctorTest
  )
otb_add_test(NAME mtTvPolynomialTimeSeriesTest1 COMMAND otbTimeSeriesTestDriver
  otbPolynomialTimeSeriesTest
  )
otb_add_test(NAME mtTvSavitzkyGolayInterpolationFunctorTest COMMAND otbTimeSeriesTestDriver
  otbSavitzkyGolayInterpolationFunctorTest
  )
otb_add_test(NAME mtTvTimeSeriesLeastSquaresFittingFunctor2 COMMAND otbTimeSeriesTestDriver
  otbTimeSeriesLeastSquareFittingFunctorTest
  10 0.3 3.123
  )
otb_add_test(NAME mtTvTimeSeriesLeastSquaresFittingFunctor COMMAND otbTimeSeriesTestDriver
  otbTimeSeriesLeastSquareFittingFunctorTest
  1 2 3
  )
otb_add_test(NAME mtTvTimeSeriesLeastSquaresFittingFunctorWeights COMMAND otbTimeSeriesTestDriver
  otbTimeSeriesLeastSquareFittingFunctorWeightsTest
  1 2 3
  )
