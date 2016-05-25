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

namespace otb
{
namespace Wrapper
{
class TrainVectorClassifier : public LearningApplicationBase<float,int>
{
public:
  typedef TrainVectorClassifier Self;
  typedef LearningApplicationBase Superclass;
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
    SetName("TrainOGRLayersClassifier");
    SetDescription("Train a SVM classifier based on labeled geometries and a list of features to consider.");

    SetDocName("TrainOGRLayersClassifier");
    SetDocLongDescription("This application trains a SVM classifier based on labeled geometries and a list of features to consider for classification.");
    SetDocLimitations("Experimental. For now only shapefiles are supported. Tuning of SVM classifier is not available.");
    SetDocAuthors("David Youssefi during internship at CNES");
    SetDocSeeAlso("OGRLayerClassifier,ComputeOGRLayersFeaturesStatistics");
    AddDocTag(Tags::Segmentation);
   
    //Group IO
    AddParameter(ParameterType_Group, "io", "Input and output data");
    SetParameterDescription("io", "This group of parameters allows setting input and output data.");

    AddParameter(ParameterType_InputVectorData, "io.vd", "Input Vector Data");

    AddParameter(ParameterType_InputFilename, "io.stats", "Input XML image statistics file");
    MandatoryOff("io.stats");
    SetParameterDescription("io.stats", "XML file containing mean and variance of each feature.");

    AddParameter(ParameterType_OutputFilename, "io.confmatout", "Output confusion matrix");
    SetParameterDescription("io.confmatout", "Output file containing the confusion matrix (.csv format).");
    MandatoryOff("io.confmatout");

    AddParameter(ParameterType_OutputFilename, "io.out", "Output model");
    SetParameterDescription("io.out", "Output file containing the model estimated (.txt format).");

    AddParameter(ParameterType_ListView,  "feat", "List of features to consider for classification.");
    SetParameterDescription("feat","List of features to consider for classification.");

    AddParameter(ParameterType_String,"cfield","Field containing the class id for supervision");
    SetParameterDescription("cfield","Field containing the class id for supervision. Only geometries with this field available will be taken into account.");
    SetParameterString("cfield","class");


    // Doc example parameter settings
    SetDocExampleParameterValue("io.vd", "vectorData.shp");
    SetDocExampleParameterValue("io.stats", "meanVar.xml");
    SetDocExampleParameterValue("io.out", "svmModel.svm");
    SetDocExampleParameterValue("feat", "perimeter");
    SetDocExampleParameterValue("cfield", "predicted");

    Superclass::DoInit();
  }

  void DoUpdateParameters()
  {
    if ( HasValue("io.vd") )
      {
      std::string shapefile = GetParameterString("io.vd");

       otb::ogr::DataSource::Pointer ogrDS;
       otb::ogr::Layer layer(NULL, false);

       OGRSpatialReference oSRS("");
       std::vector<std::string> options;

       ogrDS = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
       std::string layername = itksys::SystemTools::GetFilenameName(shapefile);
       layername = layername.substr(0,layername.size()-4);
       layer = ogrDS->GetLayer(0);

       otb::ogr::Feature feature = layer.ogr().GetNextFeature();
       ClearChoices("feat");
       for(int iField=0; iField<feature.ogr().GetFieldCount(); iField++)
         {
           std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
           key = item;
           key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
           std::transform(key.begin(), key.end(), key.begin(), tolower);
           key="feat."+key;
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
    std::string shapefile = GetParameterString("io.vd");
    std::string XMLfile = GetParameterString("io.stats");
    std::string modelfile = GetParameterString("io.out");
    typedef int LabelPixelType;
    typedef itk::FixedArray<LabelPixelType,1> LabelSampleType;
    typedef itk::Statistics::ListSample <LabelSampleType> LabelListSampleType;

    StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
    statisticsReader->SetFileName(XMLfile);

    MeasurementType meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
    MeasurementType stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");

    otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
    otb::ogr::Layer layer = source->GetLayer(0);

    bool goesOn = true;
    
    otb::ogr::Feature feature = layer.ogr().GetNextFeature();

    ListSampleType::Pointer input = ListSampleType::New();
    LabelListSampleType::Pointer target = LabelListSampleType::New();
    const int nbFeatures = GetSelectedItems("feat").size();

    input->SetMeasurementVectorSize(nbFeatures);

    if(feature.addr())
      while(goesOn)
       {
        if(feature.ogr().IsFieldSet(feature.ogr().GetFieldIndex(GetParameterString("cfield").c_str())))
           {
             MeasurementType mv; mv.SetSize(nbFeatures);

             for(int idx=0; idx < nbFeatures; ++idx)
              mv[idx] = feature.ogr().GetFieldAsDouble(GetSelectedItems("feat")[idx]);

             input->PushBack(mv);
             target->PushBack(feature.ogr().GetFieldAsInteger(GetParameterString("cfield").c_str()));
           }
         feature = layer.ogr().GetNextFeature();
         goesOn = feature.addr() != 0;
       }

    ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
    trainingShiftScaleFilter->SetInput(input);
    trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
    trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
    trainingShiftScaleFilter->Update();

    ListSampleType::Pointer listSample;
    LabelListSampleType::Pointer labelListSample;

    listSample = trainingShiftScaleFilter->GetOutput();
    labelListSample = target;

    ListSampleType::Pointer trainingListSample = listSample;
    LabelListSampleType::Pointer trainingLabeledListSample = labelListSample;

  //--------------------------
  // Estimate model
  //--------------------------
  this->Train(trainingListSample,trainingLabeledListSample,GetParameterString("io.out"));


  //--------------------------
  // Performances estimation
  //--------------------------
  TargetListSampleType::Pointer predictedList = TargetListSampleType::New();
  ListSampleType::Pointer performanceListSample = ListSampleType::New();
  TargetListSampleType::Pointer performanceLabeledListSample = TargetListSampleType::New();

  ListSampleType::Pointer validationListSample=ListSampleType::New();
  TargetListSampleType::Pointer validationLabeledListSample = TargetListSampleType::New();
 
  //Test the input validation set size
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
