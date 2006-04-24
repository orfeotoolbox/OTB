/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   10 février 2006
  Version   :   
  Role      :   Classe permettant de géer les paramètres d'une ligne de commande d'un programme
  $Id$

=========================================================================*/
#ifndef __otbCommandLineArgumentParser_h_
#define __otbCommandLineArgumentParser_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4503 )
#endif

#include <iostream>
#include <vector>
#include <string>
#include <map>

//#include "itkIndent.h"
#include "itkProcessObject.h"

namespace otb
{


//class CommandLineArgumentParser;

/**
 * \class CommandLineArgumentParseResult
 * \brief Objet retourné par lCommandLineArgumentParser
 * \see CommandLineArgumentParser
 */
class ITK_EXPORT CommandLineArgumentParseResult : public itk::ProcessObject
{
public:
  typedef CommandLineArgumentParseResult      	Self;
  typedef itk::ProcessObject                		Superclass;
  typedef itk::SmartPointer<Self>        	Pointer;
  typedef itk::SmartPointer<const Self>  	ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(CommandLineArgumentParseResult,itk::ProcessObject);

  /** Check whether the option was passed in or not */
  bool IsOptionPresent(const char *option) const;

  /** Get one of the parameters to the option */
//  const char *GetOptionParameter(const char *option, unsigned int number = 0);
  int GetNumberOfParameters(const char *option);

  void PrintSelf(std::ostream& os/*, itk::Indent indent*/) const;


  template< typename TypeValeur >
  TypeValeur GetParameter(const char *option, unsigned int number=0)const;

  std::string GetStringParameter(const char *option, unsigned int number=0) const;

protected:
  CommandLineArgumentParseResult(){};
  ~CommandLineArgumentParseResult(){};

private:


  typedef std::vector< std::string > ParameterArrayType;
  typedef std::map< std::string, ParameterArrayType > OptionMapType;

  void Clear();
  void AddOption(const std::string &option/*, int nParms*/);
  void AddParameter(const std::string &option, const std::string &parameter);

  OptionMapType m_OptionMap;

  friend class CommandLineArgumentParser;
};

/**
 * \class CommandLineArgumentParser
 * \brief Utilisé pour parser une ligne de commande contenant des arguments et la traduit en liste de paramètres.
 * Usage:
 * \code
 *    // Initialise le parser
 *    CommandLineArgumentParser parser;
 *    parser.AddOption("-f",1);
 *    parser.AddSynonim("-f","--filename");
 *    parser.AddOption("-v",0);
 *    parser.AddSynonim("-v","--verbose");
 *
 *    // Utilise le parser
 *    CommandLineArgumentParseResult result;
 *    if(parser.TryParseCommandLine(argc,argv,result)) {
 *       if(result.IsOptionPresent("-f"))
 *          cout << "Filename " << result.GetOptionParameter("-f") << endl;
 *       ...
 *    } 
 * \endcode      
 */
class ITK_EXPORT CommandLineArgumentParser : public itk::ProcessObject
{
public: 
  typedef CommandLineArgumentParser      Self;
  typedef itk::ProcessObject                	Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(CommandLineArgumentParser,itk::ProcessObject);

  /** Add an option with 0 or more parameters (words that follow it) */
//  void AddOption(const char *name, const int nParameters, const char * comment);
  // Au moins une valeur

  void AddOption(const char *name, const  char * comment, char *synonim = NULL, int nParameters = 1, bool obligatory =true);
  // Si -1, alors on ne connait pas le nombre de parametres à l'avance.
  void AddOptionNParams(const char *name, const char * comment, char *synonim = NULL, bool obligatory =true);
  
  /** Add a different string that envokes the same option (--file and -f) */  
//  void AddSynonim(const char *option, const char *synonim);

  void ParseCommandLine(int argc, char *argv[], 
                           CommandLineArgumentParseResult * outResult,
                           bool failOnUnknownTrailingParameters = true);

protected:
 CommandLineArgumentParser(){};
 ~CommandLineArgumentParser(){};
 
private:

  void PrintUsage(std::ostream& os/*, itk::Indent indent*/) const;
  bool FindOption(const std::string & , int & index);

  /** Try processing a command line.  Returns false if something breaks */
  bool TryParseCommandLine(int argc, char *argv[], 
                           CommandLineArgumentParseResult * outResult,
                           bool failOnUnknownTrailingParameters );

  typedef struct 
    {
    std::string CommonName;             //Nom identifiant cette option
    std::string Description;            //Description de l'option
    std::string Synonim;                //Chaine synonim = raccourci
    bool NumberOfParametersFixed;       //Precise si le nombre de valeurs attendues est connu
    int NumberOfParameters;	        //Nombre de valeurs pour cette option
    bool Obligatory;                    //Precise si l'option est obligatoire
    bool Finded;                        //Precise si l'option a été trouvée dans la ligne de commande
    } OptionType;
//  typedef std::map< std::string, OptionType> OptionMapType;
  typedef std::vector< OptionType> ListOptionType;
  
  ListOptionType m_OptionList;


  std::string m_ProgramName;
};



}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCommandLineArgumentParser.txx"
#endif


#endif // __otbCommandLineArgumentParser_h_
