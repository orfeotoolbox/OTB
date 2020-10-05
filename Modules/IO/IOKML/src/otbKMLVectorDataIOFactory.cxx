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


#include "otbKMLVectorDataIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "itkVersion.h"
#include "otbKMLVectorDataIO.h"

namespace otb
{

KMLVectorDataIOFactory::KMLVectorDataIOFactory()
{
  static std::string classOverride = std::string("otbVectorDataIOBase");
  static std::string subclass      = std::string("otbKMLVectorDataIO");

  this->RegisterOverride(classOverride.c_str(), subclass.c_str(), "KML Vectordata IO", 1, itk::CreateObjectFunction<KMLVectorDataIO>::New());
}

KMLVectorDataIOFactory::~KMLVectorDataIOFactory()
{
}

const char* KMLVectorDataIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}


const char* KMLVectorDataIOFactory::GetDescription() const
{
  return "KML VectorDataIO Factory, allows the loading of KML vector data into OTB";
}

} // end namespace otb
