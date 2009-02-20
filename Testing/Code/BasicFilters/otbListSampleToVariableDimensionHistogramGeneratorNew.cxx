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
#include "otbListSampleToVariableDimensionHistogramGenerator.h"

int otbListSampleToVariableDimensionHistogramGeneratorNew(int argc, char * argv[])
{
 
  typedef double                                       PixelType;
  typedef otb::VectorImage<PixelType>                  VectorImageType;
  typedef VectorImageType::PixelType                   VectorPixelType;
  typedef itk::Statistics::ListSample<VectorPixelType> ListSampleType;
  typedef otb::ListSampleToVariableDimensionHistogramGenerator
    <ListSampleType,PixelType>                         HistogramGeneratorType;

  // Instantiation
  HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();

  return EXIT_SUCCESS;
}
