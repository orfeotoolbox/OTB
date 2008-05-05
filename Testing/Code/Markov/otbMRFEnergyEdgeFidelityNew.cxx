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

#include "otbMRFEnergyEdgeFidelity.h"
#include "otbImage.h"


int otbMRFEnergyEdgeFidelityNew(int argc, char * argv[])
{
  typedef double PixelType;
  typedef int    PixelType2;

  typedef otb::Image<PixelType, 2>                            ImageType;
  typedef otb::Image<PixelType2, 2>                           ImageType2;
  typedef otb::MRFEnergyEdgeFidelity< ImageType, ImageType >  MRFEnergyType;

  MRFEnergyType::Pointer  object  = MRFEnergyType::New();


  return EXIT_SUCCESS;
}

