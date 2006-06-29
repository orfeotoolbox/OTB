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
#include "otbCommandLineArgumentParser.h"
#include "itkMacro.h"

#include <assert.h>
#include <iostream>

namespace otb
{


// --------- CommandLineArgumentParseResult  ----------------------------------------
void
CommandLineArgumentParseResult
::PrintSelf(std::ostream& os/*, itk::Indent indent*/)const
{


}




bool 
CommandLineArgumentParseResult
::IsOptionPresent(const char *option)const
{
  return (m_OptionMap.find(std::string(option)) != m_OptionMap.end());
}




std::string 
CommandLineArgumentParseResult
::GetStringParameter(const char *option, unsigned int number)const
{
  assert(this->IsOptionPresent(option));
  
  OptionMapType::const_iterator it = m_OptionMap.begin();
  it = m_OptionMap.find(std::string(option));
  
//  assert(number < m_OptionMap[std::string(option)].size());
ParameterArrayType pat = (*it).second;

std::string lString = pat[number];

  return ( lString );
}
int  
CommandLineArgumentParseResult
::GetNumberOfParameters(const char *option)
{
  assert(IsOptionPresent(option));
  return (m_OptionMap[std::string(option)].size());
}




void  
CommandLineArgumentParseResult
::Clear()
{
  m_OptionMap.clear();
}

void  
CommandLineArgumentParseResult
::AddOption(const std::string &option)
{
  ParameterArrayType pat;
//  pat.reserve(nParms);
  m_OptionMap[option] = pat;
}

void  
CommandLineArgumentParseResult
::AddParameter(const std::string &option, const std::string &parameter)
{
  m_OptionMap[option].push_back(parameter);  
}


// --------- CommandLineArgumentParser  ----------------------------------------
void 
CommandLineArgumentParser
::AddOption(const char *name, const  char * comment, char *synonim, int nParameters, bool obligatory )
{
  // Create a structure for the command
  OptionType option;
  option.CommonName  = std::string(name);
  option.Description = std::string(comment);
  option.Synonim     = std::string(synonim);
  option.NumberOfParameters = nParameters;
  option.NumberOfParametersFixed = true;
  option.Obligatory = obligatory;
  option.Finded = false;

  // Add the option to the map
  m_OptionList.push_back(option);
  
}

void 
CommandLineArgumentParser
::AddOptionNParams(const char *name, const char * comment, char *synonim, bool obligatory )
{
  // Create a structure for the command
  OptionType option;
  option.CommonName  = std::string(name);
  option.Description = std::string(comment);
  option.Synonim     = std::string(synonim);
  option.NumberOfParameters = -1;
  option.NumberOfParametersFixed = false;
  option.Obligatory = obligatory;
  option.Finded = false;

  // Add the option to the map
  m_OptionList.push_back(option);

	
}

void 
CommandLineArgumentParser
::ParseCommandLine(int argc, char *argv[], 
                           CommandLineArgumentParseResult * outResult,
                           bool failOnUnknownTrailingParameters ) 
{
	
	
	bool tryParse = TryParseCommandLine(argc, argv, outResult, failOnUnknownTrailingParameters);
	bool IsHelp = outResult->IsOptionPresent("-help");

	if ( (IsHelp == true) )
	{
		PrintUsage(std::cout);
		itkExceptionMacro(<<"ParseCommandLine(): Help Parser");
	}

	if ( (tryParse == false) )
  	{	
		PrintUsage(std::cerr);
		itkExceptionMacro(<<"ParseCommandLine() argument Error");
	}
}


bool 
CommandLineArgumentParser
::TryParseCommandLine(int argc, char *argv[], 
                      CommandLineArgumentParseResult * outResult,
                      bool failOnUnknownTrailingParameters)
{
  // Clear the result
  outResult->Clear();

  int i;
  m_ProgramName = std::string(argv[0]);
  int index(0);
  
  // Go through the arguments
  for(i=1; i < argc; i++)
    {
    // Get the next argument
    std::string arg(argv[i]);

    // Check if the argument is known
    bool findOption = FindOption( arg, index );
      if (findOption == false)
      {
      if(failOnUnknownTrailingParameters)
        {
        // Unknown argument found
        std::cerr << "The following '" << arg << "' option is unknown !!" << std::endl;
        return false;
        }
      else return true;
      }
    //Check the option
    m_OptionList[index].Finded = true;
    // Si le nombre de parametres est prédéfini : 
    if (m_OptionList[index].NumberOfParametersFixed == true)
    {
    	// Check if the number of parameters is correct
    	int nParameters = m_OptionList[index].NumberOfParameters;
    	if(i+nParameters >= argc) 
      	{
      		// Too few parameters
      		std::cerr << "Missing one (or more) parameter(s) for the following '" << arg << "' option." << std::endl;
      		return false;
      	}
        // Tell the result that the option has been encountered
        outResult->AddOption(m_OptionList[index].CommonName);

        // Pass in the parameters
        for(int j=0;j<nParameters;j++,i++)
        {
      		outResult->AddParameter(m_OptionList[index].CommonName,std::string(argv[i+1]));
      	}
     }
     // Si le nombre de parametres n'est pas prédéfini, lecture jusqu'a la prochaine option ou fin argv  : 
     else
     {
         // Tell the result that the option has been encountered
        outResult->AddOption(m_OptionList[index].CommonName);
    	bool continuer(true);
     	while (continuer == true )
     	{
     		if ( argv[i+1] != NULL )
     		{
     			std::string strArgv = std::string(argv[i+1]);
     			if ( strArgv[0] == '-' )
     			{
     				continuer = false;
     			}
     			else
     			{
      				outResult->AddParameter(m_OptionList[index].CommonName,strArgv);
     		i++;
     			}
     		}
     		else continuer = false;
//     		i++;
     	}
     }

    
    
    
    }

  // Controle que toutes les options obligatoire sont présentes dans la ligne d'argument
  for(i=0 ; i < m_OptionList.size() ; i++ )
  {
  	if ( (m_OptionList[i].Obligatory == true) && (m_OptionList[i].Finded == false) )
  	{
      		// Too few parameters
      		std::cerr << "'" << m_OptionList[i].CommonName << " option(s) is(are) obligatory(ies) !!!" << std::endl;
      		return false;
	}	
  }
  
  // Everything is good
  return true;
}

bool 
CommandLineArgumentParser
::FindOption(const std::string & option, int & index)
{
	//Cherche dans la liste des options installées
	bool trouve(false);
	bool continuer(true);
	int cpt(0);
	std::string strOption(option);
	while ( continuer == true )
	{
		if (  (m_OptionList[cpt].CommonName == strOption) || (m_OptionList[cpt].Synonim == strOption) )
		{
			index = cpt;
			continuer = false;
			trouve = true;	
		}
		cpt++;
		if( cpt >= m_OptionList.size() )
		{
			continuer = false;	
		}
	}
        return (trouve);
}

void
CommandLineArgumentParser
::PrintUsage(std::ostream& os/*, itk::Indent indent*/)const
{
	os << std::endl;
        os << " Usage : "<<m_ProgramName<<std::endl;
  	// Calcul de la largeur max en caractere de l'affichage des options (pour mise en page)
  	int largeurmax(-1);
	int i;

  	for(i=0 ; i < m_OptionList.size() ; i++ )
  	{
  		int largeur = m_OptionList[i].CommonName.size() + m_OptionList[i].Synonim.size();
  		if ( largeur > largeurmax ) largeurmax = largeur;
	}
  	
  	// Controle que toutes les options obligatoire sont présentes dans la ligne d'argument
  	for(i=0 ; i < m_OptionList.size() ; i++ )
  	{
  		int largeur = m_OptionList[i].CommonName.size() + m_OptionList[i].Synonim.size();
  		os << "      ";
  		if ( m_OptionList[i].Obligatory == false ) os <<"[";
  		else os << " ";
  		os << m_OptionList[i].CommonName ;
  		if (m_OptionList[i].Synonim.empty() == false )
  		{
  			os << "|"<<m_OptionList[i].Synonim;	
  		}
  		if ( m_OptionList[i].Obligatory == false ) os <<"]";
  		else os << " ";
  		//Aligne le texte avec la différence en blanc
  		for (int b=largeur ; b< largeurmax ; b++) os <<" ";
  		os <<   "  :  "<<m_OptionList[i].Description;
  		if (m_OptionList[i].NumberOfParametersFixed == true ) os << "  ("<<m_OptionList[i].NumberOfParameters<<" parameters)";
  		else os << "  (N parameters)";
  		os << std::endl;
	}
	os << std::endl;
}




}


