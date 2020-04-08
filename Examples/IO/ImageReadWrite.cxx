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


//  The classes responsible for reading and writing images are located at the
//  beginning and end of the data processing pipeline. These classes are
//  known as data sources (readers) and data sinks (writers).
//  Generally speaking they are referred to as filters, although readers have
//  no pipeline input and writers have no pipeline output.
//
//  The reading of images is managed by the class \doxygen{otb}{ImageFileReader}
//  while writing is performed by the class \doxygen{otb}{ImageFileWriter}. These
//  two classes are independent of any particular file format. The actual low
//  level task of reading and writing specific file formats is done behind
//  the scenes by a family of classes of type
//  \doxygen{itk}{ImageIO}. Actually, the OTB image Readers and
//  Writers are very similar to those of ITK, but provide new
//  functionnalities which are specific to remote sensing images.
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
  //  represent the image processed by the pipeline. Note that when reading
  //  and writing images, the pixel type of the image \textbf{is not
  //  necessarily} the same as the pixel type stored in the file.  Your
  //  choice of the pixel type (and hence template parameter) should be
  //  driven mainly by two considerations:
  //
  //  \begin{itemize}
  //  \item It should be possible to cast the file pixel type in the file to
  //  the pixel type you select. This casting will be performed using the
  //  standard C-language rules, so you will have to make sure that the
  //  conversion does not result in information being lost.
  //  \item The pixel type in memory should be appropriate to the type of
  //  processing you intended to apply on the images.
  //  \end{itemize}
  //
  //  A typical selection for remote sensing images is illustrated in
  //  the following lines.

  using PixelType              = unsigned short;
  const unsigned int Dimension = 2;
  using ImageType              = otb::Image<PixelType, Dimension>;

  //  Note that the dimension of the image in memory should match the one of
  //  the image in file. There are a couple of special cases in which this
  //  condition may be relaxed, but in general it is better to ensure that both
  //  dimensions match. This is not a real issue in remote sensing,
  //  unless you want to consider multi-band images as volumes (3D) of data.
  //
  //  We can now instantiate the types of the reader and writer. These two
  //  classes are parameterized over the image type.
  //
  //  \index{otb::ImageFileReader!Instantiation}
  //  \index{otb::ImageFileWriter!Instantiation}

  using ReaderType = otb::ImageFileReader<ImageType>;
  using WriterType = otb::ImageFileWriter<ImageType>;

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
  //  pipeline. For example, we can create a short pipeline by passing
  //  the output of the reader directly to the input of the writer.

  writer->SetInput(reader->GetOutput());

  //  At first view, this may seem as a quite useless program, but it is
  //  actually implementing a powerful file format conversion tool! The
  //  execution of the pipeline is triggered by the invocation of the
  //  \code{Update()} methods in one of the final objects. In this case, the final
  //  data pipeline object is the writer. It is a wise practice of defensive
  //  programming to insert any \code{Update()} call inside a \code{try/catch} block
  //  in case exceptions are thrown during the execution of the pipeline.

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

  //  Note that exceptions should only be caught by pieces of code that know
  //  what to do with them. In a typical application this \code{catch} block
  //  should probably reside on the GUI code. The action on the \code{catch}
  //  block could inform the user about the failure of the IO operation.
  //
  //  The IO architecture of the toolkit makes it possible to avoid explicit
  //  specification of the file format used to read or write
  //  images.\footnote{In this example no file format is specified; this
  //  program can be used as a general file conversion utility.}  The object
  //  factory mechanism enables the ImageFileReader and ImageFileWriter to
  //  determine (at run-time) with which file format it is working
  //  with. Typically, file formats are chosen based on the filename
  //  extension, but the architecture supports arbitrarily complex processes
  //  to determine whether a file can be read or written. Alternatively, the
  //  user can specify the data file format by explicit instantiation and
  //  assignment the appropriate \doxygen{itk}{ImageIO} subclass.
  //

  return EXIT_SUCCESS;
}
