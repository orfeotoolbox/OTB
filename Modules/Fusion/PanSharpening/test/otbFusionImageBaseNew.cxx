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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbFusionImageBase.h"

namespace Functor
{
template <class TInputMultiSpectral, class TInputMultiSpectralInterp, class TInputPanchro, class TOutput>
class NewFunctorTest
{
public:
  NewFunctorTest() {}
  ~NewFunctorTest() {}

  inline TOutput operator ()(const TInputMultiSpectral& A, const TInputMultiSpectralInterp& B, const TInputPanchro& C)
  {
    return (static_cast<TOutput>(A[0]) + static_cast<TOutput>(B[0]) + static_cast<TOutput>(C));

  }
};
}

int otbFusionImageBaseNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;

  typedef otb::Image<InputPixelType, Dimension>       InputPanchroImageType;
  typedef otb::VectorImage<InputPixelType, Dimension> InputMultiSpectralImageType;
  typedef otb::VectorImage<InputPixelType, Dimension> InputMultiSpectralInterpImageType;
  typedef otb::Image<OutputPixelType, Dimension>      OutputImageType;

  typedef otb::FusionImageBase<InputMultiSpectralImageType,
      InputMultiSpectralInterpImageType,
      InputPanchroImageType,
      OutputImageType,
      Functor::NewFunctorTest<InputMultiSpectralImageType::PixelType,
          InputMultiSpectralInterpImageType::PixelType,
          InputPanchroImageType::PixelType,
          OutputImageType::PixelType>
      >  FusionImageBaseType;

  // Instantiation
  FusionImageBaseType::Pointer base = FusionImageBaseType::New();

  std::cout << base << std::endl;

  return EXIT_SUCCESS;
}
