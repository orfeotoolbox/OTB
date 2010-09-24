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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbROIdataConversion.h"
#include "otbImage.h"
#include "otbVectorImage.h"

int otbROIdataConversionNew(int argc, char* argv[])
{
  typedef double PixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType,  Dimension>                InputImageType;
  typedef otb::ROIdataConversion<InputImageType, InputImageType> ConvertorType;

  ConvertorType::Pointer convertor = ConvertorType::New();

  std::cout << convertor << std::endl;

  return EXIT_SUCCESS;
}
