/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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
