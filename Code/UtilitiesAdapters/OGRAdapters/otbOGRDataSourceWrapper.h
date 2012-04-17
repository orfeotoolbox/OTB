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
#ifndef __otbOGRDataSourceWrapper_h
#define __otbOGRDataSourceWrapper_h

#include <string>


#include "itkVector.h"
#include "itkPoint.h"
#include "itkDataObject.h"
#include "itkMacro.h" // itkNewMacro
#include "itkObjectFactory.h" // that should have been included by itkMacro.h

#include "otbOGRLayerWrapper.h"

class OGRDataSource;
class OGRLayer;
class OGRSpatialReference;
class OGRGeometry;
#include "ogr_core.h" // OGRwkbGeometryType

namespace otb { namespace ogr {
  /**\ingroup Geometry
   * \class ImageReference
   * \todo see how mix it with the \c otb::ogr::DataSource wrapper as it was
   * with \c VectorData.
   */
  template <typename TPrecision>
    class ImageReference
      {
    public:
      typedef TPrecision PrecisionType;
      enum { Dimension = 2 };
      /**\name Standard ITK typedefs */
      //@{
      //@}
      /**\name Template-parameters typedefs */
      //@{
      typedef itk::Vector<PrecisionType, 2> SpacingType;
      typedef itk::Point<PrecisionType, 2>  OriginType;
      typedef itk::Point<PrecisionType, 2>  PointType;
      //@}

      /** Default constructor.
       * \post <tt>m_Spacing = {1,1}</tt>
       * \post <tt>m_Origin = {0,0}</tt>
       */
      ImageReference()
        {
        m_Spacing.Fill(1);
        m_Origin.Fill(0);
        }
      /** Init constructor.
       * \post <tt>m_Spacing = spacing</tt>
       * \post <tt>m_Origin = origin</tt>
       */
      ImageReference(SpacingType const& spacing, OriginType const& origin)
        : m_Spacing(spacing), m_Origin(origin)
        {
        }

      /**\name Origin property
       * Represents the origin of the geometries in the image coordinates system.
       */
      //@{
      itkGetConstReferenceMacro(Origin, OriginType);     //!< Origin getter.
      itkSetMacro(Origin, OriginType);                   //!< Origin setter.
      void SetOrigin(const TPrecision origin[Dimension]) //!< Origin setter.
        {
        const OriginType p(origin);
        this->SetOrigin(p);
        }
      //@}

      /**\name Spacing property
       * Spacing of the geometries to put in the corresponding image coordinates.
       */
      //@{
      itkGetConstReferenceMacro(Spacing, SpacingType);         //!< Spacing getter.
      void SetSpacing(const SpacingType& spacing)              //!< Spacing setter.
        {
        itkDebugMacro("setting Spacing to " << spacing);
        if (this->m_Spacing != spacing)
          {
          this->m_Spacing = spacing;
          this->Modified();
          }
        }
      void SetSpacing(const TPrecision spacing[Dimension]) //!< Spacing setter.
        {
        const SpacingType s(spacing);
        this->SetSpacing(s);
        }
      //@}

      /**
       * Projects a point from the Data Source coordinates system to the image
       * coordinates system.
       * \param[in] point  point in Data Source coordinates system
       * \param[out] physicalPoint  point in the image coordinates system.
       * \throw None
       */
      void TransformPointToPhysicalPoint(const PointType& point, PointType& physicalPoint) const
        {
        for (size_t i=0; i!=Dimension; ++i)
          physicalPoint[i] = point[i] * m_Spacing[i] + m_Origin[i];
        }

      /**
       * Projects a point from the Data Source coordinates system to the image
       * coordinates system.
       * \param[in] point  point in Data Source coordinates system
       * \return the point projected in the image coordinates system.
       * \throw None
       */
      void TransformPointToPhysicalPoint(const PointType& point) const
        {
        // why no loop on VDimension ?
        PointType physicalPoint;
        for (size_t i=0; i!=Dimension; ++i)
          physicalPoint[i] = point[i] * m_Spacing[i] + m_Origin[i];
        return physicalPoint;
        }
    private:
      SpacingType                 m_Spacing;
      OriginType                  m_Origin;
      };

    /**\ingroup Geometry
     * \class DataSource
     * Collection of geometric objects.
     *
     * This class is meant to supercede \c otb::VectorData class.  It provides
     * an encapsulation of OGR classes. In that particular case, it's an
     * encapsulation of \c OGRDataSource.
     *
     * \note Not meant to be inherited
     * \note This class has an entity semantics: \em non-copyable, nor \em
     * assignable.
     * \note \c OGRRegisterAll() is implicitly called on construction
     * \internal As the class is not meant to be inherited, no new function is virtual.
     *
     * \note The following function haven't been encapsulated (yet?):
     * - \c SetStyleTable() & \c GetStyleTable()
     * - \c SetDriver() & \c GetDriver()
     * - all functions related to the reference count.
     */
    class DataSource : public itk::DataObject
    {
  public:
    /**\name Standard ITK typedefs */
    //@{
    typedef DataSource                    Self;
    typedef itk::DataObject               Superclass;
    typedef itk::SmartPointer<Self>       Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;
    //@}

    /**\name Standard macros */
    //@{
    /** Default builder.
     * This builder function creates a new \c DataSource() with its default
     * constructor. The actual \c OGRDataSource is using the <em>in-memory</em>
     * \c OGRSFDriver: \c OGRMemDriver.
     *
     * \throw itk::ExceptionObject if the inner \c OGRDataSource cannot be
     * opened.
     *
     * \note \c OGRRegisterAll() is implicitly called on construction
     * \see \c DataSource()
     */
    itkNewMacro(Self);
    itkTypeMacro(Layer, DataObject);
    // itkStaticConstMacro(Dimension, unsigned int, VDimension);
    //@}
    /**\name Creation functions */
    //@{
    struct Modes { enum type { invalid, read=1, write=2, MAX__ }; };

    /**
     * Builder from an existing named data source.
     * \param[in] filename filename of the data source
     * \param[in] mode     opening mode (read or read-write)
     * \return a newly created \c DataSource.
     * \throw itk::ExceptionObject if the inner \c OGRDataSource cannot be
     * opened.
     * \note \c OGRRegisterAll() is implicitly called on construction
     * \see \c DataSource(OGRDataSource *)
     */
    static Pointer New(std::string const& filename, Modes::type mode=Modes::read);
    /**
     * Builder from a built \c OGRDataSource.
     * \param[in,out] source  \c OGRDataSource already constructed.
     * \return a newly created \c DataSource that assumes ownership of \c
     * source.
     * \throw Nothing
     * \note \c OGRRegisterAll() is supposed to have been called before building
     * \c source.
     * \note no condition is assumed on the non-nullity of \c source.
     * \see \c DataSource(OGRDataSource *)
     */
    static Pointer New(OGRDataSource * source);
    //@}

    /**\name Projection Reference property */
    //@{
    void SetProjectionRef(const std::string& projectionRef);
    std::string GetProjectionRef() const;
    //@}

    /** Clears the data source.
     * \post the \c OGRDataSource owned is destroyed with the dedicated function
     * from OGR %API.
     * \post <tt>m_DataSource = 0</tt>
     */
    bool Clear();

    /**
     * Applies a functor on all layers.
     * \param[in] f  functor (copied) to execute on each layer.
     *
     * \throw itk::ExceptionObject in case one layer can't be accessed.
     * \throw * whatever the functor \c may throw.
     * \note the functor is expected to receive an \c ogr::Layer by reference.
     * \sa std::for_each
     */
    template <class Functor> void ForEachLayer(Functor f) const;

    /**
     * Accumulates the result of a functor on all layers.
     * \param[in] f  functor (copied) to execute on each layer.
     *
     * \return the result accumulated (with +)
     * \throw itk::ExceptionObject in case one layer can't be accessed.
     * \throw * whatever the functor \c may throw.
     * \note the functor is expected to receive an \c ogr::Layer by reference.
     * \sa std::accumulate
     */
    template <class Functor, typename V> V AccumulateOnLayers(Functor f, V v0) const;

    /** Returns the number of elements in the Data Source.
     * \param[in] doForceComputation  indicates whether the size shall be
     * computed on each layer even so it's expensive to do so.
     *
     * \return the number of features in the Data Source, -1 if count is unknown
     * \throw None
     * \sa OGRLayer::GetFeatureCount
     */
    int Size(bool doForceComputation) const;

    /** Grafts data and information from one data source to another.
     * \deprecated \c OGRLayer has an embedded input iterator. As a consequence,
     * the layer cannot support multiple access to its elements.
     *
     * This is a convenience function to setup a second data source with all the
     * meta information of another data source and use the same underlying \c
     * OGRDataSource.
     */
    virtual void Graft(const itk::DataObject *data);

    /**
     * Resets current data source with the one in parameter.
     * \param[in,out] source source \c OGRDataSource that this instance will own.
     * \throw None
     * \post Assumes ownership of the \c source.
     */
    void Reset(OGRDataSource * source);

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
     * \note a \em proxy-class is returned instead of a plain \c OGRLayer is
     * order to encapsulate all lifetime management of the \c OGRLayer obtained
     * (i.e. never to be destroyed). If you want to delete a layer obtained
     * with \c CreateLayer, you must use \c DeleteLayer.
     * \note the \c papszOptions parameter may later become a \c
     * std::vector<std::string>
     * \sa OGRDataSource::CreateLayer
     */
    Layer CreateLayer(
      std::string        const& name,
      OGRSpatialReference     * poSpatialRef = NULL,
      OGRwkbGeometryType        eGType = wkbUnknown,
      char                   ** papszOptions = NULL);

    /**
     * Deletes the i-th layer from the data source.
     * \param[in] i  layer index
     *
     * \throw it::ExceptionObject in case the index is out of range
     * \throw it::ExceptionObject if the layer cannot be deleted from the data
     * source.
     *
     * \pre the data source must support the delete operation
     * \pre the index \c i must be in range [0, GetLayersCount())
     * \sa OGRDataSource::DeleteLayer
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
     * \note a \em proxy-class is returned instead of a plain \c OGRLayer is
     * order to encapsulate all lifetime management of the \c OGRLayer obtained
     * (i.e. never to be destroyed). If you want to delete a layer obtained
     * with \c CreateLayer, you must use \c DeleteLayer.
     * \note the \c papszOptions parameter may later become a \c
     * std::vector<std::string>
     * \sa OGRDataSource::CopyLayer
     */
    Layer CopyLayer(
      Layer            & srcLayer,
      std::string const& newName,
      char            ** papszOptions = NULL);
    //@}

    /**\name Layers access
     *\note as the following accessors are not inlined, they aren't optimized.
     */
    //@{
    /** Returns the number of layers.
     */
    int GetLayersCount() const;

    /**
     * Unchecked Accessor to a given layer.
     * \param[in] i  index of the layer to access
     * \return the layer requested.
     * \pre <tt>i < GetLayersCount()</tt>, an assertion will abort the program
     * otherwise.
     * \pre the layer must available, an assertion will abort the program
     * otherwise.
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
     * \note Use \c GetLayerUnchecked(std::string const&) if you'd rather have
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
     * \pre the layer must available, an exception is raised otherwise.
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
     * \note use \c GetLayer(std::string const&) if you'd rather test the
     * obtained layer instead of catching an exception.
     */
    Layer       GetLayerChecked(std::string const& name);
    /**\copydoc otb::ogr::DataSource::GetLayerChecked(std::string const&)
     */
    Layer const GetLayerChecked(std::string const& name) const;

    /**
     * Excecutes the statement..
     * \param[in] statement  textual description of the SQL statement.
     * \param[in] poSpatialFilter  \c Geometry representing a spatial filter -- may be null.
     * \param[in] pszDialect  allows control of the statement dialect. If set to
     *                     NULL, the OGR SQL engine will be used, except for
     *                     RDBMS drivers that will use their dedicated SQL
     *                     engine, unless OGRSQL is explicitely passed as the
     *                     dialect.
     * \return a new \c Layer that contains the matching \c Features. In case of
     * error, or no matching result sets, a \em null Layer will be returned.
     * Check for \¢ Layer's validity before doing anything else.
     * \throw None even when there is an error -- OGR can not report errors,
     * neither this wrapping.
     * \note the returned \c Layer will be automatically collected on its
     * destruction ; i.e. unlike OGR API, no need to explicitly call \c
     * OGRDataSource::ReleaseResultSet().
     * \sa OGRDataSource::ExecuteSQL
     */
    Layer ExecuteSQL(
      std::string const& statement,
      OGRGeometry *  	 poSpatialFilter,
      char        const* pszDialect);

    //@}


    struct boolean{ int i; };
    /** Can the data source be used (ie not null).
     *
     * Hack to provide a boolean operator that is convertible only to a
     * boolean expression to be used in \c if tests.
     * \see <em>Imperfect C++</em>, Matthew Wilson, Addisson-Welsey, par 24.6
     */
    operator int boolean ::* () const
      {
      return m_DataSource ? &boolean::i : 0;
      }

    /**
     * Flushes all changes to disk.
     * \throw itd::ExceptionObject in case the flush operation failed.
     * \sa OGRDataSource::SyncToDisk
     */
    void SyncToDisk();

    /**
     * Returns whether a capability is avalaible.
     * \param[in] capabilityName  name of the capability to check.
     * \throw None
     * \sa OGRDataSource::TestCapability
     */
    bool HasCapability(std::string const& capabilityName);

    /** Access to raw \c OGRDataSource.
     * This function provides an abstraction leak in case deeper control on the
     * underlying \c OGRDataSource is required.
     * \pre the underlying \c OGRDataSource must be valid, i.e.
     * <tt>m_DataSource != 0</tt>, an assertion is fired otherwise.
   * \warning you must under no circonstance try to delete the \c OGRDataSource
   * obtained this way.
     */
    OGRDataSource & ogr();

  protected:
    /** Default constructor.
     * The actual \c OGRDataSource is using the <em>in-memory</em> \c
     * OGRSFDriver: \c OGRMemDriver.
     * \throw itk::ExceptionObject if the inner \c OGRDataSource cannot be
     * opened.
     *
     * \note \c OGRRegisterAll() is implicitly called on construction
     * \see \c DataSource::New()
     */
    DataSource();
    /** Init constructor.
     * \post the newly constructed object owns the \c source parameter.
     */
    DataSource(OGRDataSource * source);
    /** Destructor.
     * \post the \c OGRDataSource owned is released (if not null).
     */
    virtual ~DataSource();
    /** Prints self into stream. */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  private:
    /**
     * Internal unchecked accessor to a given layer.
     * \param[in] i  index of the layer to access
     * \return a reference to the layer requested.
     * \pre <tt>i < GetLayersCount()</tt>, return 0 otherwise
     * \pre the layer must available, 0 is returned otherwise.
     * \throw None
     * \internal this function is a simple encapsulation of \c
     * OGRDataSource::GetLayer().
     */
    OGRLayer* GetLayerUnchecked(size_t i);

    DataSource(const Self&);             //purposely not implemented
    DataSource& operator =(const Self&); //purposely not implemented

  private:
    OGRDataSource  *m_DataSource;
    // ImageReference  m_ImageReference;
    }; // end class DataSource
} } // end namespace otb::ogr

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRDataSourceWrapper.txx"
#endif

#endif // __otbOGRDataSourceWrapper_h
