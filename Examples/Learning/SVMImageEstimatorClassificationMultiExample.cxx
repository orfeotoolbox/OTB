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



//  Software Guide : BeginCommandLineArgs
//    INPUTS: {ROI_QB_MUL_1.png}, {ROI_mask_multi.png}
//    OUTPUTS: {ROI_QB_MUL_1_SVN_CLASS_MULTI.png}, {ROI_QB_MUL_1_SVN_CLASS_MULTI_Rescaled.jpg}
//    NORMALIZE_EPS_OUTPUT_OF: {ROI_mask_multi.png}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the OTB's multi-class SVM
// capabilities. The theory behind this kind of classification is out
// of the scope of this guide. In OTB, the multi-class SVM
// classification is used in the same way as the two-class
// one. Figure~\ref{fig:SVMROISMULTI} shows the image to be classified
// and the associated ground truth, which is composed of 4 classes.
// \begin{figure}
// \center
// \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1.eps}
// \includegraphics[width=0.45\textwidth]{ROI_mask_multi.eps}
// \itkcaption[SVM Image Model Estimation]{Images used for the
// estimation of the SVM model. Left: RGB image. Right: image of labels.}
// \label{fig:SVMROISMULTI}
// \end{figure}

// The following header files are needed for the program:

//  Software Guide : EndLatex

#include "itkMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <iostream>

//  Software Guide : BeginCodeSnippet
#include "otbLibSVMMachineLearningModel.h"
#include "itkImageToListSampleFilter.h"
#include "otbImageClassificationFilter.h"
//  Software Guide : EndCodeSnippet

#include "otbImageFileWriter.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkBinaryThresholdImageFilter.h"

#include "otbImageFileReader.h"

int main(int itkNotUsed(argc), char *argv[])
{

  const char* inputImageFileName = argv[1];
  const char* trainingImageFileName = argv[2];
  const char* outputImageFileName = argv[3];
  const char* outputRescaledImageFileName = argv[4];
//  const char* outputModelFileName = argv[4];

//  Software Guide : BeginLatex
//
//  We define the types for the input and training images. Even if the
//  input image will be an RGB image, we can read it as a 3 component
//  vector image. This simplifies the interfacing with OTB's SVM
//  framework.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  typedef unsigned short InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::VectorImage<InputPixelType, Dimension> InputImageType;

  typedef otb::Image<InputPixelType,  Dimension> TrainingImageType;
//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The \doxygen{otb}{LibSVMMachineLearningModel} class is templated over
//  the input (features) and the training (labels) values.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  typedef otb::LibSVMMachineLearningModel<InputPixelType,
      InputPixelType>   ModelType;

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  As usual, we define the readers for the images.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  typedef otb::ImageFileReader<InputImageType>    InputReaderType;
  typedef otb::ImageFileReader<TrainingImageType> TrainingReaderType;

  InputReaderType::Pointer    inputReader = InputReaderType::New();
  TrainingReaderType::Pointer trainingReader = TrainingReaderType::New();

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We read the images. It is worth to note that, in order to ensure
//  the pipeline coherence, the output of the objects which precede the
//  model estimator in the pipeline, must be up to date, so we call
//  the corresponding \code{Update} methods.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  inputReader->SetFileName(inputImageFileName);
  trainingReader->SetFileName(trainingImageFileName);

  //~ inputReader->Update();
  //~ trainingReader->Update();

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The input data is contained in images. Only label values greater than 0
//  shall be used, so we create two iterators to fill the input and target
//  ListSamples.
//
// Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet

  typedef itk::BinaryThresholdImageFilter<TrainingImageType,TrainingImageType> ThresholdFilterType;
  ThresholdFilterType::Pointer thresholder = ThresholdFilterType::New();
  thresholder->SetInput(trainingReader->GetOutput());
  thresholder->SetLowerThreshold(1);
  thresholder->SetOutsideValue(0);
  thresholder->SetInsideValue(1);

  typedef itk::Statistics::ImageToListSampleFilter<InputImageType,TrainingImageType> ImageToListSample;
  typedef itk::Statistics::ImageToListSampleFilter<TrainingImageType,TrainingImageType> ImageToTargetListSample;

  ImageToListSample::Pointer imToList = ImageToListSample::New();
  imToList->SetInput(inputReader->GetOutput());
  imToList->SetMaskImage(thresholder->GetOutput());
  imToList->SetMaskValue(1);
  imToList->Update();

  ImageToTargetListSample::Pointer imToTargetList = ImageToTargetListSample::New();
  imToTargetList->SetInput(trainingReader->GetOutput());
  imToTargetList->SetMaskImage(thresholder->GetOutput());
  imToTargetList->SetMaskValue(1);
  imToTargetList->Update();

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  We can now instantiate the model and set its parameters.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  ModelType::Pointer svmModel = ModelType::New();
  svmModel->SetInputListSample(const_cast<ModelType::InputListSampleType*>(imToList->GetOutput()));
  svmModel->SetTargetListSample(const_cast<ModelType::TargetListSampleType*>(imToTargetList->GetOutput()));

//  Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  The model training procedure is triggered by calling the
//  model's \code{Train} method.
//
// Software Guide : EndLatex
//  Software Guide : BeginCodeSnippet
  svmModel->Train();

//  Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We have now all the elements to create a classifier. The classifier
// is templated over the sample type (the type of the data to be
// classified) and the label type (the type of the output of the classifier).
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::ImageClassificationFilter<InputImageType, TrainingImageType> ClassifierType;

  ClassifierType::Pointer classifier = ClassifierType::New();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We set the classifier parameters : number of classes, SVM model,
// the sample data. And we trigger the classification process by
// calling the \code{Update} method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  classifier->SetModel(svmModel);
  classifier->SetInput(inputReader->GetOutput());
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// After the classification step, we usually want to get the
// results. The classifier gives an output under the form of a sample
// list. This list supports the classical STL iterators. Therefore, we
// will create an output image and fill it up with the results of the
// classification. The pixel type of the output image is the same as
// the one used for the labels.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet

// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We allocate the memory for the output image using the information
// from the input image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet

// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now declare the iterators on the list that we get at the
// output of the classifier as well as the iterator to fill the output image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet

// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We will iterate through the list, get the labels and assign pixel
// values to the output image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet

// Software Guide : EndCodeSnippet

  typedef otb::ImageFileWriter<TrainingImageType> WriterType;

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(outputImageFileName);
  writer->SetInput(classifier->GetOutput());

  writer->Update();

// Software Guide : BeginLatex
//
// Only for visualization purposes, we choose a color mapping to the image of
// classes before saving it to a file. The
// \subdoxygen{itk}{Functor}{ScalarToRGBPixelFunctor} class is a special
// function object designed to hash a scalar value into an
// \doxygen{itk}{RGBPixel}. Plugging this functor into the
// \doxygen{itk}{UnaryFunctorImageFilter} creates an image filter for that
// converts scalar images to RGB images.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef otb::Image<RGBPixelType, 2>  RGBImageType;
  typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long>
  ColorMapFunctorType;
  typedef itk::UnaryFunctorImageFilter<TrainingImageType,
      RGBImageType,
      ColorMapFunctorType> ColorMapFilterType;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();

  colormapper->SetInput(classifier->GetOutput());
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// We can now create an image file writer and save the image.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::ImageFileWriter<RGBImageType> WriterRescaledType;

  WriterRescaledType::Pointer writerRescaled = WriterRescaledType::New();

  writerRescaled->SetFileName(outputRescaledImageFileName);
  writerRescaled->SetInput(colormapper->GetOutput());

  writerRescaled->Update();
// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
// Figure \ref{fig:SVMCLASSMULTI} shows the result of the SVM classification.
// \begin{figure}
// \center
// \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1.eps}
// \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1_SVN_CLASS_MULTI_Rescaled.eps}
// \itkcaption[SVM Image Classification]{Result of the SVM
// classification . Left: RGB image. Right: image of classes.}
// \label{fig:SVMCLASSMULTI}
// \end{figure}
//  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
