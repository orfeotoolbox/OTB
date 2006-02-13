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

int otbTestCommandLineArgumentParser( int argc, char ** argv )
{
  try 
    { 
        // Parse command line parameters
        otb::CommandLineArgumentParser parser;
  
        parser.AddOption("-image",1,"Nom d'une image");
        parser.AddOption("-entier",1,"Valeur entiere");
        parser.AddSynonim("-entier","-e");
        parser.AddOption("-deuxentiers", 2,"Liste de deux entiers");
        parser.AddOption("-double", 1,"Valeur réelle double");
        parser.AddSynonim("-double", "-d");
        parser.AddOption("-float",1,"Valeur réelle double");
        parser.AddSynonim("-float","-f");
        parser.AddOption("-help",0,"");
        parser.AddSynonim("-help","-h");
  
        otb::CommandLineArgumentParseResult parseResult;
  
        if(!parser.TryParseCommandLine(argc,argv,parseResult) || parseResult.IsOptionPresent("-help"))
        {
                parser.PrintUsage(std::cout);
                return EXIT_FAILURE;
        }
        
        parseResult.PrintSelf(std::cout);
        
    } 

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 


  return EXIT_SUCCESS;
}


