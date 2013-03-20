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

//Estimator
#include "otbKNearestNeighborsMachineLearningModel.h"
#include "otbRandomForestsMachineLearningModel.h"
#include "otbSVMMachineLearningModel.h"
#include "otbLibSVMMachineLearningModel.h"

// Statistic XML Reader
#include "otbStatisticsXMLFileReader.h"

// Validation
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

class TrainMachineLearningImagesClassifier: public Application
{
public:
  /** Standard class typedefs. */
  typedef TrainMachineLearningImagesClassifier Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(TrainMachineLearningImagesClassifier, otb::Application);

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


  // Machine Learning models
  typedef otb::RandomForestsMachineLearningModel<ImageType::PixelType,ListSampleGeneratorType::ClassLabelType> RandomForestType;
  typedef otb::KNearestNeighborsMachineLearningModel<ImageType::PixelType,ListSampleGeneratorType::ClassLabelType> KNNType;
  typedef otb::SVMMachineLearningModel<ImageType::PixelType,ListSampleGeneratorType::ClassLabelType> SVMType;
  typedef otb::LibSVMMachineLearningModel<ImageType::PixelType,ListSampleGeneratorType::ClassLabelType> LibSVMType;


  // Estimate performance on validation sample
  typedef otb::ConfusionMatrixCalculator<LabelListSampleType, LabelListSampleType> ConfusionMatrixCalculatorType;

  // VectorData projection filter
  typedef otb::VectorDataProjectionFilter<VectorDataType, VectorDataType> VectorDataProjectionFilterType;

  // Extract ROI
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType, VectorImageType> VectorDataReprojectionType;

private:
  void DoInit()
  {
    SetName("TrainMachineLearningImagesClassifier");
    SetDescription("Train a classifier (avalaible in OpenCV machine learning) from multiple pairs of images and training vector data.");

    // Documentation
    SetDocName("Train an OpenCV classifier from multiple images");
    SetDocLongDescription("This application performs a classifier training from multiple pairs of input images and training vector data. Samples are composed of pixel values in each band optionally centered and reduced using XML statistics file produced by the ComputeImagesStatistics application.\n The training vector data must contain polygons with a positive integer field representing the class label. Name of the field can be set using the \"Class label field\" parameter. Training and validation sample lists are built such that each class is equally represented in both lists. One parameter allows to control the ratio between the number of samples in training and validation sets. Two parameters allow to manage the size of the training and validation sets per class and per image.\n Several classifier parameters can be set depending on the classifier. In the validation process, the confusion matrix is organized the following way: rows = reference labels, columns = produced labels.");
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
    AddParameter(ParameterType_OutputFilename, "io.out", "Output model");
    SetParameterDescription("io.out", "Output file containing the model estimated");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    //Group Sample list
    AddParameter(ParameterType_Group,"sample","Training and validation samples parameters");
    SetParameterDescription("sample","This group of parameters allows to set training and validation sample lists parameters.");

    AddParameter(ParameterType_Int, "sample.mt", "Maximum training sample size");
    //MandatoryOff("mt");
    SetDefaultParameterInt("sample.mt", 1000);
    SetParameterDescription("sample.mt", "Maximum size of the training sample list (default = 1000).");
    AddParameter(ParameterType_Int, "sample.mv", "Maximum validation sample size");
    // MandatoryOff("mv");
    SetDefaultParameterInt("sample.mv", 1000);
    SetParameterDescription("sample.mv", "Maximum size of the validation sample list (default = 1000)");

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


    AddParameter(ParameterType_Choice, "classifier", "Classifier to used.");
       SetParameterDescription("classifier", "Choice of the classifier to used.");

       //Group LibSVM
       AddChoice("classifier.libsvm", "LibSVM classifier");
    //AddParameter(ParameterType_Group,"libsvm","LibSVM classifier parameters");
    SetParameterDescription("classifier.libsvm","This group of parameters allows to set SVM classifier parameters.");
    AddParameter(ParameterType_Choice, "classifier.libsvm.k", "SVM Kernel Type");
    AddChoice("classifier.libsvm.k.linear", "Linear");
    AddChoice("classifier.libsvm.k.rbf", "Gaussian radial basis function");
    AddChoice("classifier.libsvm.k.poly", "Polynomial");
    AddChoice("classifier.libsvm.k.sigmoid", "Sigmoid");
    SetParameterString("classifier.libsvm.k", "linear");
    SetParameterDescription("classifier.libsvm.k", "SVM Kernel Type.");
    AddParameter(ParameterType_Float, "classifier.libsvm.c", "Cost parameter C.");
    SetParameterFloat("classifier.libsvm.c", 1.0);
    SetParameterDescription("classifier.libsvm.c", "SVM models have a cost parameter C (1 by default) to control the trade-off between training errors and forcing rigid margins.");
    AddParameter(ParameterType_Empty, "classifier.libsvm.opt", "parameters optimization");
    MandatoryOff("classifier.libsvm.opt");
    SetParameterDescription("classifier.libsvm.opt", "SVM optimization flag");

    //Group SVM (openCV)
    AddChoice("classifier.svm", "SVM classifier (OpenCV)");
       //AddParameter(ParameterType_Group,"svm","SVM classifier parameters (OpenCV)");
       SetParameterDescription("classifier.svm","This group of parameters allows to set SVM classifier parameters.");
       AddParameter(ParameterType_Choice, "classifier.svm.m", "SVM Model Type");
       AddChoice("classifier.svm.m.csvc", "C support vector classification");
       AddChoice("classifier.svm.m.nusvc", "Nu support vector classification");
       AddChoice("classifier.svm.m.oneclass", "Distribution estimation (One Class SVM)");
       AddChoice("classifier.svm.m.epssvr", "Epsilon Support Vector Regression");
       AddChoice("classifier.svm.m.nusvr", "Nu Support Vector Regression");
       SetParameterString("classifier.svm.m", "csvc");
       SetParameterDescription("classifier.svm.m", "Type of SVM formulation.");
       AddParameter(ParameterType_Choice, "classifier.svm.k", "SVM Kernel Type");
       AddChoice("classifier.svm.k.linear", "Linear");
       AddChoice("classifier.svm.k.rbf", "Gaussian radial basis function");
       AddChoice("classifier.svm.k.poly", "Polynomial");
       AddChoice("classifier.svm.k.sigmoid", "Sigmoid");
       SetParameterString("classifier.svm.k", "linear");
       SetParameterDescription("classifier.svm.k", "SVM Kernel Type.");
       AddParameter(ParameterType_Float, "classifier.svm.c", "Cost parameter C.");
       SetParameterFloat("classifier.svm.c", 1.0);
       SetParameterDescription("classifier.svm.c", "SVM models have a cost parameter C (1 by default) to control the trade-off between training errors and forcing rigid margins.");
       AddParameter(ParameterType_Float, "classifier.svm.nu", "Parameter nu of a SVM optimization problem (NU_SVC / ONE_CLASS / NU_SVR).");
       SetParameterFloat("classifier.svm.nu", 0.0);
       SetParameterDescription("classifier.svm.nu", "Parameter nu of a SVM optimization problem.");
       AddParameter(ParameterType_Float, "classifier.svm.p", "Parameter epsilon of a SVM optimization problem (EPS_SVR).");
       SetParameterFloat("classifier.svm.p", 0.0);
       SetParameterDescription("classifier.svm.p", "Parameter epsilon of a SVM optimization problem (EPS_SVR).");
       AddParameter(ParameterType_Float, "classifier.svm.coef0", "Parameter coef0 of a kernel function (POLY / SIGMOID).");
       SetParameterFloat("classifier.svm.coef0", 0.0);
       SetParameterDescription("classifier.svm.coef0", "Parameter coef0 of a kernel function (POLY / SIGMOID).");
       AddParameter(ParameterType_Float, "classifier.svm.gamma", "Parameter gamma of a kernel function (POLY / RBF / SIGMOID).");
       SetParameterFloat("classifier.svm.gamma", 1.0);
       SetParameterDescription("classifier.svm.gamma", "Parameter gamma of a kernel function (POLY / RBF / SIGMOID).");
       AddParameter(ParameterType_Float, "classifier.svm.degree", "Parameter degree of a kernel function (POLY).");
       SetParameterFloat("classifier.svm.degree", 0.0);
       SetParameterDescription("classifier.svm.degree", "Parameter degree of a kernel function (POLY).");

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

  void LogConfusionMatrix(ConfusionMatrixCalculatorType* confMatCalc)
  {
    ConfusionMatrixCalculatorType::ConfusionMatrixType matrix = confMatCalc->GetConfusionMatrix();

    // Compute minimal width
    size_t minwidth = 0;

    for (unsigned int i = 0; i < matrix.Rows(); i++)
      {
      for (unsigned int j = 0; j < matrix.Cols(); j++)
        {
        std::ostringstream os;
        os << matrix(i,j);
        size_t size = os.str().size();

        if (size > minwidth)
          {
          minwidth = size;
          }
        }
      }

    typedef std::map<int, ConfusionMatrixCalculatorType::ClassLabelType> MapOfIndicesType;
    MapOfIndicesType mapOfIndices = confMatCalc->GetMapOfIndices();

    MapOfIndicesType::const_iterator it = mapOfIndices.begin();
    MapOfIndicesType::const_iterator end = mapOfIndices.end();

    for(; it != end; ++it)
      {
      std::ostringstream os;
      os << "[" << it->second << "]";

      size_t size = os.str().size();
      if (size > minwidth)
        {
        minwidth = size;
        }
      }


    // Generate matrix string, with 'minwidth' as size specifier
    std::ostringstream os;

    // Header line
    for (size_t i = 0; i < minwidth; ++i)
      os << " ";
    os << " ";

    it = mapOfIndices.begin();
    end = mapOfIndices.end();
    for(; it != end; ++it)
      {
      os << "[" << it->second << "]" << " ";
      }

    os << std::endl;

    // Each line of confusion matrix
    for (unsigned int i = 0; i < matrix.Rows(); i++)
      {
      ConfusionMatrixCalculatorType::ClassLabelType label = mapOfIndices[i];
      os << "[" << std::setw(minwidth - 2) << label << "]" << " ";
      for (unsigned int j = 0; j < matrix.Cols(); j++)
        {
        os << std::setw(minwidth) << matrix(i,j) << " ";
        }
      os << std::endl;
      }

    otbAppLogINFO("Confusion matrix (rows = reference labels, columns = produced labels):\n" << os.str());
  }


  void TrainLibSVM(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
  {
         LibSVMType::Pointer libSVMClassifier = LibSVMType::New();
         libSVMClassifier->SetInputListSample(trainingListSample);
         libSVMClassifier->SetTargetListSample(trainingLabeledListSample);
         //SVM Option
         //TODO : Add other options ?
         if (IsParameterEnabled("classifier.libsvm.opt"))
         {
                libSVMClassifier->SetParameterOptimization(true);
         }
         libSVMClassifier->SetC(GetParameterFloat("classifier.libsvm.c"));

         switch (GetParameterInt("classifier.libsvm.k"))
         {
                case 0: // LINEAR
                       libSVMClassifier->SetKernelType(LINEAR);
                     break;
                case 1: // RBF
                       libSVMClassifier->SetKernelType(RBF);
                     break;
                case 2: // POLY
                       libSVMClassifier->SetKernelType(POLY);
                     break;
                case 3: // SIGMOID
                       libSVMClassifier->SetKernelType(SIGMOID);
                     break;
                default: // DEFAULT = LINEAR
                       libSVMClassifier->SetKernelType(LINEAR);
                     break;
         }
         libSVMClassifier->Train();
         libSVMClassifier->Save(GetParameterString("io.out"));
         //otbAppLogINFO( "Learning done -> Final SVM accuracy: " << libSVMClassifier->GetFinalCrossValidationAccuracy() << std::endl);
  }

  void ClassifyLibSVM(ListSampleType::Pointer validationListSample, LabelListSampleType::Pointer predictedList)
  {
         //Classification
         LibSVMType::Pointer libSVMClassifier = LibSVMType::New();
         libSVMClassifier->Load(GetParameterString("io.out"));
         libSVMClassifier->SetInputListSample(validationListSample);
         libSVMClassifier->SetTargetListSample(predictedList);
         libSVMClassifier->PredictAll();
  }

  void TrainSVM(ListSampleType::Pointer trainingListSample, LabelListSampleType::Pointer trainingLabeledListSample)
  {
         std::cout<<"svm open CV"<<std::endl;
         SVMType::Pointer SVMClassifier = SVMType::New();
         SVMClassifier->SetInputListSample(trainingListSample);
         SVMClassifier->SetTargetListSample(trainingLabeledListSample);
         switch (GetParameterInt("classifier.svm.k"))
         {
                  case 0: // LINEAR
                         SVMClassifier->SetKernelType(CvSVM::LINEAR);
                         std::cout<<"CvSVM::LINEAR = "<<CvSVM::LINEAR<<std::endl;
                       break;
                  case 1: // RBF
                       SVMClassifier->SetKernelType(CvSVM::RBF);
                       std::cout<<"CvSVM::RBF = "<<CvSVM::RBF<<std::endl;
                     break;
                case 2: // POLY
                       SVMClassifier->SetKernelType(CvSVM::POLY);
                       std::cout<<"CvSVM::POLY = "<<CvSVM::POLY<<std::endl;
                     break;
                case 3: // SIGMOID
                       SVMClassifier->SetKernelType(CvSVM::SIGMOID);
                       std::cout<<"CvSVM::SIGMOID = "<<CvSVM::SIGMOID<<std::endl;
                     break;
                default: // DEFAULT = LINEAR
                       SVMClassifier->SetKernelType(CvSVM::LINEAR);
                       std::cout<<"CvSVM::LINEAR = "<<CvSVM::LINEAR<<std::endl;
                     break;
         }
         switch (GetParameterInt("classifier.svm.m"))
         {
                case 0: // C_SVC
                       SVMClassifier->SetSVMType(CvSVM::C_SVC);
                       std::cout<<"CvSVM::C_SVC = "<<CvSVM::C_SVC<<std::endl;
                     break;
                case 1: // NU_SVC
                       SVMClassifier->SetSVMType(CvSVM::NU_SVC);
                       std::cout<<"CvSVM::NU_SVC = "<<CvSVM::NU_SVC<<std::endl;
                     break;
                case 2: // ONE_CLASS
                       SVMClassifier->SetSVMType(CvSVM::ONE_CLASS);
                       std::cout<<"CvSVM::ONE_CLASS = "<<CvSVM::ONE_CLASS<<std::endl;
                     break;
                case 3: // EPS_SVR
                       SVMClassifier->SetSVMType(CvSVM::EPS_SVR);
                       std::cout<<"CvSVM::EPS_SVR = "<<CvSVM::EPS_SVR<<std::endl;
                     break;
                case 4: // NU_SVR
                       SVMClassifier->SetSVMType(CvSVM::NU_SVR);
                       std::cout<<"CvSVM::NU_SVR = "<<CvSVM::NU_SVR<<std::endl;
                     break;
                default: // DEFAULT = C_SVC
                       SVMClassifier->SetSVMType(CvSVM::C_SVC);
                       std::cout<<"CvSVM::C_SVC = "<<CvSVM::C_SVC<<std::endl;
                     break;
         }
         SVMClassifier->SetC(GetParameterFloat("classifier.svm.c"));
         SVMClassifier->SetNu(GetParameterFloat("classifier.svm.nu"));
         SVMClassifier->SetP(GetParameterFloat("classifier.svm.p"));
         SVMClassifier->SetCoef0(GetParameterFloat("classifier.svm.coef0"));
         SVMClassifier->SetGamma(GetParameterFloat("classifier.svm.gamma"));
         SVMClassifier->SetDegree(GetParameterFloat("classifier.svm.degree"));
         SVMClassifier->Train();
         SVMClassifier->Save(GetParameterString("io.out"));
  }

  void ClassifySVM(ListSampleType::Pointer validationListSample, LabelListSampleType::Pointer predictedList)
  {
         //Classification
         SVMType::Pointer SVMClassifier = SVMType::New();
         SVMClassifier->Load(GetParameterString("io.out"));
         SVMClassifier->SetInputListSample(validationListSample);
         SVMClassifier->SetTargetListSample(predictedList);
         SVMClassifier->PredictAll();
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
    // Estimate model
    //--------------------------
    LabelListSampleType::Pointer predictedList = LabelListSampleType::New();
    const std::string classifierType = GetParameterString("classifier");
    if (classifierType == "libsvm")
    {
           TrainLibSVM(trainingListSample, trainingLabeledListSample);
           ClassifyLibSVM(validationListSample, predictedList);
    }
    else if (classifierType == "svm")
    {
           TrainSVM(trainingListSample, trainingLabeledListSample);
           ClassifySVM(validationListSample, predictedList);
    }

    //--------------------------
    // Performances estimation
    //--------------------------

    ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

    std::cout<<"predicted list size == "<<predictedList->Size()<<std::endl;
    std::cout<<"validationLabeledListSample size == "<<validationLabeledListSample->Size()<<std::endl;
    confMatCalc->SetReferenceLabels(validationLabeledListSample);
    confMatCalc->SetProducedLabels(predictedList);

    confMatCalc->Update();

    otbAppLogINFO("SVM training performances");
    LogConfusionMatrix(confMatCalc);

    for (unsigned int itClasses = 0; itClasses < confMatCalc->GetNumberOfClasses(); itClasses++)
      {
      ConfusionMatrixCalculatorType::ClassLabelType classLabel = confMatCalc->GetMapOfIndices()[itClasses];

      otbAppLogINFO("Precision of class [" << classLabel << "] vs all: " << confMatCalc->GetPrecisions()[itClasses]);
      otbAppLogINFO("Recall of class    [" << classLabel << "] vs all: " << confMatCalc->GetRecalls()[itClasses]);
      otbAppLogINFO("F-score of class   [" << classLabel << "] vs all: " << confMatCalc->GetFScores()[itClasses] << "\n");
      }
    otbAppLogINFO("Global performance, Kappa index: " << confMatCalc->GetKappaIndex());
    // TODO: implement hyperplan distance classifier and performance validation (cf. object detection) ?


  }

  VectorDataReprojectionType::Pointer vdreproj;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainMachineLearningImagesClassifier)

