/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#include "otbDimensionalityReductionModelFactory.h"
#include <time.h>

namespace otb
{
namespace Wrapper
{
/**
 * \class VectorDimensionalityReduction
 *
 * Apply a dimensionality reduction model on a vector file
 */
class VectorDimensionalityReduction : public Application
{
public:
    /** Standard class typedefs. */
  typedef VectorDimensionalityReduction Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(Self, Application)

  /** Filters typedef */
  typedef float                                           ValueType;
  typedef itk::VariableLengthVector<ValueType>            InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType>    ListSampleType;
  typedef MachineLearningModel<
    itk::VariableLengthVector<ValueType>,
    itk::VariableLengthVector<ValueType> >                DimensionalityReductionModelType;
  typedef DimensionalityReductionModelFactory<
    ValueType,ValueType>                                  DimensionalityReductionModelFactoryType;
  typedef DimensionalityReductionModelType::Pointer       ModelPointerType;

  /** Statistics Filters typedef */
  typedef itk::VariableLengthVector<ValueType>            MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType>   StatisticsReader;
  typedef otb::Statistics::ShiftScaleSampleListFilter<
    ListSampleType, ListSampleType>                       ShiftScaleFilterType;

protected:
  ~VectorDimensionalityReduction() override
    {
    DimensionalityReductionModelFactoryType::CleanFactories();
    }

private:
  void DoInit() override
    {
    SetName("VectorDimensionalityReduction");
    SetDescription("Performs dimensionality reduction of the input vector data "
      "according to a model file.");
    SetDocAuthors("OTB-Team");
    SetDocLongDescription("This application performs a vector data "
      "dimensionality reduction based on a model file produced by the "
      "TrainDimensionalityReduction application.");
    SetDocSeeAlso("TrainDimensionalityReduction");
    SetDocLimitations("None");
    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputVectorData, "in", "Name of the input vector data");
    SetParameterDescription("in","The input vector data to reduce.");

    AddParameter(ParameterType_InputFilename, "instat", "Statistics file");
    SetParameterDescription("instat", "An XML file containing mean and standard "
      "deviation to center and reduce samples before dimensionality reduction "
      "(produced by ComputeImagesStatistics application).");
    MandatoryOff("instat");

    AddParameter(ParameterType_InputFilename, "model", "Model file");
    SetParameterDescription("model", "A model file (produced by the "
      "TrainDimensionalityReduction application,");

    AddParameter(ParameterType_OutputFilename, "out", "Output vector data file "
      "containing the reduced vector");
    SetParameterDescription("out","Output vector data file storing sample "
      "values (OGR format). If not given, the input vector data file is used. "
      "In overwrite mode, the original features will be lost.");
    MandatoryOff("out");

    AddParameter(ParameterType_ListView, "feat", "Input features to use for reduction");
    SetParameterDescription("feat", "List of field names in the input vector data used as features for reduction.");

    AddParameter(ParameterType_Choice, "featout", "Output feature");
    SetParameterDescription("featout", "Naming of output features");

    AddChoice("featout.prefix", "Prefix");
    SetParameterDescription("featout.prefix", "Use a name prefix");

    AddParameter(ParameterType_String, "featout.prefix.name", "Feature name prefix");
    SetParameterDescription("featout.prefix.name","Name prefix for output "
      "features. This prefix is followed by the numeric index of each output feature.");
    SetParameterString("featout.prefix.name","reduced_", false);

    AddChoice("featout.list","List");
    SetParameterDescription("featout.list", "Use a list with all names");

    AddParameter(ParameterType_StringList, "featout.list.names", "Feature name list");
    SetParameterDescription("featout.list.names",
                            "List of field names for the output "
                            "features which result from the reduction.");

    AddParameter(ParameterType_Int, "pcadim", "Principal component dimension");
    SetParameterDescription("pcadim",
                            "This optional parameter can be set to "
                            "reduce the number of eignevectors used in the PCA model file. This "
                            "parameter can't be used for other models");
    MandatoryOff("pcadim");

    AddParameter(ParameterType_Choice, "mode", "Writing mode");
    SetParameterDescription("mode", "This parameter determines if the output "
      "file is overwritten or updated [overwrite/update]. If an output file "
      "name is given, the original file is copied before creating the new features.");

    AddChoice("mode.overwrite", "Overwrite");
    SetParameterDescription("mode.overwrite", "Overwrite mode");

    AddChoice("mode.update", "Update");
    SetParameterDescription("mode.update", "Update mode");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "vectorData.shp");
    SetDocExampleParameterValue("instat", "meanVar.xml");
    SetDocExampleParameterValue("model", "model.txt");
    SetDocExampleParameterValue("out", "vectorDataOut.shp");
    SetDocExampleParameterValue("feat", "perimeter area width");
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
        std::string::iterator end = std::remove_if( key.begin(), key.end(), [](char c){return !std::isalnum(c);});
        std::transform( key.begin(), end, key.begin(), tolower );
        std::string tmpKey = "feat." + key.substr( 0, static_cast<unsigned long>( end - key.begin() ) );
        AddChoice(tmpKey,item);
        }
      }
    }

  void DoExecute() override
    {
    clock_t tic = clock();

    std::string shapefile = GetParameterString("in");
    otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(
      shapefile, otb::ogr::DataSource::Modes::Read);
    otb::ogr::Layer layer = source->GetLayer(0);
    ListSampleType::Pointer input = ListSampleType::New();
    std::vector<int> inputIndexes = GetSelectedItems("feat");
    int nbFeatures = inputIndexes.size();

    input->SetMeasurementVectorSize(nbFeatures);
    otb::ogr::Layer::const_iterator it = layer.cbegin();
    otb::ogr::Layer::const_iterator itEnd = layer.cend();

    // Get the list of non-selected field indexes
    // /!\ The 'feat' is assumed to expose all available fields, hence the
    // mapping between GetSelectedItems() and OGR field indexes
    OGRFeatureDefn &inLayerDefn = layer.GetLayerDefn();
    std::set<int> otherInputFields;
    for (int i=0 ; i < inLayerDefn.GetFieldCount() ; i++)
      otherInputFields.insert(i);
    for (int k=0 ; k < nbFeatures ; k++)
      otherInputFields.erase(inputIndexes[k]);

    for( ; it!=itEnd ; ++it)
      {
      MeasurementType mv;
      mv.SetSize(nbFeatures);
      
      for(int idx=0; idx < nbFeatures; ++idx)
        {
        switch ((*it)[inputIndexes[idx]].GetType())
        {
          case OFTInteger:
            mv[idx] = static_cast<ValueType>((*it)[inputIndexes[idx]].GetValue<int>());
            break;
          case OFTInteger64:
            mv[idx] = static_cast<ValueType>((*it)[inputIndexes[idx]].GetValue<int>());
            break;
          case OFTReal:
            mv[idx] = static_cast<ValueType>((*it)[inputIndexes[idx]].GetValue<double>());
            break;
          default:
            itkExceptionMacro(<< "incorrect field type: " << (*it)[inputIndexes[idx]].GetType() << ".");
        }
        }
      input->PushBack(mv);
      }

    /** Statistics for shift/scale */    
    MeasurementType meanMeasurementVector;
    MeasurementType stddevMeasurementVector;

    if (HasValue("instat") && IsParameterEnabled("instat"))
      {
      StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
      std::string XMLfile = GetParameterString("instat");
      statisticsReader->SetFileName(XMLfile);
      meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
      stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
      otbAppLogINFO("Mean used: " << meanMeasurementVector);
      otbAppLogINFO("Standard deviation used: " << stddevMeasurementVector);
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

    otbAppLogINFO("Loading model");
    /** Read the model */
    m_Model = DimensionalityReductionModelFactoryType::CreateDimensionalityReductionModel(
      GetParameterString("model"),
      DimensionalityReductionModelFactoryType::ReadMode);
    if (m_Model.IsNull())
      {
      otbAppLogFATAL(<< "Error when loading model " << GetParameterString("model")
        << " : unsupported model type");
      }
    m_Model->Load(GetParameterString("model"));
    if (HasValue("pcadim") && IsParameterEnabled("pcadim"))
      {
      std::string modelName(m_Model->GetNameOfClass());
      if (modelName != "PCAModel")
        {
        otbAppLogFATAL(<< "Can't set 'pcadim' on a model : "<< modelName);
        }
      m_Model->SetDimension( GetParameterInt("pcadim") );
      }
    otbAppLogINFO("Model loaded, dimension : "<< m_Model->GetDimension());

    /** Perform Dimensionality Reduction */    
    ListSampleType::Pointer listSample = trainingShiftScaleFilter->GetOutput();
    ListSampleType::Pointer target = m_Model->PredictBatch(listSample);

    /** Create/Update Output Shape file */          
    ogr::DataSource::Pointer output;
    ogr::DataSource::Pointer buffer = ogr::DataSource::New();
    bool updateMode = false;

    if (IsParameterEnabled("out") && HasValue("out"))
      {
      // Create new OGRDataSource
      if (GetParameterString("mode")=="overwrite")
        {
        output = ogr::DataSource::New(GetParameterString("out"), ogr::DataSource::Modes::Overwrite);
        otb::ogr::Layer newLayer = output->CreateLayer(
          GetParameterString("out"),
          const_cast<OGRSpatialReference*>(layer.GetSpatialRef()),
          layer.GetGeomType());
        // Copy existing fields except the ones selected for reduction
        for (const int& k : otherInputFields)
          {
          OGRFieldDefn fieldDefn(inLayerDefn.GetFieldDefn(k));
          newLayer.CreateField(fieldDefn);
          }
        }
      else if (GetParameterString("mode")=="update")
        {
        //output = ogr::DataSource::New(GetParameterString("out"), ogr::DataSource::Modes::Update_LayerCreateOnly);
        // Update mode
        otb::ogr::DataSource::Pointer source_output =
          otb::ogr::DataSource::New(GetParameterString("out"), otb::ogr::DataSource::Modes::Read);
        layer = source_output->GetLayer(0);
        updateMode = true;
        otbAppLogINFO("Update input vector data.");

        // fill temporary buffer for the transfer
        otb::ogr::Layer inputLayer = layer;
        layer = buffer->CopyLayer(inputLayer, std::string("Buffer"));
        // close input data source 
        source_output->Clear();
        // Re-open input data source in update mode
        output = otb::ogr::DataSource::New(
          GetParameterString("out"),
          otb::ogr::DataSource::Modes::Update_LayerUpdate);
        }
      else
        {
        otbAppLogFATAL(<< "Error when creating the output file" <<
          GetParameterString("mode") << " : unsupported writing mode type");
        }
      }

    otb::ogr::Layer outLayer = output->GetLayer(0);    
    OGRErr errStart = outLayer.ogr().StartTransaction();

    if (errStart != OGRERR_NONE)
      {
      otbAppLogFATAL(<< "Unable to start transaction for OGR layer " << outLayer.ogr().GetName() << ".");
      }

    // Build the list of output fields
    std::vector<std::string> outFields;
    if(GetParameterString("featout") == "prefix")
      {
      std::string prefix = GetParameterString("featout.prefix.name");
      std::ostringstream oss;
      for (unsigned int i=0 ; i < m_Model->GetDimension() ; i++)
        {
        oss.str(prefix);
        oss.seekp(0,std::ios_base::end);
        oss << i;
        outFields.push_back(oss.str());
        }
      }
    else if(GetParameterString("featout") == "list")
      {
      outFields = GetParameterStringList("featout.list.names");
      if (outFields.size() != m_Model->GetDimension())
        {
        otbAppLogFATAL( << "Wrong number of output field names, expected "
          << m_Model->GetDimension() << " , got "<< outFields.size());
        }
      }
    else
      {
      otbAppLogFATAL( << "Unsupported output feature mode : "
        << GetParameterString("featout"));
      }

    // Add the field of prediction in the output layer if field not exist
    for (unsigned int i=0; i<outFields.size() ;i++)
      {
      OGRFeatureDefn &layerDefn = outLayer.GetLayerDefn();
      int idx = layerDefn.GetFieldIndex(outFields[i].c_str());
      
      if (idx >= 0)
        {
        if (layerDefn.GetFieldDefn(idx)->GetType() != OFTReal)
        otbAppLogFATAL("Field name "<< outFields[i]
          << " already exists with a different type!");
        }
      else
        {
        OGRFieldDefn predictedField(outFields[i].c_str(), OFTReal);
        ogr::FieldDefn predictedFieldDef(predictedField);
        outLayer.CreateField(predictedFieldDef);
        }
      }

    // Fill output layer
    unsigned int count=0;
    it = layer.cbegin();
    itEnd = layer.cend();
    for( ; it!=itEnd ; ++it, ++count)
      {
      ogr::Feature dstFeature(outLayer.GetLayerDefn());

      dstFeature.SetFrom( *it , TRUE);
      dstFeature.SetFID(it->GetFID());

      for (std::size_t i=0; i<outFields.size(); ++i)
        {
        switch (dstFeature[outFields[i]].GetType())
        {
        case OFTInteger:
          dstFeature[outFields[i]].SetValue<int>(target->GetMeasurementVector(count)[0]);
          break;
        case OFTInteger64:
          dstFeature[outFields[i]].SetValue<int>(target->GetMeasurementVector(count)[0]);
          break;
        case OFTReal:
          dstFeature[outFields[i]].SetValue<double>(target->GetMeasurementVector(count)[0]);
          break;
        case OFTString:
          dstFeature[outFields[i]].SetValue<std::string>(std::to_string(target->GetMeasurementVector(count)[0]));
          break;
        default:
          itkExceptionMacro(<< "incorrect field type: " << dstFeature[outFields[i]].GetType() << ".");
        }
        }
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
        otbAppLogFATAL(<< "Unable to commit transaction for OGR layer " <<
          outLayer.ogr().GetName() << ".");
        }
      }
    output->SyncToDisk();
    clock_t toc = clock();
    otbAppLogINFO( "Elapsed: "<< ((double)(toc - tic) / CLOCKS_PER_SEC)<<" seconds.");
    }

  ModelPointerType m_Model;
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDimensionalityReduction)
