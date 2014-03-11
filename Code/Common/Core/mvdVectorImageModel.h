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
#include "otbGenericRSTransform.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractImageModel.h"
#include "Core/mvdRenderingImageFilter.h"
#include "Core/mvdTypes.h"
#include "Gui/mvdColorSetupWidget.h"

#define USE_BYTE_POINTER 1

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
#if USE_BYTE_POINTER
  typedef QSharedPointer< unsigned char > BytePointer;
#endif

  /**
   * \brief WIP.
   */
  class Settings
  {
    //
    // Public types.
  public:
    /**
     */
    typedef UIntVector ChannelVector;

    //
    // Public methods.
  public:
    /**
     * \brief Constructor.
     */
    Settings();

    /**
     * \brief Copy constructor.
     *
     * \param other
     *
     * Copy settings POD content without the modified flag.
     */
    Settings( const Settings& other );

    /**
     * \brief Destructor.
     */
    ~Settings();

    /**
     * \brief Assignment operator.
     *
     * \param other
     */
    Settings& operator=( const Settings& other );

    /**
     */
    inline bool IsApplied() const;

    /**
     */
    inline bool IsModified() const;

    /**
     */
    inline void SetModified();

    /**
     */
    inline void ClearModified();

    /**
     */
    inline void SetApplied();

    //
    // COLOR COMPOSITION.
    //

    /**
     */
    inline void SetRgbChannels( const ChannelVector& rgb );

    /**
     */
    inline const ChannelVector& GetRgbChannels() const;

    /**
     */
    inline
    void SetRgbChannel( ChannelVector::size_type i,
			const ChannelVector::value_type& channel );

    /**
     */
    inline
    const ChannelVector::value_type&
    GetRgbChannel( ChannelVector::size_type i ) const;

    /**
     * \return the band-index for the given component taking the
     * grayscale-mode activation state flag into account.
     */
    inline
    ChannelVector::value_type
    GetSmartChannel( RgbwChannel channel ) const;

    /**
     * \return the band-index for the given component not taking the
     * grayscale-mode activation state flag into account.
     */
    inline
    ChannelVector::value_type
    GetRgbwChannel( RgbwChannel channel ) const;

    /**
     * \return the channels band-index vector taking the
     * grayscale-mode activation state flag into account.
     */
    inline void GetSmartChannels( ChannelVector& channels ) const;

    //
    // COLOR DYNAMICS.
    //

    /**
     */
    inline void SetRgbDynamicsParams( const ParametersType& params );

    /**
     */
    inline const ParametersType& GetRgbDynamicsParams() const;

    /**
     */
    inline ParametersType GetSmartDynamicsParams() const;

    /**
     * Set low-intensity dynamics parameter for given channel. If
     * channel is RGBW_CHANNEL_WHITE, this method sets all RGB
     * channels.
     *
     * \param channel The RGB/W channel to set low-intensity dynamics for.
     * \param intensity low-intensity value.
     */
    inline void SetLowIntensity( RgbwChannel channel,
				 ParametersType::ValueType intensity );

    /**
     */
    inline
    ParametersType::ValueType
    GetLowIntensity( RgbwChannel channel ) const;

    /**
     * Set high-intensity dynamics parameter for given channel. If
     * channel is RGBW_CHANNEL_WHITE, this method sets all RGB
     * channels.
     *
     * \param channel The RGB/W channel to set high-intensity dynamics for.
     * \param intensity high-intensity value.
     */
    inline void SetHighIntensity( RgbwChannel channel,
                                  ParametersType::ValueType intensity );

    /**
     */
    inline
    ParametersType::ValueType
    GetHighIntensity( RgbwChannel channel ) const;

    //
    // GRAYSCALE MODE.
    //

    /**
     * \brief Set/clear grayscale-mode activation-state flag for image.
     */
    inline void SetGrayscaleActivated( bool activated );

    /**
     * \return Grayscale-mode activation state flag.
     */
    inline bool IsGrayscaleActivated() const;

    /**
     * \brief Set white (gray) channel index.
     *
     * \param index The new index of white (gray) channel.
     */
    inline void SetGrayChannel( unsigned int );

    /**
     * \return White (gray) channel index.
     */
    inline unsigned int GetGrayChannel() const;

    /**
     */
    inline void SetGrayDynamicsParams( const ParametersType& params );

    /**
     */
    inline const ParametersType& GetGrayDynamicsParams() const;

    /**
     */
    inline void SetGamma( double gamma );

    /**
     */
    inline double GetGamma() const;

    //
    // Private methods.
  private:

    /**
     */
    inline
    const ParametersType::ValueType& GetRgbDynamicsParam( CountType i ) const;

    /**
     */
    inline
    void SetRgbDynamicsParam( CountType i,
			      const ParametersType::ValueType& param );

    /**
     */
    inline
    const ParametersType::ValueType& GetGrayDynamicsParam( bool high ) const;

    /**
     */
    inline
    void SetGrayDynamicsParam( bool high,
			       const ParametersType::ValueType& param );

    //
    // Private attributes.
  private:
    /**
     * \brief Color-composition setup (file-component to video
     * RGB-components).
     */
    ChannelVector m_RgbChannels;

    /**
     * \brief Color-dynamics parameters (\sa
     * HistogramModel::Quantile()).
     */
    ParametersType m_RgbDynamicsParams;

    /**
     * \brief Grayscale-mode activation state.
     */
    bool m_IsGrayscaleActivated;

    /**
     * \brief Grayscale-mode band-index.
     */
    ChannelVector::value_type m_GrayChannel;

    /**
     * \brief Color-dynamics parameters (\sa
     * HistogramModel::Quantile()).
     */
    ParametersType m_GrayDynamicsParams;

    /**
     * \brief Flag which notices that rendering settings have been
     * edited.
     */
    // TODO: Optimize using C++ bitset bool foo:1;
    bool m_IsModified: 1;

    /**
     * \brief Flag which notices that rendering settings have been
     * applied to display.
     */
    // TODO: Optimize using C++ bitset bool foo:1;
    bool m_IsApplied: 1;

    /**
     */
    double m_Gamma;
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
  virtual const DatasetModel* GetDatasetModel() const;

  /**
   * \brief Get the parent DatasetModel.
   *
   * \return The parent DatasetModel.
   */
  virtual DatasetModel* GetDatasetModel();

  /** */
  static void EnsureValidImage( const QString& filename );

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
  inline void SetSettings( const Settings& settings );

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
#if USE_BYTE_POINTER
  BytePointer
#else
  unsigned char *
#endif
    RasterizeRegion( const ImageRegionType& region,
		     const double zoomFactor,
		     bool refresh );

  /**
   * Following the zoom factor, get the best level of detail
   */
  CountType ComputeBestLod( double ZoomFactor ) const;

  /**
   * Following the zoom factor, get the best level of detail
   */
  CountType ComputeBestLod( int width, int height ) const;

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

  //
  // AbstractModel overrides.

  virtual bool IsModified() const;

  virtual void ClearModified();

  // get image size in byte
  std::streamoff GetImageSizeInBytes()
  {
    return m_ImageFileReader->GetImageIO()->GetImageSizeInBytes();
  }

  /** Release as much memory as possible. This implementation will
  clear the current pipeline and loaded image data */
  virtual void ReleaseMemory();

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public slots.
public slots:
  /**
   */
  void OnPhysicalCursorPositionChanged1(double Xpc, double Ypc);
  /**
   */
  void OnPhysicalCursorPositionChanged( const PointType&,
                                        const DefaultImageType::PixelType& );

  /**
   */
  void OnUserCoordinatesEditingFinished(const QString& coord);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void SettingsUpdated( AbstractImageModel* imageModel =NULL );

  /**
   */
  void PropertiesUpdated( AbstractImageModel* imageModel =NULL );

  /**   */
  void ViewportRegionChanged(double, double);

  /** */
  void CurrentIndexUpdated( const IndexType&, bool isInside );
  void CurrentIndexUpdated(const QString&);
  void CurrentPhysicalUpdated(const QStringList&);
  void CurrentGeographicUpdated(const QStringList&);
  void CurrentRadioUpdated(const QString&);
  void CurrentPixelValueUpdated(const VectorImageType::PixelType &, 
                                const QStringList& );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** Clear buffer */
  void ClearBuffer();

  //
  // AbstractModel overrides.

  virtual void virtual_BuildModel( void* context =NULL );

  //
  // AbstractImageModel overrides.

  void InitializeColorSetupSettings();

  void InitializeColorDynamicsSettings();

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
    mvd::RenderingImageFilter< SourceImageType, DisplayImageType >
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
   * 2D region.
   *
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
    * Helper method to get the best closest Jpeg2K level of detail.
    */
  static
    unsigned int
    Closest(double invZoomfactor,
	    const std::vector<unsigned int> & res);

  /** 
    * helper to setup the lod image using a width/height or a zoom factor
    */
  void SetupCurrentLodImage(int w, int h);

  /**
   */
  void ApplySettings();

  /**
   */
  void BuildGdalOverviews();

  //
  // AbstractImageModel overrides.

  virtual void virtual_SetCurrentLod( CountType lod );

  virtual void virtual_RefreshHistogram();

//
// Private attributes.
private:
  // Default image reader
  DefaultImageFileReaderType::Pointer m_ImageFileReader;

#if USE_BYTE_POINTER
  BytePointer m_RasterizedBuffer;
#else
  // Buffer where to store the image pixels needed by the renderer
  unsigned char* m_RasterizedBuffer;
#endif

  // Extract filter
  ExtractFilterType::Pointer m_ExtractFilter;
  RenderingFilterType::Pointer m_RenderingFilter;

  /**
   * User-configurable settings of image-model.
   */
  Settings m_Settings;

  // store the previously requested region
  ImageRegionType m_PreviousRegion;

  // Store the already loaded region
  ImageRegionType m_AlreadyLoadedRegion;

  // store the current requested region
  ImageRegionType m_Region;

  // Vector storing the region to load
  std::vector< ImageRegionType > m_RegionsToLoadVector;

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
inline
VectorImageModel::Settings
::Settings() :
  m_RgbChannels(),
  m_RgbDynamicsParams( 6 ),
  m_IsGrayscaleActivated( false ),
  m_GrayChannel( 0 ),
  m_GrayDynamicsParams( 6 ),
  m_IsModified( false ),
  m_IsApplied( false ),
  m_Gamma(1)
{
}

/*****************************************************************************/
inline
VectorImageModel::Settings
::Settings( const Settings& other ) :
  m_RgbChannels( other.m_RgbChannels ),
  m_RgbDynamicsParams( other.m_RgbDynamicsParams ),
  m_IsGrayscaleActivated( false ),
  m_GrayChannel( other.m_GrayChannel ),
  m_GrayDynamicsParams( 6 ),
  m_IsModified( false ),
  m_IsApplied( other.m_IsApplied ),
  m_Gamma( other.m_Gamma)
{
}

/*****************************************************************************/
inline
VectorImageModel::Settings&
VectorImageModel::Settings
::operator=( const Settings& other )
{
  if( &other==this )
    return *this;

  m_RgbChannels = other.m_RgbChannels;
  m_RgbDynamicsParams = other.m_RgbDynamicsParams;
  m_IsGrayscaleActivated = other.m_IsGrayscaleActivated;
  m_GrayChannel = other.m_GrayChannel;
  m_GrayDynamicsParams = other.m_GrayDynamicsParams;
  m_IsApplied = other.m_IsApplied;
  m_Gamma = other.m_Gamma;

  return *this;
}

/*****************************************************************************/
inline
VectorImageModel::Settings
::~Settings() 
{
}

/*****************************************************************************/
inline
bool
VectorImageModel::Settings
::IsApplied() const
{
  return m_IsApplied;
}

/*****************************************************************************/
inline
bool
VectorImageModel::Settings
::IsModified() const
{
  return m_IsModified;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetModified()
{
  // qDebug() << this << "::SetModified()";

  m_IsModified = true;
  m_IsApplied = false;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::ClearModified()
{
  // qDebug() << this << "::ClearModified()";

  m_IsModified = false;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetApplied()
{
  // qDebug() << this << "::SetApplied()";

  m_IsApplied = true;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetRgbChannels( const ChannelVector& rgb )
{
  // qDebug() << "setRgbChannels()";

  assert( m_RgbChannels.empty() || m_RgbChannels.size()==rgb.size() );

  if( !m_RgbChannels.empty() &&
      std::equal( m_RgbChannels.begin(), m_RgbChannels.end(), rgb.begin() ) )
    return;

  m_RgbChannels = rgb;

  SetModified();
}

/*****************************************************************************/
inline
const VectorImageModel::Settings::ChannelVector&
VectorImageModel::Settings
::GetRgbChannels() const
{
  return m_RgbChannels;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::GetSmartChannels( VectorImageModel::Settings::ChannelVector& channels ) const
{
  if( IsGrayscaleActivated() )
    {
    channels.resize( m_RgbChannels.size() );
    std::fill( channels.begin(), channels.end(), m_GrayChannel );
    }
  else
    {
    channels = m_RgbChannels;
    }
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetRgbChannel( ChannelVector::size_type i,
		 const ChannelVector::value_type& channel )
{
  // qDebug() << "SetRgbChannel()";

  if( m_RgbChannels[ i ]==channel )
    return;

  SetModified();

  m_RgbChannels[ i ] = channel;
}

/*****************************************************************************/
inline
const VectorImageModel::Settings::ChannelVector::value_type&
VectorImageModel::Settings
::GetRgbChannel( ChannelVector::size_type i ) const
{
  return m_RgbChannels[ i ];
}

/*****************************************************************************/
inline
VectorImageModel::Settings::ChannelVector::value_type
VectorImageModel::Settings
::GetSmartChannel( RgbwChannel channel ) const
{
  return GetRgbwChannel(
    m_IsGrayscaleActivated
    ? RGBW_CHANNEL_WHITE
    : channel
  );

  /*
  if( m_IsGrayscaleActivated )
    {
    return m_GrayChannel;
    }
  else
    {
    return GetRgbwChannel( channel );
    }
  */
}

/*****************************************************************************/
inline
VectorImageModel::Settings::ChannelVector::value_type
VectorImageModel::Settings
::GetRgbwChannel( RgbwChannel channel ) const
{
  switch( channel )
    {
    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      return m_RgbChannels[ channel ];
      break;

    case RGBW_CHANNEL_WHITE:
      return m_GrayChannel;
      break;

    default:
      throw std::invalid_argument(
	ToStdString(
	  tr( "Invalid argument: %1 ('%2')" )
	  .arg( channel )
	  .arg( RGBW_CHANNEL_NAMES[ channel ] )
	)
      );
      break;
    }
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetRgbDynamicsParams( const ParametersType& params )
{
  // qDebug() << "SetDynamicsParams()";

  assert( m_RgbDynamicsParams.size()==params.size() );

  if( std::equal( m_RgbDynamicsParams.begin(),
		  m_RgbDynamicsParams.end(),
		  params.begin() ) )
    return;

  m_RgbDynamicsParams = params;

  SetModified();
}

/*****************************************************************************/
inline
const ParametersType&
VectorImageModel::Settings
::GetRgbDynamicsParams() const
{
  return m_RgbDynamicsParams;
}

/*****************************************************************************/
inline
ParametersType
VectorImageModel::Settings
::GetSmartDynamicsParams() const
{
  return m_IsGrayscaleActivated ? m_GrayDynamicsParams : m_RgbDynamicsParams;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetLowIntensity( RgbwChannel channel, ParametersType::ValueType intensity )
{
  switch( channel )
    {
    case RGBW_CHANNEL_RGB:
      SetRgbDynamicsParam( 2 * RGBW_CHANNEL_RED, intensity );
      SetRgbDynamicsParam( 2 * RGBW_CHANNEL_GREEN, intensity );
      SetRgbDynamicsParam( 2 * RGBW_CHANNEL_BLUE, intensity );
      break;

    case RGBW_CHANNEL_WHITE:
      SetGrayDynamicsParam( 0, intensity );
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      SetRgbDynamicsParam( 2 * channel, intensity );
      break;

    default:
      throw std::invalid_argument(
	ToStdString(
	  tr( "Invalid argument: %1 ('%2')" )
	  .arg( channel )
	  .arg( RGBW_CHANNEL_NAMES[ channel ] )
	)
      );
      break;
    }
}

/*****************************************************************************/
inline
ParametersType::ValueType
VectorImageModel::Settings
::GetLowIntensity( RgbwChannel channel ) const
{
  switch( channel )
    {
    case RGBW_CHANNEL_RGB:
      {
      ParametersType::ValueType r =
	GetRgbDynamicsParam( 2 * RGBW_CHANNEL_RED );

      ParametersType::ValueType g =
	GetRgbDynamicsParam( 2 * RGBW_CHANNEL_GREEN );

      ParametersType::ValueType b =
	GetRgbDynamicsParam( 2 * RGBW_CHANNEL_BLUE );

      return (r + g + b) / 3;
      }
      break;

    case RGBW_CHANNEL_WHITE:
      return GetGrayDynamicsParam( false );
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      return GetRgbDynamicsParam( 2 * channel );
      break;

    default:
      throw std::invalid_argument(
	ToStdString(
	  tr( "Invalid argument: %1 ('%2')" )
	  .arg( channel )
	  .arg( RGBW_CHANNEL_NAMES[ channel ] )
	)
      );
      break;
    }
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetHighIntensity( RgbwChannel channel, ParametersType::ValueType intensity )
{
  switch( channel )
    {
    case RGBW_CHANNEL_RGB:
      SetRgbDynamicsParam( 1 + 2 * RGBW_CHANNEL_RED, intensity );
      SetRgbDynamicsParam( 1 + 2 * RGBW_CHANNEL_GREEN, intensity );
      SetRgbDynamicsParam( 1 + 2 * RGBW_CHANNEL_BLUE, intensity );
      break;

    case RGBW_CHANNEL_WHITE:
      SetGrayDynamicsParam( 1, intensity );
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      SetRgbDynamicsParam( 1 + 2 * channel, intensity );
      break;

    default:
      assert( false );
      break;
    }
}

/*****************************************************************************/
inline
ParametersType::ValueType
VectorImageModel::Settings
::GetHighIntensity( RgbwChannel channel ) const
{
  switch( channel )
    {
    case RGBW_CHANNEL_RGB:
      {
      ParametersType::ValueType r =
	GetRgbDynamicsParam( 1+ 2*RGBW_CHANNEL_RED );

      ParametersType::ValueType g =
	GetRgbDynamicsParam( 1+ 2*RGBW_CHANNEL_GREEN );

      ParametersType::ValueType b =
	GetRgbDynamicsParam( 1+ 2*RGBW_CHANNEL_BLUE );

      return (r + g + b) / 3;
      }
      break;

    case RGBW_CHANNEL_WHITE:
      return GetGrayDynamicsParam( 1 );
      break;

    case RGBW_CHANNEL_RED:
    case RGBW_CHANNEL_GREEN:
    case RGBW_CHANNEL_BLUE:
      return GetRgbDynamicsParam( 1 + 2 * channel );
      break;

    default:
      throw std::invalid_argument(
	ToStdString(
	  tr( "Invalid argument: %1 ('%2')" )
	  .arg( channel )
	  .arg( RGBW_CHANNEL_NAMES[ channel ] )
	)
      );
      break;
    }
}

/*****************************************************************************/
inline
const ParametersType::ValueType&
VectorImageModel::Settings
::GetRgbDynamicsParam( CountType i ) const
{
  return m_RgbDynamicsParams[ i ];
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetRgbDynamicsParam( CountType i,
		       const ParametersType::ValueType& param )
{
  // qDebug() << "SetDynamicsParam()";

  if( m_RgbDynamicsParams[ i ]==param )
    return;

  SetModified();

  m_RgbDynamicsParams[ i ] = param;
}

/*****************************************************************************/
inline
bool
VectorImageModel::Settings
::IsGrayscaleActivated() const
{
  return m_IsGrayscaleActivated;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetGrayscaleActivated( bool activated )
{
  m_IsGrayscaleActivated = activated;

  SetModified();
}

/*****************************************************************************/
inline
unsigned int
VectorImageModel::Settings
::GetGrayChannel() const
{
  return m_GrayChannel;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetGrayChannel( unsigned int index )
{
  m_GrayChannel = index;

  SetModified();
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetGrayDynamicsParams( const ParametersType& params )
{
  // qDebug() << "SetDynamicsParams()";

  assert( m_GrayDynamicsParams.size()==params.size() );

  if( std::equal( m_GrayDynamicsParams.begin(),
		  m_GrayDynamicsParams.end(),
		  params.begin() ) )
    return;

  m_GrayDynamicsParams = params;

  SetModified();
}

/*****************************************************************************/
inline
const ParametersType&
VectorImageModel::Settings
::GetGrayDynamicsParams() const
{
  return m_GrayDynamicsParams;
}

/*****************************************************************************/
inline
const ParametersType::ValueType&
VectorImageModel::Settings
::GetGrayDynamicsParam( bool high ) const
{
  return m_GrayDynamicsParams[ high ? 1 : 0 ];
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetGrayDynamicsParam( bool high,
			const ParametersType::ValueType& param )
{
  // qDebug() << "SetDynamicsParam()";

  CountType ofs = high ? 1 : 0;

  if( m_GrayDynamicsParams[ 0 * 2 + ofs ]==param )
    return;

  SetModified();

  // Indices are detailed for better readability since the compiler
  // will optimize constants.
  m_GrayDynamicsParams[ 0 * 2 + ofs ] = param;
  m_GrayDynamicsParams[ 1 * 2 + ofs ] = param;
  m_GrayDynamicsParams[ 2 * 2 + ofs ] = param;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetGamma( double value )
{
  m_Gamma = value;
}

/*****************************************************************************/
inline
double
VectorImageModel::Settings
::GetGamma() const
{
  return m_Gamma;
}

} // end namespace 'mvd'.

/*****************************************************************************/
/*****************************************************************************/

namespace mvd
{

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
::SetSettings( const Settings& settings )
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
