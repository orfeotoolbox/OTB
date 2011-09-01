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

//#include <string>
#include "otbMacro.h"
#include "itkObject.h"
#include "itkObjectFactory.h"

//#include "otbWrapperTypes.h"
//#include "otbWrapperParameterGroup.h"
#include "otbWrapperApplication.h"
#include "itksys/SystemTools.hxx"

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
  typedef enum { OK, EMPTYEXPRESSION, NOMODULENAME, MULTIPLEMODULENAME, INVALIDMODULENAME, WRONGMODULENAME,
                 NOMODULEPATH, INVALIDMODULEPATH,
                 MISSINGMANDATORYATTRIBUT } ParseResultType;
  
  /** Check expression validity */
  ParseResultType CheckExpression( const std::string & exp );
  ParseResultType CheckExpression( const char * exp );

  bool CheckApplicationArgument( const std::string & exp );
  bool CheckApplicationArgument( const char * exp );

  std::vector<itksys::String> GetAttribut( const std::string & key, const std::string & exp );

protected:
  /** Constructor */
  CommandLineParser();

  /** Destructor */
  virtual ~CommandLineParser();



  bool LoadApplication( const std::string & moduleName );


  ParseResultType CheckModuleName( const char * exp );

  ParseResultType CheckPath( const char * exp );

  void LoadPath();

  std::string ExtractApplicationArgument( const char * exp );

private:

  CommandLineParser(const CommandLineParser &); //purposely not implemented
  void operator =(const CommandLineParser&); //purposely not implemented
  
  std::string m_Path;
  std::string m_ModuleName;
  const std::string m_ModuleNameKey;
  const std::string m_PathKey;

  Application::Pointer m_Application;
}; //end class

} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperCommandLineParser_h_
