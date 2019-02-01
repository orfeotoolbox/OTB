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

#ifndef otbSimplifyPathListFilter_h
#define otbSimplifyPathListFilter_h

#include "otbUnaryFunctorObjectListFilter.h"
#include "otbSimplifyPathFunctor.h"
#include "otbObjectList.h"

namespace otb
{
/** \class SimplifyPathListFilter
   *  \brief This filter performs a simplification of the path in the input list.
   *
   *  It reduces the number of vertices in each path, according to a tolerance criterion. It aims at
   *  removing aligned vertices while keeping sharp angular points.
   *
   *  In order to ensure the unicity of its output, each path is considered first from begin to end, then
   *  from begin to the first vertex before the end. At each step, the consistency of the path is checked :
   *  the equation of the line passing by the first and last vertices is computed. Then, for each
   *  vertices between them, the euclidean distance to this line is computed. If for one vertex, this distance
   *  is upper than the tolerance threshold, the path is considered to be inconsistent and no vertices can be removed.
   *
   *  If the path is considered consistent (which will occur at least with a 2 vertices path),
   * only the beginning and ending
   *  vertices are kept and a new search iteration begin at its end.
   *
   *  This filter is part of the road extraction framework.
   *
   * This class is just a shortcut to the UnaryFunctorObjectListFilter with
   * the SimplifyPathFunctor.
   *
   *   <b>Recent API changes:</b>
   * Now part of the UnaryFunctorObjectListFilter hierarchy, replace call to SetTolerance()
   * by GetFunctor().SetTolerance().
   *
   *
   * \sa BreakAngularPathListFilter
   * \sa RemoveTortuousPathFilter.
   * \sa UnaryFunctorObjectListFilter
   * \sa SimplifyPathFunctor
   *
   * \example FeatureExtraction/ExtractRoadByStepsExample.cxx
   *
 *
 * \ingroup OTBRoadExtraction
 */

template <class TPath>
class ITK_EXPORT SimplifyPathListFilter :
  public UnaryFunctorObjectListFilter<
      ObjectList<TPath>,
      ObjectList<TPath>,
      SimplifyPathFunctor<TPath, TPath> >
{};

} // End namespace otb

#endif
