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

#ifndef otbLibSVMMachineLearningModel_txx
#define otbLibSVMMachineLearningModel_txx

#include <fstream>
#include "otbLibSVMMachineLearningModel.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::LibSVMMachineLearningModel()
{
  m_SVMestimator = SVMEstimatorType::New();
  m_SVMestimator->SetSVMType(C_SVC);
  m_SVMestimator->SetC(1.0);
  m_SVMestimator->SetKernelType(LINEAR);
  m_SVMestimator->SetParametersOptimization(false);
  m_SVMestimator->DoProbabilityEstimates(false);
  //m_SVMestimator->SetEpsilon(1e-6);
  this->m_IsRegressionSupported = true;
}


template <class TInputValue, class TOutputValue>
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::~LibSVMMachineLearningModel()
{
  //delete m_SVMModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  // Set up SVM's parameters
  // CvSVMParams params;
  // params.svm_type    = m_SVMType;
  // params.kernel_type = m_KernelType;
  // params.term_crit   = cvTermCriteria(m_TermCriteriaType, m_MaxIter, m_Epsilon);

  // // Train the SVM
  m_SVMestimator->SetInputSampleList(this->GetInputListSample());
  m_SVMestimator->SetTrainingSampleList(this->GetTargetListSample());

  m_SVMestimator->Update();

  this->m_ConfidenceIndex = this->GetDoProbabilityEstimates();
}

template <class TInputValue, class TOutputValue>
typename LibSVMMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::DoPredict(const InputSampleType & input, ConfidenceValueType *quality) const
{
  TargetSampleType target;

  MeasurementVectorFunctorType mfunctor;

  target = m_SVMestimator->GetModel()->EvaluateLabel(mfunctor(input));

  if (quality != ITK_NULLPTR)
    {
    if (!this->m_ConfidenceIndex)
      {
      itkExceptionMacro("Confidence index not available for this classifier !");
      }
    typename SVMEstimatorType::ModelType::ProbabilitiesVectorType probaVector =
        m_SVMestimator->GetModel()->EvaluateProbabilities(mfunctor(input));
    double maxProb = 0.0;
    double secProb = 0.0;
    for (unsigned int i=0 ; i<probaVector.Size() ; ++i)
      {
      if (maxProb < probaVector[i])
        {
          secProb = maxProb;
          maxProb = probaVector[i];
        }
      else if (secProb < probaVector[i])
        {
          secProb = probaVector[i];
        }
      }
    (*quality) = static_cast<ConfidenceValueType>(maxProb - secProb);
    }

  return target;
}

template <class TInputValue, class TOutputValue>
void
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & itkNotUsed(name))
{
  m_SVMestimator->GetModel()->SaveModel(filename.c_str());
}

template <class TInputValue, class TOutputValue>
void
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & itkNotUsed(name))
{
  m_SVMestimator->GetModel()->LoadModel(filename.c_str());

  this->m_ConfidenceIndex = m_SVMestimator->GetModel()->HasProbabilities();
}

template <class TInputValue, class TOutputValue>
bool
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const std::string & file)
{
  //TODO: Rework.
  std::ifstream ifs;
  ifs.open(file.c_str());

  if(!ifs)
    {
    std::cerr<<"Could not read file "<<file<<std::endl;
    return false;
    }

  //Read only the first line.
  std::string line;
  std::getline(ifs, line);

  //if (line.find(m_SVMModel->getName()) != std::string::npos)
  if (line.find("svm_type") != std::string::npos)
    {
    //std::cout<<"Reading a libSVM model"<<std::endl;
    return true;
    }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
