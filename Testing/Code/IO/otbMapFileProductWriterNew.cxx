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

#include "otbMapFileProductWriter.h"
#include "otbVectorImage.h"

int otbMapFileProductWriterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef otb::VectorImage<double>               ImageType;
  typedef otb::MapFileProductWriter<ImageType>   MapFileProductWriterType;

  MapFileProductWriterType::Pointer object = MapFileProductWriterType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
