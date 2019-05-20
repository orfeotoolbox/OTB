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


//  Given that \href{http://www.itk.org}{ITK} and OTB are based on the Generic
//  Programming paradigm, most of the types are defined at compilation
//  time. It is sometimes important to anticipate conversion between different
//  types of images. The following example illustrates the common case of
//  reading an image of one pixel type and writing it on a different pixel
//  type. This process not only involves casting but also rescaling the image
//  intensity since the dynamic range of the input and output pixel types can
//  be quite different.  The \doxygen{itk}{RescaleIntensityImageFilter} is used
//  here to linearly rescale the image values.
//
//  The first step in this example is to include the appropriate headers.
//
//  \index{otb::ImageFileReader!header}
//  \index{otb::ImageFileWriter!header}
//  \index{itk::RescaleIntensityImageFilter!header}

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

#include "otbImage.h"

int main(int argc, char* argv[])
{
  // Verify the number of parameters in the command line
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    return EXIT_FAILURE;
  }

  //  Then, as usual, a decision should be made about the pixel type that
  //  should be used to represent the images. Note that when reading an
  //  image, this pixel type \textbf{is not necessarily} the pixel type of
  //  the image stored in the file.  Instead, it is the type that will be
  //  used to store the image as soon as it is read into memory.

  using InputPixelType         = float;
  using OutputPixelType        = unsigned char;
  const unsigned int Dimension = 2;

  using InputImageType  = otb::Image<InputPixelType, Dimension>;
  using OutputImageType = otb::Image<OutputPixelType, Dimension>;

  //  We can now instantiate the types of the reader and writer. These two
  //  classes are parameterized over the image type.
  //
  //  \index{otb::ImageFileReader!Instantiation}
  //  \index{otb::ImageFileWriter!Instantiation}

  using ReaderType = otb::ImageFileReader<InputImageType>;
  using WriterType = otb::ImageFileWriter<OutputImageType>;

  //  Below we instantiate the RescaleIntensityImageFilter class that will
  //  linearly scale the image intensities.

  using FilterType = itk::RescaleIntensityImageFilter<InputImageType, OutputImageType>;

  //  A filter object is constructed and the minimum and maximum values of
  //  the output are selected using the SetOutputMinimum() and
  //  SetOutputMaximum() methods.
  //
  //  \index{itk::RescaleIntensityImageFilter!SetOutputMinimum()}
  //  \index{itk::RescaleIntensityImageFilter!SetOutputMaximum()}

  FilterType::Pointer filter = FilterType::New();
  filter->SetOutputMinimum(0);
  filter->SetOutputMaximum(255);

  //  Then, we create the reader and writer and connect the pipeline.
  //
  //  \index{otb::ImageFileReader!New()}
  //  \index{otb::ImageFileWriter!New()}
  //  \index{otb::ImageFileReader!SmartPointer}
  //  \index{otb::ImageFileWriter!SmartPointer}

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());

  //
  // Here we recover the file names from the command line arguments
  //
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  //  The name of the files to be read and written are passed with the
  //  SetFileName() method.
  //
  //  \index{otb::ImageFileReader!SetFileName()}
  //  \index{otb::ImageFileWriter!SetFileName()}
  //  \index{SetFileName()!otb::ImageFileReader}
  //  \index{SetFileName()!otb::ImageFileWriter}

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  //  Finally we trigger the execution of the pipeline with the Update()
  //  method on the writer. The output image will then be the scaled and cast
  //  version of the input image.

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
