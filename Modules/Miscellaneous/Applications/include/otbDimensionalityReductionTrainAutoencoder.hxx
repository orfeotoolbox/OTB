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
#ifndef otbDimensionalityReductionTrainAutoencoder_hxx
#define otbDimensionalityReductionTrainAutoencoder_hxx

#include "otbTrainDimensionalityReductionApplicationBase.h"
#include "otbAutoencoderModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::InitAutoencoderParams()
{
  AddChoice("algorithm.autoencoder", "Shark Autoencoder");
  SetParameterDescription("algorithm.autoencoder", "This group of parameters allows setting Shark autoencoder parameters. ");

  // Number Of Iterations
  AddParameter(ParameterType_Int, "algorithm.autoencoder.nbiter", "Maximum number of iterations during training");
  SetParameterInt("algorithm.autoencoder.nbiter", 100, false);
  SetParameterDescription("algorithm.autoencoder.nbiter", "The maximum number of iterations used during training.");

  AddParameter(ParameterType_Int, "algorithm.autoencoder.nbiterfinetuning", "Maximum number of iterations during training");
  SetParameterInt("algorithm.autoencoder.nbiterfinetuning", 0, false);
  SetParameterDescription("algorithm.autoencoder.nbiterfinetuning", "The maximum number of iterations used during fine tuning of the whole network.");

  AddParameter(ParameterType_Float, "algorithm.autoencoder.epsilon", "Epsilon");
  SetParameterFloat("algorithm.autoencoder.epsilon", 0, false);
  SetParameterDescription("algorithm.autoencoder.epsilon", "Epsilon");

  AddParameter(ParameterType_Float, "algorithm.autoencoder.initfactor", "Weight initialization factor");
  SetParameterFloat("algorithm.autoencoder.initfactor", 1, false);
  SetParameterDescription("algorithm.autoencoder.initfactor", "Parameter that control the weight initialization of the autoencoder");

  // Number Of Hidden Neurons
  AddParameter(ParameterType_StringList, "algorithm.autoencoder.nbneuron", "Size");
  SetParameterDescription("algorithm.autoencoder.nbneuron", "The number of neurons in each hidden layer.");

  // Regularization
  AddParameter(ParameterType_StringList, "algorithm.autoencoder.regularization", "Strength of the regularization");
  SetParameterDescription("algorithm.autoencoder.regularization", "Strength of the L2 regularization used during training");

  // Noise strength
  AddParameter(ParameterType_StringList, "algorithm.autoencoder.noise", "Strength of the noise");
  SetParameterDescription("algorithm.autoencoder.noise", "Strength of the noise");

  // Sparsity parameter
  AddParameter(ParameterType_StringList, "algorithm.autoencoder.rho", "Sparsity parameter");
  SetParameterDescription("algorithm.autoencoder.rho", "Sparsity parameter");

  // Sparsity regularization strength
  AddParameter(ParameterType_StringList, "algorithm.autoencoder.beta", "Sparsity regularization strength");
  SetParameterDescription("algorithm.autoencoder.beta", "Sparsity regularization strength");

  AddParameter(ParameterType_OutputFilename, "algorithm.autoencoder.learningcurve", "Learning curve");
  SetParameterDescription("algorithm.autoencoder.learningcurve", "Learning error values");
  MandatoryOff("algorithm.autoencoder.learningcurve");
}

template <class TInputValue, class TOutputValue>
void TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::BeforeTrainAutoencoder(typename ListSampleType::Pointer trainingListSample,
                                                                                                    std::string modelPath)
{
  typedef shark::LogisticNeuron NeuronType;
  typedef otb::AutoencoderModel<InputValueType, NeuronType> AutoencoderModelType;
  TrainAutoencoder<AutoencoderModelType>(trainingListSample, modelPath);
}

template <class TInputValue, class TOutputValue>
template <typename autoencoderchoice>
void TrainDimensionalityReductionApplicationBase<TInputValue, TOutputValue>::TrainAutoencoder(typename ListSampleType::Pointer trainingListSample,
                                                                                              std::string modelPath)
{
  typename autoencoderchoice::Pointer  dimredTrainer = autoencoderchoice::New();
  itk::Array<unsigned int>             nb_neuron;
  itk::Array<float>                    noise;
  itk::Array<float>                    regularization;
  itk::Array<float>                    rho;
  itk::Array<float>                    beta;
  std::vector<std::basic_string<char>> s_nbneuron       = GetParameterStringList("algorithm.autoencoder.nbneuron");
  std::vector<std::basic_string<char>> s_noise          = GetParameterStringList("algorithm.autoencoder.noise");
  std::vector<std::basic_string<char>> s_regularization = GetParameterStringList("algorithm.autoencoder.regularization");
  std::vector<std::basic_string<char>> s_rho            = GetParameterStringList("algorithm.autoencoder.rho");
  std::vector<std::basic_string<char>> s_beta           = GetParameterStringList("algorithm.autoencoder.beta");
  nb_neuron.SetSize(s_nbneuron.size());
  noise.SetSize(s_nbneuron.size());
  regularization.SetSize(s_nbneuron.size());
  rho.SetSize(s_nbneuron.size());
  beta.SetSize(s_nbneuron.size());
  for (unsigned int i = 0; i < s_nbneuron.size(); i++)
  {
    nb_neuron[i]      = std::stoi(s_nbneuron[i]);
    noise[i]          = std::stof(s_noise[i]);
    regularization[i] = std::stof(s_regularization[i]);
    rho[i]            = std::stof(s_rho[i]);
    beta[i]           = std::stof(s_beta[i]);
  }
  dimredTrainer->SetNumberOfHiddenNeurons(nb_neuron);
  dimredTrainer->SetNumberOfIterations(GetParameterInt("algorithm.autoencoder.nbiter"));
  dimredTrainer->SetNumberOfIterationsFineTuning(GetParameterInt("algorithm.autoencoder.nbiterfinetuning"));
  dimredTrainer->SetEpsilon(GetParameterFloat("algorithm.autoencoder.epsilon"));
  dimredTrainer->SetInitFactor(GetParameterFloat("algorithm.autoencoder.initfactor"));
  dimredTrainer->SetRegularization(regularization);
  dimredTrainer->SetNoise(noise);
  dimredTrainer->SetRho(rho);
  dimredTrainer->SetBeta(beta);
  dimredTrainer->SetWriteWeights(true);
  if (HasValue("algorithm.autoencoder.learningcurve") && IsParameterEnabled("algorithm.autoencoder.learningcurve"))
  {
    dimredTrainer->SetWriteLearningCurve(true);
    dimredTrainer->SetLearningCurveFileName(GetParameterString("algorithm.autoencoder.learningcurve"));
  }

  dimredTrainer->SetInputListSample(trainingListSample);
  dimredTrainer->Train();
  dimredTrainer->Save(modelPath);
}

} // end namespace wrapper
} // end namespace otb

#endif
