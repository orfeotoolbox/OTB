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


#include "otbAtmosphericEffects.h"
#include "otbSatelliteRSR.h"
#include "otbReduceSpectralResponse.h"

#include "otbLibSVMMachineLearningModel.h"
#include "otbConfusionMatrixCalculator.h"

#include "itkMersenneTwisterRandomVariateGenerator.h"

#include <itksys/Glob.hxx>

int otbAtmosphericCorrectionsRSRSVMClassifier(int argc, char* argv[])
{

  typedef otb::SpectralResponse<double, double> SpectralResponseType;
  typedef SpectralResponseType::Pointer SpectralResponsePointerType;

  typedef otb::SatelliteRSR<double, double> SatRSRType;
  typedef SatRSRType::Pointer SatRSRPointerType;

  typedef otb::ReduceSpectralResponse<SpectralResponseType, SatRSRType> ReduceSpectralResponseType;

  typedef otb::AtmosphericEffects<SpectralResponseType, SatRSRType> AtmosphericEffectsType;

  /*typedef AtmosphericEffectsType::AtmosphericCorrectionParametersType AtmosphericCorrectionParametersType; chris
  typedef AtmosphericCorrectionParametersType::AerosolModelType AerosolModelType;
  typedef AtmosphericCorrectionParametersType::AerosolModelType AerosolModelType; */


  typedef otb::RadiometryCorrectionParametersToAtmosphericRadiativeTerms CorrectionParametersToRadiativeTermsType;

  typedef otb::AtmosphericCorrectionParameters  AtmoCorrectionParametersType;
  typedef AtmoCorrectionParametersType::Pointer AtmoCorrectionParametersPointerType;

  typedef AtmoCorrectionParametersType::AerosolModelType AerosolModelType;

  typedef otb::ImageMetadataCorrectionParameters AcquiCorrectionParametersType;
  typedef AcquiCorrectionParametersType::Pointer AcquiCorrectionParametersPointerType;

  typedef otb::AtmosphericRadiativeTerms         AtmosphericRadiativeTermsType;
  typedef AtmosphericRadiativeTermsType::Pointer AtmosphericRadiativeTermsPointerType;


  typedef itk::VariableLengthVector<double>       SampleType;
  typedef itk::Statistics::ListSample<SampleType> SampleListType;
  typedef itk::FixedArray<unsigned long, 1> TargetSampleType;
  typedef itk::Statistics::ListSample<TargetSampleType> TargetSampleListType;

  typedef otb::LibSVMMachineLearningModel<double, unsigned long> SVMType;

  typedef otb::ConfusionMatrixCalculator<TargetSampleListType, TargetSampleListType> ConfusionMatrixCalculatorType;

  if (argc != 20)
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
              << "<day>"
              << "\t"
              << "<month>"
              << "\t"
              << "<zenithSolarAngle>"
              << "\t"
              << "<azimutSolarAngle>"
              << "\t"
              << "<viewingZenitalAngle>"
              << "\t"
              << "<viewingAzimutalAngle>"
              << "\t"
              << "<atmoPressure>"
              << "\t"
              << "<waterVaporAmount>"
              << "\t"
              << "<ozoneAmount>"
              << "\t"
              << "<aerosolModelValue>"
              << "\t"
              << "<aerosolOptical>"
              << "\t"
              << "<percentage_validation_files>" << std::endl;
    return EXIT_FAILURE;
  }
  /*
    27.3    #elevation et azimuth solaire
    4       #day
    12      #month
    # AtmosphericCorrectionParametersTo6SAtmosphericRadiativeTerms parameters
    152.7 #solar azimutal angle
    2.5 #viewing zenithal angle
    -77.0 #viewing azimutal angle
    1013. #atmo pressure
    2.48134 #water vapour amount
    0.34400 #ozone amount
    1 #aerosol model type
    0.199854 #aerosol optical
  */

  // Get input parameters
  std::vector<std::string> dirSR;
  dirSR.push_back(argv[1]);
  dirSR.push_back(argv[2]);
  dirSR.push_back(argv[3]);
  dirSR.push_back(argv[4]);
  dirSR.push_back(argv[5]);
  const std::string fileSatG(argv[6]);
  unsigned int      nbBand = atoi(argv[7]);
  const int         day(atoi(argv[8]));
  const int         month(atoi(argv[9]));
  const double      zenithSolarAngle(static_cast<double>(atof(argv[10])));
  const double      azimutSolarAngle(static_cast<double>(atof(argv[11])));
  const double      viewingZenitalAngle(static_cast<double>(atof(argv[12])));
  const double      viewingAzimutalAngle(static_cast<double>(atof(argv[13])));
  const double      atmoPressure(static_cast<double>(atof(argv[14])));
  const double      waterVaporAmount(static_cast<double>(atof(argv[15])));
  const double      ozoneAmount(static_cast<double>(atof(argv[16])));
  const int         aerosolModelValue(::atoi(argv[17]));
  const double      aerosolOptical(static_cast<double>(atof(argv[18])));
  float             percentage = atof(argv[19]);

  // Instantiation
  /*AtmosphericCorrectionParametersType::Pointer chris
    dataAtmosphericCorrectionParameters = AtmosphericCorrectionParametersType::New(); */

  AcquiCorrectionParametersPointerType paramAcqui = AcquiCorrectionParametersType::New();
  AtmoCorrectionParametersPointerType  paramAtmo  = AtmoCorrectionParametersType::New();

  SatRSRPointerType satRSR = SatRSRType::New();

  satRSR->SetNbBands(nbBand);
  satRSR->Load(fileSatG);

  // Set parameters
  /*dataAtmosphericCorrectionParameters->SetSolarZenithalAngle(zenithSolarAngle); chris
  dataAtmosphericCorrectionParameters->SetSolarAzimutalAngle(azimutSolarAngle);
  dataAtmosphericCorrectionParameters->SetViewingZenithalAngle(viewingZenitalAngle);
  dataAtmosphericCorrectionParameters->SetViewingAzimutalAngle(viewingAzimutalAngle);
  dataAtmosphericCorrectionParameters->SetMonth(month);
  dataAtmosphericCorrectionParameters->SetDay(day);
  dataAtmosphericCorrectionParameters->SetAtmosphericPressure(atmoPressure);
  dataAtmosphericCorrectionParameters->SetWaterVaporAmount(waterVaporAmount);
  dataAtmosphericCorrectionParameters->SetOzoneAmount(ozoneAmount);

  AerosolModelType aerosolModel = static_cast<AerosolModelType> (aerosolModelValue);
  dataAtmosphericCorrectionParameters->SetAerosolModel(aerosolModel);
  dataAtmosphericCorrectionParameters->SetAerosolOptical(aerosolOptical); */

  paramAcqui->SetSolarZenithalAngle(zenithSolarAngle);
  paramAcqui->SetSolarAzimutalAngle(azimutSolarAngle);
  paramAcqui->SetViewingZenithalAngle(viewingZenitalAngle);
  paramAcqui->SetViewingAzimutalAngle(viewingAzimutalAngle);
  paramAcqui->SetMonth(month);
  paramAcqui->SetDay(day);
  paramAtmo->SetAtmosphericPressure(atmoPressure);
  paramAtmo->SetWaterVaporAmount(waterVaporAmount);
  paramAtmo->SetOzoneAmount(ozoneAmount);
  AerosolModelType aerosolModel = static_cast<AerosolModelType>(aerosolModelValue);
  paramAtmo->SetAerosolModel(aerosolModel);
  paramAtmo->SetAerosolOptical(aerosolOptical);

  AtmosphericRadiativeTermsPointerType radiative = CorrectionParametersToRadiativeTermsType::Compute(paramAtmo, paramAcqui);

  // divide into training and testing files
  // 90% of files are used for training and 10% for testing
  std::vector<std::string>  trainingFiles;    // contains training files for all classes
  std::vector<unsigned int> trainingClasses;  // contains training classes for each files
  std::vector<std::string>  testingFiles;     // contains testing files for this classes
  std::vector<unsigned int> testingGTClasses; // contains testing ground truth classes for each files
  unsigned int              ind;


  for (unsigned int i = 0; i < dirSR.size(); ++i) // for each class (directory)
  {
    std::cout << "dirSR[" << i << "] : " << dirSR[i] << std::endl;

    // Find all .txt file in the directory
    std::string fileExp = dirSR[i];
    fileExp.append("/*.txt");
    itksys::Glob glob;
    if (glob.FindFiles(fileExp) == false)
    {
      std::cout << "No .txt file found in " << dirSR[i] << "." << std::endl;
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
      training.erase(training.begin() + j);
    }

    // add to global training files and testing files
    for (unsigned int k = 0; k < testing.size(); ++k)
    {
      std::cout << "testing[" << k << "] : " << testing[k] << std::endl;
      testingFiles.push_back(testing[k]);
      testingGTClasses.push_back(i);
    }

    for (unsigned int l = 0; l < training.size(); ++l)
    {
      std::cout << "training[" << l << "] : " << training[l] << std::endl;
      trainingFiles.push_back(training[l]);
      trainingClasses.push_back(i); // class is the directory number
    }
  }

  // compute spectral response for all training files
  SampleListType::Pointer       sampleList   = SampleListType::New();
  TargetSampleListType::Pointer trainingList = TargetSampleListType::New();

  for (unsigned int i = 0; i < trainingFiles.size(); ++i)
  {
    SpectralResponsePointerType         spectralResponse         = SpectralResponseType::New();
    ReduceSpectralResponseType::Pointer reduceSpectralResponse   = ReduceSpectralResponseType::New();
    AtmosphericEffectsType::Pointer     atmosphericEffectsFilter = AtmosphericEffectsType::New();

    std::cout << "training file : " << trainingFiles[i] << std::endl;
    spectralResponse->Load(trainingFiles[i], 100.0);

    // Compute Reduce Spectral Response
    reduceSpectralResponse->SetInputSatRSR(satRSR);
    reduceSpectralResponse->SetInputSpectralResponse(spectralResponse);
    reduceSpectralResponse->CalculateResponse();

    atmosphericEffectsFilter->SetAtmosphericRadiativeTerms(radiative);
    atmosphericEffectsFilter->SetInputSatRSR(satRSR);
    atmosphericEffectsFilter->SetInputSpectralResponse(reduceSpectralResponse->GetReduceResponse());
    atmosphericEffectsFilter->Process();

    // Get the response in an itk::VariableLengthVector and add it to the sample list for SVMModelEstimator
    SampleType       sample;
    TargetSampleType trainingSample;
    sample.SetSize(atmosphericEffectsFilter->GetCorrectedSpectralResponse()->Size());
    std::cout << "corrected response : [";
    for (unsigned int j = 0; j < atmosphericEffectsFilter->GetCorrectedSpectralResponse()->Size(); ++j)
    {
      sample[j] = atmosphericEffectsFilter->GetCorrectedSpectralResponse()->GetResponse()[j].second;
      std::cout << atmosphericEffectsFilter->GetCorrectedSpectralResponse()->GetResponse()[j].second << " ";
    }
    std::cout << "]" << std::endl;
    sampleList->PushBack(sample);
    trainingSample = trainingClasses[i];
    std::cout << "training class : " << trainingSample << std::endl;
    trainingList->PushBack(trainingSample);
  }

  // SVM model estimator
  SVMType::Pointer classifier = SVMType::New();
  classifier->SetInputListSample(sampleList);
  classifier->SetTargetListSample(trainingList);
  classifier->SetDoProbabilityEstimates(true);
  classifier->Train();
  classifier->Save("model.txt");

  // compute spectral response for testing files
  sampleList->Clear(); // clear the sample list to re use it for testing samples
  TargetSampleListType::Pointer groundTruthClassList = TargetSampleListType::New();
  for (unsigned int i = 0; i < testingFiles.size(); ++i)
  {
    SpectralResponsePointerType         spectralResponse         = SpectralResponseType::New();
    ReduceSpectralResponseType::Pointer reduceSpectralResponse   = ReduceSpectralResponseType::New();
    AtmosphericEffectsType::Pointer     atmosphericEffectsFilter = AtmosphericEffectsType::New();

    std::cout << "testing file : " << testingFiles[i] << std::endl;
    spectralResponse->Load(testingFiles[i], 100.0);

    // Compute Reduce Spectral Response
    reduceSpectralResponse->SetInputSatRSR(satRSR);
    reduceSpectralResponse->SetInputSpectralResponse(spectralResponse);
    reduceSpectralResponse->CalculateResponse();

    atmosphericEffectsFilter->SetAtmosphericRadiativeTerms(radiative);
    atmosphericEffectsFilter->SetInputSatRSR(satRSR);
    atmosphericEffectsFilter->SetInputSpectralResponse(reduceSpectralResponse->GetReduceResponse());
    atmosphericEffectsFilter->Process();

    // Get the response in an itk::VariableLengthVector and add it to the sample list for SVMClassifier
    SampleType       sample;
    TargetSampleType gtClass;
    sample.SetSize(atmosphericEffectsFilter->GetCorrectedSpectralResponse()->Size());
    for (unsigned int j = 0; j < atmosphericEffectsFilter->GetCorrectedSpectralResponse()->Size(); ++j)
    {
      sample[j] = atmosphericEffectsFilter->GetCorrectedSpectralResponse()->GetResponse()[j].second;
    }
    sampleList->PushBack(sample);
    gtClass = testingGTClasses[i];
    groundTruthClassList->PushBack(gtClass);
  }

  // SVM Classifier
  TargetSampleListType::Pointer classifierListLabel = classifier->PredictBatch(sampleList);

  for (unsigned int i = 0; i < testingFiles.size(); ++i)
  {
    std::cout << "ground truth class : " << testingGTClasses[i] << std::endl;
  }

  // Compute confusion matrix
  ConfusionMatrixCalculatorType::Pointer confMatCalc = ConfusionMatrixCalculatorType::New();
  confMatCalc->SetReferenceLabels(groundTruthClassList);
  confMatCalc->SetProducedLabels(classifierListLabel);
  confMatCalc->Compute();

  std::cout << std::endl;
  std::cout << "Confusion matrix: " << std::endl << confMatCalc->GetConfusionMatrix() << std::endl << std::endl;
  std::cout << "Kappa Index: " << std::endl << confMatCalc->GetKappaIndex() << std::endl << std::endl;

  return EXIT_SUCCESS;
}
