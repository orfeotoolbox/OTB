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
#ifndef __otbSVMMachineLearningModel_txx
#define __otbSVMMachineLearningModel_txx

#include "otbSVMMachineLearningModel.h"
#include "otbOpenCVUtils.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
SVMMachineLearningModel<TInputValue,TOutputValue>
::SVMMachineLearningModel()
{
  m_SVMModel = new CvSVM;
  m_SVMType = CvSVM::C_SVC;
  m_KernelType = CvSVM::LINEAR;
  m_TermCriteriaType = CV_TERMCRIT_ITER;
  m_MaxIter = 100;
  m_Epsilon = 1e-6;
}


template <class TInputValue, class TOutputValue>
SVMMachineLearningModel<TInputValue,TOutputValue>
::~SVMMachineLearningModel()
{
  delete m_SVMModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
SVMMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  // Set up SVM's parameters
  CvSVMParams params;
  params.svm_type    = m_SVMType;
  params.kernel_type = m_KernelType;
  params.term_crit   = cvTermCriteria(m_TermCriteriaType, m_MaxIter, m_Epsilon);

  // Train the SVM
  m_SVMModel->train(samples, labels, cv::Mat(), cv::Mat(), params);
}

template <class TInputValue, class TOutputValue>
typename SVMMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
SVMMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);

  double result = m_SVMModel->predict(sample,false);

  TargetSampleType target;

  target[0] = static_cast<TOutputValue>(result);

  return target;
}

template <class TInputValue, class TOutputValue>
void
SVMMachineLearningModel<TInputValue,TOutputValue>
::Save(char * filename, const char * name)
{
  m_SVMModel->save(filename, name);
}

template <class TInputValue, class TOutputValue>
void
SVMMachineLearningModel<TInputValue,TOutputValue>
::Load(char * filename, const char * name)
{
  m_SVMModel->load(filename, name);
}

template <class TInputValue, class TOutputValue>
bool
SVMMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const char * file)
{
  return false;
}

template <class TInputValue, class TOutputValue>
bool
SVMMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const char * file)
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
SVMMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
