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
./TrainMachineLearningModelFromImagesExample Input/QB_1_ortho.tif Input/VectorData_QB1.shp Output/clLIBSVMModelQB1.libsvm
*/


// This example illustrates the use of the
// \doxygen{otb}{MachineLearningModel} class. This class allows the
// estimation of a classification model (supervised learning) from images. In this example, we will train an SVM
// with 4 classes. We start by including the appropriate header files.
// List sample generator
#include "otbListSampleGenerator.h"

// Extract a ROI of the vectordata
#include "otbVectorDataIntoImageProjectionFilter.h"

// SVM model Estimator
#include "otbSVMMachineLearningModel.h"


// Image
#include "otbVectorImage.h"
#include "otbVectorData.h"

// Reader
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"

// Normalize the samples
//#include "otbShiftScaleSampleListFilter.h"


int main(int itkNotUsed(argc), char* argv[])
{
  const char* inputImageFileName  = argv[1];
  const char* trainingShpFileName = argv[2];
  const char* outputModelFileName = argv[3];

  using InputPixelType         = unsigned int;
  const unsigned int Dimension = 2;
  using InputImageType         = otb::VectorImage<InputPixelType, Dimension>;
  using VectorDataType         = otb::VectorData<double, 2>;
  using InputReaderType        = otb::ImageFileReader<InputImageType>;
  using VectorDataReaderType   = otb::VectorDataFileReader<VectorDataType>;

  // In this framework, we must transform the input samples store in a vector
  // data into a \subdoxygen{itk}{Statistics}{ListSample} which is the structure
  // compatible with the machine learning classes. On the one hand, we are using feature vectors
  // for the characterization of the classes, and on the other hand, the class labels
  // are scalar values. We first re-project the input vector data over the input image, using the
  // \doxygen{otb}{VectorDataIntoImageProjectionFilter} class. To convert the
  // input samples store in a vector data into a
  // \subdoxygen{itk}{Statistics}{ListSample}, we use the
  // \doxygen{otb}{ListSampleGenerator} class.

  // VectorData projection filter
  using VectorDataReprojectionType = otb::VectorDataIntoImageProjectionFilter<VectorDataType, InputImageType>;

  InputReaderType::Pointer inputReader = InputReaderType::New();
  inputReader->SetFileName(inputImageFileName);

  InputImageType::Pointer image = inputReader->GetOutput();
  image->UpdateOutputInformation();

  // Read the Vectordata
  VectorDataReaderType::Pointer vectorReader = VectorDataReaderType::New();
  vectorReader->SetFileName(trainingShpFileName);
  vectorReader->Update();

  VectorDataType::Pointer vectorData = vectorReader->GetOutput();
  vectorData->Update();

  VectorDataReprojectionType::Pointer vdreproj = VectorDataReprojectionType::New();

  vdreproj->SetInputImage(image);
  vdreproj->SetInput(vectorData);
  vdreproj->SetUseOutputSpacingAndOriginFromImage(false);

  vdreproj->Update();

  using ListSampleGeneratorType = otb::ListSampleGenerator<InputImageType, VectorDataType>;
  ListSampleGeneratorType::Pointer sampleGenerator;
  sampleGenerator = ListSampleGeneratorType::New();

  sampleGenerator->SetInput(image);
  sampleGenerator->SetInputVectorData(vdreproj->GetOutput());
  sampleGenerator->SetClassKey("Class");

  sampleGenerator->Update();


  // std::cout << "Number of classes: " << sampleGenerator->GetNumberOfClasses() << std::endl;

  // using ListSampleType = ListSampleGeneratorType::ListSampleType;
  // using ShiftScaleFilterType = otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType>;

  // // Shift scale the samples
  // ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
  // trainingShiftScaleFilter->SetInput(concatenateTrainingSamples->GetOutput());
  // trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
  // trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
  // trainingShiftScaleFilter->Update();


  // Now, we need to declare the machine learning model which will be used by the
  // classifier. In this example, we train an SVM model. The
  // \doxygen{otb}{SVMMachineLearningModel} class inherits from the pure virtual
  // class \doxygen{otb}{MachineLearningModel} which is templated over the type of
  // values used for the measures and the type of pixels used for the labels. Most
  // of the classification and regression algorithms available through this
  // interface in OTB is based on the OpenCV library \cite{opencv_library}. Specific methods
  // can be used to set classifier parameters. In the case of SVM, we set here the type
  // of the kernel. Other parameters are let with their default values.

  using SVMType = otb::SVMMachineLearningModel<InputImageType::InternalPixelType, ListSampleGeneratorType::ClassLabelType>;

  SVMType::Pointer SVMClassifier = SVMType::New();

  SVMClassifier->SetInputListSample(sampleGenerator->GetTrainingListSample());
  SVMClassifier->SetTargetListSample(sampleGenerator->GetTrainingListLabel());

  SVMClassifier->SetKernelType(CvSVM::LINEAR);

  // The machine learning interface is generic and gives access to other classifiers. We now train the
  // SVM model using the \code{Train} and save the model to a text file using the
  // \code{Save} method.

  SVMClassifier->Train();
  SVMClassifier->Save(outputModelFileName);


  // You can now use the \code{Predict} method which takes a
  // \subdoxygen{itk}{Statistics}{ListSample} as input and estimates the label of each
  // input sample using the model. Finally, the
  // \doxygen{otb}{ImageClassificationModel} inherits from the
  // \doxygen{itk}{ImageToImageFilter} and allows classifying pixels in the
  // input image by predicting their labels using a model.
}
