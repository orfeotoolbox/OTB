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


#include "otbOGRVectorDataIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"
#include "otbOGRVectorDataIO.h"

namespace otb
{

OGRVectorDataIOFactory::OGRVectorDataIOFactory()
{
  static std::string classOverride = std::string("otbVectorDataIOBase");
  static std::string subclass = std::string("otbOGRVectorDataIO");

  this->RegisterOverride(classOverride.c_str(),
                         subclass.c_str(),
                         "OGR Vectordata IO",
                         1,
                         itk::CreateObjectFunction<OGRVectorDataIO>::New());
}

OGRVectorDataIOFactory::~OGRVectorDataIOFactory()
{}

const char*
OGRVectorDataIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
OGRVectorDataIOFactory::GetDescription() const
{
  return "OGR VectorDataIO Factory, allows the loading of ogr supported vector data into OTB";
}

} // end namespace otb
