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

#include "itkExceptionObject.h"
#include "otbCommandLineArgumentParser.h"

// Test de sortie en erreur
int otbTestCommandLineArgumentParserHelp( int argc, char * argv[] )
{
  try 
    { 
        // Parse command line parameters
        typedef otb::CommandLineArgumentParser ParserType;
	ParserType::Pointer parser = ParserType::New();
  
        parser->AddOption("-help","Help","-h",0,false);
        parser->AddOption("-image","Nom d'une image","-i",1,true);
  
        typedef otb::CommandLineArgumentParseResult ParserResultType;
        ParserResultType::Pointer  parseResult = ParserResultType::New();

        parser->ParseCommandLine(argc,argv,parseResult) ;


        std::cout << "Image : "<<parseResult->GetStringParameter("-image")<<std::endl;

    } 

  catch(itk::ExceptionObject &err) 
    { 
    std::cout << "Exception itk::ExceptionObject levee ! Cas nominal" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_SUCCESS;
     } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 


  return EXIT_FAILURE;
}


