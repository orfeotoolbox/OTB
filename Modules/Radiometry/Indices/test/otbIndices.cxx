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
#include <iomanip>

template <typename T> itk::VariableLengthVector<T> build_pixel(const std::initializer_list<T> & il)
{
  itk::VariableLengthVector<T> res(il.size());
  size_t idx = 0;

  for(auto v: il)
    {
    res[idx]=v;
    ++idx;
    }
  return res;
}

template <class TIndice> bool CheckResult(const std::string & testName, std::map<typename TIndice::BandNameType,size_t> bandMap,
                                          const std::initializer_list<typename TIndice::InputType>& input, const typename TIndice::OutputType & expected)
{
  TIndice indice;
  
  indice.SetBandsIndices(bandMap);
  
  auto pixel = build_pixel(input);

  typename TIndice::OutputType v = indice(pixel);

  if(std::abs(expected - v) > TIndice::Epsilon)
    {
    std::cerr<<std::setprecision(10);
    std::cerr<<testName <<"\t- failed: expected "<<expected<<", got "<<v<<std::endl;
    return false;
    }
  else
    {
    std::cerr<<testName <<"\t- success"<<std::endl;
    return true;
    }
}


using namespace otb::Functor;

int otbVegetationIndicesTest(int, char**)
{
  
  const std::map<CommonBandNames,size_t> bandMap = {{CommonBandNames::BLUE,0},{CommonBandNames::GREEN,1},{CommonBandNames::RED,2},{CommonBandNames::NIR,3}};

  // Syntax: CheckResult<Indice Class>("test_name",bandMap,{red_value,nir_value},expected_result)
  bool res = CheckResult< NDVI<int,double> >("ndvi_null ",bandMap,{0,0,0,0},0.);
  res = res & CheckResult< NDVI<int,double> >("ndvi_pixel",bandMap,{0,0,1,2},0.3333333);
  res = res & CheckResult< RVI<int,double> >("rvi_null",bandMap,{0,0,0,0},0.);
  res = res & CheckResult< RVI<int,double> >("rvi_pixel",bandMap,{0,0,2,1},0.5);
  res = res & CheckResult< PVI<int,double> >("pvi_pixel",bandMap,{0,0,1,2},-62.0544166);
  res = res & CheckResult< SAVI<double,double> >("savi_null",bandMap,{0,0,0,-0.5},0.);
  res = res & CheckResult< SAVI<int,double> >("savi_pixel",bandMap,{0,0,1,2},0.42857142857);
  res = res & CheckResult< TSAVI<double,double> >("tsavi_null",bandMap,{0,0,-0.1192,0.},0.);
  res = res & CheckResult< TSAVI<int,double> >("tsavi_pixel",bandMap,{0,0,1,2},0.1111463957);
  res = res & CheckResult< WDVI<int,double> >("wdvi_pixel",bandMap,{0,0,1,2},1.6);
  res = res & CheckResult< MSAVI<int,double> >("msavi_pixel",bandMap,{0,0,1,2},0.4402985075);
  res = res & CheckResult< MSAVI2<int,double> >("msavi2_pixel",bandMap,{0,0,1,2},0.4384471872);
  res = res & CheckResult< GEMI<int,double> >("gemi_pixel",bandMap,{1,4,3,2},2.0625);
  res = res & CheckResult< AVI<int,double> >("avi_pixel",bandMap,{0,0,1,2},0.1017245527);
  res = res & CheckResult< ARVI<int,double> >("arvi_pixel",bandMap,{0,0,1,2},0.1428571429);
  /// TODO: TSARVI always returns 0, remove it?
  res = res & CheckResult< TSARVI<int,double> >("tsarvi_pixel",bandMap,{1,4,3,2},0.);
  res = res & CheckResult< EVI<int,double> >("evi_pixel",bandMap,{0,0,1,2},0.2777777778);
  res = res & CheckResult< IPVI<int,double> >("ipvi_pixel",bandMap,{0,0,1,2},0.6666666667);
  res = res & CheckResult< LAIFromNDVILogarithmic<int,double> >("lailog_pixel",bandMap,{0,0,1,2},0.4930511672);
  res = res & CheckResult< LAIFromReflectancesLinear<int,double> >("lailog_pixel",bandMap,{0,0,1,2},6.61);
  res = res & CheckResult< LAIFromNDVIFormosat2Functor<int,double> >("laiform_pixel",bandMap,{0,0,1,2},-0.08836147592);  

  if(res)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}
