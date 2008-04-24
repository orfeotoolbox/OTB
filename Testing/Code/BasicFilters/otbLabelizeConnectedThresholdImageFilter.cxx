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

#include <iostream>

#include "otbLabelizeConnectedThresholdImageFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiplyByScalarImageFilter.h"

int otbLabelizeConnectedThresholdImageFilter( int argc, char * argv[] )
{
  // Arguments
  char* inputImageName = argv[1];
  char* outputImageName = argv[2];
  
  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;
  
  typedef otb::Image< InputPixelType, Dimension > InputImageType;
  typedef otb::Image< OutputPixelType, Dimension > OutputImageType;

  InputPixelType lowerThreshold( (InputPixelType)::atoi(argv[3]) );
  InputPixelType upperThreshold( (InputPixelType)::atoi(argv[4]) );
  InputPixelType deltaLower( (InputPixelType)::atoi(argv[5]) );
  InputPixelType deltaUpper( (InputPixelType)::atoi(argv[6]) );
  
  // Reader
  typedef otb::ImageFileReader<InputImageType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImageName);
  
  // Writer
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputImageName);
  
  // Labelize filter
  typedef otb::LabelizeConnectedThresholdImageFilter<InputImageType, OutputImageType> LabelizeFilterType;
  LabelizeFilterType::Pointer filter = LabelizeFilterType::New();
  
  filter->SetLowerThreshold(lowerThreshold);
  filter->SetUpperThreshold(upperThreshold);
  filter->SetLowerThresholdDelta(deltaLower);
  filter->SetUpperThresholdDelta(deltaUpper);
  
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->Update();
   
  return EXIT_SUCCESS;
}
