/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#if defined(_MSC_VER)
#pragma warning(disable : 4786)
#endif

#include "otbWrapperCommandLineParser.h"

typedef otb::Wrapper::CommandLineParser ParserType;


int CheckReturn(ParserType::ParseResultType res)
{
  if (res == ParserType::OK)
  {
    return EXIT_SUCCESS;
  }
  if (res == ParserType::NOMODULENAME)
  {
    std::cout << "No module name detected." << std::endl;
    return EXIT_FAILURE;
  }
  else if (res == ParserType::MULTIPLEMODULENAME)
  {
    std::cout << "Multiple module name detected. Only one possible." << std::endl;
    return EXIT_FAILURE;
  }
  else if (res == ParserType::INVALIDMODULENAME)
  {
    std::cout << "Invalid module name. Must only contain alphanumerical characters." << std::endl;
    return EXIT_FAILURE;
  }
  else if (res == ParserType::INVALIDMODULEPATH)
  {
    std::cout << "Invalid module path. At least one path doesn't exists." << std::endl;
    return EXIT_FAILURE;
  }
  else if (res == ParserType::NOMODULEPATH)
  {
    std::cout << "No module path specified." << std::endl;
    return EXIT_FAILURE;
  }
  else
  {
    std::cout << "Invalid return code " << res << "." << std::endl;
    return EXIT_FAILURE;
  }
}


int otbWrapperCommandLineParserTest1(int argc, char* argv[])
{
  ParserType::Pointer      parser = ParserType::New();
  std::vector<std::string> cmdVector;
  for (int i = 2; i < argc; ++i)
  {
    cmdVector.push_back(std::string(argv[i]));
  }

  std::vector<std::string> res = parser->GetAttribut(argv[1], cmdVector);

  if (res.size() == 0)
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
    for (int i = 1; i < argc; ++i)
    {
      cmdVector.push_back(std::string(argv[i]));
    }
  }

  std::vector<std::string>    paths;
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
  ParserType::Pointer      parser = ParserType::New();
  std::vector<std::string> cmdVector;
  if (argc > 2)
  {
    for (int i = 2; i < argc; ++i)
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
  ParserType::Pointer      parser = ParserType::New();
  std::vector<std::string> cmdVector;
  if (argc > 2)
  {
    for (int i = 2; i < argc; ++i)
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
