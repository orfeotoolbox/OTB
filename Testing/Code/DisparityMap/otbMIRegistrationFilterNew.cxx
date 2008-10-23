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
#include "otbMIRegistrationFilter.h"
#include "otbImage.h"

int otbMIRegistrationFilterNew(int argc, char* argv[])
{
  const unsigned int ImageDimension = 2;

  typedef double                                PixelType;
  typedef itk::Vector<double,ImageDimension>    DeformationPixelType;

  typedef double                   CoordinateRepresentationType;

  //Allocate Images
  typedef otb::Image<PixelType,ImageDimension>         MovingImageType;
  typedef otb::Image<PixelType,ImageDimension>         FixedImageType;
  typedef otb::Image<DeformationPixelType,
                               ImageDimension>         DeformationFieldType;

  //Create the filter
  typedef otb::MIRegistrationFilter< FixedImageType, 
                                       MovingImageType,
                                       DeformationFieldType >
                                           RegistrationFilterType;

  // Instantiation
  RegistrationFilterType::Pointer registrator = RegistrationFilterType::New();
  
 return EXIT_SUCCESS;
}
