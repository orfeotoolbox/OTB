/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  This code is from a Jan Wegner contribution.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include "otbMRFEnergyFisherClassification.h"
#include "otbImage.h"
#include <fstream>

int otbMRFEnergyFisherClassificationNew(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef double                        PixelTypeInput;
  typedef int                           PixelTypeLabel;
  typedef otb::Image<PixelTypeInput, 2> ImageType;
  typedef otb::Image<PixelTypeLabel, 2> LabelType;

  typedef otb::MRFEnergyFisherClassification<ImageType, LabelType> MRFFisherType;

  MRFFisherType::Pointer classif = MRFFisherType::New();

  std::cout << classif << std::endl;

  return EXIT_SUCCESS;
}

int otbMRFEnergyFisherClassification(int itkNotUsed(argc), char * argv[])
{
  const char * outputFile = argv[1];

  typedef double                        PixelTypeInput;
  typedef int                           PixelTypeLabel;
  typedef otb::Image<PixelTypeInput, 2> ImageType;
  typedef otb::Image<PixelTypeLabel, 2> LabelType;

  typedef otb::MRFEnergyFisherClassification<ImageType, LabelType> MRFFisherType;

  MRFFisherType::Pointer classif = MRFFisherType::New();

  classif->SetNumberOfParameters(6);
  MRFFisherType::ParametersType param(6);
  param[0] = 0;
  param[1] = 1;
  param[2] = 2;
  param[3] = 3;
  param[4] = 4;
  param[5] = 5;
  classif->SetParameters(param);
  std::ofstream file;
  file.open(outputFile);

  file << std::endl;
  file << "Number of parameters: " << classif->GetNumberOfParameters() << std::endl;
  file << "GetSingleValue(10, 1): " << classif->GetSingleValue(10, 1) << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
