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
//#include "otbOpenCVUtils.h"

// SVM estimator
//#include "otbSVMSampleListModelEstimator.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::LibSVMMachineLearningModel()
{
  // m_SVMModel = new CvSVM;
  // m_SVMType = CvSVM::C_SVC;
  m_KernelType = LINEAR;
  // m_TermCriteriaType = CV_TERMCRIT_ITER;
  m_C = 1.0;
  // m_Epsilon = 1e-6;
  m_ParameterOptimization = false;
  m_SVMestimator = SVMEstimatorType::New();
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

  m_SVMestimator->SetC(m_C);
  m_SVMestimator->SetKernelType(m_KernelType);
  m_SVMestimator->SetParametersOptimization(m_ParameterOptimization);

  m_SVMestimator->SetInputSampleList(this->GetInputListSample());
  m_SVMestimator->SetTrainingSampleList(this->GetTargetListSample());

  m_SVMestimator->Update();
}

template <class TInputValue, class TOutputValue>
typename LibSVMMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input) const
{
  TargetSampleType target;

  otbMsgDevMacro(<< "Starting iterations ");

  MeasurementVectorFunctorType mfunctor;

  target = m_SVMestimator->GetModel()->EvaluateLabel(mfunctor(input));

  return target;
}

template <class TInputValue, class TOutputValue>
void
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  m_SVMestimator->GetModel()->SaveModel(filename.c_str());
}

template <class TInputValue, class TOutputValue>
void
LibSVMMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  m_SVMestimator->GetModel()->LoadModel(filename.c_str());
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
::CanWriteFile(const std::string & file)
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
