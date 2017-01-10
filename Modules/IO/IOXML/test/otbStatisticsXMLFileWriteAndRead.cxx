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

#include "otbStatisticsXMLFileWriter.h"
#include "otbStatisticsXMLFileReader.h"
#include "itkVariableLengthVector.h"
#include <fstream>

template <typename MapT>
void printMap(const MapT &map)
{
  for (typename MapT::const_iterator it = map.begin() ; it != map.end() ; ++it)
    {
    std::cout << "  * "<<it->first << " -> " << it->second << std::endl; 
    }
}

int otbStatisticsXMLFileWriteAndRead(int argc, char* argv [])
{
  typedef itk::VariableLengthVector<float> MeasurementType;
  typedef otb::StatisticsXMLFileReader<MeasurementType> ReaderType;
  typedef otb::StatisticsXMLFileWriter<MeasurementType> WriterType;
  
  typedef std::map<unsigned long, double> NumericMapType;
  typedef std::map<std::string, unsigned long> StrMapType;
  
  if (argc < 4)
    {
    std::cout << "Usage : "<< argv[0] << " vectors.xml  maps.xml  vectorsAndMaps.xml" << std::endl;
    return EXIT_FAILURE;
    }
  
  bool errorsFlag = false;
  
  // Output files
  std::string outPath1(argv[1]);
  std::string outPath2(argv[2]);
  std::string outPath3(argv[3]);

  // Test vectors and maps
  MeasurementType testVector1;
  testVector1.SetSize(4);
  testVector1[0] = 531.4;
  testVector1[1] = 0.151;
  testVector1[2] = -43.9;
  testVector1[3] = 1.5e-4;

  MeasurementType testVector2;
  testVector2.SetSize(3);
  testVector2[0] = -5;
  testVector2[1] = 67.3;
  testVector2[2] = -35e3;

  NumericMapType testMap1;
  testMap1[98315] = 64.7;
  testMap1[126] = -4.6;
  testMap1[5567] = 46e7;

  StrMapType testMap2;
  testMap2[std::string("water")] = 4986;
  testMap2[std::string("soil")] = 7856126;
  testMap2[std::string("forest")] = 32896;

  // Storage names
  std::string vectorName1("meanTest");
  std::string vectorName2("meanTestBis");
  std::string mapName1("polygonSize");
  std::string mapName2("classCounts");

  // Filters
  WriterType::Pointer writer = WriterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  // Test 1 : write vectors only
  writer->SetFileName(outPath1);
  writer->AddInput("meanTest",testVector1);
  writer->AddInput("meanTestBis",testVector2);
  writer->Update();
  reader->SetFileName(outPath1);
  MeasurementType vec1 = reader->GetStatisticVectorByName("meanTest");
  MeasurementType vec2 = reader->GetStatisticVectorByName("meanTestBis");
  if (testVector1 != vec1)
    {
    std::cout << "Measurement vectors have changed !" << std::endl;
    std::cout << "Before : "<< testVector1 << std::endl;
    std::cout << "After : "<< vec1 << std::endl;
    errorsFlag = true;
    }
  if (testVector2 != vec2)
    {
    std::cout << "Measurement vectors have changed !" << std::endl;
    std::cout << "Before : "<< testVector2 << std::endl;
    std::cout << "After : "<< vec2 << std::endl;
    errorsFlag = true;
    }
  
  // Test 2 : write maps only
  writer->CleanInputs();
  writer->SetFileName(outPath2);
  writer->AddInputMap<NumericMapType>("polygonSize",testMap1);
  writer->AddInputMap<StrMapType>("classCounts",testMap2);
  writer->Update();
  reader->SetFileName(outPath2);
  NumericMapType map1 = reader->GetStatisticMapByName<NumericMapType>("polygonSize");
  StrMapType map2 = reader->GetStatisticMapByName<StrMapType>("classCounts");
  if (testMap1 != map1)
    {
    std::cout << "Statistic maps have changed !" << std::endl;
    std::cout << "Before : " << std::endl;
    printMap<NumericMapType>(testMap1);
    std::cout << "After : " << std::endl;
    printMap<NumericMapType>(map1);
    errorsFlag = true;
    }
  if (testMap2 != map2)
    {
    std::cout << "Statistic maps have changed !" << std::endl;
    std::cout << "Before : " << std::endl;
    printMap<StrMapType>(testMap2);
    std::cout << "After : " << std::endl;
    printMap<StrMapType>(map2);
    errorsFlag = true;
    }
  
  // Test 3 : write both vectors and maps
  writer->CleanInputs();
  writer->SetFileName(outPath3);
  writer->AddInput("meanTest",testVector1);
  writer->AddInput("meanTestBis",testVector2);
  writer->AddInputMap<NumericMapType>("polygonSize",testMap1);
  writer->AddInputMap<StrMapType>("classCounts",testMap2);
  writer->Update();
  reader->SetFileName(outPath3);
  // print available names
  std::vector<std::string> vectorList = reader->GetStatisticVectorNames();
  std::vector<std::string> mapList = reader->GetStatisticMapNames();
  std::cout << "Available vectors :";
  for (unsigned int i=0; i<vectorList.size() ; ++i)
    {
    std::cout << " " << vectorList[i];
    }
  std::cout << std::endl;
  std::cout << "Available map :";
  for (unsigned int i=0; i<mapList.size() ; ++i)
    {
    std::cout << " " << mapList[i];
    }
  std::cout << std::endl;

  vec1 = reader->GetStatisticVectorByName("meanTest");
  vec2 = reader->GetStatisticVectorByName("meanTestBis");
  map1 = reader->GetStatisticMapByName<NumericMapType>("polygonSize");
  map2 = reader->GetStatisticMapByName<StrMapType>("classCounts");
  if (testVector1 != vec1)
    {
    std::cout << "Measurement vectors have changed !" << std::endl;
    std::cout << "Before : "<< testVector1 << std::endl;
    std::cout << "After : "<< vec1 << std::endl;
    errorsFlag = true;
    }
  if (testVector2 != vec2)
    {
    std::cout << "Measurement vectors have changed !" << std::endl;
    std::cout << "Before : "<< testVector2 << std::endl;
    std::cout << "After : "<< vec2 << std::endl;
    errorsFlag = true;
    }
  if (testMap1 != map1)
    {
    std::cout << "Statistic maps have changed !" << std::endl;
    std::cout << "Before : " << std::endl;
    printMap<NumericMapType>(testMap1);
    std::cout << "After : " << std::endl;
    printMap<NumericMapType>(map1);
    errorsFlag = true;
    }
  if (testMap2 != map2)
    {
    std::cout << "Statistic maps have changed !" << std::endl;
    std::cout << "Before : " << std::endl;
    printMap<StrMapType>(testMap2);
    std::cout << "After : " << std::endl;
    printMap<StrMapType>(map2);
    errorsFlag = true;
    }
  
  std::cout << writer << std::endl;
  std::cout << reader << std::endl;
  if (errorsFlag) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
