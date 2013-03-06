/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdVectorImageModel_h
#define __mvdVectorImageModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkExtractImageFilter.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbRenderingImageFilter.h"
#include "otbGenericRSTransform.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdColorSetupWidget.h"
#include "mvdAbstractImageModel.h"
#include "mvdTypes.h"

/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
class DatasetModel;


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class VectorImageModel
 *
 */
class Monteverdi2_EXPORT VectorImageModel :
    public AbstractImageModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /**
   * In-memory storage type of source image (from file).
   */
  typedef DefaultImageType SourceImageType;

  /**
   */
  class Settings
  {
    //
    // Public types.
  public:
    typedef UIntVector ChannelVector;

    //
    // Public methods.
  public:
    /** Constructor */
    Settings() :
      m_IsDirty( false ),
      m_RgbChannels(),
      m_DynamicsParams( 6 )
    {
    }

    /** Destructor */
    ~Settings() 
    {
    }

    /** */
    bool
    IsDirty() const
    {
      return m_IsDirty;
    }

    /** */
    inline
    void
    SetDirty()
    {
      m_IsDirty = true;
    }

    /** */
    inline void ClearDirty()
    {
      m_IsDirty = false;
    }

    /** */
    inline
    void
    SetRgbChannels( const ChannelVector& rgb )
    {
      m_RgbChannels = rgb;
      SetDirty();
    }

    /** */
    inline
    const ChannelVector&
    GetRgbChannels() const
    {
      return m_RgbChannels;
    }

    /**
     */
    inline
    ChannelVector::value_type&
    RgbChannel( ChannelVector::size_type i )
    {
      SetDirty();
      return m_RgbChannels[ i ];
    }

    /**
     */
    inline
    const ChannelVector::value_type&
    RgbChannel( ChannelVector::size_type i ) const
    {
      return m_RgbChannels[ i ];
    }

    /**
     */
    inline
    void
    SetDynamicsParams( const ParametersType& params )
    {
      m_DynamicsParams = params;
      SetDirty();
    }

    /**
     */
    inline
    const ParametersType&
    GetDynamicsParams() const
    {
      return m_DynamicsParams;
    }

    /**
     */
    inline
    const ParametersType::ValueType&
    DynamicsParam( CountType i ) const
    {
      return m_DynamicsParams[ i ];
    }

    /**
     */
    inline
    ParametersType::ValueType&
    DynamicsParam( CountType i )
    {
      SetDirty();
      return m_DynamicsParams[ i ];
    }

#if 0
    /**
     */
    inline
    const ParametersType::ValueType&
    DynamicsParam( RgbaChannel channel ) const
    {
      return m_DynamicsParams[ m_RgbChannels[ channel ] ];
    }

    /**
     */
    inline
    ParametersType::ValueType&
    DynamicsParam( RgbaChannel channel )
    {
      return m_DynamicsParams[ m_RgbChannels[ channel ] ];
    }
#endif

    //
    // Private attributes.
  private:
    /** Flag which notices when display Settings has been modified and
     * have not yet been rendered.
     */
    bool m_IsDirty;
    /**
     * Color-composition setup (file-component to video RGB-components).
     */
    ChannelVector m_RgbChannels;
    /**
     */
    ParametersType m_DynamicsParams;
  };

//
// Public methods.
public:

  /** Constructor */
  VectorImageModel( QObject* parent =NULL );

  /** Destructor */
  virtual ~VectorImageModel();

  /**
   * \brief Get the parent DatasetModel.
   *
   * \return The parent DatasetModel.
   */
  inline const DatasetModel* GetDatasetModel() const;

  /**
   * \brief Get the parent DatasetModel.
   *
   * \return The parent DatasetModel.
   */
  inline DatasetModel* GetDatasetModel();

  /** */
  // TODO: Move into template wrapper base-class.
  SourceImageType::ConstPointer ToImage() const;

  /** */
  // TODO: Move into template wrapper base-class.
  const SourceImageType::Pointer& ToImage();

  /**
   */
  inline const Settings& GetSettings() const;

  /** */
  inline Settings& GetSettings();

  /** */
  inline otb::GenericRSTransform<> * GetGenericRSTransform();

  /**
   * Width and height are added to compute the best level of detail to
   * load from the image if multi-resolution image.
   */
  void SetFilename( const QString& filename, int width, int height );

  /** Get input filename */
  inline QString GetFilename() const;

  /** Rasterize the buffered region in a buffer */
  unsigned char * RasterizeRegion(const ImageRegionType& region,
                                  const double zoomFactor,
                                  bool refresh);

  /**
   * Following the zoom factor, get the best level of detail
   */
  CountType ComputeBestLevelOfDetail(const double ZoomFactor);

  //
  // AbstractImageModel overrides.

  /** 
   * Get the number of available LOD.
   */
  virtual CountType GetNbLod() const;

  /**
   * Get a smart-pointer to the current LOD image-base.
   */
  virtual ImageBaseType::ConstPointer ToImageBase() const;

  /**
   * Get a smart-pointer to the current LOD image-base.
   */
  virtual ImageBaseType::Pointer ToImageBase();

  /**
   * Get the placename from the center pixel
   */
  std::string GetCenterPixelPlaceName();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public slots.
public slots:
  /** */
  void OnPhysicalCursorPositionChanged(double Xpc, double Ypc);
  /** */
  void OnUserCoordinatesEditingFinished(const QString& coord);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**   */
  void SettingsUpdated();
  /**   */
  void ViewportRegionChanged(double, double);
  /** */
  void CurrentIndexUpdated(const QString&);
  void CurrentPhysicalUpdated(const QString&);
  void CurrentGeographicUpdated(const QString&);
  void CurrentRadioUpdated(const QString&);

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** */
  inline void SetSettings( const Settings& settings );

  /** Clear buffer */
  void ClearBuffer();

  //
  // AbstractModel overrides.

  /** */
  virtual void virtual_BuildModel( void* context =NULL );

  //
  // AbstractImageModel overrides.

  /** */
  virtual void virtual_SetCurrentLod( CountType lod );

  /**
   */
  void InitializeColorSetupSettings();

  /** 
   */
  void InitializeColorDynamicsSettings();

  /**
   */
  void InitializeRgbaPipeline();

//
// Protected attributes.
protected:

  /** */
  SourceImageType::Pointer m_Image;

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:
  /**
   * Display type of source image (to OpenGL).
   */
  typedef RGBAImageType DisplayImageType;

  /**
   * Extract filter.
   */
  typedef
    itk::ExtractImageFilter< SourceImageType, SourceImageType >
    ExtractFilterType;

  /**
   * Rendering filter.
   */
  // 
  typedef
    otb::RenderingImageFilter< SourceImageType, DisplayImageType >
    RenderingFilterType;

//
// Private methods.
private:

  /** Compute the linear buffer index according to the 2D region and
  * its 2D index.This method is used when OTB_GL_USE_ACCEL is ON.
  * \param index 2D index
  * \param region 2D region
  */
  static inline
    unsigned int
    ComputeBufferIndex(const IndexType& index,
		       const ImageRegionType& region);

  /** Compute the linear buffer index according to the 2D region and
   * its 2D index.This method is used when OTB_GL_USE_ACCEL is OFF.
   * The resulting buffer will be flipped over the X axis.
   * \param index 2D index
   * \param region 2D region
   */
  static inline
    unsigned int
    ComputeXAxisFlippedBufferIndex(const IndexType& index,
				   const ImageRegionType& region);

  /** Compute the image index according to a linear buffer index and its
   * 2D region
   * \param index 2D index
   * \param region 2D region
   */
  static inline
    IndexType
    ComputeImageIndexFromFlippedBuffer(const unsigned int & index,
				       const ImageRegionType& region);

  /** Dump pixels within the region in argument of the method into the
  * m_RasterizedBuffer (used for OpenGl rendering)
  * \param region 2D region
  */
  void DumpImagePixelsWithinRegionIntoBuffer(const ImageRegionType& region);

  /** Compute the regions to be loaded and computes the already loaded
  * region. Four regions are computed here {upper, lower,
  * right, left} region. Each region with size greater than 0 is added
  * to the m_RegionsToLoadVector vector.
  * \param region 2D region
  */
  void ComputeRegionsToLoad(const ImageRegionType& region);

  /**
    * Helper method to get the best closest Jpeg2K level of detail
    * 
    */
  static
    unsigned int
    Closest(double invZoomfactor,
	    const std::vector<unsigned int> & res);

  /** 
    * helper to setup the lod image using a width/height or a zoom factor
    */
  void SetupCurrentLodImage(int w, int h);


//
// Private attributes.
private:
  // Default image reader
  DefaultImageFileReaderType::Pointer m_ImageFileReader;

  // Buffer where to store the image pixels needed by the renderer
  unsigned char *                     m_RasterizedBuffer;

  // Extract filter
  ExtractFilterType::Pointer          m_ExtractFilter;
  RenderingFilterType::Pointer        m_RenderingFilter;

  /**
   * User-configurable settings of image-model.
   */
  Settings m_Settings;

  // store the previously requested region
  ImageRegionType                     m_PreviousRegion;

  // Store the already loaded region
  ImageRegionType                     m_AlreadyLoadedRegion;

  // store the current requested region
  ImageRegionType                     m_Region;

  // Vector storing the region to load
  std::vector< ImageRegionType >       m_RegionsToLoadVector;

  // store the input image filename
  QString m_Filename;

  /** List of all Level of detail (Resolution) available from the file */
  UIntVector m_AvailableLod;
  
  //  Generic RS Transform to get lat/long coordinates 
  otb::GenericRSTransform<>::Pointer m_GenericRSTransform;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /** */
  void OnModelUpdated();
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdDatasetModel.h"

namespace mvd
{

/*****************************************************************************/
const DatasetModel*
VectorImageModel
::GetDatasetModel() const
{
  return qobject_cast< const DatasetModel* >( parent() );
}

/*****************************************************************************/
DatasetModel*
VectorImageModel
::GetDatasetModel()
{
  return qobject_cast< DatasetModel* >( parent() );
}

/*****************************************************************************/
inline
VectorImageModel::SourceImageType::ConstPointer
VectorImageModel
::ToImage() const
{
  // TODO: Fix unsafe weak-pointer dereferencing.
  return otb::ConstCast< VectorImageModel::SourceImageType >( m_Image );
}

/*****************************************************************************/
inline
const VectorImageModel::SourceImageType::Pointer&
VectorImageModel
::ToImage()
{
  return m_Image;
}

/*****************************************************************************/
inline
void
VectorImageModel
::SetSettings(const Settings& settings)
{
  m_Settings = settings;
}

/*****************************************************************************/
inline
const VectorImageModel::Settings&
VectorImageModel
::GetSettings() const
{
  return m_Settings;
}

/*****************************************************************************/
inline
VectorImageModel::Settings&
VectorImageModel
::GetSettings()
{
  return m_Settings;
}

/*****************************************************************************/
inline
otb::GenericRSTransform<>*
VectorImageModel
::GetGenericRSTransform()
{
  return m_GenericRSTransform;
}

/*****************************************************************************/
inline
QString
VectorImageModel
::GetFilename() const
{
  return m_Filename;
}

/*****************************************************************************/
inline
unsigned int
VectorImageModel
::ComputeBufferIndex(const IndexType& index,
		     const ImageRegionType& region)
{
  return (index[1] - region.GetIndex()[1]) * 4 * region.GetSize()[0] + 4 * (index[0] - region.GetIndex()[0]);
}

/*****************************************************************************/
inline
unsigned int
VectorImageModel
::ComputeXAxisFlippedBufferIndex(const IndexType& index,
				 const ImageRegionType& region)
{
  return (region.GetSize()[1] - 1 + region.GetIndex()[1] -
	  index[1]) * 4 * region.GetSize()[0] + 4 * (index[0] - region.GetIndex()[0]);
}

/*****************************************************************************/
inline
IndexType
VectorImageModel
::ComputeImageIndexFromFlippedBuffer(const unsigned int & index,
				     const ImageRegionType& region)
{
  IndexType imageIndex;
  imageIndex[0] =  ( index % region.GetSize(0) ) + region.GetIndex()[0];

  imageIndex[1] =  region.GetSize()[1] - 1 + region.GetIndex()[1]
    - static_cast<unsigned int>(vcl_floor( static_cast<double>(index) / static_cast<double>(region.GetSize(0) )));

  return imageIndex;
}

/*****************************************************************************/

} // end namespace 'mvd'

#endif // __mvdVectorImageModel_h
