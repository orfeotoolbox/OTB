/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperCommandLineParser_h
#define otbWrapperCommandLineParser_h

#include "itkObjectFactory.h"
#include <string>


namespace otb
{
namespace Wrapper
{

/** \class CommandLineParser
 *  \brief This class check the validity of a command line application.
 *
 * To be valid, the expression must be as follow:
 * ModuleName (ModulePaths) --attribut1_Key attrubut1_Value --attribut2_Key attrubut2_Value
 * After the attribute key, if the user give several values (expression without \"--\" separated by space), it will automacally be interpreted as a list.
 *
 * \ingroup OTBCommandLine
 */

class ITK_ABI_EXPORT CommandLineParser : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CommandLineParser             Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(CommandLineParser, itk::Object);

  /** Parse result enum */
  typedef enum { OK, NOMODULENAME, MULTIPLEMODULENAME, INVALIDMODULENAME, NOMODULEPATH, INVALIDMODULEPATH, DEFAULT } ParseResultType;

  /** Get the attribute associated to a key (list of element after the key and before the next "--") as a vector of string. */
  std::vector<std::string> GetAttribute(const std::string& key, const std::string& exp);
  std::vector<std::string> GetAttribute(const std::string& key, const std::vector<std::string>& exp);
  /** Get the attribute associated to a key (list of element after the key and before the next "--") as a string separated by spaces. */
  std::string GetAttributeAsString(const std::string& key, const std::string& exp);
  std::string GetAttributeAsString(const std::string& key, const std::vector<std::string>& exp);
  /** Look if a given key is in an expression. */
  bool IsAttributeExists(const std::string &key, const std::string& exp);
  bool IsAttributeExists(const std::string &key, const std::vector<std::string>& exp);

  /** Get the paths executables in an expression. Store the list in a vector of string. */
  CommandLineParser::ParseResultType GetPaths(std::vector<std::string>& paths, const std::string& exp);
  CommandLineParser::ParseResultType GetPaths(std::vector<std::string>& paths, const std::vector<std::string>& exp);

  /** Get the paths executables in an expression. Store the list in a vector of string. */
  std::string GetPathsAsString(const std::string& exp);
  std::string GetPathsAsString(const std::vector<std::string>& vexp);


  /** Get the module name in an expression. It can be the first element of the expression (if the expression doesn't start with a "--" or the attribute
   * associated to the key m_ModuleNameKey). */
  CommandLineParser::ParseResultType GetModuleName(std::string& modName, const std::string& exp);
  CommandLineParser::ParseResultType GetModuleName(std::string& modName, const std::vector<std::string>& exp);

  /** Get the list of keys in an expression. That is to say each word starting by "--". */
  std::vector<std::string> GetKeyList(const std::string& exp);
  std::vector<std::string> GetKeyList(const std::vector<std::string>& exp);

protected:
  /** Constructor */
  CommandLineParser();

  /** Destructor */
  ~CommandLineParser() override;

private:
  CommandLineParser(const CommandLineParser&) = delete;
  void operator=(const CommandLineParser&) = delete;


  /** Return true if the found key is valid : not a number */
  bool IsAValidKey(const std::string& foundKey);


}; // end class

} // end namespace Wrapper
} // end namespace otb

#endif // otbWrapperCommandLineParser_h_
