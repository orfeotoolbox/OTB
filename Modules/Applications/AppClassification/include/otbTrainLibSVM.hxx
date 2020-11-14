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

#ifndef otbTrainLibSVM_hxx
#define otbTrainLibSVM_hxx
#include "otbLearningApplicationBase.h"
#include "otbLibSVMMachineLearningModel.h"

namespace otb
{
namespace Wrapper
{

template <class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::InitLibSVMParams()
{
  AddChoice("classifier.libsvm", "LibSVM classifier");
  SetParameterDescription("classifier.libsvm", "This group of parameters allows setting SVM classifier parameters.");
  AddParameter(ParameterType_Choice, "classifier.libsvm.k", "SVM Kernel Type");
  AddChoice("classifier.libsvm.k.linear", "Linear");
  SetParameterDescription("classifier.libsvm.k.linear", "Linear Kernel, no mapping is done, this is the fastest option.");

  AddChoice("classifier.libsvm.k.rbf", "Gaussian radial basis function");
  SetParameterDescription("classifier.libsvm.k.rbf",
                          "This kernel is a good choice in most of the case. It is "
                          "an exponential function of the euclidian distance between "
                          "the vectors.");

  AddChoice("classifier.libsvm.k.poly", "Polynomial");
  SetParameterDescription("classifier.libsvm.k.poly", "Polynomial Kernel, the mapping is a polynomial function.");

  AddChoice("classifier.libsvm.k.sigmoid", "Sigmoid");
  SetParameterDescription("classifier.libsvm.k.sigmoid", "The kernel is a hyperbolic tangente function of the vectors.");

  SetParameterString("classifier.libsvm.k", "linear");
  SetParameterDescription("classifier.libsvm.k", "SVM Kernel Type.");
  AddParameter(ParameterType_Choice, "classifier.libsvm.m", "SVM Model Type");
  SetParameterDescription("classifier.libsvm.m", "Type of SVM formulation.");
  if (this->m_RegressionFlag)
  {
    AddChoice("classifier.libsvm.m.epssvr", "Epsilon Support Vector Regression");
    SetParameterDescription("classifier.libsvm.m.epssvr",
                            "The distance between feature vectors from the training set and the "
                            "fitting hyper-plane must be less than Epsilon. For outliers the penalty "
                            "multiplier C is used ");

    AddChoice("classifier.libsvm.m.nusvr", "Nu Support Vector Regression");
    SetParameterString("classifier.libsvm.m", "epssvr");
    SetParameterDescription("classifier.libsvm.m.nusvr",
                            "Same as the epsilon regression except that this time the bounded "
                            "parameter nu is used instead of epsilon");
  }
  else
  {
    AddChoice("classifier.libsvm.m.csvc", "C support vector classification");
    SetParameterDescription("classifier.libsvm.m.csvc",
                            "This formulation allows imperfect separation of classes. The penalty "
                            "is set through the cost parameter C.");

    AddChoice("classifier.libsvm.m.nusvc", "Nu support vector classification");
    SetParameterDescription("classifier.libsvm.m.nusvc",
                            "This formulation allows imperfect separation of classes. The penalty "
                            "is set through the cost parameter Nu. As compared to C, Nu is harder "
                            "to optimize, and may not be as fast.");

    AddChoice("classifier.libsvm.m.oneclass", "Distribution estimation (One Class SVM)");
    SetParameterDescription("classifier.libsvm.m.oneclass",
                            "All the training data are from the same class, SVM builds a boundary "
                            "that separates the class from the rest of the feature space.");
    SetParameterString("classifier.libsvm.m", "csvc");
  }

  AddParameter(ParameterType_Float, "classifier.libsvm.c", "Cost parameter C");
  SetParameterFloat("classifier.libsvm.c", 1.0);
  SetParameterDescription("classifier.libsvm.c",
                          "SVM models have a cost parameter C (1 by default) to control the "
                          "trade-off between training errors and forcing rigid margins.");

  AddParameter(ParameterType_Float, "classifier.libsvm.gamma", "Gamma parameter");
  SetParameterFloat("classifier.libsvm.gamma", 1.0);
  SetMinimumParameterFloatValue("classifier.libsvm.gamma", 0.0);
  SetParameterDescription("classifier.libsvm.gamma", "Set gamma parameter in poly/rbf/sigmoid kernel function");

  AddParameter(ParameterType_Float, "classifier.libsvm.coef0", "Coefficient parameter");
  SetParameterFloat("classifier.libsvm.coef0", 0.0);
  SetParameterDescription("classifier.libsvm.coef0", "Set coef0 parameter in poly/sigmoid kernel function");

  AddParameter(ParameterType_Int, "classifier.libsvm.degree", "Degree parameter");
  SetParameterInt("classifier.libsvm.degree", 3);
  SetMinimumParameterIntValue("classifier.libsvm.degree", 1);
  SetParameterDescription("classifier.libsvm.degree", "Set polynomial degree in poly kernel function");

  AddParameter(ParameterType_Float, "classifier.libsvm.nu", "Cost parameter Nu");
  SetParameterFloat("classifier.libsvm.nu", 0.5);
  SetParameterDescription("classifier.libsvm.nu",
                          "Cost parameter Nu, in the range 0..1, the larger the value, "
                          "the smoother the decision.");

  // It seems that it miss a nu parameter for the nu-SVM use.
  AddParameter(ParameterType_Bool, "classifier.libsvm.opt", "Parameters optimization");
  SetParameterDescription("classifier.libsvm.opt", "SVM parameters optimization flag.");

  AddParameter(ParameterType_Bool, "classifier.libsvm.prob", "Probability estimation");
  SetParameterDescription("classifier.libsvm.prob", "Probability estimation flag.");

  if (this->m_RegressionFlag)
  {
    AddParameter(ParameterType_Float, "classifier.libsvm.eps", "Epsilon");
    SetParameterFloat("classifier.libsvm.eps", 1e-3);
    SetParameterDescription("classifier.libsvm.eps",
                            "The distance between feature vectors from the training set and "
                            "the fitting hyper-plane must be less than Epsilon. For outliers"
                            "the penalty mutliplier is set by C.");
  }
}

template <class TInputValue, class TOutputValue>
void LearningApplicationBase<TInputValue, TOutputValue>::TrainLibSVM(typename ListSampleType::Pointer trainingListSample,
                                                                     typename TargetListSampleType::Pointer trainingLabeledListSample, std::string modelPath)
{
  typedef otb::LibSVMMachineLearningModel<InputValueType, OutputValueType> LibSVMType;
  typename LibSVMType::Pointer libSVMClassifier = LibSVMType::New();
  libSVMClassifier->SetRegressionMode(this->m_RegressionFlag);
  libSVMClassifier->SetInputListSample(trainingListSample);
  libSVMClassifier->SetTargetListSample(trainingLabeledListSample);
  // SVM Option
  // TODO : Add other options ?
  libSVMClassifier->SetParameterOptimization(GetParameterInt("classifier.libsvm.opt"));
  libSVMClassifier->SetDoProbabilityEstimates(GetParameterInt("classifier.libsvm.prob"));
  libSVMClassifier->SetNu(GetParameterFloat("classifier.libsvm.nu"));
  libSVMClassifier->SetC(GetParameterFloat("classifier.libsvm.c"));

  switch (GetParameterInt("classifier.libsvm.k"))
  {
  case 0: // LINEAR
    libSVMClassifier->SetKernelType(LINEAR);
    break;
  case 1: // RBF
    libSVMClassifier->SetKernelType(RBF);
    libSVMClassifier->SetKernelGamma(GetParameterFloat("classifier.libsvm.gamma"));
    break;
  case 2: // POLY
    libSVMClassifier->SetKernelType(POLY);
    libSVMClassifier->SetKernelGamma(GetParameterFloat("classifier.libsvm.gamma"));
    libSVMClassifier->SetKernelCoef0(GetParameterFloat("classifier.libsvm.coef0"));
    libSVMClassifier->SetPolynomialKernelDegree(GetParameterInt("classifier.libsvm.degree"));
    break;
  case 3: // SIGMOID
    libSVMClassifier->SetKernelType(SIGMOID);
    libSVMClassifier->SetKernelGamma(GetParameterFloat("classifier.libsvm.gamma"));
    libSVMClassifier->SetKernelCoef0(GetParameterFloat("classifier.libsvm.coef0"));
    break;
  default: // DEFAULT = LINEAR
    libSVMClassifier->SetKernelType(LINEAR);
    break;
  }
  if (this->m_RegressionFlag)
  {
    switch (GetParameterInt("classifier.libsvm.m"))
    {
    case 0: // EPSILON_SVR
      libSVMClassifier->SetSVMType(EPSILON_SVR);
      break;
    case 1: // NU_SVR
      libSVMClassifier->SetSVMType(NU_SVR);
      break;
    default:
      libSVMClassifier->SetSVMType(EPSILON_SVR);
      break;
    }
    libSVMClassifier->SetEpsilon(GetParameterFloat("classifier.libsvm.eps"));
  }
  else
  {
    switch (GetParameterInt("classifier.libsvm.m"))
    {
    case 0: // C_SVC
      libSVMClassifier->SetSVMType(C_SVC);
      break;
    case 1: // NU_SVC
      libSVMClassifier->SetSVMType(NU_SVC);
      break;
    case 2: // ONE_CLASS
      libSVMClassifier->SetSVMType(ONE_CLASS);
      break;
    default:
      libSVMClassifier->SetSVMType(C_SVC);
      break;
    }
  }


  libSVMClassifier->Train();
  libSVMClassifier->Save(modelPath);
}

} // end namespace wrapper
} // end namespace otb

#endif
