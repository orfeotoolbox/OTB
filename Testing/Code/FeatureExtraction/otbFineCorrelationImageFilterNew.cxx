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
#include "otbFineCorrelationImageFilter.h"


int otbFineCorrelationImageFilterNew( int argc, char * argv[] )
{
  typedef double      PixelType;

  typedef itk::FixedArray<PixelType>                                          DeformationValueType;
  typedef otb::Image< PixelType>                                              ImageType;
  typedef otb::Image<DeformationValueType,2>                                  FieldImageType;
  typedef otb::FineCorrelationImageFilter<ImageType,ImageType,FieldImageType> CorrelationFilterType;
  
  CorrelationFilterType::Pointer correlation = CorrelationFilterType::New();

  return EXIT_SUCCESS;
}
