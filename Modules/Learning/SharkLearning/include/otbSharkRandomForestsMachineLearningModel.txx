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
#ifndef __otbSharkRandomForestsMachineLearningModel_txx
#define __otbSharkRandomForestsMachineLearningModel_txx

#include <fstream>
#include "itkMacro.h"
#include "otbSharkRandomForestsMachineLearningModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif
#include <shark/Models/Converter.h>
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif



#include "otbSharkUtils.h"
#include <algorithm>

namespace otb
{

template <class TInputValue, class TOutputValue>
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::SharkRandomForestsMachineLearningModel()
{
  this->m_ConfidenceIndex = true;
  this->m_IsRegressionSupported = false;
  this->m_IsDoPredictBatchMultiThreaded = true;
}


template <class TInputValue, class TOutputValue>
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::~SharkRandomForestsMachineLearningModel()
{
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
#ifdef _OPENMP
  omp_set_num_threads(itk::MultiThreader::GetGlobalDefaultNumberOfThreads());
#endif
  
  std::vector<shark::RealVector> features;
  std::vector<unsigned int> class_labels;

  Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
  Shark::ListSampleToSharkVector(this->GetTargetListSample(), class_labels);
  shark::ClassificationDataset TrainSamples = shark::createLabeledDataFromRange(features,class_labels);

  //Set parameters
  m_RFTrainer.setMTry(m_MTry);
  m_RFTrainer.setNTrees(m_NumberOfTrees);
  m_RFTrainer.setNodeSize(m_NodeSize);
  m_RFTrainer.setOOBratio(m_OobRatio);
  m_RFTrainer.train(m_RFModel, TrainSamples);

}

template <class TInputValue, class TOutputValue>
typename SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::ConfidenceValueType
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::ComputeConfidence(shark::RealVector & probas, bool computeMargin) const
{
  assert(!probas.empty()&&"probas vector is empty");
  assert((!computeMargin||probas.size()>1)&&"probas size should be at least 2 if computeMargin is true");
  
  ConfidenceValueType conf{0};
  if(computeMargin)
    {
    std::nth_element(probas.begin(), probas.begin()+1, 
                     probas.end(), std::greater<double>());
    conf = static_cast<ConfidenceValueType>(probas[0]-probas[1]);
    }
  else
    {
    auto max_proba = *(std::max_element(probas.begin(), 
                                        probas.end()));
    conf = static_cast<ConfidenceValueType>(max_proba);
    }
  return conf;
}

template <class TInputValue, class TOutputValue>
typename SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::DoPredict(const InputSampleType & value, ConfidenceValueType *quality) const
{
  shark::RealVector samples(value.Size());
  for(size_t i = 0; i < value.Size();i++)
    {
    samples.push_back(value[i]);
    }
  if (quality != ITK_NULLPTR)
    {
    shark::RealVector & probas = m_RFModel(samples);
    (*quality) = ComputeConfidence(probas, m_ComputeMargin);
    }
  shark::ArgMaxConverter<shark::RFClassifier> amc;
  amc.decisionFunction() = m_RFModel;
  unsigned int res;
  amc.eval(samples, res);
  TargetSampleType target;
  target[0] = static_cast<TOutputValue>(res);
  return target;
}

template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::DoPredictBatch(const InputListSampleType *input, const unsigned int & startIndex, const unsigned int & size, TargetListSampleType * targets, ConfidenceListSampleType * quality) const
{
  assert(input != ITK_NULLPTR);
  assert(targets != ITK_NULLPTR);

  assert(input->Size()==targets->Size()&&"Input sample list and target label list do not have the same size.");
  assert(((quality==ITK_NULLPTR)||(quality->Size()==input->Size()))&&"Quality samples list is not null and does not have the same size as input samples list");
  
  if(startIndex+size>input->Size())
    {
    itkExceptionMacro(<<"requested range ["<<startIndex<<", "<<startIndex+size<<"[ partially outside input sample list range.[0,"<<input->Size()<<"[");
    }
  
  std::vector<shark::RealVector> features;
  Shark::ListSampleRangeToSharkVector(input, features,startIndex,size);
  shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange(features);

  #ifdef _OPENMP
  omp_set_num_threads(itk::MultiThreader::GetGlobalDefaultNumberOfThreads());
  #endif
  
  auto probas = m_RFModel(inputSamples);

  if(quality != ITK_NULLPTR)
    {
    unsigned int id = startIndex;
    for(const auto& p : probas.elements())
      {
      ConfidenceSampleType confidence;
      auto conf = ComputeConfidence(p, m_ComputeMargin);
      confidence[0] = static_cast<ConfidenceValueType>(conf);
      quality->SetMeasurementVector(id,confidence);
      ++id;
      }
    }
    
  shark::ArgMaxConverter<shark::RFClassifier> amc;
  amc.decisionFunction() = m_RFModel;
  auto prediction = amc(inputSamples);
  unsigned int id = startIndex;
  for(const auto& p : prediction.elements())
    {
    TargetSampleType target;
    target[0] = static_cast<TOutputValue>(p);
    targets->SetMeasurementVector(id,target);
    ++id;
    }
}

template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & itkNotUsed(name))
{
  std::ofstream ofs(filename.c_str());
  if(!ofs)
    {
    itkExceptionMacro(<< "Error opening " << filename.c_str() );
    }
  boost::archive::polymorphic_text_oarchive oa(ofs);
  m_RFModel.save(oa,0);
}

template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & itkNotUsed(name))
{
  std::ifstream ifs(filename.c_str());
  boost::archive::polymorphic_text_iarchive ia(ifs);
  m_RFModel.load(ia,0);
}

template <class TInputValue, class TOutputValue>
bool
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const std::string & file)
{
  try
    {
    this->Load(file);
    }
  catch(...)
    {
    return false;
    }
  return true;
}

template <class TInputValue, class TOutputValue>
bool
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & itkNotUsed(file))
{
  return true;
}

template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
