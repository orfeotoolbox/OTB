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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperDocExampleStructure.h"
#include "itksys/SystemTools.hxx"
#include <fstream>

int otbWrapperDocExampleStructureNew(int argc, char* argv[])
{
  typedef otb::Wrapper::DocExampleStructure DocExampleStructureType;
  DocExampleStructureType::Pointer docStruct = DocExampleStructureType::New();

  return EXIT_SUCCESS;
}


int otbWrapperDocExampleStructureTest(int argc, char* argv[])
{
  typedef otb::Wrapper::DocExampleStructure DocExampleStructureType;
  DocExampleStructureType::Pointer docStruct = DocExampleStructureType::New();

  docStruct->SetApplicationName("TestApplication");

  docStruct->SetExampleComment("Exmaple1", 0);

  docStruct->AddParameter( "key1", "val1_1" );
  docStruct->AddParameter( "key2", "val1_2" );

  docStruct->AddExample("Example2");
  docStruct->AddParameter( "key2", "val2_2", 1 );
  docStruct->AddParameter( "key3", "name3" );
  docStruct->AddParameter( "key3", "val2_3", 1 );

  std::ofstream ofs( argv[1] );
  if (!ofs.is_open())
    {
    fprintf(stderr, "Error, can't open file");
    return EXIT_FAILURE;
    }
  ofs << docStruct->GenerateCLExample();
  ofs.close();

  return EXIT_SUCCESS;
}
