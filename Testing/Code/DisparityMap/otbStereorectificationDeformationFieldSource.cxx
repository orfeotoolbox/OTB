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
#include "itkVector.h"
#include "otbImage.h"
#include "otbStereorectificationDeformationFieldSource.h"

typedef itk::Vector<double,2>       DeformationType;
typedef otb::Image<DeformationType> DeformationFieldType;
typedef otb::Image<unsigned short>  ImageType;
typedef otb::StereorectificationDeformationFieldSource
   <ImageType,DeformationFieldType> DeformationFieldSourceType;


int otbStereorectificationDeformationFieldSourceNew(int argc, char * argv[])
{
  // Instantiation
  DeformationFieldSourceType::Pointer dfSource = DeformationFieldSourceType::New();

  return EXIT_SUCCESS;
}
