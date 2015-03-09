/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//  Software Guide : BeginLatex
//
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
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbVectorImage.h"

int main(int argc, char * argv[])
{
  // Verify the number of parameters in the command line
  if (argc < 3)
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    return EXIT_FAILURE;
    }

  //  Software Guide : BeginLatex
  //
  //  Then, as usual, a decision must be made about the type of pixel used to
  //  represent the image processed by the pipeline. The pixel type
  //  corresponds to the scalar type stored in the vector
  //  components. Therefore, for a multiband Pl\'eiades image we will
  //  do:
  //
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef unsigned short PixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  We can now instantiate the types of the reader and writer. These two
  //  classes are parameterized over the image type.
  //
  //  \index{otb::ImageFileReader!Instantiation}
  //  \index{otb::ImageFileWriter!Instantiation}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Then, we create one object of each type using the New() method and
  //  assigning the result to a \doxygen{itk}{SmartPointer}.
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
  //  We can now connect these readers and writers to filters to create a
  //  pipeline. The only thig to take care of is, when executing the
  //  program, choosing an output image file format which supports
  //  multiband images.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  writer->SetInput(reader->GetOutput());
  // Software Guide : EndCodeSnippet

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
