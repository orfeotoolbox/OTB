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

// Balancing ListSample
#include "otbListSampleToBalancedListSampleFilter.h"

// VectorData projection filter
#include "otbVectorDataProjectionFilter.h"

// Extrat a ROI of the vectordata
#include "otbVectorDataExtractROI.h"

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
  descriptor->AddOption("SVM_Kernel", "Type of kernel use to estimate SVM model : 0 = LINEAR (default), 1 = RBF,  2 = POLY, 3 = SIGMOID",
                        "k", 1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("MaxTrainingSize", "Maximum size ot the training sample (default = -1)",
                        "mt", 1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("MaxValidationSize", "Maximum size of the validation sample (default = -1)",
                        "mv", 1, false, ApplicationDescriptor::Integer);
  descriptor->AddOption("ValidationTrainingRatio", "Ratio between training and validation sample (0.0 = all training, 1.0 = all validation) default = 0.5",
                        "vtr", 1, false, ApplicationDescriptor::Real);
  descriptor->AddOption("SVMParamOptim", "Use SVM parameters optimization",
                        "opt", 1, false, ApplicationDescriptor::Integer);

  return EXIT_SUCCESS;
}

int TrainImagesClassifier::Execute(otb::ApplicationOptionsResult* parseResult)
{
  // Input Image
  //typedef double                                          ValueType;
  typedef float                                           PixelType;
  typedef otb::VectorImage<PixelType, 2>                   VectorImageType;
  typedef otb::Image<PixelType, 2>                         ImageType;
  typedef otb::ImageFileReader<VectorImageType>           ReaderType;

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
  typedef otb::Statistics::ListSampleToBalancedListSampleFilter<
      ListSampleType, LabelListSampleType>                  BalancingListSampleFilterType;
  typedef otb::Statistics::ShiftScaleSampleListFilter<
      ListSampleType, ListSampleType>                       ShiftScaleFilterType;

  // SVM Estimator
  typedef otb::Functor::VariableLengthVectorToMeasurementVectorFunctor<
      MeasurementType>                                      MeasurementVectorFunctorType;
  typedef otb::SVMSampleListModelEstimator<
      ListSampleType,
      LabelListSampleType,
      MeasurementVectorFunctorType>                         SVMEstimatorType;
  typedef otb::SVMClassifier<ListSampleType, LabelType::ValueType>     ClassifierType;

  // Estimate performance on validation sample
  typedef otb::ConfusionMatrixCalculator<LabelListSampleType,
      LabelListSampleType>                                  ConfusionMatrixCalculatorType;
  typedef ClassifierType::OutputType ClassifierOutputType;

  // VectorData projection filter
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType>     VectorDataProjectionFilterType;

  // Extract ROI
  typedef otb::VectorDataExtractROI<VectorDataType>            VectorDataExtractROIType;
  typedef VectorDataExtractROIType::RegionType                 RemoteSensingRegionType;


  //Create training and validation for list samples and label list samples
  ConcatenateLabelListSampleFilterType::Pointer concatenateTrainingLabels =
                                                  ConcatenateLabelListSampleFilterType::New();
  ConcatenateListSampleFilterType::Pointer concatenateTrainingSamples =
                                             ConcatenateListSampleFilterType::New();
  ConcatenateLabelListSampleFilterType::Pointer concatenateValidationLabels =
                                                  ConcatenateLabelListSampleFilterType::New();
  ConcatenateListSampleFilterType::Pointer concatenateValidationSamples =
                                            ConcatenateListSampleFilterType::New();

  //--------------------------
  // Load measurements from images
  unsigned int nbBands = 0;
  //Iterate over all input images
  for(int imgIndex = 0; imgIndex<parseResult->GetNumberOfParameters("InputImages"); ++imgIndex)
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
    std::cout<<"Processing vectordata ("<<imgIndex<<"): "<< parseResult->GetParameterString("VectorDataSamples", imgIndex) <<std::endl;

    // Extract a ROI corresponding to the Extent of the image
    VectorDataExtractROIType::Pointer  vdextract = VectorDataExtractROIType::New();
    vdextract = VectorDataExtractROIType::New();
    vdextract->SetInput(vdreader->GetOutput());

    // Ge the index of the corner of the image
    ImageType::IndexType ul, ur, ll, lr;
    ImageType::PointType pul, pur, pll, plr;
    ul = reader->GetOutput()->GetLargestPossibleRegion().GetIndex();
    ur = ul;
    ll = ul;
    lr = ul;
    ur[0] += reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
    lr[0] += reader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
    lr[1] += reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
    ll[1] += reader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];

    // Transform to physical point
    reader->GetOutput()->TransformIndexToPhysicalPoint(ul, pul);
    reader->GetOutput()->TransformIndexToPhysicalPoint(ur, pur);
    reader->GetOutput()->TransformIndexToPhysicalPoint(ll, pll);
    reader->GetOutput()->TransformIndexToPhysicalPoint(lr, plr);

    // Build the cartographic region
    RemoteSensingRegionType                     rsRegion;
    RemoteSensingRegionType::IndexType rsOrigin;
    RemoteSensingRegionType::SizeType  rsSize;
    rsOrigin[0] = std::min(pul[0], plr[0]);
    rsOrigin[1] = std::min(pul[1], plr[1]);
    rsSize[0] = vcl_abs(pul[0] - plr[0]);
    rsSize[1] = vcl_abs(pul[1] - plr[1]);

    rsRegion.SetOrigin(rsOrigin);
    rsRegion.SetSize(rsSize);
    rsRegion.SetRegionProjection(reader->GetOutput()->GetProjectionRef());
    rsRegion.SetKeywordList(reader->GetOutput()->GetImageKeywordlist());

    // Set the cartographic region to the extract roi filter
    vdextract->SetRegion(rsRegion);


    // Project the vectorData in the Image Coodinate system
    VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
    vproj->SetInput(vdextract->GetOutput());
    vproj->SetInputProjectionRef(vdreader->GetOutput()->GetProjectionRef());
    vproj->SetOutputKeywordList(reader->GetOutput()->GetImageKeywordlist());
    vproj->SetOutputProjectionRef(reader->GetOutput()->GetProjectionRef());
    // The 2 following lines are necessary in the case where the vectordata is used with a viewer
    // with listSampleGenerator we don't need this parameters.
    //vproj->SetOutputOrigin(reader->GetOutput()->GetOrigin());
    //vproj->SetOutputSpacing(reader->GetOutput()->GetSpacing());
    // TODO add DEM support
    vproj->Update();


    //Sample list generator
    ListSampleGeneratorType::Pointer sampleGenerator = ListSampleGeneratorType::New();

    //Set inputs of the sample generator
    //TODO the ListSampleGenerator perform UpdateOutputData over the input image (need a persistent implementation)
    sampleGenerator->SetInput(reader->GetOutput());
    sampleGenerator->SetInputVectorData(vproj->GetOutput());
    if (parseResult->IsOptionPresent("MaxTrainingSize"))
      {
      sampleGenerator->SetMaxTrainingSize(parseResult->GetParameterInt("MaxTrainingSize"));
      }
    if (parseResult->IsOptionPresent("MaxValidationSize"))
      {
      sampleGenerator->SetMaxValidationSize(parseResult->GetParameterInt("MaxValidationSize"));
      }
    if (parseResult->IsOptionPresent("ValidationTrainingRatio"))
      {
      sampleGenerator->SetValidationTrainingProportion(parseResult->GetParameterDouble("ValidationTrainingRatio"));
      }
    else
      {
      sampleGenerator->SetValidationTrainingProportion(0.5);
      }
    sampleGenerator->SetClassKey("Class");

    sampleGenerator->Update();

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

  //--------------------------
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

  std::cout << "Mean vector loaded and used: " << meanMeasurentVector  << std::endl;
  std::cout << "Variance vector loaded and used: " << varianceMeasurentVector  << std::endl;

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

  //--------------------------
  // Balancing training sample (if needed)
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

  //--------------------------
  // Split the data set into training/validation set
  ListSampleType::Pointer trainingListSample = listSample;
  ListSampleType::Pointer validationListSample = validationShiftScaleFilter->GetOutputSampleList();
  LabelListSampleType::Pointer trainingLabeledListSample = labelListSample;
  LabelListSampleType::Pointer validationLabeledListSample = concatenateValidationLabels->GetOutputSampleList();

  std::cout<<"Size of training set: "<<trainingListSample->Size()<<std::endl;
  std::cout<<"Size of validation set: "<<validationListSample->Size()<<std::endl;
  std::cout<<"Size of labeled training set: "<<trainingLabeledListSample->Size()<<std::endl;
  std::cout<<"Size of labeled validation set: "<<validationLabeledListSample->Size()<<std::endl;

  //--------------------------
  // Estimate SVM model
  SVMEstimatorType::Pointer svmestimator = SVMEstimatorType::New();
  svmestimator->SetInputSampleList(trainingListSample);
  svmestimator->SetTrainingSampleList(trainingLabeledListSample);

  //SVM Option
  //TODO : Add some other options ?
  if(parseResult->IsOptionPresent("SVMParamOptim"))
    {
    svmestimator->SetParametersOptimization(true);
    }

  if(parseResult->IsOptionPresent("Margin"))
    {
    svmestimator->SetC(parseResult->GetParameterDouble("Margin"));
    }

  if(parseResult->IsOptionPresent("SVM_Kernel"))
    {
    switch (parseResult->GetParameterInt("SVM_Kernel"))
      {
      case 0: // LINEAR
        svmestimator->SetKernelType(LINEAR);
        break;
      case 1: // RBF
        svmestimator->SetKernelType(RBF);
        break;
      case 2: // POLY
        svmestimator->SetKernelType(POLY);
        break;
      case 3: // SIGMOID
        svmestimator->SetKernelType(SIGMOID);
        break;
      default: // DEFAULT = LINEAR
        svmestimator->SetKernelType(LINEAR);
        break;
      }
    }
  else
    {
    svmestimator->SetKernelType(LINEAR);
    }
  svmestimator->Update();
  svmestimator->GetModel()->SaveModel(parseResult->GetParameterString("Output"));

  std::cout<<"Learning done -> Final SVM accuracy: " << svmestimator->GetFinalCrossValidationAccuracy() <<std::endl;

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

  std::cout << "*** SVM training performances ***\n" <<"Confusion matrix:\n" << confMatCalc->GetConfusionMatrix() << std::endl;

  for (unsigned int itClasses = 0; itClasses < svmestimator->GetModel()->GetNumberOfClasses(); itClasses++)
    {
    std::cout << "Precision of class [" << itClasses << "] vs all: " << confMatCalc->GetPrecisions()[itClasses] << std::endl;
    std::cout << "Recall of class [" << itClasses << "] vs all: "  << confMatCalc->GetRecalls()[itClasses] << std::endl;
    std::cout << "F-score of class [" << itClasses << "] vs all: "  << confMatCalc->GetFScores()[itClasses] << "\n" << std::endl;
    }
  std::cout << "Global performance, Kappa index: " << confMatCalc->GetKappaIndex() << std::endl;

  // TODO: implement hyperplan distance classifier and performance validation (cf. object detection) ?

  return EXIT_SUCCESS;
}

}
