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
  std::string xmlPath(argv[1]);
  unsigned int nbSamples = atoi(argv[2]);
  std::string outputRatesTxt(argv[3]);

  typedef std::map<std::string, unsigned long>      ClassCountMapType;
  typedef itk::VariableLengthVector<float> MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType> ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(xmlPath.c_str());
  ClassCountMapType classCount = reader->GetStatisticMapByName<ClassCountMapType>("classCounts");

  typedef otb::SamplingRateCalculator RateCalculatorype;
  RateCalculatorype::Pointer rateCalculator = RateCalculatorype::New();
  rateCalculator->setNbofSamplesAllClasses(nbSamples);
  rateCalculator->write(outputRatesTxt);
  rateCalculator->Print(std::cout);
  
  return EXIT_SUCCESS;
}
