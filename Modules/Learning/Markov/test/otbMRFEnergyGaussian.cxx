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


#include "otbMRFEnergyGaussian.h"
#include "otbImage.h"
#include <fstream>

int otbMRFEnergyGaussian(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef double                    PixelType;
  typedef int                       PixelType2;
  typedef otb::Image<PixelType, 2>  ImageType;
  typedef otb::Image<PixelType2, 2> LabelType;

  typedef otb::MRFEnergyGaussian<ImageType, LabelType>  MRFEnergyGaussianType;
  typedef MRFEnergyGaussianType::InputImagePixelType    InputImagePixelType;
  typedef MRFEnergyGaussianType::LabelledImagePixelType LabelledImagePixelType;

  MRFEnergyGaussianType::Pointer object    = MRFEnergyGaussianType::New();

  InputImagePixelType    inPix = 10;
  LabelledImagePixelType inLab = 5;

  if (object->GetSingleValue(inPix, inLab) != 25)
    {
    return EXIT_FAILURE;
    }

  inPix = 5;
  inLab = 10;
  if (object->GetSingleValue(inPix, inLab) != 25)
    {
    return EXIT_FAILURE;
    }

  inPix = 0;
  inLab = 0;
  if (object->GetSingleValue(inPix, inLab) != 0)
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
