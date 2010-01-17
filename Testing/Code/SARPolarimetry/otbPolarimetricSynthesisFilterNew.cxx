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

#include "itkExceptionObject.h"
#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbPolarimetricSynthesisFilter.h"

int otbPolarimetricSynthesisFilterNew( int argc, char * argv[] )
{
  typedef std::complex<double> InputPixelType;
  typedef double           OutputPixelType;
  const   unsigned int         Dimension = 2;

  typedef otb::Image< InputPixelType,  Dimension >        InputImageType;
  typedef otb::Image< OutputPixelType, Dimension >        OutputImageType;

  typedef otb::PolarimetricSynthesisFilter< InputImageType,InputImageType,InputImageType,InputImageType,OutputImageType >   FilterType;

  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}


