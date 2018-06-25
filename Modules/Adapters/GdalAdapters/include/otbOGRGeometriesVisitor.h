/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOGRGeometriesVisitor_h
#define otbOGRGeometriesVisitor_h

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include "ogr_geometry.h"

namespace otb
{
namespace ogr
{
/**\ingroup boost
 * \brief propagation of const-qualifier.
 * \note This type traits is likelly to appear in boost at some point in time.
 * \note As a consequence, it follows C++ standard and boost naming policy.
 * \since OTB v 3.14.0
 * @{
 */
template <typename Tin, typename Tout>
struct propagate_const
{ typedef Tout type; };
template <typename Tin, typename Tout>
struct propagate_const<Tin const, Tout>
{ typedef typename boost::add_const<Tout>::type type; };
/** @} */

#define TRY_APPLY(TYPE, geometry, functor) \
if (typename propagate_const<TGeometry, TYPE>::type * dc_##TYPE \
  = dynamic_cast<typename propagate_const<TGeometry, TYPE>::type*>(geometry))\
{\
  return functor(dc_##TYPE); \
}


/**\ingroup gGeometry
 * External polymorphic call of functions on \c OGRGeometry.
 * This helper function tries to polymorphically dispatch the call of a function
 * on the right \c OGRGeometry subtype.
 * \internal
 * In a perfect world, \c OGRGeometry would have provided a visitor to extend
 * the number of functions to polymorphically %apply on them. As this isn't the
 * case, \c apply executes many downcasts until it finds the right subtype.
 *
 * \tparam TResult type of the result
 * \tparam TGeometry matched an \c OGRGeometry that may or may not be const
 * qualified.
 * \tparam TFunctor functor to apply on the geometry.
 * \todo Support a list of types to check
 * \since OTB v 3.14.0
 */
template <typename TResult, class TGeometry, typename TFunctor>
TResult apply(TGeometry * geometry, TFunctor functor)
{
  BOOST_MPL_ASSERT((boost::is_same<OGRGeometry, typename boost::remove_const<TGeometry>::type >));
  TRY_APPLY(OGRPoint, geometry, functor)
    else TRY_APPLY(OGRLinearRing, geometry, functor)
    else TRY_APPLY(OGRLineString, geometry, functor)
    // else TRY_APPLY(OGRCurve, geometry, functor)
    else TRY_APPLY(OGRPolygon, geometry, functor)
    // else TRY_APPLY(OGRSurface, geometry, functor)
    else TRY_APPLY(OGRMultiLineString, geometry, functor)
    else TRY_APPLY(OGRMultiPoint, geometry, functor)
    else TRY_APPLY(OGRMultiPolygon, geometry, functor)
    else TRY_APPLY(OGRGeometryCollection, geometry, functor)
      {
      // functor(geometry);
      }
    return TResult(); // keep compiler happy
}
} // ogr namespace

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbOGRGeometriesVisitor.hxx"
#endif

#endif // otbOGRGeometriesVisitor_h
