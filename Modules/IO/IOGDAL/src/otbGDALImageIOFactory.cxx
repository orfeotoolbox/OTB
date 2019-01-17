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

#include "otbGDALImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "otbGDALImageIO.h"
#include "itkVersion.h"

namespace otb
{

GDALImageIOFactory::GDALImageIOFactory()
{
  this->RegisterOverride("otbImageIOBase",
                         "otbGDALImageIO",
                         "GDAL Image IO",
                         1,
                         itk::CreateObjectFunction<GDALImageIO>::New());
}

GDALImageIOFactory::~GDALImageIOFactory()
{
}

const char*
GDALImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
GDALImageIOFactory::GetDescription() const
{
  return "GDAL ImageIO Factory, enabling loading gdal images in OTB";
}

// Undocumented API used to register during static initialization.
// DO NOT CALL DIRECTLY.

static bool GDALImageIOFactoryHasBeenRegistered;

void GDALImageIOFactoryRegister__Private(void)
{
  if( ! GDALImageIOFactoryHasBeenRegistered )
    {
    GDALImageIOFactoryHasBeenRegistered = true;
    GDALImageIOFactory::RegisterOneFactory();
    }
}
} // end namespace otb
