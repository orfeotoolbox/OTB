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
#include "otbMultiChannelExtractROI.h"

int otbImageFileWriterWithExtendedOptionBox(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const std::string inputFilename = argv[1];
  const std::string outputFilename = argv[2];

  const unsigned int startx = atoi(argv[3]);
  const unsigned int starty = atoi(argv[4]);
  const unsigned int sizex = atoi(argv[5]);
  const unsigned int sizey = atoi(argv[6]);
  const unsigned int ram = atoi(argv[7]);
  const std::string separator = ":";

  typedef float InputPixelType;
  typedef otb::VectorImage<InputPixelType, 2> InputImageType;
  typedef otb::MultiChannelExtractROI<InputImageType::InternalPixelType,
                                      InputImageType::InternalPixelType> ExtractROIFilterType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;
  typedef otb::ImageFileWriter<InputImageType> WriterType;

  typedef itk::ImageRegionConstIterator< InputImageType >  ConstIteratorType;

  ReaderType::Pointer reader = ReaderType::New();
  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);

  //Build output filename with extended box
  std::ostringstream outputFilenameExtended;
  outputFilenameExtended
                         << outputFilename
                         << "?&box=" << startx << separator
                         << starty << separator
                         << sizex << separator
                         << sizey
;

  std::cout << "Output image with user defined path " << outputFilenameExtended.str() << std::endl;

  writer->SetFileName(outputFilenameExtended.str());
  writer->SetInput(reader->GetOutput());
  writer->SetAutomaticAdaptativeStreaming(ram);

  extractROIFilter->SetStartX(startx);
  extractROIFilter->SetStartY(starty);
  extractROIFilter->SetSizeX(sizex);
  extractROIFilter->SetSizeY(sizey);
  extractROIFilter->SetInput(reader->GetOutput());

  writer->Update();
  extractROIFilter->Update();

  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName(outputFilename);

  reader2->Update();

  InputImageType::ConstPointer readImage = reader2->GetOutput();
  InputImageType::ConstPointer extractImage = extractROIFilter->GetOutput();

  ConstIteratorType ritr( readImage, readImage->GetLargestPossibleRegion() );
  ConstIteratorType extractitr( extractImage, extractImage->GetLargestPossibleRegion() );

  ritr.GoToBegin();
  extractitr.GoToBegin();

  std::cout << "Comparing the pixel values.. :" << std::endl;

  while( !ritr.IsAtEnd() &&  !extractitr.IsAtEnd() )
    {
    if( ritr.Get() != extractitr.Get() )
      {
      std::cerr << "Pixel comparison failed at index = " << ritr.GetIndex() << std::endl;
      std::cerr << "Expected pixel value " << extractitr.Get() << std::endl;
      std::cerr << "Read Image pixel value " << ritr.Get() << std::endl;
      return EXIT_FAILURE;
      }
    ++extractitr;
    ++ritr;
    }

  std::cout << std::endl;
  std::cout << "Test PASSED !" << std::endl;

  return EXIT_SUCCESS;
}
