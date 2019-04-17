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
#ifndef otbSharkKMeansMachineLearningModel_hxx
#define otbSharkKMeansMachineLearningModel_hxx

#include <fstream>
#include "boost/make_shared.hpp"
#include "itkMacro.h"
#include "otbSharkKMeansMachineLearningModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif

#include "otb_shark.h"
#include "otbSharkUtils.h"
#include "shark/Algorithms/KMeans.h" //k-means algorithm
#include "shark/Models/Clustering/HardClusteringModel.h"
#include "shark/Models/Clustering/SoftClusteringModel.h"
#include <shark/Data/Csv.h> //load the csv file

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif



namespace otb
{
template<class TInputValue, class TOutputValue>
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::SharkKMeansMachineLearningModel() :
        m_K(2), m_MaximumNumberOfIterations( 10 )
{
  // Default set HardClusteringModel
  this->m_ConfidenceIndex = true;
  m_ClusteringModel = boost::make_shared<ClusteringModelType>( &m_Centroids );
}


template<class TInputValue, class TOutputValue>
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::~SharkKMeansMachineLearningModel()
{
}

/** Train the machine learning model */
template<class TInputValue, class TOutputValue>
void
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::Train()
{
  // Parse input data and convert to Shark Data
  std::vector<shark::RealVector> vector_data;
  otb::Shark::ListSampleToSharkVector( this->GetInputListSample(), vector_data );
  shark::Data<shark::RealVector> data = shark::createDataFromRange( vector_data );

  // Use a Hard Clustering Model for classification
  shark::kMeans( data, m_K, m_Centroids, m_MaximumNumberOfIterations );
  m_ClusteringModel = boost::make_shared<ClusteringModelType>( &m_Centroids );
}

template<class TInputValue, class TOutputValue>
typename SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::TargetSampleType
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::DoPredict(const InputSampleType &value, ConfidenceValueType *quality, ProbaSampleType *proba) const
{
  shark::RealVector data( value.Size());
  for( size_t i = 0; i < value.Size(); i++ )
    {
    data.push_back( value[i] );
    }

  // Change quality measurement only if SoftClustering or other clustering method is used.
  if( quality != nullptr )
    {
    //unsigned int probas = (*m_ClusteringModel)( data );
    ( *quality ) = ConfidenceValueType( 1.);
    }

  if (proba != nullptr)
    {
    if (!this->m_ProbaIndex)
      {
      itkExceptionMacro("Probability per class not available for this classifier !");
      }
    }
  TargetSampleType target;
  ClusteringOutputType predictedValue = (*m_ClusteringModel)( data );
  target[0] = static_cast<TOutputValue>(predictedValue);
  return target;
}

template<class TInputValue, class TOutputValue>
void
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::DoPredictBatch(const InputListSampleType *input,
                 const unsigned int &startIndex,
                 const unsigned int &size,
                 TargetListSampleType *targets,
                 ConfidenceListSampleType *quality,
		 ProbaListSampleType * proba) const
{

  // Perform check on input values
  assert( input != nullptr );
  assert( targets != nullptr );

  // input list sample and target list sample should be initialized and without
  assert( input->Size() == targets->Size() && "Input sample list and target label list do not have the same size." );
  assert( ( ( quality == nullptr ) || ( quality->Size() == input->Size() ) ) &&
          "Quality samples list is not null and does not have the same size as input samples list" );
  if( startIndex + size > input->Size() )
    {
    itkExceptionMacro(
            <<"requested range ["<<startIndex<<", "<<startIndex+size<<"[ partially outside input sample list range.[0,"<<input->Size()<<"[" );
    }

  // Convert input list of features to shark data format
  std::vector<shark::RealVector> features;
  otb::Shark::ListSampleRangeToSharkVector( input, features, startIndex, size );
  shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );

  shark::Data<ClusteringOutputType> clusters;
  try
    {
    clusters = ( *m_ClusteringModel )( inputSamples );
    }
  catch( ... )
    {
    itkExceptionMacro( "Failed to run clustering classification. "
                               "The number of features of input samples and the model could differ.");
    }

  unsigned int id = startIndex;
  for( const auto &p : clusters.elements() )
    {
    TargetSampleType target;
    target[0] = static_cast<TOutputValue>(p);
    targets->SetMeasurementVector( id, target );
    ++id;
    }

  // Change quality measurement only if SoftClustering or other clustering method is used.
  if( quality != nullptr )
    {
    for( unsigned int qid = startIndex; qid < startIndex+size; ++qid )
      {
      quality->SetMeasurementVector( qid, static_cast<ConfidenceValueType>(1.) );
      }
    }
  if (proba !=nullptr && !this->m_ProbaIndex)
    {
      itkExceptionMacro("Probability per class not available for this classifier !");
    }
}


template<class TInputValue, class TOutputValue>
void
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::Save(const std::string &filename, const std::string & itkNotUsed( name ))
{
  std::ofstream ofs( filename);
  if( !ofs )
    {
    itkExceptionMacro( << "Error opening " << filename.c_str());
    }
  ofs << "#" << m_ClusteringModel->name() << std::endl;
  shark::TextOutArchive oa( ofs );
  m_ClusteringModel->save( oa, 1 );
}

template<class TInputValue, class TOutputValue>
void
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::Load(const std::string &filename, const std::string & itkNotUsed( name ))
{
  m_CanRead = false;
  std::ifstream ifs( filename);
  if(ifs.good())
    {
    // Check if first line contains model name
    std::string line;
    std::getline(ifs, line);
    m_CanRead = line.find(m_ClusteringModel->name()) != std::string::npos;
    }

  if(!m_CanRead)
    return;

  shark::TextInArchive ia( ifs );
  m_ClusteringModel->load( ia, 0 );
  ifs.close();
}

template<class TInputValue, class TOutputValue>
bool
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::CanReadFile(const std::string &file)
{
  try
    {
    m_CanRead = true;
    this->Load( file );
    }
  catch( ... )
    {
    return false;
    }
  return m_CanRead;
}

template<class TInputValue, class TOutputValue>
bool
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::CanWriteFile(const std::string & itkNotUsed( file ))
{
  return true;
}

template<class TInputValue, class TOutputValue>
void
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::PrintSelf(std::ostream &os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf( os, indent );
}
} //end namespace otb

#endif
