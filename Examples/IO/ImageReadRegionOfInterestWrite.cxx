/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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



//  Software Guide : BeginLatex
//
//  This example should arguably be placed in the filtering
//  chapter. However its usefulness for typical IO operations makes it
//  interesting to mention here. The purpose of this example is to read and
//  image, extract a subregion and write this subregion to a file. This is a
//  common task when we want to apply a computationally intensive method to
//  the region of interest of an image.
//
//  As usual with OTB IO, we begin by including the appropriate header files.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The \doxygen{otb}{ExtractROI} is the filter used to extract a
//  region from an image. Its header is included below.
//
//  \index{otb::ExtractROI!header}
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbExtractROI.h"
// Software Guide : EndCodeSnippet


int main(int argc, char * argv[])
{
  // Verify the number of parameters in the command line
  if (argc < 7)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    std::cerr << " startX startY sizeX sizeY" << std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  //  Image types are defined below.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef unsigned char InputPixelType;
  typedef unsigned char OutputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::Image<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The types for the \doxygen{otb}{ImageFileReader} and \doxygen{otb}{ImageFileWriter}
  //  are instantiated using the image types.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The ExtractROI type is instantiated using
  //  the input and output pixel types. Using the pixel types as
  //  template parameters instead of the image types allows
  //  restricting the use of this class to \doxygen{otb}{Image}s which
  //  are used with scalar pixel types. See section
  //  \ref{sec:ExtractROI} for the extraction of ROIs on
  //  \doxygen{otb}{VectorImage}s. A filter object is created with the
  //  New() method and assigned to a \doxygen{itk}{SmartPointer}.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ExtractROI<InputImageType::PixelType,
      OutputImageType::PixelType> FilterType;

  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  The ExtractROI requires a region to be
  //  defined by the user. This is done by defining a rectangle with
  //  the following methods (the filter assumes that a 2D image is
  //  being processed, for N-D region extraction, you can use the
  //  \doxygen{itk}{RegionOfInterestImageFilter} class).
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetStartX(atoi(argv[3]));
  filter->SetStartY(atoi(argv[4]));
  filter->SetSizeX(atoi(argv[5]));
  filter->SetSizeY(atoi(argv[6]));
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Below, we create the reader and writer using the New() method and
  //  assigning the result to a SmartPointer.
  //
  //  \index{otb::ImageFileReader!New()}
  //  \index{otb::ImageFileWriter!New()}
  //  \index{otb::ImageFileReader!SmartPointer}
  //  \index{otb::ImageFileWriter!SmartPointer}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  //
  // Here we recover the file names from the command line arguments
  //
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  //  Software Guide : BeginLatex
  //
  //  The name of the file to be read or written is passed with the
  //  SetFileName() method.
  //
  //  \index{otb::ImageFileReader!SetFileName()}
  //  \index{otb::ImageFileWriter!SetFileName()}
  //  \index{SetFileName()!otb::ImageFileReader}
  //  \index{SetFileName()!otb::ImageFileWriter}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Below we connect the reader, filter and writer to form the data
  //  processing pipeline.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Finally we execute the pipeline by invoking Update() on the writer. The
  //  call is placed in a \code{try/catch} block in case exceptions are
  //  thrown.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
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
  // Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
