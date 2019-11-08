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

#ifndef otbWrapperApplicationRegistry_h
#define otbWrapperApplicationRegistry_h

#include <string>
#include "itkObject.h"

#include "otbWrapperApplication.h"

namespace otb
{
namespace Wrapper
{

/** \class Application
 *  \brief This class represent an application
 *  TODO
 *
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT ApplicationRegistry : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef ApplicationRegistry           Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(ApplicationRegistry, Object);

  /** Convenient typedefs. */
  typedef otb::Wrapper::Application::Pointer ApplicationPointer;

  /** Set the specified path to the list of application search path. Reinit all previously set paths */
  static void SetApplicationPath(std::string path);

  /** Add the specified path to the list of application search path */
  static void AddApplicationPath(std::string path);

  /** Return the application search path */
  static std::string GetApplicationPath();

  /** Return the list of available applications */
  static std::vector<std::string> GetAvailableApplications(bool useFactory = true);

  /** Create the specified Application */
  static Application::Pointer CreateApplication(const std::string& applicationName, bool useFactory = true);

  /** Create the specified Application (faster)
   *  method using dynamic library name to load the right module */
  static Application::Pointer CreateApplicationFaster(const std::string& applicationName);

  /** Clean registry by releasing unused modules */
  static void CleanRegistry();

protected:
  ApplicationRegistry();
  ~ApplicationRegistry() override;

private:
  ApplicationRegistry(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Load an application from a shared library */
  static Application::Pointer LoadApplicationFromPath(std::string path, std::string name);
};

} // end namespace Wrapper
} // end namespace otb

#endif // otbWrapperApplication_h_
