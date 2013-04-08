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
#ifndef __otbGradientBoostedTreeMachineLearningModel_txx
#define __otbGradientBoostedTreeMachineLearningModel_txx

#include "otbGradientBoostedTreeMachineLearningModel.h"
#include "otbOpenCVUtils.h"

#include <opencv2/opencv.hpp>

namespace otb
{

template <class TInputValue, class TOutputValue>
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::GradientBoostedTreeMachineLearningModel() :
 m_LossFunctionType(CvGBTrees::DEVIANCE_LOSS), m_WeakCount(200),
 m_Shrinkage(0.8), m_SubSamplePortion(0.01), m_MaxDepth(3),
 m_UseSurrogates(false), m_IsRegression(false)
{
  m_GBTreeModel = new CvGBTrees;
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

  if (!m_IsRegression) //Classification
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  m_GBTreeModel->train(samples,CV_ROW_SAMPLE,labels,cv::Mat(),cv::Mat(),var_type,cv::Mat(),params, false);
}

template <class TInputValue, class TOutputValue>
typename GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::Predict(const InputSampleType & input) const
{
  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);

  double result = m_GBTreeModel->predict(sample); //, cv::Mat(), false)->value;

  TargetSampleType target;

  target[0] = static_cast<TOutputValue>(result);

  return target;
}

template <class TInputValue, class TOutputValue>
void
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_GBTreeModel->save(filename.c_str(), 0);
  else
    m_GBTreeModel->save(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
void
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
  if (name == "")
    m_GBTreeModel->load(filename.c_str(), 0);
  else
    m_GBTreeModel->load(filename.c_str(), name.c_str());
}

template <class TInputValue, class TOutputValue>
bool
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
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
    if (line.find(CV_TYPE_NAME_ML_GBT) != std::string::npos)
    {
       std::cout<<"Reading a "<<CV_TYPE_NAME_ML_GBT<<" model !!!"<<std::endl;
       return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool
GradientBoostedTreeMachineLearningModel<TInputValue,TOutputValue>
::CanWriteFile(const std::string & file)
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
