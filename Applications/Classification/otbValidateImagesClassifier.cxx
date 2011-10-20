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
#include "otbValidateImagesClassifier.h"

#include <iostream>
#include <fstream>
#include "otbCommandLineArgumentParser.h"

//Image
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
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

// Classification filter
#include "otbSVMImageClassificationFilter.h"

// VectorData projection filter
#include "otbVectorDataProjectionFilter.h"

namespace otb
{

int ValidateImagesClassifier::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("ValidateImagesClassifier");
  descriptor->SetDescription("Perform SVM validation from multiple input images and multiple vector data.");
  descriptor->AddOptionNParams("InputImages", "A list of images",
                               "in", true, ApplicationDescriptor::InputImage);
  descriptor->AddOptionNParams("VectorDataSamples", "Vector Data of sample used to validate the estimator",
                               "vd", true, ApplicationDescriptor::FileName);
  descriptor->AddOption("ImagesStatistics", "XML file containing mean and standard deviation of input images which have been used to train the svm estimator.",
                        "is", 1, false, ApplicationDescriptor::FileName);
  descriptor->AddOption("SVMmodel", "SVM model to validate its performances",
                        "svm", 1, true, ApplicationDescriptor::FileName);
  descriptor->AddOption("OutputPerfEstimation", "File which will contain the performance of the svm model",
                        "out", 1, false, ApplicationDescriptor::FileName);
  return EXIT_SUCCESS;
}

int ValidateImagesClassifier::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Input Image
  typedef float                                           PixelType;
  typedef otb::VectorImage<PixelType, 2>                   VectorImageType;
  typedef otb::Image<PixelType, 2>                         ImageType;
  typedef otb::ImageFileReader<VectorImageType>           ReaderType;
  typedef int                                             LabeledPixelType;
  typedef otb::Image<LabeledPixelType, 2>                 LabeledImageType;

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

  // Enhance List Sample
  typedef otb::Statistics::ShiftScaleSampleListFilter<
      ListSampleType, ListSampleType>                       ShiftScaleFilterType;

  /// Classification typedefs
  typedef otb::SVMImageClassificationFilter<VectorImageType, LabeledImageType>   ClassificationFilterType;
  typedef ClassificationFilterType::Pointer                                      ClassificationFilterPointerType;
  typedef ClassificationFilterType::ModelType                                    ModelType;
  typedef ModelType::Pointer                                                     ModelPointerType;

  typedef otb::Functor::VariableLengthVectorToMeasurementVectorFunctor<
      MeasurementType>                                      MeasurementVectorFunctorType;
  typedef otb::SVMSampleListModelEstimator<
      ListSampleType,
      LabelListSampleType,
      MeasurementVectorFunctorType>                         SVMEstimatorType;

  // Estimate performance on validation sample
  typedef otb::SVMClassifier<ListSampleType, LabelType::ValueType>     ClassifierType;
  typedef otb::ConfusionMatrixCalculator<LabelListSampleType,
      LabelListSampleType>                                  ConfusionMatrixCalculatorType;
  typedef ClassifierType::OutputType ClassifierOutputType;

  // Projection of a vectorData
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType>     VectorDataProjectionFilterType;

  //Create validation list samples and validation label list samples
  ConcatenateLabelListSampleFilterType::Pointer concatenateValidationLabels =
                                                  ConcatenateLabelListSampleFilterType::New();
  ConcatenateListSampleFilterType::Pointer concatenateValidationSamples =
                                            ConcatenateListSampleFilterType::New();
  //--------------------------
  // Load measuremnts from input images
  unsigned int nbBands = 0;
  //Iterate over all input images
  for(int imgIndex = 0; imgIndex < parseResult->GetNumberOfParameters("InputImages"); ++imgIndex)
    {
    // Read the image
    ReaderType::Pointer    reader  = ReaderType::New();
    reader->SetFileName(parseResult->GetParameterString("InputImages", imgIndex));
    reader->UpdateOutputInformation();

    std::cout<<"Processing image ("<<imgIndex<<"): "<<reader->GetFileName()<<std::endl;

    if (imgIndex == 0)
      {
      nbBands = reader->GetOutput()->GetNumberOfComponentsPerPixel();
      }

    // read the Vectordata
    VectorDataReaderType::Pointer vdreader = VectorDataReaderType::New();
    vdreader->SetFileName(parseResult->GetParameterString("VectorDataSamples", imgIndex));
    vdreader->Update();
    std::cout<<"Set VectorData filename: "<< parseResult->GetParameterString("VectorDataSamples", imgIndex) <<std::endl;

    // Project the vectorData in the Image Coodinate system
    VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
    vproj->SetInput(vdreader->GetOutput());
    vproj->SetInputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
    vproj->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
    vproj->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
    // TODO add DEM support
    vproj->Update();

    //Sample list generator
    ListSampleGeneratorType::Pointer sampleGenerator = ListSampleGeneratorType::New();

    //Set inputs of the sample generator
    //TODO the ListSampleGenerator perform UpdateOutputData over the input image (need a persistent implementation)
    sampleGenerator->SetInput(reader->GetOutput());
    sampleGenerator->SetInputVectorData(vproj->GetOutput());
    sampleGenerator->SetValidationTrainingProportion(1.0); // All in validation

    sampleGenerator->SetClassKey("Class");

    sampleGenerator->Update();

    //Concatenate training and validation samples from the image
    concatenateValidationLabels->AddInput(sampleGenerator->GetValidationListLabel());
    concatenateValidationSamples->AddInput(sampleGenerator->GetValidationListSample());
    }

  // Update
  concatenateValidationSamples->Update();
  concatenateValidationLabels->Update();

  //--------------------------
  // Normalize the samples
  // Read the mean and standard deviation form the XML file (estimate with the otbEstimateImagesStatistics application)
  MeasurementType  meanMeasurentVector;
  MeasurementType  stddevMeasurentVector;
  if(parseResult->IsOptionPresent("ImagesStatistics"))
    {
    StatisticsReader::Pointer  statisticsReader = StatisticsReader::New();
    statisticsReader->SetFileName(parseResult->GetParameterString("ImagesStatistics").c_str());
    meanMeasurentVector     = statisticsReader->GetStatisticVectorByName("mean");
    stddevMeasurentVector = statisticsReader->GetStatisticVectorByName("stddev");
    }
  else
    {
    meanMeasurentVector.SetSize(nbBands);
    meanMeasurentVector.Fill(0.);
    stddevMeasurentVector.SetSize(nbBands);
    stddevMeasurentVector.Fill(1.);
    }

  std::cout << "Mean vector loaded and used: " << meanMeasurentVector  << std::endl;
  std::cout << "Standard deviation vector loaded and used: " << stddevMeasurentVector  << std::endl;

  // Shift scale the samples
  ShiftScaleFilterType::Pointer validationShiftScaleFilter = ShiftScaleFilterType::New();
  validationShiftScaleFilter->SetInput(concatenateValidationSamples->GetOutput());
  validationShiftScaleFilter->SetShifts(meanMeasurentVector);
  validationShiftScaleFilter->SetScales(stddevMeasurentVector);
  validationShiftScaleFilter->Update();

  //--------------------------
  // split the data set into training/validation set
  ListSampleType::Pointer validationListSample = validationShiftScaleFilter->GetOutputSampleList();
  LabelListSampleType::Pointer validationLabeledListSample = concatenateValidationLabels->GetOutputSampleList();

  std::cout<<"Size of validation set: "<<validationListSample->Size()<<std::endl;
  std::cout<<"Size of labeled validation set: "<<validationLabeledListSample->Size()<<std::endl;

  //--------------------------
  // Load svm model
  ModelPointerType modelSVM = ModelType::New();
  modelSVM->LoadModel(parseResult->GetParameterString("SVMmodel").c_str());

  //--------------------------
  // Performances estimation
  ClassifierType::Pointer validationClassifier = ClassifierType::New();
  validationClassifier->SetSample(validationListSample);
  validationClassifier->SetNumberOfClasses(modelSVM->GetNumberOfClasses());
  validationClassifier->SetModel(modelSVM);
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

  std::cout << "*** SVM training performances ***\n" <<"Confusion matrix:\n" << confMatCalc->GetConfusionMatrix() << std::endl;

    for (unsigned int itClasses = 0; itClasses < modelSVM->GetNumberOfClasses(); itClasses++)
      {
      std::cout << "Precision of class [" << itClasses << "] vs all: " << confMatCalc->GetPrecisions()[itClasses] << std::endl;
      std::cout << "Recall of class [" << itClasses << "] vs all: "  << confMatCalc->GetRecalls()[itClasses] << std::endl;
      std::cout << "F-score of class [" << itClasses << "] vs all: "  << confMatCalc->GetFScores()[itClasses] << "\n" << std::endl;
      }
    std::cout << "Global performance, Kappa index: " << confMatCalc->GetKappaIndex() << std::endl;

  //--------------------------
  // Save output in a ascii file (if needed)
  if(parseResult->IsOptionPresent("OutputPerfEstimation"))
    {
    std::ofstream file;
    file.open(parseResult->GetParameterString("OutputPerfEstimation").c_str());
    file << "Precision of the different class: " << confMatCalc->GetPrecisions() << std::endl;
    file << "Recall of the different class: " << confMatCalc->GetRecalls() << std::endl;
    file << "F-score of the different class: " << confMatCalc->GetFScores() << std::endl;
    file << "Kappa index: " << confMatCalc->GetKappaIndex() << std::endl;
    file.close();
    }

  return EXIT_SUCCESS;
}

}
