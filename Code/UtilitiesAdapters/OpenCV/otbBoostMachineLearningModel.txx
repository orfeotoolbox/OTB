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

#include <opencv2/opencv.hpp>

namespace otb
{

template <class TInputValue, class TOutputValue>
BoostMachineLearningModel<TInputValue,TOutputValue>
::BoostMachineLearningModel() :
 m_BoostType(CvBoost::REAL), m_SplitCrit(CvBoost::DEFAULT), m_WeakCount(100),
 m_WeightTrimRate(0.95), m_MaxDepth(1), m_IsRegression(false)
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

  CvBoostParams params = CvBoostParams(m_BoostType, m_WeakCount, m_WeightTrimRate, m_MaxDepth, false, 0);
  params.split_criteria = m_SplitCrit;

  //train the Boost model
  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

  if (!m_IsRegression) //Classification
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  m_BoostModel->train(samples,CV_ROW_SAMPLE,labels,cv::Mat(),cv::Mat(),var_type,cv::Mat(),params);
}

template <class TInputValue, class TOutputValue>
typename BoostMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
BoostMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);

  cv::Mat missing  = cv::Mat(1,input.Size(), CV_8U );
  missing.setTo(0);
  double result = m_BoostModel->predict(sample,missing);

  TargetSampleType target;

  target[0] = static_cast<TOutputValue>(result);

  return target;
}

template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_BoostModel->save(filename.c_str(), 0);
  else
    m_BoostModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
BoostMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  if (name == "")
      m_BoostModel->load(filename.c_str(), 0);
  else
      m_BoostModel->load(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
bool
BoostMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const std::string & file)
{
  std::ifstream ifs;
  ifs.open(file.c_str());

  if(!ifs)
  {
    std::cerr<<"Could not read file "<<file<<std::endl;
    return false;
  }

  while (!ifs.eof())
  {
    std::string line;
    std::getline(ifs, line);

    //if (line.find(m_SVMModel->getName()) != std::string::npos)
    if (line.find(CV_TYPE_NAME_ML_BOOSTING) != std::string::npos)
    {
       std::cout<<"Reading a "<<CV_TYPE_NAME_ML_BOOSTING<<" model !!!"<<std::endl;
       return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
BoostMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & file)
{
  return false;
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
