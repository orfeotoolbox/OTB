/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


/* Example usage:
./PrintableImageFilterExample Input/IMAGERY_SSECH.tif Output/PrintableExampleOutput1.jpg 1 2 3
*/


/* Example usage:
./PrintableImageFilterExample Input/IMAGERY_SSECH.tif Output/PrintableExampleOutput2.jpg 1 4 2
*/


#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"

int main(int argc, char* argv[])
{

  if (argc != 6)
  {
    std::cerr << "Usage: " << argv[0] << " <inputImageFile> ";
    std::cerr << " <outputImageFile> <RedBand> <GreenBand> <BlueBand>" << std::endl;
    return EXIT_FAILURE;
  }

  const char* inputFilename      = argv[1];
  const char* outputFilename     = argv[2];
  int         redChannelNumber   = atoi(argv[3]);
  int         greenChannelNumber = atoi(argv[4]);
  int         blueChannelNumber  = atoi(argv[5]);

  using InputPixelType         = double;
  const unsigned int Dimension = 2;

  using InputImageType = otb::VectorImage<InputPixelType, Dimension>;

  using ReaderType = otb::ImageFileReader<InputImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // To easily convert the image to a printable format, i.e. 3 bands
  // unsigned char value, you can use the PrintableImageFilter.
  using PrintableFilterType                         = otb::PrintableImageFilter<InputImageType>;
  PrintableFilterType::Pointer printableImageFilter = PrintableFilterType::New();

  printableImageFilter->SetInput(reader->GetOutput());
  printableImageFilter->SetChannel(redChannelNumber);
  printableImageFilter->SetChannel(greenChannelNumber);
  printableImageFilter->SetChannel(blueChannelNumber);

  // When you create the writer to plug at the output of the printableImageFilter
  // you may want to use the direct type definition as it is a good way to avoid mismatch:
  using OutputImageType = PrintableFilterType::OutputImageType;
  using WriterType      = otb::ImageFileWriter<OutputImageType>;

  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(printableImageFilter->GetOutput());

  writer->Update();
}
