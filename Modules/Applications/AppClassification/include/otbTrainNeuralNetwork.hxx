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

#ifndef otbTrainNeuralNetwork_hxx
#define otbTrainNeuralNetwork_hxx
#include <boost/lexical_cast.hpp>
#include "otbLearningApplicationBase.h"
#include "otbNeuralNetworkMachineLearningModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::InitNeuralNetworkParams()
{
  AddChoice("classifier.ann", "Artificial Neural Network classifier");
  SetParameterDescription("classifier.ann", "http://docs.opencv.org/modules/ml/doc/neural_networks.html");

  // TrainMethod
  AddParameter(ParameterType_Choice, "classifier.ann.t", "Train Method Type");
  AddChoice("classifier.ann.t.back", "Back-propagation algorithm");
  SetParameterDescription("classifier.ann.t.back",
                          "Method to compute the gradient of the loss function and adjust weights "
                          "in the network to optimize the result.");
  AddChoice("classifier.ann.t.reg", "Resilient Back-propagation algorithm");
  SetParameterDescription("classifier.ann.t.reg",
                          "Almost the same as the Back-prop algorithm except that it does not "
                          "take into account the magnitude of the partial derivative (coordinate "
                          "of the gradient) but only its sign.");

  SetParameterString("classifier.ann.t", "reg");
  SetParameterDescription("classifier.ann.t", "Type of training method for the multilayer perceptron (MLP) neural network.");

  // LayerSizes
  // There is no ParameterType_IntList, so i use a ParameterType_StringList and convert it.
  /*std::vector<std::string> layerSizes;
   layerSizes.push_back("100");
   layerSizes.push_back("100"); */
  AddParameter(ParameterType_StringList, "classifier.ann.sizes", "Number of neurons in each intermediate layer");
  // SetParameterStringList("classifier.ann.sizes", layerSizes);
  SetParameterDescription("classifier.ann.sizes", "The number of neurons in each intermediate layer (excluding input and output layers).");

  // ActivateFunction
  AddParameter(ParameterType_Choice, "classifier.ann.f", "Neuron activation function type");
  AddChoice("classifier.ann.f.ident", "Identity function");
  AddChoice("classifier.ann.f.sig", "Symmetrical Sigmoid function");
  AddChoice("classifier.ann.f.gau", "Gaussian function (Not completely supported)");
  SetParameterString("classifier.ann.f", "sig");
  SetParameterDescription("classifier.ann.f",
                          "This function determine whether the output of the node is positive or not "
                          "depending on the output of the transfert function.");

  // Alpha
  AddParameter(ParameterType_Float, "classifier.ann.a", "Alpha parameter of the activation function");
  SetParameterFloat("classifier.ann.a", 1.);
  SetParameterDescription("classifier.ann.a", "Alpha parameter of the activation function (used only with sigmoid and gaussian functions).");

  // Beta
  AddParameter(ParameterType_Float, "classifier.ann.b", "Beta parameter of the activation function");
  SetParameterFloat("classifier.ann.b", 1.);
  SetParameterDescription("classifier.ann.b", "Beta parameter of the activation function (used only with sigmoid and gaussian functions).");

  // BackPropDWScale
  AddParameter(ParameterType_Float, "classifier.ann.bpdw", "Strength of the weight gradient term in the BACKPROP method");
  SetParameterFloat("classifier.ann.bpdw", 0.1);
  SetParameterDescription("classifier.ann.bpdw",
                          "Strength of the weight gradient term in the BACKPROP method. The "
                          "recommended value is about 0.1.");

  // BackPropMomentScale
  AddParameter(ParameterType_Float, "classifier.ann.bpms", "Strength of the momentum term (the difference between weights on the 2 previous iterations)");
  SetParameterFloat("classifier.ann.bpms", 0.1);
  SetParameterDescription("classifier.ann.bpms",
                          "Strength of the momentum term (the difference between weights on the 2 previous "
                          "iterations). This parameter provides some inertia to smooth the random "
                          "fluctuations of the weights. It can vary from 0 (the feature is disabled) "
                          "to 1 and beyond. The value 0.1 or so is good enough.");

  // RegPropDW0
  AddParameter(ParameterType_Float, "classifier.ann.rdw", "Initial value Delta_0 of update-values Delta_{ij} in RPROP method");
  SetParameterFloat("classifier.ann.rdw", 0.1);
  SetParameterDescription("classifier.ann.rdw", "Initial value Delta_0 of update-values Delta_{ij} in RPROP method (default = 0.1).");

  // RegPropDWMin
  AddParameter(ParameterType_Float, "classifier.ann.rdwm", "Update-values lower limit Delta_{min} in RPROP method");
  SetParameterFloat("classifier.ann.rdwm", 1e-7);
  SetParameterDescription("classifier.ann.rdwm",
                          "Update-values lower limit Delta_{min} in RPROP method. It must be positive "
                          "(default = 1e-7).");

  // TermCriteriaType
  AddParameter(ParameterType_Choice, "classifier.ann.term", "Termination criteria");
  AddChoice("classifier.ann.term.iter", "Maximum number of iterations");
  SetParameterDescription("classifier.ann.term.iter",
                          "Set the number of iterations allowed to the network for its "
                          "training. Training will stop regardless of the result when this "
                          "number is reached");
  AddChoice("classifier.ann.term.eps", "Epsilon");
  SetParameterDescription("classifier.ann.term.eps",
                          "Training will focus on result and will stop once the precision is"
                          "at most epsilon");
  AddChoice("classifier.ann.term.all", "Max. iterations + Epsilon");
  SetParameterDescription("classifier.ann.term.all", "Both termination criteria are used. Training stop at the first reached");
  SetParameterString("classifier.ann.term", "all");
  SetParameterDescription("classifier.ann.term", "Termination criteria.");

  // Epsilon
  AddParameter(ParameterType_Float, "classifier.ann.eps", "Epsilon value used in the Termination criteria");
  SetParameterFloat("classifier.ann.eps", 0.01);
  SetParameterDescription("classifier.ann.eps", "Epsilon value used in the Termination criteria.");

  // MaxIter
  AddParameter(ParameterType_Int, "classifier.ann.iter", "Maximum number of iterations used in the Termination criteria");
  SetParameterInt("classifier.ann.iter", 1000);
  SetParameterDescription("classifier.ann.iter", "Maximum number of iterations used in the Termination criteria.");
}

template <class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::TrainNeuralNetwork(typename ListSampleType::Pointer trainingListSample,
                                                                            typename TargetListSampleType::Pointer trainingLabeledListSample,
                                                                            std::string                            modelPath)
{
  typedef otb::NeuralNetworkMachineLearningModel<InputValueType, OutputValueType> NeuralNetworkType;
  typename NeuralNetworkType::Pointer classifier = NeuralNetworkType::New();
  classifier->SetRegressionMode(this->m_RegressionFlag);
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);

  switch (GetParameterInt("classifier.ann.t"))
  {
  case 0: // BACKPROP
    classifier->SetTrainMethod(CvANN_MLP_TrainParams::BACKPROP);
    break;
  case 1: // RPROP
    classifier->SetTrainMethod(CvANN_MLP_TrainParams::RPROP);
    break;
  default: // DEFAULT = RPROP
    classifier->SetTrainMethod(CvANN_MLP_TrainParams::RPROP);
    break;
  }

  std::vector<unsigned int> layerSizes;
  std::vector<std::string>  sizes = GetParameterStringList("classifier.ann.sizes");


  unsigned int nbImageBands = trainingListSample->GetMeasurementVectorSize();
  layerSizes.push_back(nbImageBands);
  for (unsigned int i = 0; i < sizes.size(); i++)
  {
    unsigned int nbNeurons = boost::lexical_cast<unsigned int>(sizes[i]);
    layerSizes.push_back(nbNeurons);
  }


  unsigned int nbClasses = 0;
  if (this->m_RegressionFlag)
  {
    layerSizes.push_back(1);
  }
  else
  {
    std::set<TargetValueType> labelSet;
    TargetSampleType          currentLabel;
    for (unsigned int itLab = 0; itLab < trainingLabeledListSample->Size(); ++itLab)
    {
      currentLabel = trainingLabeledListSample->GetMeasurementVector(itLab);
      labelSet.insert(currentLabel[0]);
    }
    nbClasses = labelSet.size();
    layerSizes.push_back(nbClasses);
  }

  classifier->SetLayerSizes(layerSizes);

  switch (GetParameterInt("classifier.ann.f"))
  {
  case 0: // ident
    classifier->SetActivateFunction(CvANN_MLP::IDENTITY);
    break;
  case 1: // sig
    classifier->SetActivateFunction(CvANN_MLP::SIGMOID_SYM);
    break;
  case 2: // gaussian
    classifier->SetActivateFunction(CvANN_MLP::GAUSSIAN);
    break;
  default: // DEFAULT = RPROP
    classifier->SetActivateFunction(CvANN_MLP::SIGMOID_SYM);
    break;
  }

  classifier->SetAlpha(GetParameterFloat("classifier.ann.a"));
  classifier->SetBeta(GetParameterFloat("classifier.ann.b"));
  classifier->SetBackPropDWScale(GetParameterFloat("classifier.ann.bpdw"));
  classifier->SetBackPropMomentScale(GetParameterFloat("classifier.ann.bpms"));
  classifier->SetRegPropDW0(GetParameterFloat("classifier.ann.rdw"));
  classifier->SetRegPropDWMin(GetParameterFloat("classifier.ann.rdwm"));

  switch (GetParameterInt("classifier.ann.term"))
  {
  case 0: // CV_TERMCRIT_ITER
    classifier->SetTermCriteriaType(CV_TERMCRIT_ITER);
    break;
  case 1: // CV_TERMCRIT_EPS
    classifier->SetTermCriteriaType(CV_TERMCRIT_EPS);
    break;
  case 2: // CV_TERMCRIT_ITER + CV_TERMCRIT_EPS
    classifier->SetTermCriteriaType(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS);
    break;
  default: // DEFAULT = CV_TERMCRIT_ITER + CV_TERMCRIT_EPS
    classifier->SetTermCriteriaType(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS);
    break;
  }
  classifier->SetEpsilon(GetParameterFloat("classifier.ann.eps"));
  classifier->SetMaxIter(GetParameterInt("classifier.ann.iter"));
  classifier->Train();
  classifier->Save(modelPath);
}

} // end namespace wrapper
} // end namespace otb

#endif
