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
 * ModuleName --attribut1_Key attrubut1_Value --attribut2_Key attrubut2_Value
 * After the attribut key, if the user give several values (expression without \"--\" separated by space), it will automacally be interpreted as a list.
 * The module name can be set as the first element of the expression or in the expression with the key --moduleName.
 * The exe path have to be set in with the option --modulePath.
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
  itkTypeMacro(CommandLineParser,itk::Object);
  
  /** Parse result enum */
  typedef enum { OK, NOMODULENAME, MULTIPLEMODULENAME, INVALIDMODULENAME,
                 NOMODULEPATH, INVALIDMODULEPATH} ParseResultType;
                  

  std::vector<std::string> GetAttribut( const std::string & key, const std::string & exp );

  CommandLineParser::ParseResultType GetPaths( std::vector<std::string> paths, const std::string & exp );
  CommandLineParser::ParseResultType GetModuleName( std::string modName, const std::string & exp );

  itkGetStringMacro(ModuleNameKey);
  itkGetStringMacro(PathKey);
  itkSetStringMacro(ModuleNameKey);
  itkSetStringMacro(PathKey);

protected:
  /** Constructor */
  CommandLineParser();

  /** Destructor */
  virtual ~CommandLineParser();
 
private:

  CommandLineParser(const CommandLineParser &); //purposely not implemented
  void operator =(const CommandLineParser&); //purposely not implemented
  
  std::string m_ModuleNameKey;
  std::string m_PathKey;

}; //end class

} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperCommandLineParser_h_
