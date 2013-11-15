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
#ifndef __otbWrapperCommandLineParser_h
#define __otbWrapperCommandLineParser_h

#include "otbMacro.h"
#include "itkObject.h"
#include "itkObjectFactory.h"


namespace otb
{
namespace Wrapper
{

/** \class CommandLineParser
 *  \brief This class check the validity of a command line application.
 *
 * To be valid, the expression must be as follow:
 * ModuleName (ModulePaths) --attribut1_Key attrubut1_Value --attribut2_Key attrubut2_Value
 * After the attribut key, if the user give several values (expression without \"--\" separated by space), it will automacally be interpreted as a list.
 */

class ITK_EXPORT CommandLineParser : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef CommandLineParser Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(CommandLineParser, itk::Object);

  /** Parse result enum */
  typedef enum { OK, NOMODULENAME, MULTIPLEMODULENAME, INVALIDMODULENAME,
                 NOMODULEPATH, INVALIDMODULEPATH, DEFAULT} ParseResultType;

  /** Get the attribut associatd to a key (list of element after the key and before the next "--") as a vector of string. */
  std::vector<std::string> GetAttribut( const std::string & key, const std::string & exp );
  /** Get the attribut associatd to a key (list of element after the key and before the next "--") as a string separated by spaces. */
  std::string GetAttributAsString( const std::string & key, const std::string & exp );

  /** Look if a given key is in an expression. */
  bool IsAttributExists(const std::string key, const std::string & exp );

  /** Get the paths executables in an expression. Store the list in a vector of string. */
  CommandLineParser::ParseResultType GetPaths( std::vector<std::string> & paths, const std::string & exp );
  /** Get the paths executables in an expression. Store the list in a vector of string. */
  std::string GetPathsAsString( const std::string & exp );

  /** Get the module name in an expression. It can be the first element of the expression (if the expression doesn't start with a "--" or the attribut associated to the key m_ModuleNameKey). */
  CommandLineParser::ParseResultType GetModuleName( std::string & modName, const std::string & exp );

  /** Get the list of keys in an expression. That is to say each word starting by "--". */
  std::vector<std::string> GetKeyList( const std::string & exp );

protected:
  /** Constructor */
  CommandLineParser();

  /** Destructor */
  virtual ~CommandLineParser();

private:
  CommandLineParser(const CommandLineParser &); //purposely not implemented
  void operator =(const CommandLineParser&); //purposely not implemented


  /** Return true if the found key is valid : not a number */
  bool IsAValidKey( const std::string & foundKey );


}; //end class

} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperCommandLineParser_h_
