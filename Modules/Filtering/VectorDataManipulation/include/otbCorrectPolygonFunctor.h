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

#ifndef otbCorrectPolygonFunctor_h
#define otbCorrectPolygonFunctor_h

#include "otbSimplifyPathFunctor.h"
#include "otbClosePathFunctor.h"

namespace otb
{

/** \class CorrectPolygonFunctor
*    \brief This filter simplify and close the input polygon, making the last point equal to the first one.
*
* This filter may be useful when a truly closed polygon is needed (to draw it for example)
*
* \sa UnaryFunctorObjectListFilter
*
 *  \ingroup Functor
 *
 * \ingroup OTBVectorDataManipulation
 */
template <class TPolygon>
class CorrectPolygonFunctor
{
public:
  typedef TPolygon                      PolygonType;
  typedef typename PolygonType::Pointer PolygonPointerType;

  /** Some typedefs specific to functors*/
  typedef otb::SimplifyPathFunctor<PolygonType, PolygonType> SimplifyFunctorType;
  typedef ClosePathFunctor<PolygonType, PolygonType>         CloseFunctorType;
  CorrectPolygonFunctor()
  {
  }
  ~CorrectPolygonFunctor()
  {
  }

  inline PolygonPointerType operator()(const TPolygon* input)
  {
    /**create functors */
    SimplifyFunctorType simplifyFunctor;
    CloseFunctorType    closeFunctor;

    simplifyFunctor.SetTolerance(0.0);

    /** Erase aligned points and close polygon*/
    return closeFunctor(simplifyFunctor(input));
  }
};
}

#endif
