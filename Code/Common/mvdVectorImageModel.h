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
// Monteverdi includes (sorted by alphabetic order)
#include "mvdColorSetupWidget.h"
#include "mvdAbstractImageModel.h"
#include "mvdTypes.h"

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
      m_RenderingFuncParams()
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
    RgbChannel( int i )
    {
      SetDirty();
      return m_RgbChannels[ i ];
    }

    /**
     */
    inline
    const ChannelVector::value_type&
    RgbChannel( int i ) const
    {
      return m_RgbChannels[ i ];
    }

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
    ParametersType m_RenderingFuncParams;
  };

//
// Public methods.
public:

  /** Constructor */
  VectorImageModel( QObject* parent =NULL );

  /** Destructor */
  virtual ~VectorImageModel();

  /** */
  // TODO: Move into template wrapper base-class.
  SourceImageType::ConstPointer ToImage() const;

  /** */
  // TODO: Move into template wrapper base-class.
  const SourceImageType::Pointer& ToImage();

  /**
   * TEMPORARY?
   */
  // TODO: Remove getter onto internal type when interface is designed.
  inline DefaultImageType* GetOutput( int index );

  /**
   * TEMPORARY?
   */
  // TODO: Remove getter onto internal type when interface is designed.
  inline const DefaultImageType* GetOutput( int index ) const;

  /**
   * Get the largest possible region of the input image
   */
  inline const ImageRegionType GetImageLargestPossibleRegion() const;

  /**
   */
  QStringList GetBandNames() const;

  /**
   */
  inline const Settings& GetSettings() const;

  /**
   * w and h are added to compute the best level of detail to load
   * from the image if multi-resolution image
   */
  void LoadFile( const QString& filename, int w, int h );

  /** Rasterize the buffered region in a buffer */
  unsigned char * RasterizeRegion(const ImageRegionType& region, const double zoomFactor);

  /**
   * Following the zoom factor, get the best level of detail knowing
   * the overviews size
   */
  bool GetBestLevelOfDetail(const double ZoomFactor, int& lod);
    
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

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public slots.
public slots:
  /**
   */
  // TODO: Separate RenderingFilter and Settings (QObject) from
  // VectorImageModel and move slot.
  void OnCurrentIndexChanged( RgbaChannel channel, int index );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void SettingsUpdated();

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** Clear buffer */
  void ClearBuffer();

  //
  // AbstractImageModel

  /** */
  virtual void virtual_SetCurrentLod( unsigned int lod );

  /** helper to compose the filename used for j2k to load different
    * resolution 
    */
  const std::string FilenameHelper(double zoomFactor);
  const std::string FilenameHelper(int w, int h);

//
// Protected attributes.
protected:

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

  /** */
  inline
    DefaultImageType::ImageMetadataInterfacePointerType
    GetMetadataInterface() const;

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
  unsigned int Closest(double invZoomfactor, const std::vector<unsigned int> & res);
  
//
// Private attributes.
private:
  // Default image reader
  DefaultImageFileReaderType::Pointer m_ImageFileReader;

  /** */
  SourceImageType::Pointer m_Image;

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
  std::string                          m_InputFilename;

  // store the previous levelofDetail found
  unsigned int   m_PreviousBestLevelOfDetail;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

/*****************************************************************************/
/* INLINE SECTION                                                            */

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
DefaultImageType*
VectorImageModel
::GetOutput( int index )
{
  return m_ImageFileReader->GetOutput( index );
}

/*****************************************************************************/
inline
const DefaultImageType*
VectorImageModel
::GetOutput( int index ) const
{
  //return const_cast< VectorImageModel* >( this )->GetOutput( index
  //);
  return m_ImageFileReader->GetOutput( index );
}

/*****************************************************************************/
inline
const ImageRegionType
VectorImageModel
::GetImageLargestPossibleRegion() const
{
  // make sure to return the largest region of the resolution 0 (if j2k)
  DefaultImageFileReaderType::Pointer reader = DefaultImageFileReaderType::New();
  reader->SetFileName(m_InputFilename);
  reader->UpdateOutputInformation();
  
  return reader->GetOutput()->GetLargestPossibleRegion();
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
DefaultImageType::ImageMetadataInterfacePointerType
VectorImageModel
::GetMetadataInterface() const
{
  assert( m_ImageFileReader->GetNumberOfOutputs()==1 );

  itk::MetaDataDictionary dictionary(
    m_ImageFileReader->GetOutput( 0 )->GetMetaDataDictionary()
  );

  return otb::ImageMetadataInterfaceFactory::CreateIMI( dictionary );
}

/*****************************************************************************/
inline
unsigned int
VectorImageModel
::ComputeBufferIndex(const IndexType& index,
		     const ImageRegionType& region)
{
  return (index[1] - region.GetIndex()[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
}

/*****************************************************************************/
inline
unsigned int
VectorImageModel
::ComputeXAxisFlippedBufferIndex(const IndexType& index,
				 const ImageRegionType& region)
{
  return (region.GetSize()[1] - 1 + region.GetIndex()[1] -
	  index[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
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
