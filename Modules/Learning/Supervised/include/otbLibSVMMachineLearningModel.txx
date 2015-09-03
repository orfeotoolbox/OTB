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
#ifndef __otbLibSVMMachineLearningModel_txx
#define __otbLibSVMMachineLearningModel_txx

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
::Predict(const InputSampleType & input, ConfidenceValueType *quality) const
{
  TargetSampleType target;

  MeasurementVectorFunctorType mfunctor;

  target = m_SVMestimator->GetModel()->EvaluateLabel(mfunctor(input));

  if (quality != NULL)
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
