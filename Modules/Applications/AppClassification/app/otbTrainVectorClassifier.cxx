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

#include "otbLearningApplicationBase.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"
#include "otbStatisticsXMLFileWriter.h"

#include "itkVariableLengthVector.h"
#include "otbStatisticsXMLFileReader.h"

#include "itkListSample.h"
#include "otbShiftScaleSampleListFilter.h"

// Validation
#include "otbConfusionMatrixCalculator.h"

#include <algorithm>
#include <locale>

namespace otb
{
namespace Wrapper
{

/** Utility function to negate std::isalnum */
bool IsNotAlphaNum(char c)
  {
  return !std::isalnum(c);
  }

class TrainVectorClassifier : public LearningApplicationBase<float,int>
{
public:
  typedef TrainVectorClassifier Self;
  typedef LearningApplicationBase<float, int> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self)

  itkTypeMacro(Self, Superclass)

  typedef Superclass::SampleType              SampleType;
  typedef Superclass::ListSampleType          ListSampleType;
  typedef Superclass::TargetListSampleType    TargetListSampleType;
  typedef Superclass::SampleImageType         SampleImageType;
  
  typedef double ValueType;
  typedef itk::VariableLengthVector<ValueType> MeasurementType;

  typedef otb::StatisticsXMLFileReader<SampleType> StatisticsReader;

  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  // Estimate performance on validation sample
  typedef otb::ConfusionMatrixCalculator<TargetListSampleType, TargetListSampleType> ConfusionMatrixCalculatorType;
  typedef ConfusionMatrixCalculatorType::ConfusionMatrixType ConfusionMatrixType;
  typedef ConfusionMatrixCalculatorType::MapOfIndicesType MapOfIndicesType;
  typedef ConfusionMatrixCalculatorType::ClassLabelType ClassLabelType;

private:
  void DoInit()
  {
    SetName("TrainVectorClassifier");
    SetDescription("Train a classifier based on labeled geometries and a list of features to consider.");

    SetDocName("Train Vector Classifier");
    SetDocLongDescription("This application trains a classifier based on "
      "labeled geometries and a list of features to consider for classification.");
    SetDocLimitations(" ");
    SetDocAuthors("OTB Team");
    SetDocSeeAlso(" ");
   
    //Group IO
    AddParameter(ParameterType_Group, "io", "Input and output data");
    SetParameterDescription("io", "This group of parameters allows setting input and output data.");

    AddParameter(ParameterType_InputVectorDataList, "io.vd", "Input Vector Data");
    SetParameterDescription("io.vd", "Input geometries used for training (note : all geometries from the layer will be used)");

    AddParameter(ParameterType_InputFilename, "io.stats", "Input XML image statistics file");
    MandatoryOff("io.stats");
    SetParameterDescription("io.stats", "XML file containing mean and variance of each feature.");

    AddParameter(ParameterType_OutputFilename, "io.confmatout", "Output confusion matrix");
    SetParameterDescription("io.confmatout", "Output file containing the confusion matrix (.csv format).");
    MandatoryOff("io.confmatout");

    AddParameter(ParameterType_OutputFilename, "io.out", "Output model");
    SetParameterDescription("io.out", "Output file containing the model estimated (.txt format).");

    AddParameter(ParameterType_ListView,  "feat", "Field names for training features.");
    SetParameterDescription("feat","List of field names in the input vector data to be used as features for training.");

    AddParameter(ParameterType_String,"cfield","Field containing the class id for supervision");
    SetParameterDescription("cfield","Field containing the class id for supervision. "
      "Only geometries with this field available will be taken into account.");
    SetParameterString("cfield","class");

    AddParameter(ParameterType_Int, "layer", "Layer Index");
    SetParameterDescription("layer", "Index of the layer to use in the input vector file.");
    MandatoryOff("layer");
    SetDefaultParameterInt("layer",0);

    AddParameter(ParameterType_Group, "valid", "Validation data");
    SetParameterDescription("valid", "This group of parameters defines validation data.");

    AddParameter(ParameterType_InputVectorDataList, "valid.vd", "Validation Vector Data");
    SetParameterDescription("valid.vd", "Geometries used for validation "
      "(must contain the same fields used for training, all geometries from the layer will be used)");
    MandatoryOff("valid.vd");

    AddParameter(ParameterType_Int, "valid.layer", "Layer Index");
    SetParameterDescription("valid.layer", "Index of the layer to use in the validation vector file.");
    MandatoryOff("valid.layer");
    SetDefaultParameterInt("valid.layer",0);

    // Add parameters for the classifier choice
    Superclass::DoInit();

    AddRANDParameter();
    // Doc example parameter settings
    SetDocExampleParameterValue("io.vd", "vectorData.shp");
    SetDocExampleParameterValue("io.stats", "meanVar.xml");
    SetDocExampleParameterValue("io.out", "svmModel.svm");
    SetDocExampleParameterValue("feat", "perimeter  area  width");
    SetDocExampleParameterValue("cfield", "predicted");
  }

  void DoUpdateParameters()
  {
    if ( HasValue("io.vd") )
      {
      std::vector<std::string> vectorFileList = GetParameterStringList("io.vd");
      ogr::DataSource::Pointer ogrDS =
        ogr::DataSource::New(vectorFileList[0], ogr::DataSource::Modes::Read);
      ogr::Layer layer = ogrDS->GetLayer(this->GetParameterInt("layer"));
      ogr::Feature feature = layer.ogr().GetNextFeature();

      ClearChoices("feat");
      for(int iField=0; iField<feature.ogr().GetFieldCount(); iField++)
        {
        std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
        key = item;
        std::string::iterator end = std::remove_if(key.begin(),key.end(),IsNotAlphaNum);
        std::transform(key.begin(), end, key.begin(), tolower);
        key="feat."+key.substr(0, end - key.begin());
        AddChoice(key,item);
        }
      }
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
      os << matrix(i, j);
      size_t size = os.str().size();

      if (size > minwidth)
        {
        minwidth = size;
        }
      }
    }

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


void DoExecute()
  {
  typedef int LabelPixelType;
  typedef itk::FixedArray<LabelPixelType,1> LabelSampleType;
  typedef itk::Statistics::ListSample <LabelSampleType> LabelListSampleType;

  // Prepare selected field names (their position may change between two inputs)
  std::vector<int> selectedIdx = GetSelectedItems("feat");
  const unsigned int nbFeatures = selectedIdx.size();
  std::vector<std::string> fieldNames = GetChoiceNames("feat");
  std::vector<std::string> selectedNames(nbFeatures);
  for (unsigned int i=0 ; i<nbFeatures ; i++)
    {
    selectedNames[i] = fieldNames[selectedIdx[i]];
    }
  std::vector<int> featureFieldIndex(nbFeatures, -1);
  int cFieldIndex = -1;

  // List of available fields
  std::ostringstream oss;
  for (unsigned int i=0 ; i<fieldNames.size() ; i++)
    {
    if (i) oss << ", ";
    oss << fieldNames[i];
    }
  std::string availableFields(oss.str());

  // Statistics for shift/scale
  MeasurementType meanMeasurementVector;
  MeasurementType stddevMeasurementVector;
  if (HasValue("io.stats") && IsParameterEnabled("io.stats"))
    {
    StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
    std::string XMLfile = GetParameterString("io.stats");
    statisticsReader->SetFileName(XMLfile);
    meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
    stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
    }
  else
    {
    meanMeasurementVector.SetSize(nbFeatures);
    meanMeasurementVector.Fill(0.);
    stddevMeasurementVector.SetSize(nbFeatures);
    stddevMeasurementVector.Fill(1.);
    }

  ListSampleType::Pointer input = ListSampleType::New();
  LabelListSampleType::Pointer target = LabelListSampleType::New();
  input->SetMeasurementVectorSize(nbFeatures);

  std::vector<std::string> vectorFileList = GetParameterStringList("io.vd");
  for (unsigned int k=0 ; k<vectorFileList.size() ; k++)
    {
    otbAppLogINFO("Reading input vector file "<<k+1<<"/"<<vectorFileList.size());
    ogr::DataSource::Pointer source = ogr::DataSource::New(vectorFileList[k], ogr::DataSource::Modes::Read);
    ogr::Layer layer = source->GetLayer(this->GetParameterInt("layer"));
    ogr::Feature feature = layer.ogr().GetNextFeature();
    bool goesOn = feature.addr() != 0;
    if (!goesOn)
      {
      otbAppLogWARNING("The layer "<<GetParameterInt("layer")<<" of "
        <<vectorFileList[k]<<" is empty, input is skipped.");
      continue;
      }

    // Check all needed fields are present :
    //   - check class field
    cFieldIndex = feature.ogr().GetFieldIndex(GetParameterString("cfield").c_str());
    if (cFieldIndex < 0)
      otbAppLogFATAL("The field name for class label ("<<GetParameterString("cfield")
        <<") has not been found in the input vector file! Choices are "<< availableFields);
    //   - check feature fields
    for (unsigned int i=0 ; i<nbFeatures ; i++)
      {
      featureFieldIndex[i] = feature.ogr().GetFieldIndex(selectedNames[i].c_str());
      if (featureFieldIndex[i] < 0)
        otbAppLogFATAL("The field name for feature "<<selectedNames[i]
        <<" has not been found in the input vector file! Choices are "<< availableFields);
      }

    while(goesOn)
      {
      if(feature.ogr().IsFieldSet(cFieldIndex))
        {
        MeasurementType mv;
        mv.SetSize(nbFeatures);
        for(unsigned int idx=0; idx < nbFeatures; ++idx)
          mv[idx] = feature.ogr().GetFieldAsDouble(featureFieldIndex[idx]);

        input->PushBack(mv);
        target->PushBack(feature.ogr().GetFieldAsInteger(cFieldIndex));
        }
      feature = layer.ogr().GetNextFeature();
      goesOn = feature.addr() != 0;
      }
    }

  ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
  trainingShiftScaleFilter->SetInput(input);
  trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
  trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
  trainingShiftScaleFilter->Update();

  ListSampleType::Pointer trainingListSample= trainingShiftScaleFilter->GetOutput();
  TargetListSampleType::Pointer trainingLabeledListSample = target;

  //--------------------------
  // Estimate model
  //--------------------------
  this->Train(trainingListSample,trainingLabeledListSample,GetParameterString("io.out"));

  //--------------------------
  // Performances estimation
  //--------------------------
  ListSampleType::Pointer validationListSample=ListSampleType::New();
  TargetListSampleType::Pointer validationLabeledListSample = TargetListSampleType::New();

  // Import validation data
  if (HasValue("valid.vd") && IsParameterEnabled("valid.vd"))
    {
    input = ListSampleType::New();
    target = LabelListSampleType::New();
    input->SetMeasurementVectorSize(nbFeatures);

    std::vector<std::string> validFileList = this->GetParameterStringList("valid.vd");
    for (unsigned int k=0 ; k<validFileList.size() ; k++)
      {
      otbAppLogINFO("Reading validation vector file "<<k+1<<"/"<<validFileList.size());
      ogr::DataSource::Pointer source = ogr::DataSource::New(validFileList[k], ogr::DataSource::Modes::Read);
      ogr::Layer layer = source->GetLayer(this->GetParameterInt("valid.layer"));
      ogr::Feature feature = layer.ogr().GetNextFeature();
      bool goesOn = feature.addr() != 0;
      if (!goesOn)
        {
        otbAppLogWARNING("The layer "<<GetParameterInt("valid.layer")<<" of "
          <<validFileList[k]<<" is empty, input is skipped.");
        continue;
        }

      // Check all needed fields are present :
      //   - check class field
      cFieldIndex = feature.ogr().GetFieldIndex(GetParameterString("cfield").c_str());
      if (cFieldIndex < 0)
        otbAppLogFATAL("The field name for class label ("<<GetParameterString("cfield")
          <<") has not been found in the input vector file! Choices are "<< availableFields);
      //   - check feature fields
      for (unsigned int i=0 ; i<nbFeatures ; i++)
        {
        featureFieldIndex[i] = feature.ogr().GetFieldIndex(selectedNames[i].c_str());
        if (featureFieldIndex[i] < 0)
          otbAppLogFATAL("The field name for feature "<<selectedNames[i]
          <<" has not been found in the input vector file! Choices are "<< availableFields);
        }

      while(goesOn)
        {
        if(feature.ogr().IsFieldSet(cFieldIndex))
          {
          MeasurementType mv;
          mv.SetSize(nbFeatures);
          for(unsigned int idx=0; idx < nbFeatures; ++idx)
            mv[idx] = feature.ogr().GetFieldAsDouble(featureFieldIndex[idx]);

          input->PushBack(mv);
          target->PushBack(feature.ogr().GetFieldAsInteger(cFieldIndex));
          }
        feature = layer.ogr().GetNextFeature();
        goesOn = feature.addr() != 0;
        }
      }

    ShiftScaleFilterType::Pointer validShiftScaleFilter = ShiftScaleFilterType::New();
    validShiftScaleFilter->SetInput(input);
    validShiftScaleFilter->SetShifts(meanMeasurementVector);
    validShiftScaleFilter->SetScales(stddevMeasurementVector);
    validShiftScaleFilter->Update();

    validationListSample = validShiftScaleFilter->GetOutput();
    validationLabeledListSample = target;
    }
 
  //Test the input validation set size
  TargetListSampleType::Pointer predictedList = TargetListSampleType::New();
  ListSampleType::Pointer performanceListSample;
  TargetListSampleType::Pointer performanceLabeledListSample;
  if(validationLabeledListSample->Size() != 0)
    {
    performanceListSample = validationListSample;
    performanceLabeledListSample = validationLabeledListSample;
    }
  else
    {
    otbAppLogWARNING("The validation set is empty. The performance estimation is done using the input training set in this case.");
    performanceListSample = trainingListSample;
    performanceLabeledListSample = trainingLabeledListSample;
    }

  this->Classify(performanceListSample, predictedList, GetParameterString("io.out"));

  ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

  otbAppLogINFO("Predicted list size : " << predictedList->Size());
  otbAppLogINFO("ValidationLabeledListSample size : " << performanceLabeledListSample->Size());
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

    MapOfIndicesType::iterator itMapOfIndicesValid, itMapOfIndicesPred;
    ClassLabelType labelValid = 0;

    ConfusionMatrixType confusionMatrix = confMatCalc->GetConfusionMatrix();
    MapOfIndicesType mapOfIndicesValid = confMatCalc->GetMapOfIndices();

    unsigned int nbClassesPred = mapOfIndicesValid.size();

    /////////////////////////////////////////////
    // Filling the 2 headers for the output file
    const std::string commentValidStr = "#Reference labels (rows):";
    const std::string commentPredStr = "#Produced labels (columns):";
    const char separatorChar = ',';
    std::ostringstream ossHeaderValidLabels, ossHeaderPredLabels;

    // Filling ossHeaderValidLabels and ossHeaderPredLabels for the output file
    ossHeaderValidLabels << commentValidStr;
    ossHeaderPredLabels << commentPredStr;

    itMapOfIndicesValid = mapOfIndicesValid.begin();

    while (itMapOfIndicesValid != mapOfIndicesValid.end())
      {
      // labels labelValid of mapOfIndicesValid are already sorted in otbConfusionMatrixCalculator
      labelValid = itMapOfIndicesValid->second;

      otbAppLogINFO("mapOfIndicesValid[" << itMapOfIndicesValid->first << "] = " << labelValid);

      ossHeaderValidLabels << labelValid;
      ossHeaderPredLabels << labelValid;

      ++itMapOfIndicesValid;

      if (itMapOfIndicesValid != mapOfIndicesValid.end())
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

    unsigned int indexLabelValid = 0, indexLabelPred = 0;

    for (itMapOfIndicesValid = mapOfIndicesValid.begin(); itMapOfIndicesValid != mapOfIndicesValid.end(); ++itMapOfIndicesValid)
      {
      indexLabelPred = 0;

      for (itMapOfIndicesPred = mapOfIndicesValid.begin(); itMapOfIndicesPred != mapOfIndicesValid.end(); ++itMapOfIndicesPred)
        {
        // Writing the confusion matrix (sorted in otbConfusionMatrixCalculator) in the output file
        outFile << confusionMatrix(indexLabelValid, indexLabelPred);
        if (indexLabelPred < (nbClassesPred - 1))
          {
          outFile << separatorChar;
          }
        else
          {
          outFile << std::endl;
          }
        ++indexLabelPred;
        }

      ++indexLabelValid;
      }

    outFile.close();
    } // END if (this->HasValue("io.confmatout"))
  }

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainVectorClassifier)
