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
#include "itkExceptionObject.h"

#include "otbScalarImageTextureFunctor.h"
#include "otbImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbUnaryFunctorNeighborhoodImageFilter.h"


int otbScalarImageTextureFunctorNew(int argc, char * argv[])
{
  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType,Dimension> InputImageType;

  typedef itk::ConstNeighborhoodIterator<InputImageType>   IterType;
  //typedef itk::Statistics::ScalarImageTextureCalculator< InputImageType > TextureCalcType;
  //typedef TextureCalcType::FeatureValueVectorPointer TextureCalcType;

  typedef otb::Functor::ScalarImageTextureFunctor<IterType, InputImageType, InputPixelType>  FunctorType;

  typedef otb::UnaryFunctorNeighborhoodImageFilter<InputImageType, InputImageType, FunctorType> UnaryFunctorNeighborhoodImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();

  return EXIT_SUCCESS;
}
