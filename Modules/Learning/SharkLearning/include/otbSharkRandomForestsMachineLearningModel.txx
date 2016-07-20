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
#include <shark/Models/Converter.h>
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
  this->m_ConfidenceBatchMode = false;
  m_ConfidenceListSample = ConfidenceListSampleType::New();
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
::ComputeConfidence(shark::RealVector probas, bool computeMargin) const
{
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
::Predict(const InputSampleType & value, ConfidenceValueType *quality) const
{
  shark::RealVector samples;
  for(size_t i = 0; i < value.Size();i++)
    {
    samples.push_back(value[i]);
    }
  if (quality != NULL)
    {
    auto probas = m_RFModel(samples);
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
::PredictAll()
{
  std::vector<shark::RealVector> features;
  Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
  shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange(features);
  if(this->m_ConfidenceBatchMode)
    {
    auto probas = m_RFModel(inputSamples);
    ConfidenceListSampleType * confidences = this->GetConfidenceListSample();
    confidences->Clear();
    for(const auto& p : probas.elements())
      {
      ConfidenceSampleType confidence;
      auto conf = ComputeConfidence(p, m_ComputeMargin);
      confidence[0] = static_cast<ConfidenceValueType>(conf);
      confidences->PushBack(confidence);
      }
    }
  shark::ArgMaxConverter<shark::RFClassifier> amc;
  amc.decisionFunction() = m_RFModel;
  auto prediction = amc(inputSamples);
  TargetListSampleType * targets = this->GetTargetListSample();
  targets->Clear();
  for(const auto& p : prediction.elements())
    {
    TargetSampleType target;
    target[0] = static_cast<TOutputValue>(p);
    targets->PushBack(target);
    }
}

template <class TInputValue, class TOutputValue>
void
SharkRandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & itkNotUsed(name))
{
  std::ofstream ofs(filename.c_str());
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
