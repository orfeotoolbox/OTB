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
#ifndef otbAutoencoderModel_hxx
#define otbAutoencoderModel_hxx

#include "otbAutoencoderModel.h"
#include "otbMacro.h"

#include <fstream>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#endif
#include "otbSharkUtils.h"
// include train function
#include <shark/ObjectiveFunctions/ErrorFunction.h>
//~ #include <shark/ObjectiveFunctions/SparseAutoencoderError.h>//the error function performing the regularisation of the hidden neurons

#include <shark/Algorithms/GradientDescent/Rprop.h>    // the RProp optimization algorithm
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h> // squared loss used for regression
#include <shark/ObjectiveFunctions/Regularizer.h>      //L2 regulariziation
//~ #include <shark/Models/ImpulseNoiseModel.h> //noise source to corrupt the inputs

#include <shark/Algorithms/StoppingCriteria/MaxIterations.h> //A simple stopping criterion that stops after a fixed number of iterations
#include <shark/Algorithms/StoppingCriteria/TrainingProgress.h> //Stops when the algorithm seems to converge, Tracks the progress of the training error over a period of time

#include <shark/Algorithms/GradientDescent/Adam.h>
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

namespace otb
{

template <class TInputValue, class NeuronType>
AutoencoderModel<TInputValue, NeuronType>::AutoencoderModel()
{
  this->m_IsDoPredictBatchMultiThreaded = true;
  this->m_WriteLearningCurve            = false;
}

template <class TInputValue, class NeuronType>
AutoencoderModel<TInputValue, NeuronType>::~AutoencoderModel()
{
}

template <class TInputValue, class NeuronType>
void AutoencoderModel<TInputValue, NeuronType>::Train()
{
  std::vector<shark::RealVector> features;
  Shark::ListSampleToSharkVector(this->GetInputListSample(), features);
  shark::Data<shark::RealVector> inputSamples      = shark::createDataFromRange(features);
  shark::Data<shark::RealVector> inputSamples_copy = inputSamples;

  std::ofstream ofs;
  if (this->m_WriteLearningCurve == true)
  {
    ofs.open(m_LearningCurveFileName);
    ofs << "learning curve" << std::endl;
  }

  // Initialization of the feed forward neural network
  m_Encoder = ModelType();
  m_InLayers.clear();
  size_t previousShape = shark::dataDimension(inputSamples);
  for (unsigned int i = 0; i < m_NumberOfHiddenNeurons.Size(); ++i)
  {
    m_InLayers.push_back(LayerType(previousShape, m_NumberOfHiddenNeurons[i]));
    previousShape = m_NumberOfHiddenNeurons[i];
    m_Encoder.add(&(m_InLayers.back()), true);
  }
  for (unsigned int i = std::max(0, static_cast<int>(m_NumberOfHiddenNeurons.Size() - 1)); i > 0; --i)
  {
    m_InLayers.push_back(LayerType(previousShape, m_NumberOfHiddenNeurons[i - 1]));
    previousShape = m_NumberOfHiddenNeurons[i - 1];
  }
  m_OutLayer = OutLayerType(previousShape, shark::dataDimension(inputSamples));

  // Training of the autoencoders pairwise, starting from the first and last layers
  for (unsigned int i = 0; i < m_NumberOfHiddenNeurons.Size(); ++i)
  {
    if (m_Epsilon > 0)
    {
      shark::TrainingProgress<> criterion(5, m_Epsilon);
      // Shark doesn't allow to train a layer using a sparsity term AND a noisy input.
      if (m_Noise[i] != 0)
      {
        TrainOneLayer(criterion, i, inputSamples, ofs);
      }
      else
      {
        TrainOneSparseLayer(criterion, i, inputSamples, ofs);
      }
      criterion.reset();
    }
    else
    {
      shark::MaxIterations<> criterion(m_NumberOfIterations);
      // Shark doesn't allow to train a layer using a sparsity term AND a noisy input.
      if (m_Noise[i] != 0)
      {
        TrainOneLayer(criterion, i, inputSamples, ofs);
        otbMsgDevMacro(<< "m_Noise " << m_Noise[0]);
      }
      else
      {
        TrainOneSparseLayer(criterion, i, inputSamples, ofs);
      }
      criterion.reset();
    }
    // encode the samples with the last encoder trained
    inputSamples = m_InLayers[i](inputSamples);
  }
  if (m_NumberOfIterationsFineTuning > 0)
  {
    shark::MaxIterations<> criterion(m_NumberOfIterationsFineTuning);
    TrainNetwork(criterion, inputSamples_copy, ofs);
  }
  this->SetDimension(m_NumberOfHiddenNeurons[m_NumberOfHiddenNeurons.Size() - 1]);
}

template <class TInputValue, class NeuronType>
template <class T>
void AutoencoderModel<TInputValue, NeuronType>::TrainOneLayer(shark::AbstractStoppingCriterion<T>& criterion, unsigned int layer_index,
                                                              shark::Data<shark::RealVector>& samples, std::ostream& File)
{
  typedef shark::AbstractModel<shark::RealVector, shark::RealVector> BaseModelType;
  ModelType net;
  net.add(&(m_InLayers[layer_index]), true);
  net.add((layer_index ? (BaseModelType*)&(m_InLayers[m_NumberOfHiddenNeurons.Size() * 2 - 1 - layer_index]) : (BaseModelType*)&m_OutLayer), true);

  otbMsgDevMacro(<< "Noise " << m_Noise[layer_index]);
  std::size_t inputs = dataDimension(samples);
  initRandomUniform(net, -m_InitFactor * std::sqrt(1.0 / inputs), m_InitFactor * std::sqrt(1.0 / inputs));

  //~ shark::ImpulseNoiseModel noise(inputs,m_Noise[layer_index],1.0); //set an input pixel with probability m_Noise to 0
  //~ shark::ConcatenatedModel<shark::RealVector,shark::RealVector> model = noise>> net;
  shark::LabeledData<shark::RealVector, shark::RealVector> trainSet(samples, samples); // labels identical to inputs
  shark::SquaredLoss<shark::RealVector> loss;
  //~ shark::ErrorFunction error(trainSet, &model, &loss);
  shark::ErrorFunction<> error(trainSet, &net, &loss);

  shark::TwoNormRegularizer<> regularizer(error.numberOfVariables());
  error.setRegularizer(m_Regularization[layer_index], &regularizer);

  shark::Adam<> optimizer;
  error.init();
  optimizer.init(error);

  otbMsgDevMacro(<< "Error before training : " << optimizer.solution().value);
  if (this->m_WriteLearningCurve == true)
  {
    File << "end layer" << std::endl;
  }

  unsigned int i = 0;
  do
  {
    i++;
    optimizer.step(error);
    if (this->m_WriteLearningCurve == true)
    {
      File << optimizer.solution().value << std::endl;
    }
    otbMsgDevMacro(<< "Error after " << i << " iterations : " << optimizer.solution().value);
  } while (!criterion.stop(optimizer.solution()));

  net.setParameterVector(optimizer.solution().point);
}

template <class TInputValue, class NeuronType>
template <class T>
void AutoencoderModel<TInputValue, NeuronType>::TrainOneSparseLayer(shark::AbstractStoppingCriterion<T>& criterion, unsigned int layer_index,
                                                                    shark::Data<shark::RealVector>& samples, std::ostream& File)
{
  typedef shark::AbstractModel<shark::RealVector, shark::RealVector> BaseModelType;
  ModelType net;
  net.add(&(m_InLayers[layer_index]), true);
  net.add((layer_index ? (BaseModelType*)&(m_InLayers[m_NumberOfHiddenNeurons.Size() * 2 - 1 - layer_index]) : (BaseModelType*)&m_OutLayer), true);

  std::size_t inputs = dataDimension(samples);
  shark::initRandomUniform(net, -m_InitFactor * std::sqrt(1.0 / inputs), m_InitFactor * std::sqrt(1.0 / inputs));

  // Idea : set the initials value for the output weights higher than the input weights

  shark::LabeledData<shark::RealVector, shark::RealVector> trainSet(samples, samples); // labels identical to inputs
  shark::SquaredLoss<shark::RealVector> loss;
  //~ shark::SparseAutoencoderError error(trainSet,&net, &loss, m_Rho[layer_index], m_Beta[layer_index]);
  // SparseAutoencoderError doesn't exist anymore, for now use a plain ErrorFunction
  shark::ErrorFunction<> error(trainSet, &net, &loss);

  shark::TwoNormRegularizer<> regularizer(error.numberOfVariables());
  error.setRegularizer(m_Regularization[layer_index], &regularizer);
  shark::Adam<> optimizer;
  error.init();
  optimizer.init(error);

  otbMsgDevMacro(<< "Error before training : " << optimizer.solution().value);
  unsigned int i = 0;
  do
  {
    i++;
    optimizer.step(error);
    otbMsgDevMacro(<< "Error after " << i << " iterations : " << optimizer.solution().value);
    if (this->m_WriteLearningCurve == true)
    {
      File << optimizer.solution().value << std::endl;
    }
  } while (!criterion.stop(optimizer.solution()));
  if (this->m_WriteLearningCurve == true)
  {
    File << "end layer" << std::endl;
  }
  net.setParameterVector(optimizer.solution().point);
}

template <class TInputValue, class NeuronType>
template <class T>
void AutoencoderModel<TInputValue, NeuronType>::TrainNetwork(shark::AbstractStoppingCriterion<T>& criterion, shark::Data<shark::RealVector>& samples,
                                                             std::ostream& File)
{
  // create full network
  ModelType net;
  for (auto& layer : m_InLayers)
  {
    net.add(&layer, true);
  }
  net.add(&m_OutLayer, true);

  // labels identical to inputs
  shark::LabeledData<shark::RealVector, shark::RealVector> trainSet(samples, samples);
  shark::SquaredLoss<shark::RealVector> loss;

  shark::ErrorFunction<>      error(trainSet, &net, &loss);
  shark::TwoNormRegularizer<> regularizer(error.numberOfVariables());
  error.setRegularizer(m_Regularization[0], &regularizer);

  shark::Adam<> optimizer;
  error.init();
  optimizer.init(error);
  otbMsgDevMacro(<< "Error before training : " << optimizer.solution().value);
  unsigned int i = 0;
  while (!criterion.stop(optimizer.solution()))
  {
    i++;
    optimizer.step(error);
    otbMsgDevMacro(<< "Error after " << i << " iterations : " << optimizer.solution().value);
    if (this->m_WriteLearningCurve == true)
    {
      File << optimizer.solution().value << std::endl;
    }
  }
}

template <class TInputValue, class NeuronType>
bool AutoencoderModel<TInputValue, NeuronType>::CanReadFile(const std::string& filename)
{
  try
  {
    this->Load(filename);
  }
  catch (...)
  {
    return false;
  }
  return true;
}

template <class TInputValue, class NeuronType>
bool AutoencoderModel<TInputValue, NeuronType>::CanWriteFile(const std::string& /*filename*/)
{
  return true;
}

template <class TInputValue, class NeuronType>
void AutoencoderModel<TInputValue, NeuronType>::Save(const std::string& filename, const std::string& /*name*/)
{
  otbMsgDevMacro(<< "saving model ...");
  std::ofstream ofs(filename);
  ofs << "Autoencoder" << std::endl;           // the first line of the model file contains a key
  ofs << (m_InLayers.size() + 1) << std::endl; // second line is the number of encoders/decoders
  shark::TextOutArchive oa(ofs);
  for (const auto& layer : m_InLayers)
  {
    oa << layer;
  }
  oa << m_OutLayer;
  ofs.close();
}

template <class TInputValue, class NeuronType>
void AutoencoderModel<TInputValue, NeuronType>::Load(const std::string& filename, const std::string& /*name*/)
{
  std::ifstream ifs(filename);
  char          buffer[256];
  // check first line
  ifs.getline(buffer, 256);
  std::string bufferStr(buffer);
  if (bufferStr != "Autoencoder")
  {
    itkExceptionMacro(<< "Error opening " << filename.c_str());
  }
  // check second line
  ifs.getline(buffer, 256);
  int nbLevels = boost::lexical_cast<int>(buffer);
  if (nbLevels < 2 || nbLevels % 2 == 1)
  {
    itkExceptionMacro(<< "Unexpected number of levels : " << buffer);
  }
  m_InLayers.clear();
  m_Encoder = ModelType();
  shark::TextInArchive ia(ifs);
  for (int i = 0; (i + 1) < nbLevels; i++)
  {
    LayerType layer;
    ia >> layer;
    m_InLayers.push_back(layer);
  }
  ia >> m_OutLayer;
  ifs.close();

  for (int i = 0; i < nbLevels / 2; i++)
  {
    m_Encoder.add(&(m_InLayers[i]), true);
  }

  this->SetDimension(m_Encoder.outputShape()[0]);
}

template <class TInputValue, class NeuronType>
typename AutoencoderModel<TInputValue, NeuronType>::TargetSampleType
AutoencoderModel<TInputValue, NeuronType>::DoPredict(const InputSampleType& value, ConfidenceValueType* /*quality*/, ProbaSampleType* /*proba*/) const
{
  shark::RealVector samples(value.Size());
  for (size_t i = 0; i < value.Size(); i++)
  {
    samples[i] = value[i];
  }

  std::vector<shark::RealVector> features;
  features.push_back(samples);

  shark::Data<shark::RealVector> data = shark::createDataFromRange(features);

  // features layer for a network containing the encoder and decoder part
  data = m_Encoder(data);
  TargetSampleType target;
  target.SetSize(this->m_Dimension);

  for (unsigned int a = 0; a < this->m_Dimension; ++a)
  {
    target[a] = data.element(0)[a];
  }
  return target;
}

template <class TInputValue, class NeuronType>
void AutoencoderModel<TInputValue, NeuronType>::DoPredictBatch(const InputListSampleType* input, const unsigned int& startIndex, const unsigned int& size,
                                                               TargetListSampleType* targets, ConfidenceListSampleType* /*quality*/,
                                                               ProbaListSampleType* /*proba*/) const
{
  std::vector<shark::RealVector> features;
  Shark::ListSampleRangeToSharkVector(input, features, startIndex, size);
  shark::Data<shark::RealVector> data = shark::createDataFromRange(features);
  TargetSampleType               target;
  // features layer for a network containing the encoder and decoder part
  data = m_Encoder(data);

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
