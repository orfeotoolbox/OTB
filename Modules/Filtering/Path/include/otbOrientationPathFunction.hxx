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

#ifndef otbOrientationPathFunction_hxx
#define otbOrientationPathFunction_hxx

#include "otbOrientationPathFunction.h"
#include "otbPathFunction.h"
#include "itkNumericTraits.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{

template <class TInputPath, class TOutput>
void
OrientationPathFunction<TInputPath, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

template <class TInputPath, class TOutput>
typename OrientationPathFunction<TInputPath,
    TOutput>::OutputType
OrientationPathFunction<TInputPath, TOutput>
::Evaluate(const PathType& path) const
{
  VertexListPointer vertexList;
  VertexType        cindex;
  VertexType        IndexOut;
  int               nbPath;
  RealType          Theta;

  vertexList = path.GetVertexList();
  nbPath = vertexList->Size();

  if (nbPath == 2)
    {
    cindex = vertexList->GetElement(0);
    RealType x1 = cindex[0];
    RealType y1 = cindex[1];
    cindex = vertexList->GetElement(1);
    RealType x2 = cindex[0];
    RealType y2 = cindex[1];

    Theta = std::atan2(y2 - y1, x2 - x1);
    } // IF loop
  else
    {
    itkExceptionMacro(<< "OrientationPathFunction::Evaluate() FAILED -- path must have 2 points");
    }
  return (static_cast<OutputType>(Theta));

}

template <class TInputPath, class TOutput>
typename OrientationPathFunction<TInputPath,
    TOutput>::OutputType
OrientationPathFunction<TInputPath, TOutput>
::Evaluate() const
{
  if (!this->GetInputPath())
    {
    otbMsgDevMacro(<< "Problem with GetInputPath");
    return static_cast<OutputType>(itk::NumericTraits<OutputType>::max());
    }

  OutputType Result =  Evaluate(*(this->GetInputPath()));

  return Result;
}

} // namespace otb

#endif
