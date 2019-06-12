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


/* Example usage:
./SVMImageEstimatorClassificationMultiExample Input/ROI_QB_MUL_1.png \
                                              Input/ROI_mask_multi.png \
                                              Output/ROI_QB_MUL_1_SVN_CLASS_MULTI.png \
                                              Output/ROI_QB_MUL_1_SVN_CLASS_MULTI_Rescaled.jpg \
                                              Output/ROI_mask_multi.png
*/


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


#include "itkMacro.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include <iostream>

#include "otbLibSVMMachineLearningModel.h"
#include "itkImageToListSampleFilter.h"
#include "otbImageClassificationFilter.h"

#include "otbImageFileWriter.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkBinaryThresholdImageFilter.h"

#include "otbImageFileReader.h"

int main(int itkNotUsed(argc), char* argv[])
{

  const char* inputImageFileName          = argv[1];
  const char* trainingImageFileName       = argv[2];
  const char* outputImageFileName         = argv[3];
  const char* outputRescaledImageFileName = argv[4];
  //  const char* outputModelFileName = argv[4];

  //  We define the types for the input and training images. Even if the
  //  input image will be an RGB image, we can read it as a 3 component
  //  vector image. This simplifies the interfacing with OTB's SVM
  //  framework.
  using InputPixelType         = unsigned short;
  const unsigned int Dimension = 2;

  using InputImageType = otb::VectorImage<InputPixelType, Dimension>;

  using TrainingImageType = otb::Image<InputPixelType, Dimension>;

  //  The \doxygen{otb}{LibSVMMachineLearningModel} class is templated over
  //  the input (features) and the training (labels) values.
  using ModelType = otb::LibSVMMachineLearningModel<InputPixelType, InputPixelType>;


  //  As usual, we define the readers for the images.
  using InputReaderType    = otb::ImageFileReader<InputImageType>;
  using TrainingReaderType = otb::ImageFileReader<TrainingImageType>;

  InputReaderType::Pointer    inputReader    = InputReaderType::New();
  TrainingReaderType::Pointer trainingReader = TrainingReaderType::New();


  //  We read the images. It is worth to note that, in order to ensure
  //  the pipeline coherence, the output of the objects which precede the
  //  model estimator in the pipeline, must be up to date, so we call
  //  the corresponding \code{Update} methods.
  inputReader->SetFileName(inputImageFileName);
  trainingReader->SetFileName(trainingImageFileName);

  //~ inputReader->Update();
  //~ trainingReader->Update();


  //  The input data is contained in images. Only label values greater than 0
  //  shall be used, so we create two iterators to fill the input and target
  //  ListSamples.


  using ThresholdFilterType                = itk::BinaryThresholdImageFilter<TrainingImageType, TrainingImageType>;
  ThresholdFilterType::Pointer thresholder = ThresholdFilterType::New();
  thresholder->SetInput(trainingReader->GetOutput());
  thresholder->SetLowerThreshold(1);
  thresholder->SetOutsideValue(0);
  thresholder->SetInsideValue(1);

  using ImageToListSample       = itk::Statistics::ImageToListSampleFilter<InputImageType, TrainingImageType>;
  using ImageToTargetListSample = itk::Statistics::ImageToListSampleFilter<TrainingImageType, TrainingImageType>;

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


  //  We can now instantiate the model and set its parameters.
  ModelType::Pointer svmModel = ModelType::New();
  svmModel->SetInputListSample(const_cast<ModelType::InputListSampleType*>(imToList->GetOutput()));
  svmModel->SetTargetListSample(const_cast<ModelType::TargetListSampleType*>(imToTargetList->GetOutput()));


  //  The model training procedure is triggered by calling the
  //  model's \code{Train} method.
  svmModel->Train();


  // We have now all the elements to create a classifier. The classifier
  // is templated over the sample type (the type of the data to be
  // classified) and the label type (the type of the output of the classifier).

  using ClassifierType = otb::ImageClassificationFilter<InputImageType, TrainingImageType>;

  ClassifierType::Pointer classifier = ClassifierType::New();

  // We set the classifier parameters : number of classes, SVM model,
  // the sample data. And we trigger the classification process by
  // calling the \code{Update} method.

  classifier->SetModel(svmModel);
  classifier->SetInput(inputReader->GetOutput());

  // After the classification step, we usually want to get the
  // results. The classifier gives an output under the form of a sample
  // list. This list supports the classical STL iterators. Therefore, we
  // will create an output image and fill it up with the results of the
  // classification. The pixel type of the output image is the same as
  // the one used for the labels.


  // We allocate the memory for the output image using the information
  // from the input image.


  // We can now declare the iterators on the list that we get at the
  // output of the classifier as well as the iterator to fill the output image.


  // We will iterate through the list, get the labels and assign pixel
  // values to the output image.


  using WriterType = otb::ImageFileWriter<TrainingImageType>;

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(outputImageFileName);
  writer->SetInput(classifier->GetOutput());

  writer->Update();

  // Only for visualization purposes, we choose a color mapping to the image of
  // classes before saving it to a file. The
  // \subdoxygen{itk}{Functor}{ScalarToRGBPixelFunctor} class is a special
  // function object designed to hash a scalar value into an
  // \doxygen{itk}{RGBPixel}. Plugging this functor into the
  // \doxygen{itk}{UnaryFunctorImageFilter} creates an image filter for that
  // converts scalar images to RGB images.

  using RGBPixelType                      = itk::RGBPixel<unsigned char>;
  using RGBImageType                      = otb::Image<RGBPixelType, 2>;
  using ColorMapFunctorType               = itk::Functor::ScalarToRGBPixelFunctor<unsigned long>;
  using ColorMapFilterType                = itk::UnaryFunctorImageFilter<TrainingImageType, RGBImageType, ColorMapFunctorType>;
  ColorMapFilterType::Pointer colormapper = ColorMapFilterType::New();

  colormapper->SetInput(classifier->GetOutput());

  // We can now create an image file writer and save the image.

  using WriterRescaledType = otb::ImageFileWriter<RGBImageType>;

  WriterRescaledType::Pointer writerRescaled = WriterRescaledType::New();

  writerRescaled->SetFileName(outputRescaledImageFileName);
  writerRescaled->SetInput(colormapper->GetOutput());

  writerRescaled->Update();

  // Figure \ref{fig:SVMCLASSMULTI} shows the result of the SVM classification.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1.eps}
  // \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1_SVN_CLASS_MULTI_Rescaled.eps}
  // \itkcaption[SVM Image Classification]{Result of the SVM
  // classification . Left: RGB image. Right: image of classes.}
  // \label{fig:SVMCLASSMULTI}
  // \end{figure}

  return EXIT_SUCCESS;
}
