/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"

#include "itkVariableLengthVector.h"
#include "otbStatisticsXMLFileReader.h"

#include "itkListSample.h"
#include "otbShiftScaleSampleListFilter.h"

#include "otbMachineLearningModelFactory.h"

#include "otbMachineLearningModel.h"

#include <time.h>

namespace otb
{
namespace Wrapper
{

/** Utility function to negate std::isalnum */
bool IsNotAlphaNum(char c)
  {
  return !std::isalnum(c);
  }

class VectorClassifier : public Application
{
public:
  /** Standard class typedefs. */
  typedef VectorClassifier              Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Self, Application)

  /** Filters typedef */
  typedef double                                        ValueType;
  typedef unsigned int                                  LabelType;
  typedef itk::FixedArray<LabelType,1>                  LabelSampleType;
  typedef itk::Statistics::ListSample<LabelSampleType>  LabelListSampleType;

  typedef otb::MachineLearningModel<ValueType,LabelType>          MachineLearningModelType;
  typedef otb::MachineLearningModelFactory<ValueType, LabelType>  MachineLearningModelFactoryType;
  typedef MachineLearningModelType::Pointer                       ModelPointerType;
  typedef MachineLearningModelType::ConfidenceListSampleType      ConfidenceListSampleType;

  /** Statistics Filters typedef */
  typedef itk::VariableLengthVector<ValueType>                    MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>           StatisticsReader;

  typedef itk::VariableLengthVector<ValueType>                    InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType>            ListSampleType;
  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  ~VectorClassifier() ITK_OVERRIDE
    {
    MachineLearningModelFactoryType::CleanFactories();
    }

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("VectorClassifier");
    SetDescription("Performs a classification of the input vector data according to a model file.");

    SetDocName("Vector Classification");
    SetDocAuthors("OTB-Team");
    SetDocLongDescription("This application performs a vector data classification based on a model file produced by the TrainVectorClassifier application.");
    SetDocLimitations("Only shapefiles are supported for now. SQLite format is not supported.");
    SetDocSeeAlso("TrainVectorClassifier");

    AddParameter(ParameterType_InputVectorData, "in", "Name of the input vector data");
    SetParameterDescription("in","The input vector data to classify.");

    AddParameter(ParameterType_InputFilename, "instat", "Statistics file");
    SetParameterDescription("instat", "A XML file containing mean and standard deviation to center"
      "and reduce samples before classification (produced by ComputeImagesStatistics application).");
    MandatoryOff("instat");

    AddParameter(ParameterType_InputFilename, "model", "Model file");
    SetParameterDescription("model", "A model file (produced by TrainVectorClassifier application,"
      "maximal class label = 65535).");

    AddParameter(ParameterType_String,"cfield","Field containing the predicted class");
    SetParameterDescription("cfield","Field containing the predicted class."
      "Only geometries with this field available will be taken into account.");
    SetParameterString("cfield","predicted", false);

    AddParameter(ParameterType_ListView, "feat", "Field names to be calculated."); //
    SetParameterDescription("feat","List of field names in the input vector data used as features for training."); //

    AddParameter(ParameterType_Empty, "confmap",  "Confidence map");
    SetParameterDescription( "confmap", "Confidence map of the produced classification. The confidence index depends on the model : \n"
      "  - LibSVM : difference between the two highest probabilities (needs a model with probability estimates, so that classes probabilities can be computed for each sample)\n"
      "  - OpenCV\n"
      "    * Boost : sum of votes\n"
      "    * DecisionTree : (not supported)\n"
      "    * GradientBoostedTree : (not supported)\n"
      "    * KNearestNeighbors : number of neighbors with the same label\n"
      "    * NeuralNetwork : difference between the two highest responses\n"
      "    * NormalBayes : (not supported)\n"
      "    * RandomForest : Confidence (proportion of votes for the majority class). Margin (normalized difference of the votes of the 2 majority classes) is not available for now.\n"
      "    * SVM : distance to margin (only works for 2-class models)\n");
    MandatoryOff("confmap");

    AddParameter(ParameterType_OutputFilename, "out", "Output vector data file containing class labels");
    SetParameterDescription("out","Output vector data file storing sample values (OGR format)."
      "If not given, the input vector data file is updated.");
    MandatoryOff("out");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "vectorData.shp");
    SetDocExampleParameterValue("instat", "meanVar.xml");
    SetDocExampleParameterValue("model", "svmModel.svm");
    SetDocExampleParameterValue("out", "svmModel.svm");
    SetDocExampleParameterValue("feat", "perimeter  area  width");
    SetDocExampleParameterValue("cfield", "predicted");
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    if ( HasValue("in") )
    {
      std::string shapefile = GetParameterString("in");

      otb::ogr::DataSource::Pointer ogrDS;
      otb::ogr::Layer layer(ITK_NULLPTR, false);

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
        std::string::iterator end = std::remove_if(key.begin(),key.end(),IsNotAlphaNum);
        std::transform(key.begin(), end, key.begin(), tolower);

        OGRFieldType fieldType = feature.ogr().GetFieldDefnRef(iField)->GetType();
        if(fieldType == OFTInteger ||  ogr::version_proxy::IsOFTInteger64(fieldType) || fieldType == OFTReal)
          {
          std::string tmpKey="feat."+key.substr(0, end - key.begin());
          AddChoice(tmpKey,item);
          }
      }
    }
  }

  void DoExecute() ITK_OVERRIDE
  {
    clock_t tic = clock();

    std::string shapefile = GetParameterString("in").c_str();

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
        MeasurementType mv;
        mv.SetSize(nbFeatures);

        for(int idx=0; idx < nbFeatures; ++idx)
        {
          mv[idx] = feature.ogr().GetFieldAsDouble(GetSelectedItems("feat")[idx]);
        }
        input->PushBack(mv);
        feature = layer.ogr().GetNextFeature();
        goesOn = feature.addr() != ITK_NULLPTR;
      }

    // Statistics for shift/scale
    MeasurementType meanMeasurementVector;
    MeasurementType stddevMeasurementVector;
    if (HasValue("instat") && IsParameterEnabled("instat"))
      {
      StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
      std::string XMLfile = GetParameterString("instat");
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

    ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
    trainingShiftScaleFilter->SetInput(input);
    trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
    trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
    trainingShiftScaleFilter->Update();
    otbAppLogINFO("mean used: " << meanMeasurementVector);
    otbAppLogINFO("standard deviation used: " << stddevMeasurementVector);

    otbAppLogINFO("Loading model");
    m_Model = MachineLearningModelFactoryType::CreateMachineLearningModel(GetParameterString("model"),
                                                MachineLearningModelFactoryType::ReadMode);

    if (m_Model.IsNull())
      {
      otbAppLogFATAL(<< "Error when loading model " << GetParameterString("model") << " : unsupported model type");
      }

    m_Model->Load(GetParameterString("model"));
    otbAppLogINFO("Model loaded");

    ListSampleType::Pointer listSample;
    listSample = trainingShiftScaleFilter->GetOutput();

    typename ConfidenceListSampleType::Pointer quality;

    bool computeConfidenceMap(IsParameterEnabled("confmap") && m_Model->HasConfidenceIndex() 
                              && !m_Model->GetRegressionMode());

    if (!m_Model->HasConfidenceIndex() && IsParameterEnabled("confmap"))
      {
      otbAppLogWARNING("Confidence map requested but the classifier doesn't support it!");
      }

    if (computeConfidenceMap)
      {
      quality = ConfidenceListSampleType::New();
      target = m_Model->PredictBatch(listSample, quality);
      }
      else
      {
      target = m_Model->PredictBatch(listSample);
      }

    LabelListSampleType::Pointer labelListSample = target;

    ogr::DataSource::Pointer output;
    otb::ogr::Layer outLayer(ITK_NULLPTR, false);

    if (IsParameterEnabled("out") && HasValue("out"))
      {
      // Copy input layer
      output = ogr::DataSource::New(GetParameterString("out"), ogr::DataSource::Modes::Overwrite);
      outLayer = output->CopyLayer(layer, GetParameterString("out").c_str());
      }
    else
      {
      // Update mode
      otbAppLogINFO("Update input vector data.");
      output = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Update_LayerUpdate);
      outLayer = output->GetLayer(0);
      }

    OGRErr errStart = outLayer.ogr().StartTransaction();
    if (errStart != OGRERR_NONE)
      {
      itkExceptionMacro(<< "Unable to start transaction for OGR layer " << outLayer.ogr().GetName() << ".");
      }

    // First get list of current fields
    OGRFeatureDefn &layerDefn = outLayer.GetLayerDefn();
    std::map<std::string, OGRFieldType> currentFields;
    for (int k=0 ; k<layerDefn.GetFieldCount() ; k++)
      {
      OGRFieldDefn fieldDefn(layerDefn.GetFieldDefn(k));
      std::string currentName(fieldDefn.GetNameRef());
      currentFields[currentName] = fieldDefn.GetType();
      }

    // Add the field of prediction in the output layer if field not exist
    OGRFieldDefn predictedField(GetParameterString("cfield").c_str(), OFTInteger);
    ogr::FieldDefn predictedFieldDef(predictedField);
    //test if field is already present
    if (currentFields.count(predictedFieldDef.GetName()))
    {
      // test the field type
      if (currentFields[predictedFieldDef.GetName()] != predictedFieldDef.GetType())
        itkExceptionMacro("Field name "<< predictedFieldDef.GetName() << " already exists with a different type!");
    }
    else
    {
      outLayer.CreateField(predictedFieldDef);
    }

    // Add confidence field in the output layer
    if (computeConfidenceMap)
     {
      OGRFieldDefn confidenceField("confidence", OFTReal);
      confidenceField.SetWidth(confidenceField.GetWidth());
      confidenceField.SetPrecision(confidenceField.GetPrecision());
      ogr::FieldDefn confFieldDefn(confidenceField);
      //test if field is already present
      if (currentFields.count(confFieldDefn.GetName()))
      {
        // test the field type
        if (currentFields[confFieldDefn.GetName()] != confFieldDefn.GetType())
          itkExceptionMacro("Field name "<< confFieldDefn.GetName() << " already exists with a different type!");
      }
      else
      {
        outLayer.CreateField(confFieldDefn);
      }
     }

    bool goesOn2 = true;
    unsigned int count=0;
    outLayer.ogr().ResetReading();
    otb::ogr::Feature feature2 = outLayer.ogr().GetNextFeature();
    std::string classfieldname = GetParameterString("cfield");

    if(feature2.addr())
      while(goesOn2)
       {
        feature2.ogr().SetField(classfieldname.c_str(),(int)labelListSample->GetMeasurementVector(count)[0]);
        if (computeConfidenceMap) feature2.ogr().SetField("confidence",(int)quality->GetMeasurementVector(count)[0]);
        outLayer.SetFeature(feature2);
        feature2 = outLayer.ogr().GetNextFeature();
        goesOn2 = feature2.addr() != ITK_NULLPTR;
        count++;
       }

    if(outLayer.ogr().TestCapability("Transactions"))
      {
      const OGRErr errCommitX = outLayer.ogr().CommitTransaction();
      if (errCommitX != OGRERR_NONE)
        {
        itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << outLayer.ogr().GetName() << ".");
        }
      }

    output->SyncToDisk();

    clock_t toc = clock();
    otbAppLogINFO( "Elapsed: "<< ((double)(toc - tic) / CLOCKS_PER_SEC)<<" seconds.");

  }

  ModelPointerType m_Model;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorClassifier)
