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

#include "otbScalarVectorImageTextureFunctor.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbUnaryFunctorNeighborhoodImageFilter.h"


int otbScalarVectorImageTextureFunctorNew(int argc, char * argv[])
{
  typedef double InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType,Dimension> InputImageType;
  typedef otb::VectorImage<InputPixelType,Dimension> InputVectorImageType;
  typedef InputVectorImageType::PixelType VectorImagePixelType;

  typedef itk::ConstNeighborhoodIterator<InputImageType>   IterType;
  //typedef itk::Statistics::ScalarImageTextureCalculator< InputImageType > TextureCalcType;
  //typedef TextureCalcType::FeatureValueVectorPointer TextureCalcType;

  typedef otb::Functor::ScalarVectorImageTextureFunctor<IterType, InputImageType, VectorImagePixelType>  FunctorType;

  typedef otb::UnaryFunctorNeighborhoodImageFilter<InputImageType, InputVectorImageType, FunctorType> UnaryFunctorNeighborhoodImageFilterType;

  // Instantiating object
  UnaryFunctorNeighborhoodImageFilterType::Pointer object = UnaryFunctorNeighborhoodImageFilterType::New();

  return EXIT_SUCCESS;
}
