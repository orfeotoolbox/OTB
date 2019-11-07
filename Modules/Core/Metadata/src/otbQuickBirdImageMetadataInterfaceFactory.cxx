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


#include <typeinfo>
#include <cassert>

#include "otbQuickBirdImageMetadataInterfaceFactory.h"
#include "otbQuickBirdImageMetadataInterface.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"

namespace otb
{

QuickBirdImageMetadataInterfaceFactory::QuickBirdImageMetadataInterfaceFactory()
{
  this->RegisterOverride("OpticalImageMetadataInterface", "otbQuickBirdImageMetadataInterface", "QuickBird Metadata Interface", 1,
                         itk::CreateObjectFunction<QuickBirdImageMetadataInterface>::New());
}

QuickBirdImageMetadataInterfaceFactory::~QuickBirdImageMetadataInterfaceFactory()
{
}

const char* QuickBirdImageMetadataInterfaceFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char* QuickBirdImageMetadataInterfaceFactory::GetDescription() const
{
  return "QuickBird Metadata Interface Factory, handle QuickBird metadata in OTB";
}

} // end namespace otb
