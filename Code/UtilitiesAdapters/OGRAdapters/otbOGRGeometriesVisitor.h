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
#ifndef __otbOGRGeometriesVisitor_h
#define __otbOGRGeometriesVisitor_h

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include "ogr_geometry.h"

namespace otb
{
namespace ogr
{
template <typename Tin, typename Tout>
struct propagate_const
{
typedef Tout type;
};
template <typename Tin, typename Tout>
struct propagate_const<Tin const, Tout>
{
typedef typename boost::add_const<Tout>::type type;
};

#define TRY_APPLY(TYPE, geometry, functor) \
if (typename propagate_const<TGeometry, TYPE>::type * dc = dynamic_cast<typename propagate_const<TGeometry, TYPE>::type*>(geometry))\
{\
  return functor(dc); \
}


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
}
} // ogr namespace

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbOGRGeometriesVisitor.txx"
#endif

#endif // __otbOGRGeometriesVisitor_h
