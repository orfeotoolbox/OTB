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
#include <cassert>

#include "itkVector.h"
#include "itkPoint.h"
#include "itkDataObject.h"
#include "itkMacro.h" // itkNewMacro
#include "itkObjectFactory.h" // that should have been included by itkMacro.h 

// class OGRDataSource;
#include "ogrsf_frmts.h" // OGRDataSource
class OGRLayer;

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
     * \note not meant to be inherited
     * \note this class has an entity semantics: \em non-copyable, nor \em
     * assignable. 
     * \note \c OGRRegisterAll() is implicitly called on construction
     * \internal as the class is not meant to be inherited, no new function is virtual.
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
     * \note the functor is expected to receive an \c OGRLayer by reference.
     * \sa std::for_each
     */
    template <class Functor> void ForEachLayer(Functor f)
      {
      assert(m_DataSource && "OGRDataSource not initialized");
      const int nbLayers = this->GetLayersCount();
      for (int i=0; i!=nbLayers; ++i)
        {
        OGRLayer * l = m_DataSource->GetLayer(i);
        if (!l)
          {
          itkExceptionMacro(<< "Failed to fetch "<< i <<"th layer from OGRDataSource");
          }
        f(*l);
        }
      }

    /**
     * Accumulates the result of a functor on all layers.
     * \param[in] f  functor (copied) to execute on each layer.
     *
     * \return the result accumulated (with +)
     * \throw itk::ExceptionObject in case one layer can't be accessed.
     * \throw * whatever the functor \c may throw.
     * \note the functor is expected to receive an \c OGRLayer by reference.
     * \sa std::accumulate
     */
    template <class Functor, typename V> V AccumulateOnLayers(Functor f, V v0) const
      {
      assert(m_DataSource && "OGRDataSource not initialized");
      const int nbLayers = this->GetLayersCount();
      for (int i=0; i!=nbLayers; ++i)
        {
        OGRLayer * l = m_DataSource->GetLayer(i);
        if (!l)
          {
          itkExceptionMacro(<< "Failed to fetch "<< i <<"th layer from OGRDataSource");
          }
        v0 = f(*l, v0);
        }
      return v0;
      }

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
     * \return a reference to the layer requested.
     * \pre <tt>i < GetLayersCount()</tt>, an assertion will abort the program
     * otherwise.
     * \pre the layer must available, an assertion will abort the program
     * otherwise.
     * \throw None
     * \note Use \c GetLayerUnchecked() if invalid indices are programming
     * errors, or if null layers are to be expected.
     */
    OGRLayer& GetLayer(size_t i)
      {
      assert(int(i) < GetLayersCount());
      OGRLayer * layer_ptr = GetLayerUnchecked(i);
      assert(layer_ptr);
      return *layer_ptr;
      }
    /**\copydoc otb::ogr::DataSource::GetLayer()
     */
    OGRLayer const& GetLayer(size_t i) const
      {
      return const_cast <DataSource*>(this)->GetLayer(i);
      }

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
    OGRLayer& GetLayerChecked(size_t i);
    /**\copydoc otb::ogr::DataSource::GetLayerChecked()
     */
    OGRLayer const& GetLayerChecked(size_t i) const;
    //@}


    struct boolean{ int i;};
    /** Can the data source be used (ie not null).
     * 
     * Hack to provide a boolean operator that is convertible only to a
     * boolean expression to be used in \c if tests.
     * \see <em>Imperfect C++</em>, Matthew Wilson, Addisson-Welsey, par 24.6
     */
    operator int boolean ::* () const {
      return m_DataSource ? &boolean::i : 0;
    }

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
// #include "otbOGRDataSourceWrapper.txx"
#endif

#endif // __otbOGRDataSourceWrapper_h
