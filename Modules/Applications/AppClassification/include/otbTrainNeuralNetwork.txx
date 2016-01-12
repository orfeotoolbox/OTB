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
#ifndef __otbTrainNeuralNetwork_txx
#define __otbTrainNeuralNetwork_txx
#include <boost/lexical_cast.hpp>
#include "otbLearningApplicationBase.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::InitNeuralNetworkParams()
{
  AddChoice("classifier.ann", "Artificial Neural Network classifier");
  SetParameterDescription("classifier.ann",
                          "This group of parameters allows setting Artificial Neural Network classifier parameters. "
                          "See complete documentation here \\url{http://docs.opencv.org/modules/ml/doc/neural_networks.html}.");

  //TrainMethod
  AddParameter(ParameterType_Choice, "classifier.ann.t", "Train Method Type");
  AddChoice("classifier.ann.t.reg", "RPROP algorithm");
  AddChoice("classifier.ann.t.back", "Back-propagation algorithm");
  SetParameterString("classifier.ann.t", "reg");
  SetParameterDescription("classifier.ann.t", "Type of training method for the multilayer perceptron (MLP) neural network.");

  //LayerSizes
  //There is no ParameterType_IntList, so i use a ParameterType_StringList and convert it.
  /*std::vector<std::string> layerSizes;
   layerSizes.push_back("100");
   layerSizes.push_back("100"); */
  AddParameter(ParameterType_StringList, "classifier.ann.sizes", "Number of neurons in each intermediate layer");
  //SetParameterStringList("classifier.ann.sizes", layerSizes);
  SetParameterDescription("classifier.ann.sizes",
                          "The number of neurons in each intermediate layer (excluding input and output layers).");

  //ActivateFunction
  AddParameter(ParameterType_Choice, "classifier.ann.f", "Neuron activation function type");
  AddChoice("classifier.ann.f.ident", "Identity function");
  AddChoice("classifier.ann.f.sig", "Symmetrical Sigmoid function");
  AddChoice("classifier.ann.f.gau", "Gaussian function (Not completely supported)");
  SetParameterString("classifier.ann.f", "sig");
  SetParameterDescription("classifier.ann.f", "Neuron activation function.");

  //Alpha
  AddParameter(ParameterType_Float, "classifier.ann.a", "Alpha parameter of the activation function");
  SetParameterFloat("classifier.ann.a", 1.);
  SetParameterDescription("classifier.ann.a",
                          "Alpha parameter of the activation function (used only with sigmoid and gaussian functions).");

  //Beta
  AddParameter(ParameterType_Float, "classifier.ann.b", "Beta parameter of the activation function");
  SetParameterFloat("classifier.ann.b", 1.);
  SetParameterDescription("classifier.ann.b",
                          "Beta parameter of the activation function (used only with sigmoid and gaussian functions).");

  //BackPropDWScale
  AddParameter(ParameterType_Float, "classifier.ann.bpdw",
               "Strength of the weight gradient term in the BACKPROP method");
  SetParameterFloat("classifier.ann.bpdw", 0.1);
  SetParameterDescription(
      "classifier.ann.bpdw",
      "Strength of the weight gradient term in the BACKPROP method. The recommended value is about 0.1.");

  //BackPropMomentScale
  AddParameter(ParameterType_Float, "classifier.ann.bpms",
               "Strength of the momentum term (the difference between weights on the 2 previous iterations)");
  SetParameterFloat("classifier.ann.bpms", 0.1);
  SetParameterDescription(
      "classifier.ann.bpms",
      "Strength of the momentum term (the difference between weights on the 2 previous iterations). "
      "This parameter provides some inertia to smooth the random fluctuations of the weights. "
      "It can vary from 0 (the feature is disabled) to 1 and beyond. The value 0.1 or so is good enough.");

  //RegPropDW0
  AddParameter(ParameterType_Float, "classifier.ann.rdw",
               "Initial value Delta_0 of update-values Delta_{ij} in RPROP method");
  SetParameterFloat("classifier.ann.rdw", 0.1);
  SetParameterDescription("classifier.ann.rdw", "Initial value Delta_0 of update-values Delta_{ij} in RPROP method (default = 0.1).");

  //RegPropDWMin
  AddParameter(ParameterType_Float, "classifier.ann.rdwm", "Update-values lower limit Delta_{min} in RPROP method");
  SetParameterFloat("classifier.ann.rdwm", 1e-7);
  SetParameterDescription(
      "classifier.ann.rdwm",
      "Update-values lower limit Delta_{min} in RPROP method. It must be positive (default = 1e-7).");

  //TermCriteriaType
  AddParameter(ParameterType_Choice, "classifier.ann.term", "Termination criteria");
  AddChoice("classifier.ann.term.iter", "Maximum number of iterations");
  AddChoice("classifier.ann.term.eps", "Epsilon");
  AddChoice("classifier.ann.term.all", "Max. iterations + Epsilon");
  SetParameterString("classifier.ann.term", "all");
  SetParameterDescription("classifier.ann.term", "Termination criteria.");

  //Epsilon
  AddParameter(ParameterType_Float, "classifier.ann.eps", "Epsilon value used in the Termination criteria");
  SetParameterFloat("classifier.ann.eps", 0.01);
  SetParameterDescription("classifier.ann.eps", "Epsilon value used in the Termination criteria.");

  //MaxIter
  AddParameter(ParameterType_Int, "classifier.ann.iter",
               "Maximum number of iterations used in the Termination criteria");
  SetParameterInt("classifier.ann.iter", 1000);
  SetParameterDescription("classifier.ann.iter", "Maximum number of iterations used in the Termination criteria.");

}

template <class TInputValue, class TOutputValue>
void
LearningApplicationBase<TInputValue,TOutputValue>
::TrainNeuralNetwork(typename ListSampleType::Pointer trainingListSample,
                     typename TargetListSampleType::Pointer trainingLabeledListSample,
                     std::string modelPath)
{
  typename NeuralNetworkType::Pointer classifier = NeuralNetworkType::New();
  classifier->SetRegressionMode(this->m_RegressionFlag);
  classifier->SetInputListSample(trainingListSample);
  classifier->SetTargetListSample(trainingLabeledListSample);

  switch (GetParameterInt("classifier.ann.t"))
    {
    case 0: // RPROP
      classifier->SetTrainMethod(CvANN_MLP_TrainParams::RPROP);
      break;
    case 1: // BACKPROP
      classifier->SetTrainMethod(CvANN_MLP_TrainParams::BACKPROP);
      break;
    default: // DEFAULT = RPROP
      classifier->SetTrainMethod(CvANN_MLP_TrainParams::RPROP);
      break;
    }

  std::vector<unsigned int> layerSizes;
  std::vector<std::string> sizes = GetParameterStringList("classifier.ann.sizes");


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
    TargetSampleType currentLabel;
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

} //end namespace wrapper
} //end namespace otb

#endif
