/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbNormalBayesMachineLearningModel_hxx
#define otbNormalBayesMachineLearningModel_hxx

#include <fstream>
#include "itkMacro.h"
#include "otbNormalBayesMachineLearningModel.h"
#include "otbOpenCVUtils.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
NormalBayesMachineLearningModel<TInputValue, TOutputValue>::NormalBayesMachineLearningModel()
  : m_NormalBayesModel(cv::ml::NormalBayesClassifier::create())
{
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void NormalBayesMachineLearningModel<TInputValue, TOutputValue>::Train()
{
  // convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(), labels);

  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U);
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL)); // all inputs are numerical
  var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  m_NormalBayesModel->train(cv::ml::TrainData::create(samples, cv::ml::ROW_SAMPLE, labels, cv::noArray(), cv::noArray(), cv::noArray(), var_type));
}

template <class TInputValue, class TOutputValue>
typename NormalBayesMachineLearningModel<TInputValue, TOutputValue>::TargetSampleType
NormalBayesMachineLearningModel<TInputValue, TOutputValue>::DoPredict(const InputSampleType& input, ConfidenceValueType* quality, ProbaSampleType* proba) const
{
  TargetSampleType target;

  // convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input, sample);

  cv::Mat missing = cv::Mat(1, input.Size(), CV_8U);
  missing.setTo(0);
  double result = m_NormalBayesModel->predict(sample);

  target[0] = static_cast<TOutputValue>(result);

  if (quality != nullptr)
  {
    if (!this->HasConfidenceIndex())
    {
      itkExceptionMacro("Confidence index not available for this classifier !");
    }
  }
  if (proba != nullptr && !this->m_ProbaIndex)
    itkExceptionMacro("Probability per class not available for this classifier !");

  return target;
}

template <class TInputValue, class TOutputValue>
void NormalBayesMachineLearningModel<TInputValue, TOutputValue>::Save(const std::string& filename, const std::string& name)
{
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  fs << (name.empty() ? m_NormalBayesModel->getDefaultName() : cv::String(name)) << "{";
  m_NormalBayesModel->write(fs);
  fs << "}";
  fs.release();
}

template <class TInputValue, class TOutputValue>
void NormalBayesMachineLearningModel<TInputValue, TOutputValue>::Load(const std::string& filename, const std::string& name)
{
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  m_NormalBayesModel->read(name.empty() ? fs.getFirstTopLevelNode() : fs[name]);
}

template <class TInputValue, class TOutputValue>
bool NormalBayesMachineLearningModel<TInputValue, TOutputValue>::CanReadFile(const std::string& file)
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

    if (line.find(CV_TYPE_NAME_ML_NBAYES) != std::string::npos || line.find(m_NormalBayesModel->getDefaultName()) != std::string::npos)
    {
      return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool NormalBayesMachineLearningModel<TInputValue, TOutputValue>::CanWriteFile(const std::string& itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void NormalBayesMachineLearningModel<TInputValue, TOutputValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
