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
#ifndef otbTrainVectorBase_hxx
#define otbTrainVectorBase_hxx

#include "otbTrainVectorBase.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
TrainVectorBase<TInputValue, TOutputValue>
::DoInit()
{
  // Common Parameters for all Learning Application
  this->AddParameter( ParameterType_Group, "io", "Input and output data" );
  this->SetParameterDescription( "io", 
    "This group of parameters allows setting input and output data." );

  this->AddParameter( ParameterType_InputVectorDataList, "io.vd", "Input Vector Data" );
  this->SetParameterDescription( "io.vd",
    "Input geometries used for training (note: all geometries from the layer will be used)" );

  this->AddParameter( ParameterType_InputFilename, "io.stats", "Input XML image statistics file" );
  this->MandatoryOff( "io.stats" );
  this->SetParameterDescription( "io.stats", 
    "XML file containing mean and variance of each feature." );

  this->AddParameter( ParameterType_OutputFilename, "io.out", "Output model" );
  this->SetParameterDescription( "io.out", 
    "Output file containing the model estimated (.txt format)." );

  this->AddParameter( ParameterType_Int, "layer", "Layer Index" );
  this->SetParameterDescription( "layer", 
    "Index of the layer to use in the input vector file." );
  this->MandatoryOff( "layer" );
  this->SetDefaultParameterInt( "layer", 0 );

  this->AddParameter(ParameterType_ListView,  "feat", "Field names for training features");
  this->SetParameterDescription("feat",
    "List of field names in the input vector data to be used as features for training.");

  // Add validation data used to compute confusion matrix or contingency table
  this->AddParameter( ParameterType_Group, "valid", "Validation data" );
  this->SetParameterDescription( "valid", 
    "This group of parameters defines validation data." );

  this->AddParameter( ParameterType_InputVectorDataList, "valid.vd", 
    "Validation Vector Data" );
  this->SetParameterDescription( "valid.vd", "Geometries used for validation "
          "(must contain the same fields used for training, all geometries from the layer will be used)" );
  this->MandatoryOff( "valid.vd" );

  this->AddParameter( ParameterType_Int, "valid.layer", "Layer Index" );
  this->SetParameterDescription( "valid.layer", 
    "Index of the layer to use in the validation vector file." );
  this->MandatoryOff( "valid.layer" );
  this->SetDefaultParameterInt( "valid.layer", 0 );

  // Add class field if we used validation
  this->AddParameter( ParameterType_ListView, "cfield",
    "Field containing the class integer label for supervision" );
  this->SetParameterDescription( "cfield", 
    "Field containing the class id for supervision. "
    "The values in this field shall be cast into integers. "
    "Only geometries with this field available will be taken into account." );
  this->SetListViewSingleSelectionMode( "cfield", true );

  this->AddParameter(ParameterType_Bool, "v", "Verbose mode");
  this->SetParameterDescription("v", "Verbose mode, display the contingency table result.");
  this->SetParameterInt("v", 1);

  // Doc example parameter settings
  this->SetDocExampleParameterValue( "io.vd", "vectorData.shp" );
  this->SetDocExampleParameterValue( "io.stats", "meanVar.xml" );
  this->SetDocExampleParameterValue( "io.out", "svmModel.svm" );
  this->SetDocExampleParameterValue( "feat", "perimeter  area  width" );
  this->SetDocExampleParameterValue( "cfield", "predicted" );


  // Add parameters for the classifier choice
  Superclass::DoInit();

  this->AddRANDParameter();
}

template <class TInputValue, class TOutputValue>
void 
TrainVectorBase<TInputValue, TOutputValue>
::DoUpdateParameters()
{
  // if vector data is present and updated then reload fields
  if( this->HasValue( "io.vd" ) )
    {
    std::vector<std::string> vectorFileList = this->GetParameterStringList( "io.vd" );
    ogr::DataSource::Pointer ogrDS = ogr::DataSource::New( vectorFileList[0], ogr::DataSource::Modes::Read );
    ogr::Layer layer = ogrDS->GetLayer( static_cast<size_t>( this->GetParameterInt( "layer" ) ) );
    ogr::Feature feature = layer.ogr().GetNextFeature();

    this->ClearChoices( "feat" );
    this->ClearChoices( "cfield" );

    for( int iField = 0; iField < feature.ogr().GetFieldCount(); iField++ )
      {
      std::string key, item = feature.ogr().GetFieldDefnRef( iField )->GetNameRef();
      key = item;
      std::string::iterator end = std::remove_if( key.begin(), key.end(), IsNotAlphaNum );
      std::transform( key.begin(), end, key.begin(), tolower );

      OGRFieldType fieldType = feature.ogr().GetFieldDefnRef( iField )->GetType();

      if( fieldType == OFTInteger || fieldType == OFTInteger64 || fieldType == OFTReal )
        {
        std::string tmpKey = "feat." + key.substr( 0, static_cast<unsigned long>( end - key.begin() ) );
        this->AddChoice( tmpKey, item );
        }
      if( fieldType == OFTString || fieldType == OFTInteger || fieldType == OFTInteger64 || fieldType == OFTReal )
        {
        std::string tmpKey = "cfield." + key.substr( 0, static_cast<unsigned long>( end - key.begin() ) );
        this->AddChoice( tmpKey, item );
        }
      }
    }
}

template <class TInputValue, class TOutputValue>
void
TrainVectorBase<TInputValue, TOutputValue>
::DoExecute()
{
  m_FeaturesInfo.SetFieldNames( this->GetChoiceNames( "feat" ), this->GetSelectedItems( "feat" ));

  // Check input parameters
  if( m_FeaturesInfo.m_SelectedIdx.empty() )
    {
    otbAppLogFATAL( << "No features have been selected to train the classifier on!" );
    }

  ShiftScaleParameters measurement = GetStatistics( m_FeaturesInfo.m_NbFeatures );
  ExtractAllSamples( measurement );

  this->Train( m_TrainingSamplesWithLabel.listSample, m_TrainingSamplesWithLabel.labeledListSample, this->GetParameterString( "io.out" ) );

  m_PredictedList =
    this->Classify( m_ClassificationSamplesWithLabel.listSample, this->GetParameterString( "io.out" ) );
}

template <class TInputValue, class TOutputValue>
void
TrainVectorBase<TInputValue, TOutputValue>
::ExtractAllSamples(const ShiftScaleParameters &measurement)
{
  m_TrainingSamplesWithLabel = ExtractTrainingSamplesWithLabel(measurement);
  m_ClassificationSamplesWithLabel = ExtractClassificationSamplesWithLabel(measurement);
}

template <class TInputValue, class TOutputValue>
typename TrainVectorBase<TInputValue, TOutputValue>::SamplesWithLabel
TrainVectorBase<TInputValue, TOutputValue>
::ExtractTrainingSamplesWithLabel(const ShiftScaleParameters &measurement)
{
  return ExtractSamplesWithLabel( "io.vd", "layer", measurement);
}

template <class TInputValue, class TOutputValue>
typename TrainVectorBase<TInputValue, TOutputValue>::SamplesWithLabel
TrainVectorBase<TInputValue, TOutputValue>
::ExtractClassificationSamplesWithLabel(const ShiftScaleParameters &measurement)
{
  if(this->GetClassifierCategory() == Superclass::Supervised)
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

template <class TInputValue, class TOutputValue>
typename TrainVectorBase<TInputValue, TOutputValue>::ShiftScaleParameters
TrainVectorBase<TInputValue, TOutputValue>
::GetStatistics(unsigned int nbFeatures)
{
  ShiftScaleParameters measurement = ShiftScaleParameters();
  if( this->HasValue( "io.stats" ) && this->IsParameterEnabled( "io.stats" ) )
    {
    typename StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
    std::string XMLfile = this->GetParameterString( "io.stats" );
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

template <class TInputValue, class TOutputValue>
typename TrainVectorBase<TInputValue, TOutputValue>::SamplesWithLabel
TrainVectorBase<TInputValue, TOutputValue>
::ExtractSamplesWithLabel(std::string parameterName, std::string parameterLayer,
                                    const ShiftScaleParameters &measurement)
{
  SamplesWithLabel samplesWithLabel;
  if( this->HasValue( parameterName ) && this->IsParameterEnabled( parameterName ) )
    {
    typename ListSampleType::Pointer input = ListSampleType::New();
    typename TargetListSampleType::Pointer target = TargetListSampleType::New();
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
        otbAppLogWARNING( "The layer " << this->GetParameterInt( parameterLayer ) << " of " << fileList[k]
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
        {
          switch (feature[featureFieldIndex[idx]].GetType())
          {
          case OFTInteger:
            mv[idx] = static_cast<ValueType>(feature[featureFieldIndex[idx]].GetValue<int>());
            break;
          case OFTInteger64:
            mv[idx] = static_cast<ValueType>(feature[featureFieldIndex[idx]].GetValue<int>());
            break;
          case OFTReal:
            mv[idx] = static_cast<ValueType>(feature[featureFieldIndex[idx]].GetValue<double>());
            break;
          default:
            itkExceptionMacro(<< "incorrect field type: " << feature[featureFieldIndex[idx]].GetType() << ".");
          }
        }

        input->PushBack( mv );

        if(cFieldIndex>=0 && ogr::Field(feature,cFieldIndex).HasBeenSet() && !(this->GetClassifierCategory() == ClassifierCategory::Unsupervised))
        {
          switch (feature[cFieldIndex].GetType())
          {
          case OFTInteger:
            target->PushBack(static_cast<ValueType>(feature[cFieldIndex].GetValue<int>()));
            break;
          case OFTInteger64:
            target->PushBack(static_cast<ValueType>(feature[cFieldIndex].GetValue<int>()));
            break;
          case OFTReal:
            target->PushBack(static_cast<ValueType>(feature[cFieldIndex].GetValue<double>()));
            break;
          case OFTString:
            target->PushBack(static_cast<ValueType>(std::stod(feature[cFieldIndex].GetValue<std::string>())));
            break;
          default:
            itkExceptionMacro(<< "incorrect field type: " << feature[featureFieldIndex[cFieldIndex]].GetType() << ".");
          }
        }
        else
          target->PushBack( 0. );

        feature = layer.ogr().GetNextFeature();
        goesOn = feature.addr() != 0;
        }
      }



    typename ShiftScaleFilterType::Pointer shiftScaleFilter = ShiftScaleFilterType::New();
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
