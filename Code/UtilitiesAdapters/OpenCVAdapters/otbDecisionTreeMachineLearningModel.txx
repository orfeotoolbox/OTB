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
#ifndef __otbDecisionTreeMachineLearningModel_txx
#define __otbDecisionTreeMachineLearningModel_txx

#include "otbDecisionTreeMachineLearningModel.h"
#include "otbOpenCVUtils.h"

#include <opencv2/opencv.hpp>

namespace otb
{

template <class TInputValue, class TOutputValue>
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::DecisionTreeMachineLearningModel() :
 m_DTreeModel (new CvDTree),
 m_MaxDepth(INT_MAX),
 m_MinSampleCount(10),
 m_RegressionAccuracy(0.01),
 m_UseSurrogates(true),
 m_MaxCategories(10),
 m_CVFolds(10),
 m_Use1seRule(true),
 m_IsRegression(false),
 m_TruncatePrunedTree(true)
{
}


template <class TInputValue, class TOutputValue>
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::~DecisionTreeMachineLearningModel()
{
  delete m_DTreeModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  float * priors = m_Priors.empty() ? 0 : &m_Priors.front();

  CvDTreeParams params = CvDTreeParams(m_MaxDepth, m_MinSampleCount, m_RegressionAccuracy,
                                       m_UseSurrogates, m_MaxCategories, m_CVFolds, m_Use1seRule, m_TruncatePrunedTree, priors);

  //train the Decision Tree model
  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

  if (!m_IsRegression) //Classification
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  m_DTreeModel->train(samples,CV_ROW_SAMPLE,labels,cv::Mat(),cv::Mat(),var_type,cv::Mat(),params);
}

template <class TInputValue, class TOutputValue>
typename DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);

  double result = m_DTreeModel->predict(sample, cv::Mat(), false)->value;

  TargetSampleType target;

  target[0] = static_cast<TOutputValue>(result);

  return target;
}

template <class TInputValue, class TOutputValue>
void
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_DTreeModel->save(filename.c_str(), 0);
  else
    m_DTreeModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_DTreeModel->load(filename.c_str(), 0);
  else
    m_DTreeModel->load(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
bool
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
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
    if (line.find(CV_TYPE_NAME_ML_TREE) != std::string::npos)
    {
       //std::cout<<"Reading a "<<CV_TYPE_NAME_ML_TREE<<" model"<<std::endl;
       return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
