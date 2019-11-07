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
#ifndef otbPCAModel_hxx
#define otbPCAModel_hxx

#include "otbPCAModel.h"

#include <fstream>
#include "itkMacro.h"
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
#include "otbSharkUtils.h"
// include train function
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/Algorithms/GradientDescent/Rprop.h>    // the RProp optimization algorithm
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h> // squared loss used for regression
#include <shark/ObjectiveFunctions/Regularizer.h>      //L2 regulariziation
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

namespace otb
{

template <class TInputValue>
PCAModel<TInputValue>::PCAModel()
{
  this->m_IsDoPredictBatchMultiThreaded = true;
  this->m_Dimension                     = 0;
}

template <class TInputValue>
PCAModel<TInputValue>::~PCAModel()
{
}

template <class TInputValue>
void PCAModel<TInputValue>::Train()
{
  std::vector<shark::RealVector> features;

  Shark::ListSampleToSharkVector(this->GetInputListSample(), features);

  shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange(features);
  m_PCA.setData(inputSamples);
  m_PCA.encoder(m_Encoder, this->m_Dimension);
  m_PCA.decoder(m_Decoder, this->m_Dimension);
}

template <class TInputValue>
bool PCAModel<TInputValue>::CanReadFile(const std::string& filename)
{
  try
  {
    this->Load(filename);
    m_Encoder.name();
  }
  catch (...)
  {
    return false;
  }
  return true;
}

template <class TInputValue>
bool PCAModel<TInputValue>::CanWriteFile(const std::string& /*filename*/)
{
  return true;
}

template <class TInputValue>
void PCAModel<TInputValue>::Save(const std::string& filename, const std::string& /*name*/)
{
  std::ofstream ofs(filename);
  ofs << "pca" << std::endl; // first line
  shark::TextOutArchive oa(ofs);
  m_Encoder.write(oa);
  ofs.close();

  if (this->m_WriteEigenvectors == true) // output the map vectors in a txt file
  {
    std::ofstream otxt(filename + ".txt");

    otxt << "Eigenvectors : " << m_PCA.eigenvectors() << std::endl;
    otxt << "Eigenvalues : " << m_PCA.eigenvalues() << std::endl;

    std::vector<shark::RealVector> features;

    shark::SquaredLoss<shark::RealVector> loss;
    Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
    shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange(features);
    otxt << "Reconstruction error : " << loss.eval(inputSamples, m_Decoder(m_Encoder(inputSamples))) << std::endl;
    otxt.close();
  }
}

template <class TInputValue>
void PCAModel<TInputValue>::Load(const std::string& filename, const std::string& /*name*/)
{
  std::ifstream ifs(filename);
  char          encoder[256];
  ifs.getline(encoder, 256);
  std::string encoderstr(encoder);

  if (encoderstr != "pca")
  {
    itkExceptionMacro(<< "Error opening " << filename.c_str());
  }
  shark::TextInArchive ia(ifs);
  m_Encoder.read(ia);
  ifs.close();
  if (this->m_Dimension == 0)
  {
    this->m_Dimension = m_Encoder.outputShape()[0];
  }

  auto eigenvectors = m_Encoder.matrix();
  eigenvectors.resize(this->m_Dimension, m_Encoder.inputShape()[0]);

  m_Encoder.setStructure(eigenvectors, m_Encoder.offset());
}

template <class TInputValue>
typename PCAModel<TInputValue>::TargetSampleType PCAModel<TInputValue>::DoPredict(const InputSampleType& value, ConfidenceValueType* /*quality*/,
                                                                                  ProbaSampleType* /*proba*/) const
{
  shark::RealVector samples(value.Size());
  for (size_t i = 0; i < value.Size(); i++)
  {
    samples[i] = value[i];
  }

  std::vector<shark::RealVector> features;
  features.push_back(samples);

  shark::Data<shark::RealVector> data = shark::createDataFromRange(features);

  data = m_Encoder(data);
  TargetSampleType target;
  target.SetSize(this->m_Dimension);

  for (unsigned int a = 0; a < this->m_Dimension; ++a)
  {
    target[a] = data.element(0)[a];
  }
  return target;
}

template <class TInputValue>
void PCAModel<TInputValue>::DoPredictBatch(const InputListSampleType* input, const unsigned int& startIndex, const unsigned int& size,
                                           TargetListSampleType* targets, ConfidenceListSampleType* /*quality*/, ProbaListSampleType* /*proba*/) const
{
  std::vector<shark::RealVector> features;
  Shark::ListSampleRangeToSharkVector(input, features, startIndex, size);
  shark::Data<shark::RealVector> data = shark::createDataFromRange(features);
  TargetSampleType               target;
  data            = m_Encoder(data);
  unsigned int id = startIndex;
  target.SetSize(this->m_Dimension);
  for (const auto& p : data.elements())
  {
    for (unsigned int a = 0; a < this->m_Dimension; ++a)
    {
      target[a] = p[a];
    }
    targets->SetMeasurementVector(id, target);
    ++id;
  }
}

} // namespace otb
#endif
