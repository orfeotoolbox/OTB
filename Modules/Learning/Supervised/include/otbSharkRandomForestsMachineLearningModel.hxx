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

#ifndef otbSharkRandomForestsMachineLearningModel_hxx
#define otbSharkRandomForestsMachineLearningModel_hxx

#include <fstream>
#include "itkMacro.h"
#include "otbSharkRandomForestsMachineLearningModel.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#endif
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif


#include "otbSharkUtils.h"
#include <algorithm>

namespace otb
{

template <class TInputValue, class TOutputValue>
SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::SharkRandomForestsMachineLearningModel()
{
  this->m_ConfidenceIndex               = true;
  this->m_ProbaIndex                    = true;
  this->m_IsRegressionSupported         = false;
  this->m_IsDoPredictBatchMultiThreaded = true;
  this->m_NormalizeClassLabels          = true;
  this->m_ComputeMargin                 = false;
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::Train()
{
#ifdef _OPENMP
  omp_set_num_threads(itk::MultiThreader::GetGlobalDefaultNumberOfThreads());
#endif

  std::vector<shark::RealVector> features;
  std::vector<unsigned int>      class_labels;

  Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
  Shark::ListSampleToSharkVector(this->GetTargetListSample(), class_labels);
  if (m_NormalizeClassLabels)
  {
    Shark::NormalizeLabelsAndGetDictionary(class_labels, m_ClassDictionary);
  }
  shark::ClassificationDataset TrainSamples = shark::createLabeledDataFromRange(features, class_labels);

  // Set parameters
  m_RFTrainer.setMTry(m_MTry);
  m_RFTrainer.setNTrees(m_NumberOfTrees);
  m_RFTrainer.setNodeSize(m_NodeSize);
  //  m_RFTrainer.setOOBratio(m_OobRatio);
  m_RFTrainer.train(m_RFModel, TrainSamples);
}

template <class TInputValue, class TOutputValue>
typename SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::ConfidenceValueType
SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::ComputeConfidence(shark::RealVector& probas, bool computeMargin) const
{
  assert(!probas.empty() && "probas vector is empty");
  assert((!computeMargin || probas.size() > 1) && "probas size should be at least 2 if computeMargin is true");

  ConfidenceValueType conf{0};
  if (computeMargin)
  {
    std::nth_element(probas.begin(), probas.begin() + 1, probas.end(), std::greater<double>());
    conf = static_cast<ConfidenceValueType>(probas[0] - probas[1]);
  }
  else
  {
    auto max_proba = *(std::max_element(probas.begin(), probas.end()));
    conf           = static_cast<ConfidenceValueType>(max_proba);
  }
  return conf;
}

template <class TInputValue, class TOutputValue>
typename SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::TargetSampleType
SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::DoPredict(const InputSampleType& value, ConfidenceValueType* quality,
                                                                             ProbaSampleType* proba) const
{
  shark::RealVector samples(value.Size());
  for (size_t i = 0; i < value.Size(); i++)
  {
    samples.push_back(value[i]);
  }
  if (quality != nullptr || proba != nullptr)
  {
    shark::RealVector probas = m_RFModel.decisionFunction()(samples);
    if (quality != nullptr)
    {
      (*quality) = ComputeConfidence(probas, m_ComputeMargin);
    }
    if (proba != nullptr)
    {
      for (size_t i = 0; i < probas.size(); i++)
      {
        // probas contain the N class probability indexed between 0 and N-1
        (*proba)[i] = static_cast<unsigned int>(probas[i] * 1000);
      }
    }
  }
  unsigned int res{0};
  m_RFModel.eval(samples, res);

  TargetSampleType target;
  if (m_NormalizeClassLabels)
  {
    target[0] = m_ClassDictionary[static_cast<TOutputValue>(res)];
  }
  else
  {
    target[0] = static_cast<TOutputValue>(res);
  }
  return target;
}

template <class TInputValue, class TOutputValue>
void SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::DoPredictBatch(const InputListSampleType* input, const unsigned int& startIndex,
                                                                                       const unsigned int& size, TargetListSampleType* targets,
                                                                                       ConfidenceListSampleType* quality, ProbaListSampleType* proba) const
{
  assert(input != nullptr);
  assert(targets != nullptr);

  assert(input->Size() == targets->Size() && "Input sample list and target label list do not have the same size.");
  assert(((quality == nullptr) || (quality->Size() == input->Size())) &&
         "Quality samples list is not null and does not have the same size as input samples list");
  assert(((proba == nullptr) || (input->Size() == proba->Size())) && "Proba sample list and target label list do not have the same size.");

  if (startIndex + size > input->Size())
  {
    itkExceptionMacro(<< "requested range [" << startIndex << ", " << startIndex + size << "[ partially outside input sample list range.[0," << input->Size()
                      << "[");
  }

  std::vector<shark::RealVector> features;
  Shark::ListSampleRangeToSharkVector(input, features, startIndex, size);
  shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange(features);

#ifdef _OPENMP
  omp_set_num_threads(itk::MultiThreader::GetGlobalDefaultNumberOfThreads());

#endif
  if (proba != nullptr || quality != nullptr)
  {
    shark::Data<shark::RealVector> probas = m_RFModel.decisionFunction()(inputSamples);
    if (proba != nullptr)
    {
      unsigned int id = startIndex;
      for (shark::RealVector&& p : probas.elements())
      {
        ProbaSampleType prob{(unsigned int)p.size()};
        for (size_t i = 0; i < p.size(); i++)
        {
          prob[i] = p[i] * 1000;
        }
        proba->SetMeasurementVector(id, prob);
        ++id;
      }
    }
    if (quality != nullptr)
    {
      unsigned int id = startIndex;
      for (shark::RealVector&& p : probas.elements())
      {
        ConfidenceSampleType confidence;
        auto                 conf = ComputeConfidence(p, m_ComputeMargin);
        confidence[0]             = static_cast<ConfidenceValueType>(conf);
        quality->SetMeasurementVector(id, confidence);
        ++id;
      }
    }
  }

  auto         prediction = m_RFModel(inputSamples);
  unsigned int id         = startIndex;
  for (const auto& p : prediction.elements())
  {
    TargetSampleType target;
    if (m_NormalizeClassLabels)
    {
      target[0] = m_ClassDictionary[static_cast<TOutputValue>(p)];
    }
    else
    {
      target[0] = static_cast<TOutputValue>(p);
    }
    targets->SetMeasurementVector(id, target);
    ++id;
  }
}

template <class TInputValue, class TOutputValue>
void SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::Save(const std::string& filename, const std::string& itkNotUsed(name))
{
  std::ofstream ofs(filename);
  if (!ofs)
  {
    itkExceptionMacro(<< "Error opening " << filename.c_str());
  }
  // Add comment with model file name
  ofs << "#" << m_RFModel.name();
  if (m_NormalizeClassLabels)
    ofs << " with_dictionary";
  ofs << std::endl;
  if (m_NormalizeClassLabels)
  {
    ofs << m_ClassDictionary.size() << " ";
    for (const auto& l : m_ClassDictionary)
    {
      ofs << l << " ";
    }
    ofs << std::endl;
  }
  shark::TextOutArchive oa(ofs);
  m_RFModel.save(oa, 0);
}

template <class TInputValue, class TOutputValue>
void SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::Load(const std::string& filename, const std::string& itkNotUsed(name))
{
  std::ifstream ifs(filename);
  if (ifs.good())
  {
    // Check if the first line is a comment and verify the name of the model in this case.
    std::string line;
    getline(ifs, line);
    if (line.at(0) == '#')
    {
      if (line.find(m_RFModel.name()) == std::string::npos)
        itkExceptionMacro("The model file : " + filename + " cannot be read.");
      if (line.find("with_dictionary") == std::string::npos)
      {
        m_NormalizeClassLabels = false;
      }
    }
    else
    {
      // rewind if first line is not a comment
      ifs.clear();
      ifs.seekg(0, std::ios::beg);
    }
    if (m_NormalizeClassLabels)
    {
      size_t nbLabels{0};
      ifs >> nbLabels;
      m_ClassDictionary.resize(nbLabels);
      for (size_t i = 0; i < nbLabels; ++i)
      {
        unsigned int label;
        ifs >> label;
        m_ClassDictionary[i] = label;
      }
    }
    shark::TextInArchive ia(ifs);
    m_RFModel.load(ia, 0);
  }
}

template <class TInputValue, class TOutputValue>
bool SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::CanReadFile(const std::string& file)
{
  try
  {
    this->Load(file);
    m_RFModel.name();
  }
  catch (...)
  {
    return false;
  }
  return true;
}

template <class TInputValue, class TOutputValue>
bool SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::CanWriteFile(const std::string& itkNotUsed(file))
{
  return true;
}

template <class TInputValue, class TOutputValue>
void SharkRandomForestsMachineLearningModel<TInputValue, TOutputValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
