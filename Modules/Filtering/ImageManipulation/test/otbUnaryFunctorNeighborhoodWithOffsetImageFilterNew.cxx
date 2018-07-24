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
#include "otbExtern.h"
#include "otbExtern.h"


#include "otbUnaryFunctorNeighborhoodWithOffsetImageFilter.h"
#include "otbVectorImage.h"
#include "itkOffset.h"

namespace Functor
{
template <class TInput, class TOutput>
class UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest
{
public:
  UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest() {}
  ~UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest() {}

  typedef TInput                                      InputScalarType;
  typedef TOutput                                     OutputScalarType;
  typedef itk::VariableLengthVector<InputScalarType>  InputVectorType;
  typedef itk::VariableLengthVector<OutputScalarType> OutputVectorType;
  typedef itk::Offset<>                               OffsetType;
  typedef itk::Neighborhood<InputScalarType, 2>       NeighborhoodType;
  typedef itk::Neighborhood<InputVectorType, 2>       NeighborhoodVectorType;

  void SetOffset(OffsetType off)
  {
    m_Offset = off;
  }
  OffsetType GetOffset()
  {
    return m_Offset;
  }

  inline OutputScalarType operator ()(const NeighborhoodType& neigh)
  {
    return (static_cast<OutputScalarType>(neigh.GetCenterValue()));
  }
  inline OutputVectorType operator ()(const NeighborhoodVectorType& neigh)
  {
    return (static_cast<OutputVectorType>(neigh.GetCenterValue()));

  }

private:
  OffsetType m_Offset;
};
}

int otbUnaryFunctorNeighborhoodWithOffsetImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,
      Dimension>               ImageType;
  typedef Functor::UnaryFunctorNeighborhoodWithOffsetImageFilterFunctorNewTest<InputPixelType,
      InputPixelType> FunctorType;
  typedef otb::UnaryFunctorNeighborhoodWithOffsetImageFilter<ImageType, ImageType,
      FunctorType>
  UnaryFunctorNeighborhoodWithOffsetImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodWithOffsetImageFilterType::Pointer filter =
    UnaryFunctorNeighborhoodWithOffsetImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
