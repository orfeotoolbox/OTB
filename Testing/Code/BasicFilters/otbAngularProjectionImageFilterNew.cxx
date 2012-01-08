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

#include "itkFixedArray.h"

#include "otbMath.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"

#include "otbAngularProjectionImageFilter.h"

int otbAngularProjectionImageFilterNew ( int argc, char * argv[] )
{
  const unsigned int Dimension = 2;
  
  typedef float  PixelType;
  typedef double PrecisionType;
  typedef itk::FixedArray< PrecisionType, 1 > AngleType;
  typedef otb::Image< PixelType, Dimension >  ImageType;

  typedef otb::AngularProjectionImageFilter< 
    ImageType, ImageType, AngleType, PrecisionType > FilterType;

  FilterType::Pointer filter = FilterType::New();
  return EXIT_SUCCESS;
}


