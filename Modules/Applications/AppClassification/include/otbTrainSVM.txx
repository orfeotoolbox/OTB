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
#ifndef __otbTrainSVM_txx
#define __otbTrainSVM_txx
#include "otbLearningApplicationBase.h"

namespace otb
{
namespace Wrapper
{

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::InitSVMParams()
  {
    AddChoice("classifier.svm", "SVM classifier (OpenCV)");
    SetParameterDescription("classifier.svm", "This group of parameters allows setting SVM classifier parameters. "
        "See complete documentation here \\url{http://docs.opencv.org/modules/ml/doc/support_vector_machines.html}.");
    AddParameter(ParameterType_Choice, "classifier.svm.m", "SVM Model Type");
    SetParameterDescription("classifier.svm.m", "Type of SVM formulation.");
    if (this->m_RegressionFlag)
      {
      AddChoice("classifier.svm.m.epssvr", "Epsilon Support Vector Regression");
      AddChoice("classifier.svm.m.nusvr", "Nu Support Vector Regression");
      SetParameterString("classifier.svm.m", "epssvr");
      }
    else
      {
      AddChoice("classifier.svm.m.csvc", "C support vector classification");
      AddChoice("classifier.svm.m.nusvc", "Nu support vector classification");
      AddChoice("classifier.svm.m.oneclass", "Distribution estimation (One Class SVM)");
      SetParameterString("classifier.svm.m", "csvc");
      }
    AddParameter(ParameterType_Choice, "classifier.svm.k", "SVM Kernel Type");
    AddChoice("classifier.svm.k.linear", "Linear");
    AddChoice("classifier.svm.k.rbf", "Gaussian radial basis function");
    AddChoice("classifier.svm.k.poly", "Polynomial");
    AddChoice("classifier.svm.k.sigmoid", "Sigmoid");
    SetParameterString("classifier.svm.k", "linear");
    SetParameterDescription("classifier.svm.k", "SVM Kernel Type.");
    AddParameter(ParameterType_Float, "classifier.svm.c", "Cost parameter C");
    SetParameterFloat("classifier.svm.c", 1.0);
    SetParameterDescription(
        "classifier.svm.c",
        "SVM models have a cost parameter C (1 by default) to control the trade-off between training errors and forcing rigid margins.");
    AddParameter(ParameterType_Float, "classifier.svm.nu",
                 "Parameter nu of a SVM optimization problem (NU_SVC / ONE_CLASS)");
    SetParameterFloat("classifier.svm.nu", 0.0);
    SetParameterDescription("classifier.svm.nu", "Parameter nu of a SVM optimization problem.");
    if (this->m_RegressionFlag)
      {
      AddParameter(ParameterType_Float, "classifier.svm.p", "Parameter epsilon of a SVM optimization problem (EPS_SVR)");
      SetParameterFloat("classifier.svm.p", 1.0);
      SetParameterDescription("classifier.svm.p", "Parameter epsilon of a SVM optimization problem (EPS_SVR).");

      AddParameter(ParameterType_Choice, "classifier.svm.term", "Termination criteria");
      SetParameterDescription("classifier.svm.term","Termination criteria for iterative algorithm");
      AddChoice("classifier.svm.term.iter", "Stops when maximum iteration is reached.");
      AddChoice("classifier.svm.term.eps", "Stops when accuracy is lower than epsilon.");
      AddChoice("classifier.svm.term.all", "Stops when either iteration or epsilon criteria is true");

      AddParameter(ParameterType_Float, "classifier.svm.iter", "Maximum iteration");
      SetParameterFloat("classifier.svm.iter", 1000);
      SetParameterDescription("classifier.svm.iter", "Maximum number of iterations (corresponds to the termination criteria 'iter').");

      AddParameter(ParameterType_Float, "classifier.svm.eps", "Epsilon accuracy threshold");
      SetParameterFloat("classifier.svm.eps", FLT_EPSILON);
      SetParameterDescription("classifier.svm.eps", "Epsilon accuracy (corresponds to the termination criteria 'eps').");
      }
    AddParameter(ParameterType_Float, "classifier.svm.coef0", "Parameter coef0 of a kernel function (POLY / SIGMOID)");
    SetParameterFloat("classifier.svm.coef0", 0.0);
    SetParameterDescription("classifier.svm.coef0", "Parameter coef0 of a kernel function (POLY / SIGMOID).");
    AddParameter(ParameterType_Float, "classifier.svm.gamma",
                 "Parameter gamma of a kernel function (POLY / RBF / SIGMOID)");
    SetParameterFloat("classifier.svm.gamma", 1.0);
    SetParameterDescription("classifier.svm.gamma", "Parameter gamma of a kernel function (POLY / RBF / SIGMOID).");
    AddParameter(ParameterType_Float, "classifier.svm.degree", "Parameter degree of a kernel function (POLY)");
    SetParameterFloat("classifier.svm.degree", 1.0);
    SetParameterDescription("classifier.svm.degree", "Parameter degree of a kernel function (POLY).");
    AddParameter(ParameterType_Empty, "classifier.svm.opt", "Parameters optimization");
    MandatoryOff("classifier.svm.opt");
    SetParameterDescription("classifier.svm.opt", "SVM parameters optimization flag.\n-If set to True, then the optimal SVM parameters will be estimated. "
                            "Parameters are considered optimal by OpenCV when the cross-validation estimate of the test set error is minimal. "
                            "Finally, the SVM training process is computed 10 times with these optimal parameters over subsets corresponding to 1/10th of "
                            "the training samples using the k-fold cross-validation (with k = 10).\n-If set to False, the SVM classification process will be "
                            "computed once with the currently set input SVM parameters over the training samples.\n-Thus, even with identical input SVM "
                            "parameters and a similar random seed, the output SVM models will be different according to the method used (optimized or not) "
                            "because the samples are not identically processed within OpenCV.");
  }

  template <class TInputValue, class TOutputValue>
  void
  LearningApplicationBase<TInputValue,TOutputValue>
  ::TrainSVM(typename ListSampleType::Pointer trainingListSample,
             typename TargetListSampleType::Pointer trainingLabeledListSample,
             std::string modelPath)
  {
    typename SVMType::Pointer SVMClassifier = SVMType::New();
    SVMClassifier->SetRegressionMode(this->m_RegressionFlag);
    SVMClassifier->SetInputListSample(trainingListSample);
    SVMClassifier->SetTargetListSample(trainingLabeledListSample);
    switch (GetParameterInt("classifier.svm.k"))
      {
      case 0: // LINEAR
        SVMClassifier->SetKernelType(CvSVM::LINEAR);
        std::cout << "CvSVM::LINEAR = " << CvSVM::LINEAR << std::endl;
        break;
      case 1: // RBF
        SVMClassifier->SetKernelType(CvSVM::RBF);
        std::cout << "CvSVM::RBF = " << CvSVM::RBF << std::endl;
        break;
      case 2: // POLY
        SVMClassifier->SetKernelType(CvSVM::POLY);
        std::cout << "CvSVM::POLY = " << CvSVM::POLY << std::endl;
        break;
      case 3: // SIGMOID
        SVMClassifier->SetKernelType(CvSVM::SIGMOID);
        std::cout << "CvSVM::SIGMOID = " << CvSVM::SIGMOID << std::endl;
        break;
      default: // DEFAULT = LINEAR
        SVMClassifier->SetKernelType(CvSVM::LINEAR);
        std::cout << "CvSVM::LINEAR = " << CvSVM::LINEAR << std::endl;
        break;
      }
    if (this->m_RegressionFlag)
      {
      switch (GetParameterInt("classifier.svm.m"))
        {
        case 0: // EPS_SVR
           SVMClassifier->SetSVMType(CvSVM::EPS_SVR);
           std::cout<<"CvSVM::EPS_SVR = "<<CvSVM::EPS_SVR<<std::endl;
           break;
        case 1: // NU_SVR
           SVMClassifier->SetSVMType(CvSVM::NU_SVR);
           std::cout<<"CvSVM::NU_SVR = "<<CvSVM::NU_SVR<<std::endl;
           break;
        default: // DEFAULT = EPS_SVR
          SVMClassifier->SetSVMType(CvSVM::EPS_SVR);
          std::cout << "CvSVM::EPS_SVR = " << CvSVM::EPS_SVR << std::endl;
          break;
        }
      }
    else
      {
      switch (GetParameterInt("classifier.svm.m"))
        {
        case 0: // C_SVC
          SVMClassifier->SetSVMType(CvSVM::C_SVC);
          std::cout << "CvSVM::C_SVC = " << CvSVM::C_SVC << std::endl;
          break;
        case 1: // NU_SVC
          SVMClassifier->SetSVMType(CvSVM::NU_SVC);
          std::cout << "CvSVM::NU_SVC = " << CvSVM::NU_SVC << std::endl;
          break;
        case 2: // ONE_CLASS
          SVMClassifier->SetSVMType(CvSVM::ONE_CLASS);
          std::cout << "CvSVM::ONE_CLASS = " << CvSVM::ONE_CLASS << std::endl;
          break;
        default: // DEFAULT = C_SVC
          SVMClassifier->SetSVMType(CvSVM::C_SVC);
          std::cout << "CvSVM::C_SVC = " << CvSVM::C_SVC << std::endl;
          break;
        }
      }
    SVMClassifier->SetC(GetParameterFloat("classifier.svm.c"));
    SVMClassifier->SetNu(GetParameterFloat("classifier.svm.nu"));
    if (this->m_RegressionFlag)
      {
      SVMClassifier->SetP(GetParameterFloat("classifier.svm.p"));
      switch (GetParameterInt("classifier.svm.term"))
        {
        case 0: // ITER
          SVMClassifier->SetTermCriteriaType(CV_TERMCRIT_ITER);
          break;
        case 1: // EPS
          SVMClassifier->SetTermCriteriaType(CV_TERMCRIT_EPS);
          break;
        case 2: // ITER+EPS
          SVMClassifier->SetTermCriteriaType(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS);
          break;
        default:
          SVMClassifier->SetTermCriteriaType(CV_TERMCRIT_ITER);
          break;
        }
      SVMClassifier->SetMaxIter(GetParameterInt("classifier.svm.iter"));
      SVMClassifier->SetEpsilon(GetParameterFloat("classifier.svm.eps"));
      }
    SVMClassifier->SetCoef0(GetParameterFloat("classifier.svm.coef0"));
    SVMClassifier->SetGamma(GetParameterFloat("classifier.svm.gamma"));
    SVMClassifier->SetDegree(GetParameterFloat("classifier.svm.degree"));
    if (IsParameterEnabled("classifier.svm.opt"))
    {
      SVMClassifier->SetParameterOptimization(true);
    }
    SVMClassifier->Train();
    SVMClassifier->Save(modelPath);

    // Update the displayed parameters in the GUI after the training process, for further use of them
    SetParameterFloat("classifier.svm.c", static_cast<float> (SVMClassifier->GetOutputC()));
    SetParameterFloat("classifier.svm.nu", static_cast<float> (SVMClassifier->GetOutputNu()));
    if (this->m_RegressionFlag)
      {
      SetParameterFloat("classifier.svm.p", static_cast<float> (SVMClassifier->GetOutputP()));
      }
    SetParameterFloat("classifier.svm.coef0", static_cast<float> (SVMClassifier->GetOutputCoef0()));
    SetParameterFloat("classifier.svm.gamma", static_cast<float> (SVMClassifier->GetOutputGamma()));
    SetParameterFloat("classifier.svm.degree", static_cast<float> (SVMClassifier->GetOutputDegree()));
  }

} //end namespace wrapper
} //end namespace otb

#endif
