/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperApplicationFactoryBase_h
#define otbWrapperApplicationFactoryBase_h

#include "OTBApplicationEngineExport.h"
#include "itkObjectFactoryBase.h"
#include "otbWrapperApplication.h"

namespace otb
{
namespace Wrapper
{

class OTBApplicationEngine_EXPORT ApplicationFactoryBase : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef ApplicationFactoryBase        Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ApplicationFactoryBase, itk::ObjectFactoryBase);

  /** Create an application using the CreateObject() method of the current factory */
  Application::Pointer CreateApplication(const char* name);

protected:
  ApplicationFactoryBase()
  {
  }

  ~ApplicationFactoryBase() override
  {
  }

private:
  ApplicationFactoryBase(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace Wrapper
} // end namespace otb

#endif
