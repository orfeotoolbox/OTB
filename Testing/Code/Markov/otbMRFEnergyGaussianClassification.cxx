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

#include "otbMRFEnergyGaussianClassification.h"
#include "otbImage.h"
#include <fstream>


int otbMRFEnergyGaussianClassification(int argc, char * argv[])
{
  const char * outputFile = argv[1];
  
  typedef double                                       PixelTypeInput;
  typedef int                                          PixelTypeLabel;
  typedef otb::Image<PixelTypeInput, 2>                ImageType;
  typedef otb::Image<PixelTypeLabel, 2>                LabelType;

  typedef otb::MRFEnergyGaussianClassification<ImageType, LabelType> MRFGaussianType;

  MRFGaussianType::Pointer classif = MRFGaussianType::New();

 
  std::ofstream file;
  file.open(outputFile);

  file<<std::endl;
  file<<"Number of paramters: "<<classif->GetNumberOfParameters()<<std::endl;
  file<<"GetSingleValue(10, 1): "<<classif->GetSingleValue(10, 1)<<std::endl;

  file.close();

  return EXIT_SUCCESS;
}

