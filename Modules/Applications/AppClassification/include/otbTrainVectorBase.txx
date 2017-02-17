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
#ifndef otbTrainVectorBase_txx
#define otbTrainVectorBase_txx

#include "otbTrainVectorBase.h"

namespace otb
{
namespace Wrapper
{

void TrainVectorBase::DoInit()
{
  SetName( "TrainVectorClassifier" );
  SetDescription( "Train a classifier based on labeled geometries and a list of features to consider." );

  SetDocName( "Train Vector Classifier" );
  SetDocLongDescription( "This application trains a classifier based on "
                                 "labeled geometries and a list of features to consider for classification." );
  SetDocLimitations( " " );
  SetDocAuthors( "OTB Team" );
  SetDocSeeAlso( " " );

  // Common Parameters for all Learning Application
  AddParameter( ParameterType_Group, "io", "Input and output data" );
  SetParameterDescription( "io", "This group of parameters allows setting input and output data." );

  AddParameter( ParameterType_InputVectorDataList, "io.vd", "Input Vector Data" );
  SetParameterDescription( "io.vd",
                           "Input geometries used for training (note : all geometries from the layer will be used)" );

  AddParameter( ParameterType_InputFilename, "io.stats", "Input XML image statistics file" );
  MandatoryOff( "io.stats" );
  SetParameterDescription( "io.stats", "XML file containing mean and variance of each feature." );

  AddParameter( ParameterType_OutputFilename, "io.out", "Output model" );
  SetParameterDescription( "io.out", "Output file containing the model estimated (.txt format)." );

  AddParameter( ParameterType_Int, "layer", "Layer Index" );
  SetParameterDescription( "layer", "Index of the layer to use in the input vector file." );
  MandatoryOff( "layer" );
  SetDefaultParameterInt( "layer", 0 );

  AddParameter(ParameterType_ListView,  "feat", "Field names for training features.");
  SetParameterDescription("feat","List of field names in the input vector data to be used as features for training.");

  // Add validation data used to compute confusion matrix or contingence table
  AddParameter( ParameterType_Group, "valid", "Validation data" );
  SetParameterDescription( "valid", "This group of parameters defines validation data." );

  AddParameter( ParameterType_InputVectorDataList, "valid.vd", "Validation Vector Data" );
  SetParameterDescription( "valid.vd", "Geometries used for validation "
          "(must contain the same fields used for training, all geometries from the layer will be used)" );
  MandatoryOff( "valid.vd" );

  AddParameter( ParameterType_Int, "valid.layer", "Layer Index" );
  SetParameterDescription( "valid.layer", "Index of the layer to use in the validation vector file." );
  MandatoryOff( "valid.layer" );
  SetDefaultParameterInt( "valid.layer", 0 );

  // Add class field if we used validation
  AddParameter(ParameterType_ListView,"cfield","Field containing the class id for supervision");
  SetParameterDescription("cfield","Field containing the class id for supervision. "
          "Only geometries with this field available will be taken into account.");
  SetListViewSingleSelectionMode("cfield",true);


  // Add parameters for the classifier choice
  Superclass::DoInit();

  AddRANDParameter();

  DoTrainInit();
}

void TrainVectorBase::DoUpdateParameters()
{
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

  DoTrainUpdateParameters();
}

void TrainVectorBase::DoExecute()
{
  DoBeforeTrainExecute();

  featuresInfo.SetFieldNames( GetChoiceNames( "feat" ), GetSelectedItems( "feat" ));

  // Check input parameters
  if( featuresInfo.m_SelectedIdx.empty() )
    {
    otbAppLogFATAL( << "No features have been selected to train the classifier on!" );
    }

  StatisticsMeasurement measurement = ComputeStatistics( featuresInfo.m_NbFeatures );
  ExtractAllSamples( measurement );

  this->Train( trainingListSamples.listSample, trainingListSamples.labeledListSample, GetParameterString( "io.out" ) );

  predictedList = TargetListSampleType::New();
  this->Classify( classificationListSamples.listSample, predictedList, GetParameterString( "io.out" ) );

  DoAfterTrainExecute();
}


void TrainVectorBase::ExtractAllSamples(const StatisticsMeasurement &measurement)
{
  trainingListSamples = ExtractTrainingListSamples(measurement);
  classificationListSamples = ExtractClassificationListSamples(measurement);
}

TrainVectorBase::ListSamples
TrainVectorBase::ExtractTrainingListSamples(const StatisticsMeasurement &measurement)
{
  return ExtractListSamples( "io.vd", "layer", measurement);
}

TrainVectorBase::ListSamples
TrainVectorBase::ExtractClassificationListSamples(const StatisticsMeasurement &itkNotUsed(measurement))
{
  return trainingListSamples;
}


TrainVectorBase::StatisticsMeasurement
TrainVectorBase::ComputeStatistics(unsigned int nbFeatures)
{
  StatisticsMeasurement measurement = StatisticsMeasurement();
  if( HasValue( "io.stats" ) && IsParameterEnabled( "io.stats" ) )
    {
    StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
    std::string XMLfile = GetParameterString( "io.stats" );
    statisticsReader->SetFileName( XMLfile.c_str() );
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


TrainVectorBase::ListSamples
TrainVectorBase::ExtractListSamples(std::string parameterName, std::string parameterLayer,
                                    const StatisticsMeasurement &measurement)
{
  ListSamples listSamples;
  if( HasValue( parameterName ) && IsParameterEnabled( parameterName ) )
    {
    ListSampleType::Pointer input = ListSampleType::New();
    TargetListSampleType::Pointer target = TargetListSampleType::New();
    input->SetMeasurementVectorSize( featuresInfo.m_NbFeatures );

    std::vector<std::string> validFileList = this->GetParameterStringList( parameterName );
    for( unsigned int k = 0; k < validFileList.size(); k++ )
      {
      otbAppLogINFO( "Reading validation vector file " << k + 1 << "/" << validFileList.size() );
      ogr::DataSource::Pointer source = ogr::DataSource::New( validFileList[k], ogr::DataSource::Modes::Read );
      ogr::Layer layer = source->GetLayer( static_cast<size_t>(this->GetParameterInt( parameterLayer )) );
      ogr::Feature feature = layer.ogr().GetNextFeature();
      bool goesOn = feature.addr() != 0;
      if( !goesOn )
        {
        otbAppLogWARNING( "The layer " << GetParameterInt( parameterLayer ) << " of " << validFileList[k]
                                       << " is empty, input is skipped." );
        continue;
        }

      // Check all needed fields are present :
      //   - check class field if we use supervised classification or if class field name is not empty
      int cFieldIndex = feature.ogr().GetFieldIndex( featuresInfo.m_SelectedCFieldName.c_str() );
      if( cFieldIndex < 0 && !featuresInfo.m_SelectedCFieldName.empty())
        {
        otbAppLogFATAL( "The field name for class label (" << featuresInfo.m_SelectedCFieldName
                                                           << ") has not been found in the vector file "
                                                           << validFileList[k] );
        }

      //   - check feature fields
      std::vector<int> featureFieldIndex( featuresInfo.m_NbFeatures, -1 );
      for( unsigned int i = 0; i < featuresInfo.m_NbFeatures; i++ )
        {
        featureFieldIndex[i] = feature.ogr().GetFieldIndex( featuresInfo.m_SelectedNames[i].c_str() );
        if( featureFieldIndex[i] < 0 )
          otbAppLogFATAL( "The field name for feature " << featuresInfo.m_SelectedNames[i]
                                                        << " has not been found in the vector file "
                                                        << validFileList[k] );
        }


      while( goesOn )
        {
        // Retrieve all the features for each field in the ogr layer.
        MeasurementType mv;
        mv.SetSize( featuresInfo.m_NbFeatures );
        for( unsigned int idx = 0; idx < featuresInfo.m_NbFeatures; ++idx )
          mv[idx] = feature.ogr().GetFieldAsDouble( featureFieldIndex[idx] );

        input->PushBack( mv );

        if( feature.ogr().IsFieldSet( cFieldIndex ) )
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

    listSamples.listSample = shiftScaleFilter->GetOutput();
    listSamples.labeledListSample = target;
    }

  return listSamples;
}


}
}

#endif


