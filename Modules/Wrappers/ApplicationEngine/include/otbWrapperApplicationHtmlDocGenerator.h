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

#ifndef otbWrapperApplicationHtmlDocGenerator_h
#define otbWrapperApplicationHtmlDocGenerator_h

#include "otbWrapperApplication.h"

namespace otb
{
namespace Wrapper
{
/** \class ApplicationHtmlDocGenerator
 *  \brief This class genertaes the documentation of a class usung the
 *  class doc attributes.
 *
 * \ingroup OTBApplicationEngine
 */

class OTBApplicationEngine_EXPORT ApplicationHtmlDocGenerator
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
  ApplicationHtmlDocGenerator(const ApplicationHtmlDocGenerator &) = delete;
  void operator =(const ApplicationHtmlDocGenerator&) = delete;

}; // End class InputImage Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
