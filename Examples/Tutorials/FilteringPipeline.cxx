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
./FilteringPipeline Input/QB_Suburb.png Output/TutorialsFilteringPipelineOutput.png
*/


//
//  We are going to use the \doxygen{itk}{GradientMagnitudeImageFilter}
// to compute the gradient of the image. The beginning of the file is
// similar to the Pipeline.cxx.
//
// We include the required headers, without forgetting to add the header
// for the \doxygen{itk}{GradientMagnitudeImageFilter}.

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkGradientMagnitudeImageFilter.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>" << std::endl;
  }

  //  We declare the image type, the reader and the writer as
  //  before:

  using ImageType = otb::Image<unsigned char, 2>;

  using ReaderType           = otb::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();

  using WriterType           = otb::ImageFileWriter<ImageType>;
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  // Now we have to declare the filter. It is templated with the
  // input image type and the output image type like many filters
  // in OTB. Here we are using the same type for the input and the
  // output images:

  using FilterType           = itk::GradientMagnitudeImageFilter<ImageType, ImageType>;
  FilterType::Pointer filter = FilterType::New();

  // Let's plug the pipeline:

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  // And finally, we trigger the pipeline execution calling the \code{Update()}
  // method on the writer

  writer->Update();

  return EXIT_SUCCESS;
}
