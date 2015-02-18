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
#ifndef __otbSimplifyPathListFilter_h
#define __otbSimplifyPathListFilter_h

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
   *  If the path is considered consistent (which will occure at least with a 2 vertices path),
   * only the beginning and ending
   *  vertices are kept and a new search iteration begin at its end.
   *
   *  This filter is part of the road extraction framework.
   *
   * This class is just a shortcut to the UnaryFunctorObjectListFilter with
   * the SimplifyPathFunctor.
   *
   *   <b>Recent API changes:</b>
   * Now part of the UnaryFunctorObjectListFilter hierachy, replace call to SetTolerance()
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
