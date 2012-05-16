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

/**\ingroup gGeometry
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
 * \since OTB v 3.14.0
 * @{
 */

/**
 * Smart-pointer over \c OGRGeometry, with \em move-semantics.
 * In a few words:
 * - When a function receives a \em unique_ptr<>, it implicitly assumes
 * responsibility of the received pointer.
 * - When a function returns a \em unique_ptr<>, the client code implicitly
 * assumes responsibility of the returned pointer.
 * - When exiting a scope, the pointer is destroyed. Which means, as long as we
 * keep using \em unique_ptr<> around functions calls, there is no need to
 * burden our mind with the lifetime of the encapsulated pointers.
 * - Moreover, we can specify how pointers are deleted; property that we
 * exploit to use the ad'hoc destruction function from OGR API, see \c
 * GeometryDeleter.
 *
 * \see http://www2.research.att.com/~bs/C++0xFAQ.html#rval about \em move-semantics.
 * \see http://www2.research.att.com/~bs/C++0xFAQ.html#std-unique_ptr about \c
 * std::unique_ptr<>
 * \see GOTW \#103, \#104 about \c unique_ptr<> as well: http://herbsutter.com/gotw/_103/
 *
 * \note You may experiment difficulties to copy \c UniqueGeometryPtr. This is
 * likelly to be normal. You'll have to emulate \c std::move() with \c
 * boost::move() (with Boost v1.48+, or with \c boost::interprocess::move()
 * otherwise). Check for instance \c Feature::StealGeometry(), or \c
 * Feature::SetGeometryDirectly() to see examples.
 *
 * \internal
 * This should be defined on top of C++11 \c std::unique_ptr<>. By the mean
 * time, we are using an emulation provided in boost.interprocess.
 * \todo When CMake provides a way to detect C++11 features, use a \c #ifdef to
 * use the correct type.
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

/** Simplifies Geometry.
 * This function tries to uses the best simplication algorithm available in the
 * current version of GDAL. <em>Best</em> in the sense of topology preservation.
 * i.e. With GDAL v1.8.0, \c OGRGeometry::Simplify() is used; with GDAL v1.9.0+,
 * \c OGRGeometry::SimplifyPreserveTopology() is used.
 * \pre Requires GDAL 1.8.0
 * \sa \c OGRGeometry::Simplify()
 * \sa \c OGRGeometry::SimplifyPreserveTopology()
 */
UniqueGeometryPtr Simplify(OGRGeometry const& g, double tolerance);

/** Simplifies Geometry with no guarantee of preserving the geometry.
 * \pre Requires GDAL 1.8.0
 * \sa \c OGRGeometry::Simplify()
 */
UniqueGeometryPtr SimplifyDontPreserveTopology(OGRGeometry const& g, double tolerance);

/** Simplifies Geometry while preserving topology.
 * \pre Requires GDAL 1.9.0
 * \sa \c OGRGeometry::SimplifyPreserveTopology()
 */
UniqueGeometryPtr SimplifyPreserveTopology(OGRGeometry const& g, double tolerance);

/** @} */

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbOGRGeometryWrapper.txx"
#endif

#endif // __otbOGRGeometryWrapper_h
