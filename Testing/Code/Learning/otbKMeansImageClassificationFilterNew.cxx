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
#include "otbKMeansImageClassificationFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

int otbKMeansImageClassificationFilterNew(int argc, char * argv[])
{
  const unsigned int     Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::VectorImage<PixelType,Dimension> ImageType;
  typedef otb::Image<LabeledPixelType,Dimension> LabeledImageType;

  typedef otb::KMeansImageClassificationFilter<ImageType,LabeledImageType> ClassificationFilterType;
  
  // Instantiating object
  ClassificationFilterType::Pointer object = ClassificationFilterType::New();

  return EXIT_SUCCESS;
}
