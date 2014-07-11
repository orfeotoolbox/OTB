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
#include "otbVectorImage.h"
#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"

int otbListSampleToHistogramListGeneratorNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  typedef double                                       PixelType;
  typedef otb::VectorImage<PixelType>                  VectorImageType;
  typedef VectorImageType::PixelType                   VectorPixelType;
  typedef itk::Statistics::ListSample<VectorPixelType> ListSampleType;
  typedef otb::ListSampleToHistogramListGenerator
  <ListSampleType, PixelType>                         HistogramGeneratorType;

  // Instantiation
  HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();

  std::cout << generator << std::endl;

  return EXIT_SUCCESS;
}
