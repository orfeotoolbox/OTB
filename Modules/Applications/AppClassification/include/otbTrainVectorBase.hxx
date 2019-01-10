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
#ifndef otbTrainVectorBase_hxx
#define otbTrainVectorBase_hxx

#include "otbTrainVectorBase.h"

namespace otb
{
namespace Wrapper
{

void TrainVectorBase::DoInit()
{
  // Common Parameters for all Learning Application
  AddParameter( ParameterType_Group, "io", "Input and output data" );
  SetParameterDescription( "io", 
    "This group of parameters allows setting input and output data." );

  AddParameter( ParameterType_InputVectorDataList, "io.vd", "Input Vector Data" );
  SetParameterDescription( "io.vd",
    "Input geometries used for training (note: all geometries from the layer will be used)" );

  AddParameter( ParameterType_InputFilename, "io.stats", "Input XML image statistics file" );
  MandatoryOff( "io.stats" );
  SetParameterDescription( "io.stats", 
    "XML file containing mean and variance of each feature." );

  AddParameter( ParameterType_OutputFilename, "io.out", "Output model" );
  SetParameterDescription( "io.out", 
    "Output file containing the model estimated (.txt format)." );

  AddParameter( ParameterType_Int, "layer", "Layer Index" );
  SetParameterDescription( "layer", 
    "Index of the layer to use in the input vector file." );
  MandatoryOff( "layer" );
  SetDefaultParameterInt( "layer", 0 );

  AddParameter(ParameterType_ListView,  "feat", "Field names for training features");
  SetParameterDescription("feat",
    "List of field names in the input vector data to be used as features for training.");

  // Add validation data used to compute confusion matrix or contingency table
  AddParameter( ParameterType_Group, "valid", "Validation data" );
  SetParameterDescription( "valid", 
    "This group of parameters defines validation data." );

  AddParameter( ParameterType_InputVectorDataList, "valid.vd", 
    "Validation Vector Data" );
  SetParameterDescription( "valid.vd", "Geometries used for validation "
          "(must contain the same fields used for training, all geometries from the layer will be used)" );
  MandatoryOff( "valid.vd" );

  AddParameter( ParameterType_Int, "valid.layer", "Layer Index" );
  SetParameterDescription( "valid.layer", 
    "Index of the layer to use in the validation vector file." );
  MandatoryOff( "valid.layer" );
  SetDefaultParameterInt( "valid.layer", 0 );

  // Add class field if we used validation
  AddParameter( ParameterType_ListView, "cfield",
    "Field containing the class integer label for supervision" );
  SetParameterDescription( "cfield", 
    "Field containing the class id for supervision. "
    "The values in this field shall be cast into integers. "
    "Only geometries with this field available will be taken into account." );
  SetListViewSingleSelectionMode( "cfield", true );

  // Add a new parameter to compute confusion matrix / contingency table
  AddParameter( ParameterType_OutputFilename, "io.confmatout", 
    "Output confusion matrix or contingency table" );
  SetParameterDescription( "io.confmatout", 
    "Output file containing the confusion matrix or contingency table (.csv format)."
    "The contingency table is output when we unsupervised algorithms is used otherwise the confusion matrix is output." );
  MandatoryOff( "io.confmatout" );

  AddParameter(ParameterType_Bool, "v", "Verbose mode");
  SetParameterDescription("v", "Verbose mode, display the contingency table result.");
  SetParameterInt("v", 1);

  // Doc example parameter settings
  SetDocExampleParameterValue( "io.vd", "vectorData.shp" );
  SetDocExampleParameterValue( "io.stats", "meanVar.xml" );
  SetDocExampleParameterValue( "io.out", "svmModel.svm" );
  SetDocExampleParameterValue( "feat", "perimeter  area  width" );
  SetDocExampleParameterValue( "cfield", "predicted" );


  // Add parameters for the classifier choice
  Superclass::DoInit();

  AddRANDParameter();
}

void TrainVectorBase::DoUpdateParameters()
{
  // if vector data is present and updated then reload fields
  if( HasValue( "io.vd" ) )
    {
    std::vector<std::string> vectorFileList = GetParameterStringList( "io.vd" );
    ogr::DataSource::Pointer ogrDS = ogr::DataSource::New( vectorFileList[0], ogr::DataSource::Modes::Read );
    ogr::Layer layer = ogrDS->GetLayer( static_cast<size_t>( this->GetParameterInt( "layer" ) ) );
    ogr::Feature feature = layer.ogr().GetNextFeature();

    ClearChoices( "feat" );
    ClearChoices( "cfield" );

    for( int iField = 0; iField < feature.ogr().GetFieldCount(); iField++ )
      {
      std::string key, item = feature.ogr().GetFieldDefnRef( iField )->GetNameRef();
      key = item;
      std::string::iterator end = std::remove_if( key.begin(), key.end(), IsNotAlphaNum );
      std::transform( key.begin(), end, key.begin(), tolower );

      OGRFieldType fieldType = feature.ogr().GetFieldDefnRef( iField )->GetType();

      if( fieldType == OFTInteger || ogr::version_proxy::IsOFTInteger64( fieldType ) || fieldType == OFTReal )
        {
        std::string tmpKey = "feat." + key.substr( 0, static_cast<unsigned long>( end - key.begin() ) );
        AddChoice( tmpKey, item );
        }
      if( fieldType == OFTString || fieldType == OFTInteger || ogr::version_proxy::IsOFTInteger64( fieldType ) )
        {
        std::string tmpKey = "cfield." + key.substr( 0, static_cast<unsigned long>( end - key.begin() ) );
        AddChoice( tmpKey, item );
        }
      }
    }
}

void TrainVectorBase::DoExecute()
{
  m_FeaturesInfo.SetFieldNames( GetChoiceNames( "feat" ), GetSelectedItems( "feat" ));

  // Check input parameters
  if( m_FeaturesInfo.m_SelectedIdx.empty() )
    {
    otbAppLogFATAL( << "No features have been selected to train the classifier on!" );
    }

  ShiftScaleParameters measurement = GetStatistics( m_FeaturesInfo.m_NbFeatures );
  ExtractAllSamples( measurement );

  this->Train( m_TrainingSamplesWithLabel.listSample, m_TrainingSamplesWithLabel.labeledListSample, GetParameterString( "io.out" ) );

  m_PredictedList =
    this->Classify( m_ClassificationSamplesWithLabel.listSample, GetParameterString( "io.out" ) );
}


void TrainVectorBase::ExtractAllSamples(const ShiftScaleParameters &measurement)
{
  m_TrainingSamplesWithLabel = ExtractTrainingSamplesWithLabel(measurement);
  m_ClassificationSamplesWithLabel = ExtractClassificationSamplesWithLabel(measurement);
}

TrainVectorBase::SamplesWithLabel
TrainVectorBase::ExtractTrainingSamplesWithLabel(const ShiftScaleParameters &measurement)
{
  return ExtractSamplesWithLabel( "io.vd", "layer", measurement);
}

TrainVectorBase::SamplesWithLabel
TrainVectorBase::ExtractClassificationSamplesWithLabel(const ShiftScaleParameters &measurement)
{
  if(GetClassifierCategory() == Supervised)
    {
    SamplesWithLabel tmpSamplesWithLabel;
    SamplesWithLabel validationSamplesWithLabel = ExtractSamplesWithLabel( "valid.vd", "valid.layer", measurement );
    //Test the input validation set size
    if( validationSamplesWithLabel.labeledListSample->Size() != 0 )
      {
      tmpSamplesWithLabel.listSample = validationSamplesWithLabel.listSample;
      tmpSamplesWithLabel.labeledListSample = validationSamplesWithLabel.labeledListSample;
      }
    else
      {
      otbAppLogWARNING(
              "The validation set is empty. The performance estimation is done using the input training set in this case." );
      tmpSamplesWithLabel.listSample = m_TrainingSamplesWithLabel.listSample;
      tmpSamplesWithLabel.labeledListSample = m_TrainingSamplesWithLabel.labeledListSample;
      }

    return tmpSamplesWithLabel;
    }
  else
    {
    return m_TrainingSamplesWithLabel;
    }
}


TrainVectorBase::ShiftScaleParameters
TrainVectorBase::GetStatistics(unsigned int nbFeatures)
{
  ShiftScaleParameters measurement = ShiftScaleParameters();
  if( HasValue( "io.stats" ) && IsParameterEnabled( "io.stats" ) )
    {
    StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
    std::string XMLfile = GetParameterString( "io.stats" );
    statisticsReader->SetFileName( XMLfile );
    measurement.meanMeasurementVector = statisticsReader->GetStatisticVectorByName( "mean" );
    measurement.stddevMeasurementVector = statisticsReader->GetStatisticVectorByName( "stddev" );
    }
  else
    {
    measurement.meanMeasurementVector.SetSize( nbFeatures );
    measurement.meanMeasurementVector.Fill( 0. );
    measurement.stddevMeasurementVector.SetSize( nbFeatures );
    measurement.stddevMeasurementVector.Fill( 1. );
    }
  return measurement;
}


TrainVectorBase::SamplesWithLabel
TrainVectorBase::ExtractSamplesWithLabel(std::string parameterName, std::string parameterLayer,
                                    const ShiftScaleParameters &measurement)
{
  SamplesWithLabel samplesWithLabel;
  if( HasValue( parameterName ) && IsParameterEnabled( parameterName ) )
    {
    ListSampleType::Pointer input = ListSampleType::New();
    TargetListSampleType::Pointer target = TargetListSampleType::New();
    input->SetMeasurementVectorSize( m_FeaturesInfo.m_NbFeatures );

    std::vector<std::string> fileList = this->GetParameterStringList( parameterName );
    for( unsigned int k = 0; k < fileList.size(); k++ )
      {
      otbAppLogINFO( "Reading vector file " << k + 1 << "/" << fileList.size() );
      ogr::DataSource::Pointer source = ogr::DataSource::New( fileList[k], ogr::DataSource::Modes::Read );
      ogr::Layer layer = source->GetLayer( static_cast<size_t>(this->GetParameterInt( parameterLayer )) );
      ogr::Feature feature = layer.ogr().GetNextFeature();
      bool goesOn = feature.addr() != 0;
      if( !goesOn )
        {
        otbAppLogWARNING( "The layer " << GetParameterInt( parameterLayer ) << " of " << fileList[k]
                                       << " is empty, input is skipped." );
        continue;
        }

      // Check all needed fields are present :
      //   - check class field if we use supervised classification or if class field name is not empty
      int cFieldIndex = feature.ogr().GetFieldIndex( m_FeaturesInfo.m_SelectedCFieldName.c_str() );
      if( cFieldIndex < 0 && !m_FeaturesInfo.m_SelectedCFieldName.empty())
        {
        otbAppLogFATAL( "The field name for class label (" << m_FeaturesInfo.m_SelectedCFieldName
                                                           << ") has not been found in the vector file "
                                                           << fileList[k] );
        }

      //   - check feature fields
      std::vector<int> featureFieldIndex( m_FeaturesInfo.m_NbFeatures, -1 );
      for( unsigned int i = 0; i < m_FeaturesInfo.m_NbFeatures; i++ )
        {
        featureFieldIndex[i] = feature.ogr().GetFieldIndex( m_FeaturesInfo.m_SelectedNames[i].c_str() );
        if( featureFieldIndex[i] < 0 )
          otbAppLogFATAL( "The field name for feature " << m_FeaturesInfo.m_SelectedNames[i]
                                                        << " has not been found in the vector file "
                                                        << fileList[k] );
        }


      while( goesOn )
        {
        // Retrieve all the features for each field in the ogr layer.
        MeasurementType mv;
        mv.SetSize( m_FeaturesInfo.m_NbFeatures );
        for( unsigned int idx = 0; idx < m_FeaturesInfo.m_NbFeatures; ++idx )
          mv[idx] = feature.ogr().GetFieldAsDouble( featureFieldIndex[idx] );

        input->PushBack( mv );

        if(cFieldIndex>=0 && ogr::Field(feature,cFieldIndex).HasBeenSet())
          target->PushBack( feature.ogr().GetFieldAsInteger( cFieldIndex ) );
        else
          target->PushBack( 0 );

        feature = layer.ogr().GetNextFeature();
        goesOn = feature.addr() != 0;
        }
      }



    ShiftScaleFilterType::Pointer shiftScaleFilter = ShiftScaleFilterType::New();
    shiftScaleFilter->SetInput( input );
    shiftScaleFilter->SetShifts( measurement.meanMeasurementVector );
    shiftScaleFilter->SetScales( measurement.stddevMeasurementVector );
    shiftScaleFilter->Update();

    samplesWithLabel.listSample = shiftScaleFilter->GetOutput();
    samplesWithLabel.labeledListSample = target;
    samplesWithLabel.listSample->DisconnectPipeline();
    }

  return samplesWithLabel;
}


}
}

#endif
