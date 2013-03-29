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
#ifndef __otbKNearestNeighborsMachineLearningModel_txx
#define __otbKNearestNeighborsMachineLearningModel_txx

#include "otbKNearestNeighborsMachineLearningModel.h"
#include "otbOpenCVUtils.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::KNearestNeighborsMachineLearningModel() :
 m_K(10), m_IsRegression(false)
{
  m_KNearestModel = new CvKNearest;
}


template <class TInputValue, class TOutputValue>
KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::~KNearestNeighborsMachineLearningModel()
{
  delete m_KNearestModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  //train the KNN model
  m_KNearestModel->train(samples,labels,cv::Mat(),m_IsRegression, m_K,false);
}

template <class TInputValue, class TOutputValue>
typename KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;
  otb::SampleToMat<InputSampleType>(input,sample);

  double result = m_KNearestModel->find_nearest(sample,m_K);

  TargetSampleType target;

  target[0] = static_cast<TOutputValue>(result);

  return target;
}

template <class TInputValue, class TOutputValue>
void
KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  m_KNearestModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  m_KNearestModel->load(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
bool
KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const std::string & file)
{
  return false;
}

template <class TInputValue, class TOutputValue>
bool
KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & file)
{
  return false;
}


template <class TInputValue, class TOutputValue>
void
KNearestNeighborsMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
