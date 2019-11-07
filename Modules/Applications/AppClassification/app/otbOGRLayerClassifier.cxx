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
#include "otbStatisticsXMLFileWriter.h"

#include "itkVariableLengthVector.h"
#include "otbStatisticsXMLFileReader.h"

#include "itkListSample.h"
#include "otbShiftScaleSampleListFilter.h"

#ifdef OTB_USE_LIBSVM
#include "otbLibSVMMachineLearningModel.h"
#endif

#include <time.h>

namespace otb
{
namespace Wrapper
{
class OGRLayerClassifier : public Application
{
public:
  typedef OGRLayerClassifier            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self);

  itkTypeMacro(OGRLayerClassifier, otb::Application);

private:
  void DoInit() override
  {
    SetName("OGRLayerClassifier");
    SetDescription("Classify an OGR layer based on a machine learning model and a list of features to consider.");

    SetDocLongDescription(
        "This application will apply a trained machine learning model on the selected feature to get a classification of each geometry contained in an OGR "
        "layer. The list of feature must match the list used for training. The predicted label is written in the user defined field for each geometry.");
    SetDocLimitations("Experimental. Only shapefiles are supported for now.");
    SetDocAuthors("David Youssefi during internship at CNES");
    SetDocSeeAlso("ComputeOGRLayersFeaturesStatistics");
    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputVectorData, "inshp", "Name of the input shapefile");
    SetParameterDescription("inshp", "Name of the input shapefile");

    AddParameter(ParameterType_InputFilename, "instats", "XML file containing mean and variance of each feature");
    SetParameterDescription("instats", "XML file containing mean and variance of each feature.");

    AddParameter(ParameterType_OutputFilename, "insvm", "Input model filename");
    SetParameterDescription("insvm", "Input model filename.");


    AddParameter(ParameterType_ListView, "feat", "Features");
    SetParameterDescription("feat", "Features to be calculated");

    AddParameter(ParameterType_String, "cfield", "Field containing the predicted class");
    SetParameterDescription("cfield", "Field containing the predicted class");
    SetParameterString("cfield", "predicted");

    // Doc example parameter settings
    SetDocExampleParameterValue("inshp", "vectorData.shp");
    SetDocExampleParameterValue("instats", "meanVar.xml");
    SetDocExampleParameterValue("insvm", "svmModel.svm");
    SetDocExampleParameterValue("feat", "perimeter");
    SetDocExampleParameterValue("cfield", "predicted");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    if (HasValue("inshp"))
    {
      std::string shapefile = GetParameterString("inshp");

      otb::ogr::DataSource::Pointer ogrDS;
      otb::ogr::Layer               layer(nullptr, false);

      OGRSpatialReference      oSRS("");
      std::vector<std::string> options;

      ogrDS                 = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
      std::string layername = itksys::SystemTools::GetFilenameName(shapefile);
      layername             = layername.substr(0, layername.size() - 4);
      layer                 = ogrDS->GetLayer(0);

      otb::ogr::Feature feature = layer.ogr().GetNextFeature();
      ClearChoices("feat");
      std::vector<std::string> choiceKeys;

      for (int iField = 0; iField < feature.ogr().GetFieldCount(); iField++)
      {
        std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
        key = item;

        // Transform chain : lowercase and alphanumerical
        key.erase(std::remove_if(key.begin(), key.end(), std::not1(std::ptr_fun(::isalnum))), key.end());
        std::transform(key.begin(), key.end(), key.begin(), tolower);

        // Key must be unique
        choiceKeys = GetChoiceKeys("feat");
        while (choiceKeys.end() != std::find(choiceKeys.begin(), choiceKeys.end(), key))
          key.append("0");

        key = "feat." + key;
        AddChoice(key, item);
      }
    }
  }

  void DoExecute() override
  {

#ifdef OTB_USE_LIBSVM
    clock_t tic = clock();

    std::string shapefile = GetParameterString("inshp");
    std::string XMLfile   = GetParameterString("instats");
    std::string modelfile = GetParameterString("insvm");

    typedef double                                        ValueType;
    typedef itk::VariableLengthVector<ValueType>          MeasurementType;
    typedef itk::Statistics::ListSample<MeasurementType>  ListSampleType;
    typedef otb::StatisticsXMLFileReader<MeasurementType> StatisticsReader;

    typedef unsigned int LabelPixelType;
    typedef itk::FixedArray<LabelPixelType, 1> LabelSampleType;
    typedef itk::Statistics::ListSample<LabelSampleType> LabelListSampleType;

    typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

    StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
    statisticsReader->SetFileName(XMLfile);

    MeasurementType meanMeasurementVector   = statisticsReader->GetStatisticVectorByName("mean");
    MeasurementType stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");

    otb::ogr::DataSource::Pointer source  = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
    otb::ogr::Layer               layer   = source->GetLayer(0);
    bool                          goesOn  = true;
    otb::ogr::Feature             feature = layer.ogr().GetNextFeature();

    ListSampleType::Pointer      input      = ListSampleType::New();
    LabelListSampleType::Pointer target     = LabelListSampleType::New();
    const int                    nbFeatures = GetSelectedItems("feat").size();
    input->SetMeasurementVectorSize(nbFeatures);

    if (feature.addr())
      while (goesOn)
      {
        MeasurementType mv;
        mv.SetSize(nbFeatures);

        for (int idx = 0; idx < nbFeatures; ++idx)
          mv[idx]    = feature.ogr().GetFieldAsDouble(GetSelectedItems("feat")[idx]);

        input->PushBack(mv);
        target->PushBack(feature.ogr().GetFieldAsInteger("class"));
        feature = layer.ogr().GetNextFeature();
        goesOn  = feature.addr() != nullptr;
      }

    ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
    trainingShiftScaleFilter->SetInput(input);
    trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
    trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
    trainingShiftScaleFilter->Update();

    ListSampleType::Pointer      listSample;
    LabelListSampleType::Pointer labelListSample;

    listSample      = trainingShiftScaleFilter->GetOutput();
    labelListSample = target;

    ListSampleType::Pointer      trainingListSample        = listSample;
    LabelListSampleType::Pointer trainingLabeledListSample = labelListSample;

    typedef otb::LibSVMMachineLearningModel<ValueType, LabelPixelType> LibSVMType;
    LibSVMType::Pointer libSVMClassifier = LibSVMType::New();
    libSVMClassifier->Load(modelfile);
    trainingLabeledListSample = libSVMClassifier->PredictBatch(trainingListSample);

    otb::ogr::DataSource::Pointer source2 = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Update_LayerUpdate);
    otb::ogr::Layer               layer2  = source2->GetLayer(0);

    OGRFieldDefn predictedField(GetParameterString("cfield").c_str(), OFTInteger);
    layer2.CreateField(predictedField, true);

    bool goesOn2 = true;
    layer2.ogr().ResetReading();
    otb::ogr::Feature feature2 = layer2.ogr().GetNextFeature();
    unsigned int      count    = 0;

    if (feature2.addr())
      while (goesOn2)
      {
        feature2.ogr().SetField(GetParameterString("cfield").c_str(), (int)labelListSample->GetMeasurementVector(count)[0]);
        layer2.SetFeature(feature2);
        feature2 = layer2.ogr().GetNextFeature();
        goesOn2  = feature2.addr() != nullptr;
        count++;
      }

    const OGRErr err = layer2.ogr().CommitTransaction();

    if (err != OGRERR_NONE)
    {
      itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << layer2.ogr().GetName() << ".");
    }

    source2->SyncToDisk();

    clock_t toc = clock();

    otbAppLogINFO("Elapsed: " << ((double)(toc - tic) / CLOCKS_PER_SEC) << " seconds.");

#else
    otbAppLogFATAL("Module LIBSVM is not installed. You should consider turning OTB_USE_LIBSVM on during cmake configuration.");
#endif
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::OGRLayerClassifier)
