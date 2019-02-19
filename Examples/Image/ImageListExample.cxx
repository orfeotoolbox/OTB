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


//  This example illustrates the use of the \doxygen{otb}{ImageList}
//  class. This class provides the functionnalities needed in order to
//  integrate image lists as data objects into the OTB
//  pipeline. Indeed, if a \code{std::list< ImageType >} was used, the
//  update operations on the pipeline might not have the desired
//  effects.
//
//  In this example, we will only present the basic operations which
//  can be applied on an \doxygen{otb}{ImageList} object.
//
//  The first thing required to read an image from a file is to include
//  the header file of the \doxygen{otb}{ImageFileReader} class.

#include "otbImageList.h"

#include "itkMacro.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

int main(int itkNotUsed(argc), char* argv[])
{

  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];

  // As usual, we start by defining the types for the pixel and image
  // types, as well as those for the readers and writers.

  const unsigned int                            Dimension = 2;
  typedef unsigned char                         InputPixelType;
  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ImageFileWriter<InputImageType>  WriterType;

  // We can now define the type for the image list. The
  // \doxygen{otb}{ImageList} class is templated over the type of image
  // contained in it. This means that all images in a list must have the
  // same type.

  typedef otb::ImageList<InputImageType> ImageListType;

  // Let us assume now that we want to read an image from a file and
  // store it in a list. The first thing to do is to instantiate the
  // reader and set the image file name. We effectively read the image
  // by calling the \code{Update()}.

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);
  reader->Update();

  // We create an image list by using the \code{New()} method.

  ImageListType::Pointer imageList = ImageListType::New();

  // In order to store the image in the list, the \code{PushBack()}
  // method is used.

  imageList->PushBack(reader->GetOutput());

  // We could repeat this operation for other readers or the outputs of
  // filters. We will now write an image of the list to a file. We
  // therefore instantiate a writer, set the image file name and set the
  // input image for it. This is done by calling the \code{Back()}
  // method of the list, which allows us to get the last element.

  // Getting the image from the list and writing it to file
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(outputFilename);
  writer->SetInput(imageList->Back());
  writer->Update();

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
  // \doxygen{otb}{ImageListToImageListFilter} is provided in order
  // to implement filter which operate on image lists and produce image lists.

  return EXIT_SUCCESS;
}
