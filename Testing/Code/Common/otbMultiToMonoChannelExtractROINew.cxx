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
#include "otbMultiToMonoChannelExtractROI.h"

int otbMultiToMonoChannelExtractROINew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  //        typedef unsigned char                                    InputPixelType;
  //        typedef unsigned char                                    OutputPixelType;

  typedef std::complex<float>  InputPixelType;
  typedef std::complex<double> OutputPixelType;

  typedef otb::MultiToMonoChannelExtractROI<InputPixelType,
      OutputPixelType>  ExtractROIFilterType;

  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();

  std::cout << extractROIFilter << std::endl;

  return EXIT_SUCCESS;
}
