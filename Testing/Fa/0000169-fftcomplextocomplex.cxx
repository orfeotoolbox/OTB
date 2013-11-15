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

#include "itkMacro.h"
#include "otbImage.h"
#include "itkFFTComplexToComplexImageFilter.h"

int main(int argc, char * argv[])
{
  typedef otb::Image< std::complex<float>, 2 > ImageType;
  typedef itk::FFTComplexToComplexImageFilter <ImageType> FFTFilterType;

  FFTFilterType::Pointer fftFilter =    FFTFilterType::New();
  fftFilter->DebugOn();

  return EXIT_SUCCESS;
}
