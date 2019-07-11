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
./ScalingPipeline Input/QB_Suburb.png Output/TutorialsScalingPipelineOutput.png
*/


//  This example illustrates the use of the
// \doxygen{itk}{RescaleIntensityImageFilter} to convert
// the result for proper display.
//
// We include the required header including the header
// for the \doxygen{itk}{CannyEdgeDetectionImageFilter} and the
// \doxygen{itk}{RescaleIntensityImageFilter}.

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>" << std::endl;
  }

  //  We need to declare two different image types, one for the internal
  // processing and one to output the results:

  using PixelType = double;
  using ImageType = otb::Image<PixelType, 2>;

  using OutputPixelType = unsigned char;
  using OutputImageType = otb::Image<OutputPixelType, 2>;

  //  We declare the reader with the image template using the pixel type
  // double. It is worth noticing that this instantiation does not imply
  // anything about the type of the input image. The original image can be
  // anything, the reader will just convert the result to double.
  //
  //  The writer is templated with the unsigned char image to be able to save
  // the result on one byte images (like png for example).

  using ReaderType           = otb::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();

  using WriterType           = otb::ImageFileWriter<OutputImageType>;
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  // Now we are declaring the edge detection filter which is going to work with
  // double input and output.

  using FilterType           = itk::CannyEdgeDetectionImageFilter<ImageType, ImageType>;
  FilterType::Pointer filter = FilterType::New();

  // Here comes the interesting part: we declare the
  // \doxygen{itk}{RescaleIntensityImageFilter}. The input
  // image type is the output type of the edge detection
  // filter. The output type is the same as the input type
  // of the writer.
  //
  // Desired minimum and maximum values for the output are
  // specified by the methods \code{SetOutputMinimum()} and
  // \code{SetOutputMaximum()}.
  //
  // This filter will actually rescale all the pixels of
  // the image but also cast the type of these pixels.

  using RescalerType             = itk::RescaleIntensityImageFilter<ImageType, OutputImageType>;
  RescalerType::Pointer rescaler = RescalerType::New();

  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  // Let's plug the pipeline:

  filter->SetInput(reader->GetOutput());
  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());

  // And finally, we trigger the pipeline execution calling the Update()
  // method on the writer

  writer->Update();

  return EXIT_SUCCESS;
}
