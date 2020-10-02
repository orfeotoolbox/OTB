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


#include "otbMacro.h"
#include "otbSatelliteRSR.h"
#include "otbReduceSpectralResponse.h"

#include "otbLibSVMMachineLearningModel.h"
#include "otbConfusionMatrixCalculator.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"
#include <itksys/Glob.hxx>

int otbReduceSpectralResponseSVMClassifier(int argc, char* argv[])
{

  typedef otb::SpectralResponse<double, double> ResponseType;
  typedef ResponseType::Pointer ResponsePointerType;

  typedef otb::SatelliteRSR<double, double> SatRSRType;
  typedef SatRSRType::Pointer SatRSRPointerType;

  typedef otb::ReduceSpectralResponse<ResponseType, SatRSRType> ReduceResponseType;
  typedef ReduceResponseType::Pointer ReduceResponseTypePointerType;

  typedef itk::VariableLengthVector<double>       SampleType;
  typedef itk::Statistics::ListSample<SampleType> SampleListType;
  typedef itk::FixedArray<unsigned long, 1> TrainingSampleType;
  typedef itk::Statistics::ListSample<TrainingSampleType> TrainingSampleListType;

  typedef otb::LibSVMMachineLearningModel<double, unsigned long> SVMType;

  typedef otb::ConfusionMatrixCalculator<TrainingSampleListType, TrainingSampleListType> ConfusionMatrixCalculatorType;

  if (argc != 9)
  {
    std::cout << argv[0] << std::endl
              << "\t"
              << "<dir_spectres_class1>"
              << "\t"
              << "<dir_spectres_class2>"
              << "\t"
              << "<dir_spectres_class3>"
              << "\t"
              << "<dir_spectres_class4>"
              << "\t"
              << "<dir_spectres_class5>"
              << "\t"
              << "<Gabarit_SAT_fileSRname>"
              << "\t"
              << "<nbBands>"
              << "\t"
              << "<percentage_validation_files>" << std::endl;
    return EXIT_FAILURE;
  }

  // Instantiation

  // Load fileSR into vector
  std::vector<std::string> dirSR;
  dirSR.push_back(argv[1]);
  dirSR.push_back(argv[2]);
  dirSR.push_back(argv[3]);
  dirSR.push_back(argv[4]);
  dirSR.push_back(argv[5]);

  const std::string fileSatG(argv[6]);
  unsigned int      nbBand     = atoi(argv[7]);
  float             percentage = atof(argv[8]);

  SatRSRPointerType satRSR = SatRSRType::New();
  /** Set the satellite number of bands */
  satRSR->SetNbBands(nbBand);
  /** Load the satellite response file*/
  satRSR->Load(fileSatG);

  // divide into training and testing files
  // 90% of files are used for training and 10% for testing
  std::vector<std::string>  trainingFiles;    // contains training files for all classes
  std::vector<unsigned int> trainingClasses;  // contains training classes for each files
  std::vector<std::string>  testingFiles;     // contains testing files for this classes
  std::vector<unsigned int> testingGTClasses; // contains testing ground truth classes for each files
  unsigned int              ind;
  const std::string         regularExpressionPattern = ".*\\.txt$";

  for (unsigned int i = 0; i < dirSR.size(); ++i) // for each class (directory)
  {
    otbLogMacro(Info, << "dirSR[" << i << "] : " << dirSR[i]);

    std::string fileExp = dirSR[i];
    // Find all .txt file in the directory
    fileExp.append("/*.txt");
    itksys::Glob glob;
    if (glob.FindFiles(fileExp) == false)
    {
      otbLogMacro(Warning, << "No .txt file found in " << dirSR[i] << ".");
      return EXIT_FAILURE;
    }

    std::vector<std::string> result = glob.GetFiles();

    std::vector<std::string> training; // contains training files for this class (directory)
    std::vector<std::string> testing;  // contains testing files for this class (directory)
    training = result;

    // initiating random number generation
    itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer randomGen = itk::Statistics::MersenneTwisterRandomVariateGenerator::New();
    // randomGen->Initialize();

    for (unsigned int j = 0; j < static_cast<unsigned int>(percentage * result.size()); ++j)
    {
      ind = randomGen->GetIntegerVariate() % (result.size());
      testing.push_back(result[ind]);
      training.erase(training.begin() + (ind - j));
    }

    // add to global training files and testing files
    for (unsigned int k = 0; k < testing.size(); ++k)
    {
      otbLogMacro(Debug, << "testing[" << k << "] : " << testing[k]);
      testingFiles.push_back(testing[k]);
      testingGTClasses.push_back(i);
    }

    for (unsigned int l = 0; l < training.size(); ++l)
    {
      otbLogMacro(Debug, << "training[" << l << "] : " << training[l]);
      trainingFiles.push_back(training[l]);
      trainingClasses.push_back(i); // class is the directory number
    }
  }

  // compute spectral response for all training files
  SampleListType::Pointer         sampleList   = SampleListType::New();
  TrainingSampleListType::Pointer trainingList = TrainingSampleListType::New();
  for (unsigned int i = 0; i < trainingFiles.size(); ++i)
  {
    ResponsePointerType spectralResponse = ResponseType::New();
    otbLogMacro(Debug, << "training file : " << trainingFiles[i]);
    spectralResponse->Load(trainingFiles[i], 100.0);

    // Compute Reduce Spectral Response
    ReduceResponseTypePointerType reduceResponse = ReduceResponseType::New();
    /** Load the satellite response in the simulator */
    reduceResponse->SetInputSatRSR(satRSR);
    /** Load the spectral response of the object in the simulator*/
    reduceResponse->SetInputSpectralResponse(spectralResponse);
    reduceResponse->CalculateResponse();

    // Get the response in an itk::VariableLengthVector and add it to the sample list for SVMModelEstimator
    SampleType         sample;
    TrainingSampleType trainingSample;
    sample.SetSize(reduceResponse->GetReduceResponse()->Size());
    std::ostringstream oss;
    oss << "reduce response : [";
    for (unsigned int j = 0; j < reduceResponse->GetReduceResponse()->Size(); ++j)
    {
      sample[j] = reduceResponse->GetReduceResponse()->GetResponse()[j].second;
      oss << reduceResponse->GetReduceResponse()->GetResponse()[j].second << " ";
    }
    oss << "]";
    otbLogMacro(Debug, << oss.str());
    sampleList->SetMeasurementVectorSize(nbBand);
    sampleList->PushBack(sample);
    trainingSample = trainingClasses[i];
    otbLogMacro(Debug, << "training class : " << trainingSample);
    trainingList->PushBack(trainingSample);
  }

  // SVM model estimator
  SVMType::Pointer model = SVMType::New();
  model->SetInputListSample(sampleList);
  model->SetTargetListSample(trainingList);
  model->SetNu(0.5);
  model->SetKernelGamma(1);
  model->SetKernelCoef0(1);
  model->SetC(1);
  model->SetEpsilon(0.001);
  model->SetP(0.1);
  model->SetDoProbabilityEstimates(true);

  model->Train();
  model->Save("model.txt");

  // compute spectral response for testing files
  sampleList->Clear(); // clear the sample list to re use it for testing samples
  TrainingSampleListType::Pointer groundTruthClassList = TrainingSampleListType::New();
  for (unsigned int i = 0; i < testingFiles.size(); ++i)
  {
    ResponsePointerType spectralResponse = ResponseType::New();
    otbLogMacro(Debug, << "testing file : " << testingFiles[i]);
    spectralResponse->Load(testingFiles[i], 100.0);

    // Compute Reduce Spectral Response
    ReduceResponseTypePointerType reduceResponse = ReduceResponseType::New();
    /** Load the satellite response in the simulator */
    reduceResponse->SetInputSatRSR(satRSR);
    /** Load the spectral response of the object in the simulator*/
    reduceResponse->SetInputSpectralResponse(spectralResponse);
    reduceResponse->CalculateResponse();

    // Get the response in an itk::VariableLengthVector and add it to the sample list for SVMClassifier
    SampleType         sample;
    TrainingSampleType gtClass;
    sample.SetSize(reduceResponse->GetReduceResponse()->Size());
    for (unsigned int j = 0; j < reduceResponse->GetReduceResponse()->Size(); ++j)
    {
      sample[j] = reduceResponse->GetReduceResponse()->GetResponse()[j].second;
    }
    sampleList->PushBack(sample);
    gtClass = testingGTClasses[i];
    groundTruthClassList->PushBack(gtClass);
  }

  // SVM Classifier
  TrainingSampleListType::Pointer classifierListLabel = model->PredictBatch(sampleList);

  TrainingSampleListType::ConstIterator it = classifierListLabel->Begin();
  while (it != classifierListLabel->End())
  {
    otbLogMacro(Debug, << "class : " << it.GetMeasurementVector()[0]);
    ++it;
  }
  for (unsigned int i = 0; i < testingFiles.size(); ++i)
  {
    otbLogMacro(Debug, << "ground truth class : " << testingGTClasses[i]);
  }

  // Compute confusion matrix
  ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();
  confMatCalc->SetReferenceLabels(groundTruthClassList);
  confMatCalc->SetProducedLabels(classifierListLabel);
  confMatCalc->Compute();

  otbLogMacro(Info, << "Confusion matrix: " << std::endl << confMatCalc->GetConfusionMatrix());
  otbLogMacro(Info, << "Kappa Index: " << std::endl << confMatCalc->GetKappaIndex());

  return EXIT_SUCCESS;
}
