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


#include "otbMRFEnergyEdgeFidelity.h"
#include "otbImage.h"

int otbMRFEnergyEdgeFidelityNew(int argc, char * argv[])
{
  typedef double PixelType;
  typedef int    PixelType2;

  typedef otb::Image<PixelType, 2>                         ImageType;
  typedef otb::MRFEnergyEdgeFidelity<ImageType, ImageType> MRFEnergyType;

  MRFEnergyType::Pointer object  = MRFEnergyType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
