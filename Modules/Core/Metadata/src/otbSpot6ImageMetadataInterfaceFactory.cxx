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


#include <typeinfo>
#include <cassert>

#include "otbSpot6ImageMetadataInterfaceFactory.h"
#include "otbSpot6ImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
Spot6ImageMetadataInterfaceFactory::Spot6ImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface", "otbSpot6ImageMetadataInterface", "Spot6 Metadata Interface", 1,
                         itk::CreateObjectFunction<Spot6ImageMetadataInterface>::New());
}

Spot6ImageMetadataInterfaceFactory::~Spot6ImageMetadataInterfaceFactory()
{
}

const char* Spot6ImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* Spot6ImageMetadataInterfaceFactory::GetDescription() const
{
  return "Spot6 Metadata Interface Factory, handle Spot6 metadata in OTB";
}

} // end namespace otb
