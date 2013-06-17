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

namespace otb
{
namespace Wrapper
{

void TrainImagesClassifier::DoInit()
{
  SetName("TrainImagesClassifier");
  SetDescription(
    "Train a classifier (available in OpenCV machine learning) from multiple pairs of images and training vector data.");

  // Documentation
  SetDocName("Train an OpenCV classifier from multiple images");
  SetDocLongDescription(
    "This application performs a classifier training from multiple pairs of input images and training vector data. "
    "Samples are composed of pixel values in each band optionally centered and reduced using an XML statistics file produced by "
    "the ComputeImagesStatistics application.\n The training vector data must contain polygons with a positive integer field "
    "representing the class label. The name of this field can be set using the \"Class label field\" parameter. Training and validation "
    "sample lists are built such that each class is equally represented in both lists. One parameter allows to control the ratio "
    "between the number of samples in training and validation sets. Two parameters allow to manage the size of the training and "
    "validation sets per class and per image.\n Several classifier parameters can be set depending on the chosen classifier. In the "
    "validation process, the confusion matrix is organized the following way: rows = reference labels, columns = produced labels. "
    "In the header of the optional confusion matrix output file, the validation (reference) and predicted (produced) class labels"
    " are ordered according to the rows/columns of the confusion matrix.");
  SetDocLimitations("None");
  SetDocAuthors("OTB-Team");
  SetDocSeeAlso("OpenCV documentation for machine learning http://docs.opencv.org/modules/ml/doc/ml.html ");

  AddDocTag(Tags::Learning);

  //Group IO
  AddParameter(ParameterType_Group, "io", "Input and output data");
  SetParameterDescription("io", "This group of parameters allows to set input and output data.");
  AddParameter(ParameterType_InputImageList, "io.il", "Input Image List");
  SetParameterDescription("io.il", "A list of input images.");
  AddParameter(ParameterType_InputVectorDataList, "io.vd", "Input Vector Data List");
  SetParameterDescription("io.vd", "A list of vector data to select the training samples.");
  AddParameter(ParameterType_InputFilename, "io.imstat", "Input XML image statistics file");
  MandatoryOff("io.imstat");
  SetParameterDescription("io.imstat",
                          "Input XML file containing the mean and the standard deviation of the input images.");
  AddParameter(ParameterType_OutputFilename, "io.confmatout", "Output confusion matrix");
  SetParameterDescription("io.confmatout", "Output file containing the confusion matrix (.csv format).");
  MandatoryOff("io.confmatout");
  AddParameter(ParameterType_OutputFilename, "io.out", "Output model");
  SetParameterDescription("io.out", "Output file containing the model estimated (.txt format).");

  // Elevation
  ElevationParametersHandler::AddElevationParameters(this, "elev");

  //Group Sample list
  AddParameter(ParameterType_Group, "sample", "Training and validation samples parameters");
  SetParameterDescription("sample",
                          "This group of parameters allows to set training and validation sample lists parameters.");

  AddParameter(ParameterType_Int, "sample.mt", "Maximum training sample size");
  //MandatoryOff("mt");
  SetDefaultParameterInt("sample.mt", 1000);
  SetParameterDescription("sample.mt", "Maximum size of the training sample list (default = 1000) (no limit = -1).");
  AddParameter(ParameterType_Int, "sample.mv", "Maximum validation sample size");
  // MandatoryOff("mv");
  SetDefaultParameterInt("sample.mv", 1000);
  SetParameterDescription("sample.mv", "Maximum size of the validation sample list (default = 1000) (no limit = -1).");

  AddParameter(ParameterType_Empty, "sample.edg", "On edge pixel inclusion");
  SetParameterDescription("sample.edg",
                          "Takes pixels on polygon edge into consideration when building training and validation samples.");
  MandatoryOff("sample.edg");

  AddParameter(ParameterType_Float, "sample.vtr", "Training and validation sample ratio");
  SetParameterDescription("sample.vtr",
                          "Ratio between training and validation samples (0.0 = all training, 1.0 = all validation) (default = 0.5).");
  SetParameterFloat("sample.vtr", 0.5);

  AddParameter(ParameterType_String, "sample.vfn", "Name of the discrimination field");
  SetParameterDescription("sample.vfn", "Name of the field used to discriminate class labels in the input vector data files.");
  SetParameterString("sample.vfn", "Class");

  AddParameter(ParameterType_Choice, "classifier", "Classifier to use for the training");
  SetParameterDescription("classifier", "Choice of the classifier to use for the training.");

  //Group LibSVM
  InitLibSVMParams();

#ifdef OTB_USE_OPENCV
  InitSVMParams();
  InitBoostParams();
  InitDecisionTreeParams();
  InitGradientBoostedTreeParams();
  InitNeuralNetworkParams();
  InitNormalBayesParams();
  InitRandomForestsParams();
  InitKNNParams();
#endif

  AddRANDParameter();
  // Doc example parameter settings
  SetDocExampleParameterValue("io.il", "QB_1_ortho.tif");
  SetDocExampleParameterValue("io.vd", "VectorData_QB1.shp");
  SetDocExampleParameterValue("io.imstat", "EstimateImageStatisticsQB1.xml");
  SetDocExampleParameterValue("sample.mv", "100");
  SetDocExampleParameterValue("sample.mt", "100");
  SetDocExampleParameterValue("sample.vtr", "0.5");
  SetDocExampleParameterValue("sample.edg", "false");
  SetDocExampleParameterValue("sample.vfn", "Class");
  SetDocExampleParameterValue("classifier", "libsvm");
  SetDocExampleParameterValue("classifier.libsvm.k", "linear");
  SetDocExampleParameterValue("classifier.libsvm.c", "1");
  SetDocExampleParameterValue("classifier.libsvm.opt", "false");
  SetDocExampleParameterValue("io.out", "svmModelQB1.txt");
  SetDocExampleParameterValue("io.confmatout", "svmConfusionMatrixQB1.csv");
}

void TrainImagesClassifier::DoUpdateParameters()
{
  // Nothing to do here : all parameters are independent
}


void TrainImagesClassifier::LogConfusionMatrix(ConfusionMatrixCalculatorType* confMatCalc)
{
  ConfusionMatrixCalculatorType::ConfusionMatrixType matrix = confMatCalc->GetConfusionMatrix();

  // Compute minimal width
  size_t minwidth = 0;

  for (unsigned int i = 0; i < matrix.Rows(); i++)
    {
    for (unsigned int j = 0; j < matrix.Cols(); j++)
      {
      std::ostringstream os;
      os << matrix(i, j);
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

  for (; it != end; ++it)
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
  for (; it != end; ++it)
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
      os << std::setw(minwidth) << matrix(i, j) << " ";
      }
    os << std::endl;
    }

  otbAppLogINFO("Confusion matrix (rows = reference labels, columns = produced labels):\n" << os.str());
}

void TrainImagesClassifier::Classify(ListSampleType::Pointer validationListSample, LabelListSampleType::Pointer predictedList)
{
  //Classification
  ModelPointerType model = MachineLearningModelFactoryType::CreateMachineLearningModel(GetParameterString("io.out"),
                                                                                       MachineLearningModelFactoryType::ReadMode);
  model->Load(GetParameterString("io.out"));
  model->SetInputListSample(validationListSample);
  model->SetTargetListSample(predictedList);
  model->PredictAll();
}

void TrainImagesClassifier::DoExecute()
{
  GetLogger()->Debug("Entering DoExecute\n");
  //Create training and validation for list samples and label list samples
  ConcatenateLabelListSampleFilterType::Pointer concatenateTrainingLabels =
    ConcatenateLabelListSampleFilterType::New();
  ConcatenateListSampleFilterType::Pointer concatenateTrainingSamples = ConcatenateListSampleFilterType::New();
  ConcatenateLabelListSampleFilterType::Pointer concatenateValidationLabels =
    ConcatenateLabelListSampleFilterType::New();
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
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this, "elev");

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

  ListSampleType::Pointer validationListSample=ListSampleType::New();

  //Test if the validation test is empty
  if ( concatenateValidationSamples->GetOutput()->Get()->Size() != 0 )
    {
    ShiftScaleFilterType::Pointer validationShiftScaleFilter = ShiftScaleFilterType::New();
    validationShiftScaleFilter->SetInput(concatenateValidationSamples->GetOutput());
    validationShiftScaleFilter->SetShifts(meanMeasurementVector);
    validationShiftScaleFilter->SetScales(stddevMeasurementVector);
    validationShiftScaleFilter->Update();
    validationListSample = validationShiftScaleFilter->GetOutputSampleList();
    }

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
    }
#ifdef OTB_USE_OPENCV
  else if (classifierType == "svm")
    {
    TrainSVM(trainingListSample, trainingLabeledListSample);
    }
  else if (classifierType == "boost")
    {
    TrainBoost(trainingListSample, trainingLabeledListSample);
    }
  else if (classifierType == "dt")
    {
    TrainDecisionTree(trainingListSample, trainingLabeledListSample);
    }
  else if (classifierType == "gbt")
    {
    TrainGradientBoostedTree(trainingListSample, trainingLabeledListSample);
    }
  else if (classifierType == "ann")
    {
    TrainNeuralNetwork(trainingListSample, trainingLabeledListSample);
    }
  else if (classifierType == "bayes")
    {
    TrainNormalBayes(trainingListSample, trainingLabeledListSample);
    }
  else if (classifierType == "rf")
    {
    TrainRandomForests(trainingListSample, trainingLabeledListSample);
    }
  else if (classifierType == "knn")
    {
    TrainKNN(trainingListSample, trainingLabeledListSample);
    }
#endif

  //--------------------------
  // Performances estimation
  //--------------------------
  ListSampleType::Pointer performanceListSample=ListSampleType::New();
  LabelListSampleType::Pointer performanceLabeledListSample=LabelListSampleType::New();

  //Test the input validation set size
  if(validationLabeledListSample->Size() != 0)
    {
    performanceListSample = validationListSample;
    performanceLabeledListSample = validationLabeledListSample;
    }
  else
    {
    otbAppLogWARNING("The validation set is empty!Performance estimation is done using the input training set in this case.");
    performanceListSample = trainingListSample;
    performanceLabeledListSample = trainingLabeledListSample;
    }

    Classify(performanceListSample, predictedList);

    ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

    std::cout << "predicted list size == " << predictedList->Size() << std::endl;
    std::cout << "validationLabeledListSample size == " << performanceLabeledListSample->Size() << std::endl;
    confMatCalc->SetReferenceLabels(performanceLabeledListSample);
    confMatCalc->SetProducedLabels(predictedList);

    confMatCalc->Compute();

    otbAppLogINFO("training performances");
    LogConfusionMatrix(confMatCalc);

    for (unsigned int itClasses = 0; itClasses < confMatCalc->GetNumberOfClasses(); itClasses++)
      {
      ConfusionMatrixCalculatorType::ClassLabelType classLabel = confMatCalc->GetMapOfIndices()[itClasses];

      otbAppLogINFO("Precision of class [" << classLabel << "] vs all: " << confMatCalc->GetPrecisions()[itClasses]);
      otbAppLogINFO("Recall of class    [" << classLabel << "] vs all: " << confMatCalc->GetRecalls()[itClasses]);
      otbAppLogINFO(
        "F-score of class   [" << classLabel << "] vs all: " << confMatCalc->GetFScores()[itClasses] << "\n");
      }
    otbAppLogINFO("Global performance, Kappa index: " << confMatCalc->GetKappaIndex());


    if (this->HasValue("io.confmatout"))
      {
      // Writing the confusion matrix in the output .CSV file

      MapOfClassesType::iterator itMapOfClassesValid, itMapOfClassesPred;
      ClassLabelType labelValid = 0;

      ConfusionMatrixType confusionMatrix = confMatCalc->GetConfusionMatrix();
      MapOfClassesType mapOfClassesValid = confMatCalc->GetMapOfClasses();

      unsigned int nbClassesPred = mapOfClassesValid.size();

      /////////////////////////////////////////////
      // Filling the 2 headers for the output file
      const std::string commentValidStr = "#Reference labels (rows):";
      const std::string commentPredStr = "#Produced labels (columns):";
      const char separatorChar = ',';
      std::ostringstream ossHeaderValidLabels, ossHeaderPredLabels;

      // Filling ossHeaderValidLabels and ossHeaderPredLabels for the output file
      ossHeaderValidLabels << commentValidStr;
      ossHeaderPredLabels << commentPredStr;
      itMapOfClassesValid = mapOfClassesValid.begin();
      while (itMapOfClassesValid != mapOfClassesValid.end())
        {
        // labels labelValid of mapOfClassesValid are already sorted
        labelValid = itMapOfClassesValid->first;
        otbAppLogINFO("mapOfClassesValid[" << labelValid << "] = " << itMapOfClassesValid->second);

        ossHeaderValidLabels << labelValid;
        ossHeaderPredLabels << labelValid;
        ++itMapOfClassesValid;
        if (itMapOfClassesValid != mapOfClassesValid.end())
          {
          ossHeaderValidLabels << separatorChar;
          ossHeaderPredLabels << separatorChar;
          }
        else
          {
          ossHeaderValidLabels << std::endl;
          ossHeaderPredLabels << std::endl;
          }
        }


      std::ofstream outFile;
      outFile.open(this->GetParameterString("io.confmatout").c_str());
      outFile << std::fixed;
      outFile.precision(10);


      /////////////////////////////////////
      // Writing the 2 headers
      outFile << ossHeaderValidLabels.str();
      outFile << ossHeaderPredLabels.str();
      /////////////////////////////////////

      unsigned int indiceLabelValid = 0, indiceLabelPred = 0;

      for (itMapOfClassesValid = mapOfClassesValid.begin(); itMapOfClassesValid != mapOfClassesValid.end(); ++itMapOfClassesValid)
        {
        // labels labelValid = itMapOfClassesValid->first of mapOfClassesRef are already sorted

        indiceLabelPred = 0;
        for (itMapOfClassesPred = mapOfClassesValid.begin(); itMapOfClassesPred != mapOfClassesValid.end(); ++itMapOfClassesPred)
          {
          // Writing the ordered confusion matrix in the output file
          outFile << confusionMatrix(indiceLabelValid, indiceLabelPred);
          if (indiceLabelPred < (nbClassesPred - 1))
            {
            outFile << separatorChar;
            }
          else
            {
            outFile << std::endl;
            }
          ++indiceLabelPred;
          }

        ++indiceLabelValid;
        }

      outFile.close();
      } // END if (this->HasValue("io.confmatout"))

  // TODO: implement hyperplane distance classifier and performance validation (cf. object detection) ?

}


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainImagesClassifier)
