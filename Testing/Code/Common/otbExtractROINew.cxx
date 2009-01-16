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


#include "itkExceptionObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"

#include "otbExtractROI.h"

int otbExtractROINew( int argc, char * argv[] )
{
  typedef std::complex<float>                                  InputPixelType;
  typedef std::complex<double>                          OutputPixelType;

  typedef otb::ExtractROI< InputPixelType, OutputPixelType >  FilterType;


  FilterType::Pointer filter = FilterType::New();


  return EXIT_SUCCESS;
}


