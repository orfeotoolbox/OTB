/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#include <iostream>

//  Software Guide : BeginLatex
//
//  This example shows how to read a list of images and concatenate
//  them into a vector image. We will write a program which is able to
//  perform this operation taking advantage of the streaming
//  functionnalities of the processing pipeline. We will assume that
//  all the input images have the same size and a single band.
//
//  The following header files will be needed:
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbImageFileWriter.h"
// Software Guide : EndCodeSnippet

int main(int argc, char** argv)
{
  if (argc < 4)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << "outputImage image1 image2 ... " << std::endl;
    }

  const unsigned int NbImages = argc - 2;

  std::cout << "Concat of " << NbImages <<
  " images into a multi-band image " <<
  std::endl;

//  Software Guide : BeginLatex
//
//  We will start by defining the types for the input images and the
//  associated readers.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef unsigned short int PixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<PixelType, Dimension> InputImageType;

  typedef otb::ImageFileReader<InputImageType> ImageReaderType;
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We will use a list of image file readers in order to open all the
//  input images at once. For this, we use the
//  \doxygen{otb}{ObjectList} object and we template it over the type
//  of the readers.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::ObjectList<ImageReaderType> ReaderListType;

  ReaderListType::Pointer readerList = ReaderListType::New();
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We will also build a list of input images in order to store the
//  smart pointers obtained at the output of each reader. This allows
//  us to build a pipeline without really reading the images and using
//  lots of RAM. The \doxygen{otb}{ImageList} object will be used.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::ImageList<InputImageType> ImageListType;

  ImageListType::Pointer imageList = ImageListType::New();
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We can now loop over the input image list in order to populate the
//  reader list and the input image list.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  for (unsigned int i = 0; i < NbImages; ++i)
    {

    ImageReaderType::Pointer imageReader = ImageReaderType::New();

    imageReader->SetFileName(argv[i + 2]);

    std::cout << "Adding image " << argv[i + 2] << std::endl;

    imageReader->UpdateOutputInformation();

    imageList->PushBack(imageReader->GetOutput());

    readerList->PushBack(imageReader);

    }
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  All the input images will be concatenated into a single output
//  vector image. For this matter, we will use the
//  \doxygen{otb}{ImageListToVectorImageFilter} which is templated
//  over the input image list type and the output vector image type.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<PixelType, Dimension> VectorImageType;

  typedef otb::ImageListToVectorImageFilter<ImageListType, VectorImageType>
  ImageListToVectorImageFilterType;

  ImageListToVectorImageFilterType::Pointer iL2VI =
    ImageListToVectorImageFilterType::New();
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We plug the image list as input of the filter and use a
//  \doxygen{otb}{ImageFileWriter} to write the result image
//  to a file, so that the streaming capabilities of all the readers
//  and the filter are used.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  iL2VI->SetInput(imageList);

  typedef otb::ImageFileWriter<VectorImageType> ImageWriterType;

  ImageWriterType::Pointer imageWriter = ImageWriterType::New();

  imageWriter->SetFileName(argv[1]);
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We can tune the size of the image tiles, so that the
//  total memory footprint of the pipeline is constant
//  for any execution of the program.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  unsigned long memoryConsumptionInMB = 10;

  std::cout << "Memory consumption: " << memoryConsumptionInMB << std::endl;

  imageWriter->SetAutomaticTiledStreaming(memoryConsumptionInMB);

  imageWriter->SetInput(iL2VI->GetOutput());

  imageWriter->Update();
// Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
