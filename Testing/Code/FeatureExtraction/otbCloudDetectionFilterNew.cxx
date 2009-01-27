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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbCloudDetectionFilter.h"

int otbCloudDetectionFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension                                  = 2;
  typedef double                                                PixelType;
  typedef otb::Image<PixelType,Dimension>                       ImageType;
  typedef otb::VectorImage<PixelType,Dimension>                 VectorImageType;
  typedef otb::CloudDetectionFilter<VectorImageType,ImageType > CloudDetectionFilterType;


  // Instantiating object
  CloudDetectionFilterType::Pointer cloudDetection = CloudDetectionFilterType::New();


  return EXIT_SUCCESS;
}
