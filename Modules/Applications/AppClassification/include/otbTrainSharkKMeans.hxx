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
#ifndef otbTrainSharkKMeans_hxx
#define otbTrainSharkKMeans_hxx

#include "otbLearningApplicationBase.h"
#include "otbSharkKMeansMachineLearningModel.h"
#include "otbStatisticsXMLFileReader.h"
#include "shark/Models/Normalizer.h"

namespace otb
{
namespace Wrapper
{
template<class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::InitSharkKMeansParams()
{
  AddChoice( "classifier.sharkkm", "Shark kmeans classifier" );
  SetParameterDescription("classifier.sharkkm", "http://image.diku.dk/shark/sphinx_pages/build/html/rest_sources/tutorials/algorithms/kmeans.html ");

  // MaxNumberOfIterations
  AddParameter(ParameterType_Int, "classifier.sharkkm.maxiter", "Maximum number of iterations for the kmeans algorithm");
  SetParameterInt("classifier.sharkkm.maxiter", 10);
  SetMinimumParameterIntValue("classifier.sharkkm.maxiter", 0);
  SetParameterDescription("classifier.sharkkm.maxiter", "The maximum number of iterations for the kmeans algorithm. 0=unlimited");

  // Number of classes
  AddParameter(ParameterType_Int, "classifier.sharkkm.k", "Number of classes for the kmeans algorithm");
  SetParameterInt("classifier.sharkkm.k", 2);
  SetParameterDescription("classifier.sharkkm.k", "The number of classes used for the kmeans algorithm. Default set to 2 class");
  SetMinimumParameterIntValue("classifier.sharkkm.k", 2);

  // Input centroids
  AddParameter(ParameterType_InputFilename, "classifier.sharkkm.centroids", "User definied input centroids");
  SetParameterDescription("classifier.sharkkm.centroids", "Text file containing input centroids.");
  MandatoryOff("classifier.sharkkm.centroids");

  // Centroid statistics
  AddParameter(ParameterType_InputFilename, "classifier.sharkkm.centroidstats", "Statistics file");
  SetParameterDescription("classifier.sharkkm.centroidstats", "A XML file containing mean and standard deviation to center"
    "and reduce the centroids before the KMeans algorithm, produced by ComputeImagesStatistics application.");
  MandatoryOff("classifier.sharkkm.centroidstats");
  
  // output centroids
  AddParameter(ParameterType_OutputFilename, "classifier.sharkkm.outcentroids", "Output centroids text file");
  SetParameterDescription("classifier.sharkkm.outcentroids", "Output text file containing centroids after the kmean algorithm.");
  MandatoryOff("classifier.sharkkm.outcentroids");
  
}

template<class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::TrainSharkKMeans(
        typename ListSampleType::Pointer trainingListSample,
        typename TargetListSampleType::Pointer trainingLabeledListSample, std::string modelPath)
{
  unsigned int nbMaxIter = static_cast<unsigned int>(abs( GetParameterInt( "classifier.sharkkm.maxiter" ) ));
  unsigned int k = static_cast<unsigned int>(abs( GetParameterInt( "classifier.sharkkm.k" ) ));

  typedef otb::SharkKMeansMachineLearningModel<InputValueType, OutputValueType> SharkKMeansType;
  typename SharkKMeansType::Pointer classifier = SharkKMeansType::New();
  classifier->SetRegressionMode( this->m_RegressionFlag );
  classifier->SetInputListSample( trainingListSample );
  classifier->SetTargetListSample( trainingLabeledListSample );
  classifier->SetK( k );

  // Initialize centroids from file
  if(IsParameterEnabled("classifier.sharkkm.centroids") && HasValue("classifier.sharkkm.centroids"))
  {
    shark::Data<shark::RealVector> centroidData;
    shark::importCSV(centroidData, GetParameterString( "classifier.sharkkm.centroids"), ' ');
    if( HasValue( "classifier.sharkkm.centroidstats" ) )
    {
      auto statisticsReader = otb::StatisticsXMLFileReader< itk::VariableLengthVector<float> >::New();
      statisticsReader->SetFileName(GetParameterString( "classifier.sharkkm.centroidstats" ));
      auto meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
      auto stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
  
      // Convert itk Variable Length Vector to shark Real Vector
      shark::RealVector offsetRV(meanMeasurementVector.Size());
      shark::RealVector stddevMeasurementRV(stddevMeasurementVector.Size());
      
      assert(meanMeasurementVector.Size()==stddevMeasurementVector.Size());
      for (unsigned int i = 0; i<meanMeasurementVector.Size(); ++i)
      {
        stddevMeasurementRV[i] = 1/stddevMeasurementVector[i];
        // Substract the normalized mean
        offsetRV[i] = - meanMeasurementVector[i]/stddevMeasurementVector[i];
      }
      
      shark::Normalizer<> normalizer(stddevMeasurementRV, offsetRV);
      centroidData = normalizer(centroidData);
    }
    
    classifier->SetCentroidsFromData( centroidData);
  }
  
  classifier->SetMaximumNumberOfIterations( nbMaxIter );
  classifier->Train();
  classifier->Save( modelPath );
  
  if( HasValue( "classifier.sharkkm.outcentroids"))
    classifier->ExportCentroids( GetParameterString( "classifier.sharkkm.outcentroids" ));
}

} //end namespace wrapper
} //end namespace otb

#endif
