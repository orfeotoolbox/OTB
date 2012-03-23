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
#ifndef __otbRandomForestsMachineLearningModel_txx
#define __otbRandomForestsMachineLearningModel_txx

#include "otbRandomForestsMachineLearningModel.h"

namespace otb 
{

template <class TInputValue, class TOutputValue>
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::RandomForestsMachineLearningModel()
{
  m_RFModel = new CvRTrees;
}


template <class TInputValue, class TOutputValue>
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::~RandomForestsMachineLearningModel()
{
  delete m_RFModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix

  //train rf

  //convert opencv matrix to listsample
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Predict()
{
  //convert listsample to Mat
  cv::Mat test_sample;
  double result = m_RFModel->predict(test_sample, cv::Mat());
  //convert Mat to listsample
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Save()
{
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Load()
{
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
