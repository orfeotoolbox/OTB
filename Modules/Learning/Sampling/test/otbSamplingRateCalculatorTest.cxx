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

#include "otbStatisticsXMLFileReader.h"
#include "itkVariableLengthVector.h"

int otbSamplingRateCalculatorNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::SamplingRateCalculator RateCalculatorype;
  
  RateCalculatorype::Pointer rateCalculator = RateCalculatorype::New();
  std::cout << rateCalculator << std::endl;
  return EXIT_SUCCESS;
}

int otbSamplingRateCalculator(int itkNotUsed(argc), char* argv[])
{
  std::string outputRatesTxt(argv[1]);

  typedef otb::SamplingRateCalculator::ClassCountMapType     ClassCountMapType;

  ClassCountMapType classCount;
  classCount[std::string("1")] = 104;
  classCount[std::string("2")] = 160;
  classCount[std::string("3")] = 211;

  unsigned int nbSamples = 83;

  typedef otb::SamplingRateCalculator RateCalculatorype;
  RateCalculatorype::Pointer rateCalculator = RateCalculatorype::New();
  rateCalculator->SetClassCount(classCount);
  rateCalculator->SetNbOfSamplesAllClasses(nbSamples);
  rateCalculator->Write(outputRatesTxt);
  rateCalculator->Print(std::cout);
  
  // Read back
  RateCalculatorype::Pointer rateCalculatorCheck = RateCalculatorype::New();
  rateCalculatorCheck->Read(outputRatesTxt);
  if ( rateCalculator->GetRatesByClass() != rateCalculatorCheck->GetRatesByClass() )
    {
    std::cout << "Imported rates by class are different from exported ones!" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
