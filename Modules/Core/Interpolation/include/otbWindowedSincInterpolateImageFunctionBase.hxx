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

#ifndef otbWindowedSincInterpolateImageFunctionBase_hxx
#define otbWindowedSincInterpolateImageFunctionBase_hxx

#include "otbWindowedSincInterpolateImageFunctionBase.h"

namespace otb
{
/** Constructor */
template <class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
WindowedSincInterpolateImageFunctionBase<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>::WindowedSincInterpolateImageFunctionBase()
{
}

/** Destructor */
template <class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
WindowedSincInterpolateImageFunctionBase<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>::~WindowedSincInterpolateImageFunctionBase()
{
}

template <class TInputImage, class TWindowFunction, class TBoundaryCondition, class TCoordRep>
void WindowedSincInterpolateImageFunctionBase<TInputImage, TWindowFunction, TBoundaryCondition, TCoordRep>::PrintSelf(std::ostream& os,
                                                                                                                      itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // namespace otb
#endif
