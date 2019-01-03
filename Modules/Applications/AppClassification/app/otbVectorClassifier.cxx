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
  typedef float                                         ValueType;
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

  ~VectorClassifier() override
    {
    MachineLearningModelFactoryType::CleanFactories();
    }

private:
  void DoInit() override
  {
    SetName("VectorClassifier");
    SetDescription("Performs a classification of the input vector data according to a model file.");

    SetDocName("Vector Classification");
    SetDocAuthors("OTB-Team");
    SetDocLongDescription("This application performs a vector data classification "
      "based on a model file produced by the TrainVectorClassifier application."
      "Features of the vector data output will contain the class labels decided by the classifier "
      "(maximal class label = 65535). \n"
      "There are two modes: \n"
        "1) Update mode: add of the 'cfield' field containing the predicted class in the input file. \n"
        "2) Write mode: copies the existing fields of the input file in the output file "
           " and add the 'cfield' field containing the predicted class. \n"
      "If you have declared the output file, the write mode applies. "
      "Otherwise, the input file update mode will be applied.");

    SetDocLimitations("Shapefiles are supported. But the SQLite format is only supported in update mode.");
    SetDocSeeAlso("TrainVectorClassifier");
    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputVectorData, "in", "Name of the input vector data");
    SetParameterDescription("in","The input vector data file to classify.");

    AddParameter(ParameterType_InputFilename, "instat", "Statistics file");
    SetParameterDescription("instat", "A XML file containing mean and standard deviation to center"
      "and reduce samples before classification, produced by ComputeImagesStatistics application.");
    MandatoryOff("instat");

    AddParameter(ParameterType_InputFilename, "model", "Model file");
    SetParameterDescription("model", "Model file produced by TrainVectorClassifier application.");

    AddParameter(ParameterType_String,"cfield","Field class");
    SetParameterDescription("cfield","Field containing the predicted class."
      "Only geometries with this field available will be taken into account.\n"
      "The field is added either in the input file (if 'out' off) or in the output file.\n"
      "Caution, the 'cfield' must not exist in the input file if you are updating the file.");
    SetParameterString("cfield","predicted");

    AddParameter(ParameterType_ListView, "feat", "Field names to be calculated.");
    SetParameterDescription("feat","List of field names in the input vector data used as features for training. "
      "Put the same field names as the TrainVectorClassifier application.");

    AddParameter(ParameterType_Bool, "confmap",  "Confidence map");
    SetParameterDescription( "confmap", "Confidence map of the produced classification. The confidence index depends on the model: \n\n"
      "* LibSVM: difference between the two highest probabilities (needs a model with probability estimates, so that classes probabilities can be computed for each sample)\n"
      "* Boost: sum of votes\n"
      "* DecisionTree: (not supported)\n"
      "* GradientBoostedTree: (not supported)\n"
      "* KNearestNeighbors: number of neighbors with the same label\n"
      "* NeuralNetwork: difference between the two highest responses\n"
      "* NormalBayes: (not supported)\n"
      "* RandomForest: Confidence (proportion of votes for the majority class). Margin (normalized difference of the votes of the 2 majority classes) is not available for now.\n"
      "* SVM: distance to margin (only works for 2-class models)\n");

    AddParameter(ParameterType_OutputFilename, "out", "Output vector data file containing class labels");
    SetParameterDescription("out","Output vector data file storing sample values (OGR format)."
      "If not given, the input vector data file is updated.");
    MandatoryOff("out");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "vectorData.shp");
    SetDocExampleParameterValue("instat", "meanVar.xml");
    SetDocExampleParameterValue("model", "svmModel.svm");
    SetDocExampleParameterValue("out", "vectorDataLabeledVector.shp");
    SetDocExampleParameterValue("feat", "perimeter  area  width");
    SetDocExampleParameterValue("cfield", "predicted");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    if ( HasValue("in") )
    {
      std::string shapefile = GetParameterString("in");

      otb::ogr::DataSource::Pointer ogrDS;

      OGRSpatialReference oSRS("");
      std::vector<std::string> options;

      ogrDS = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
      otb::ogr::Layer layer = ogrDS->GetLayer(0);
      OGRFeatureDefn &layerDefn = layer.GetLayerDefn();

      ClearChoices("feat");

      for(int iField=0; iField< layerDefn.GetFieldCount(); iField++)
      {
        std::string item = layerDefn.GetFieldDefn(iField)->GetNameRef();
        std::string key(item);
        key.erase( std::remove_if(key.begin(),key.end(),IsNotAlphaNum), key.end());
        std::transform(key.begin(), key.end(), key.begin(), tolower);

        OGRFieldType fieldType = layerDefn.GetFieldDefn(iField)->GetType();
        if(fieldType == OFTInteger ||  fieldType == OFTInteger64 || fieldType == OFTReal)
          {
          std::string tmpKey="feat."+key;
          AddChoice(tmpKey,item);
          }
      }
    }
  }

  void DoExecute() override
  {
    clock_t tic = clock();

    std::string shapefile = GetParameterString("in");

    otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
    otb::ogr::Layer layer = source->GetLayer(0);

    ListSampleType::Pointer input = ListSampleType::New();

    const int nbFeatures = GetSelectedItems("feat").size();
    input->SetMeasurementVectorSize(nbFeatures);
  
    otb::ogr::Layer::const_iterator it = layer.cbegin();
    otb::ogr::Layer::const_iterator itEnd = layer.cend();
    for( ; it!=itEnd ; ++it)
      {
      MeasurementType mv;
      mv.SetSize(nbFeatures);
      for(int idx=0; idx < nbFeatures; ++idx)
        {
        // Beware that itemIndex differs from ogr layer field index
        unsigned int itemIndex = GetSelectedItems("feat")[idx];
        std::string fieldName = GetChoiceNames( "feat" )[itemIndex];
        
        mv[idx] = static_cast<ValueType>((*it)[fieldName].GetValue<double>());
        }
      input->PushBack(mv);
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

    ListSampleType::Pointer listSample = trainingShiftScaleFilter->GetOutput();

    ConfidenceListSampleType::Pointer quality;

    bool computeConfidenceMap(GetParameterInt("confmap") && m_Model->HasConfidenceIndex() 
                              && !m_Model->GetRegressionMode());

    if (!m_Model->HasConfidenceIndex() && GetParameterInt("confmap"))
      {
      otbAppLogWARNING("Confidence map requested but the classifier doesn't support it!");
      }

    LabelListSampleType::Pointer target;
    if (computeConfidenceMap)
      {
      quality = ConfidenceListSampleType::New();
      target = m_Model->PredictBatch(listSample, quality);
      }
      else
      {
      target = m_Model->PredictBatch(listSample);
      }

    ogr::DataSource::Pointer output;
    ogr::DataSource::Pointer buffer = ogr::DataSource::New();
    bool updateMode = false;
    if (IsParameterEnabled("out") && HasValue("out"))
      {
      // Create new OGRDataSource
      output = ogr::DataSource::New(GetParameterString("out"), ogr::DataSource::Modes::Overwrite);
      otb::ogr::Layer newLayer = output->CreateLayer(
        GetParameterString("out"),
        const_cast<OGRSpatialReference*>(layer.GetSpatialRef()),
        layer.GetGeomType());
      // Copy existing fields
      OGRFeatureDefn &inLayerDefn = layer.GetLayerDefn();
      for (int k=0 ; k<inLayerDefn.GetFieldCount() ; k++)
        {
        OGRFieldDefn fieldDefn(inLayerDefn.GetFieldDefn(k));
        newLayer.CreateField(fieldDefn);
        }
      }
    else
      {
      // Update mode
      updateMode = true;
      otbAppLogINFO("Update input vector data.");
      // fill temporary buffer for the transfer
      otb::ogr::Layer inputLayer = layer;
      layer = buffer->CopyLayer(inputLayer, std::string("Buffer"));
      // close input data source
      source->Clear();
      // Re-open input data source in update mode
      output = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Update_LayerUpdate);
      }

    otb::ogr::Layer outLayer = output->GetLayer(0);

    OGRErr errStart = outLayer.ogr().StartTransaction();
    if (errStart != OGRERR_NONE)
      {
      itkExceptionMacro(<< "Unable to start transaction for OGR layer " << outLayer.ogr().GetName() << ".");
      }

    // Add the field of prediction in the output layer if field not exist
    OGRFeatureDefn &layerDefn = layer.GetLayerDefn();
    int idx = layerDefn.GetFieldIndex(GetParameterString("cfield").c_str());
    if (idx >= 0)
      {
      if (layerDefn.GetFieldDefn(idx)->GetType() != OFTInteger)
        itkExceptionMacro("Field name "<< GetParameterString("cfield") << " already exists with a different type!");
      }
    else
      {
      OGRFieldDefn predictedField(GetParameterString("cfield").c_str(), OFTInteger);
      ogr::FieldDefn predictedFieldDef(predictedField);
      outLayer.CreateField(predictedFieldDef);
      }

    // Add confidence field in the output layer
    std::string confFieldName("confidence");
    if (computeConfidenceMap)
      {
      idx = layerDefn.GetFieldIndex(confFieldName.c_str());
      if (idx >= 0)
        {
        if (layerDefn.GetFieldDefn(idx)->GetType() != OFTReal)
          itkExceptionMacro("Field name "<< confFieldName << " already exists with a different type!");
        }
      else
        {
        OGRFieldDefn confidenceField(confFieldName.c_str(), OFTReal);
        confidenceField.SetWidth(confidenceField.GetWidth());
        confidenceField.SetPrecision(confidenceField.GetPrecision());
        ogr::FieldDefn confFieldDefn(confidenceField);
        outLayer.CreateField(confFieldDefn);
        }
      }

    // Fill output layer
    unsigned int count=0;
    std::string classfieldname = GetParameterString("cfield");
    it = layer.cbegin();
    itEnd = layer.cend();
    for( ; it!=itEnd ; ++it, ++count)
      {
      ogr::Feature dstFeature(outLayer.GetLayerDefn());
      dstFeature.SetFrom( *it , TRUE);
      dstFeature.SetFID(it->GetFID());
      dstFeature[classfieldname].SetValue<int>(target->GetMeasurementVector(count)[0]);
      if (computeConfidenceMap)
        dstFeature[confFieldName].SetValue<double>(quality->GetMeasurementVector(count)[0]);
      if (updateMode)
        {
        outLayer.SetFeature(dstFeature);
        }
      else
        {
        outLayer.CreateFeature(dstFeature);
        }
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
