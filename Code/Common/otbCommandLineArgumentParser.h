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
#include <cstdlib>

#include "itkProcessObject.h"
#include "otbMacro.h"


/** \class CommandLineArgumentParserHelpException
 * \brief This exception is thrown when the help menu is displayed.
 */ 
class ITK_EXPORT CommandLineArgumentParserHelpException 
  : public itk::ExceptionObject 
{
public:
  /** Run-time information. */
  itkTypeMacro(CommandLineArgumentParserHelpException , ExceptionObject );

  /** Constructor. */
 CommandLineArgumentParserHelpException(const char *file, unsigned int line, 
                           const char* message = "Help:",
                           const char* loc = "Unknown" ) : 
    ExceptionObject(file, line, message, loc)
  {}
  /** Constructor. */
 CommandLineArgumentParserHelpException(const std::string &file, unsigned int line, 
                           const char* message = "Help:",
                           const char* loc = "Unknown" ) :
    ExceptionObject(file, line, message, loc)
  {}
};

/** \class CommandLineArgumentParserVersionException
 * \brief This exception is thrown when the version is displayed.
 */ 
class ITK_EXPORT CommandLineArgumentParserVersionException 
  : public itk::ExceptionObject 
{
public:
  /** Run-time information. */
  itkTypeMacro(CommandLineArgumentParserVersionException , ExceptionObject );

  /** Constructor. */
 CommandLineArgumentParserVersionException(const char *file, unsigned int line, 
                           const char* message = "Version:",
                           const char* loc = "Unknown" ) : 
    ExceptionObject(file, line, message, loc)
  {}
  /** Constructor. */
 CommandLineArgumentParserVersionException(const std::string &file, unsigned int line, 
                           const char* message = "Version:",
                           const char* loc = "Unknown" ) :
    ExceptionObject(file, line, message, loc)
  {}
};

/** \class CommandLineArgumentParserArgumentErrorException
 * \brief This exception is thrown when the version is displayed.
 */ 
class ITK_EXPORT CommandLineArgumentParserArgumentErrorException 
  : public itk::ExceptionObject 
{
public:
  /** Run-time information. */
  itkTypeMacro(CommandLineArgumentParserArgumentErrorException , ExceptionObject );

  /** Constructor. */
 CommandLineArgumentParserArgumentErrorException(const char *file, unsigned int line, 
                           const char* message = "Argument error:",
                           const char* loc = "Unknown" ) : 
    ExceptionObject(file, line, message, loc)
  {}
  /** Constructor. */
 CommandLineArgumentParserArgumentErrorException(const std::string &file, unsigned int line, 
                           const char* message = "Argument error:",
                           const char* loc = "Unknown" ) :
    ExceptionObject(file, line, message, loc)
  {}
};

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
  bool IsOptionPresent(std::string option) const;

  /** Check whether the input image option was passed in or not */
  bool IsOptionInputImagePresent(void) const;

  /** Check whether the output image option was passed in or not */
  bool IsOptionOutputImagePresent(void) const;
  
  /** Check whether the OTBTesting option was passed in or not */
  bool IsOptionOTBTestingPresent(void)const;

  /** Get one of the parameters to the option */
//  const char *GetOptionParameter(const char *option, unsigned int number = 0);
  int GetNumberOfParameters(std::string option);

  void PrintSelf(std::ostream& os/*, itk::Indent indent*/) const;

#define otbGetParameterMacro(name,type)                                                 \
  virtual type GetParameter##name (std::string option, unsigned int number=0) const     \
  {                                                                                     \
        std::string parameter = this->GetParameterString(option, number);               \
        type lValeur;                                                                   \
        ::otb::StringStream flux;                                                       \
        flux << parameter;                                                              \
        flux >> lValeur;                                                                \
        return lValeur;                                                                 \
  }
  
	
	otbGetParameterMacro(Char,char);
	otbGetParameterMacro(Short,short);
  otbGetParameterMacro(UShort,unsigned short);
  otbGetParameterMacro(Int,int);
  otbGetParameterMacro(UInt,unsigned int);
  otbGetParameterMacro(Long,long);
  otbGetParameterMacro(ULong,unsigned long);
  otbGetParameterMacro(Float,float);
  otbGetParameterMacro(Double,double);


  std::string           GetParameterString(std::string option, unsigned int number=0) const;

  std::string           GetInputImage(void) const;
  std::string           GetOutputImage(void) const;


protected:
  CommandLineArgumentParseResult(){};
  ~CommandLineArgumentParseResult(){};

private:

  template< typename TypeValeur >
  TypeValeur GetParameter(std::string option, unsigned int number=0) const;

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
 *  - Initialize the parser:
 * \code
 *    CommandLineArgumentParser parser;
 *    parser.AddInputImage();
 *    parser.AddOption("-f",1);
 *    parser.AddSynonim("-f","--filename");
 *    parser.AddOption("-v",0);
 *    parser.AddSynonim("-v","--verbose");
 *    parser.AddOption("--DetailledName","Help explanation","-d", 1, false);
 * \endcode
 *  - Use the parser:
 * \code
 *    CommandLineArgumentParseResult result;
 *    if(parser.TryParseCommandLine(argc,argv,result))
 *    {
 *      if(result.IsOptionPresent("--DetailledName"))
 *      {
 *        cout << "Option " << result.GetParameterString("--DetailledName") << endl;
 *        ...
 *      }
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

  /** Add an input image option */ 
  void AddInputImage(bool obligatory=true);
  /** Add an output image option */ 
  void AddOutputImage(bool obligatory=true);
  
  /** Add an option with 0 or more parameters (words that follow it) */
//  void AddOption(const char *name, const int nParameters, const char * comment);
  // at least one value

  void AddOption(std::string name, std::string comment, std::string synonim = NULL, int nParameters = 1, bool obligatory =true);
  // if -1 we do not know the number of parameters
  void AddOptionNParams(std::string name, std::string  comment, std::string synonim = NULL, bool obligatory =true);
  
  /** Add a different string that envokes the same option (--file and -f) */  
//  void AddSynonim(const char *option, const char *synonim);

  void ParseCommandLine(int argc, char *argv[], 
                           CommandLineArgumentParseResult * outResult,
                           bool failOnUnknownTrailingParameters = true);

  void ParseGUI(   CommandLineArgumentParseResult * outResult,
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
