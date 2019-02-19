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


//  The \doxygen{otb}{Image} class with a vector pixel type could be
//  used for representing multispectral images, with one band per
//  vector component, however, this is not a practical way, since the
//  dimensionality of the vector must be known at compile time. OTB
//  offers the \doxygen{otb}{VectorImage} where the dimensionality of
//  the vector stored for each pixel can be chosen at runtime. This is
//  needed for the image file readers in order to dynamically set the
//  number of bands of an image read from a file.
//
//  The OTB Readers and Writers are able to deal with
//  \doxygen{otb}{VectorImage}s transparently for the user.
//
//  The first step for performing reading and writing is to include the
//  following headers.
//
//  \index{otb::ImageFileReader}
//  \index{otb::ImageFileReader!header}
//
//  \index{otb::ImageFileWriter}
//  \index{otb::ImageFileWriter!header}

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbImage.h"
#include "otbVectorImage.h"

int main(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    return EXIT_FAILURE;
  }

  //  Then, as usual, a decision must be made about the type of pixel used to
  //  represent the image processed by the pipeline. The pixel type
  //  corresponds to the scalar type stored in the vector
  //  components. Therefore, for a multiband Pl\'eiades image we will
  //  do:
  //

  typedef unsigned short                         PixelType;
  const unsigned int                             Dimension = 2;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;

  //  We can now instantiate the types of the reader and writer. These two
  //  classes are parameterized over the image type.
  //
  //  \index{otb::ImageFileReader!Instantiation}
  //  \index{otb::ImageFileWriter!Instantiation}

  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  //  Then, we create one object of each type using the New() method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.
  //
  //  \index{otb::ImageFileReader!New()}
  //  \index{otb::ImageFileWriter!New()}
  //  \index{otb::ImageFileReader!SmartPointer}
  //  \index{otb::ImageFileWriter!SmartPointer}

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  // Here we recover the file names from the command line arguments
  //
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  //  The name of the file to be read or written is passed with the
  //  SetFileName() method.
  //
  //  \index{otb::ImageFileReader!SetFileName()}
  //  \index{otb::ImageFileWriter!SetFileName()}
  //  \index{SetFileName()!otb::ImageFileReader}
  //  \index{SetFileName()!otb::ImageFileWriter}

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  //  We can now connect these readers and writers to filters to create a
  //  pipeline. The only thig to take care of is, when executing the
  //  program, choosing an output image file format which supports
  //  multiband images.

  writer->SetInput(reader->GetOutput());

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
