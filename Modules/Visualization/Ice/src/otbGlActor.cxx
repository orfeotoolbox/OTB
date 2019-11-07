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

#include "otbGlActor.h"

namespace otb
{
GlActor::GlActor()
  : m_Shader()
  , m_Settings(ViewSettings::New())
  , m_Name("Actor")
  , m_Visible(true)
  , m_GeometryChanged( false )
  , m_Overlay( false )
{}

GlActor::~GlActor()
{}

void
GlActor::CreateShader()
{}

} // End namespace 'otb'.
