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



#include "otbROIdataConversion.h"
#include "otbVectorImage.h"

int otbROIdataConversionNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef double PixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType,  Dimension>                InputImageType;
  typedef otb::Image<PixelType,  Dimension>                      ROIImageType;
  typedef otb::ROIdataConversion<InputImageType, ROIImageType>   ConvertorType;

  ConvertorType::Pointer convertor = ConvertorType::New();

  std::cout << convertor << std::endl;

  return EXIT_SUCCESS;
}
