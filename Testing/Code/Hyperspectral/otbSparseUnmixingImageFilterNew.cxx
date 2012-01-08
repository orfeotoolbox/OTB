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
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandProgressUpdate.h"
#include "otbCommandLineArgumentParser.h"

#include "otbSparseUnmixingImageFilter.h"

int otbSparseUnmixingImageFilterNew ( int argc, char * argv[] )
{
  // number of images to consider
  const unsigned int nbInputImages = 2;
  const unsigned int Dimension = 2;
  typedef double PixelType;
  typedef otb::Image< PixelType, Dimension > ImageType;

  typedef otb::SparseUnmixingImageFilter< ImageType, ImageType, nbInputImages > FilterType;
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}


