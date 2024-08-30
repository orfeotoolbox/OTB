/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperOutputXML_h
#define otbWrapperOutputXML_h

#include "otbWrapperApplication.h"
#include "otb_tinyxml.h"
#include <string>

namespace otb
{
namespace Wrapper
{
namespace XML
{

OTBApplicationEngine_EXPORT TiXmlElement* AddChildNodeTo(TiXmlElement* parent, std::string name, std::string value = "");

OTBApplicationEngine_EXPORT std::string pixelTypeToString(ImagePixelType pixType);

OTBApplicationEngine_EXPORT void Write(const std::string& filename, Application::Pointer application);

OTBApplicationEngine_EXPORT TiXmlElement* ParseApplication(Application::Pointer app);

OTBApplicationEngine_EXPORT std::string MakeCommandLine(otb::Wrapper::Application::Pointer application);

/** Recursive function to parse a group parameter */
OTBApplicationEngine_EXPORT void ParseGroup(Application::Pointer app, TiXmlElement* n_App, const std::string& group);

} // namespace XML
} // End namespace Wrapper
} // End namespace otb

#endif
