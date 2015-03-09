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
#include <fstream>

int otbMRFEnergyEdgeFidelity(int itkNotUsed(argc), char * argv[])
{
  const char * outputFile = argv[1];

  typedef double                                           PixelTypeInput;
  typedef int                                              PixelTypeLabel;
  typedef otb::Image<PixelTypeInput, 2>                    ImageType;
  typedef otb::Image<PixelTypeLabel, 2>                    LabelType;
  typedef otb::MRFEnergyEdgeFidelity<ImageType, LabelType> MRFEnergyType;

  MRFEnergyType::Pointer object = MRFEnergyType::New();

  std::ofstream file;
  file.open(outputFile);

  file << "GetSimngleValue(10, 50): " << object->GetSingleValue(10, 50) << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
