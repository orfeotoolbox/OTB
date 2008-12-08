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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"

#include "otbMRFSamplerMAP.h"
#include "otbImage.h"


int otbMRFSamplerMAPNew(int argc, char * argv[])
{
  typedef double PixelType;
  typedef otb::Image<PixelType, 2>               ImageType;
  typedef otb::MRFSamplerMAP< ImageType, ImageType> MRFSamplerType;

  MRFSamplerType::Pointer object = MRFSamplerType::New();

  return EXIT_SUCCESS;
}

