/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#include "otbSamplingRateCalculatorList.h"

#include "otbStatisticsXMLFileReader.h"
#include "itkVariableLengthVector.h"
#include <fstream>


int otbSamplingRateCalculatorList(int itkNotUsed(argc), char* argv[])
{
  std::ofstream file(argv[1], std::ios::out | std::ios::trunc);

  if (!file)
    {
    std::cout << " Couldn't open " << argv[1];
    return EXIT_FAILURE;
    }

  itk::Indent indent_1(1);

  typedef otb::SamplingRateCalculatorList::ClassCountMapType  ClassCountMapType;
  typedef otb::SamplingRateCalculatorList::PartitionType      PartitionType;

  PartitionType typeProportional = otb::SamplingRateCalculatorList::PROPORTIONAL;
  PartitionType typeEqual = otb::SamplingRateCalculatorList::EQUAL;
  PartitionType typeCustom = otb::SamplingRateCalculatorList::CUSTOM;

  std::string c1("1");
  std::string c2("2");
  std::string c3("3");
  std::string c4("4");

  ClassCountMapType classCount1, classCount2, classCount3;
  // input 1
  classCount1[c1] = 104;
  classCount1[c2] = 160;
  classCount1[c3] = 211;
  // input 2
  classCount2[c1] = 98;
  classCount2[c2] = 190;
  classCount2[c3] = 178;
  // input 3
  classCount3[c1] = 130;
  classCount3[c2] = 144;
  classCount3[c4] = 250;

  std::vector<unsigned long> nbSamplesCst;
  nbSamplesCst.push_back(151);

  std::vector<double> percent;
  percent.push_back(0.33);

  std::vector<unsigned long> total;
  total.push_back(300);

  std::vector<unsigned long> nbSamplesCstCustom;
  nbSamplesCstCustom.push_back(70);
  nbSamplesCstCustom.push_back(80);
  nbSamplesCstCustom.push_back(90);

  ClassCountMapType needed1;
  needed1[c1] = 140;
  needed1[c2] = 130;
  needed1[c3] = 124;

  std::vector<ClassCountMapType> nbByClass;
  nbByClass.push_back(needed1);

  ClassCountMapType needed2;
  needed2[c1] = 67;
  needed2[c2] = 55;
  needed2[c3] = 72;

  ClassCountMapType needed3;
  needed3[c1] = 77;
  needed3[c2] = 69;
  needed3[c3] = 34;

  ClassCountMapType needed4;
  needed4[c1] = 86;
  needed4[c2] = 43;
  needed4[c4] = 98;

  std::vector<ClassCountMapType> nbByClassCustom;
  nbByClassCustom.push_back(needed2);
  nbByClassCustom.push_back(needed3);
  nbByClassCustom.push_back(needed4);

  typedef otb::SamplingRateCalculatorList RateCalculatorListType;
  RateCalculatorListType::Pointer rateCalcList = RateCalculatorListType::New();
  rateCalcList->SetNthClassCount(0,classCount1);
  rateCalcList->SetNthClassCount(2,classCount3);
  rateCalcList->SetNthClassCount(1,classCount2);

  file << "# Test the strategy : smallest - equal" << std::endl;
  rateCalcList->SetMinimumNbOfSamplesByClass(typeEqual);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file << "# Test the strategy : smallest - custom" << std::endl;
  rateCalcList->SetMinimumNbOfSamplesByClass(typeCustom);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file << "# Test the strategy : constant - proportional" << std::endl;
  rateCalcList->SetNbOfSamplesAllClasses(nbSamplesCst,typeProportional);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file << "# Test the strategy : constant - equal" << std::endl;
  rateCalcList->SetNbOfSamplesAllClasses(nbSamplesCst,typeEqual);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file << "# Test the strategy : constant - custom" << std::endl;
  rateCalcList->SetNbOfSamplesAllClasses(nbSamplesCstCustom,typeCustom);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file << "# Test the strategy : byClass - proportional" << std::endl;
  rateCalcList->SetNbOfSamplesByClass(nbByClass,typeProportional);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file << "# Test the strategy : byClass - equal" << std::endl;
  rateCalcList->SetNbOfSamplesByClass(nbByClass,typeEqual);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file << "# Test the strategy : byClass - custom" << std::endl;
  rateCalcList->SetNbOfSamplesByClass(nbByClassCustom,typeCustom);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file << "# Test the strategy : all - equal" << std::endl;
  rateCalcList->SetAllSamples(typeEqual);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file <<"#Test the strategy : percent - proportional"<<std::endl;
  rateCalcList->SetPercentageOfSamples(percent,typeProportional);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }
  
  file <<"#Test the strategy : percent - equal"<<std::endl;
  rateCalcList->SetPercentageOfSamples(percent,typeEqual);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }

  file <<"#Test the strategy : total - proportional"<<std::endl;
  rateCalcList->SetTotalNumberOfSamples(total,typeProportional);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }
  
  file <<"#Test the strategy : total - equal"<<std::endl;
  rateCalcList->SetTotalNumberOfSamples(total,typeEqual);
  for (unsigned int i=0 ; i<rateCalcList->Size() ; i++)
    {
    file << "# Input "<< i << std::endl;
    rateCalcList->GetNthElement(i)->Print(file, indent_1);
    }
  
  file.close();
  return EXIT_SUCCESS;
}
