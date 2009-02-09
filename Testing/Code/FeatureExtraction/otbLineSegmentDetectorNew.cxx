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
#include "otbLineSegmentDetector.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"



int otbLineSegmentDetectorNew( int argc, char * argv[] )
{
  
  typedef float                                                     InputPixelType;
  const   unsigned int                                              Dimension = 2;
  
  /** Typedefs */
  typedef otb::Image< InputPixelType,  Dimension >                  InputImageType;
  typedef otb::LineSegmentDetector<InputImageType , InputPixelType> lsdFilterType;

  
  lsdFilterType::Pointer  lsdFilter = lsdFilterType::New();
  
  return EXIT_SUCCESS;
}

