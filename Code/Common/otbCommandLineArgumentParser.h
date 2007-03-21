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

#include "itkProcessObject.h"
#include "otbMacro.h"

namespace otb
{


//class CommandLineArgumentParser;

/**
 * \class CommandLineArgumentParseResult
 * \brief Object returned by CommandLineArgumentParser
 * \see CommandLineArgumentParser
 */
class ITK_EXPORT CommandLineArgumentParseResult : public itk::ProcessObject
{
public:
  typedef CommandLineArgumentParseResult      	Self;
  typedef itk::ProcessObject                	Superclass;
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

#define otbGetParameterMacro(name,type)                                                 \
  virtual type GetParameter##name (const char *option, unsigned int number=0) const     \
  {                                                                                     \
        std::string parameter = this->GetParameterString(option, number);               \
        type lValeur;                                                                   \
        ::otb::StringStream flux;                                                       \
        flux << parameter;                                                              \
        flux >> lValeur;                                                                \
        return lValeur;                                                                 \
  }
  
  otbGetParameterMacro(Short,short);
  otbGetParameterMacro(UShort,unsigned short);
  otbGetParameterMacro(Int,int);
  otbGetParameterMacro(UInt,unsigned int);
  otbGetParameterMacro(Long,long);
  otbGetParameterMacro(ULong,unsigned long);
  otbGetParameterMacro(Float,float);
  otbGetParameterMacro(Double,double);


  std::string           GetParameterString(const char *option, unsigned int number=0) const;

  std::string           GetInputImage(void) const;
  std::string           GetOutputImage(void) const;


protected:
  CommandLineArgumentParseResult(){};
  ~CommandLineArgumentParseResult(){};

private:

  template< typename TypeValeur >
  TypeValeur GetParameter(const char *option, unsigned int number=0) const;

  typedef std::vector< std::string > ParameterArrayType;
  typedef std::map< std::string, ParameterArrayType > OptionMapType;

  void Clear();
  void AddOption(const std::string &option);
  void AddParameter(const std::string &option, const std::string &parameter);

  OptionMapType m_OptionMap;

  friend class CommandLineArgumentParser;
};

/**
 * \class CommandLineArgumentParser
 * \brief Utility to Parse command line argument.
 * Usage:
 *   - Initialize the parser:
 * \code
 *    // Initialize the parser
 *    CommandLineArgumentParser parser;
 *    parser.AddOption("-f",1);
 *    parser.AddSynonim("-f","--filename");
 *    parser.AddOption("-v",0);
 *    parser.AddSynonim("-v","--verbose");
 * \endcode
 *  - Use the parser:
 * \code
 *    CommandLineArgumentParseResult result;
 *    if(parser.TryParseCommandLine(argc,argv,result))
 *       if(result.IsOptionPresent("-f"))
 *         {
 *           cout << "Filename " << result.GetOptionParameter("-f") << endl;    
 *           ...
 *         }
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

  /** Add an input image option */ 
  void AddInputImage(void);
  /** Add an output image option */ 
  void AddOutputImage(void);
  
  /** Add an option with 0 or more parameters (words that follow it) */
//  void AddOption(const char *name, const int nParameters, const char * comment);
  // at least one value

  void AddOption(const char *name, const  char * comment, char *synonim = NULL, int nParameters = 1, bool obligatory =true);
  // if -1 we do not know the number of parameters
  void AddOptionNParams(const char *name, const char * comment, char *synonim = NULL, bool obligatory =true);
  
  /** Add a different string that envokes the same option (--file and -f) */  
//  void AddSynonim(const char *option, const char *synonim);

  void ParseCommandLine(int argc, char *argv[], 
                           CommandLineArgumentParseResult * outResult,
                           bool failOnUnknownTrailingParameters = true);

protected:
 CommandLineArgumentParser();
 ~CommandLineArgumentParser(){};
 
private:

  void PrintUsage(std::ostream& os) const;
  void PrintVersion(std::ostream& os) const;
  bool FindOption(const std::string & , int & index);

  /** Try processing a command line.  Returns false if something breaks */
  bool TryParseCommandLine(int argc, char *argv[], 
                           CommandLineArgumentParseResult * outResult,
                           bool reportFailedMsg,
                           bool failOnUnknownTrailingParameters );

  typedef struct 
    {
    std::string CommonName;             // option name
    std::string Description;            // option description
    std::string Synonim;                // shortcut
    bool NumberOfParametersFixed;       // required number of values
      int NumberOfParameters;	        // number of values
    bool Obligatory;                    // is the option mandatory ?
    bool Finded;                        // check if the option is present
    } OptionType;
  typedef std::vector< OptionType> ListOptionType;
  
  ListOptionType m_OptionList;


  std::string m_ProgramName;
};

}

#endif // __otbCommandLineArgumentParser_h_
