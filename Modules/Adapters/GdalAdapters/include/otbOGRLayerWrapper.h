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
// #include "itkIndent.h", included from field
#include "otbOGRFeatureWrapper.h"
#include "otbOGRVersionProxy.h"

// #include "ogr_core.h" // OGRwkbGeometryType, included from feature -> field
// Forward declarations
class OGRLayer;
class OGRGeometry;
class OGRFeatureDefn;

namespace otb { namespace ogr {
class DataSource;
class Layer;

/**\ingroup gGeometry
 * Compares layers identities.
 * \return whether the two layers are in fact the same.
 */
bool operator==(Layer const& lhs, Layer const& rhs);

/**\ingroup gGeometry
 * \class Layer
 * \brief %Layer of geometric objets.
 *
 * It provides an encapsulation of OGR classes. In that particular case, it's an
 * encapsulation of \c OGRLayer.
 *
 * \note This class is a proxy class on top of an \c OGRLayer.
 * \note It can be copied, and assigned. New instances will share the underlying
 * \c OGRLayer.
 * \note When created from a \c otb::ogr::DataSource::ExecuteSQL(), it will
 * automatically manage the release of the underlying \c OGRLayer.
 * \note The default constructor is disabled on purpose.
 * \note The destructor automatically generated does everything that is
 * expected.
 *
 * \todo Find a way to be notified when the related \c OGRDataSource is released
 * \since OTB v 3.14.0
 *
 * \ingroup OTBGdalAdapters
 */
class Layer
  {
public:
  /**\name ITK standard definitions */
  //@{
  typedef Layer                         Self;
  const char *GetNameOfClass() const {return "Layer"; }
  //@}

#if 0
  typedef itk::SmartPointer<DataSource> DataSourcePtr;
#endif

  /**\name Construction */
  //@{
  /**
   * Init constructor with a layer owned by a DataSource.
   * \param layer \c OGRLayer instance that is owned by a DataSource.
   * \param datasource Pointer to the actual data source.
   * \throw None
   * On destruction of the proxy class, the internal \c OGRLayer is left alone.
   *
   * \warning If the datasource hosting the layer (built with this constructor)
   * is deleted, the layer won't be usable anymore. Unfortunatelly, there is no
   * mean to report this to this layer proxy.
   */
  Layer(OGRLayer* layer, bool modifiable);

  /**
   * Init constructor for layers that need to be released.
   * \param layer  \c OGRLayer owned by the client code.
   * \param sourceInChargeOfLifeTime  reference to the actual \c GDALDataset
   * that knows how to release the layer.
   * \post In this case, \c m_datasource is left null: we suppose (for now, that
   * the layer won't need access to the datasource meta-information).
   *
   * \throw None
   * \internal
   * This constructor is meant to be used for wrapping layers coming from \c
   * OGRDataSource::ExecuteSQL(). It's actually the constructor called by \c
   * DataSource::ExecuteSQL().
   */
    Layer(OGRLayer* layer, otb::ogr::version_proxy::GDALDatasetType& sourceInChargeOfLifeTime, bool modifiable);
  //@}

  /**\name Features collection */
  //@{
  /** Returns the number of elements in the layer.
   * \param[in] doForceCompuation  indicates whether the size shall be computed
   * even so it's expensive to do so.
   *
   * \return the number of features in the layer, -1 if count is unknown
   * \throw None
   * \sa \c OGRLayer::GetFeatureCount()
   */
  int GetFeatureCount(bool doForceComputation) const;

  /**
   * Adds a pre-existing \c Feature to the layer.
   * \param[in,out] feature feature to add. Upon successful completion, the feature
   * id will be updated (unless it was previously set)
   *
   * \throw itk::ExceptionObject if the feature can't be added.
   * \sa \c OGRLayer::CreateFeature()
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
   * \warning Calls to this function will invalidate any feature iterator
   * previously obtained.
   * \sa \c OGRFeature::DeleteFeature()
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
   * \post Result's \c GetFID() equals \c nFID
   * \warning Calls to this function will invalidate any feature iterator
   * previously obtained.
   * \sa \c OGRFeature::GetFeature()
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
   * \pre The Layer need to support <em>OLCRandomWrite</em> capability.
   * \sa \c OGRLayer::SetFeature()
   * \warning Calls to this function may invalidate any feature iterator
   * previously obtained depending on the actual \c OGRDriver.
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

  /** Retrieves the extent of the layer.
   *  \param[in] force Force computation of the extent if not available. May
   *  force the driver to walk all geometries to compute the extent.
   *  \return the extent of the layer
   *  \throw itk::ExceptionObject if the extent can not be retrieved.
  */
  OGREnvelope GetExtent(bool force = false) const;

  /** Retrieves the extent of the layer.
   *  \param[out] ulx reference to upper-left x coordinate of the extent
   *  \param[out] uly reference to upper-left y coordinate of the extent
   *  \param[out] lrx reference to lower-right x coordinate of the extent
   *  \param[out] uly reference to lower-right y coordinate of the extent
   *  \param[in] force Force computation of the extent if not available. May
   *  force the driver to walk all geometries to compute the extent.
   *  \throw itk::ExceptionObject if the extent can not be retrieved.
  */
  void GetExtent(double & ulx, double & uly, double & lrx, double & lry, bool force = false) const;

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
   * \pre The underlying \c OGRLayer must be valid, i.e.
   * <tt>m_Layer != 0</tt>, an assertion is fired otherwise.
   * \warning You must under no circonstance try to delete the \c OGRLayer
   * obtained this way.
   */
  OGRLayer & ogr();

  /**\name Spatial filter property
   * \todo We'll see later if a Geometry capsule is defined, or a
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
   * \sa \c OGRLayer::GetSpatialFilter()
   */
  OGRGeometry const* GetSpatialFilter() const;

  /**
   * Sets the current spatial filter.
   * Replaces the current spatial filter with a clone of the one passed as
   * parameter. Thus the parameter remains of the responsibility of the caller.
   *
   * The spatial filter is used to filter the \c Feature's obtained when iterating
   * on the layer.
   * \param[in] spatialFilter  new spatial filter definition, NULL clears the
   * filter.
   * \throw None
   * \note OGR warns us that the test may be incorrectly implemented, and that
   * we may have false-positives, but no missed shapes.
   * \sa \c OGRLayer::SetSpatialFilter()
   */
  void SetSpatialFilter(OGRGeometry const* spatialFilter);
  /** Sets a new rectangular spatial filter.
   * Defines the new filter as a rectangular shape.
   *
   * The coordinates used shall be in the same referential as the layer as the
   * whole (as returned by \c GetSpatialRef()).
   * \sa \c OGRLayer::SetSpatialFilterRect()
   */
  void SetSpatialFilterRect(double dfMinX, double dfMinY, double dfMaxX, double dfMaxY);
  //@}

  /** Spatial Reference property.
   * \note Read-only property. In order to set this property, you'll have to
   * create a new layer with a spatial reference.
   * \internal the I/O spatial reference is an undeletable pointer, that may be null.
   */
  OGRSpatialReference const* GetSpatialRef() const;

  /** Returns the projection ref associated with the layer.
   * \return The projection ref (wkt string) associated with the layer
   */
  std::string GetProjectionRef() const;

  /**\name Iteration */
  //@{
  /**\ingroup gGeometry
   * \class feature_iter
   * \brief Implementation class for \c Feature iterator.
   * This iterator is a single <em>pass iterator</em>. We may fetch the \c
   * Feature referenced by an iterator previously stored, but never resume the
   * iteration after a call to \c Layer::begin(), \c Layer::start_at(), \c
   * Layer::CreateFeature(), \c Layer::DeleteFeature(), \c Layer::GetFeature(),
   * \c Layer::SetFeature(), nor fork the iteration.
   * \code
   * iterator b = begin();
   * iterator i = std::advance(b, 1);
   * b++; // this is invalid
   * \endcode
   * \internal
   * \sa \c otb::ogr::Layer::iterator
   * \sa \c otb::ogr::Layer::const_iterator
   * \note Naming policy is compliant with C++ standard as the iterator
   * functions are as well. This will permit transparent integration with all
   * standard and boost algorithms, and C++11 <em>for-range loops</em> for
   * instance.
   * \see http://www.boost.org/doc/libs/1_49_0/libs/iterator/doc/iterator_facade.html#tutorial-example
   * \since OTB v 3.14.0
 *
 * \ingroup OTBGdalAdapters
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
      typename boost::enable_if<boost::is_convertible<OtherValue*,Value*> >::type* = 0
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

    otb::ogr::Layer         * m_Layer;
    otb::ogr::Feature mutable m_Crt;
    };

  template <class> friend class feature_iter;
  /// Features %iterator.
  typedef feature_iter<Feature      > iterator;
  /// Features const %iterator.
  typedef feature_iter<Feature const> const_iterator;

  /** Returns a <em>single-pass</em> %iterator to the start of the sequence.
   * \sa \c feature_iter
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
   * \sa \c feature_iter
   * Depending of the actual driver (i.e. \c OGRDriver), this may be done in O(N).
   */
  const_iterator cstart_at(GIntBig index) const;
  /** \copydoc cstart_at */
  const_iterator start_at (GIntBig index) const { return cstart_at(index); }
  /** \copydoc cstart_at */
  iterator       start_at (GIntBig index);
  //@}

  /**\name Features definition
   * \todo Shall we instead inhibit the declaration of the functions when GDAL
   * version does not match?
   */
  //@{
  /** Returns a reference to the layer definition.
   * \warning The definition obtained shall not be modified. Use the \c *Field
   * functions instead.
   * \internal
   * The return type shall have been const, but unfortunatelly \c OGRFeatureDefn
   * is not const-correct.
   * \sa \c OGRLayer::GetLayerDefn()
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
   * \sa \c OGRLayer::CreateField()
   * \warning Calls to this function may invalidate any feature iterator
   * previously obtained depending on the actual \c OGRDriver.
   * \todo Move to use \c otb::ogr::FieldDefn
   */
  void CreateField(FieldDefn const& field, bool bApproxOK = true);

  /**
   * Deletes a field.
   * \param[in] fieldIndex  index of the field to remove.
   *
   * \pre This function shall not be called while there are \c Feature in
   * existance that were obtained or created with the previous layer definition.
   * \throw itk::ExceptionObject if the new field cannot be deleted
   * \sa \c OGRLayer::DeleteField()
   * \pre To be available, this function requires OTB to be compiled against OGR
   * v1.9.0 at least.
   */
  void DeleteField(int fieldIndex);

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
   * \sa \c OGRLayer::AlterFieldDefn()
   * \pre To be available, this function requires OTB to be compiled against OGR
   * v1.9.0 at least.
   * \todo Move to use \c otb::ogr::FieldDefn
   */
  void AlterFieldDefn(size_t fieldIndex, FieldDefn const& newFieldDefn, int nFlags);

  /**
   * Moves a field from one position to another.
   * \param[in] oldPos  old field index position
   * \param[in] newPos  new field index position
   *
   * \pre This function shall not be called while there are \c Feature in
   * existance that were obtained or created with the previous layer definition.
   * \throw itk::ExceptionObject if the new field cannot be modified
   * \sa \c OGRLayer::ReorderField()
   * \pre To be available, this function requires OTB to be compiled against OGR
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
   * \sa \c OGRLayer::ReorderFields()
   * \pre To be available, this function requires OTB to be compiled against OGR
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
   * \sa \c OGRLayer::SetIgnoredFields()
   */
  void SetIgnoredFields(char const** fieldNames);
  //@}

  /** Returns the type of the geometry stored.
   * \sa \c OGRLayer::GetGeomType()
   */
  OGRwkbGeometryType GetGeomType() const;

  friend bool otb::ogr::operator==(Layer const& lhs, Layer const& rhs);

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
   * unless this is the result of \c ExecuteSQL(). In that case a deleter is set
   * to correctly release the layer.
   */
  boost::shared_ptr<OGRLayer> m_Layer;

  bool m_Modifiable;

#if 0
  /** Related DataSource.
   * Needed to acces OTB meta information.
   */
  DataSourcePtr m_DataSource;
#endif
  };

inline bool operator!=(Layer const& lhs, Layer const& rhs)
  {
  return ! (lhs == rhs);
  }

} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbLayerWrapper.txx"
#endif

#endif // __otbOGRLayerWrapper_h
