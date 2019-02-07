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

#include "otbShiftScaleSampleListFilter.h"
#include "otbStatisticsXMLFileReader.h"

#include <fstream> // write the model file

#include "otbDimensionalityReductionModelFactory.h"
#include "otbTrainDimensionalityReductionApplicationBase.h"

namespace otb
{
namespace Wrapper
{

/**
 * \class TrainDimensionalityReduction
 *
 * Training of a dimensionality reduction model
 */
class TrainDimensionalityReduction : public TrainDimensionalityReductionApplicationBase<float,float>
{
public:
  typedef TrainDimensionalityReduction Self;
  typedef TrainDimensionalityReductionApplicationBase<float, float> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  
  itkNewMacro(Self);
  itkTypeMacro(TrainDimensionalityReduction, otb::Application);

  typedef Superclass::SampleType              SampleType;
  typedef Superclass::ListSampleType          ListSampleType;
  typedef Superclass::SampleImageType         SampleImageType;

  typedef float ValueType;
  typedef itk::VariableLengthVector<ValueType> MeasurementType;

  typedef otb::StatisticsXMLFileReader<SampleType> StatisticsReader;

  typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;

  typedef otb::DimensionalityReductionModelFactory<ValueType, ValueType>  ModelFactoryType;

private:
  void DoInit() override
  {
    SetName("TrainDimensionalityReduction");
    SetDescription("Train a dimensionality reduction model");

    SetDocName("Train Dimensionality Reduction");
    SetDocLongDescription("Trainer for dimensionality reduction algorithms "
      "(autoencoders, PCA, SOM). All input samples are used to compute the "
      "model, like other machine learning models.\n"
      "The model can be used in the ImageDimensionalityReduction and "
      "VectorDimensionalityReduction applications.");

    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("ImageDimensionalityReduction, VectorDimensionalityReduction");

    AddParameter(ParameterType_Group, "io", "Input and output data");
    SetParameterDescription("io", "This group of parameters allows setting input and output data.");

    AddParameter(ParameterType_InputVectorData, "io.vd", "Input Vector Data");
    SetParameterDescription("io.vd", "Input geometries used for training (note: all geometries from the layer will be used)");

    AddParameter(ParameterType_OutputFilename, "io.out", "Output model");
    SetParameterDescription("io.out", "Output file containing the estimated model (.txt format).");

    AddParameter(ParameterType_InputFilename, "io.stats", "Input XML image statistics file");
    MandatoryOff("io.stats");
    SetParameterDescription("io.stats", "XML file containing mean and variance of each feature.");

    AddParameter(ParameterType_StringList, "feat", "Field names to be used for training");
    SetParameterDescription("feat","List of field names in the input vector data used as features for training.");

    Superclass::DoInit();

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("io.vd", "cuprite_samples.sqlite");
    SetDocExampleParameterValue("io.out", "model.som");
    SetDocExampleParameterValue("algorithm", "som");
    SetDocExampleParameterValue("algorithm.som.s", "10 10");
    SetDocExampleParameterValue("algorithm.som.n", "3 3");
    SetDocExampleParameterValue("algorithm.som.ni", "5");
    SetDocExampleParameterValue("algorithm.som.bi", "1");
    SetDocExampleParameterValue("algorithm.som.bf", "0.1");
    SetDocExampleParameterValue("algorithm.som.iv", "10");
    SetDocExampleParameterValue("feat","value_0 value_1 value_2 value_3 value_4"
      " value_5 value_6 value_7 value_8 value_9");
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    std::string shapefile = GetParameterString("io.vd");

    otb::ogr::DataSource::Pointer source =
      otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
    otb::ogr::Layer layer = source->GetLayer(0);
    ListSampleType::Pointer input = ListSampleType::New();
    const int nbFeatures = GetParameterStringList("feat").size();

    input->SetMeasurementVectorSize(nbFeatures);
    otb::ogr::Layer::const_iterator it = layer.cbegin();
    otb::ogr::Layer::const_iterator itEnd = layer.cend();
    for( ; it!=itEnd ; ++it)
      {
      MeasurementType mv;
      mv.SetSize(nbFeatures);
      for(int idx=0; idx < nbFeatures; ++idx)
        {
        mv[idx] = (*it)[GetParameterStringList("feat")[idx]].GetValue<double>();
        }
      input->PushBack(mv);
      }

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

    ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
    trainingShiftScaleFilter->SetInput(input);
    trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
    trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
    trainingShiftScaleFilter->Update();

    ListSampleType::Pointer trainingListSample= trainingShiftScaleFilter->GetOutput();

    this->Train(trainingListSample,GetParameterString("io.out"));
  }

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainDimensionalityReduction)
