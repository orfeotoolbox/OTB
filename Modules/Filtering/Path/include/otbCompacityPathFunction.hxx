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

#ifndef otbCompacityPathFunction_hxx
#define otbCompacityPathFunction_hxx

#include "otbCompacityPathFunction.h"
#include "itkNumericTraits.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{

template <class TInputPath, class TOutput>
void
CompacityPathFunction<TInputPath, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

template <class TInputPath, class TOutput>
typename CompacityPathFunction<TInputPath,
    TOutput>::OutputType
CompacityPathFunction<TInputPath, TOutput>
::Evaluate(const PathType& path) const
{
  VertexListPointer vertexList;
  VertexType        cindex;
  VertexType        IndexOut;
  int               nbPath;
  RealType          Surface = 0.0;
  RealType          Perimeter = 0.0;
  RealType          Compacity;
  RealType          x1, x2, y1, y2;

  vertexList = path.GetVertexList();
  nbPath = vertexList->Size();

  if (nbPath > 2)
    {
    for (int i = 0; i < nbPath; ++i)
      {
      cindex = vertexList->GetElement(i);
      x1 = cindex[0];
      y1 = cindex[1];

      if (i == (nbPath - 1))
        {
        cindex = vertexList->GetElement(0);
        }
      else
        {
        cindex = vertexList->GetElement(i + 1);
        }
      x2 = cindex[0];
      y2 = cindex[1];

      RealType Norm;

      Norm  = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

      Perimeter += Norm;

      RealType P = x2 * (y2 - y1);
      RealType Q = y2 * (x2 - x1);

      Surface += (P - Q);
      } // FOR loop
    } // IF loop
  else
    {
    itkExceptionMacro(<< "CompacityPathFunction::Evaluate() FAILED -- path must contains at least 3 points");
    }
  Surface /= 2.0;

  Compacity = Surface / (Perimeter * Perimeter);
  Compacity *= (4. * acos(-1.0));

  return (static_cast<OutputType>(Compacity));

}

template <class TInputPath, class TOutput>
typename CompacityPathFunction<TInputPath,
    TOutput>::OutputType
CompacityPathFunction<TInputPath, TOutput>
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
