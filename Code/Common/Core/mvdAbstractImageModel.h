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

#ifndef __mvdAbstractImageModel_h
#define __mvdAbstractImageModel_h

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

//
// OTB includes (sorted by alphabetic order)
#include "otbImageMetadataInterfaceBase.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractModel.h"
#include "mvdImageProperties.h"
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
class HistogramModel;
class ImageProperties;
class QuicklookModel;


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class AbstractImageModel
 *
 */
class Monteverdi2_EXPORT AbstractImageModel :
    public AbstractModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /**
   * \class BuildContext
   * \brief WIP.
   */
  class BuildContext
  {
    //
    // Public methods.
  public:
    /** \brief Constructor. */
    BuildContext( bool isBeingStored,
		  void* const settings =NULL,
		  const QString& filename =QString() ) :
      m_Id( -1 ),
      m_Filename( filename ),
      m_Quicklook(),
      m_Histogram(),
      m_Settings( settings ),
      m_IsBeingStored( isBeingStored )
    {
      assert( settings!=NULL || !filename.isEmpty() );
    }

    /** \brief Constructor. */
    BuildContext( bool isBeingStored,
		  const QString& filename ) :
      m_Id( -1 ),
      m_Filename( filename ),
      m_Quicklook(),
      m_Histogram(),
      m_Settings( NULL ),
      m_IsBeingStored( isBeingStored )
    {
      assert( !filename.isEmpty() );
    }

    /**
     */
    inline bool IsBeingStored() const
    {
      return m_IsBeingStored;
    }

    //
    // Public attributes
  public:
    int m_Id;
    QString m_Filename;
    QString m_Quicklook;
    QString m_Histogram;
    void * const m_Settings;

  private:
    bool m_IsBeingStored;

  };

//
// Public methods.
public:

  /** Destructor */
  virtual ~AbstractImageModel();

  /** */
  inline int GetId() const;

  /**
   */
  inline const ImageProperties* GetProperties() const;

  /**
   */
  inline ImageProperties* GetProperties();

  /** */
  const QuicklookModel* GetQuicklookModel() const;

  /** */
  QuicklookModel* GetQuicklookModel();

  /** */
  inline const HistogramModel* GetHistogramModel() const;

  /** */
  inline HistogramModel* GetHistogramModel();

  /**
   * Get a smart-pointer to the current LOD image-base.
   */
  virtual ImageBaseType::ConstPointer ToImageBase() const =0;

  /**
   * Get a smart-pointer to the current LOD image-base.
   */
  virtual ImageBaseType::Pointer ToImageBase() =0;

  /**
   * \return The largest possible region of the current LOD level.
   */
  inline const ImageRegionType& GetLodLargestRegion() const;

  /**
   * \return The largest possible region of the native image, which
   * is, by default, LOD level zero.
   */
  inline const ImageRegionType& GetNativeLargestRegion() const;

  /**
   * \return The spacing of the native image, which
   * is, by default, LOD level zero.
   */
  inline const SpacingType& GetNativeSpacing() const;

  /** */
  inline CountType GetNbComponents() const;

  /**
   */
  QStringList GetBandNames( bool enhanced =false ) const;

  /**
   * Get the number of available LOD.
   */
  virtual CountType GetNbLod() const =0;

  /**
   * Set the current LOD index (which may causes disk IOs,
   * decompressing and buffering etc.)
   */
  inline void SetCurrentLod( CountType lod );

  /**
   * Get the current LOD index.
   */
  inline CountType GetCurrentLod() const;

  /** */
  inline const SpacingType& GetSpacing() const;

  /** */
  inline const PointType& GetOrigin() const;

  /** Release as much memory as possible (default implementation does
   *  nothing). */
  virtual void ReleaseMemory() =0;

//
// Public SLOTS.
public slots:

  /**
   * \brief Refresh histogram-model based on no-data settings.
   *
   * \param noDataFlag true to enable no-data pixel filtering.
   * \param noDataValue Value of each no-data pixel component.
   */
  inline void RefreshHistogram( bool noDataFlag,
				ComponentType noDataValue =ComponentType( 0 ) );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /** */
  void SpacingChanged( const SpacingType& );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** Constructor */
  AbstractImageModel( QObject* parent =NULL );

  /**
   * \brief Set image properties pointer.
   *
   * \param properties The new properties instance.
   */
  inline void SetProperties( ImageProperties* properties );

  /**
   * \brief Refresh histogram-model based on no-data settings.
   *
   * \param context Pointer to a histogram build-context.
   */
  void RefreshHistogram( void* const context );

  /**
   */
  inline
    otb::ImageMetadataInterfaceBase::ConstPointer
    GetMetaDataInterface() const;

  //
  // AbstractModel overrides.

  virtual void virtual_BuildModel( void* context );

//
// Protected attributes.
protected:

  /**
   * The largest possible region of the native image, which is, by
   * default, LOD level zero.
   */
  ImageRegionType m_NativeLargestRegion;

  /**
   * The spacing of the native image, which is, by
   * default, LOD level zero.
   */
  SpacingType m_NativeSpacing;

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   */
  virtual void virtual_SetCurrentLod( CountType lod ) =0;

  /**
   * \brief Private virtual implementation of RefreshHistogram.
   *
   * Implement this method in subclass to construct a valid
   * HistogramModel::BuildContext and call RefreshHistogram(void*
   * const).
   *
   * \param noDataFlag true to enable no-data pixel filtering.
   * \param noDataValue Value of each no-data pixel component.
   */
  virtual
    void
    virtual_RefreshHistogram( bool noDataFlag,
			      ComponentType noDataValue =ComponentType( 0 ) ) =0;

//
// Private attributes.
private:
  /**
   */
  int m_Id;

  /**
   */
  CountType m_CurrentLod;

  /**
   */
  ImageProperties* m_Properties;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdHistogramModel.h"

namespace mvd
{

/*****************************************************************************/
int
AbstractImageModel
::GetId() const
{
  return m_Id;
}

/*****************************************************************************/
inline
const ImageProperties*
AbstractImageModel
::GetProperties() const
{
  return m_Properties;
}

/*****************************************************************************/
inline
ImageProperties*
AbstractImageModel
::GetProperties()
{
  return m_Properties;
}

/*****************************************************************************/
const HistogramModel*
AbstractImageModel
::GetHistogramModel() const
{
  return findChild< const HistogramModel* >();
}

/*****************************************************************************/
HistogramModel*
AbstractImageModel
::GetHistogramModel()
{
  return findChild< HistogramModel* >();
}

/*****************************************************************************/
CountType
AbstractImageModel
::GetCurrentLod() const
{
  return m_CurrentLod;
}

/*****************************************************************************/
void
AbstractImageModel
::SetCurrentLod( CountType lod )
{
  assert( lod<GetNbLod() );

  try
    {
    virtual_SetCurrentLod( lod );

    m_CurrentLod = lod;
    }
  catch( std::exception& exc )
    {
    throw;
    }

  // if everything ok emit the new spacing of the current lod
  emit SpacingChanged(ToImageBase()->GetSpacing());
}

/*****************************************************************************/
inline
const ImageRegionType&
AbstractImageModel
::GetLodLargestRegion() const
{
  return ToImageBase()->GetLargestPossibleRegion();
}

/*****************************************************************************/
inline
const ImageRegionType&
AbstractImageModel
::GetNativeLargestRegion() const
{
  return m_NativeLargestRegion;
}

/*****************************************************************************/
inline
const SpacingType&
AbstractImageModel
::GetNativeSpacing() const
{
  return m_NativeSpacing;
}

/*****************************************************************************/
inline
CountType
AbstractImageModel
::GetNbComponents() const
{
  return ToImageBase()->GetNumberOfComponentsPerPixel();
}

/*****************************************************************************/
inline
otb::ImageMetadataInterfaceBase::ConstPointer
AbstractImageModel
::GetMetaDataInterface() const
{
  return otb::ConstCast< const otb::ImageMetadataInterfaceBase >(
    otb::ImageMetadataInterfaceFactory::CreateIMI(
      ToImageBase()->GetMetaDataDictionary()
    )
  );
}

/*****************************************************************************/
inline
const SpacingType&
AbstractImageModel
::GetSpacing() const
{
  return ToImageBase()->GetSpacing();
}

/*****************************************************************************/
inline
const PointType&
AbstractImageModel
::GetOrigin() const
{
  return ToImageBase()->GetOrigin();
}

/*****************************************************************************/
inline
void
AbstractImageModel
::RefreshHistogram( bool noDataFlag, ComponentType noDataValue )
{
  virtual_RefreshHistogram( noDataFlag, noDataValue );
}

/*****************************************************************************/
inline
void
AbstractImageModel
::SetProperties( ImageProperties* properties )
{
  if( m_Properties==properties )
    return;

  delete m_Properties;

  m_Properties = properties;
}

} // end namespace 'mvd'

#endif // __mvdAbstractImageModel_h
