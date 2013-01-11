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

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.

/** \class VectorImageModel
 *
 */
class Monteverdi2_EXPORT VectorImageModel :
    public AbstractImageModel
{
  Q_OBJECT;

//
// Public types.
public:
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

  /**
   * TEMPORARY!
   */
  // TODO: Remove getter onto internal type when interface is designed.
  inline
    DefaultImageType*
    GetOutput( int index )
  {
    return m_ImageFileReader->GetOutput( index );
  }

  /**
   * TEMPORARY!
   */
  // TODO: Remove getter onto internal type when interface is designed.
  inline
    const DefaultImageType*
    GetOutput( int index ) const
  {
    //return const_cast< VectorImageModel* >( this )->GetOutput( index
    //);
    return m_ImageFileReader->GetOutput( index );
  }

  /**
   */
  QStringList GetBandNames() const;

  /**
   */
  inline
    const Settings&
    GetSettings() const
  {
    return m_Settings;
  }

  /**
   */
  void loadFile( const QString& filename );

  /** Rasterize the buffered region in a buffer */
  unsigned char * RasterizeRegion(const ImageRegionType& region);

//
// Public SLOTS.
public slots:
  /**
   */
  // TODO: Separate RenderingFilter and Settings (QObject) from
  // VectorImageModel and move slot.
  void onCurrentIndexChanged( ColorSetupWidget::Channel channel, int index );

//
// SIGNALS.
signals:
  /**
   */
  void settingsUpdated();

//
// Protected methods.
protected:

  /** Clear buffer */
  void ClearBuffer();

//
// Protected attributes.
protected:

//
// Private types.
private:
  /**
   * In-memory storage type of source image (from file).
   */
  typedef DefaultImageType SourceImageType;
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
    GetMetadataInterface() const
  {
    assert( m_ImageFileReader->GetNumberOfOutputs()==1 );

    itk::MetaDataDictionary dictionary(
      m_ImageFileReader->GetOutput( 0 )->GetMetaDataDictionary()
    );

    return otb::ImageMetadataInterfaceFactory::CreateIMI( dictionary );
  }

  /** Compute the linear buffer index according to the 2D region and
  * its 2D index.This method is used when OTB_GL_USE_ACCEL is ON.
  * \param index 2D index
  * \param region 2D region
  */
  static inline unsigned int ComputeBufferIndex(const IndexType& index, const ImageRegionType& region)
  {
    return (index[1] - region.GetIndex()[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
  }

  /** Compute the linear buffer index according to the 2D region and
   * its 2D index.This method is used when OTB_GL_USE_ACCEL is OFF.
   * The resulting buffer will be flipped over the X axis.
   * \param index 2D index
   * \param region 2D region
   */
  static inline unsigned int ComputeXAxisFlippedBufferIndex(const IndexType& index, const ImageRegionType& region)
  {
    return (region.GetSize()[1] - 1 + region.GetIndex()[1] -
            index[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
  }

  /** Compute the image index according a linear buffer index and its
   * 2D region
   * \param index 2D index
   * \param region 2D region
   */
  static inline IndexType
    ComputeImageIndexFromFlippedBuffer(const unsigned int & index, const ImageRegionType& region)
  {
    IndexType imageIndex;
    imageIndex[0] =  ( index % region.GetSize(0) ) + region.GetIndex()[0];

    imageIndex[1] =  region.GetSize()[1] - 1 + region.GetIndex()[1]
      - vcl_floor( index / region.GetSize(0) );

    return imageIndex;
  }

  /** Dump pixel of the current image within the region into the
  * m_RasterizedBuffer used for OpenGl rendering
  * \param region 2D region
  */
  void DumpImagePixelsWithinRegionIntoBuffer(const ImageRegionType& region);

  /** Compute the regions to be loaded and computes the already loaded
  * region (intersection with the previous requested region for
  * rasterization). Four regions are computed here {upper, lower,
  * right, left} region. Each region with size greater than 0 is added
  * to the m_RegionsToLoadVector vector.
  * \param region 2D region
  */
  void ComputeRegionsToLoad(const ImageRegionType& region);

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

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdVectorImageModel_h
