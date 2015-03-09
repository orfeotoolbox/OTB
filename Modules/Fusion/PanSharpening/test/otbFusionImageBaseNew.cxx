/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

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
