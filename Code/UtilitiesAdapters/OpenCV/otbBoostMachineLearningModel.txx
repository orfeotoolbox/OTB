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
#ifndef __otbBoostMachineLearningModel_txx
#define __otbBoostMachineLearningModel_txx

#include "otbBoostMachineLearningModel.h"
#include "otbOpenCVUtils.h"

namespace otb 
{

template <class TInputValue, class TOutputValue>
BoostMachineLearningModel<TInputValue,TOutputValue>
::BoostMachineLearningModel()
{
  m_BoostModel = new CvBoost;
}


template <class TInputValue, class TOutputValue>
BoostMachineLearningModel<TInputValue,TOutputValue>
::~BoostMachineLearningModel()
{
  delete m_BoostModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);
  
  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  CvBoostParams params;
  params.boost_type = CvBoost::DISCRETE;
  params.split_criteria = CvBoost::DEFAULT;

  //train the Boost model
  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

  var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;
  m_BoostModel->train(samples,CV_ROW_SAMPLE,labels,cv::Mat(),cv::Mat(),var_type,cv::Mat(),params);
}

template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
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
    
    cv::Mat missing  = cv::Mat(1,sIt.GetMeasurementVector().Size(), CV_8U );
    missing.setTo(0);
    double result = m_BoostModel->predict(sample,missing);

    TargetSampleType target;

    target[0] = static_cast<TOutputValue>(result);

    targets->PushBack(target);
    }
}

template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::Save(char * filename, const char * name)
{
  m_BoostModel->save(filename, name);
}

template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::Load(char * filename, const char * name)
{
  m_BoostModel->load(filename, name);
}

template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
