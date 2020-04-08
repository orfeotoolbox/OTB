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
./Pipeline Input/QB_Suburb.png Output/TutorialsPipelineOutput.png
*/


//  Start by including some necessary headers and with the
//  usual \code{main} declaration:

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <input_filename> <output_filename>" << std::endl;
  }

  //  Declare the image as an \doxygen{otb}{Image}, the pixel type
  // is declared as an unsigned char (one byte) and the image is specified as
  // having two dimensions.

  using ImageType = otb::Image<unsigned char, 2>;

  //  To read the image, we need an \doxygen{otb}{ImageFileReader}
  // which is templated with the image type.

  using ReaderType           = otb::ImageFileReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();

  // Then, we need an \doxygen{otb}{ImageFileWriter}
  // also templated with the image type.

  using WriterType           = otb::ImageFileWriter<ImageType>;
  WriterType::Pointer writer = WriterType::New();

  // The filenames are passed as arguments to the program. We keep it
  // simple for now and we don't check their validity.

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  // Now that we have all the elements, we connect the pipeline,
  // pluging the output of the reader to the input of the writer.

  writer->SetInput(reader->GetOutput());

  // And finally, we trigger the pipeline execution calling the Update()
  // method on the last element of the pipeline. The last element will make
  // sure to update all previous elements in the pipeline.

  writer->Update();

  return EXIT_SUCCESS;
}
