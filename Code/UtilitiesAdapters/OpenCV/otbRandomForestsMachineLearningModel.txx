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
  m_CalculateVariableImportance = false;
  m_MaxNumberOfVariables = 4;
  m_MaxNumberOfTrees = 100;
  m_ForestAccuracy = 0.01;
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
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);
  //Mat var_type = Mat(ATTRIBUTES_PER_SAMPLE + 1, 1, CV_8U );
  //std::cout << "priors " << m_Priors[0] << std::endl;
  //Define random forests paramneters
  //FIXME do this in the constructor?

  float * priors = m_Priors.empty() ? 0 : &m_Priors.front();

  CvRTParams params = CvRTParams(m_MaxDepth, // max depth
                                       m_MinSampleCount, // min sample count
                                       m_RegressionAccuracy, // regression accuracy: N/A here
                                       m_ComputeSurrogateSplit, // compute surrogate split, no missing data
                                       m_MaxNumberOfCategories, // max number of categories (use sub-optimal algorithm for larger numbers)
                                       priors, // the array of priors
                                       m_CalculateVariableImportance,  // calculate variable importance
                                       m_MaxNumberOfVariables,       // number of variables randomly selected at node and used to find the best split(s).
				       m_MaxNumberOfTrees,	 // max number of trees in the forest
                                       m_ForestAccuracy,				// forrest accuracy
                                       m_TerminationCriteria // termination cirteria
                                      );

  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

  var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  //train the RT model
  m_RFModel->train(samples, CV_ROW_SAMPLE, labels,
	       cv::Mat(), cv::Mat(), var_type, cv::Mat(), params);
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Predict()
{
  //convert listsample to Mat
  cv::Mat sample;

  TargetListSampleType * targets = this->GetTargetListSample();
  targets->Clear();

  for(typename InputListSampleType::ConstIterator sIt = this->GetInputListSample()->Begin();
      sIt!=this->GetInputListSample()->End();++sIt)
    {
    otb::SampleToMat<InputSampleType>(sIt.GetMeasurementVector(),sample);

    double result = m_RFModel->predict(sample);

    TargetSampleType target;

    target[0] = static_cast<TOutputValue>(result);

    targets->PushBack(target);
    }
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Save(char * filename, const char * name)
{
  m_RFModel->save(filename, name);
}

template <class TInputValue, class TOutputValue>
void
RandomForestsMachineLearningModel<TInputValue,TOutputValue>
::Load(char * filename, const char * name)
{
  m_RFModel->load(filename, name);
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
