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
#include "otbVariadicInputsImageFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkNeighborhood.h"
#include "itkImageRegionIterator.h"
#include "itkImageSource.h"
#include <tuple>


struct Funct1
{
  double operator()(double p) const
  {
    return p;
  }
};

struct Funct2
{
  itk::VariableLengthVector<double> operator()(double p) const
  {
    itk::VariableLengthVector<double> result(OutputSize);
    result[0] = result[1] = p;
    return result;
  }

  static constexpr size_t OutputSize = 2;
};

using IntImageNeighborhood = itk::Neighborhood<int>;
struct Funct3
{
  double operator()(const IntImageNeighborhood & p) const
  {
    return static_cast<double>(p.GetCenterValue());
  }
};

auto Lambda1 = [](double p)
               {
                 return p;
               };

struct Funct4
{
  itk::VariableLengthVector<double> operator()(double p, const itk::VariableLengthVector<double> & vp) const
  {
    itk::VariableLengthVector<double> result(2);
    result.Fill(p);
    return result;
  }
};

using OImage = typename otb::Image<int>;
using Neig = typename itk::Neighborhood<int>;

static_assert(otb::IsNeighborhood<Neig>::value, "err");
static_assert(!otb::IsNeighborhood<OImage>::value, "err");
static_assert(!otb::IsNeighborhood<double>::value, "err");


using namespace otb::functor_filter_details;
using namespace otb;

int otbFunctorImageFilter(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  // test functions in functor_filter_details namespace
  using VectorImageType = VectorImage<double>;
  using ImageType       = Image<double>;
  using RegionType      = typename ImageType::RegionType;
  using SizeType        = typename RegionType::SizeType;
  using IndexType       = typename RegionType::IndexType;
  
  using NeighborhoodType = itk::Neighborhood<double,2>;
  using VectorPixelType  = typename VectorImageType::PixelType;

  auto vimage = VectorImageType::New();
  auto image  = ImageType::New();

  SizeType size = {{200,200}};
  
  vimage->SetRegions(size);
  vimage->SetNumberOfComponentsPerPixel(2);
  vimage->Allocate();
  itk::VariableLengthVector<double> v(2);
  v.Fill(0);
  vimage->FillBuffer(v);
  
  image->SetRegions(size);
  image->Allocate();
  image->FillBuffer(0);

  auto iterators = MakeIterators(std::make_tuple(image,vimage),image->GetBufferedRegion());

  MoveIterators(iterators);

  Funct4 f{};
  
  auto res = CallOperator(f,iterators);

  // Test VariadicInputsImageFilter

  auto filter = otb::VariadicInputsImageFilter<VectorImageType,VectorImageType,ImageType>::New();

  filter->SetVInput<0>(vimage);
  filter->SetVInput<1>(image);

  filter->SetVInputs(vimage,image);

  std::cout<<filter->GetVInput<0>()<< filter->GetVInput<1>()<<std::endl;

  
  // test FunctorImageFilter
  auto filter1 = otb::FunctorImageFilter<Funct1>::New(Funct1{});
  auto filter2 = otb::FunctorImageFilter<Funct2>::New(Funct2{});
  //auto filter3 = otb::FunctorImageFilter<Funct3>::New(Funct3{});
  auto filter4 = otb::FunctorImageFilter<Funct4>::New(Funct4{});
  auto filterLambda = otb::FunctorImageFilter<decltype(Lambda1)>::New(Lambda1);

  filter1->SetVInputs(image);
  filter1->Update();
  
  filter2->SetVInputs(image);
  filter2->Update();

  filter4->SetVInputs(image,vimage);
  filter4->Update();

  filterLambda->SetVInputs(image);
  filterLambda->Update();

  
 return EXIT_SUCCESS;
}

