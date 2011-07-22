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

#include "itkMacro.h"
#include "otbCommandLineArgumentParser.h"

// Test de sortie en erreur
int otbTestCommandLineArgumentParserHelp(int argc, char * argv[])
{
  try
    {
    // Parse command line parameters
    typedef otb::CommandLineArgumentParser ParserType;
    ParserType::Pointer parser = ParserType::New();

    parser->AddOption("-image", "Nom d'une image", "-i", 1, true);

    typedef otb::CommandLineArgumentParseResult ParserResultType;
    ParserResultType::Pointer parseResult = ParserResultType::New();

    parser->ParseCommandLine(argc, argv, parseResult);

    std::cout << "Image : " << parseResult->GetParameterString("-image") << std::endl;
    }
  catch (CommandLineArgumentParserHelpException& err)
    {
    std::cerr << err;
    return EXIT_SUCCESS;
    }

  return EXIT_FAILURE;
}
