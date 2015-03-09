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



#include <iostream>
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtractROI.h"

int otbImageFileWriterONERAComplex(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  typedef std::complex<float> PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType,  Dimension> ImageType;
  typedef otb::ExtractROI<PixelType, PixelType>  ExtractROIFilterType;

  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer complexReader = ReaderType::New();
  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
  WriterType::Pointer complexWriter = WriterType::New();

  // Reader Parameters
  complexReader->SetFileName(inputFilename);

  // Extract ROI Parameters
  extractROIFilter->SetStartX(20);
  extractROIFilter->SetStartY(20);
  extractROIFilter->SetSizeX(100);
  extractROIFilter->SetSizeY(100);
  extractROIFilter->SetInput(complexReader->GetOutput());
  extractROIFilter->Update();

  // Writer Parameters
  complexWriter->SetFileName(outputFilename);
  complexWriter->SetInput(extractROIFilter->GetOutput());

  // Update the pipeline and Write the data
  complexWriter->Update();

  return EXIT_SUCCESS;
}
