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
#include "otbSubPixelDisparityImageFilter.h"
#include "otbPixelWiseBlockMatchingImageFilter.h"
#include "otbImage.h"

int otbSubPixelDisparityImageFilterNew(int argc, char* argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                          PixelType;
  typedef otb::Image<PixelType, Dimension>                                      FloatImageType;
  typedef unsigned char                                                         MaskPixelType;
  typedef otb::Image<MaskPixelType, Dimension>                                  MaskType;

  typedef otb::Functor::SSDBlockMatching<FloatImageType,FloatImageType> SSDBlockMatchingFunctorType;



  typedef otb::SubPixelDisparityImageFilter<FloatImageType,FloatImageType,FloatImageType,FloatImageType,
                                SSDBlockMatchingFunctorType> SubPixelDisparityImageFilterType;

  // Instantiation
  SubPixelDisparityImageFilterType::Pointer filter = SubPixelDisparityImageFilterType::New();

   return EXIT_SUCCESS;
}






