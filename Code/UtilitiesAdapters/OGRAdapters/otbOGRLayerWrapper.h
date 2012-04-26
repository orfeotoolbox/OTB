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
#ifndef __otbOGRLayerWrapper_h
#define __otbOGRLayerWrapper_h

// #include <iosfwd> // std::ostream&
#include <boost/shared_ptr.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/utility/enable_if.hpp>
#include "itkIndent.h"
#include "otbOGRFeatureWrapper.h"

#include "ogr_core.h" // OGRwkbGeometryType
// Forward declarations
class OGRLayer;
class OGRDataSource;
class OGRGeometry;
class OGRFeatureDefn;
class OGRFieldDefn;

namespace otb { namespace ogr {
/**\ingroup Geometry
 * \class Layer
 * \brief %Layer of geometric objets.
 *
 * It provides an encapsulation of OGR classes. In that particular case, it's an
 * encapsulation of \c OGRLayer.
 *
 * \note this class is a proxy class on top of an \c OGRLayer.
 * \note It can be copied, and assigned. NEw instances will share the underlying
 * \c OGRLayer.
 * \note when created from a \c otb::ogr::DataSource::ExecuteSQL, it will
 * automatically manage the release of the underlying \c OGRLayer.
 * \note the default constructor is disabled on purpose
 * \note the destructor automatically generated does everything that is
 * expected.
 *
 * \todo find a way to be notified when the related \c OGRDataSource is released
 */
class Layer
  {
public:
  /**\name ITK class typedefs */
  //@{
  typedef Layer                         Self;
  //@}

  /**\name Construction */
  //@{
  /**
   * Init constructor with a layer owned by a DataSource.
   * \param layer \c OGRLayer instance that is owned by a DataSource.
   * \throw None
   * On destruction of the proxy class, the internal \c OGRLayer is left alone.
   *
   * @warning if the datasource hosting the layer (built with this constructor)
   * is deleted, the layer won't be usable anymore. Unfortunatelly, there is no
   * mean to report this to this layer proxy.
   */
  explicit Layer(OGRLayer* layer);

  /**
   * Init constructor for layers that need to be released.
   * \param layer  \c OGRLayer owned by the client code.
   * \param sourceInChargeOfLifeTime  reference to the actual \c OGRDataSource
   * that knows how to release the layer.
   *
   * \throw None
   * \internal
   * This constructor is meant to be used for wrapping layers coming from \c
   * OGRDataSource::ExecuteSQL(). It's actually the constructor called by \c
   * DataSource::ExecuteSQL().
   */
  Layer(OGRLayer* layer, OGRDataSource& sourceInChargeOfLifeTime);
  //@}

  /**\name Features collection */
  //@{
  /** Returns the number of elements in the layer.
   * \param[in] doForceCompuation  indicates whether the size shall be computed
   * even so it's expensive to do so.
   *
   * \return the number of features in the layer, -1 if count is unknown
   * \throw None
   * \sa OGRLayer::GetFeatureCount
   */
  int GetFeatureCount(bool doForceComputation) const;

  /**
   * Adds a pre-existing \c Feature to the layer.
   * \param[in,out] feature feature to add. Upon successful completion, the feature
   * id will be updated (unless it was previously set)
   *
   * \throw itk::ExceptionObject if the feature can't be added.
   * \sa OGRLayer::CreateFeature
   * \internal
   * Whilst the \c Feature id is updated, it is not the same feature than the
   * one stored in the layer. In other words, \c Feature is still in charge of
   * the actual \c OGRFeature (in case it was), and the feature added is of the
   * responsibility of the layer.
   */
  void CreateFeature(Feature feature);

  /**
   * Removes a feature identified by its id from the \c Layer.
   * \param[in] nFID  feature id.
   *
   * \throw itk::ExceptionObject if the feature can't be added.
   * \warning calls to this function will invalidate any feature iterator
   * previously obtained.
   * \sa OGRFeature::DeleteFeature
   */
  void DeleteFeature(long nFID);

  /**
   * Finds a feature from its id.
   * \param[in] nFID  feature id.
   *
   * \return a RAII capsule around the \c OGRFeature stored in the layer and
   * that matches the requested id.
   * \throw itk::ExceptionObject if nFID is null
   *
   * \pre \c nFID value cannot be \c OGRNullFID
   * \post result's \c GetFID() equals \c nFID
   * \warning calls to this function will invalidate any feature iterator
   * previously obtained.
   * \sa OGRFeature::GetFeature
   * \internal
   * The feature obtained is owned by the \c Feature instance.
   */
  Feature GetFeature(long nFID);

  /**
   * Changes a \c Feature in the Layer.
   * \param[in,out] feature feature to set. Upon successful completion, the feature
   * id will be updated (in case it was previously set)
   *
   * \throw itk::ExceptionObject if the feature can't be set.
   * \pre the Layer need to support <em>OLCRandomWrite</em> capability.
   * \sa OGRLayer::SetFeature
   * \internal
   * Whilst the \c Feature id is updated, it is not the same feature than the
   * one stored in the layer. In other words, \c Feature is still in charge of
   * the actual \c OGRFeature (in case it was), and the feature added is of the
   * responsibility of the layer.
   */
  void SetFeature(Feature feature);
  //@}

  /** Returns the name given to the layer, if any.
  */
  std::string GetName() const;

  /** Prints self into stream. */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;


  /**\copydoc operator int boolean ::* () const
  */
  struct boolean{ int i; };
  /** Can the layer be used (ie not null).
   *
   * Hack to provide a boolean operator that is convertible only to a
   * boolean expression to be used in \c if tests.
   * @see <em>Imperfect C++</em>, Matthew Wilson, Addisson-Welsey, par 24.6
   */
  operator int boolean ::* () const
    {
    return m_Layer ? &boolean::i : 0;
    }

  /** Access to raw \c OGRLayer.
   * This function provides an abstraction leak in case deeper control on the
   * underlying \c OGRLayer is required.
   * \pre the underlying \c OGRLayer must be valid, i.e.
   * <tt>m_Layer != 0</tt>, an assertion is fired otherwise.
   * \warning you must under no circonstance try to delete the \c OGRLayer
   * obtained this way.
   */
  OGRLayer & ogr();

  /**\name Spatial filter property
   * \todo we'll see later if a Geometry capsule is defined, or a
   * \c nondeletable<> pointer type.
   * \internal
   * The I/O geometry is an undeletable pointer, that may be null (hence the
   * need of pointers instead of references).
   */
  //@{
  /**
   * Returns a reference to the current spatial filter, if any.
   *
   * \return a reference to the current spatial filter. Spatial filter that isn't
   * supposed to be modified this way. Use \c SetSpatialFilter or \c
   * SetSpatialFilterRect for this purpose.
   * \throw None
   * \sa OGRLayer::GetSpatialFilter
   */
  OGRGeometry const* GetSpatialFilter() const;

  /**
   * Sets the current spatial filter.
   * \param[in] spatialFilter  new spatial filter definition, NULL clears the
   * filter.
   * \throw None
   * Replaces the current spatial filter with a clone of the one passed as
   * parameter. Thus parameter remains the responsibility of the caller.
   *
   * The spatial filter is used to filter the \c Feature's obtained when iterating
   * on the layer.
   * \note OGR warns us that the test may be incorrectly implemented, and that
   * we may have false-positives, but no missed shapes.
   * \sa OGRLayer::SetSpatialFilter
   */
  void SetSpatialFilter(OGRGeometry const* spatialFilter);
  /** Sets a new rectangular spatial filter.
   * Defines the new filter as a rectangular shape.
   *
   * The coordinates used shall be in the same referential as the layer as the
   * whole (as returned by \c GetSpatialRef()).
   * \sa OGRLayer::SetSpatialFilterRect
   */
  void SetSpatialFilterRect(double dfMinX, double dfMinY, double dfMaxX, double dfMaxY);
  //@}

  /**Spatial Reference property.
   * \internal the I/O spatial reference is an undeletable pointer, that may be null.
   * \note read-only property
   */
  OGRSpatialReference const* GetSpatialRef() const;

  /**\name Iteration */
  //@{
  /**\ingroup Geometry
   * \class feature_iter
   * Implementation class for \c Feature iterator.
   * This iterator is a single <em>pass iterator</em>. We may fetch the \c
   * Feature referenced by an iterator previously stored, but never resume the
   * iteration after a call to \c Layer::begin(), \c Layer::start(), \c
   * Layer::CreateFeature(), \c Layer::DeleteFeature(), \c Layer::GetFeature(),
   * \c Layer::SetFeature(), nor fork the iteration.
   * \code
   * iterator b = begin();
   * iterator i = std::advance(b, 1);
   * b++; // this is invalid
   * \endcode
   * \internal
   * \sa otb::ogr::Layer::iterator
   * \sa otb::ogr::Layer::const_iterator
   * \note Naming policy is compliant with C++ standard as the iterator are as
   * well. This will permit transparent integration with all standard and boost
   * algorithms, and C++11 <em>for-range loops</em> for instance.
   * \see http://www.boost.org/doc/libs/1_49_0/libs/iterator/doc/iterator_facade.html#tutorial-example
   */
  template <class Value> class feature_iter
    : public boost::iterator_facade<feature_iter<Value>, Value, boost::single_pass_traversal_tag>
    {
    struct enabler {};
  public:
    feature_iter()
      : m_Layer(0), m_Crt(0) {}
    explicit feature_iter(otb::ogr::Layer & layer)
      : m_Layer(&layer), m_Crt(layer.GetNextFeature()) {}
    template <class OtherValue> feature_iter(
      feature_iter<OtherValue> const& other,
      typename boost::enable_if<boost::is_convertible<OtherValue*,Value*>
      , enabler
      >::type = enabler()
    )
      : m_Layer(other.m_Layer), m_Crt(other.m_Crt)
      {}
  private:
    friend class boost::iterator_core_access;
    template <class> friend class feature_iter;

    template <class OtherValue> bool equal(feature_iter<OtherValue> const& other) const
      { return other.m_Crt == m_Crt; }
    void increment()
      {
      assert(m_Layer && "cannot increment end()");
      m_Crt = m_Layer->GetNextFeature();
      }
    Value & dereference() const
      { return m_Crt; }

    otb::ogr::Layer * m_Layer;
    otb::ogr::Feature m_Crt;
    };

  template <class> friend class feature_iter;
  /// Features %iterator.
  typedef feature_iter<Feature      > iterator;
  /// Features const %iterator.
  typedef feature_iter<Feature const> const_iterator;

  /** Returns a <em>single-pass</em> %iterator to the start of the sequence.
   * \sa feature_iter
   */
  const_iterator cbegin() const;
  /** Returns the %end %iterator of the sequence.
   */
  const_iterator cend  () const { return iterator(); }
  /**\copydoc cbegin */
  const_iterator begin () const { return cbegin(); }
  /**\copydoc cend */
  const_iterator end   () const { return cend  (); }
  /**\copydoc cbegin */
  iterator       begin ();
  /**\copydoc cend */
  iterator       end   () { return iterator(); }

  /** Returns a <em>single-pass</em> %iterator to the i-th \c Feature of the
   * sequence.
   * \sa feature_iter
   * Depending of the actual driver (i.e. \c OGRDriver), this may be done in O(N).
   */
  const_iterator cstart_at(size_t index) const;
  /** \copydoc cstart_at */
  const_iterator start_at (size_t index) const { return cstart_at(index); }
  /** \copydoc cstart_at */
  iterator       start_at (size_t index);
  //@}

  /**\name Features definition
   * \todo shall we instead inhibit the declaration of the functions when GDAL
   * version does not match?
   */
  //@{
  /** Returns a reference to the layer definition.
   * @warning the definition obtained shall not be modified. Use the \c *Field
   * functions instead.
   * \internal
   * The return type shall have been const, but unfortunatelly \c OGRFeatureDefn
   * is not const-correct.
   * \sa OGRLayer::GetLayerDefn
   */
  OGRFeatureDefn & GetLayerDefn() const;

  /**
   * Adds a new field given its definition.
   * \param[in] field  field definition
   * \param[in] bApproxOK  If true, the field may be created in a slightly
   * different form depending on the limitations of the format driver.
   *
   * \pre This function shall not be called while there are \c Feature in
   * existance that were obtained or created with the previous layer definition.
   * \throw itk::ExceptionObject if the new field cannot be created
   * \sa OGRLayer::CreateField
   * \todo move to use \c otb::ogr::FieldDefn
   */
  void CreateField(OGRFieldDefn const& field, bool bApproxOK = true);

  /**
   * Deletes a field.
   * \param[in] fieldIndex  index of the field to remove.
   *
   * \pre This function shall not be called while there are \c Feature in
   * existance that were obtained or created with the previous layer definition.
   * \throw itk::ExceptionObject if the new field cannot be deleted
   * \sa OGRLayer::DeleteField
   * \pre to be available, this function requires OTB to be compiled against OGR
   * v1.9.0 at least.
   */
  void DeleteField(size_t fieldIndex);

  /**
   * Changes the definition of the i-th field.
   * \param[in] fieldIndex  index of the field to change
   * \param[in,out] newFieldDefn  definition of the new field.
   * \param[in] nFlags  combination of \c ALTER_NAME_FLAG, \c ALTER_TYPE_FLAG
   * and \c ALTER_WIDTH_PRECISION_FLAG to indicate which of the name and/or type
   * and/or width and precision fields from the new field definition must be
   * taken into account.
   *
   * \pre This function shall not be called while there are \c Feature in
   * existance that were obtained or created with the previous layer definition.
   * \throw itk::ExceptionObject if the new field cannot be modified
   * \sa OGRLayer::AlterFieldDefn
   * \pre to be available, this function requires OTB to be compiled against OGR
   * v1.9.0 at least.
   * \todo move to use \c otb::ogr::FieldDefn
   */
  void AlterFieldDefn(size_t fieldIndex, OGRFieldDefn const& newFieldDefn, int nFlags);

  /**
   * Moves a field from one position to another.
   * \param[in] oldPos  old field index position
   * \param[in] newPos  new field index position
   *
   * \pre This function shall not be called while there are \c Feature in
   * existance that were obtained or created with the previous layer definition.
   * \throw itk::ExceptionObject if the new field cannot be modified
   * \sa OGRLayer::ReorderField
   * \pre to be available, this function requires OTB to be compiled against OGR
   * v1.9.0 at least.
   */
  void ReorderField(size_t oldPos, size_t newPos);

  /**
   * Reorder all the fields of the layer.
   * \param[in] map array that tells the new position of each field.
   *
   * \pre This function shall not be called while there are \c Feature in
   * existance that were obtained or created with the previous layer definition.
   * \throw itk::ExceptionObject if the new field cannot be modified
   * \sa OGRLayer::ReorderFields
   * \pre to be available, this function requires OTB to be compiled against OGR
   * v1.9.0 at least.
   */
  void ReorderFields(int *map);

  /**
   * Sets which fields can be omitted when retrieving features from the layer.
   * \param[in] fieldNames 0-terminated array of the field names to ignore when
   * fetching features from the layer. 0 to clear the list.
   * end
   *
   * Besides field names of the layers, the following special fields can be
   * passed: "OGR_GEOMETRY" to ignore geometry and "OGR_STYLE" to ignore layer
   * style.
   *
   * By default, no fields are ignored.
   * \throw itk::ExceptionObject if the new field cannot be modified
   * \sa OGRLayer::SetIgnoredFields
   */
  void SetIgnoredFields(char const** fieldNames);
  //@}

  /** Returns the type of the geometry stored.
   * \sa OGRLayer::GetGeomType
   */
  OGRwkbGeometryType GetGeomType() const;
private:
  /**
   * Internal encapsulation of \c OGRLayer::GetNextFeature().
   *
   * \return the next \c OGRFeature of the layer, encapsulated in a \c Feature.
   * \throw None
   * \internal
   * Required to implement iterators without exposing/including \c OGRFeature
   * class definition.
   */
  Feature GetNextFeature();

  /** Data implementation.
   * \internal
   * The actual %layer implementation belongs to the \c otb::Layer object,
   * unless this is the result of \c ExecuteSQL. In that case a deleter is set
   * to correctly release the layer.
   */
  boost::shared_ptr<OGRLayer> m_Layer;
  };

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbLayerWrapper.txx"
#endif

#endif // __otbOGRLayerWrapper_h
