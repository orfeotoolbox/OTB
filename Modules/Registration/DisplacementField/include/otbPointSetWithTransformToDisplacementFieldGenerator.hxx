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

#ifndef otbPointSetWithTransformToDisplacementFieldGenerator_hxx
#define otbPointSetWithTransformToDisplacementFieldGenerator_hxx

#include "otbPointSetWithTransformToDisplacementFieldGenerator.h"

#include "otbMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPointSet, class TDisplacementField>
PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::PointSetWithTransformToDisplacementFieldGenerator()
{
  m_Transform = nullptr;    // has to be provided by the user
}
/**
 * PrintSelf Method
 */
template <class TPointSet, class TDisplacementField>
void
PointSetWithTransformToDisplacementFieldGenerator<TPointSet, TDisplacementField>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
