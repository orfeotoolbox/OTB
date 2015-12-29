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
//  This example illustrates the use of the \subdoxygen{otb}{ImageList}
//  class. This class provides the functionnalities needed in order to
//  integrate image lists as data objects into the OTB
//  pipeline. Indeed, if a \code{std::list< ImageType >} was used, the
//  update operations on the pipeline might not have the desired
//  effects.
//
//  In this example, we will only present the basic operations which
//  can be applied on an \subdoxygen{otb}{ImageList} object.
//
//  The first thing required to read an image from a file is to include
//  the header file of the \subdoxygen{otb}{ImageFileReader} class.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImageList.h"
// Software Guide : EndCodeSnippet

#include "itkMacro.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int main(int itkNotUsed(argc), char * argv[])
{

  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];

// Software Guide : BeginLatex
//
// As usual, we start by defining the types for the pixel and image
// types, as well as those for the readers and writers.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef unsigned char                         InputPixelType;
  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<InputImageType>  WriterType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now define the type for the image list. The
// \subdoxygen{otb}{ImageList} class is templated over the type of image
// contained in it. This means that all images in a list must have the
// same type.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::ImageList<InputImageType> ImageListType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Let us assume now that we want to read an image from a file and
// store it in a list. The first thing to do is to instantiate the
// reader and set the image file name. We effectively read the image
// by calling the \code{Update()}.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We create an image list by using the \code{New()} method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ImageListType::Pointer imageList = ImageListType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// In order to store the image in the list, the \code{PushBack()}
// method is used.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  imageList->PushBack(reader->GetOutput());
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We could repeat this operation for other readers or the outputs of
// filters. We will now write an image of the list to a file. We
// therefore instantiate a writer, set the image file name and set the
// input image for it. This is done by calling the \code{Back()}
// method of the list, which allows us to get the last element.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
// Getting the image from the list and writing it to file
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(imageList->Back());
  writer->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Other useful methods are:
// \begin{itemize}
// \item \code{SetNthElement()} and \code{GetNthElement()} allow
// randomly accessing any element of the list.
// \item \code{Front()} to access to the first element of the list.
// \item \code{Erase()} to remove an element.
// \end{itemize}
//
// Also, iterator classes are defined in order to have an efficient
// mean of moving through the list. Finally, the
// \subdoxygen{otb}{ImageListToImageListFilter} is provided in order
// to implement filter which operate on image lists and produce image lists.
// Software Guide : EndLatex

  return EXIT_SUCCESS;
}
