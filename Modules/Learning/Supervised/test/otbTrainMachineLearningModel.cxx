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


#include <fstream>
#include <string>
#include <algorithm>
#include <chrono>

#include "otbMacro.h"

#include <otbMachineLearningModel.h>
#include "otbConfusionMatrixCalculator.h"

#include "otbReadDataFile.h"

#include "otb_boost_string_header.h"

typedef otb::MachineLearningModel<float, short> MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       InputValueType;
typedef MachineLearningModelType::InputSampleType      InputSampleType;
typedef MachineLearningModelType::InputListSampleType  InputListSampleType;
typedef MachineLearningModelType::TargetValueType      TargetValueType;
typedef MachineLearningModelType::TargetSampleType     TargetSampleType;
typedef MachineLearningModelType::TargetListSampleType TargetListSampleType;

typedef otb::MachineLearningModel<float, float> MachineLearningModelRegressionType;
typedef MachineLearningModelRegressionType::InputValueType       InputValueRegressionType;
typedef MachineLearningModelRegressionType::InputSampleType      InputSampleRegressionType;
typedef MachineLearningModelRegressionType::InputListSampleType  InputListSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetValueType      TargetValueRegressionType;
typedef MachineLearningModelRegressionType::TargetSampleType     TargetSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetListSampleType TargetListSampleRegressionType;

typedef otb::ConfusionMatrixCalculator<TargetListSampleType, TargetListSampleType> ConfusionMatrixCalculatorType;

float GetConfusionMatrixResults(TargetListSampleType::Pointer predicted, TargetListSampleType::Pointer ref)
{
  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();
  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(ref);
  cmCalculator->Compute();

  otbLogMacro(Debug, << "Confusion matrix:\n" << cmCalculator->GetConfusionMatrix());
  otbLogMacro(Info, << "Kappa: " << cmCalculator->GetKappaIndex());
  otbLogMacro(Debug, << "Overall Accuracy: " << cmCalculator->GetOverallAccuracy());

  return cmCalculator->GetKappaIndex();
}

template <class TModel>
void SetupModel(TModel* /*model*/)
{
  // do nothing by default
}

template <class TModel>
int otbGenericMachineLearningModel(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Wrong number of arguments " << std::endl;
    std::cout << "Usage : sample file, output file " << std::endl;
    return EXIT_FAILURE;
  }
  InputListSampleType::Pointer  samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels  = TargetListSampleType::New();
  if (!otb::ReadDataFile(argv[1], samples, labels))
  {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  typename TModel::Pointer classifier = TModel::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  SetupModel<TModel>(classifier);
  classifier->Train();
  classifier->Save(argv[2]);
  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);
  const float                   kappa     = GetConfusionMatrixResults(predicted, labels);

  typename TModel::Pointer classifierLoad = TModel::New();
  classifierLoad->Load(argv[2]);
  auto start = std::chrono::system_clock::now();
  otbLogMacro(Debug, << "Predict loaded");
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples, NULL);
  using TimeT                                 = std::chrono::milliseconds;
  auto duration                               = std::chrono::duration_cast<TimeT>(std::chrono::system_clock::now() - start);
  auto elapsed                                = duration.count();
  otbLogMacro(Debug, << "PredictBatch took " << elapsed << " ms");
  const float kappaLoad = GetConfusionMatrixResults(predictedLoad, labels);

  return (std::abs(kappaLoad - kappa) < 0.00000001 ? EXIT_SUCCESS : EXIT_FAILURE);
}

// -------------------------- LibSVM -------------------------------------------
#ifdef OTB_USE_LIBSVM
#include "otbLibSVMMachineLearningModel.h"

using LibSVMType = otb::LibSVMMachineLearningModel<InputValueType, TargetValueType>;
int otbLibSVMMachineLearningModel(int argc, char* argv[])
{
  return otbGenericMachineLearningModel<LibSVMType>(argc, argv);
}
#endif

// -------------------------- OpenCV -------------------------------------------
#ifdef OTB_USE_OPENCV
#include "otbSVMMachineLearningModel.h"
#include "otbKNearestNeighborsMachineLearningModel.h"
#include "otbRandomForestsMachineLearningModel.h"
#include "otbBoostMachineLearningModel.h"
#include "otbNeuralNetworkMachineLearningModel.h"
#include "otbNormalBayesMachineLearningModel.h"
#include "otbDecisionTreeMachineLearningModel.h"
#include "otbKNearestNeighborsMachineLearningModel.h"

using SVMType = otb::SVMMachineLearningModel<InputValueType, TargetValueType>;
int otbSVMMachineLearningModel(int argc, char* argv[])
{
  return otbGenericMachineLearningModel<SVMType>(argc, argv);
}

int otbSVMMachineLearningRegressionModel(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cout << "Wrong number of arguments " << std::endl;
    std::cout << "Usage : sample file, output file " << std::endl;
    return EXIT_FAILURE;
  }

  typedef otb::SVMMachineLearningModel<InputValueRegressionType, TargetValueRegressionType> SVMType;

  InputListSampleRegressionType::Pointer  samples = InputListSampleRegressionType::New();
  TargetListSampleRegressionType::Pointer labels  = TargetListSampleRegressionType::New();

  if (!otb::ReadDataFile(argv[1], samples, labels))
  {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  SVMType::Pointer classifier = SVMType::New();

  // Init SVM type in regression mode
  classifier->SetRegressionMode(1);
  classifier->SetSVMType(CvSVM::EPS_SVR);
  classifier->SetP(10);
  classifier->SetKernelType(CvSVM::RBF);


  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->Train();

  // Predict age using first line of abalone dataset
  // 1:-1 2:0.027027 3:0.0420168 4:-0.831858 5:-0.63733 6:-0.699395 7:-0.735352
  // 8:-0.704036
  // Input value is 15.
  InputListSampleRegressionType::Pointer samplesT = InputListSampleRegressionType::New();

  // Init sample list to 8 (size of abalone dataset)
  InputSampleRegressionType sample(8);
  sample.Fill(0);
  sample[0] = -1;
  sample[1] = 0.027027;
  sample[2] = 0.0420168;
  sample[3] = -0.831858;
  sample[4] = -0.63733;
  sample[5] = -0.699395;
  sample[6] = -0.735352;
  sample[7] = -0.704036;

  samplesT->SetMeasurementVectorSize(itk::NumericTraits<InputSampleRegressionType>::GetLength(sample));
  samplesT->PushBack(sample);

  TargetListSampleRegressionType::Pointer predicted = classifier->PredictBatch(samplesT, NULL);

  const float age = 15;

  if (std::abs(age - predicted->GetMeasurementVector(0)[0]) <= 0.3)
  {
    return EXIT_SUCCESS;
  }
  else
  {
    std::cout << age << "\t" << predicted->GetMeasurementVector(0)[0] << "\n";
    return EXIT_FAILURE;
  }
}

using KNearestNeighborsType = otb::KNearestNeighborsMachineLearningModel<InputValueType, TargetValueType>;
int otbKNearestNeighborsMachineLearningModel(int argc, char* argv[])
{
  return otbGenericMachineLearningModel<KNearestNeighborsType>(argc, argv);
}

using RandomForestType = otb::RandomForestsMachineLearningModel<InputValueType, TargetValueType>;
int otbRandomForestsMachineLearningModel(int argc, char* argv[])
{
  return otbGenericMachineLearningModel<RandomForestType>(argc, argv);
}

template <>
void SetupModel(RandomForestType* model)
{
  std::vector<float> priors(26, 1.);
  model->SetPriors(priors);
}

using BoostType = otb::BoostMachineLearningModel<InputValueType, TargetValueType>;
int otbBoostMachineLearningModel(int argc, char* argv[])
{
  return otbGenericMachineLearningModel<BoostType>(argc, argv);
}

template <>
void SetupModel(BoostType* model)
{
  // Since otb::BoostMachineLearningModel ONLY handles 2-class classifications, then the
  // labels are split into 2 subsets: even (label = 1) and odd (label = 3) labels
  TargetListSampleType::Pointer labels = model->GetTargetListSample();
  TargetSampleType              currentLabel;
  for (unsigned itLabel = 0; itLabel < labels->Size(); ++itLabel)
  {
    currentLabel = labels->GetMeasurementVector(itLabel);
    labels->SetMeasurementVector(itLabel, (2 * (currentLabel[0] % 2)) + 1);
  }
  model->SetTargetListSample(labels);
}

using ANNType = otb::NeuralNetworkMachineLearningModel<InputValueType, TargetValueType>;
int otbANNMachineLearningModel(int argc, char* argv[])
{
  return otbGenericMachineLearningModel<ANNType>(argc, argv);
}

template <>
void SetupModel(ANNType* model)
{
  std::vector<unsigned int> layerSizes;
  layerSizes.push_back(16);
  layerSizes.push_back(100);
  layerSizes.push_back(100);
  layerSizes.push_back(26);
  model->SetLayerSizes(layerSizes);
}

using NormalBayesType = otb::NormalBayesMachineLearningModel<InputValueType, TargetValueType>;
int otbNormalBayesMachineLearningModel(int argc, char* argv[])
{
  return otbGenericMachineLearningModel<NormalBayesType>(argc, argv);
}

using DecisionTreeType = otb::DecisionTreeMachineLearningModel<InputValueType, TargetValueType>;
int otbDecisionTreeMachineLearningModel(int argc, char* argv[])
{
  return otbGenericMachineLearningModel<DecisionTreeType>(argc, argv);
}
#endif

// -------------------------- Shark --------------------------------------------
#ifdef OTB_USE_SHARK
#include "otbSharkRandomForestsMachineLearningModel.h"

using SharkRandomForestType = otb::SharkRandomForestsMachineLearningModel<InputValueType, TargetValueType>;
int otbSharkRFMachineLearningModel(int argc, char* argv[])
{
  return otbGenericMachineLearningModel<SharkRandomForestType>(argc, argv);
}

template <>
void SetupModel(SharkRandomForestType* model)
{
  model->SetRegressionMode(false);
  model->SetNumberOfTrees(100);
  model->SetMTry(0);
  model->SetNodeSize(25);
  model->SetOobRatio(0.3);
}
#endif
