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

#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbVectorImage.h"

int otbStreamingStatisticsVectorImageFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char PixelType;

  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef otb::StreamingStatisticsVectorImageFilter<ImageType> StreamingStatisticsVectorImageFilterType;

  // Instantiating object
  StreamingStatisticsVectorImageFilterType::Pointer object = StreamingStatisticsVectorImageFilterType::New();


  return EXIT_SUCCESS;
}
