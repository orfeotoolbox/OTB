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

#include "otbWrapperCommandLineParser.h"

typedef otb::Wrapper::CommandLineParser ParserType;

int otbWrapperCommandLineParserNew(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  ParserType::Pointer parser = ParserType::New();

  return EXIT_SUCCESS;
}

int CheckReturn(ParserType::ParseResultType res)
{
 if( res == ParserType::OK )
    {
      return EXIT_SUCCESS;
    }
   if( res == ParserType::NOMODULENAME )
    {
      std::cout<<"No module name detected."<<std::endl;
      return EXIT_FAILURE;
    }
  else if( res == ParserType::MULTIPLEMODULENAME )
    {
      std::cout<<"Multiple module name detected. Only one possible."<<std::endl;
      return EXIT_FAILURE;
    }
  else if( res == ParserType::INVALIDMODULENAME )
    {
      std::cout<<"Invalid module name. Must only contain alphanumerical characters."<<std::endl;
      return EXIT_FAILURE;
    }
 else if( res == ParserType::INVALIDMODULEPATH )
    {
      std::cout<<"Invalid module path. At least one path doesn't exists."<<std::endl;
      return EXIT_FAILURE;
    }
 else if( res == ParserType::NOMODULEPATH )
    {
      std::cout<<"No module path specified."<<std::endl;
      return EXIT_FAILURE;
    }
 else
    {
      std::cout<<"Invalid return code "<<res<<"."<<std::endl;
      return EXIT_FAILURE;
    }
}


int otbWrapperCommandLineParserTest1(int argc, char* argv[])
{
  ParserType::Pointer parser = ParserType::New();
  std::vector<std::string> cmdVector;
  for (int i=2; i<argc; ++i)
    {
    cmdVector.push_back(std::string(argv[i]));
    }

  std::vector<std::string> res = parser->GetAttribut( argv[1], cmdVector );

  if( res.size() == 0 )
    {
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
 }


int otbWrapperCommandLineParserTest2(int argc, char* argv[])
{
  ParserType::Pointer parser = ParserType::New();

  std::cout << "Search for paths..." << std::endl;

  std::vector<std::string> cmdVector;
  if (argc > 2)
    {
    for (int i=1; i<argc; ++i)
      {
      cmdVector.push_back(std::string(argv[i]));
      }
    }

  std::vector<std::string> paths;
  ParserType::ParseResultType res = parser->GetPaths(paths, cmdVector);
  if (CheckReturn(res) != ParserType::OK)
    {
    std::cout << "Can't find paths." << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Paths found." << std::endl;
  std::cout << "Search for module name..." << std::endl;

  std::string name;
  res = parser->GetModuleName(name, cmdVector);

  if (CheckReturn(res) != ParserType::OK)
    {
    std::cout << "Can't find module name." << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Module name found." << std::endl;
  return CheckReturn(res);
}

int otbWrapperCommandLineParserTest3(int argc, char* argv[])
{
  ParserType::Pointer parser = ParserType::New();
  std::vector<std::string> cmdVector;
  if (argc > 2)
    {
    for (int i=2; i<argc; ++i)
      {
      cmdVector.push_back(std::string(argv[i]));
      }
    }

  if (parser->IsAttributExists(argv[1], cmdVector))
    {
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}


int otbWrapperCommandLineParserTest4(int argc, char* argv[])
{
  ParserType::Pointer parser = ParserType::New();
  std::vector<std::string> cmdVector;
  if (argc > 2)
    {
    for (int i=2; i<argc; ++i)
      {
      cmdVector.push_back(std::string(argv[i]));
      }
    }

  if (parser->IsAttributExists(argv[1], cmdVector))
    {
    return EXIT_SUCCESS;
    }
  return EXIT_FAILURE;
}
