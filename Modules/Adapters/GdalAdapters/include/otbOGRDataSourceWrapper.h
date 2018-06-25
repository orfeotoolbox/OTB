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


#ifndef otbOGRDataSourceWrapper_h
#define otbOGRDataSourceWrapper_h

#include <string>

// to implement copy_const
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#include <boost/mpl/if.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/noncopyable.hpp>
#pragma GCC diagnostic pop
#else
#include <boost/mpl/if.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/noncopyable.hpp>
#endif

#include "itkDataObject.h"
#include "itkMacro.h" // itkNewMacro
#include "itkObjectFactory.h" // that should have been included by itkMacro.h

#include "otbOGRLayerWrapper.h"
#include "otbOGRVersionProxy.h"
#include "otbOGRExtendedFilenameToOptions.h"

class OGRLayer;
class OGRSpatialReference;
class OGRGeometry;
// #include "ogr_core.h" // OGRwkbGeometryType, included from Layer

namespace otb { namespace ogr {
/**\ingroup gGeometry
 * \class DataSource
 * \brief Collection of geometric objects.
 *
 * This class is meant to supersede \c otb::VectorData class.  It provides
 * an encapsulation of OGR classes. In that particular case, it's an
 * encapsulation of \c GDALDataset.
 *
 * \note Not meant to be inherited.
 * \note This class has an entity semantics: \em non-copyable, nor \em
 * assignable.
 * \note \c OGRRegisterAll() is implicitly called on construction.
 *
 * \note The following functions haven't been encapsulated (yet?):
 * - \c SetStyleTable() & \c GetStyleTable()
 * - \c SetDriver() & \c GetDriver()
 * - all functions related to the reference count.
 * \since OTB v 3.14.0
 * \internal As the class is not meant to be inherited, no new function is virtual.
 *
 * \ingroup OTBGdalAdapters
 */
 #include "OTBGdalAdaptersExport.h"
 
class OTBGdalAdapters_EXPORT DataSource : public itk::DataObject , public boost::noncopyable
  {
public:
  /**\name Standard ITK typedefs */
  //@{
  typedef DataSource                    Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  //@}

  typedef OGRExtendedFilenameToOptions FileNameHelperType;
  /**\name Standard macros */
  //@{
  /** Default builder.
   * This builder function creates a new \c DataSource with its default
   * constructor. The actual \c GDALDataset is using the <em>in-memory</em>
   * \c OGRSFDriver: \c OGRMemDriver.
   *
   * \throw itk::ExceptionObject if the inner \c GDALDataset cannot be
   * opened.
   *
   * \note \c OGRRegisterAll() is implicitly called on construction.
   * \see \c DataSource()
   */
  itkNewMacro(Self);
  itkTypeMacro(DataSource, DataObject);
  //@}
  /**\name Creation functions */
  //@{
  /**
   * I/O modes for \c DataSources.
   * \note Read/Write mode should have been <tt>read | write</tt>, but actually
   * OGR data source are always at least in read mode.
   */
  struct Modes {
    enum type
    {
      Invalid,
      Read, ///< Open data source in read-only mode
      Overwrite, ///< Open data source in overwrite mode
                 ///<
                 ///< Data source is deleted if it exists
                 ///< and a new data source is created
                 ///< Warning : this can delete a whole database
                 ///<           if the existing datasource contains a list of layers
      Update_LayerOverwrite, ///< Open data source in update mode with layers being overwritten
                             ///<
                             ///< When requesting a layer, it is opened in overwrite mode
                             ///< with OVERWRITE=YES creation option.
                             ///< If the layer does not exists, it is created on the fly
      Update_LayerUpdate,    ///< Open data source in update mode with layers being updated
                             ///<
                             ///< New geometries are added to existing layers.
                             ///< If the layer does not exists, it is created on the fly
      Update_LayerCreateOnly,///< Open data source in update mode with layers being created only
                             ///<
                             ///< This option prevents the loss of data.
                             ///< One can open an existing database with existing layer, and
                             ///< and add new layers to it.
                             ///< Only non-existing layers can be requested
      MAX__ };
  };

  /**
   * Builder from an existing named data source.
   * \param[in] datasourcename OGR identifier of the data source
   * \param[in] mode     opening mode (read or read-write)
   * \return a newly created \c DataSource.
   * \throw itk::ExceptionObject if the inner \c GDALDataset cannot be
   * opened.
   * \note \c OGRRegisterAll() is implicitly called on construction
   * \see \c DataSource(GDALDataset *)
   */
  static Pointer New(std::string const& datasourcename, Modes::type mode = Modes::Read);
  /**
   * Builder from a built \c GDALDataset.
   * \param[in,out] source  \c GDALDataset already constructed.
   * \return a newly created \c DataSource that assumes ownership of \c
   * source.
   * \throw Nothing
   * \note \c OGRRegisterAll() is supposed to have been called before building
   * \c source.
   * \note No condition is assumed on the non-nullity of \c source.
   * \see \c DataSource(GDALDataset *)
   */
  static Pointer New(ogr::version_proxy::GDALDatasetType * sourcemode, Modes::type mode = Modes::Read , const std::vector< std::string > & layerOptions = std::vector< std::string >() );
  //@}

  /**\name Projection Reference property */
  //@{
#if 0
  void SetProjectionRef(const std::string& projectionRef);
  std::string GetProjectionRef() const;
#endif
  //@}

  /** Clears the data source.
   * \post The \c GDALDataset owned is destroyed with the dedicated function
   * from OGR %API.
   * \post <tt>m_DataSource = 0</tt>
   */
  bool Clear();

  /**\name Iteration */
  //@{
  /**\ingroup gGeometry
   * \class layer_iter
   * \brief Implementation class for \c Layer iterator.
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
  template <class Value> class layer_iter
    : public boost::iterator_facade<layer_iter<Value>, Value, boost::random_access_traversal_tag, Value>
    {
    struct enabler {};

    /** Const-synchronized type of the \c DataSource container.
     * \internal
     * The definition of a new series of functions \c boost::copy_const,
     * \c boost::copy_cv, etc have been <a
     * href="http://lists.boost.org/Archives/boost/2010/03/162526.php">discussed
     * on boost mailing-list</a>. However nothing seems to have been undertaken
     * in this way, even if a <a
     * href="https://svn.boost.org/trac/boost/ticket/3970">ticket</a> has been
     * opened.
     *
     * So here is the hard-coded result of what \c boost::copy_const would have
     * given in order to avoid any licensing issue.
     */
    typedef typename boost::mpl::if_ <boost::is_const<Value> , otb::ogr::DataSource const , otb::ogr::DataSource >::type
      container_type;
  public:
    layer_iter(container_type & datasource, size_t index);
    layer_iter();

    template <class OtherValue> layer_iter(
      layer_iter<OtherValue> const& other,
      typename boost::enable_if<boost::is_convertible<OtherValue*,Value*>
      , enabler
      >::type = enabler()
    );
  private:
    friend class boost::iterator_core_access;
    template <class> friend class layer_iter;

    template <class OtherValue> bool equal(layer_iter<OtherValue> const& other) const;
    void increment();
    Value dereference() const;

    container_type * m_DataSource;
    size_t           m_index;
    };

  template <class> friend class layer_iter;
  typedef layer_iter<Layer      > iterator;
  typedef layer_iter<Layer const> const_iterator;

  const_iterator begin () const { return cbegin(); }
  const_iterator end   () const { return cend  (); }
  const_iterator cbegin() const;
  const_iterator cend  () const;
  iterator       begin ();
  iterator       end   ();
  //@}

  /** Returns the number of elements in the Data Source.
   * \param[in] doForceComputation  indicates whether the size shall be
   * computed on each layer even so it's expensive to do so.
   *
   * \return the number of features in the Data Source, -1 if count is unknown
   * \throw None
   * \sa \c OGRLayer::GetFeatureCount()
   */
  int Size(bool doForceComputation) const;

  /** Retrieves the union of the extents of all layers.
   *  \param[out] ulx reference to upper-left x coordinate of the extent
   *  \param[out] uly reference to upper-left y coordinate of the extent
   *  \param[out] lrx reference to lower-right x coordinate of the extent
   *  \param[out] uly reference to lower-right y coordinate of the extent
   *  \param[in] force Force computation of layers extents if not available. May
   *  force the driver to walk all geometries to compute the extent.
   *  \return The Wkt of the extent projection (which is the wkt of the first
   *  layer SRS)
   *  \throw itk::ExceptionObject if the layers extents can not be retrieved.
   */
  std::string GetGlobalExtent(double & ulx, double & uly, double & lrx, double & lry, bool force = false) const;

  /** Retrieves the union of the extents of all layers.
   *  \param[in] force Force computation of layers extents if not available. May
   *  force the driver to walk all geometries to compute the extent.
   *  \param[out] outwkt The Wkt of the extent projection (which is the wkt of
   *  the first layer SRS); if null, nothing is returned this way
   *  \return the extent of all layers
   *  \throw itk::ExceptionObject if the layers extents can not be retrieved.
   */
  OGREnvelope GetGlobalExtent(bool force = false, std::string * outwkt=nullptr) const;

  /** Grafts data and information from one data source to another.
   * \deprecated \c OGRLayer has an embedded input iterator. As a consequence,
   * the layer cannot support multiple access to its elements.
   *
   * This is a convenience function to setup a second data source with all the
   * meta information of another data source and use the same underlying \c
   * GDALDataset.
   */
  void Graft(const itk::DataObject *data) override;

  /**
   * Resets current data source with the one in parameter.
   * \param[in,out] source source \c GDALDataset that this instance will own.
   * \throw None
   * \post Assumes ownership of the \c source.
   */
  void Reset(ogr::version_proxy::GDALDatasetType * source);

  /**\name Layers modification */
  //@{
  /**
   * Creates a new layer.
   * \param[in] name          name for the layer
   * \param poSpatialRef      the coordinate system to use for the new layer,
   *                          or NULL if no coordinate system is available.
   * \param[in] eGType        the geometry type for the layer. Use wkbUnknown
   *                          if there are no constraints on the types
   *                          geometry to be written.
   * \param[in] papszOptions  a StringList of name=value options. Options are
   *                          driver specific.
   *
   * \return a proxy on the \c OGRLayer created.
   * \throw itk::ExceptionObject in case the layer cannot be created on the
   * data source.
   *
   * \note A \em proxy-class is returned instead of a plain \c OGRLayer is
   * order to encapsulate all lifetime management of the \c OGRLayer obtained
   * (i.e. never to be destroyed). If you want to delete a layer obtained
   * with \c CreateLayer(), you must use \c DeleteLayer().
   * \note The \c papszOptions parameter may later become a \c
   * std::vector<std::string>.
   * \sa \c GDALDataset::CreateLayer()
   */
  Layer CreateLayer(
    std::string        const& name,
    OGRSpatialReference     * poSpatialRef = nullptr,
    OGRwkbGeometryType        eGType = wkbUnknown,
    std::vector<std::string> const& papszOptions = std::vector<std::string>());

  /**
   * Deletes the i-th layer from the data source.
   * \param[in] i  layer index
   *
   * \throw itk::ExceptionObject in case the index is out of range
   * \throw itk::ExceptionObject if the layer cannot be deleted from the data
   * source.
   *
   * \pre The data source must support the delete operation.
   * \pre The index \c i must be in range [0, GetLayersCount()).
   * \sa \c GDALDataset::DeleteLayer()
   */
  void DeleteLayer(size_t i);

  /**
   * Copies a layer.
   * \param[in] srcLayer      Source layer to copy. It may come from another \c
   * DataSource.
   * \param[in] newName       Name of the new layer
   * \param[in] papszOptions  Creation options
   *
   * \return a proxy on the \c OGRLayer created.
   * \throw itk::ExceptionObject in case the layer cannot be created on the
   * data source.
   *
   * \note A \em proxy-class is returned instead of a plain \c OGRLayer in
   * order to encapsulate all lifetime management of the \c OGRLayer obtained
   * (i.e. never to be destroyed). If you want to delete a layer obtained
   * with \c CreateLayer(), you must use \c DeleteLayer().
   * \note The \c papszOptions parameter may later become a \c
   * std::vector<std::string>.
   * \sa \c GDALDataset::CopyLayer()
   */
  Layer CopyLayer(
    Layer            & srcLayer,
    std::string const& newName,
    std::vector<std::string> const& papszOptions = std::vector<std::string>() );
  //@}

  /**\name Layers access
   *\note As the following accessors are not inlined, they aren't optimized.
   */
  //@{
  /** Returns the number of layers.
   * \sa \c GDALDataset::GetLayersCount()
   */
  int GetLayersCount() const;

  /**
   * Unchecked Accessor to a given layer.
   * \param[in] i  index of the layer to access
   * \return the layer requested.
   * \pre <tt>i < GetLayersCount()</tt>, an assertion will be fired otherwise.
   * \pre The layer must be available, an assertion will be fired otherwise.
   * \throw None
   * \note Use \c GetLayerUnchecked() if invalid indices are programming
   * errors, or if null layers are to be expected.
   */
  Layer       GetLayer(size_t i);
  /**\copydoc otb::ogr::DataSource::GetLayer(size_t)
  */
  Layer const GetLayer(size_t i) const;

  /**
   * Unchecked Accessor to a given layer.
   * \param[in] name  name of the layer to search
   * \return the layer requested, possibly a null one.
   * \throw None
   * \note Use \c GetLayerChecked(std::string const&) if you'd rather have
   * an exception instead of testing whether the layer obtained is valid.
   */
  Layer       GetLayer(std::string const& name);
  /**\copydoc otb::ogr::DataSource::GetLayer(std::string const&)
  */
  Layer const GetLayer(std::string const& name) const;

  /**
   * Checked Accessor to a given layer.
   * \param[in] i  index of the layer to access
   * \return a reference to the layer requested.
   * \pre <tt>i < GetLayersCount()</tt>, an exception is raised otherwise.
   * \pre The layer must available, an exception is raised otherwise.
   * \note Use \c GetLayer() if invalid indices, and null layers, are expected
   * to be programming errors.
   * \throw None
   */
  Layer GetLayerChecked(size_t i);
  /**\copydoc otb::ogr::DataSource::GetLayerChecked()
  */
  Layer const GetLayerChecked(size_t i) const;

  /**
   * Checked Accessor to a given layer.
   * \param[in] name  name of the layer to search
   * \return the layer requested, possibly a null one.
   * \throw itk::ExceptionObject if there exist no layer by that name
   * \note Use \c GetLayer(std::string const&) if you'd rather test the
   * obtained layer instead of catching an exception.
   */
  Layer       GetLayerChecked(std::string const& name);
  /**\copydoc otb::ogr::DataSource::GetLayerChecked(std::string const&)
  */
  Layer const GetLayerChecked(std::string const& name) const;

  /**
   * Executes the statement..
   * \param[in] statement  textual description of the SQL statement.
   * \param[in] poSpatialFilter  \c Geometry representing a spatial filter -- may be null.
   * \param[in] pszDialect  allows control of the statement dialect. If set to
   *                     NULL, the OGR SQL engine will be used, except for
   *                     RDBMS drivers that will use their dedicated SQL
   *                     engine, unless OGRSQL is explicitly passed as the
   *                     dialect.
   * \return a new \c Layer that contains the matching \c Features. In case of
   * error, or no matching result sets, a \em null Layer will be returned.
   * Check for \c Layer's validity before doing anything else.
   * \throw None even when there is an error -- OGR can not report errors,
   * neither this wrapping.
   * \note The returned \c Layer will be automatically collected on its
   * destruction; i.e. unlike OGR API, no need to explicitly call \c
   * GDALDataset::ReleaseResultSet().
   * \sa \c GDALDataset::ExecuteSQL()
   */
  Layer ExecuteSQL(
    std::string const& statement,
    OGRGeometry *      poSpatialFilter,
    char        const* pszDialect);

  //@}


  struct boolean{ int i; };
  /** Can the data source be used (ie not null).
   *
   * Hack to provide a boolean operator that is convertible only to a
   * boolean expression to be used in \c if tests.
   * \see <em>Imperfect C++</em>, Matthew Wilson, Addisson-Welsey, par 24.6
   */
  operator int boolean ::* () const {
    return m_DataSource ? &boolean::i : nullptr;
    }

  /** Flushes all changes to disk.
   * \throw itd::ExceptionObject in case the flush operation failed.
   * \sa \c GDALDataset::SyncToDisk()
   */
  void SyncToDisk();

  /** Returns whether a capability is avalaible.
   * \param[in] capabilityName  name of the capability to check.
   * \throw None
   * \sa \c GDALDataset::TestCapability()
   */
  bool HasCapability(std::string const& capabilityName) const;

  /** Access to raw \c GDALDataset.
   * This function provides an abstraction leak in case deeper control on the
   * underlying \c GDALDataset is required.
   * \pre The underlying \c GDALDataset must be valid, i.e.
   * <tt>m_DataSource != 0</tt>, an assertion is fired otherwise.
   * \warning You must under no circumstance try to delete the \c GDALDataset
   * obtained this way.
   */
    ogr::version_proxy::GDALDatasetType & ogr();

    void SetLayerCreationOptions( const std::vector< std::string > & options );
    void AddLayerCreationOptions( std::vector< std::string > options );
    const std::vector< std::string > & GetLayerCreationOptions() const ;

protected:
  /** Default constructor.
   * The actual \c GDALDataset is using the <em>in-memory</em> \c
   * OGRSFDriver: \c OGRMemDriver.
   * \throw itk::ExceptionObject if the inner \c GDALDataset cannot be
   * opened.
   *
   * \note \c OGRRegisterAll() is implicitly called on construction
   * \see \c DataSource::New()
   */
  DataSource();
  /** Init constructor.
   * \post The newly constructed object owns the \c source parameter.
   */
  DataSource(ogr::version_proxy::GDALDatasetType * source, Modes::type mode , const std::vector< std::string > & layerOption = std::vector< std::string >() );
  /** Destructor.
   * \post The \c GDALDataset owned is released (if not null).
   */
  ~DataSource() override;

  static Pointer OpenDataSource(std::string const& datasourceName, Modes::type mode);

  /** Prints self into stream. */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  /**
   * Internal unchecked accessor to a given layer.
   * \param[in] i  index of the layer to access
   * \return a reference to the layer requested.
   * \pre <tt>i < GetLayersCount()</tt>, return 0 otherwise
   * \pre The layer must available, 0 is returned otherwise.
   * \throw None
   * \internal this function is a simple encapsulation of \c
   * GDALDataset::GetLayer().
   */
  OGRLayer* GetLayerUnchecked(size_t i);
  /** @copydoc OGRLayer* otb::ogr::DataSource::GetLayerUnchecked(size_t i)
   */
  OGRLayer* GetLayerUnchecked(size_t i) const;

  bool IsLayerModifiable(size_t i) const;
  bool IsLayerModifiable(std::string const& name) const;

  size_t GetLayerID(std::string const& name) const;
  int GetLayerIDUnchecked(std::string const& name) const;

  /** Get a string describing the dataset */
  std::string GetDatasetDescription() const;

private:
  ogr::version_proxy::GDALDatasetType *m_DataSource;
  std::vector< std::string > m_LayerOptions;
  Modes::type    m_OpenMode;
  int            m_FirstModifiableLayerID;
  }; // end class DataSource
} } // end namespace otb::ogr

#if 0
// Either this, or inheriting from noncopyable is required for DataSource to be
// compatible with BOOST_FOREACH
namespace boost { namespace foreach {
  template<typename T> struct is_noncopyable; // forward declaration
  template <>
  struct is_noncopyable<otb::ogr::DataSource> : mpl::true_ {};
}}
#endif

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRDataSourceWrapper.hxx"
#endif

#endif // otbOGRDataSourceWrapper_h
