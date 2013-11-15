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

int otbTestCommandLineArgumentParser(int argc, char * argv[])
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

  parser->ParseCommandLine(argc, argv, parseResult);

  std::cout << "Image : " << parseResult->GetParameterString("-image") << std::endl;

  unsigned int lEntier = parseResult->GetParameterUInt("-entier");
  std::cout << "Entier : " << lEntier << std::endl;
  if (parseResult->IsOptionPresent("-deuxentiers"))
    {
    std::cout << "Entier : " << lEntier << std::endl;
    }
  double lDouble = parseResult->GetParameterDouble("-double");
  std::cout << "Double : " << lDouble << std::endl;

  return EXIT_SUCCESS;
}
