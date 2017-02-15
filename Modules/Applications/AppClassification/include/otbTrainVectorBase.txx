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

  //Can be in both Supervised and Unsupervised ?
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

  AddParameter(ParameterType_ListView,  "feat", "Field names for training features.");
  SetParameterDescription("feat","List of field names in the input vector data to be used as features for training.");

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
    ogr::Layer layer = ogrDS->GetLayer( this->GetParameterInt( "layer" ) );
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
        std::string tmpKey = "feat." + key.substr( 0, end - key.begin() );
        AddChoice( tmpKey, item );
        }
      if( fieldType == OFTString || fieldType == OFTInteger || ogr::version_proxy::IsOFTInteger64( fieldType ) )
        {
        std::string tmpKey = "cfield." + key.substr( 0, end - key.begin() );
        AddChoice( tmpKey, item );
        }
      }
    }

  DoTrainUpdateParameters();
}

void TrainVectorBase::DoExecute()
{
  typedef int LabelPixelType;
  typedef itk::FixedArray<LabelPixelType, 1> LabelSampleType;
  typedef itk::Statistics::ListSample<LabelSampleType> LabelListSampleType;

  FeaturesInfo featuresInfo( GetChoiceNames( "feat" ), GetChoiceNames( "cfield" ), GetSelectedItems( "feat" ),
                             GetSelectedItems( "cfield" ) );

  // Check input parameters
  if( featuresInfo.m_SelectedIdx.empty() )
    {
    otbAppLogFATAL( << "No features have been selected to train the classifier on!" );
    }

  // Todo only Log warning and set CFieldName to 0, 1, 2, 3... (default behavior)
  if( featuresInfo.m_SelectedCFieldIdx.empty() )
    {
    otbAppLogFATAL( << "No field has been selected for data labelling!" );
    }

  StatisticsMeasurement measurement = ComputeStatistics( featuresInfo.m_NbFeatures );
  ExtractSamples(measurement, featuresInfo);

  this->Train( trainingListSamples.listSample, trainingListSamples.labeledListSample, GetParameterString( "io.out" ) );

  predictedList = TargetListSampleType::New();
  this->Classify( classificationListSamples.listSample, predictedList, GetParameterString( "io.out" ) );

  DoTrainExecute();
}


void TrainVectorBase::ExtractSamples(const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo)
{
  trainingListSamples = ExtractTrainingListSamples(measurement, featuresInfo);
  validationListSamples = ExtractValidationListSamples(measurement, featuresInfo);
  classificationListSamples = ExtractClassificationListSamples(measurement, featuresInfo);
}

TrainVectorBase::ListSamples
TrainVectorBase::ExtractTrainingListSamples(const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo)
{
  return ExtractListSamples( "io.vd", "layer", measurement, featuresInfo );
}

TrainVectorBase::ListSamples
TrainVectorBase::ExtractValidationListSamples(const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo)
{
  return ExtractListSamples( "valid.vd", "valid.layer", measurement, featuresInfo );
}


TrainVectorBase::ListSamples
TrainVectorBase::ExtractClassificationListSamples(const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo)
{
  ListSamples performanceSample;

  //Test the input validation set size
  if( validationListSamples.labeledListSample->Size() != 0 )
    {
    performanceSample.listSample = validationListSamples.listSample;
    performanceSample.labeledListSample = validationListSamples.labeledListSample;
    }
  else
    {
    otbAppLogWARNING(
            "The validation set is empty. The performance estimation is done using the input training set in this case." );
    performanceSample.listSample = trainingListSamples.listSample;
    performanceSample.labeledListSample = trainingListSamples.labeledListSample;
    }

  return performanceSample;
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
                                    const StatisticsMeasurement &measurement, FeaturesInfo &featuresInfo)
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
      //   - check class field
      int cFieldIndex = feature.ogr().GetFieldIndex( featuresInfo.m_SelectedCFieldName.c_str() );
      if( cFieldIndex < 0 )
        otbAppLogFATAL( "The field name for class label (" << featuresInfo.m_SelectedCFieldName
                                                           << ") has not been found in the vector file "
                                                           << validFileList[k] );
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
        if( feature.ogr().IsFieldSet( cFieldIndex ) )
          {
          MeasurementType mv;
          mv.SetSize( featuresInfo.m_NbFeatures );
          for( unsigned int idx = 0; idx < featuresInfo.m_NbFeatures; ++idx )
            mv[idx] = feature.ogr().GetFieldAsDouble( featureFieldIndex[idx] );

          input->PushBack( mv );
          target->PushBack( feature.ogr().GetFieldAsInteger( cFieldIndex ) );
          }
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


