/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbGradientBoostedTreeMachineLearningModel_hxx
#define otbGradientBoostedTreeMachineLearningModel_hxx

#include "otbGradientBoostedTreeMachineLearningModel.h"
#include "otbOpenCVUtils.h"

#include <fstream>
#include "itkMacro.h"

#ifndef OTB_OPENCV_3
namespace otb
{

template <class TInputValue, class TOutputValue>
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::GradientBoostedTreeMachineLearningModel() :
 m_GBTreeModel (new CvGBTrees),
 m_LossFunctionType(CvGBTrees::DEVIANCE_LOSS),//m_LossFunctionType(CvGBTrees::SQUARED_LOSS),
 m_WeakCount(200),
 m_Shrinkage(0.01),
 m_SubSamplePortion(0.8),
 m_MaxDepth(3),
 m_UseSurrogates(false)
{
  this->m_IsRegressionSupported = true;
}


template <class TInputValue, class TOutputValue>
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::~GradientBoostedTreeMachineLearningModel()
{
  delete m_GBTreeModel;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::Train()
{
  //convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(),labels);

  CvGBTreesParams params = CvGBTreesParams(m_LossFunctionType, m_WeakCount, m_Shrinkage, m_SubSamplePortion,
                                           m_MaxDepth, m_UseSurrogates);

  //train the Decision Tree model
  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

  if (!this->m_RegressionMode) //Classification
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  m_GBTreeModel->train(samples,CV_ROW_SAMPLE,labels,cv::Mat(),cv::Mat(),var_type,cv::Mat(),params, false);
}

template <class TInputValue, class TOutputValue>
typename GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::DoPredict(const InputSampleType & input, ConfidenceValueType *quality) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);

  double result = m_GBTreeModel->predict(sample); //, cv::Mat(), false)->value;

  TargetSampleType target;

  target[0] = static_cast<TOutputValue>(result);

  if (quality != nullptr)
    {
    if (!this->m_ConfidenceIndex)
      {
      itkExceptionMacro("Confidence index not available for this classifier !");
      }
    }

  return target;
}

template <class TInputValue, class TOutputValue>
void
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_GBTreeModel->save(filename.c_str(), nullptr);
  else
    m_GBTreeModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_GBTreeModel->load(filename.c_str(), nullptr);
  else
    m_GBTreeModel->load(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
bool
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::CanReadFile(const std::string & file)
{
  std::ifstream ifs;
  ifs.open(file);

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
    if (line.find(CV_TYPE_NAME_ML_GBT) != std::string::npos)
    {
       //std::cout<<"Reading a "<<CV_TYPE_NAME_ML_GBT<<" model"<<std::endl;
       return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} //end namespace otb

#endif
#endif
