/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */




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
