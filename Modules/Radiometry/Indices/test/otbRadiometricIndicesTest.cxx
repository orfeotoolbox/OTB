/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbVegetationIndicesFunctor.h"
#include "otbWaterIndicesFunctor.h"
#include "otbBuiltUpIndicesFunctor.h"
#include "otbSoilIndicesFunctor.h"
#include "otbIndicesStackFunctor.h"

#include <iomanip>

template <typename T>
itk::VariableLengthVector<T> build_pixel(const std::initializer_list<T>& il)
{
  itk::VariableLengthVector<T> res(il.size());
  size_t                       idx = 0;

  for (auto v : il)
  {
    res[idx] = v;
    ++idx;
  }
  return res;
}

template <class TIndice>
bool CheckResult(const std::string& testName, std::map<typename TIndice::BandNameType, size_t> bandMap,
                 const std::initializer_list<typename TIndice::InputType>& input, const typename TIndice::OutputType& expected)
{
  TIndice indice;

  indice.SetBandsIndices(bandMap);

  auto pixel = build_pixel(input);

  typename TIndice::OutputType v = indice(pixel);

  if (std::abs(expected - v) > TIndice::Epsilon)
  {
    std::cerr << std::setprecision(10);
    std::cerr << testName << "\t- failed: expected " << expected << ", got " << v << std::endl;
    return false;
  }
  else
  {
    return true;
  }
}


using namespace otb::Functor;

int otbVegetationIndicesTest(int, char**)
{

  const std::map<CommonBandNames, size_t> bandMap = {
      {CommonBandNames::BLUE, 1}, {CommonBandNames::GREEN, 2}, {CommonBandNames::RED, 3}, {CommonBandNames::NIR, 4}};

  // Syntax: CheckResult<Indice Class>("test_name",bandMap,{red_value,nir_value},expected_result)
  bool res = CheckResult<NDVI<int, double>>("ndvi_null ", bandMap, {0, 0, 0, 0}, 0.);
  res      = res & CheckResult<NDVI<int, double>>("ndvi_pixel", bandMap, {0, 0, 1, 2}, 0.3333333);
  res      = res & CheckResult<RVI<int, double>>("rvi_null", bandMap, {0, 0, 0, 0}, 0.);
  res      = res & CheckResult<RVI<int, double>>("rvi_pixel", bandMap, {0, 0, 2, 1}, 0.5);
  res      = res & CheckResult<PVI<int, double>>("pvi_pixel", bandMap, {0, 0, 1, 2}, -62.0544166);
  res      = res & CheckResult<SAVI<double, double>>("savi_null", bandMap, {0, 0, 0, -0.5}, 0.);
  res      = res & CheckResult<SAVI<int, double>>("savi_pixel", bandMap, {0, 0, 1, 2}, 0.42857142857);
  res      = res & CheckResult<TSAVI<double, double>>("tsavi_null", bandMap, {0, 0, -0.1192, 0.}, 0.);
  res      = res & CheckResult<TSAVI<int, double>>("tsavi_pixel", bandMap, {0, 0, 1, 2}, 0.1111463957);
  res      = res & CheckResult<WDVI<int, double>>("wdvi_pixel", bandMap, {0, 0, 1, 2}, 1.6);
  res      = res & CheckResult<MSAVI<int, double>>("msavi_pixel", bandMap, {0, 0, 1, 2}, 0.4402985075);
  res      = res & CheckResult<MSAVI2<int, double>>("msavi2_pixel", bandMap, {0, 0, 1, 2}, 0.4384471872);
  res      = res & CheckResult<GEMI<int, double>>("gemi_pixel", bandMap, {1, 4, 3, 2}, 2.0625);
  res      = res & CheckResult<AVI<int, double>>("avi_pixel", bandMap, {0, 0, 1, 2}, 0.1017245527);
  res      = res & CheckResult<ARVI<int, double>>("arvi_pixel", bandMap, {0, 0, 1, 2}, 0.1428571429);
  res      = res & CheckResult<EVI<int, double>>("evi_pixel", bandMap, {0, 0, 1, 2}, 0.2777777778);
  res      = res & CheckResult<IPVI<int, double>>("ipvi_pixel", bandMap, {0, 0, 1, 2}, 0.6666666667);
  res      = res & CheckResult<LAIFromNDVILogarithmic<int, double>>("lailog_pixel", bandMap, {0, 0, 1, 2}, 0.4930511672);
  res      = res & CheckResult<LAIFromReflectancesLinear<int, double>>("lailog_pixel", bandMap, {0, 0, 1, 2}, 6.61);
  res      = res & CheckResult<LAIFromNDVIFormosat2Functor<int, double>>("laifrom_pixel", bandMap, {0, 0, 1, 2}, 0.3120010659);

  if (res)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}

int otbWaterIndicesTest(int, char**)
{
  const std::map<CommonBandNames, size_t> bandMap = {
      {CommonBandNames::BLUE, 1}, {CommonBandNames::GREEN, 2}, {CommonBandNames::RED, 3}, {CommonBandNames::NIR, 4}, {CommonBandNames::MIR, 5}};

  // Syntax: CheckResult<Indice Class>("test_name",bandMap,{red_value,nir_value},expected_result)
  bool res = CheckResult<NDWI<int, double>>("ndwi_null ", bandMap, {0, 0, 0, 0, 0}, 0.);
  res      = res & CheckResult<NDWI<int, double>>("ndwi_pixel", bandMap, {1, 2, 3, 4, 5}, -0.1111111111);
  res      = res & CheckResult<NDWI2<int, double>>("ndwi2_null", bandMap, {0, 0, 0, 0, 0}, 0.);
  res      = res & CheckResult<NDWI2<int, double>>("ndwi2_pixel", bandMap, {1, 2, 3, 4, 5}, -0.3333333333);
  res      = res & CheckResult<MNDWI<int, double>>("mndwi_null", bandMap, {0, 0, 0, 0, 0}, 0.);
  res      = res & CheckResult<MNDWI<int, double>>("mndwi_pixel", bandMap, {1, 2, 3, 4, 5}, -0.4285714286);
  res      = res & CheckResult<NDTI<int, double>>("ndti_null", bandMap, {0, 0, 0, 0, 0}, 0.);
  res      = res & CheckResult<NDTI<int, double>>("ndti_pixel", bandMap, {1, 2, 3, 4, 5}, 0.2);

  if (res)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}

int otbSoilIndicesTest(int, char**)
{
  const std::map<CommonBandNames, size_t> bandMap = {
      {CommonBandNames::BLUE, 1}, {CommonBandNames::GREEN, 2}, {CommonBandNames::RED, 3}, {CommonBandNames::NIR, 4}, {CommonBandNames::MIR, 5}};

  // Syntax: CheckResult<Indice Class>("test_name",bandMap,{red_value,nir_value},expected_result)
  bool res = CheckResult<CI<int, double>>("ci_null ", bandMap, {0, 0, 0, 0, 0}, 0.);
  res      = res & CheckResult<CI<int, double>>("ci_pixel", bandMap, {1, 2, 3, 4, 5}, 0.2);
  res      = res & CheckResult<BI<int, double>>("bi_pixel", bandMap, {1, 2, 3, 4, 5}, 2.549509757);
  res      = res & CheckResult<BI2<int, double>>("bi2_pixel", bandMap, {1, 2, 3, 4, 5}, 3.109126351);

  if (res)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}

int otbBuiltUpIndicesTest(int, char**)
{
  const std::map<CommonBandNames, size_t> bandMap = {
      {CommonBandNames::BLUE, 1}, {CommonBandNames::GREEN, 2}, {CommonBandNames::RED, 3}, {CommonBandNames::NIR, 4}, {CommonBandNames::MIR, 5}};

  // Syntax: CheckResult<Indice Class>("test_name",bandMap,{red_value,nir_value},expected_result)
  bool res = CheckResult<ISU<int, double>>("isu_null", bandMap, {0, 0, 0, 0, 0}, 0.);
  res      = res & CheckResult<ISU<int, double>>("isu_pixel", bandMap, {1, 2, 3, 4, 5}, 81.25);

  if (res)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}


int otbRadiometricIndexTest(int, char**)
{
  auto ndvi = NDVI<double, double>();

  auto requiredBands = ndvi.GetRequiredBands();

  bool success = true;

  if (requiredBands.size() != 2 || requiredBands.find(CommonBandNames::RED) == requiredBands.end() ||
      requiredBands.find(CommonBandNames::NIR) == requiredBands.end())
  {
    std::cerr << "Required bands is not {RED,NIR} for NDVI" << std::endl;
    success = false;
  }

  ndvi.SetBandIndex(CommonBandNames::RED, 10);

  if (ndvi.GetBandIndex(CommonBandNames::RED) != 10)
  {
    std::cerr << "Could not Set/Get band index properly" << std::endl;
    success = false;
  }

  const std::map<CommonBandNames, size_t> bandMap = {{CommonBandNames::RED, 100}, {CommonBandNames::NIR, 200}};

  ndvi.SetBandsIndices(bandMap);

  if (ndvi.GetBandIndex(CommonBandNames::RED) != 100 || ndvi.GetBandIndex(CommonBandNames::NIR) != 200)
  {
    std::cerr << "Could not set all band indices at once with SetBandIndices" << std::endl;
    success = false;
  }

  try
  {
    ndvi.SetBandIndex(CommonBandNames::MAX, 1);
    std::cerr << "Calling SetBandIndices with ::MAX should raise a runtime_error exception." << std::endl;
    success = false;
  }
  catch (const std::runtime_error& e)
  {
  }

  if (success)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}

int otbIndicesStackFunctorTest(int, char**)
{
  using IndicesType      = RadiometricIndex<double, int, CommonBandNames>;
  using StackFunctorType = IndicesStackFunctor<IndicesType>;

  auto ndvi = NDVI<double, int>();
  auto ndwi = NDWI<double, int>();

  std::vector<IndicesType*> indices = {&ndvi, &ndwi};

  auto stack = StackFunctorType(indices);

  bool success = true;

  if (stack.OutputSize() != 2)
  {
    std::cerr << "Size of output pixel for stack functor should be 2" << std::endl;
    success = false;
  }

  const std::map<CommonBandNames, size_t> bandMap = {
      {CommonBandNames::BLUE, 1}, {CommonBandNames::GREEN, 2}, {CommonBandNames::RED, 3}, {CommonBandNames::NIR, 4}, {CommonBandNames::MIR, 5}};

  ndvi.SetBandsIndices(bandMap);
  ndwi.SetBandsIndices(bandMap);

  StackFunctorType::OutputType out(2);

  auto in = build_pixel<double>({1, 2, 3, 4, 5});

  stack(out, in);

  if (out[0] != ndvi(in))
  {
    std::cerr << "First output band should correspond to ndvi" << std::endl;
    success = false;
  }

  if (out[1] != ndwi(in))
  {
    std::cerr << "Second output band should correspond to ndwi" << std::endl;
    success = false;
  }

  if (success)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    return EXIT_FAILURE;
  }
}
