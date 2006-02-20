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
  void AddOption(const char *name, const  char * comment, char *synonim = NULL, int nParameters = 1, bool obligatory =true);
  // Si -1, alors on ne connait pas le nombre de parametres à l'avance.
  void AddOptionNParams(const char *name, const char * comment, char *synonim = NULL, bool obligatory =true);
  
        parser.AddOption("-image","Nom d'une image","-i",1,true);
        parser.AddOption("-entier","Une Valeur entiere (obligatoire)","-e");
        parser.AddOption("-deuxentiers","Deux Valeurs entieres non obligatoire","",2,false);
        parser.AddOption("-double", "Valeur réelle double");
        parser.AddOptionNParams("-doubles", "Liste de Valeurs réelles","-ld");
  
        otb::CommandLineArgumentParseResult parseResult;
  
        parser.ParseCommandLine(argc,argv,parseResult) ;//|| parseResult.IsOptionPresent("-help"))


        std::cout << "Image : "<<parseResult.GetStringParameter("-image")<<std::endl;
        unsigned int lEntier = parseResult.GetParameter<unsigned int>("-entier");
        std::cout << "Entier : "<<lEntier<<std::endl;
        unsigned int lEntierDeux = parseResult.GetParameter<unsigned int>("-deuxentiers",1);
        std::cout << "Entier : "<<lEntier<<std::endl;
        double lDouble = parseResult.GetParameter<double>("-double");
        std::cout << "Double : "<<lDouble<<std::endl;
        std::cout << "List de Double : "<<parseResult.GetNumberOfParameters("-double")<<std::endl;
        for (int i =0 ; i<parseResult.GetNumberOfParameters("-double") ; i++)
        {
                double value = parseResult.GetParameter<double>("-double",i);
                std::cout << "  "<<value;
        }
        std::cout << std::endl;
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


