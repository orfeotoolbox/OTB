/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbBSQImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbBSQImageIO.h"
#include "itkVersion.h"

namespace otb
{

BSQImageIOFactory::BSQImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase", "otbBSQImageIO", "BSQ Image IO", 1, itk::CreateObjectFunction<BSQImageIO>::New());
}

BSQImageIOFactory::~BSQImageIOFactory()
{
}

const char* BSQImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* BSQImageIOFactory::GetDescription() const
{
  return "BSQ ImageIO Factory, permettant le chargement d'image au format BSQ dans l'OTB";
}

// Undocumented API used to register during static initialization.
// DO NOT CALL DIRECTLY.

static bool BSQImageIOFactoryHasBeenRegistered;

void BSQImageIOFactoryRegister__Private(void)
{
  if (!BSQImageIOFactoryHasBeenRegistered)
  {
    BSQImageIOFactoryHasBeenRegistered = true;
    BSQImageIOFactory::RegisterOneFactory();
  }
}

} // end namespace otb
