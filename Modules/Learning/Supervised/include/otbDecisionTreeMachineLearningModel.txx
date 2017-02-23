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
#ifndef otbDecisionTreeMachineLearningModel_txx
#define otbDecisionTreeMachineLearningModel_txx

#include "otbDecisionTreeMachineLearningModel.h"
#include "otbOpenCVUtils.h"

#include <fstream>
#include "itkMacro.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::DecisionTreeMachineLearningModel() :
#ifdef OTB_OPENCV_3
 m_DTreeModel(cv::ml::DTrees::create()),
 m_MaxDepth(10),
 m_MinSampleCount(10),
 m_RegressionAccuracy(0.01),
 m_UseSurrogates(false),
 m_MaxCategories(10),
 m_CVFolds(0),
#else
 m_DTreeModel (new CvDTree),
 m_MaxDepth(INT_MAX),
 m_MinSampleCount(10),
 m_RegressionAccuracy(0.01),
 m_UseSurrogates(true),
 m_MaxCategories(10),
 m_CVFolds(10),
#endif
 m_Use1seRule(true),
 m_TruncatePrunedTree(true)
{
  this->m_IsRegressionSupported = true;
}

template <class TInputValue, class TOutputValue>
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::~DecisionTreeMachineLearningModel()
{
#ifndef OTB_OPENCV_3
  delete m_DTreeModel;
#endif
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

  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U );
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL) ); // all inputs are numerical

  if (!this->m_RegressionMode) //Classification
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

#ifdef OTB_OPENCV_3
  m_DTreeModel->setMaxDepth(m_MaxDepth);
  m_DTreeModel->setMinSampleCount(m_MinSampleCount);
  m_DTreeModel->setRegressionAccuracy(m_RegressionAccuracy);
  m_DTreeModel->setUseSurrogates(m_UseSurrogates);
  m_DTreeModel->setMaxCategories(m_MaxCategories);
  m_DTreeModel->setCVFolds(m_CVFolds);
  m_DTreeModel->setUse1SERule(m_Use1seRule);
  m_DTreeModel->setTruncatePrunedTree(m_TruncatePrunedTree);
  m_DTreeModel->setPriors(cv::Mat(m_Priors));
  m_DTreeModel->train(cv::ml::TrainData::create(
    samples,
    cv::ml::ROW_SAMPLE,
    labels,
    cv::noArray(),
    cv::noArray(),
    cv::noArray(),
    var_type));
#else
  float * priors = m_Priors.empty() ? ITK_NULLPTR : &m_Priors.front();

  CvDTreeParams params = CvDTreeParams(m_MaxDepth, m_MinSampleCount, m_RegressionAccuracy,
                                       m_UseSurrogates, m_MaxCategories, m_CVFolds, m_Use1seRule, m_TruncatePrunedTree, priors);

  //train the Decision Tree model
  m_DTreeModel->train(samples,CV_ROW_SAMPLE,labels,cv::Mat(),cv::Mat(),var_type,cv::Mat(),params);
#endif
}

template <class TInputValue, class TOutputValue>
typename DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::TargetSampleType
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::DoPredict(const InputSampleType & input, ConfidenceValueType *quality) const
{
  TargetSampleType target;

  //convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input,sample);
#ifdef OTB_OPENCV_3
  double result = m_DTreeModel->predict(sample);
#else
  double result = m_DTreeModel->predict(sample, cv::Mat(), false)->value;
#endif

  target[0] = static_cast<TOutputValue>(result);

  if (quality != ITK_NULLPTR)
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
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::Save(const std::string & filename, const std::string & name)
{
#ifdef OTB_OPENCV_3
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  fs << (name.empty() ? m_DTreeModel->getDefaultName() : cv::String(name)) << "{";
  m_DTreeModel->write(fs);
  fs << "}";
  fs.release();
#else
  if (name == "")
    m_DTreeModel->save(filename.c_str(), ITK_NULLPTR);
  else
    m_DTreeModel->save(filename.c_str(), name.c_str());
#endif
}

template <class TInputValue, class TOutputValue>
void
DecisionTreeMachineLearningModel<TInputValue,TOutputValue>
::Load(const std::string & filename, const std::string & name)
{
#ifdef OTB_OPENCV_3
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  m_DTreeModel->read(name.empty() ? fs.getFirstTopLevelNode() : fs[name]);
#else
  if (name == "")
    m_DTreeModel->load(filename.c_str(), ITK_NULLPTR);
  else
    m_DTreeModel->load(filename.c_str(), name.c_str());
#endif
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
    if (line.find(CV_TYPE_NAME_ML_TREE) != std::string::npos
#ifdef OTB_OPENCV_3
        || line.find(m_DTreeModel->getDefaultName()) != std::string::npos
#endif
      )
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
