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

#include "otbWrapperInputFilenameListParameter.h"


namespace otb
{


namespace Wrapper
{


const std::string FILENAME_FILTER(
    "All files (*);;"
    "CSV files (.csv);;"
    "Text files (.txt);;"
    "XML files (.xml)");


/*****************************************************************************/
InputFilenameListParameter::InputFilenameListParameter()
{
  SetName("Input Filename List");
  SetKey("inList");
}


/*****************************************************************************/
InputFilenameListParameter::~InputFilenameListParameter()
{
}


/*****************************************************************************/
Role InputFilenameListParameter::GetDirection(std::size_t) const
{
#if 0
  assert( i<m_FilenameList->Size() );
  assert( !m_FilenameList->GetNthElement( i ).IsNull() );

  return m_FilenameList->GetNthElement( i )->GetRole();

#else
  // otb::Parameter::GetRole() does not necessarily stand for
  // direction of parameter.
  return GetDirection();

#endif
}


/*****************************************************************************/
Role InputFilenameListParameter::GetDirection() const
{
  return Role_Input;
}


/*****************************************************************************/
const std::string& InputFilenameListParameter::GetFilenameFilter(std::size_t) const
{
  return GetFilenameFilter();
}


/*****************************************************************************/
const std::string& InputFilenameListParameter::GetFilenameFilter() const
{
  return FILENAME_FILTER;
}

ParameterType InputFilenameListParameter::GetType() const
{
  return ParameterType_InputFilenameList;
}
}
}
