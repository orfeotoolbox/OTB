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
     * \param
     *
     * Copy settings POD content without modified nor applied flag.
     */
    Settings( const Settings& other );

    /**
     * \brief Destructor.
     */
    ~Settings() ;

    /**
     */
    inline bool IsApplied() const;

    /**
     */
    inline bool IsModified() const;

    /** */
    inline void SetModified();

    /** */
    inline void ClearModified();

    /** */
    inline void SetApplied();

    /** */
    inline void SetRgbChannels( const ChannelVector& rgb );

    /** */
    inline const ChannelVector& GetRgbChannels() const;

    /**
     */
#if 0
    inline ChannelVector::value_type& RgbChannel( ChannelVector::size_type i );
#endif

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
     */
    inline void SetDynamicsParams( const ParametersType& params );

    /**
     */
    inline const ParametersType& GetDynamicsParams() const;

    /**
     */
    inline
    const ParametersType::ValueType& GetDynamicsParam( CountType i ) const;

    /**
     */
    inline
    void SetDynamicsParam( CountType i,
			   const ParametersType::ValueType& param );

    /**
     */
#if 0
    inline ParametersType::ValueType& DynamicsParam( CountType i );
#endif

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
    ParametersType m_DynamicsParams;

    /**
     * \brief Flag which notices that rendering settings have been
     * edited.
     */
    // TODO: Optimize using C++ bitset bool foo:1;
    bool m_IsModified;

    /**
     * \brief Flag which notices that rendering settings have been
     * applied to display.
     */
    // TODO: Optimize using C++ bitset bool foo:1;
    bool m_IsApplied;
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
  static void EnsureValidImage(QString filename)
  {
    try
    {
      DefaultImageFileReaderType::Pointer imageFileReader = DefaultImageFileReaderType::New();
      imageFileReader->SetFileName( ToStdString( filename ) );
      imageFileReader->UpdateOutputInformation();
    }
    catch(std::exception& exc )
    {
      // TODO manage the message returned by OTB
      qDebug() << "Error returned by OTB:" << exc.what();
      throw std::runtime_error(
          ToStdString(
              tr("OTB can not read this file ('%1').\n"
                  "Please check.").arg( filename ) ) );
    }
  }

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

  //
  // AbstractModel overrides.

  virtual bool IsModified() const;

  virtual void ClearModified();

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
  /**
   */
  void SettingsUpdated( AbstractImageModel* imageModel =NULL );

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
inline
VectorImageModel::Settings
::Settings() :
  m_RgbChannels(),
  m_DynamicsParams( 6 ),
  m_IsModified( false ),
  m_IsApplied( false )
{
}

/*****************************************************************************/
inline
VectorImageModel::Settings
::Settings( const Settings& other ) :
  m_RgbChannels( other.m_RgbChannels ),
  m_DynamicsParams( other.m_DynamicsParams ),
  m_IsModified( false ),
  m_IsApplied( false )
{
  qDebug() << "Settings( const Settigs& )";
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
  qDebug() << "SetModified()";

  m_IsModified = true;
  m_IsApplied = false;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::ClearModified()
{
  qDebug() << "ClearModified()";

  m_IsModified = false;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetApplied()
{
  qDebug() << "SetApplied()";

  m_IsApplied = true;
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetRgbChannels( const ChannelVector& rgb )
{
  qDebug() << "setRgbChannels()";

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
#if 0
inline
const VectorImageModel::Settings::ChannelVector::value_type&
VectorImageModel::Settings
::RgbChannel( ChannelVector::size_type i )
{
  SetModified();

  return m_RgbChannels[ i ];
}
#endif

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetRgbChannel( ChannelVector::size_type i,
		 const ChannelVector::value_type& channel )
{
  qDebug() << "SetRgbChannel()";

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
void
VectorImageModel::Settings
::SetDynamicsParams( const ParametersType& params )
{
  qDebug() << "SetDynamicsParams()";

  assert( m_DynamicsParams.size()==params.size() );

  if( std::equal( m_DynamicsParams.begin(),
		  m_DynamicsParams.end(),
		  params.begin() ) )
    return;

  m_DynamicsParams = params;

  SetModified();
}

/*****************************************************************************/
inline
const ParametersType&
VectorImageModel::Settings
::GetDynamicsParams() const
{
  return m_DynamicsParams;
}

/*****************************************************************************/
inline
const ParametersType::ValueType&
VectorImageModel::Settings
::GetDynamicsParam( CountType i ) const
{
  return m_DynamicsParams[ i ];
}

/*****************************************************************************/
inline
void
VectorImageModel::Settings
::SetDynamicsParam( CountType i,
		    const ParametersType::ValueType& param )
{
  qDebug() << "SetDynamicsParam()";

  if( m_DynamicsParams[ i ]==param )
    return;

  SetModified();

  m_DynamicsParams[ i ] = param;
}

/*****************************************************************************/
#if 0
inline
ParametersType::ValueType&
VectorImageModel::Settings
::DynamicsParam( CountType i )
{
  SetModified();

  return m_DynamicsParams[ i ];
}
#endif

} // end namespace 'mvd'.

namespace mvd
{

/*****************************************************************************/
const DatasetModel*
VectorImageModel
::GetDatasetModel() const
{
  // assert( GetParentModel()==GetParentModel< DatasetModel >() );
  return GetParentModel< DatasetModel >();
}

/*****************************************************************************/
DatasetModel*
VectorImageModel
::GetDatasetModel()
{
  // assert( GetParentModel()==GetParentModel< DatasetModel >() );
  return GetParentModel< DatasetModel >();
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
