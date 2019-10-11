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


#include "itkMacro.h"
#include <iostream>

#include "otbSVMSampleListModelEstimator.h"
#include "otbSVMClassifier.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "otbSVMClassifier.h"
#include "otbConfusionMatrixCalculator.h"


#include <fstream>

/*
This test show a problem with the SVM library using the probability
estimation : bug 209.
If the probability estimation is activated, the classifier isn't
abble to find the hyperplan even if the sample are linearly
seperable.
cf. test leTvBug209_SVMValidationLinearlySeparableWithoutProbEstimate
=> OK
and leTvBug209_SVMValidationLinearlySeparableWithProbEstimate => KO
http://bugs.orfeo-toolbox.org/view.php?id=209
*/

int main(int argc, char* argv[])
{
  if (argc != 14)
  {
    std::cerr << "Usage: " << argv[0] << " nbTrainingSamples nbValidationSamples positiveCenterX positiveCenterY negativeCenterX negativeCenterY "
                                         "positiveRadiusMin positiveRadiusMax negativeRadiusMin negativeRadiusMax kernel probEstimate"
              << std::endl;
    return EXIT_FAILURE;
  }
  unsigned int nbTrainingSamples   = atoi(argv[2]);
  unsigned int nbValidationSamples = atoi(argv[3]);
  double       cpx                 = atof(argv[4]);
  double       cpy                 = atof(argv[5]);
  double       cnx                 = atof(argv[6]);
  double       cny                 = atof(argv[7]);
  double       prmin               = atof(argv[8]);
  double       prmax               = atof(argv[9]);
  double       nrmin               = atof(argv[10]);
  double       nrmax               = atof(argv[11]);
  unsigned int kernel              = atoi(argv[12]);
  bool         probEstimate        = atoi(argv[13]);

  typedef double         InputPixelType;
  typedef unsigned short LabelType;

  typedef itk::VariableLengthVector<InputPixelType> SampleType;
  typedef itk::Statistics::ListSample<SampleType>   ListSampleType;
  typedef itk::FixedArray<LabelType, 1> TrainingSampleType;
  typedef itk::Statistics::ListSample<TrainingSampleType>        TrainingListSampleType;
  typedef itk::Statistics::MersenneTwisterRandomVariateGenerator RandomGeneratorType;
  typedef otb::SVMSampleListModelEstimator<ListSampleType, TrainingListSampleType> EstimatorType;
  typedef otb::SVMClassifier<ListSampleType, LabelType>                            ClassifierType;
  typedef ClassifierType::OutputType ClassifierOutputType;
  typedef otb::ConfusionMatrixCalculator<TrainingListSampleType, TrainingListSampleType> ConfusionMatrixCalculatorType;


  RandomGeneratorType::Pointer random = RandomGeneratorType::New();
  random->SetSeed((unsigned int)0);

  // First, generate training and validation sets
  ListSampleType::Pointer         trainingSamples   = ListSampleType::New();
  TrainingListSampleType::Pointer trainingLabels    = TrainingListSampleType::New();
  ListSampleType::Pointer         validationSamples = ListSampleType::New();
  TrainingListSampleType::Pointer validationLabels  = TrainingListSampleType::New();

  // Set the size of the measurement vectors
  trainingSamples->SetMeasurementVectorSize(2);
  trainingLabels->SetMeasurementVectorSize(1);
  validationSamples->SetMeasurementVectorSize(2);
  validationLabels->SetMeasurementVectorSize(1);

  // Generate training set
  // std::ofstream training("training.csv");
  for (unsigned int i = 0; i < nbTrainingSamples; ++i)
  {
    // Generate a positive sample
    double     angle  = random->GetVariateWithOpenUpperRange(otb::CONST_2PI);
    double     radius = random->GetUniformVariate(prmin, prmax);
    SampleType pSample(2);
    pSample[0] = cpx + radius * std::sin(angle);
    pSample[1] = cpy + radius * std::cos(angle);
    TrainingSampleType label;
    label[0] = 1;
    trainingSamples->PushBack(pSample);
    trainingLabels->PushBack(label);

    // training<<"1 1:"<<pSample[0]<<" 2:"<<pSample[1]<<std::endl;

    // Generate a negative sample
    angle  = random->GetVariateWithOpenUpperRange(otb::CONST_2PI);
    radius = random->GetUniformVariate(nrmin, nrmax);
    SampleType nSample(2);
    nSample[0] = cnx + radius * std::sin(angle);
    nSample[1] = cny + radius * std::cos(angle);
    label[0]   = 2;
    trainingSamples->PushBack(nSample);
    trainingLabels->PushBack(label);

    // training<<"2 1:"<<nSample[0]<<" 2:"<<nSample[1]<<std::endl;
  }
  // training.close();

  // Generate validation set

  std::ofstream validation("validation.csv");
  for (unsigned int i = 0; i < nbValidationSamples; ++i)
  {
    // Generate a positive sample
    double     angle  = random->GetVariateWithOpenUpperRange(otb::CONST_2PI);
    double     radius = random->GetUniformVariate(prmin, prmax);
    SampleType pSample(2);
    pSample[0] = cpx + radius * std::sin(angle);
    pSample[1] = cpy + radius * std::cos(angle);
    TrainingSampleType label;
    label[0] = 1;
    validationSamples->PushBack(pSample);
    validationLabels->PushBack(label);
    // validation<<"1 1:"<<pSample[0]<<" 2:"<<pSample[1]<<std::endl;

    // Generate a negative sample
    angle  = random->GetVariateWithOpenUpperRange(otb::CONST_2PI);
    radius = random->GetUniformVariate(nrmin, nrmax);
    SampleType nSample(2);
    nSample[0] = cnx + radius * std::sin(angle);
    nSample[1] = cny + radius * std::cos(angle);
    label[0]   = 2;
    validationSamples->PushBack(nSample);
    validationLabels->PushBack(label);
    // validation<<"2 1:"<<nSample[0]<<" 2:"<<nSample[1]<<std::endl;
  }
  // validation.close();

  // Learn
  EstimatorType::Pointer estimator = EstimatorType::New();
  estimator->SetInputSampleList(trainingSamples);
  estimator->SetTrainingSampleList(trainingLabels);

  estimator->SetKernelType(kernel);
  estimator->SetDoProbabilityEstimates(probEstimate);
  //  estimator->SetParametersOptimization(true);
  estimator->Update();

  // estimator->SaveModel("model.svm");

  // Classify
  ClassifierType::Pointer validationClassifier = ClassifierType::New();
  validationClassifier->SetInput(validationSamples);
  validationClassifier->SetNumberOfClasses(2);
  validationClassifier->SetModel(estimator->GetModel());
  validationClassifier->Update();

  // Confusion
  ClassifierOutputType::ConstIterator it    = validationClassifier->GetOutput()->Begin();
  ClassifierOutputType::ConstIterator itEnd = validationClassifier->GetOutput()->End();

  TrainingListSampleType::Pointer classifierListLabel = TrainingListSampleType::New();

  while (it != itEnd)
  {
    classifierListLabel->PushBack(it.GetClassLabel());
    ++it;
  }

  ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();

  confMatCalc->SetReferenceLabels(validationLabels);
  confMatCalc->SetProducedLabels(classifierListLabel);

  confMatCalc->Compute();

  std::cout << std::endl;
  std::cout << "Confusion matrix: " << std::endl << confMatCalc->GetConfusionMatrix() << std::endl << std::endl;
  std::cout << "Kappa Index: " << std::endl << confMatCalc->GetKappaIndex() << std::endl << std::endl;

  if (confMatCalc->GetKappaIndex() != 1)
  {
    std::cerr << "Kappa index should be 1." << std::endl;
    return EXIT_FAILURE;
  }
  else
  {
    return EXIT_SUCCESS;
  }
}
