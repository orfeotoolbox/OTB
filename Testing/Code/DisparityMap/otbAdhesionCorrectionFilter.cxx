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
#include "otbAdhesionCorrectionFilter.h"
#include "otbImage.h"

int otbAdhesionCorrectionFilterNew(int argc, char* argv[])
{
  const unsigned int Dimension = 2;
  typedef float                                                         PixelType;
  typedef otb::Image<PixelType, Dimension>                              ImageType;
  typedef unsigned char                                                 MaskPixelType;
  typedef otb::Image<MaskPixelType, Dimension>                          MaskType;

  typedef otb::AdhesionCorrectionFilter<ImageType, MaskType> AdhesionCorrectionFilterType;

  // Instantiation
  AdhesionCorrectionFilterType::Pointer filter = AdhesionCorrectionFilterType::New();

   return EXIT_SUCCESS;
}

int otbAdhesionCorrectionFilter(int argc, char* argv[])
{

   return EXIT_SUCCESS;
}


