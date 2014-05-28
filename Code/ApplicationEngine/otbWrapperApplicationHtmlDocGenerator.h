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
#ifndef __otbWrapperApplicationHtmlDocGenerator_h
#define __otbWrapperApplicationHtmlDocGenerator_h

#include "otbWrapperApplication.h"

namespace otb
{
namespace Wrapper
{
/** \class ApplicationHtmlDocGenerator
 *  \brief This class genertaes the documentation of a class usung the
 *  class doc attributes.
 */

class ITK_ABI_EXPORT ApplicationHtmlDocGenerator
{
public:
 /** Standard class typedef */
  typedef ApplicationHtmlDocGenerator   Self;

  /** Constructor */
  ApplicationHtmlDocGenerator();

  /** Destructor */
  virtual ~ApplicationHtmlDocGenerator();

  /** Generate ans dave the documentation. */
  /** Add the key is the description of the parameter if showKey is
  * set at true. */
  static void GenerateDoc( const Application::Pointer app, const std::string & filename, const bool showKey = false );
  /** Generate the documentation as a string. */
  /** Add the key is the description of the parameter if showKey is
  * set at true. */
  static void GenerateDoc( const Application::Pointer app, std::string & val, const bool showKey = false);

protected:

  /** Get the parameter description of one parameter. */
  /** Add the key is the description of the parameter if showKey is
  * set at true. */
  static void GetDocParameters( const Application::Pointer app, std::string & val, const bool showKey = false );

  /** generate the documentation associated to a group.*/
  /** Add the key is the description of the parameter if showKey is
  * set at true. */
  static void GetDocParameterGroup( const Application::Pointer app, std::string & val, const std::string & key, const bool showKey = false );

  /** generate the documentation associated to a choice.*/
  /** Add the key is the description of the parameter if showKey is
  * set at true. */
  static void GetDocParameterChoice( const Application::Pointer app, std::string & val, const std::string & key, const bool showKey = false );


private:
  ApplicationHtmlDocGenerator(const ApplicationHtmlDocGenerator &); //purposely not implemented
  void operator =(const ApplicationHtmlDocGenerator&); //purposely not implemented

}; // End class InputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
