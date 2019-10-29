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

#include "otbRADImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbRADImageIO.h"
#include "itkVersion.h"

namespace otb
{

RADImageIOFactory::RADImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase", "otbRADImageIO", "RAD Image IO", 1, itk::CreateObjectFunction<RADImageIO>::New());
}

RADImageIOFactory::~RADImageIOFactory()
{
}

const char* RADImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* RADImageIOFactory::GetDescription() const
{
  return "RAD ImageIO Factory, permettant le chargement d'image au format RAD dans l'OTB";
}

// Undocumented API used to register during static initialization.
// DO NOT CALL DIRECTLY.

static bool RADImageIOFactoryHasBeenRegistered;

void RADImageIOFactoryRegister__Private(void)
{
  if (!RADImageIOFactoryHasBeenRegistered)
  {
    RADImageIOFactoryHasBeenRegistered = true;
    RADImageIOFactory::RegisterOneFactory();
  }
}
} // end namespace otb
