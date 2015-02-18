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
#include "otbCommandLineArgumentParser.h"
#include "otbOSMDataToVectorDataGenerator.h"
#include "otbVectorDataFileWriter.h"


typedef otb::OSMDataToVectorDataGenerator                     FilterType;
typedef otb::VectorDataFileWriter<FilterType::VectorDataType> VectorDataFileWriterType;


int otbOSMToVectorDataGeneratorNew (int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  FilterType::Pointer filter = FilterType::New();

  return EXIT_SUCCESS;
}

int otbOSMToVectorDataGeneratorTest (int itkNotUsed(argc), char * argv[])
{
  FilterType::Pointer filter = FilterType::New();
  filter->SetFileName(argv[1]);
  filter->SetUseUrl(false);
  filter->SetNorth(43.62811);
  filter->SetSouth(43.60185);
  filter->SetWest(1.54911);
  filter->SetEast(1.59323);
  filter->Update();

  // Write the IndexTile
  VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();
  writer->SetInput(filter->GetVectorDataByName("building"));
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}

int otbOSMToVectorDataGeneratorByName (int argc, char * argv[])
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();
  parser->AddOption("--OSMFile","The osm file containig the points to extract",
                    "-osm", 1, true);

  parser->AddOption("--OutputVectorData","The output vectordata",
                    "-vd", 1, true);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
    {
    parser->ParseCommandLine(argc, argv, parseResult);
    }
  catch ( itk::ExceptionObject )
    {
    return EXIT_FAILURE;
    }

  FilterType::Pointer filter = FilterType::New();
  filter->SetFileName(parseResult->GetParameterString("--OSMFile"));
  filter->SetUseUrl(false);

  // Set the extent of the request
  filter->SetNorth(43.62811);
  filter->SetSouth(43.60185);
  filter->SetWest(1.54911);
  filter->SetEast(1.59323);

  filter->Update();

  const FilterType::VectorDataType*  v1 = filter->GetVectorDataByName("waterway");
  std::cout <<"Size of the vectordata v1 : "<< v1->Size() << std::endl;

  const FilterType::VectorDataType*  v2 = filter->GetVectorDataByName("highway");
  std::cout <<"Size of the vectordata v2 : "<< v2->Size() << std::endl;


  const FilterType::VectorDataType*  v3 = filter->GetVectorDataByName("building");
  std::cout <<"Size of the vectordata v3 : "<< v3->Size() << std::endl;

  const FilterType::VectorDataType*  v4 = filter->GetVectorDataByName("highway","motorway_link");
  std::cout <<"Size of the vectordata v4 : "<< v4->Size() << std::endl;

  // Write the VectorData
  VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();
  writer->SetFileName(parseResult->GetParameterString("--OutputVectorData"));
  writer->SetInput(v4);
  writer->Update();


  return EXIT_SUCCESS;
}
