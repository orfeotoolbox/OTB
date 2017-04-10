/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbTrainLibSVM_txx
#define otbTrainLibSVM_txx
#include "otbLearningApplicationBase.h"
#include "otbLibSVMMachineLearningModel.h"

namespace otb
{
namespace Wrapper
{

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::InitLibSVMParams()
  {
    AddChoice("classifier.libsvm", "LibSVM classifier");
    SetParameterDescription("classifier.libsvm", "This group of parameters allows setting SVM classifier parameters.");
    AddParameter(ParameterType_Choice, "classifier.libsvm.k", "SVM Kernel Type");
    AddChoice("classifier.libsvm.k.linear", "Linear");
    AddChoice("classifier.libsvm.k.rbf", "Gaussian radial basis function");
    AddChoice("classifier.libsvm.k.poly", "Polynomial");
    AddChoice("classifier.libsvm.k.sigmoid", "Sigmoid");
    SetParameterString("classifier.libsvm.k", "linear", false);
    SetParameterDescription("classifier.libsvm.k", "SVM Kernel Type.");
    AddParameter(ParameterType_Choice, "classifier.libsvm.m", "SVM Model Type");
    SetParameterDescription("classifier.libsvm.m", "Type of SVM formulation.");
    if (this->m_RegressionFlag)
      {
      AddChoice("classifier.libsvm.m.epssvr", "Epsilon Support Vector Regression");
      AddChoice("classifier.libsvm.m.nusvr", "Nu Support Vector Regression");
      SetParameterString("classifier.libsvm.m", "epssvr", false);
      }
    else
      {
      AddChoice("classifier.libsvm.m.csvc", "C support vector classification");
      AddChoice("classifier.libsvm.m.nusvc", "Nu support vector classification");
      AddChoice("classifier.libsvm.m.oneclass", "Distribution estimation (One Class SVM)");
      SetParameterString("classifier.libsvm.m", "csvc", false);
      }
    AddParameter(ParameterType_Float, "classifier.libsvm.c", "Cost parameter C");
    SetParameterFloat("classifier.libsvm.c",1.0, false);
    SetParameterDescription(
        "classifier.libsvm.c",
        "SVM models have a cost parameter C (1 by default) to control the trade-off between training errors and forcing rigid margins.");
    AddParameter(ParameterType_Empty, "classifier.libsvm.opt", "Parameters optimization");
    MandatoryOff("classifier.libsvm.opt");
    SetParameterDescription("classifier.libsvm.opt", "SVM parameters optimization flag.");
    AddParameter(ParameterType_Empty, "classifier.libsvm.prob", "Probability estimation");
    MandatoryOff("classifier.libsvm.prob");
    SetParameterDescription("classifier.libsvm.prob", "Probability estimation flag.");

    if (this->m_RegressionFlag)
      {
      AddParameter(ParameterType_Float, "classifier.libsvm.eps", "Epsilon");
      SetParameterFloat("classifier.libsvm.eps",1e-3, false);
      AddParameter(ParameterType_Float, "classifier.libsvm.nu", "Nu");
      SetParameterFloat("classifier.libsvm.nu",0.5, false);
      }
  }

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::TrainLibSVM(typename ListSampleType::Pointer trainingListSample,
                typename TargetListSampleType::Pointer trainingLabeledListSample,
                std::string modelPath)
  {
    typedef otb::LibSVMMachineLearningModel<InputValueType, OutputValueType> LibSVMType;
    typename LibSVMType::Pointer libSVMClassifier = LibSVMType::New();
    libSVMClassifier->SetRegressionMode(this->m_RegressionFlag);
    libSVMClassifier->SetInputListSample(trainingListSample);
    libSVMClassifier->SetTargetListSample(trainingLabeledListSample);
    //SVM Option
    //TODO : Add other options ?
    if (IsParameterEnabled("classifier.libsvm.opt"))
      {
      libSVMClassifier->SetParameterOptimization(true);
      }
    if (IsParameterEnabled("classifier.libsvm.prob"))
      {
      libSVMClassifier->SetDoProbabilityEstimates(true);
      }
    libSVMClassifier->SetC(GetParameterFloat("classifier.libsvm.c"));

    switch (GetParameterInt("classifier.libsvm.k"))
      {
      case 0: // LINEAR
        libSVMClassifier->SetKernelType(LINEAR);
        break;
      case 1: // RBF
        libSVMClassifier->SetKernelType(RBF);
        break;
      case 2: // POLY
        libSVMClassifier->SetKernelType(POLY);
        break;
      case 3: // SIGMOID
        libSVMClassifier->SetKernelType(SIGMOID);
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
      libSVMClassifier->SetNu(GetParameterFloat("classifier.libsvm.nu"));
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

} //end namespace wrapper
} //end namespace otb

#endif
