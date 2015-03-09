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
#ifndef __otbOGRFeatureWrapper_h
#define __otbOGRFeatureWrapper_h

// #include <iosfwd> // std::ostream&
#include <boost/shared_ptr.hpp>
// #include "itkIndent.h", included from field
#include "otbOGRFieldWrapper.h"
#include "otbOGRGeometryWrapper.h"

class OGRFeature;
class OGRFeatureDefn;

namespace otb {
namespace ogr {
class Feature;
bool operator==(Feature const& lhs, Feature const& rhs);

/**\ingroup gGeometry
 * \class Feature proxy class.
 * \brief Geometric objet with descriptive fields.
 *
 * It provides an encapsulation of OGR classes. In that particular case, it's an
 * encapsulation of \c OGRFeature.
 *
 * \note This class is a proxy class on top of an \c OGRLayer.
 * \note It can be copied, and assigned. New instances will share the underlying
 * \c OGRLayer: \c Feature assignment will just make one \c Feature proxy point
 * to another \c OGRFeature. In order to truly assign from one to another, use
 * \c SetFrom, or \c Clone in order to duplicate the current feature.
 * \note The default constructor is disabled on purpose.
 * \note The destructor automatically generated does everything that is
 * expected.
 * \invariant <tt>m_Feature != 0</tt> Actually, this isn't a true invariant per
 * se, as it may be null in some cases, for instance to define mark the \c
 * Layer::end() iterator.
 * \since OTB v 3.14.0
 *
 * \ingroup OTBGdalAdapters
 */
class Feature
  {
public:
  /**\name Construction, copy and destruction */
  //@{
  /**
   * Construction from a definition.
   * \param[in] definition  definition of the fields of the feature.
   * \throw None
   *
   * \post <tt>m_Feature != 0</tt>
   * \post \c definition internal reference count is incremented
   * \warning DO NOT destroy the definition while it is relied upon by features.
   * \internal The OGR feature is created with the ad'hoc function from OGR API.
   * \sa \c OGRFeature::CreateFeature()
   * \todo See whether wrapping \c OGRFeatureDefn makes sense
   */
  Feature(OGRFeatureDefn & definition);

  /**
   * Construction from a pre-existing \c OGRFeature.
   * \param[in,out] feature  pre-existing feature that will be owned by the
   * wrapper.
   * \throw None
   * \pre The feature is expected to have been built with OGR API.
   * \internal
   * As all functions form \c OGRLayer returns features that are meant to be be
   * of the responsibility of the client code, this constructor is perfect. It
   * has been used in \c Layer interface to return features.
   */
  Feature(OGRFeature * feature);

  /**
   * Destructor.
   * \throw None
   * \internal The OGR feature is released with the ad'hoc function from OGR API.
   * \sa \c OGRFeature::DestroyFeature()
   */
  ~Feature();

  /**
   * Duplicates the current feature.
   * \return a duplicita of \c *this.
   * \throw None
   * \invariant <tt>m_Feature != 0</tt>
   *
   * As \c Feature is a proxy class that shares its underlying \c OGRFeature,
   * this function represents the only way to duplicate a feature.
   * \sa \c OGRFeature::Clone()
   */
  Feature Clone() const;

  /**
   * Assigns a pre-existing feature to the current one.
   * \param[in] rhs  pre-existing feature to assign ourseleves from.
   * \param[in] map  integer list of the fields to duplicated (default: all)
   * \param[in] mustForgive  tells whether the operation should continue despite
   * lacking output fields matching some of the source fields.
   *
   * \throw itk::ExceptionObject if no value is transferred
   * \sa \c OGRFeature::SetFrom()
   */
  void SetFrom(Feature const& rhs, int *map, bool mustForgive = true);
  /** \copydoc Feature::SetFrom(Feature const&, int *, bool mustForgive);
   */
  void SetFrom(Feature const& rhs, bool mustForgive = true);
  //@}

  /** Prints self into stream. */
  void PrintSelf(std::ostream &os, itk::Indent indent) const;

  /** Access to raw \c OGRFeature.
   * This function provides an abstraction leak in case deeper control on the
   * underlying \c OGRFeature is required.
   * \invariant The underlying \c OGRFeature must be valid, i.e.
   * <tt>m_DataSource != 0</tt>, an assertion is fired otherwise.
   * \warning You must under no circonstance try to delete the \c OGRFeature
   * obtained this way.
   */
  OGRFeature & ogr() const;
  /** \copydoc Feature::ogr() const */
  OGRFeature & ogr();

  /** Abstraction leak to the internal shared \c OGRFeature.
   * \throw None
   * \internal
   * This function is meant to link a \c Field to the \c OGRFeature where it is
   * stored, without introducing a cyclic dependency, nor deporting the fields
   * R/W to \c Feature interface.
   */
  boost::shared_ptr<OGRFeature>      & sptr()       {return m_Feature; }
  /** \copydoc Feature::sptr() */
  boost::shared_ptr<OGRFeature> const& sptr() const {return m_Feature; }

  /**
   * Obtains feature ID..
   * \return its ID, or \c OGRNullFID if none has been assigned.
   * \throw None
   * \invariant <tt>m_Feature != 0</tt>
   * \sa \c OGRFeature::GetFID()
   */
  long GetFID() const;
  /**
   * %Feature ID setter.
   * \param[in] fid  new id.
   * \throw itk::ExceptionObject if it cannot be set.
   * \invariant <tt>m_Feature != 0</tt>
   * \sa \c OGRFeature::SetFID()
   */
  void SetFID(long fid);

  /**
   * %Feature definition accessor.
   * \return a reference to the internal definition.
   * \throw None
   * \invariant <tt>m_Feature != 0</tt>
   * \sa \c OGRFeature::GetDefnRef()
   */
  OGRFeatureDefn& GetDefn() const;

  /**\name Fields
   * Fields access.
   * Unlike original OGR API, the access to the fields of a feature is done
   * directly from the \c Field interface, and not the \c Feature interface.
   *
   * Ones first need to iterate on the fields of a feature, and then use the
   * appropriate setter/getter to access to each field.
   */
  //@{
  /**
   * Returns the number of fields.
   * \invariant <tt>m_Feature != 0</tt>
   * \sa \c OGRFeature::GetFieldCount()
   */
  size_t GetSize() const;
  /**
   * Read-Write access to the i-th field.
   * \invariant <tt>m_Feature != 0</tt>
   * \pre <tt>index < GetSize()</tt>
   * \throw None
   */
  Field       operator[](size_t index);
  /**
   * Read-only access to the i-th field.
   * \invariant <tt>m_Feature != 0</tt>
   * \pre <tt>index < GetSize()</tt>
   * \throw None
   */
  Field const operator[](size_t index) const;
  /**
   * Read-Write access to a field by name.
   * \invariant <tt>m_Feature != 0</tt>
   * \throw itk::ExceptionObject if no field named \c name exists.
   */
  Field       operator[](std::string const& name);
  /**
   * Read-Only access to a field by name.
   * \invariant <tt>m_Feature != 0</tt>
   * \throw itk::ExceptionObject if no field named \c name exists.
   */
  Field const operator[](std::string const& name) const;
  /** Accessor to the definition of the i-th field.
   * \invariant <tt>m_Feature != 0</tt>
   * \pre <tt>index < GetSize()</tt>
   * \throw None
   * \sa \c OGRFeature::GetFieldDefnRef()
   */
  FieldDefn   GetFieldDefn(size_t index) const;
  /** Accessor to the definition of a field gven its name.
   * \invariant <tt>m_Feature != 0</tt>
   * \throw itk::ExceptionObject if no field named \c name exists.
   * \sa \c OGRFeature::GetFieldDefnRef()
   */
  FieldDefn   GetFieldDefn(std::string const& name) const;
  //@}

  /**\name Geometries
   * \todo We should detect whether the official C++11 \c std::unique_ptr<> is
   * available instead of always using <tt>boost.interprocess.unique_ptr<></tt>.
   * \internal
   * \c OGRGeometry pointers are used and not references are used because the
   * current geometry can be unset.
   */
  //@{
  /**
   * Changes the current geometry.
   * \param[in] geometry  Pointer to an existing geometry, null to dismiss any
   * geometry.
   * \post The input \c OGRGeometry is copied (cloned actually).
   * \throw itk::ExceptionObject on failure.
   * \invariant <tt>m_Feature != 0</tt>
   * \sa \c OGRFeature::SetGeometry()
   */
  void SetGeometry(OGRGeometry const* geometry);
  /**
   * Accessor to the internal \c OGRGeometry stored.
   *
   * \return The result may be null if no geometry is associated to the feature.
   * \return In any case the result shall not be modified.
   * \throw None
   * \invariant <tt>m_Feature != 0</tt>
   * \sa \c OGRFeature::GetGeometryRef()
   */
  OGRGeometry const* GetGeometry() const;

  /**
   * Changes the current geometry (transfers ownership).
   * \param[in] geometry  Unique-pointer to an existing geometry, null to
   * dismiss any geometry.
   * \post Ownership of the input \c OGRGeometry is given to the feature.
   * \post <tt>GetGeometry() == initial_geometry</tt>
   * \post The user is no longer responsible of the input geometry, even on
   * failure of the operation. In that case, the geometry would have been
   * definitivelly destroyed with the ad'hoc function (\c
   * OGRGeometryFactory::destroyGeometry()).
   * \throw itk::ExceptionObject on failure.
   * \invariant <tt>m_Feature != 0</tt>
   * \sa \c OGRFeature::SetGeometryDirectly()
   */
  void SetGeometryDirectly(UniqueGeometryPtr geometry);
  /**
   * Steals the geometry stored..
   *
   * \return A unique-pointer to a \c OGRGeometry that is now of the
   * responsibility of the caller.
   * \post <tt>GetGeometry() == 0</tt>
   * \throw None
   * \invariant <tt>m_Feature != 0</tt>
   * \sa \c OGRFeature::StealGeometry()
   * \internal
   * The \c UniqueGeometryPtr RAII wrapper will know how to correctly destroy
   * the \c OGRGeometry instance.
   */
  UniqueGeometryPtr StealGeometry();
  //@}

  /**
   * Tells whether two features are equal.
   * \return whether both are null or equal.
   * \throw None
   * \sa \c OGRFeature::Equals()
   * \internal
   * Used to determine when two \c Layer::iterator (\c Layer::feature_iter) are
   * equal.
   */
  friend bool otb::ogr::operator==(Feature const& lhs, Feature const& rhs);
  OGRFeature const* addr() const { return m_Feature.get(); }
private:
  /**\name Unchecked definitions
   * All the definitions that follow do the real work. However, they are not the
   * exposed public functions. The design of this class follows the principle
   * behind the NVI (<em>Non-Virtual Interface</em>) pattern:
   * - The public functions are inlined and check invariants and preconditions,
   * - While the private functions do the work.
   */
  //@{
  Feature            UncheckedClone() const;
  void               UncheckedSetFrom(Feature const& rhs, int *map, bool mustForgive = true);
  void               UncheckedSetFrom(Feature const& rhs, bool mustForgive = true);
  void               UncheckedPrintSelf(std::ostream &os, itk::Indent indent) const;
  Field              UncheckedGetElement(size_t index);
  Field              UncheckedGetElement(std::string const& name);
  FieldDefn          UncheckedGetFieldDefn(size_t index) const;
  FieldDefn          UncheckedGetFieldDefn(std::string const& name) const;
  int                UncheckedGetFieldIndex(std::string const& name) const;
  long               UncheckedGetFID() const;
  void               UncheckedSetFID(long fid);
  OGRFeatureDefn&    UncheckedGetDefn() const;
  void               UncheckedSetGeometryDirectly(UniqueGeometryPtr geometry);
  UniqueGeometryPtr  UncheckedStealGeometry();
  OGRGeometry const* UncheckedGetGeometry() const;
  void               UncheckedSetGeometry(OGRGeometry const* geometry);
  //@}

  /** Searches the index of a field given a name.
   * \invariant <tt>m_Feature != 0</tt>
   * \throw itk::ExceptionObject if no field named \c name exists.
   */
  int GetFieldIndex(std::string const& name) const;

  /**
   * Checks whether the internal \c OGRFeature is non null.
   * Fires an assertion otherwise.
   * \internal
   * Used by all the functions of \c Feature. As the definition of this function
   * is inlined, \c assert() actual behaviour will be conditionned by the
   * compilation mode (\em debug or \em release; or more exactly on \c NDEBUG
   * precompiler directive).
   */
  void CheckInvariants() const;
  /** Internal pointer to the shared \c OGRFeature.
   */
  boost::shared_ptr<OGRFeature> m_Feature;
  };


} // ogr namespace
} // otb namespace


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRFeatureWrapper.txx"
#endif

#endif // __otbOGRFeatureWrapper_h
