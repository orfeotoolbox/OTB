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
#include "otbImageToLineSegmentVectorData.h"



int otbImageToLineSegmentVectorDataNew( int argc, char * argv[] )
{
  
  typedef float                                              InputPixelType;
  const   unsigned int                                       Dimension = 2;
  
  /** Typedefs */
  typedef otb::Image< InputPixelType,  Dimension >           ImageType;
  typedef otb::ImageToLineSegmentVectorData<ImageType> PersistentLSDFilterType;

  
 PersistentLSDFilterType::Pointer  lsdFilter = PersistentLSDFilterType::New();
  
  return EXIT_SUCCESS;
}

