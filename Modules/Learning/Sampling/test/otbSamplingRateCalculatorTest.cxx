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

#include "otbSamplingRateCalculator.h"

int otbSamplingRateCalculatorNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::SamplingRateCalculator RateCalculatorype;
  
  RateCalculatorype::Pointer rateCalculator = RateCalculatorype::New();
  std::cout << rateCalculator << std::endl;
  return EXIT_SUCCESS;
}


int otbSamplingRateCalculatorUI(int argc, char* argv[])
{
  unsigned int nbSamples = atoi(argv[1]);

  typedef otb::SamplingRateCalculator RateCalculatorype;
  
  RateCalculatorype::Pointer rateCalculator = RateCalculatorype::New();
  rateCalculator->produceMap(2,3,1000);
  rateCalculator->setNbofSamplesAllClasses(nbSamples);
  rateCalculator->write(argv[2]);
  rateCalculator->Print(std::cout);
  return EXIT_SUCCESS;
}

int otbSamplingRateCalculatorS(int argc, char* argv[])
{
  std::string nbSamples(argv[1]);

  typedef otb::SamplingRateCalculator RateCalculatorype;
  
  RateCalculatorype::Pointer rateCalculator = RateCalculatorype::New();
  rateCalculator->produceMap(2,3,1000);
  rateCalculator->setNbofSamplesAllClasses(nbSamples);
  rateCalculator->write(argv[2]);
  rateCalculator->Print(std::cout);
  return EXIT_SUCCESS;
}


int otbSamplingRateCalculatorSmallest(int argc, char* argv[])
{
  typedef otb::SamplingRateCalculator RateCalculatorype;
  
  RateCalculatorype::Pointer rateCalculator = RateCalculatorype::New();
  rateCalculator->produceMap(2,3,1000);
  rateCalculator->setMinimumNbofSamplesByClass();
  rateCalculator->write(argv[1]);
  rateCalculator->Print(std::cout);
  return EXIT_SUCCESS;
}


int otbSamplingRateCalculatorSmallest2(int argc, char* argv[])
{
  typedef otb::SamplingRateCalculator RateCalculatorype;
  
  RateCalculatorype::Pointer rateCalculator = RateCalculatorype::New();
  rateCalculator->produceMap();
  rateCalculator->setMinimumNbofSamplesByClass();
  std::map<std::string, double>  ratesbyClass = rateCalculator->GetRatesbyClass();
  rateCalculator->write(argv[1]);
  rateCalculator->Print(std::cout);
  return EXIT_SUCCESS;
}
