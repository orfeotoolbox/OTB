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
#ifndef __otbTrainLibSVM_txx
#define __otbTrainLibSVM_txx
#include "otbLearningApplicationBase.h"

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
    SetParameterString("classifier.libsvm.k", "linear");
    SetParameterDescription("classifier.libsvm.k", "SVM Kernel Type.");
    AddParameter(ParameterType_Choice, "classifier.libsvm.m", "SVM Model Type");
    SetParameterDescription("classifier.libsvm.m", "Type of SVM formulation.");
    if (this->m_RegressionFlag)
      {
      AddChoice("classifier.libsvm.m.epssvr", "Epsilon Support Vector Regression");
      AddChoice("classifier.libsvm.m.nusvr", "Nu Support Vector Regression");
      SetParameterString("classifier.libsvm.m", "epssvr");
      }
    else
      {
      AddChoice("classifier.libsvm.m.csvc", "C support vector classification");
      AddChoice("classifier.libsvm.m.nusvc", "Nu support vector classification");
      AddChoice("classifier.libsvm.m.oneclass", "Distribution estimation (One Class SVM)");
      SetParameterString("classifier.libsvm.m", "csvc");
      }
    AddParameter(ParameterType_Float, "classifier.libsvm.c", "Cost parameter C");
    SetParameterFloat("classifier.libsvm.c", 1.0);
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
      SetParameterFloat("classifier.libsvm.eps", 1e-3);
      AddParameter(ParameterType_Float, "classifier.libsvm.nu", "Nu");
      SetParameterFloat("classifier.libsvm.nu", 0.5);
      }
  }

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::TrainLibSVM(typename ListSampleType::Pointer trainingListSample,
                typename TargetListSampleType::Pointer trainingLabeledListSample,
                std::string modelPath)
  {
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
