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

#include "otbSpotImageMetadataInterfaceFactory.h"
#include "otbSpotImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{
SpotImageMetadataInterfaceFactory::SpotImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface", "otbSpotImageMetadataInterface", "Spot Metadata Interface", 1,
                         itk::CreateObjectFunction<SpotImageMetadataInterface>::New());
}

SpotImageMetadataInterfaceFactory::~SpotImageMetadataInterfaceFactory()
{
}

const char* SpotImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* SpotImageMetadataInterfaceFactory::GetDescription() const
{
  return "Spot Metadata Interface Factory, handle Spot metadata in OTB";
}

} // end namespace otb
