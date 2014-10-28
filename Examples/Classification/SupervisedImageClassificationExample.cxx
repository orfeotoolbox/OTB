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
// In OTB, a generic streamed filter called \doxygen{otb}{ImageClassificationFilter}
// is available to classify any input multi-channel image according to an input
// classification model file. This filter is generic because it works with any
// classification model type (SVM, KNN, Artificial Neural Network,...) generated
// within the OTB generic Machine Learning framework based on OpenCV (\cite{opencv_library}).
// The input model file is smartly parsed according to its content in order to
// identify which learning method was used to generate it. Once the classification
// method and model are known, the input image can be classified. More details are
// given in subsections \ref{ssec:LearningFromSamples} and \ref{ssec:LearningFromImages}
// to generate a classification model either from samples or from images.
// In this example we will illustrate its use. We start by including the
// appropriate header files.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbMachineLearningModelFactory.h"
#include "otbImageClassificationFilter.h"
// Software Guide : EndCodeSnippet

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int itkNotUsed(argc), char * argv[])
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
  const unsigned int     Dimension = 2;
  typedef double         PixelType;
  typedef unsigned short LabeledPixelType;
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// Our classifier is generic enough to be able to process images
// with any number of bands. We read the input image as a
// \doxygen{otb}{VectorImage}. The labeled image will be a scalar image.
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
  typedef otb::ImageClassificationFilter<ImageType, LabeledImageType>
                                                          ClassificationFilterType;
  typedef ClassificationFilterType::ModelType ModelType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// Moreover, it is necessary to define a \doxygen{otb}{MachineLearningModelFactory}
// which is templated over its input and output pixel types. This factory is used
// to parse the input model file and to define which classification method to use.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef otb::MachineLearningModelFactory<PixelType, LabeledPixelType>
                                                   MachineLearningModelFactoryType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// And finally, we define the reader and the writer. Since the images
// to classify can be very big, we will use a streamed writer which
// will trigger the streaming ability of the classifier.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<ImageType>        ReaderType;
  typedef otb::ImageFileWriter<LabeledImageType> WriterType;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We instantiate the classifier and the reader objects and we set
// the existing model obtained in a previous training step.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The input model file is parsed according to its content and the generated model
// is then loaded within the \doxygen{otb}{ImageClassificationFilter}.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
  ModelType::Pointer model;
  model = MachineLearningModelFactoryType::CreateMachineLearningModel(
                                        modelfname,
                                        MachineLearningModelFactoryType::ReadMode);
  model->Load(modelfname);

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
