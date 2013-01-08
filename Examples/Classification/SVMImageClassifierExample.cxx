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

// Software Guide : BeginLatex
//
// In previous examples, we have used the
// \doxygen{otb}{SVMClassifier}, which uses the ITK classification
// framework. This good for compatibility with the ITK framework, but
// introduces the limitations of not being able to use streaming and
// being able to know at compilation time the number of bands of the
// image to be classified. In OTB we have avoided this limitation by
// developing the \doxygen{otb}{SVMImageClassificationFilter}. In
// this example we will illustrate its use. We start by including the
// appropriate header file.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbSVMImageClassificationFilter.h"
// Software Guide : EndCodeSnippet

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * modelfname = argv[2];
  const char * outfname = argv[3];

// Software Guide : BeginLatex
//
// We will assume double precision input images and will also define
// the type for the labeled pixels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Our classifier will be generic enough to be able to process images
// with any number of bands. We read the images as
// \doxygen{otb}{VectorImage}s. The labeled image will be a scalar image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::VectorImage<PixelType, Dimension>  ImageType;
  typedef otb::Image<LabeledPixelType, Dimension> LabeledImageType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We can now define the type for the classifier filter, which is
// templated over its input and output image types.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SVMImageClassificationFilter
  <ImageType, LabeledImageType>  ClassificationFilterType;
  typedef ClassificationFilterType::ModelType ModelType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// And finally, we define the reader and the writer. Since the images
// to classify can be very big, we will use a streamed writer which
// will trigger the streaming ability of the classifier.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::ImageFileWriter<LabeledImageType> WriterType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We instantiate the classifier and the reader objects and we set
// the existing SVM model obtained in a previous training step.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);

  ModelType::Pointer model = ModelType::New();
  model->LoadModel(modelfname);

  filter->SetModel(model);
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We plug the pipeline and
// trigger its execution by updating the output of the writer.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();
// Software Guide : EndCodeSnippet
  return EXIT_SUCCESS;
}
