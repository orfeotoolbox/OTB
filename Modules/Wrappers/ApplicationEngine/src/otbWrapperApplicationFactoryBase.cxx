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

#include "otbWrapperApplicationFactoryBase.h"

namespace otb
{
namespace Wrapper
{

Application::Pointer ApplicationFactoryBase::CreateApplication(const char* name)
{
  Application::Pointer appli;

  LightObject::Pointer obj = this->CreateObject(name);
  if (obj.IsNotNull())
  {
    Application* app = dynamic_cast<Application*>(obj.GetPointer());
    if (app)
    {
      appli = app;
    }
  }
  return appli;
}
}
}
