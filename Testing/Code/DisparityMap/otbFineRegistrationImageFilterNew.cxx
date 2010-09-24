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

#include "otbImage.h"
#include "itkFixedArray.h"
#include "otbFineRegistrationImageFilter.h"


int otbFineRegistrationImageFilterNew( int argc, char * argv[] )
{
  typedef double      PixelType;
  const unsigned int  Dimension = 2;

  typedef itk::FixedArray<PixelType,Dimension>                                 DeformationValueType;
  typedef otb::Image< PixelType>                                               ImageType;
  typedef otb::Image<DeformationValueType,2>                                   FieldImageType;
  typedef otb::FineRegistrationImageFilter<ImageType,ImageType,FieldImageType> RegistrationFilterType;
  
  RegistrationFilterType::Pointer filter = RegistrationFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
