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

#ifndef otbWrapperInputXML_h
#define otbWrapperInputXML_h

#include "otbWrapperApplication.h"
#include "otb_tinyxml.h"
#include <sstream>
#include <string>

namespace otb
{
namespace Wrapper
{
namespace XML
{

OTBApplicationEngine_EXPORT const std::string GetChildNodeTextOf(TiXmlElement* parentElement, std::string key);

OTBApplicationEngine_EXPORT int Read(const std::string& filename, Application::Pointer application);

/* copied from Utilities/tinyXMLlib/tinyxml.cpp. Must have a FIX inside tinyxml.cpp */
OTBApplicationEngine_EXPORT FILE* TiXmlFOpen(const char* filename, const char* mode);

} // namespace XML
} // End namespace Wrapper
} // End namespace otb

#endif
