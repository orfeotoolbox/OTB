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

#include <otbMachineLearningModel.h>
#include "otbConfusionMatrixCalculator.h"

#include "otbReadDataFile.h"

#include "otb_boost_string_header.h"

typedef otb::MachineLearningModel<float,short>         MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       InputValueType;
typedef MachineLearningModelType::InputSampleType      InputSampleType;
typedef MachineLearningModelType::InputListSampleType  InputListSampleType;
typedef MachineLearningModelType::TargetValueType      TargetValueType;
typedef MachineLearningModelType::TargetSampleType     TargetSampleType;
typedef MachineLearningModelType::TargetListSampleType TargetListSampleType;

typedef otb::MachineLearningModel<float,float>                   MachineLearningModelRegressionType;
typedef MachineLearningModelRegressionType::InputValueType       InputValueRegressionType;
typedef MachineLearningModelRegressionType::InputSampleType      InputSampleRegressionType;
typedef MachineLearningModelRegressionType::InputListSampleType  InputListSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetValueType      TargetValueRegressionType;
typedef MachineLearningModelRegressionType::TargetSampleType     TargetSampleRegressionType;
typedef MachineLearningModelRegressionType::TargetListSampleType TargetListSampleRegressionType;

typedef otb::ConfusionMatrixCalculator<TargetListSampleType, TargetListSampleType> ConfusionMatrixCalculatorType;

#ifdef OTB_USE_LIBSVM
#include "otbLibSVMMachineLearningModel.h"

int otbLibSVMMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3)
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }


  typedef otb::LibSVMMachineLearningModel<InputValueType, TargetValueType> SVMType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if (!otb::ReadDataFile(argv[1], samples, labels))
    {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
    }

  SVMType::Pointer classifier = SVMType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->Train();

  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout << "Confusion matrix: " << std::endl;
  std::cout << cmCalculator->GetConfusionMatrix() << std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout << "Overall Accuracy: " << cmCalculator->GetOverallAccuracy() << std::endl;

  classifier->Save(argv[2]);

  //Load Model to new LibSVM
  SVMType::Pointer classifierLoad = SVMType::New();

  classifierLoad->Load(argv[2]);
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( std::abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}
#endif

#ifdef OTB_USE_OPENCV
#include "otbSVMMachineLearningModel.h"
#include "otbKNearestNeighborsMachineLearningModel.h"
#include "otbRandomForestsMachineLearningModel.h"
#include "otbBoostMachineLearningModel.h"
#include "otbNeuralNetworkMachineLearningModel.h"
#include "otbNormalBayesMachineLearningModel.h"
#include "otbDecisionTreeMachineLearningModel.h"
#include "otbKNearestNeighborsMachineLearningModel.h"


int otbSVMMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::SVMMachineLearningModel<InputValueType, TargetValueType> SVMType;

  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if(!otb::ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  SVMType::Pointer classifier = SVMType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->Train();

  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);

  classifier->Save(argv[2]);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  //Load Model to new SVM
  SVMType::Pointer classifierLoad = SVMType::New();

  classifierLoad->Load(argv[2]);
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples);

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( std::abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}

int otbSVMMachineLearningRegressionModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::SVMMachineLearningModel<InputValueRegressionType, TargetValueRegressionType> SVMType;

  InputListSampleRegressionType::Pointer samples = InputListSampleRegressionType::New();
  TargetListSampleRegressionType::Pointer labels = TargetListSampleRegressionType::New();

  if(!otb::ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  SVMType::Pointer classifier = SVMType::New();

  //Init SVM type in regression mode
  classifier->SetRegressionMode(1);
  classifier->SetSVMType(CvSVM::EPS_SVR);
  classifier->SetP(10);
  classifier->SetKernelType(CvSVM::RBF);


  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->Train();

  //Predict age using first line of abalone dataset
  //1:-1 2:0.027027 3:0.0420168 4:-0.831858 5:-0.63733 6:-0.699395 7:-0.735352
  //8:-0.704036
  // Input value is 15.
  InputListSampleRegressionType::Pointer samplesT = InputListSampleRegressionType::New();

  //Init sample list to 8 (size of abalone dataset)
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

  if ( std::abs(age - predicted->GetMeasurementVector(0)[0]) <= 0.3 )
    {
    return EXIT_SUCCESS;
    }
  else
    {
    std::cout << age << "\t" << predicted->GetMeasurementVector(0)[0] << "\n";
    return EXIT_FAILURE;
    }
}


int otbKNearestNeighborsMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
    std::cout<<"Wrong number of arguments "<<std::endl;
    std::cout<<"Usage : sample file, output file"<<std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::KNearestNeighborsMachineLearningModel<InputValueType,TargetValueType> KNearestNeighborsType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if(!otb::ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  KNearestNeighborsType::Pointer classifier = KNearestNeighborsType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->Train();
  //write the model
  classifier->Save(argv[2]);

  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;


  //Load Model to new KNN
  KNearestNeighborsType::Pointer classifierLoad = KNearestNeighborsType::New();

  classifierLoad->Load(argv[2]);
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( std::abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}


int otbRandomForestsMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
    std::cout<<"Wrong number of arguments "<<std::endl;
    std::cout<<"Usage : sample file, output file "<<std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::RandomForestsMachineLearningModel<InputValueType,TargetValueType> RandomForestType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if(!otb::ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  std::vector<float> priors(26,1.);


  RandomForestType::Pointer classifier = RandomForestType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);

  //set parameters
  classifier->SetPriors(priors);
  // classifier->SetMaxNumberOfTrees(30);
  // classifier->SetMaxDepth(30);
  // classifier->SetMaxNumberOfCategories(30);
  // classifier->SetMaxNumberOfVariables(4);

  classifier->Train();
  classifier->Save(argv[2]);

  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  //Load Model to new RF
  RandomForestType::Pointer classifierLoad = RandomForestType::New();

  classifierLoad->Load(argv[2]);
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( std::abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}


int otbBoostMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
    std::cout<<"Wrong number of arguments "<<std::endl;
    std::cout<<"Usage : sample file, output file "<<std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::BoostMachineLearningModel<InputValueType, TargetValueType> BoostType;

  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if(!otb::ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  // Since otb::BoostMachineLearningModel ONLY handles 2-class classifications, then the
  // labels are split into 2 subsets: even (label = 1) and odd (label = 3) labels
  TargetSampleType currentLabel;
  for (unsigned itLabel = 0; itLabel < labels->Size(); ++itLabel)
    {
    currentLabel = labels->GetMeasurementVector(itLabel);
    labels->SetMeasurementVector(itLabel, (2 * (currentLabel[0] % 2)) + 1);
    }

  BoostType::Pointer classifier = BoostType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->Train();

  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);

  classifier->Save(argv[2]);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  //Load Model to new Boost model
  BoostType::Pointer classifierLoad = BoostType::New();

  classifierLoad->Load(argv[2]);
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( std::abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}


int otbANNMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3)
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }


  typedef otb::NeuralNetworkMachineLearningModel<InputValueType, TargetValueType> ANNType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if (!otb::ReadDataFile(argv[1], samples, labels))
    {
    std::cout << "Failed to read samples file " << argv[1] << std::endl;
    return EXIT_FAILURE;
    }

  std::vector<unsigned int> layerSizes;
  layerSizes.push_back(16);
  layerSizes.push_back(100);
  layerSizes.push_back(100);
  layerSizes.push_back(26);

  ANNType::Pointer classifier = ANNType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->SetLayerSizes(layerSizes);
  /*classifier->SetTrainMethod(CvANN_MLP_TrainParams::RPROP);
  classifier->SetRegPropDW0(0.1);
  classifier->SetRegPropDWMin(0.1);
  classifier->SetTermCriteriaType(CV_TERMCRIT_ITER);
  classifier->SetMaxIter(300);
  classifier->SetEpsilon(0.01); */
  classifier->Train();

  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout << "Confusion matrix: " << std::endl;
  std::cout << cmCalculator->GetConfusionMatrix() << std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout << "Overall Accuracy: " << cmCalculator->GetOverallAccuracy() << std::endl;

  classifier->Save(argv[2]);

  //Load Model to new ANN
  ANNType::Pointer classifierLoad = ANNType::New();

  classifierLoad->Load(argv[2]);
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( std::abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}


int otbNormalBayesMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::NormalBayesMachineLearningModel<InputValueType, TargetValueType> NormalBayesType;

  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if(!otb::ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  NormalBayesType::Pointer classifier = NormalBayesType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->Train();

  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);

  classifier->Save(argv[2]);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  //Load Model to new Normal Bayes
  NormalBayesType::Pointer classifierLoad = NormalBayesType::New();

  classifierLoad->Load(argv[2]);
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( std::abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}


int otbDecisionTreeMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
      std::cout<<"Wrong number of arguments "<<std::endl;
      std::cout<<"Usage : sample file, output file "<<std::endl;
      return EXIT_FAILURE;
    }

  typedef otb::DecisionTreeMachineLearningModel<InputValueType, TargetValueType> DecisionTreeType;

  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if(!otb::ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }

  DecisionTreeType::Pointer classifier = DecisionTreeType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->Train();

  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);

  classifier->Save(argv[2]);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  //Load Model to new Decision Tree
  DecisionTreeType::Pointer classifierLoad = DecisionTreeType::New();

  classifierLoad->Load(argv[2]);
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

  cmCalculatorLoad->SetProducedLabels(predictedLoad);
  cmCalculatorLoad->SetReferenceLabels(labels);
  cmCalculatorLoad->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
  const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


  if ( std::abs(kappaIdxLoad - kappaIdx) < 0.00000001)
    {
    return EXIT_SUCCESS;
    }
  else
    {
    return EXIT_FAILURE;
    }
}

#endif

#ifdef OTB_USE_SHARK
#include <chrono> // If shark is on, then we are using c++11

#include "otbSharkRandomForestsMachineLearningModel.h"

int otbSharkRFMachineLearningModel(int argc, char * argv[])
{
  if (argc != 3 )
    {
    std::cout<<"Wrong number of arguments "<<std::endl;
    std::cout<<"Usage : sample file, output file "<<std::endl;
    return EXIT_FAILURE;
    }

  typedef otb::SharkRandomForestsMachineLearningModel<InputValueType,TargetValueType> RandomForestType;
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  if(!otb::ReadDataFile(argv[1],samples,labels))
    {
    std::cout<<"Failed to read samples file "<<argv[1]<<std::endl;
    return EXIT_FAILURE;
    }


  RandomForestType::Pointer classifier = RandomForestType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->SetRegressionMode(false);
  classifier->SetNumberOfTrees(100);
  classifier->SetMTry(0);
  classifier->SetNodeSize(25);
  classifier->SetOobRatio(0.3);
  std::cout << "Train\n";
  classifier->Train();
  std::cout << "Save\n";
  classifier->Save(argv[2]);

  std::cout << "Predict\n";
  TargetListSampleType::Pointer predicted = classifier->PredictBatch(samples, NULL);

  ConfusionMatrixCalculatorType::Pointer cmCalculator = ConfusionMatrixCalculatorType::New();

  cmCalculator->SetProducedLabels(predicted);
  cmCalculator->SetReferenceLabels(labels);
  cmCalculator->Compute();

  std::cout<<"Confusion matrix: "<<std::endl;
  std::cout<<cmCalculator->GetConfusionMatrix()<<std::endl;
  const float kappaIdx = cmCalculator->GetKappaIndex();
  std::cout<<"Kappa: "<<kappaIdx<<std::endl;
  std::cout<<"Overall Accuracy: "<<cmCalculator->GetOverallAccuracy()<<std::endl;

  // //Predict single samples. Written for benchmarking purposes, but
  // too long for regression testing
  // std::cout << "Predict single samples\n";
  // auto sIt = samples->Begin();
  // auto lIt = labels->Begin();
  // auto start = std::chrono::system_clock::now();
  // for(; sIt != samples->End(); ++sIt, ++lIt)
  //   {
  //   classifier->Predict(sIt.GetMeasurementVector())[0];
  //   }
  // auto duration = std::chrono::duration_cast< TimeT>
  //   (std::chrono::system_clock::now() - start);
  // auto elapsed = duration.count();
  // std::cout << "Predict took " << elapsed << " ms\n";
  //  std::cout << "Single sample OA = " << oa << '\n';
//Load Model to new RF
  RandomForestType::Pointer classifierLoad = RandomForestType::New();

  std::cout << "Load\n";
  classifierLoad->Load(argv[2]);
  auto start = std::chrono::system_clock::now();
  std::cout << "Predict loaded\n";
  TargetListSampleType::Pointer predictedLoad = classifierLoad->PredictBatch(samples, NULL);
  using TimeT = std::chrono::milliseconds;
  auto duration = std::chrono::duration_cast< TimeT>
    (std::chrono::system_clock::now() - start);
  auto elapsed = duration.count();
  std::cout << "PredictBatch took " << elapsed << " ms\n";
  ConfusionMatrixCalculatorType::Pointer cmCalculatorLoad = ConfusionMatrixCalculatorType::New();

   cmCalculatorLoad->SetProducedLabels(predictedLoad);
   cmCalculatorLoad->SetReferenceLabels(labels);
   cmCalculatorLoad->Compute();

   std::cout<<"Confusion matrix: "<<std::endl;
   std::cout<<cmCalculatorLoad->GetConfusionMatrix()<<std::endl;
   const float kappaIdxLoad = cmCalculatorLoad->GetKappaIndex();
   std::cout<<"Kappa: "<<kappaIdxLoad<<std::endl;
   std::cout<<"Overall Accuracy: "<<cmCalculatorLoad->GetOverallAccuracy()<<std::endl;


   if ( std::abs(kappaIdxLoad - kappaIdx) < 0.00000001)
     {
     return EXIT_SUCCESS;
     }
   else
     {
     return EXIT_FAILURE;
     }

   return EXIT_SUCCESS;
}


#endif
