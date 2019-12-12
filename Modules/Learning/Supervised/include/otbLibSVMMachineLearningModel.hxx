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

#ifndef otbLibSVMMachineLearningModel_hxx
#define otbLibSVMMachineLearningModel_hxx

#include <fstream>
#include "otbLibSVMMachineLearningModel.h"
#include "otbSVMCrossValidationCostFunction.h"
#include "otbExhaustiveExponentialOptimizer.h"
#include "otbMacro.h"
#include "otbUtils.h"

namespace otb
{

template <class TInputValue, class TOutputValue>
LibSVMMachineLearningModel<TInputValue, TOutputValue>::LibSVMMachineLearningModel()
{
  this->SetSVMType(C_SVC);
  this->SetKernelType(LINEAR);
  this->SetPolynomialKernelDegree(3);
  this->SetKernelGamma(1.); // 1/k
  this->SetKernelCoef0(1.);
  this->SetNu(0.5);
  this->SetC(1.0);
  this->SetEpsilon(1e-3);
  this->SetP(0.1);
  this->SetDoProbabilityEstimates(false);
  this->DoShrinking(true);
  this->SetCacheSize(40); // MB
  this->m_ParameterOptimization = false;
  this->m_IsRegressionSupported = true;
  this->SetCVFolders(5);
  this->m_InitialCrossValidationAccuracy  = 0.;
  this->m_FinalCrossValidationAccuracy    = 0.;
  this->m_CoarseOptimizationNumberOfSteps = 5;
  this->m_FineOptimizationNumberOfSteps   = 5;
  this->m_ConfidenceMode                  = LibSVMMachineLearningModel<TInputValue, TOutputValue>::CM_INDEX;

  this->m_Parameters.nr_weight    = 0;
  this->m_Parameters.weight_label = nullptr;
  this->m_Parameters.weight       = nullptr;

  this->m_Model = nullptr;

  this->m_Problem.l = 0;
  this->m_Problem.y = nullptr;
  this->m_Problem.x = nullptr;
#ifdef NDEBUG
  svm_set_print_string_function(&otb::Utils::PrintNothing);
#endif
}

template <class TInputValue, class TOutputValue>
LibSVMMachineLearningModel<TInputValue, TOutputValue>::~LibSVMMachineLearningModel()
{
  this->DeleteModel();
  this->DeleteProblem();
}

/** Train the machine learning model */
template <class TInputValue, class TOutputValue>
void LibSVMMachineLearningModel<TInputValue, TOutputValue>::Train()
{
  this->DeleteProblem();
  this->DeleteModel();

  // Build problem
  this->BuildProblem();

  // Check consistency
  this->ConsistencyCheck();

  // Compute accuracy and eventually optimize parameters
  this->OptimizeParameters();

  // train the model
  m_Model = svm_train(&m_Problem, &m_Parameters);

  this->m_ConfidenceIndex = this->HasProbabilities();
}

template <class TInputValue, class TOutputValue>
typename LibSVMMachineLearningModel<TInputValue, TOutputValue>::TargetSampleType
LibSVMMachineLearningModel<TInputValue, TOutputValue>::DoPredict(const InputSampleType& input, ConfidenceValueType* quality, ProbaSampleType* proba) const
{
  TargetSampleType target;
  target.Fill(0);

  // Get type and number of classes
  int svm_type = svm_get_svm_type(m_Model);

  // Allocate nodes (/TODO if performances problems are related to too
  // many allocations, a cache approach can be set)
  struct svm_node* x = new struct svm_node[input.Size() + 1];

  // Fill the node
  for (unsigned int i = 0; i < input.Size(); i++)
  {
    x[i].index = i + 1;
    x[i].value = input[i];
  }

  // terminate node
  x[input.Size()].index = -1;
  x[input.Size()].value = 0;
  if (proba != nullptr && !this->m_ProbaIndex)
    itkExceptionMacro("Probability per class not available for this classifier !");

  if (quality != nullptr)
  {
    if (!this->m_ConfidenceIndex)
    {
      itkExceptionMacro("Confidence index not available for this classifier !");
    }
    if (this->m_ConfidenceMode == CM_INDEX)
    {
      if (svm_type == C_SVC || svm_type == NU_SVC)
      {
        // Eventually allocate space for probabilities
        unsigned int nr_class       = svm_get_nr_class(m_Model);
        double*      prob_estimates = new double[nr_class];
        // predict
        target[0]      = static_cast<TargetValueType>(svm_predict_probability(m_Model, x, prob_estimates));
        double maxProb = 0.0;
        double secProb = 0.0;
        for (unsigned int i = 0; i < nr_class; ++i)
        {
          if (maxProb < prob_estimates[i])
          {
            secProb = maxProb;
            maxProb = prob_estimates[i];
          }
          else if (secProb < prob_estimates[i])
          {
            secProb = prob_estimates[i];
          }
        }
        (*quality) = static_cast<ConfidenceValueType>(maxProb - secProb);

        delete[] prob_estimates;
      }
      else
      {
        target[0] = static_cast<TargetValueType>(svm_predict(m_Model, x));
        // Prob. model for test data: target value = predicted value + z
        // z: Laplace distribution e^(-|z|/sigma)/(2sigma)
        // sigma is output as confidence index
        (*quality) = svm_get_svr_probability(m_Model);
      }
    }
    else if (this->m_ConfidenceMode == CM_PROBA)
    {
      target[0] = static_cast<TargetValueType>(svm_predict_probability(m_Model, x, quality));
    }
    else if (this->m_ConfidenceMode == CM_HYPER)
    {
      target[0] = static_cast<TargetValueType>(svm_predict_values(m_Model, x, quality));
    }
  }
  else
  {
    // default case : if the model has probabilities, we call svm_predict_probabilities()
    // which gives different results than svm_predict()
    if (svm_check_probability_model(m_Model))
    {
      unsigned int nr_class       = svm_get_nr_class(m_Model);
      double*      prob_estimates = new double[nr_class];
      target[0]                   = static_cast<TargetValueType>(svm_predict_probability(m_Model, x, prob_estimates));
      delete[] prob_estimates;
    }
    else
    {
      target[0] = static_cast<TargetValueType>(svm_predict(m_Model, x));
    }
  }

  // Free allocated memory
  delete[] x;

  return target;
}

template <class TInputValue, class TOutputValue>
void LibSVMMachineLearningModel<TInputValue, TOutputValue>::Save(const std::string& filename, const std::string& itkNotUsed(name))
{
  if (svm_save_model(filename.c_str(), m_Model) != 0)
  {
    itkExceptionMacro(<< "Problem while saving SVM model " << filename);
  }
}

template <class TInputValue, class TOutputValue>
void LibSVMMachineLearningModel<TInputValue, TOutputValue>::Load(const std::string& filename, const std::string& itkNotUsed(name))
{
  this->DeleteModel();
  m_Model = svm_load_model(filename.c_str());
  if (m_Model == nullptr)
  {
    itkExceptionMacro(<< "Problem while loading SVM model " << filename);
  }
  m_Parameters = m_Model->param;

  this->m_ConfidenceIndex = this->HasProbabilities();
}

template <class TInputValue, class TOutputValue>
bool LibSVMMachineLearningModel<TInputValue, TOutputValue>::CanReadFile(const std::string& file)
{
  // TODO: Rework.
  std::ifstream ifs;
  ifs.open(file);

  if (!ifs)
  {
    std::cerr << "Could not read file " << file << std::endl;
    return false;
  }

  // Read only the first line.
  std::string line;
  std::getline(ifs, line);

  // if (line.find(m_SVMModel->getName()) != std::string::npos)
  if (line.find("svm_type") != std::string::npos)
  {
    // std::cout<<"Reading a libSVM model"<<std::endl;
    return true;
  }
  ifs.close();
  return false;
}

template <class TInputValue, class TOutputValue>
bool LibSVMMachineLearningModel<TInputValue, TOutputValue>::CanWriteFile(const std::string& itkNotUsed(file))
{
  return false;
}

template <class TInputValue, class TOutputValue>
void LibSVMMachineLearningModel<TInputValue, TOutputValue>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

template <class TInputValue, class TOutputValue>
bool LibSVMMachineLearningModel<TInputValue, TOutputValue>::HasProbabilities(void) const
{
  bool modelHasProba = static_cast<bool>(svm_check_probability_model(m_Model));
  int  type          = svm_get_svm_type(m_Model);
  int  cmMode        = this->m_ConfidenceMode;
  bool ret           = false;
  if (type == EPSILON_SVR || type == NU_SVR)
  {
    ret = (modelHasProba && cmMode == CM_INDEX);
  }
  else if (type == C_SVC || type == NU_SVC)
  {
    ret = (modelHasProba && (cmMode == CM_INDEX || cmMode == CM_PROBA)) || (cmMode == CM_HYPER);
  }
  return ret;
}

template <class TInputValue, class TOutputValue>
void LibSVMMachineLearningModel<TInputValue, TOutputValue>::BuildProblem()
{
  // Get number of samples
  typename InputListSampleType::Pointer  samples = this->GetInputListSample();
  typename TargetListSampleType::Pointer target  = this->GetTargetListSample();
  int                                    probl   = samples->Size();

  if (probl < 1)
  {
    itkExceptionMacro(<< "No samples, can not build SVM problem.");
  }
  otbMsgDebugMacro(<< "Building problem ...");

  // Get the size of the samples
  long int elements = samples->GetMeasurementVectorSize();

  // Allocate the problem
  m_Problem.l = probl;
  m_Problem.y = new double[probl];
  m_Problem.x = new struct svm_node*[probl];
  for (int i = 0; i < probl; ++i)
  {
    m_Problem.x[i] = new struct svm_node[elements + 1];
  }

  // Iterate on the samples
  typename InputListSampleType::ConstIterator  sIt         = samples->Begin();
  typename TargetListSampleType::ConstIterator tIt         = target->Begin();
  int                                          sampleIndex = 0;

  while (sIt != samples->End() && tIt != target->End())
  {
    // Set the label
    m_Problem.y[sampleIndex]      = tIt.GetMeasurementVector()[0];
    const InputSampleType& sample = sIt.GetMeasurementVector();
    for (int k = 0; k < elements; ++k)
    {
      m_Problem.x[sampleIndex][k].index = k + 1;
      m_Problem.x[sampleIndex][k].value = sample[k];
    }
    // terminate node
    m_Problem.x[sampleIndex][elements].index = -1;
    m_Problem.x[sampleIndex][elements].value = 0;

    ++sampleIndex;
    ++sIt;
    ++tIt;
  }

  // Compute the kernel gamma from number of elements if necessary
  if (this->GetKernelGamma() == 0)
  {
    this->SetKernelGamma(1.0 / static_cast<double>(elements));
  }

  // allocate buffer for cross validation
  m_TmpTarget.resize(m_Problem.l);
}

template <class TInputValue, class TOutputValue>
void LibSVMMachineLearningModel<TInputValue, TOutputValue>::ConsistencyCheck()
{
  if (this->GetSVMType() == ONE_CLASS && this->GetDoProbabilityEstimates())
  {
    otbMsgDebugMacro(<< "Disabling SVM probability estimates for ONE_CLASS SVM type.");
    this->SetDoProbabilityEstimates(false);
  }

  const char* error_msg = svm_check_parameter(&m_Problem, &m_Parameters);

  if (error_msg)
  {
    std::string err(error_msg);
    itkExceptionMacro("SVM parameter check failed : " << err);
  }
}

template <class TInputValue, class TOutputValue>
void LibSVMMachineLearningModel<TInputValue, TOutputValue>::DeleteProblem()
{
  if (m_Problem.y)
  {
    delete[] m_Problem.y;
    m_Problem.y = nullptr;
  }
  if (m_Problem.x)
  {
    for (int i = 0; i < m_Problem.l; ++i)
    {
      if (m_Problem.x[i])
      {
        delete[] m_Problem.x[i];
      }
    }
    delete[] m_Problem.x;
    m_Problem.x = nullptr;
  }
  m_Problem.l = 0;
}

template <class TInputValue, class TOutputValue>
void LibSVMMachineLearningModel<TInputValue, TOutputValue>::DeleteModel(void)
{
  if (m_Model)
  {
    svm_free_and_destroy_model(&m_Model);
  }
  m_Model = nullptr;
}

template <class TInputValue, class TOutputValue>
unsigned int LibSVMMachineLearningModel<TInputValue, TOutputValue>::GetNumberOfKernelParameters()
{
  unsigned int nb = 1;
  switch (this->GetKernelType())
  {
  case LINEAR:
    // C
    nb = 1;
    break;
  case POLY:
    // C, gamma and coef0
    nb = 3;
    break;
  case RBF:
    // C and gamma
    nb = 2;
    break;
  case SIGMOID:
    // C, gamma and coef0
    nb = 3;
    break;
  default:
    // C
    nb = 1;
    break;
  }
  return nb;
}

template <class TInputValue, class TOutputValue>
double LibSVMMachineLearningModel<TInputValue, TOutputValue>::CrossValidation(void)
{
  double accuracy = 0.0;
  // Get the length of the problem
  unsigned int length = m_Problem.l;
  if (length == 0 || m_TmpTarget.size() < length)
    return accuracy;

  // Do cross validation
  svm_cross_validation(&m_Problem, &m_Parameters, m_CVFolders, &m_TmpTarget[0]);

  // Evaluate accuracy
  double total_correct = 0.;
  for (unsigned int i = 0; i < length; ++i)
  {
    if (m_TmpTarget[i] == m_Problem.y[i])
    {
      ++total_correct;
    }
  }
  accuracy = total_correct / length;

  // return accuracy value
  return accuracy;
}

template <class TInputValue, class TOutputValue>
void LibSVMMachineLearningModel<TInputValue, TOutputValue>::OptimizeParameters()
{
  typedef SVMCrossValidationCostFunction<LibSVMMachineLearningModel<TInputValue, TOutputValue>> CrossValidationFunctionType;
  typename CrossValidationFunctionType::Pointer crossValidationFunction = CrossValidationFunctionType::New();
  crossValidationFunction->SetModel(this);

  typename CrossValidationFunctionType::ParametersType initialParameters, coarseBestParameters, fineBestParameters;

  unsigned int nbParams = this->GetNumberOfKernelParameters();
  initialParameters.SetSize(nbParams);
  initialParameters[0] = this->GetC();
  if (nbParams > 1)
    initialParameters[1] = this->GetKernelGamma();
  if (nbParams > 2)
    initialParameters[2] = this->GetKernelCoef0();

  m_InitialCrossValidationAccuracy = crossValidationFunction->GetValue(initialParameters);
  m_FinalCrossValidationAccuracy   = m_InitialCrossValidationAccuracy;

  otbMsgDebugMacro(<< "Initial accuracy : " << m_InitialCrossValidationAccuracy << ", Parameters Optimization" << m_ParameterOptimization);

  if (m_ParameterOptimization)
  {
    otbMsgDebugMacro(<< "Model parameters optimization");
    typename ExhaustiveExponentialOptimizer::Pointer   coarseOptimizer = ExhaustiveExponentialOptimizer::New();
    typename ExhaustiveExponentialOptimizer::StepsType coarseNbSteps(initialParameters.Size());
    coarseNbSteps.Fill(m_CoarseOptimizationNumberOfSteps);

    coarseOptimizer->SetNumberOfSteps(coarseNbSteps);
    coarseOptimizer->SetCostFunction(crossValidationFunction);
    coarseOptimizer->SetInitialPosition(initialParameters);
    coarseOptimizer->StartOptimization();

    coarseBestParameters = coarseOptimizer->GetMaximumMetricValuePosition();

    otbMsgDevMacro(<< "Coarse minimum accuracy: " << coarseOptimizer->GetMinimumMetricValue() << " " << coarseOptimizer->GetMinimumMetricValuePosition());
    otbMsgDevMacro(<< "Coarse maximum accuracy: " << coarseOptimizer->GetMaximumMetricValue() << " " << coarseOptimizer->GetMaximumMetricValuePosition());

    typename ExhaustiveExponentialOptimizer::Pointer   fineOptimizer = ExhaustiveExponentialOptimizer::New();
    typename ExhaustiveExponentialOptimizer::StepsType fineNbSteps(initialParameters.Size());
    fineNbSteps.Fill(m_FineOptimizationNumberOfSteps);

    double stepLength = 1. / static_cast<double>(m_FineOptimizationNumberOfSteps);

    fineOptimizer->SetNumberOfSteps(fineNbSteps);
    fineOptimizer->SetStepLength(stepLength);
    fineOptimizer->SetCostFunction(crossValidationFunction);
    fineOptimizer->SetInitialPosition(coarseBestParameters);
    fineOptimizer->StartOptimization();

    otbMsgDevMacro(<< "Fine minimum accuracy: " << fineOptimizer->GetMinimumMetricValue() << " " << fineOptimizer->GetMinimumMetricValuePosition());
    otbMsgDevMacro(<< "Fine maximum accuracy: " << fineOptimizer->GetMaximumMetricValue() << " " << fineOptimizer->GetMaximumMetricValuePosition());

    fineBestParameters = fineOptimizer->GetMaximumMetricValuePosition();

    m_FinalCrossValidationAccuracy = fineOptimizer->GetMaximumMetricValue();

    this->SetC(fineBestParameters[0]);
    if (nbParams > 1)
      this->SetKernelGamma(fineBestParameters[1]);
    if (nbParams > 2)
      this->SetKernelCoef0(fineBestParameters[2]);
  }
}

} // end namespace otb

#endif
