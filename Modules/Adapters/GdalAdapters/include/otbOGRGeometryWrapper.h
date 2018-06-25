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

#ifndef otbOGRGeometryWrapper_h
#define otbOGRGeometryWrapper_h

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#pragma GCC diagnostic pop
#else
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#endif

#include "OTBGdalAdaptersExport.h"

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
struct OTBGdalAdapters_EXPORT GeometryDeleter
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
OTBGdalAdapters_EXPORT bool Intersects (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Returns wheither if two geometries are equivalent.
OTBGdalAdapters_EXPORT bool Equals (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for disjointness.
OTBGdalAdapters_EXPORT bool Disjoint (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for touching.
OTBGdalAdapters_EXPORT bool Touches (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for crossing.
OTBGdalAdapters_EXPORT bool Crosses (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for containment.
OTBGdalAdapters_EXPORT bool Within (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for containment.
OTBGdalAdapters_EXPORT bool Contains (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Tests for overlap.
OTBGdalAdapters_EXPORT bool Overlaps (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Computes distance between two geometries.
OTBGdalAdapters_EXPORT double Distance (OGRGeometry const& lhs, OGRGeometry const& rhs);

/// Computes intersection.
OTBGdalAdapters_EXPORT UniqueGeometryPtr Intersection (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Computes union.
OTBGdalAdapters_EXPORT UniqueGeometryPtr Union (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Computes union using cascading.
OTBGdalAdapters_EXPORT UniqueGeometryPtr UnionCascaded (OGRGeometry const& this_);
/// Computes difference.
OTBGdalAdapters_EXPORT UniqueGeometryPtr Difference (OGRGeometry const& lhs, OGRGeometry const& rhs);
/// Computes symmetric difference.
OTBGdalAdapters_EXPORT UniqueGeometryPtr SymDifference (OGRGeometry const& lhs, OGRGeometry const& rhs); // -1.8

/** Simplifies Geometry.
 * This function tries to uses the best simplication algorithm available in the
 * current version of GDAL. <em>Best</em> in the sense of topology preservation.
 * i.e. With GDAL v1.8.0, \c OGRGeometry::Simplify() is used; with GDAL v1.9.0+,
 * \c OGRGeometry::SimplifyPreserveTopology() is used.
 * \pre Requires GDAL 1.8.0
 * \sa \c OGRGeometry::Simplify()
 * \sa \c OGRGeometry::SimplifyPreserveTopology()
 */
OTBGdalAdapters_EXPORT UniqueGeometryPtr Simplify(OGRGeometry const& g, double tolerance);

/** Simplifies Geometry with no guarantee of preserving the geometry.
 * \pre Requires GDAL 1.8.0
 * \sa \c OGRGeometry::Simplify()
 */
OTBGdalAdapters_EXPORT UniqueGeometryPtr SimplifyDontPreserveTopology(OGRGeometry const& g, double tolerance);

/** Simplifies Geometry while preserving topology.
 * \pre Requires GDAL 1.9.0
 * \sa \c OGRGeometry::SimplifyPreserveTopology()
 */
OTBGdalAdapters_EXPORT UniqueGeometryPtr SimplifyPreserveTopology(OGRGeometry const& g, double tolerance);

/** @} */

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbOGRGeometryWrapper.hxx"
#endif

#endif // otbOGRGeometryWrapper_h
