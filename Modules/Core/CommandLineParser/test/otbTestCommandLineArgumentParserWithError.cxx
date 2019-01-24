/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include <iostream>

#include "itkMacro.h"
#include "otbCommandLineArgumentParser.h"

int otbTestCommandLineArgumentParserWithError(int argc, char * argv[])
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddOption("-image", "Nom d'une image", "-i", 1, true);
  parser->AddOption("-entier", "Une Valeur entiere (obligatoire)", "-e");
  parser->AddOption("-deuxentiers", "Deux Valeurs entieres non obligatoire", "-dede", 2, false);
  parser->AddOption("-double", "Valeur reelle double", "-d");

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer parseResult = ParserResultType::New();

  try
    {

    parser->ParseCommandLine(argc, argv, parseResult);

    std::cout << "Image : " << parseResult->GetParameterString("-image") << std::endl;
    //unsigned int lEntier = otb::GetParameter<unsigned int>(parseResult,"-entier");
    unsigned int lEntier = parseResult->GetParameterUInt("-entier");
    std::cout << "Entier : " << lEntier << std::endl;
    if (parseResult->IsOptionPresent("-deuxentiers"))
      {
      //unsigned int lEntierDeux = otb::GetParameter<unsigned int>(parseResult,"-deuxentiers", 1);
      unsigned int lEntierDeux = parseResult->GetParameterUInt("-deuxentiers", 1);
      std::cout << "Entier : " << lEntierDeux << std::endl;
      }
    //double lDouble = otb::GetParameter<double>(parseResult,"-double");
    double lDouble = parseResult->GetParameterDouble("-double");
    std::cout << "Double : " << lDouble << std::endl;
    }
  catch (CommandLineArgumentParserArgumentErrorException& err)
    {
    std::cerr << err;
    return EXIT_SUCCESS;
    }
  return EXIT_FAILURE;
}
