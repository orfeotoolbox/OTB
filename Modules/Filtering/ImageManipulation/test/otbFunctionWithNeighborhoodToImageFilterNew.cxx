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

#include "itkMacro.h"
#include "itkNeighborhood.h"
#include "otbImage.h"
//#include "itkVariableLengthVector.h"
//#include "itkConstNeighborhoodIterator.h"
#include "otbTextureImageFunction.h"
#include "otbFunctionWithNeighborhoodToImageFilter.h"
//#include "otbTextureFunctorBase.h"

template <class TInputScalarType, class TOutputScalarType> //IterInput1, class TIterInput2, class TOutput>
class TextureFunctorTest
{
public:
  /** Return the texture name */
  virtual std::string GetName() const
  {
    return "TextureFunctorTest";
  }

  TextureFunctorTest()
  {
    m_Offset.Fill(1);
  }
  virtual ~TextureFunctorTest() {}

  typedef itk::Offset<>                          OffsetType;
  typedef itk::Neighborhood<TInputScalarType, 2> NeighborhoodType;

  void SetOffset(OffsetType off){ m_Offset = off; }

  inline TOutputScalarType operator ()(const NeighborhoodType& neigh)
  {
    return static_cast<TOutputScalarType>(neigh.GetCenterValue());
  }

private:
  OffsetType m_Offset;
};

int otbFunctionWithNeighborhoodToImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef double                                    PixelType;
  typedef otb::Image<PixelType, Dimension>          ImageType;
  typedef TextureFunctorTest<PixelType, PixelType>                                       FunctorType;
  typedef otb::TextureImageFunction<ImageType, FunctorType>                              FunctionType;
  typedef otb::FunctionWithNeighborhoodToImageFilter<ImageType, ImageType, FunctionType> FilterType;

  // Instantiating object
  FilterType::Pointer filter = FilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
