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

//Image
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbListSampleGenerator.h"

//Reader
#include "otbImageFileReader.h"
#include "otbVectorDataFileReader.h"

//Estimator
# include "otbSVMMachineLearningModel.h"
#include "otbMachineLearningModelFactory.h"

// Normalize the samples
#include "otbShiftScaleSampleListFilter.h"

// Extract a ROI of the vectordata
#include "otbVectorDataIntoImageProjectionFilter.h"

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {Classification/QB_1_ortho.tif}, {VectorData_QB1.shp}
//    OUTPUTS: {clLIBSVMModelQB1.libsvm}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the use of the
// \doxygen{otb}{MachineLearningModel} class. This class allows the
// estimation of a classification model (supervised learning) from samples. In this example, we will train an SVM
// to separate between water and non-water pixels by using the RGB
// values only. The images used for this example are shown in
// figure~\ref{fig:SVMROIS}.
// \begin{figure}
// \center
// \includegraphics[width=0.45\textwidth]{ROI_QB_MUL_1.eps}
// \includegraphics[width=0.45\textwidth]{ROI_mask.eps}
// \itkcaption[SVM Image Model Estimation]{Images used for the
// estimation of the SVM model. Left: RGB image. Right: image of labels.}
// \label{fig:SVMROIS}
// \end{figure}
// The first thing to do is include the header file for the class.
//
//  Software Guide : EndLatex

int main(int argc, char* argv[])
{

  const char* inputImageFileName = argv[1];
  const char* trainingShpFileName = argv[2];
  const char* outputModelFileName = argv[3];

  typedef unsigned int InputPixelType;
  const unsigned int Dimension = 2;

  

  typedef otb::VectorImage<InputPixelType,  Dimension> InputImageType;
  typedef otb::Image<InputImageType::InternalPixelType, 2> ImageType;

  typedef otb::VectorData<double, 2>                   VectorDataType;
  
  typedef otb::ImageFileReader<InputImageType>    InputReaderType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;

  // VectorData projection filter
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, InputImageType> VectorDataReprojectionType;

  InputReaderType::Pointer    inputReader = InputReaderType::New();
  inputReader->SetFileName(inputImageFileName);
  
  InputImageType::Pointer image = inputReader->GetOutput();
  image->UpdateOutputInformation();

  // read the Vectordata
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

  typedef otb::ListSampleGenerator<InputImageType, VectorDataType> ListSampleGeneratorType;
  ListSampleGeneratorType::Pointer sampleGenerator = ListSampleGeneratorType::New();

  sampleGenerator->SetInput(image);
  sampleGenerator->SetInputVectorData(vdreproj->GetOutput());
  sampleGenerator->SetClassKey("Class");

  sampleGenerator->Update();

  std::cout << "Number of classes: " << sampleGenerator->GetNumberOfClasses() << std::endl;

  // typedef ListSampleGeneratorType::ListSampleType ListSampleType;
  // typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  // // Shift scale the samples
  // ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
  // trainingShiftScaleFilter->SetInput(concatenateTrainingSamples->GetOutput());
  // trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
  // trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
  // trainingShiftScaleFilter->Update();

  ListSampleGeneratorType::ListSamplePointerType samples = sampleGenerator->GetTrainingListSample();
  ListSampleGeneratorType::ListLabelPointerType  labels = sampleGenerator->GetTrainingListLabel();

  typedef otb::SVMMachineLearningModel<InputImageType::InternalPixelType, ListSampleGeneratorType::ClassLabelType> SVMType;

  SVMType::Pointer SVMClassifier = SVMType::New();
  SVMClassifier->SetInputListSample(samples);
  SVMClassifier->SetTargetListSample(labels);

  SVMClassifier->SetKernelType(CvSVM::LINEAR);

  SVMClassifier->Train();
  SVMClassifier->Save(outputModelFileName);
}

