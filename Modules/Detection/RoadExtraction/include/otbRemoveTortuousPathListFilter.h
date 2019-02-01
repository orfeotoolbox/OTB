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

#ifndef otbRemoveTortuousPathListFilter_h
#define otbRemoveTortuousPathListFilter_h

#include "otbUnaryFunctorObjectListBooleanFilter.h"
#include "otbPathMeanDistanceFunctor.h"
#include "otbObjectList.h"

namespace otb
{
/** \class RemoveTortuousPathListFilter
   *  \brief This filter remove path considered as tortuous.
   *
   *  A path is considered to be tortuous if the mean distance between each consecutive vertices
   *  is strictly lower than the user provided threshold.
   *
   * This class is just a shortcut to the UnaryFunctorObjectListBooleanFilter with
   * the PathMeanDistanceFunctor.
   *
   *<b>Recent API changes:</b>
   * Now part of the UnaryFunctorObjectListBooleanFilter hierarchy, replace call to SetMeanDistanceThreshold()
   * by GetFunctor().SetThreshold().
   *
   * The inequality is now a strict one.
   *
   * \sa BreakAngularPathListFilter
   * \sa SimplifyPathFilter
   * \sa UnaryFunctorObjectListBooleanFilter
   * \sa PathMeanDistanceFunctor
   *
   * \example FeatureExtraction/ExtractRoadByStepsExample.cxx
   *
 *
 * \ingroup OTBRoadExtraction
 */

//   template <class TPath>
template <class TPath>
class ITK_EXPORT RemoveTortuousPathListFilter :
  public UnaryFunctorObjectListBooleanFilter<
      ObjectList<TPath>,
      ObjectList<TPath>,
      PathMeanDistanceFunctor<typename TPath::Pointer> >
{};

} // End namespace otb

#endif
