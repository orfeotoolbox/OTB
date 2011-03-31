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
#include "otbTrainImagesClassifier.h"

#include <iostream>
#include "otbCommandLineArgumentParser.h"

//Image 
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageList.h"
#include "otbImageFileReader.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbListSampleGenerator.h"
//#include "otbTypeManager.h"

// ListSample
#include "itkListSample.h"
#include "itkVariableLengthVector.h"
#include "itkFixedArray.h"

// SVM estimator
#include "otbSVMSampleListModelEstimator.h"

// Statistic XML Reader
#include "otbStatisticsXMLFileReader.h"

// Validation
#include "otbSVMClassifier.h"
#include "otbConfusionMatrixCalculator.h"

#include "itkTimeProbe.h"
#include "otbStandardFilterWatcher.h"

// Normalize the samples
#include "otbShiftScaleSampleListFilter.h"

// List sample concatenation
#include "otbConcatenateSampleListFilter.h"

// Balancing ListSample
#include "otbListSampleToBalancedListSampleFilter.h"

namespace otb
{

int TrainImagesClassifier::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("TrainImagesClassifier");
  descriptor->SetDescription("Perform SVM training from multiple input images and multiple vector data.");
  descriptor->AddOptionNParams("InputImages", "A list of images", 
                               "in", true, ApplicationDescriptor::InputImage);
  descriptor->AddOptionNParams("VectorDataSamples", "Vector Data of sample used to train the estimator", 
                               "vd", true, ApplicationDescriptor::FileName);
  descriptor->AddOption("ImagesStatistics", "XML file containing mean and variance of input images.",
                        "is", 1, false, ApplicationDescriptor::FileName);
  descriptor->AddOption("Output", "Output SVM model",
                        "out", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("Margin", "Margin for SVM learning.",
                        "m", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("Balancing", "Balance and grow the training set.",
                        "b", 1, false, ApplicationDescriptor::Integer);
  return EXIT_SUCCESS;
}

int TrainImagesClassifier::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Input Image
  //typedef double                                          ValueType;
  //typedef unsigned short                                  PixelType; // def by manuel
  typedef float                                           PixelType;
  typedef otb::VectorImage<PixelType,2>                   VectorImageType;
  typedef otb::Image<PixelType,2>                         ImageType;
  typedef otb::ImageFileReader<VectorImageType>           ReaderType;
  typedef otb::ImageList<ImageType>                       ImageListType;
  typedef otb::VectorImageToImageListFilter
      <VectorImageType, ImageListType>                    VI2ILFilterType;

  // Training vectordata
  typedef otb::VectorData<>                               VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType>       VectorDataReaderType;
  typedef itk::VariableLengthVector<PixelType>            MeasurementType;

  // SampleList manipulation
  typedef otb::ListSampleGenerator<VectorImageType, VectorDataType> ListSampleGeneratorType;

  typedef ListSampleGeneratorType::ListSampleType           ListSampleType;
  typedef ListSampleGeneratorType::LabelType                LabelType;
  typedef ListSampleGeneratorType::ListLabelType            LabelListSampleType;
  typedef otb::Statistics::ConcatenateSampleListFilter
      <ListSampleType>                                      ConcatenateListSampleFilterType;
  typedef otb::Statistics::ConcatenateSampleListFilter
      <LabelListSampleType>                                 ConcatenateLabelListSampleFilterType;

  // Statistic XML file Reader 
  typedef otb::StatisticsXMLFileReader<MeasurementType>     StatisticsReader;

  typedef otb::Statistics::ListSampleToBalancedListSampleFilter<
      ListSampleType, LabelListSampleType>                  BalancingListSampleFilterType;

  typedef otb::Statistics::ShiftScaleSampleListFilter<
      ListSampleType, ListSampleType>                       ShiftScaleFilterType;

  // SVM Estimator
  //typedef itk::Statistics::ListSample<MeasurementType>    ListSampleType2;

  typedef otb::Functor::VariableLengthVectorToMeasurementVectorFunctor<
      MeasurementType>                                      MeasurementVectorFunctorType;
  typedef otb::SVMSampleListModelEstimator<
      ListSampleType,
      LabelListSampleType,
      MeasurementVectorFunctorType>                         SVMEstimatorType;
  typedef otb::SVMClassifier<ListSampleType, LabelType::ValueType>     ClassifierType;
  typedef otb::ConfusionMatrixCalculator<LabelListSampleType,
      LabelListSampleType>                                  ConfusionMatrixCalculatorType;
  typedef ClassifierType::OutputType ClassifierOutputType;

  //Create training and validation for list samples and label list samples
  ConcatenateLabelListSampleFilterType::Pointer concatenateTrainingLabels =
                                                  ConcatenateLabelListSampleFilterType::New();
  ConcatenateListSampleFilterType::Pointer concatenateTrainingSamples =
                                             ConcatenateListSampleFilterType::New();
  ConcatenateLabelListSampleFilterType::Pointer concatenateValidationLabels =
                                                  ConcatenateLabelListSampleFilterType::New();
  ConcatenateListSampleFilterType::Pointer concatenateValidationSamples =
                                            ConcatenateListSampleFilterType::New();

  //Set the size of training and validation set
  //TODO use default parameters of the filter?
  const long int maxTrainingSize = 100;
  const long int maxValidationSize = 100;
  const double validationTrainingProportion = 0.5;

  unsigned int nbBands = 0;
  //Iterate over all input images
  for(int imgIndex = 0; imgIndex<parseResult->GetNumberOfParameters("InputImages");++imgIndex)
    {
    // Read the image
    ReaderType::Pointer    reader  = ReaderType::New();
    reader->SetFileName(parseResult->GetParameterString("InputImages",imgIndex));
    reader->UpdateOutputInformation();

    std::cout<<"Processing image ("<<imgIndex<<"): "<<reader->GetFileName()<<std::endl;

    if (imgIndex == 0)
      {
      nbBands = reader->GetOutput()->GetNumberOfComponentsPerPixel();
      }

    // read the Vectordata
    VectorDataReaderType::Pointer vdreader = VectorDataReaderType::New();
    vdreader->SetFileName(parseResult->GetParameterString("VectorDataSamples",imgIndex));
    vdreader->Update();
    std::cout<<"Set VectorData filename: "<< parseResult->GetParameterString("VectorDataSamples",imgIndex) <<std::endl;

    //Sample list generator
    ListSampleGeneratorType::Pointer sampleGenerator = ListSampleGeneratorType::New();

    //Set inputs of the sample generator
    //TODO the ListSampleGenerator perform UpdateOutputData over the input image (need a persistent implementation)
    sampleGenerator->SetInput(reader->GetOutput());
    sampleGenerator->SetInputVectorData(vdreader->GetOutput());
    sampleGenerator->SetMaxTrainingSize(maxTrainingSize);
    sampleGenerator->SetMaxValidationSize(maxValidationSize);
    sampleGenerator->SetValidationTrainingProportion(validationTrainingProportion);

    sampleGenerator->SetClassKey("Class");

    sampleGenerator->Update();

    std::cout << "output size: " << sampleGenerator->GetTrainingListLabel()->GetMeasurementVectorSize() <<std::endl;

    //Concatenate training and validation samples from the image
    concatenateTrainingLabels->AddInput(sampleGenerator->GetTrainingListLabel());
    concatenateTrainingSamples->AddInput(sampleGenerator->GetTrainingListSample());
    concatenateValidationLabels->AddInput(sampleGenerator->GetValidationListLabel());
    concatenateValidationSamples->AddInput(sampleGenerator->GetValidationListSample());
    }

  // Update
  concatenateTrainingSamples->Update();
  concatenateTrainingLabels->Update();
  concatenateValidationSamples->Update();
  concatenateValidationLabels->Update();

  // Normalize the samples
  // Read the mean and variance form the XML file (estimate with the otbEstimateImagesStatistics application)

  MeasurementType  meanMeasurentVector;
  MeasurementType  varianceMeasurentVector;
  if(parseResult->IsOptionPresent("ImagesStatistics"))
    {
    StatisticsReader::Pointer  statisticsReader = StatisticsReader::New();
    statisticsReader->SetFileName(parseResult->GetParameterString("ImagesStatistics").c_str());
    meanMeasurentVector     = statisticsReader->GetStatisticVectorByName("mean");
    varianceMeasurentVector = statisticsReader->GetStatisticVectorByName("variance");
    }
  else
    {
    meanMeasurentVector.SetSize(nbBands);
    meanMeasurentVector.Fill(0.);
    varianceMeasurentVector.SetSize(nbBands);
    varianceMeasurentVector.Fill(1.);
    }

  std::cout << "MeanVector: " << meanMeasurentVector  << std::endl;
  std::cout << "Variance Vector: " << varianceMeasurentVector  << std::endl;

  // Shift scale the samples
  ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
  trainingShiftScaleFilter->SetInput(concatenateTrainingSamples->GetOutput());
  trainingShiftScaleFilter->SetShifts(meanMeasurentVector);
  trainingShiftScaleFilter->SetScales(varianceMeasurentVector);
  trainingShiftScaleFilter->Update();

  ShiftScaleFilterType::Pointer validationShiftScaleFilter = ShiftScaleFilterType::New();
  validationShiftScaleFilter->SetInput(concatenateValidationSamples->GetOutput());
  validationShiftScaleFilter->SetShifts(meanMeasurentVector);
  validationShiftScaleFilter->SetScales(varianceMeasurentVector);
  validationShiftScaleFilter->Update();

  ListSampleType::Pointer listSample;
  LabelListSampleType::Pointer labelListSample;

  if(parseResult->IsOptionPresent("Balancing"))
    {
    // Balance the list sample.
    std::cout<<"Number of training samples before balancing: "<<concatenateTrainingSamples->GetOutputSampleList()->Size()<<std::endl;
    BalancingListSampleFilterType::Pointer balancingFilter = BalancingListSampleFilterType::New();
    balancingFilter->SetInput(trainingShiftScaleFilter->GetOutput()/*GetOutputSampleList()*/);
    balancingFilter->SetInputLabel(concatenateTrainingLabels->GetOutput()/*GetOutputSampleList()*/);
    balancingFilter->SetBalancingFactor(parseResult->GetParameterUInt("Balancing"));
    balancingFilter->Update();
    listSample = balancingFilter->GetOutputSampleList();
    labelListSample = balancingFilter->GetOutputLabelSampleList();
    std::cout<<"Number of samples after balancing: "<<balancingFilter->GetOutputSampleList()->Size()<<std::endl;
    }
  else
    {
    listSample = trainingShiftScaleFilter->GetOutputSampleList();
    labelListSample = concatenateTrainingLabels->GetOutputSampleList();
    std::cout<<"Number of training samples: "<<concatenateTrainingSamples->GetOutputSampleList()->Size()<<std::endl;
    }

  // split the data set into training/validation set
  ListSampleType::Pointer trainingListSample = listSample;
  ListSampleType::Pointer validationListSample = validationShiftScaleFilter->GetOutputSampleList();
  LabelListSampleType::Pointer trainingLabeledListSample = labelListSample;
  LabelListSampleType::Pointer validationLabeledListSample = concatenateValidationLabels->GetOutputSampleList();

  std::cout<<"Size of training set: "<<trainingListSample->Size()<<std::endl;
  std::cout<<"Size of validation set: "<<validationListSample->Size()<<std::endl;
  std::cout<<"Size of labeled training set: "<<trainingLabeledListSample->Size()<<std::endl;
  std::cout<<"Size of labeled validation set: "<<validationLabeledListSample->Size()<<std::endl;

  // Estimate SVM model
  SVMEstimatorType::Pointer svmestimator = SVMEstimatorType::New();
  svmestimator->SetInputSampleList(trainingListSample);
  svmestimator->SetTrainingSampleList(trainingLabeledListSample);

  if(parseResult->IsOptionPresent("Margin"))
    {
    svmestimator->SetC(parseResult->GetParameterDouble("Margin"));
    }

  svmestimator->SetKernelType(LINEAR);
  svmestimator->Update();
  svmestimator->GetModel()->SaveModel(parseResult->GetParameterString("Output"));

  std::cout<<"Learning done ... "<<std::endl;

  //--------------------------
  // Performances estimation
  ClassifierType::Pointer validationClassifier = ClassifierType::New();
  validationClassifier->SetSample(validationListSample);
  validationClassifier->SetNumberOfClasses(svmestimator->GetModel()->GetNumberOfClasses());
  validationClassifier->SetModel(svmestimator->GetModel());
  validationClassifier->Update();

  // Estimate performances
  ClassifierOutputType::ConstIterator it = validationClassifier->GetOutput()->Begin();
  ClassifierOutputType::ConstIterator itEnd = validationClassifier->GetOutput()->End();

  LabelListSampleType::Pointer classifierListLabel = LabelListSampleType::New();

  while (it != itEnd)
    {
    // Due to a bug in SVMClassifier, outlier in one-class SVM are labeled with unsigned int max
    classifierListLabel->PushBack(
        it.GetClassLabel() == itk::NumericTraits<unsigned int>::max() ? 2 : it.GetClassLabel()
                                 );
    ++it;
    }

  ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

  confMatCalc->SetReferenceLabels(validationLabeledListSample);
  confMatCalc->SetProducedLabels(classifierListLabel);

  confMatCalc->Update();

  std::cout<< "confusion matrix: \n" << confMatCalc->GetConfusionMatrix() << std::endl;

  // TODO: cout more information about the confusion matrix

  // TODO: implement hyperplan validation (cf. object detection)


  return EXIT_SUCCESS;
}

}
