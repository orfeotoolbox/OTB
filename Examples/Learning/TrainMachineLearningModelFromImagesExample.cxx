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

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_1_ortho.tif}, {VectorData_QB1.shp}
//    OUTPUTS: {clLIBSVMModelQB1.libsvm}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the use of the
// \doxygen{otb}{MachineLearningModel} class. This class allows the
// estimation of a classification model (supervised learning) from images. In this example, we will train an SVM
// with 4 classes. We start by including the appropriate header files.
//
//  Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
// List sample generator
#include "otbListSampleGenerator.h"

// Extract a ROI of the vectordata
#include "otbVectorDataIntoImageProjectionFilter.h"

// SVM model Estimator
#include "otbSVMMachineLearningModel.h"
// Software Guide : EndCodeSnippet


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
  const char* inputImageFileName = argv[1];
  const char* trainingShpFileName = argv[2];
  const char* outputModelFileName = argv[3];

  typedef unsigned int InputPixelType;
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::VectorData<double, 2>                   VectorDataType;
  typedef otb::ImageFileReader<InputImageType>         InputReaderType;
  typedef otb::VectorDataFileReader<VectorDataType>    VectorDataReaderType;

// Software Guide : BeginLatex
//
// In this framework, we must transform the input samples store in a vector
// data into a \subdoxygen{itk}{Statistics}{ListSample} which is the structure
// compatible with the machine learning classes. On the one hand, we are using feature vectors
// for the characterization of the classes, and on the other hand, the class labels
// are scalar values. We first re-project the input vector data over the input image, using the
// \doxygen{otb}{VectorDataIntoImageProjectionFilter} class. To convert the
// input samples store in a vector data into a
// \subdoxygen{itk}{Statistics}{ListSample}, we use the
// \doxygen{otb}{ListSampleGenerator} class.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  // VectorData projection filter
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, InputImageType>
                                                        VectorDataReprojectionType;

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

  typedef otb::ListSampleGenerator<InputImageType, VectorDataType>
                                                           ListSampleGeneratorType;
  ListSampleGeneratorType::Pointer sampleGenerator;
  sampleGenerator = ListSampleGeneratorType::New();

  sampleGenerator->SetInput(image);
  sampleGenerator->SetInputVectorData(vdreproj->GetOutput());
  sampleGenerator->SetClassKey("Class");

  sampleGenerator->Update();
  // Software Guide : EndCodeSnippet


  //std::cout << "Number of classes: " << sampleGenerator->GetNumberOfClasses() << std::endl;

  // typedef ListSampleGeneratorType::ListSampleType ListSampleType;
  // typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  // // Shift scale the samples
  // ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
  // trainingShiftScaleFilter->SetInput(concatenateTrainingSamples->GetOutput());
  // trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
  // trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
  // trainingShiftScaleFilter->Update();


// Software Guide : BeginLatex
//
// Now, we need to declare the machine learning model which will be used by the
// classifier. In this example, we train an SVM model. The
// \doxygen{otb}{SVMMachineLearningModel} class inherits from the pure virtual
// class \doxygen{otb}{MachineLearningModel} which is templated over the type of
// values used for the measures and the type of pixels used for the labels. Most
// of the classification and regression algorithms available through this
// interface in OTB is based on the OpenCV library \cite{opencv_library}. Specific methods
// can be used to set classifier parameters. In the case of SVM, we set here the type
// of the kernel. Other parameters are let with their default values.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::SVMMachineLearningModel
                                 <InputImageType::InternalPixelType,
                                  ListSampleGeneratorType::ClassLabelType> SVMType;

  SVMType::Pointer SVMClassifier = SVMType::New();

  SVMClassifier->SetInputListSample(sampleGenerator->GetTrainingListSample());
  SVMClassifier->SetTargetListSample(sampleGenerator->GetTrainingListLabel());

  SVMClassifier->SetKernelType(CvSVM::LINEAR);
  // Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
//
// The machine learning interface is generic and gives access to other classifiers. We now train the
// SVM model using the \code{Train} and save the model to a text file using the
// \code{Save} method.
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  SVMClassifier->Train();
  SVMClassifier->Save(outputModelFileName);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex

// You can now use the \code{Predict} method which takes a
// \subdoxygen{itk}{Statistics}{ListSample} as input and estimates the label of each
// input sample using the model. Finally, the
// \doxygen{otb}{ImageClassificationModel} inherits from the
// \doxygen{itk}{ImageToImageFilter} and allows classifying pixels in the
// input image by predicting their labels using a model.
//
// Software Guide : EndLatex

}
