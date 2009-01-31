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
#include "otbMorphologicalPyramidSegmenter.h"
#include "otbImage.h"

int otbMorphologicalPyramidSegmenterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef unsigned char InputPixelType;
  typedef unsigned short OutputPixelType;

  typedef otb::Image<InputPixelType,Dimension> InputImageType;
  typedef otb::Image<OutputPixelType,Dimension> OutputImageType;

  typedef otb::MorphologicalPyramid::Segmenter<InputImageType,OutputImageType>
  SegmenterType;

  // Instantiation
  SegmenterType::Pointer segmenter = SegmenterType::New();


  return EXIT_SUCCESS;
}
