/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   11 janvier 2005
  Version   :   
  Role      :   Test l'extraction d'une ROI dans une image mono canal, dont les valeurs sont codées en "unsigned char"
  $Id$

=========================================================================*/

#include <iostream>

#include "itkExceptionObject.h"
#include "otbCommandLineArgumentParser.h"

// Test de sortie en erreur
int otbTestCommandLineArgumentParserHelp( int argc, char ** argv )
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

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
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


