/*=========================================================================

 Program: ORFEO Toolbox
 Language: C++
 Date: $Date$
 Version: $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE. See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbOGRGeometryWrapper_h
#define __otbOGRGeometryWrapper_h

#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
class OGRGeometry;

namespace otb { namespace ogr {
namespace internal {
/**\ingroup GeometryInternals
 * Deleter dedicated to \c OGRGeometry.
 * \internal
 * Unlike OGR, works as a no-op on null geometries.
 *
 * Unlike the other deleters used, the type of this one needs to be known in the
 * header file in order to define the type \c UniqueGeometryPtr. As a
 * consequence, it is not in an anonymous namespace , but in \c
 * otb::ogr::internal.
 */
struct GeometryDeleter
  {
  void operator()(OGRGeometry* p);
  };
} // internal namespace


// we don't encapsulate OGRGeometry, but please, don't create new geometries with a new.

/**\ingroup Geometry
 * \defgroup OGRGeometryWrapper OGRGeometry Wrappers
 * %Helper definition to handle \c OGRGeometry objects.
 *
 * \note %OTB doesn't provide anything on top of \c OGRGeometryFactory to create
 * new factories. Please, never create new \c OGRGeometry by hand with
 * <tt>new</tt> operator as there is no garanty they'll get destroyed within the
 * proper memory context when released from an owning \c OGRFeature.
 *
 * Thus, always use \c OGRGeometryFactory functions to create new geometries.
 * You can then manage their lifetime manually or rely on \c UniqueGeometryPtr
 * that provides a non-copyable, but movable RAII wrapper around \c OGRGeometry.
 * @{
 */
typedef boost::interprocess::unique_ptr<OGRGeometry, internal::GeometryDeleter> UniqueGeometryPtr;
///Do these features intersect?
bool Intersects (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Returns wheither if two geometries are equivalent.
bool Equals (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for disjointness.
bool Disjoint (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for touching.
bool Touches (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for crossing.
bool Crosses (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for containment.
bool Within (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for containment.
bool Contains (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for overlap.
bool Overlaps (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Computes distance between two geometries.
double Distance (OGRGeometry const& lhs, OGRGeometry const& rhs);

/// Computes intersection.
UniqueGeometryPtr Intersection (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Computes union.
UniqueGeometryPtr Union (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Computes union using cascading.
UniqueGeometryPtr UnionCascaded (OGRGeometry const& this_);
/// Computes difference.
UniqueGeometryPtr Difference (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Computes symmetric difference.
UniqueGeometryPtr SymDifference (OGRGeometry const& lhs, OGRGeometry const& rhs); // -1.8

/** @} */

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbOGRGeometryWrapper.txx"
#endif

#endif // __otbOGRGeometryWrapper_h
