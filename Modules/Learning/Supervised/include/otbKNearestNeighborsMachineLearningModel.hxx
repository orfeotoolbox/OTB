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

#ifndef otbKNearestNeighborsMachineLearningModel_hxx
#define otbKNearestNeighborsMachineLearningModel_hxx

#include "otb_boost_lexicalcast_header.h"
#include "otbKNearestNeighborsMachineLearningModel.h"
#include "otbOpenCVUtils.h"

#include <fstream>
#include <set>
#include "itkMacro.h"

namespace otb
{

template <class TInputValue, class TTargetValue>
KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::KNearestNeighborsMachineLearningModel()
  :
    m_KNearestModel(cv::ml::KNearest::create()),
    m_K(32),
    m_DecisionRule(KNN_VOTING)
{
  this->m_ConfidenceIndex       = true;
  this->m_IsRegressionSupported = true;
}


template <class TInputValue, class TTargetValue>
KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::~KNearestNeighborsMachineLearningModel()
{
}

/** Train the machine learning model */
template <class TInputValue, class TTargetValue>
void KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::Train()
{
  // convert listsample to opencv matrix
  cv::Mat samples;
  otb::ListSampleToMat<InputListSampleType>(this->GetInputListSample(), samples);

  cv::Mat labels;
  otb::ListSampleToMat<TargetListSampleType>(this->GetTargetListSample(), labels);

  // update decision rule if needed
  if (this->m_RegressionMode)
  {
    if (this->m_DecisionRule == KNN_VOTING)
    {
      this->SetDecisionRule(KNN_MEAN);
    }
  }
  else
  {
    if (this->m_DecisionRule != KNN_VOTING)
    {
      this->SetDecisionRule(KNN_VOTING);
    }
  }

  m_KNearestModel->setDefaultK(m_K);
  // would be nice to expose KDTree mode ( maybe in a different classifier)
  m_KNearestModel->setAlgorithmType(cv::ml::KNearest::BRUTE_FORCE);
  m_KNearestModel->setIsClassifier(!this->m_RegressionMode);
  // setEmax() ?
  m_KNearestModel->train(cv::ml::TrainData::create(samples, cv::ml::ROW_SAMPLE, labels));
}

template <class TInputValue, class TTargetValue>
typename KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::TargetSampleType
KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::DoPredict(const InputSampleType& input, ConfidenceValueType* quality,
                                                                            ProbaSampleType* proba) const
{
  TargetSampleType target;

  // convert listsample to Mat
  cv::Mat sample;
  otb::SampleToMat<InputSampleType>(input, sample);

  float   result;
  cv::Mat nearest(1, m_K, CV_32FC1);
  result = m_KNearestModel->findNearest(sample, m_K, cv::noArray(), nearest, cv::noArray());

  // compute quality if asked (only happens in classification mode)
  if (quality != nullptr)
  {
    assert(!this->m_RegressionMode);
    unsigned int accuracy = 0;
    for (int k = 0; k < m_K; ++k)
    {
      if (nearest.at<float>(0, k) == result)
      {
        accuracy++;
      }
    }
    (*quality) = static_cast<ConfidenceValueType>(accuracy);
  }
  if (proba != nullptr && !this->m_ProbaIndex)
    itkExceptionMacro("Probability per class not available for this classifier !");

  // Decision rule :
  //  VOTING is OpenCV default behaviour for classification
  //  MEAN is OpenCV default behaviour for regression
  //  MEDIAN : only case that must be handled here
  if (this->m_DecisionRule == KNN_MEDIAN)
  {
    std::multiset<float> values;
    for (int k = 0; k < m_K; ++k)
    {
      values.insert(nearest.at<float>(0, k));
    }
    std::multiset<float>::iterator median = values.begin();
    int                            pos    = (m_K >> 1);
    for (int k = 0; k < pos; ++k, ++median)
    {
    }
    result = *median;
  }

  target[0] = static_cast<TTargetValue>(result);
  return target;
}

template <class TInputValue, class TTargetValue>
void KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::Save(const std::string& filename, const std::string& name)
{
  cv::FileStorage fs(filename, cv::FileStorage::WRITE);
  fs << (name.empty() ? m_KNearestModel->getDefaultName() : cv::String(name)) << "{";
  m_KNearestModel->write(fs);
  fs << "DecisionRule" << m_DecisionRule;
  fs << "}";
  fs.release();
}

template <class TInputValue, class TTargetValue>
void KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::Load(const std::string& filename, const std::string& itkNotUsed(name))
{
  std::ifstream ifs(filename);
  if (!ifs)
  {
    itkExceptionMacro(<< "Could not read file " << filename);
  }
  // try to load with the 3.x syntax
  bool isKNNv3 = false;
  while (!ifs.eof())
  {
    std::string line;
    std::getline(ifs, line);
    if (line.find(m_KNearestModel->getDefaultName()) != std::string::npos)
    {
      isKNNv3 = true;
      break;
    }
  }
  ifs.close();
  if (isKNNv3)
  {
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    m_KNearestModel->read(fs.getFirstTopLevelNode());
    m_DecisionRule = (int)(fs.getFirstTopLevelNode()["DecisionRule"]);
    return;
  }
  ifs.open(filename);
  // there is no m_KNearestModel->load(filename.c_str(), name.c_str());

  // first line is the K parameter of this algorithm.
  std::string line;
  std::getline(ifs, line);
  std::istringstream iss(line);
  if (line.find("K") == std::string::npos)
  {
    itkExceptionMacro(<< "Could not read file " << filename);
  }
  std::string::size_type pos     = line.find_first_of("=", 0);
  std::string::size_type nextpos = line.find_first_of(" \n\r", pos + 1);
  this->SetK(boost::lexical_cast<int>(line.substr(pos + 1, nextpos - pos - 1)));

  // second line is the IsRegression parameter
  std::getline(ifs, line);
  if (line.find("IsRegression") == std::string::npos)
  {
    itkExceptionMacro(<< "Could not read file " << filename);
  }
  pos     = line.find_first_of("=", 0);
  nextpos = line.find_first_of(" \n\r", pos + 1);
  this->SetRegressionMode(boost::lexical_cast<bool>(line.substr(pos + 1, nextpos - pos - 1)));
  // third line is the DecisionRule parameter (only for regression)
  if (this->m_RegressionMode)
  {
    std::getline(ifs, line);
    pos     = line.find_first_of("=", 0);
    nextpos = line.find_first_of(" \n\r", pos + 1);
    this->SetDecisionRule(boost::lexical_cast<int>(line.substr(pos + 1, nextpos - pos - 1)));
  }
  // Clear previous listSample (if any)
  typename InputListSampleType::Pointer  samples = InputListSampleType::New();
  typename TargetListSampleType::Pointer labels  = TargetListSampleType::New();

  // Read a txt file. First column is the label, other columns are the sample data.
  unsigned int nbFeatures = 0;
  while (!ifs.eof())
  {
    std::getline(ifs, line);

    if (nbFeatures == 0)
    {
      nbFeatures = std::count(line.begin(), line.end(), ' ');
    }

    if (line.size() > 1)
    {
      // Parse label
      pos = line.find_first_of(" ", 0);
      TargetSampleType label;
      label[0] = static_cast<TargetValueType>(boost::lexical_cast<unsigned int>(line.substr(0, pos)));
      // Parse sample features
      InputSampleType sample(nbFeatures);
      sample.Fill(0);
      unsigned int id = 0;
      nextpos         = line.find_first_of(" ", pos + 1);
      while (nextpos != std::string::npos)
      {
        nextpos             = line.find_first_of(" \n\r", pos + 1);
        std::string subline = line.substr(pos + 1, nextpos - pos - 1);
        // sample[id] = static_cast<InputValueType>(boost::lexical_cast<float>(subline));
        sample[id] = atof(subline.c_str());
        pos        = nextpos;
        id++;
      }
      samples->SetMeasurementVectorSize(itk::NumericTraits<InputSampleType>::GetLength(sample));
      samples->PushBack(sample);
      labels->PushBack(label);
    }
  }
  ifs.close();

  this->SetInputListSample(samples);
  this->SetTargetListSample(labels);
  this->Train();
}

template <class TInputValue, class TTargetValue>
bool KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::CanReadFile(const std::string& file)
{
  try
  {
    this->Load(file);
  }
  catch (...)
  {
    return false;
  }
  return true;
}

template <class TInputValue, class TTargetValue>
bool KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::CanWriteFile(const std::string& itkNotUsed(file))
{
  return false;
}


template <class TInputValue, class TTargetValue>
void KNearestNeighborsMachineLearningModel<TInputValue, TTargetValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
