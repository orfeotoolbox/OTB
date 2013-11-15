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

#include <iostream>

#include "otbCommandLineArgumentParser.h"

int otbTestCommandLineArgumentParserList(int argc, char * argv[])
{
  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddOption("-image", "Nom d'une image", "-i", 1, true);
  parser->AddOption("-entier", "Une Valeur entiere (obligatoire)", "-e");
  parser->AddOption("-deuxentiers", "Deux Valeurs entieres non obligatoire", "-dede", 2, false);
  parser->AddOption("-double", "Valeur reelle double", "-d");
  parser->AddOptionNParams("-doubles", "Liste de Valeurs reelles", "-ld", false);

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer parseResult = ParserResultType::New();

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

  std::cout << "List de Double : " << parseResult->GetNumberOfParameters("-doubles") << std::endl;
  for (int i = 0; i < parseResult->GetNumberOfParameters("-doubles"); ++i)
    {
    //double value = otb::GetParameter<double>(parseResult,"-doubles", i);
    double value = parseResult->GetParameterDouble("-doubles", i);
    std::cout << "  " << value;
    }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
