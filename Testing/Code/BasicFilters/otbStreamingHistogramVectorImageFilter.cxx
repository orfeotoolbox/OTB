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

#include "otbStreamingHistogramVectorImageFilter.h"
#include "otbVectorImage.h"

typedef otb::VectorImage<unsigned char>               VectorImageType;
typedef otb::StreamingHistogramVectorImageFilter<VectorImageType>                SHVIFType;



int otbStreamingHistogramVectorImageFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{

  SHVIFType::Pointer SHVIFFilter = SHVIFType::New();

  std::cout << SHVIFFilter << std::endl;

  return EXIT_SUCCESS;
}
