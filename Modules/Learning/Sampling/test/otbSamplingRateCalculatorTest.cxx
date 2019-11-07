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


#include "otbSamplingRateCalculator.h"

#include "otbStatisticsXMLFileReader.h"
#include "itkVariableLengthVector.h"


int otbSamplingRateCalculator(int itkNotUsed(argc), char* argv[])
{
  std::string outputRatesTxt(argv[1]);

  typedef otb::SamplingRateCalculator::ClassCountMapType ClassCountMapType;

  ClassCountMapType classCount;
  classCount[std::string("1")] = 104;
  classCount[std::string("2")] = 160;
  classCount[std::string("3")] = 211;

  unsigned int nbSamples = 83;

  typedef otb::SamplingRateCalculator RateCalculatorype;
  RateCalculatorype::Pointer          rateCalculator = RateCalculatorype::New();
  rateCalculator->SetClassCount(classCount);
  rateCalculator->SetNbOfSamplesAllClasses(nbSamples);
  rateCalculator->Write(outputRatesTxt);
  rateCalculator->Print(std::cout);

  // Read back
  RateCalculatorype::Pointer rateCalculatorCheck = RateCalculatorype::New();
  rateCalculatorCheck->Read(outputRatesTxt);
  if (rateCalculator->GetRatesByClass() != rateCalculatorCheck->GetRatesByClass())
  {
    std::cout << "Imported rates by class are different from exported ones!" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
