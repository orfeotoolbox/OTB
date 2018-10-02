/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "itkMacro.h"
#include "otbFunctorImageFilter.h"

struct Funct1 : otb::ImageFunctor<double, double>
{
  double operator()(double p)
  {
    return p;
  }
};

struct Funct2 : otb::ImageFunctor<double, itk::VariableLengthVector<double>>
{
  itk::VariableLengthVector<double> operator()(double p)
  {
    itk::VariableLengthVector<double> result(2);
    result[0] = result[1] = p;
    return result;
  }
};

using IntImageNeighborhood = itk::ConstNeighborhoodIterator<otb::Image<int>>;
struct Funct3 : otb::ImageFunctor<IntImageNeighborhood, 
                                  double>
{
  double operator()(IntImageNeighborhood p)
  {
    return static_cast<double>(p.GetPixel(0));
  }
};

auto Lambda1 = [](double p)
               {
                 return p;
               };


using OImage = typename otb::Image<int>;
using Neig = typename itk::ConstNeighborhoodIterator<OImage>;

static_assert(otb::IsNeighborhood<Neig>::value, "err");
static_assert(!otb::IsNeighborhood<OImage>::value, "err");
static_assert(!otb::IsNeighborhood<double>::value, "err");


int otbFunctorImageFilter(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  auto filter1 = otb::FunctorImageFilter<Funct1>::New(Funct1{});
  auto filter2 = otb::FunctorImageFilter<Funct2>::New(Funct2{});
  auto filter3 = otb::FunctorImageFilter<Funct3>::New(Funct3{});
 auto filter4 = otb::FunctorImageFilter<decltype(Lambda1)>::New(Lambda1);
 return EXIT_SUCCESS;
}

