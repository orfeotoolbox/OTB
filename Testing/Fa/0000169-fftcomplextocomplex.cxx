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
#include "itkFFTComplexToComplexImageFilter.h"

int main(int argc, char * argv[])
{
  typedef itk::FFTComplexToComplexImageFilter <float, 2> FFTFilterType;

  FFTFilterType::Pointer fftFilter =    FFTFilterType::New();
  fftFilter->DebugOn();

  return EXIT_SUCCESS;
}
