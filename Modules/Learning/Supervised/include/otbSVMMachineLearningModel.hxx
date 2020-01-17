/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSVMMachineLearningModel_hxx
#define otbSVMMachineLearningModel_hxx

#include <fstream>
#include "itkMacro.h"
#include "otbSVMMachineLearningModel.h"
#include "otbOpenCVUtils.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
SVMMachineLearningModel<TInputValue, TOutputValue>::SVMMachineLearningModel()
  :
    m_SVMModel(cv::ml::SVM::create()),
    m_SVMType(CvSVM::C_SVC),
    m_KernelType(CvSVM::RBF),
    m_Degree(0),
    m_Gamma(1),
    m_Coef0(0),
    m_C(1),
    m_Nu(0),
    m_P(0),
    m_TermCriteriaType(CV_TERMCRIT_ITER),
    m_MaxIter(1000),
    m_Epsilon(FLT_EPSILON),
    m_ParameterOptimization(false),
    m_OutputDegree(0),
    m_OutputGamma(1),
    m_OutputCoef0(0),
    m_OutputC(1),
    m_OutputNu(0),
    m_OutputP(0)
{
  this->m_ConfidenceIndex       = true;
  this->m_IsRegressionSupported = true;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void SVMMachineLearningModel<TInputValue, TOutputValue>::Train()
{
  // Check that the SVM type is compatible with the chosen mode (classif/regression)
  if (bool(m_SVMType == CvSVM::NU_SVR || m_SVMType == CvSVM::EPS_SVR) != this->m_RegressionMode)
  {
    itkGenericExceptionMacro(
        "SVM type incompatible with chosen mode (classification or regression."
        "SVM types for classification are C_SVC, NU_SVC, ONE_CLASS. "
        "SVM types for regression are NU_SVR, EPS_SVR");
  }

  // convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(), labels);

  cv::Mat var_type = cv::Mat(this->GetInputListSample()->GetMeasurementVectorSize() + 1, 1, CV_8U);
  var_type.setTo(cv::Scalar(CV_VAR_NUMERICAL)); // all inputs are numerical

  if (!this->m_RegressionMode) // Classification
    var_type.at<uchar>(this->GetInputListSample()->GetMeasurementVectorSize(), 0) = CV_VAR_CATEGORICAL;

  m_SVMModel->setType(m_SVMType);
  m_SVMModel->setKernel(m_KernelType);
  m_SVMModel->setDegree(m_Degree);
  m_SVMModel->setGamma(m_Gamma);
  m_SVMModel->setCoef0(m_Coef0);
  m_SVMModel->setC(m_C);
  m_SVMModel->setNu(m_Nu);
  m_SVMModel->setP(m_P);
  m_SVMModel->setTermCriteria(cv::TermCriteria(m_TermCriteriaType, m_MaxIter, m_Epsilon));

  if (!m_ParameterOptimization)
  {
    m_SVMModel->train(cv::ml::TrainData::create(samples, cv::ml::ROW_SAMPLE, labels, cv::noArray(), cv::noArray(), cv::noArray(), var_type));
  }
  else
  {
    m_SVMModel->trainAuto(cv::ml::TrainData::create(samples, cv::ml::ROW_SAMPLE, labels, cv::noArray(), cv::noArray(), cv::noArray(), var_type));
  }

  m_OutputDegree = m_SVMModel->getDegree();
  m_OutputGamma  = m_SVMModel->getGamma();
  m_OutputCoef0  = m_SVMModel->getCoef0();
  m_OutputC      = m_SVMModel->getC();
  m_OutputNu     = m_SVMModel->getNu();
  m_OutputP      = m_SVMModel->getP();
}

template <class TInputValue, class TOutputValue>
typename SVMMachineLearningModel<TInputValue, TOutputValue>::TargetSampleType
SVMMachineLearningModel<TInputValue, TOutputValue>::DoPredict(const InputSampleType& input, ConfidenceValueType* quality, ProbaSampleType* proba) const
{
  TargetSampleType target;
  // convert listsample to Mat
  cv::Mat sample;

  otb::SampleToMat<InputSampleType>(input, sample);

  double result = m_SVMModel->predict(sample);

  target[0] = static_cast<TOutputValue>(result);

  if (quality != nullptr)
  {
    (*quality) = m_SVMModel->predict(sample, cv::noArray(), cv::ml::StatModel::RAW_OUTPUT);
  }
  if (proba != nullptr && !this->m_ProbaIndex)
    itkExceptionMacro("Probability per class not available for this classifier !");

  return target;
}

template <class TInputValue, class TOutputValue>
void SVMMachineLearningModel<TInputValue, TOutputValue>::Save(const std::string& filename, const std::string& name)
{
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  fs << (name.empty() ? m_SVMModel->getDefaultName() : cv::String(name)) << "{";
  m_SVMModel->write(fs);
  fs << "}";
  fs.release();
}

template <class TInputValue, class TOutputValue>
void SVMMachineLearningModel<TInputValue, TOutputValue>::Load(const std::string& filename, const std::string& name)
{
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  m_SVMModel->read(name.empty() ? fs.getFirstTopLevelNode() : fs[name]);
}

template <class TInputValue, class TOutputValue>
bool SVMMachineLearningModel<TInputValue, TOutputValue>::CanReadFile(const std::string& file)
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
    if (line.find(CV_TYPE_NAME_ML_SVM) != std::string::npos || line.find(m_SVMModel->getDefaultName()) != std::string::npos)
    {
      return true;
    }
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool SVMMachineLearningModel<TInputValue, TOutputValue>::CanWriteFile(const std::string& itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void SVMMachineLearningModel<TInputValue, TOutputValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
