/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbCommandLineArgumentParser_h
#define otbCommandLineArgumentParser_h

#include <vector>
#include <string>
#include <map>

#include "itkProcessObject.h"

/** \class CommandLineArgumentParserHelpException
 * \brief This exception is thrown when the help menu is displayed.
 *
 * \ingroup OTBCommandLineParser
 */
class ITK_EXPORT CommandLineArgumentParserHelpException
  : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(CommandLineArgumentParserHelpException, ExceptionObject);

  /** Constructor. */
  CommandLineArgumentParserHelpException(const char *file, unsigned int line,
                                         const char* message = "Help:",
                                         const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
  /** Constructor. */
  CommandLineArgumentParserHelpException(const std::string & file, unsigned int line,
                                         const char* message = "Help:",
                                         const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
};

/** \class CommandLineArgumentParserVersionException
 * \brief This exception is thrown when the version is displayed.
 *
 * \ingroup OTBCommandLineParser
 */
class ITK_EXPORT CommandLineArgumentParserVersionException
  : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(CommandLineArgumentParserVersionException, ExceptionObject);

  /** Constructor. */
  CommandLineArgumentParserVersionException(const char *file, unsigned int line,
                                            const char* message = "Version:",
                                            const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
  /** Constructor. */
  CommandLineArgumentParserVersionException(const std::string & file, unsigned int line,
                                            const char* message = "Version:",
                                            const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
};

/** \class CommandLineArgumentParserArgumentErrorException
 * \brief This exception is thrown when the version is displayed.
 *
 * \ingroup OTBCommandLineParser
 */
class ITK_EXPORT CommandLineArgumentParserArgumentErrorException
  : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro(CommandLineArgumentParserArgumentErrorException, ExceptionObject);

  /** Constructor. */
  CommandLineArgumentParserArgumentErrorException(const char *file, unsigned int line,
                                                  const char* message = "Argument error:",
                                                  const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
  /** Constructor. */
  CommandLineArgumentParserArgumentErrorException(const std::string & file, unsigned int line,
                                                  const char* message = "Argument error:",
                                                  const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc) {}
};

namespace otb
{

//class CommandLineArgumentParser;

/**
 * \class CommandLineArgumentParseResult
 * \brief Object returned by CommandLineArgumentParser
 *
 * \see CommandLineArgumentParser
 *
 * \ingroup OTBCommandLineParser
 */
class ITK_EXPORT CommandLineArgumentParseResult : public itk::ProcessObject
{
public:
  typedef CommandLineArgumentParseResult Self;
  typedef itk::ProcessObject             Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(CommandLineArgumentParseResult, itk::ProcessObject);

  /** Check whether the option was passed in or not */
  bool IsOptionPresent(const std::string& option) const;

  /** Check whether the input image option was passed in or not */
  bool IsOptionInputImagePresent(void) const;

  /** Check whether the output image option was passed in or not */
  bool IsOptionOutputImagePresent(void) const;

  /** Check whether the OTBTesting option was passed in or not */
  bool IsOptionOTBTestingPresent(void) const;

  /** Get one of the parameters to the option */
//  const char *GetOptionParameter(const char *option, unsigned int number = 0);
  int GetNumberOfParameters(const std::string& option);

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

#define otbGetParameterMacro(name, type)                                                 \
  virtual type GetParameter ## name (const std::string& option, unsigned int number = 0) const     \
  {                                                                     \
    std::string parameter = this->GetParameterString(option, number);   \
    type        lValeur;                                                \
    std::stringstream flux;                                             \
    flux << parameter;                                                  \
    flux >> lValeur;                                                    \
    return lValeur;                                                     \
  }

  otbGetParameterMacro(Char, char);
  otbGetParameterMacro(Short, short);
  otbGetParameterMacro(UShort, unsigned short);
  otbGetParameterMacro(Int, int);
  otbGetParameterMacro(UInt, unsigned int);
  otbGetParameterMacro(Long, long);
  otbGetParameterMacro(ULong, unsigned long);
  otbGetParameterMacro(Float, float);
  otbGetParameterMacro(Double, double);

  std::string GetParameterString(const std::string& option, unsigned int number = 0) const;

  std::string GetInputImage(void) const;
  std::string GetOutputImage(void) const;

protected:
  CommandLineArgumentParseResult();
  ~CommandLineArgumentParseResult() override;

private:
  template<typename TypeValeur>
  TypeValeur GetParameter(const std::string& option, unsigned int number = 0) const;

  typedef std::vector<std::string>                  ParameterArrayType;
  typedef std::map<std::string, ParameterArrayType> OptionMapType;

  void Clear();
  void AddOption(const std::string & option);
  void AddParameter(const std::string & option, const std::string & parameter);

  OptionMapType m_OptionMap;

  friend class CommandLineArgumentParser;
};

/**
 * \class CommandLineArgumentParser
 * \brief Utility to Parse command line argument.
 *
 * Usage:
 *  - Initialize the parser:
 * \code
 *    CommandLineArgumentParser parser;
 *    parser.SetProgramDescription("This program is an example");
 *    parser.AddInputImage();
 *    parser.AddOption("--DetailedName","Help explanation","-d", 1, false);
 * \endcode
 *  - Use the parser:
 * \code
 *    CommandLineArgumentParseResult result;
 *    if( parser.ParseCommandLine(argc, argv, &result) )
 *    {
 *      if( result.IsOptionPresent("--DetailledName") )
 *      {
 *        std::cout << "DetailedName : " << result.GetParameterString("--DetailedName") << std::endl;
 *        ...
 *      }
 *    }
 * \endcode
 *
 * \ingroup OTBCommandLineParser
 */
class ITK_EXPORT CommandLineArgumentParser : public itk::ProcessObject
{
public:
  typedef CommandLineArgumentParser     Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(CommandLineArgumentParser, itk::ProcessObject);

  /** Add an input image option */
  void AddInputImage(bool obligatory = true);
  /** Add an output image option */
  void AddOutputImage(bool obligatory = true);

  /** Set the program name */
  itkSetStringMacro(ProgramDescription);

  /** Get the program name */
  itkGetStringMacro(ProgramDescription);

  /** Add a new option with fixed number of parameters */
  void AddOption(const std::string& name,
                 const std::string& comment,
                 const std::string& synonym = nullptr,
                 int nParameters = 1,
                 bool obligatory = true);

  /** Add a new option with unknown number of parameters */
  void AddOptionNParams(const std::string& name, const std::string& comment, const std::string& synonym = nullptr, bool obligatory = true);

  /** Interpret options from the command line */
  void ParseCommandLine(int argc, char *argv[],
                        CommandLineArgumentParseResult * outResult,
                        bool failOnUnknownTrailingParameters = true);

protected:
  CommandLineArgumentParser();
  ~CommandLineArgumentParser() override;

private:

  void PrintUsage(std::ostream& os) const;
  void PrintVersion(std::ostream& os) const;
  bool FindOption(const std::string&, int& index);
  bool IsNumber(const std::string& text);

  /** Try processing a command line.  Returns false if something breaks */
  bool TryParseCommandLine(int argc, char *argv[],
                           CommandLineArgumentParseResult * outResult,
                           bool reportFailedMsg,
                           bool failOnUnknownTrailingParameters);

  typedef struct
  {
    std::string CommonName;             // option name
    std::string Description;            // option description
    std::string Synonym;                // shortcut
    bool NumberOfParametersFixed;       // required number of values
    int NumberOfParameters;             // number of values
    bool Obligatory;                    // is the option mandatory ?
    bool Finded;                        // check if the option is present
  } OptionType;

  typedef std::vector<OptionType> ListOptionType;

  ListOptionType m_OptionList;

  std::string m_ProgramName;
  std::string m_ProgramDescription;
};

}

#endif // otbCommandLineArgumentParser_h_
