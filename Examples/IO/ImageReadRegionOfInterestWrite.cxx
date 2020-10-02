/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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


//  This example should arguably be placed in the filtering
//  chapter. However its usefulness for typical IO operations makes it
//  interesting to mention here. The purpose of this example is to read and
//  image, extract a subregion and write this subregion to a file. This is a
//  common task when we want to apply a computationally intensive method to
//  the region of interest of an image.
//
//  As usual with OTB IO, we begin by including the appropriate header files.

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

//  The \doxygen{otb}{ExtractROI} is the filter used to extract a
//  region from an image. Its header is included below.
//
//  \index{otb::ExtractROI!header}

#include "otbExtractROI.h"


int main(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  if (argc < 7)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    std::cerr << " startX startY sizeX sizeY" << std::endl;
    return EXIT_FAILURE;
  }

  //  Image types are defined below.

  using InputPixelType         = unsigned char;
  using OutputPixelType        = unsigned char;
  const unsigned int Dimension = 2;
  using InputImageType         = otb::Image<InputPixelType, Dimension>;
  using OutputImageType        = otb::Image<OutputPixelType, Dimension>;

  //  The types for the \doxygen{otb}{ImageFileReader} and \doxygen{otb}{ImageFileWriter}
  //  are instantiated using the image types.

  using ReaderType = otb::ImageFileReader<InputImageType>;
  using WriterType = otb::ImageFileWriter<OutputImageType>;

  //  The ExtractROI type is instantiated using
  //  the input and output pixel types. Using the pixel types as
  //  template parameters instead of the image types allows
  //  restricting the use of this class to \doxygen{otb}{Image}s which
  //  are used with scalar pixel types. See section
  //  \ref{sec:ExtractROI} for the extraction of ROIs on
  //  \doxygen{otb}{VectorImage}s. A filter object is created with the
  //  New() method and assigned to a \doxygen{itk}{SmartPointer}.

  using FilterType = otb::ExtractROI<InputImageType::PixelType, OutputImageType::PixelType>;

  FilterType::Pointer filter = FilterType::New();

  //  The ExtractROI requires a region to be
  //  defined by the user. This is done by defining a rectangle with
  //  the following methods (the filter assumes that a 2D image is
  //  being processed, for N-D region extraction, you can use the
  //  \doxygen{itk}{RegionOfInterestImageFilter} class).

  filter->SetStartX(atoi(argv[3]));
  filter->SetStartY(atoi(argv[4]));
  filter->SetSizeX(atoi(argv[5]));
  filter->SetSizeY(atoi(argv[6]));

  //  Below, we create the reader and writer using the New() method and
  //  assigning the result to a SmartPointer.
  //
  //  \index{otb::ImageFileReader!New()}
  //  \index{otb::ImageFileWriter!New()}
  //  \index{otb::ImageFileReader!SmartPointer}
  //  \index{otb::ImageFileWriter!SmartPointer}

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  //
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

  //  Below we connect the reader, filter and writer to form the data
  //  processing pipeline.

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  //  Finally we execute the pipeline by invoking Update() on the writer. The
  //  call is placed in a \code{try/catch} block in case exceptions are
  //  thrown.

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
