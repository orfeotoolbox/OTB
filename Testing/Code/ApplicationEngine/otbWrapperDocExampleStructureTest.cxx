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
  
  std::vector<std::string> bPath;
  bPath.push_back("binPath1");

  docStruct->SetBinPath(bPath);
  docStruct->AddBinPath("binPath2");
  
  docStruct->AddParameter( "key1", "name1" );
  docStruct->AddParameter( "key2", "name2" );

  docStruct->SetParameterValue( "key1", "val1" );
  docStruct->SetParameterValue( "key2", "val2" );

  std::string exp = docStruct->GenerateCLExample();

  if( exp.find("otbApplicationLauncherCommandLine binPath1 binPath2 -key1 val1 -key2 val2") == std::string::npos )
    {
    std::cout<<"ERROR in expression: "<<exp<<std::endl;
    std::cout<<"The returned expression is not the waited one"<<std::endl;
    return EXIT_FAILURE;
    }

  std::cout<<exp.substr( 0, exp.find_first_of(" ") ) <<std::endl;

  if( !itksys::SystemTools::FileExists( exp.substr( 0, exp.find_first_of(" ") ).c_str() ) )
    {
    std::cout<<"ERROR in expression: "<<exp<<std::endl;
    std::cout<<"The executable is not valid"<<std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
