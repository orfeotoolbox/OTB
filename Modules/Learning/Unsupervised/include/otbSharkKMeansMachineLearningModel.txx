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
#ifndef otbSharkKMeansMachineLearningModel_txx
#define otbSharkKMeansMachineLearningModel_txx

#include <fstream>
#include "itkMacro.h"
#include "otbSharkKMeansMachineLearningModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif

#include "otbSharkUtils.h"
#include "shark/Algorithms/Trainers/NormalizeComponentsUnitVariance.h" //normalize
#include "shark/Algorithms/KMeans.h" //k-means algorithm
#include "shark/Models/Clustering/HardClusteringModel.h"
#include "shark/Models/Clustering/SoftClusteringModel.h"
#include "shark/Algorithms/Trainers/NormalizeComponentsUnitVariance.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif



namespace otb
{
template<class TInputValue, class TOutputValue>
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::SharkKMeansMachineLearningModel() :
        m_Normalized( true ), m_K(2), m_MaximumNumberOfIterations( 0 )
{
  // Default set HardClusteringModel
  m_ClusteringModel = boost::shared_ptr<ClusteringModelType>(new ClusteringModelType( &m_Centroids ));
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
  std::vector<RealVector> vector_data;
  Shark::ListSampleToSharkVector( this->GetInputListSample(), vector_data );
  Data<RealVector> data = createDataFromRange( vector_data );

  // Normalized input value if necessary
  if( m_Normalized )
    data = NormalizeData( data );

  // Use a Hard Clustering Model for classification
  kMeans( data, m_K, m_Centroids, m_MaximumNumberOfIterations );
  m_ClusteringModel = boost::shared_ptr<ClusteringModelType>(new ClusteringModelType( &m_Centroids ));
}

template<class TInputValue, class TOutputValue>
template<typename DataType>
DataType
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::NormalizeData(const DataType &data) const
{
  shark::Normalizer<> normalizer;
  shark::NormalizeComponentsUnitVariance<> normalizingTrainer( true );//zero mean
  normalizingTrainer.train( normalizer, data );
  return normalizer( data );
}

template<class TInputValue, class TOutputValue>
typename SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::TargetSampleType
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::DoPredict(const InputSampleType &value, ConfidenceValueType *quality) const
{
  RealVector data( value.Size());
  for( size_t i = 0; i < value.Size(); i++ )
    {
    data.push_back( value[i] );
    }

  // Change quality measurement only if SoftClustering or other clustering method is used.
  if( quality != ITK_NULLPTR )
    {
    //unsigned int probas = (*m_ClusteringModel)( data );
    ( *quality ) = ConfidenceValueType( 1.);
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
                 ConfidenceListSampleType *quality) const
{

  // Perform check on input values
  assert( input != ITK_NULLPTR );
  assert( targets != ITK_NULLPTR );

  // input list sample and target list sample should be initialized and without
  assert( input->Size() == targets->Size() && "Input sample list and target label list do not have the same size." );
  assert( ( ( quality == ITK_NULLPTR ) || ( quality->Size() == input->Size() ) ) &&
          "Quality samples list is not null and does not have the same size as input samples list" );
  if( startIndex + size > input->Size() )
    {
    itkExceptionMacro(
            <<"requested range ["<<startIndex<<", "<<startIndex+size<<"[ partially outside input sample list range.[0,"<<input->Size()<<"[" );
    }

  // Convert input list of features to shark data format
  std::vector<RealVector> features;
  Shark::ListSampleRangeToSharkVector( input, features, startIndex, size );
  Data<RealVector> inputSamples = shark::createDataFromRange( features );

  Data<ClusteringOutputType> clusters;
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
  if( quality != ITK_NULLPTR )
    {
    for( unsigned int qid = startIndex; qid < size; ++qid )
      {
      quality->SetMeasurementVector( qid, static_cast<ConfidenceValueType>(1.) );
      }
    }

}


template<class TInputValue, class TOutputValue>
void
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::Save(const std::string &filename, const std::string & itkNotUsed( name ))
{
  std::ofstream ofs( filename.c_str());
  if( !ofs )
    {
    itkExceptionMacro( << "Error opening " << filename.c_str());
    }
  shark::TextOutArchive oa( ofs );
  std::string name = m_ClusteringModel->name();
  oa << name;
  m_ClusteringModel->save( oa, 1 );
}

template<class TInputValue, class TOutputValue>
void
SharkKMeansMachineLearningModel<TInputValue, TOutputValue>
::Load(const std::string &filename, const std::string & itkNotUsed( name ))
{
  m_CanRead = false;
  std::ifstream ifs( filename.c_str());
  if(ifs.good())
    {
    std::string line;
    std::getline(ifs, line);
    m_CanRead = line.find(m_ClusteringModel->name()) != std::string::npos;
    }

  if(!m_CanRead)
    return;

  // Go to the start of the file
  ifs.seekg(0, std::ios::beg);
  shark::TextInArchive ia( ifs );
  std::string name;
  ia & name;
  m_ClusteringModel->load( ia, 1 );
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
