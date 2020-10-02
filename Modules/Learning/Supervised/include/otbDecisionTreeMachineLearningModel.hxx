/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbDecisionTreeMachineLearningModel_hxx
#define otbDecisionTreeMachineLearningModel_hxx

#include "otbDecisionTreeMachineLearningModel.h"
#include "otbOpenCVUtils.h"

#include <fstream>
#include "itkMacro.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
DecisionTreeMachineLearningModel<TInputValue, TOutputValue>::DecisionTreeMachineLearningModel()
  :
    m_DTreeModel(cv::ml::DTrees::create()),
    m_MaxDepth(10),
    m_MinSampleCount(10),
    m_RegressionAccuracy(0.01),
    m_UseSurrogates(false),
    m_MaxCategories(10),
    m_Use1seRule(true),
    m_TruncatePrunedTree(true)
{
  this->m_IsRegressionSupported = true;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void DecisionTreeMachineLearningModel<TInputValue, TOutputValue>::Train()
{
  // convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(), labels);

  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U);
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL)); // all inputs are numerical

  if (!this->m_RegressionMode) // Classification
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  m_DTreeModel->setMaxDepth(m_MaxDepth);
  m_DTreeModel->setMinSampleCount(m_MinSampleCount);
  m_DTreeModel->setRegressionAccuracy(m_RegressionAccuracy);
  m_DTreeModel->setUseSurrogates(m_UseSurrogates);
  // CvFold is not exposed because it crashes in OpenCV 3 and 4
  m_DTreeModel->setCVFolds(0);
  m_DTreeModel->setMaxCategories(m_MaxCategories);
  m_DTreeModel->setUse1SERule(m_Use1seRule);
  m_DTreeModel->setTruncatePrunedTree(m_TruncatePrunedTree);
  m_DTreeModel->setPriors(cv::Mat(m_Priors));
  m_DTreeModel->train(cv::ml::TrainData::create(samples, cv::ml::ROW_SAMPLE, labels, cv::noArray(), cv::noArray(), cv::noArray(), var_type));
}

template <class TInputValue, class TOutputValue>
typename DecisionTreeMachineLearningModel<TInputValue, TOutputValue>::TargetSampleType
DecisionTreeMachineLearningModel<TInputValue, TOutputValue>::DoPredict(const InputSampleType& input, ConfidenceValueType* quality, ProbaSampleType* proba) const
{
  TargetSampleType target;

  // convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input, sample);
  double result = m_DTreeModel->predict(sample);

  target[0] = static_cast<TOutputValue>(result);

  if (quality != nullptr)
  {
    if (!this->m_ConfidenceIndex)
    {
      itkExceptionMacro("Confidence index not available for this classifier !");
    }
  }
  if (proba != nullptr && !this->m_ProbaIndex)
    itkExceptionMacro("Probability per class not available for this classifier !");

  return target;
}

template <class TInputValue, class TOutputValue>
void DecisionTreeMachineLearningModel<TInputValue, TOutputValue>::Save(const std::string& filename, const std::string& name)
{
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  fs << (name.empty() ? m_DTreeModel->getDefaultName() : cv::String(name)) << "{";
  m_DTreeModel->write(fs);
  fs << "}";
  fs.release();
}

template <class TInputValue, class TOutputValue>
void DecisionTreeMachineLearningModel<TInputValue, TOutputValue>::Load(const std::string& filename, const std::string& name)
{
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  m_DTreeModel->read(name.empty() ? fs.getFirstTopLevelNode() : fs[name]);
}

template <class TInputValue, class TOutputValue>
bool DecisionTreeMachineLearningModel<TInputValue, TOutputValue>::CanReadFile(const std::string& file)
{
  std::ifstream ifs;
  ifs.open(file);

  if (!ifs)
  {
    std::cerr << "Could not read file " << file << std::endl;
    return false;
  }

  while (!ifs.eof())
  {
    std::string line;
    std::getline(ifs, line);

    // if (line.find(m_SVMModel->getName()) != std::string::npos)
    if (line.find(CV_TYPE_NAME_ML_TREE) != std::string::npos || line.find(m_DTreeModel->getDefaultName()) != std::string::npos)
    {
      return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool DecisionTreeMachineLearningModel<TInputValue, TOutputValue>::CanWriteFile(const std::string& itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void DecisionTreeMachineLearningModel<TInputValue, TOutputValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
