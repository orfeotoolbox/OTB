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

#include "otbImageSettings.h"
#include "otbFragmentShaderRegistry.h"
#include "otbGlVersionChecker.h"
#include <GL/glew.h>

namespace otb
{

ImageSettings::ImageSettings()
  : m_MinRed( 0 ),
    m_MaxRed( 255 ),
    m_MinGreen( 0 ),
    m_MaxGreen( 255 ),
    m_MinBlue( 0 ),
    m_MaxBlue( 255 ),
    m_UseNoData( false ),
    m_NoData( 0 ),
    m_Gamma( 1. ),
    m_Alpha( 1. ),
    m_CurrentRed( 0 ),
    m_CurrentGreen( 0 ),
    m_CurrentBlue( 0 )
{
}


ImageSettings::~ImageSettings()
{
}


} // End namespace otb
