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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"


#include <iostream>
#include "otbConfigurationFile.h"

//Image
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVectorData.h"
#include "otbListSampleGenerator.h"

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

// Extract a ROI of the vectordata
#include "otbVectorDataIntoImageProjectionFilter.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class TrainSVMImagesClassifier: public Application
{
public:
  /** Standard class typedefs. */
  typedef TrainSVMImagesClassifier Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self)
;

  itkTypeMacro(TrainSVMImagesClassifier, otb::Application)
;

  typedef otb::Image<FloatVectorImageType::InternalPixelType, 2> ImageReaderType;

  typedef FloatVectorImageType::PixelType PixelType;
  typedef FloatVectorImageType VectorImageType;
  typedef FloatImageType ImageType;

  // Training vectordata
  typedef itk::VariableLengthVector<ImageType::PixelType> MeasurementType;

  // SampleList manipulation
  typedef otb::ListSampleGenerator<VectorImageType, VectorDataType> ListSampleGeneratorType;

  typedef ListSampleGeneratorType::ListSampleType ListSampleType;
  typedef ListSampleGeneratorType::LabelType LabelType;
  typedef ListSampleGeneratorType::ListLabelType LabelListSampleType;
  typedef otb::Statistics::ConcatenateSampleListFilter<ListSampleType> ConcatenateListSampleFilterType;
  typedef otb::Statistics::ConcatenateSampleListFilter<LabelListSampleType> ConcatenateLabelListSampleFilterType;

  // Statistic XML file Reader
  typedef otb::StatisticsXMLFileReader<MeasurementType> StatisticsReader;

  // Enhance List Sample  typedef otb::Statistics::ListSampleToBalancedListSampleFilter<ListSampleType, LabelListSampleType>      BalancingListSampleFilterType;
  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  // SVM Estimator
  typedef otb::Functor::VariableLengthVectorToMeasurementVectorFunctor<MeasurementType> MeasurementVectorFunctorType;
  typedef otb::SVMSampleListModelEstimator<ListSampleType, LabelListSampleType, MeasurementVectorFunctorType>
      SVMEstimatorType;

  typedef otb::SVMClassifier<ListSampleType, LabelType::ValueType> ClassifierType;

  // Estimate performance on validation sample
  typedef otb::ConfusionMatrixCalculator<LabelListSampleType, LabelListSampleType> ConfusionMatrixCalculatorType;
  typedef ClassifierType::OutputType ClassifierOutputType;

  // VectorData projection filter
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> VectorDataProjectionFilterType;

  // Extract ROI
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, VectorImageType> VectorDataReprojectionType;

private:
  void DoInit()
  {
    SetName("TrainSVMImagesClassifier");
    SetDescription("Train a SVM classifier from multiple pairs of images and training vector data.");

    // Documentation
    SetDocName("Train SVM classifier from multiple images");
    SetDocLongDescription("This application performs SVM classifier training from multiple pairs of input images and training vector data. Samples are composed of pixel values in each band optionally centered and reduced using XML statistics file produced by the ComputeImagesStatistics application.\n The training vector data must contain polygons with a positive integer field representing the class label. Name of the field can be set using the \"Class label field\" parameter. Training and validation sample lists are built such that each class is equally represented in both lists. One parameter allows to control the ratio between the number of samples in training and validation sets. Two parameters allow to manage the size of the training and validation sets per class and per image.\n Several SVM classifier parameters can be set, such as the kernel function which defines the feature space (for example if the kernel is a Gaussian radial basis function, the corresponding feature space is an infinite-dimensional space). To allow some flexibility in the classification, SVM models have a cost parameter, C, that controls the trade-off between allowing training errors and forcing rigid margins. It creates a soft margin that permits some misclassifications. Increasing the value of C increases the cost of misclassifying points and forces the creation of a more accurate model that may not generalize well. Classifier parameters can also be optimized.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
 
    AddDocTag(Tags::Learning);

    //Group IO
    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io","This group of parameters allows to set input and output data.");
    AddParameter(ParameterType_InputImageList, "io.il", "Input Image List");
    SetParameterDescription("io.il", "A list of input images.");
    AddParameter(ParameterType_InputVectorDataList, "io.vd", "Vector Data List");
    SetParameterDescription("io.vd", "A list of vector data to select the training samples.");
    AddParameter(ParameterType_InputFilename, "io.imstat", "XML image statistics file");
    MandatoryOff("io.imstat");
    SetParameterDescription("io.imstat", "Filename of an XML file containing mean and standard deviation of input images.");
    AddParameter(ParameterType_OutputFilename, "io.out", "Output SVM model");
    SetParameterDescription("io.out", "Output file containing the SVM model estimated");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    //Group Sample list
    AddParameter(ParameterType_Group,"sample","Training and validation samples parameters");
    SetParameterDescription("sample","This group of parameters allows to set training and validation sample lists parameters.");
    
    AddParameter(ParameterType_Int, "sample.mt", "Maximum training sample size");
    //MandatoryOff("mt");
    SetDefaultParameterInt("sample.mt", -1);
    SetParameterDescription("sample.mt", "Maximum size of the training sample list (default = -1).");
    AddParameter(ParameterType_Int, "sample.mv", "Maximum validation sample size");
    // MandatoryOff("mv");
    SetDefaultParameterInt("sample.mv", -1);
    SetParameterDescription("sample.mv", "Maximum size of the validation sample list (default = -1)");

    AddParameter(ParameterType_Empty, "sample.edg", "On edge pixel inclusion");
    SetParameterDescription("sample.edg", "Take pixels on polygon edge into consideration when building training and validation samples.");
    MandatoryOff("sample.edg");
        
    AddParameter(ParameterType_Float, "sample.vtr", "training and validation sample ratio");
    SetParameterDescription("sample.vtr",
                            "Ratio between training and validation samples (0.0 = all training, 1.0 = all validation) default = 0.5.");
    SetParameterFloat("sample.vtr", 0.5);
    
    AddParameter(ParameterType_String, "sample.vfn", "Name of the discrimination field");
    SetParameterDescription("sample.vfn", "Name of the field used to discriminate class in the vector data files.");
    SetParameterString("sample.vfn", "Class");

    //Group SVM
    AddParameter(ParameterType_Group,"svm","SVM classifier parameters");
    SetParameterDescription("svm","This group of parameters allows to set SVM classifier parameters.");
    AddParameter(ParameterType_Choice, "svm.k", "SVM Kernel Type");
    AddChoice("svm.k.linear", "Linear");
    AddChoice("svm.k.rbf", "Gaussian radial basis function");
    AddChoice("svm.k.poly", "Polynomial");
    AddChoice("svm.k.sigmoid", "Sigmoid");
    SetParameterString("svm.k", "linear");
    SetParameterDescription("svm.k", "SVM Kernel Type.");
    AddParameter(ParameterType_Float, "svm.c", "Cost parameter C.");
    SetParameterFloat("svm.c", 1.0);
    SetParameterDescription("svm.c", "SVM models have a cost parameter C (1 by default) to control the trade-off between training errors and forcing rigid margins.");
    AddParameter(ParameterType_Empty, "svm.opt", "parameters optimization");
    MandatoryOff("svm.opt");
    SetParameterDescription("svm.opt", "SVM optimization flag");

    AddRANDParameter();
    // Doc example parameter settings
    SetDocExampleParameterValue("io.il", "QB_1_ortho.tif");
    SetDocExampleParameterValue("io.vd", "VectorData_QB1.shp");
    SetDocExampleParameterValue("io.imstat", "EstimateImageStatisticsQB1.xml");
    SetDocExampleParameterValue("sample.mv", "100");
    SetDocExampleParameterValue("sample.mt", "100");
    SetDocExampleParameterValue("sample.vtr", "0.5");
    SetDocExampleParameterValue("svm.opt", "true");
    SetDocExampleParameterValue("io.out", "svmModelQB1.svm");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    GetLogger()->Debug("Entering DoExecute\n");
    //Create training and validation for list samples and label list samples
    ConcatenateLabelListSampleFilterType::Pointer
        concatenateTrainingLabels = ConcatenateLabelListSampleFilterType::New();
    ConcatenateListSampleFilterType::Pointer concatenateTrainingSamples = ConcatenateListSampleFilterType::New();
    ConcatenateLabelListSampleFilterType::Pointer
        concatenateValidationLabels = ConcatenateLabelListSampleFilterType::New();
    ConcatenateListSampleFilterType::Pointer concatenateValidationSamples = ConcatenateListSampleFilterType::New();

    MeasurementType meanMeasurementVector;
    MeasurementType stddevMeasurementVector;

    //--------------------------
    // Load measurements from images
    unsigned int nbBands = 0;
    //Iterate over all input images

    FloatVectorImageListType* imageList = GetParameterImageList("io.il");
    VectorDataListType* vectorDataList = GetParameterVectorDataList("io.vd");

    vdreproj = VectorDataReprojectionType::New();

    //Iterate over all input images
    for (unsigned int imgIndex = 0; imgIndex < imageList->Size(); ++imgIndex)
      {
      FloatVectorImageType::Pointer image = imageList->GetNthElement(imgIndex);
      image->UpdateOutputInformation();

      if (imgIndex == 0)
        {
        nbBands = image->GetNumberOfComponentsPerPixel();
        }

      // read the Vectordata
      VectorDataType::Pointer vectorData = vectorDataList->GetNthElement(imgIndex);
      vectorData->Update();

      vdreproj->SetInputImage(image);
      vdreproj->SetInput(vectorData);
      vdreproj->SetUseOutputSpacingAndOriginFromImage(false);

    // Setup the DEM Handler
      otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

      vdreproj->Update();

      //Sample list generator
      ListSampleGeneratorType::Pointer sampleGenerator = ListSampleGeneratorType::New();


      sampleGenerator->SetInput(image);
      sampleGenerator->SetInputVectorData(vdreproj->GetOutput());

      sampleGenerator->SetClassKey(GetParameterString("sample.vfn"));
      sampleGenerator->SetMaxTrainingSize(GetParameterInt("sample.mt"));
      sampleGenerator->SetMaxValidationSize(GetParameterInt("sample.mv"));
      sampleGenerator->SetValidationTrainingProportion(GetParameterFloat("sample.vtr"));

     // take pixel located on polygon edge into consideration
     if (IsParameterEnabled("sample.edg"))
     {
       sampleGenerator->SetPolygonEdgeInclusion(true);
     }

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

    if (concatenateTrainingSamples->GetOutputSampleList()->Size() == 0)
      {
      otbAppLogFATAL("No training samples, cannot perform SVM training.");
      }

    if (concatenateValidationSamples->GetOutputSampleList()->Size() == 0)
      {
      otbAppLogWARNING("No validation samples.");
      }

    if (IsParameterEnabled("io.imstat"))
      {
      StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
      statisticsReader->SetFileName(GetParameterString("io.imstat"));
      meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
      stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
      }
    else
      {
      meanMeasurementVector.SetSize(nbBands);
      meanMeasurementVector.Fill(0.);
      stddevMeasurementVector.SetSize(nbBands);
      stddevMeasurementVector.Fill(1.);
      }

    // Shift scale the samples
    ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
    trainingShiftScaleFilter->SetInput(concatenateTrainingSamples->GetOutput());
    trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
    trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
    trainingShiftScaleFilter->Update();

    ShiftScaleFilterType::Pointer validationShiftScaleFilter = ShiftScaleFilterType::New();
    validationShiftScaleFilter->SetInput(concatenateValidationSamples->GetOutput());
    validationShiftScaleFilter->SetShifts(meanMeasurementVector);
    validationShiftScaleFilter->SetScales(stddevMeasurementVector);
    validationShiftScaleFilter->Update();

    ListSampleType::Pointer listSample;
    LabelListSampleType::Pointer labelListSample;
    //--------------------------
    // Balancing training sample (if needed)
    // if (IsParameterEnabled("sample.b"))
    //   {
    //   // Balance the list sample.
    //   otbAppLogINFO("Number of training samples before balancing: " << concatenateTrainingSamples->GetOutputSampleList()->Size())
    //   BalancingListSampleFilterType::Pointer balancingFilter = BalancingListSampleFilterType::New();
    //   balancingFilter->SetInput(trainingShiftScaleFilter->GetOutput()/*GetOutputSampleList()*/);
    //   balancingFilter->SetInputLabel(concatenateTrainingLabels->GetOutput()/*GetOutputSampleList()*/);
    //   balancingFilter->SetBalancingFactor(GetParameterInt("sample.b"));
    //   balancingFilter->Update();
    //   listSample = balancingFilter->GetOutputSampleList();
    //   labelListSample = balancingFilter->GetOutputLabelSampleList();
    //   otbAppLogINFO("Number of samples after balancing: " << balancingFilter->GetOutputSampleList()->Size());

    //   }
    // else
    //   {
      listSample = trainingShiftScaleFilter->GetOutputSampleList();
      labelListSample = concatenateTrainingLabels->GetOutputSampleList();
      otbAppLogINFO("Number of training samples: " << concatenateTrainingSamples->GetOutputSampleList()->Size());
    //  }
    //--------------------------
    // Split the data set into training/validation set
    ListSampleType::Pointer trainingListSample = listSample;
    ListSampleType::Pointer validationListSample = validationShiftScaleFilter->GetOutputSampleList();
    LabelListSampleType::Pointer trainingLabeledListSample = labelListSample;
    LabelListSampleType::Pointer validationLabeledListSample = concatenateValidationLabels->GetOutputSampleList();
    otbAppLogINFO("Size of training set: " << trainingListSample->Size());
    otbAppLogINFO("Size of validation set: " << validationListSample->Size());
    otbAppLogINFO("Size of labeled training set: " << trainingLabeledListSample->Size());
    otbAppLogINFO("Size of labeled validation set: " << validationLabeledListSample->Size());

    //--------------------------
    // Estimate SVM model
    SVMEstimatorType::Pointer svmestimator = SVMEstimatorType::New();
    svmestimator->SetInputSampleList(trainingListSample);
    svmestimator->SetTrainingSampleList(trainingLabeledListSample);
    //SVM Option
    //TODO : Add other options ?
    if (IsParameterEnabled("svm.opt"))
      {
      svmestimator->SetParametersOptimization(true);
      }


    svmestimator->SetC(GetParameterFloat("svm.c"));


    switch (GetParameterInt("svm.k"))
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

    svmestimator->Update();
    svmestimator->GetModel()->SaveModel(GetParameterString("io.out"));

    otbAppLogINFO( "Learning done -> Final SVM accuracy: " << svmestimator->GetFinalCrossValidationAccuracy() << std::endl);

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
                                    it.GetClassLabel() == itk::NumericTraits<unsigned int>::max() ? 2
                                                                                                  : it.GetClassLabel());
      ++it;
      }

    ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

    confMatCalc->SetReferenceLabels(validationLabeledListSample);
    confMatCalc->SetProducedLabels(classifierListLabel);

    confMatCalc->Update();

    otbAppLogINFO("*** SVM training performances ***\n" << "Confusion matrix:\n" << confMatCalc->GetConfusionMatrix());

    for (unsigned int itClasses = 0; itClasses < svmestimator->GetModel()->GetNumberOfClasses(); itClasses++)
      {
      otbAppLogINFO("Precision of class [" << itClasses << "] vs all: " << confMatCalc->GetPrecisions()[itClasses]);
      otbAppLogINFO("Recall of class [" << itClasses << "] vs all: " << confMatCalc->GetRecalls()[itClasses]);
      otbAppLogINFO("F-score of class [" << itClasses << "] vs all: " << confMatCalc->GetFScores()[itClasses] << "\n");
      }
    otbAppLogINFO("Global performance, Kappa index: " << confMatCalc->GetKappaIndex());
    // TODO: implement hyperplan distance classifier and performance validation (cf. object detection) ?


  }
  VectorDataReprojectionType::Pointer vdreproj;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainSVMImagesClassifier)

