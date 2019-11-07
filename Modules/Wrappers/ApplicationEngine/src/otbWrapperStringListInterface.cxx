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


#include "otbWrapperStringListInterface.h"


namespace otb
{


namespace Wrapper
{

const std::string NULL_STRING;


/*****************************************************************************/
void StringListInterface::AddNullElement()
{
  InsertNullElement();
}


/*****************************************************************************/
Role StringListInterface::GetDirection(std::size_t) const
{
  return GetDirection();
}


/*****************************************************************************/
const std::string& StringListInterface::GetFilenameFilter(std::size_t) const
{
  return GetFilenameFilter();
}


/*****************************************************************************/
const std::string& StringListInterface::GetFilenameFilter() const
{
  return NULL_STRING;
}

/*****************************************************************************/
bool StringListInterface::IsFilename() const
{
  return true;
}

/*****************************************************************************/
void StringListInterface::Erase(std::size_t id)
{
  Erase(id, 1);
}

/*****************************************************************************/
std::size_t StringListInterface::SetStrings(const StringVector&)
{
  return 0;
}

/*****************************************************************************/
std::size_t StringListInterface::GetStrings(StringVector&) const
{
  return 0;
}

} // End of Namespace 'Wrapper'

} // End of Namespace 'otb'
