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


#include "otbMRFEnergyPotts.h"
#include "otbImage.h"
#include <fstream>

int otbMRFEnergyPotts(int itkNotUsed(argc), char * argv[])
{
  const char * outputFile = argv[1];

  typedef double                    PixelType;
  typedef int                       PixelType2;
  typedef otb::Image<PixelType, 2>  ImageType;
  typedef otb::Image<PixelType2, 2> LabelType;

  typedef otb::MRFEnergyPotts<ImageType, LabelType>  MRFEnergyPottsType;
  typedef MRFEnergyPottsType::InputImagePixelType    InputImagePixelType;
  typedef MRFEnergyPottsType::LabelledImagePixelType LabelledImagePixelType;

  MRFEnergyPottsType::Pointer object    = MRFEnergyPottsType::New();

  InputImagePixelType    inPix = 10;
  LabelledImagePixelType inLab = 5;

  std::ofstream file;
  file.open(outputFile);

  file << "GetValue(const InputImagePixelType = 10, const LabelledImagePixelType = 5): " << object->GetValue(inPix,
                                                                                                             inLab) <<
  std::endl;
  inPix = 5;
  file << "GetValue(const InputImagePixelType = 5, const LabelledImagePixelType = 5) : " << object->GetValue(inPix,
                                                                                                             inLab) <<
  std::endl;

  file.close();

  return EXIT_SUCCESS;
}
