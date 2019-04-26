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


/* Example usage:
./LeeImageFilter Input/GomaSmall.png Output/GomaSmallLeeFiltered.png 3 1
*/

#include "otbLeeImageFilter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile radius NbLooks" << std::endl;
    return EXIT_FAILURE;
  }

  typedef unsigned char PixelType;

  // The images are defined using the pixel type and the dimension.
  typedef otb::Image<PixelType, 2> InputImageType;
  typedef otb::Image<PixelType, 2> OutputImageType;

  // The filter can be instantiated using the image types defined above.
  typedef otb::LeeImageFilter<InputImageType, OutputImageType> FilterType;

  // An ImageFileReader class is also instantiated in order to read
  // image data from a file.
  typedef otb::ImageFileReader<InputImageType> ReaderType;

  // An ImageFileWriter is instantiated in order to write the
  // output image to a file.
  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  // Both the filter and the reader are created by invoking their New()
  // methods and assigning the result to SmartPointers.
  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  reader->SetFileName(argv[1]);

  // The image obtained with the reader is passed as input to the
  // LeeImageFilter.
  filter->SetInput(reader->GetOutput());

  // The method SetRadius() defines the size of the window to
  // be used for the computation of the local statistics. The method
  // SetNbLooks() sets the number of looks of the input
  // image.
  FilterType::SizeType Radius;
  Radius[0] = atoi(argv[3]);
  Radius[1] = atoi(argv[3]);

  filter->SetRadius(Radius);
  filter->SetNbLooks(atoi(argv[4]));

  writer->SetFileName(argv[2]);
  writer->Update();
}
