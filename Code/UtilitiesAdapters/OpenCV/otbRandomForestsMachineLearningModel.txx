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
#include "otbOpenCVUtils.h"

namespace otb 
{

template <class TInputValue, class TOutputValue>
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::RandomForestsMachineLearningModel()
{
  m_RFModel = new CvRTrees;
  m_MaxDepth = 25;
  m_MinSampleCount = 5;
  m_RegressionAccuracy = 0;
  m_ComputeSurrogateSplit = false;
  m_MaxNumberOfCategories = 15;
  m_Priors = 0;
  m_CalculateVariableImportance = false;
  m_MaxNumberOfVariables = 4;
  m_MaxNumberOfTrees = 100;
  m_ForrestAccuracy = 0.01;
  m_TerminationCriteria = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
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
  cv::Mat cvInputSample = ListSampleToCvMat(m_InputListSample);
  cv::Mat training_classifications;
  //Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
  cv::Mat var_type;
  //train rf

  CvRTParams params = CvRTParams(m_MaxDepth, // max depth
                                       m_MinSampleCount, // min sample count
                                       m_RegressionAccuracy, // regression accuracy: N/A here
                                       m_ComputeSurrogateSplit, // compute surrogate split, no missing data
                                       m_MaxNumberOfCategories, // max number of categories (use sub-optimal algorithm for larger numbers)
                                       m_Priors, // the array of priors
                                       m_CalculateVariableImportance,  // calculate variable importance
                                       m_MaxNumberOfVariables,       // number of variables randomly selected at node and used to find the best split(s).
				       m_MaxNumberOfTrees,	 // max number of trees in the forest
                                       m_ForrestAccuracy,				// forrest accuracy
                                       m_TerminationCriteria // termination cirteria
                                      );

  rtree->train(cvInputSample, CV_ROW_SAMPLE, training_classifications,
                     Mat(), Mat(), var_type, Mat(), params);
  //convert opencv matrix to listsample
  this->SetTargetListSample(CvMatToListSample(cvInputSample));
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
