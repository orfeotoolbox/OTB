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

namespace core
{

//
// Internal classes pre-declaration.
class HistogramModel;
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
    BuildContext( void* const settings ) :
      m_Id( -1 ),
      m_Filename(),
      m_Quicklook(),
      m_Settings( settings )
    {
    }

    /** \brief Constructor. */
    BuildContext( const QString& filename ) :
      m_Id( -1 ),
      m_Filename( filename ),
      m_Quicklook(),
      m_Settings( NULL )
    {
    }

    //
    // Public attributes
  public:
    int m_Id;
    QString m_Filename;
    QString m_Quicklook;
    void * const m_Settings;
  };

//
// Public methods.
public:

  /** Destructor */
  virtual ~AbstractImageModel();

  /** */
  inline int GetId() const;

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
  inline ImageRegionType GetLodLargestRegion() const;

  /**
   * \return The largest possible region of the native image, which
   * is, by default, LOD level zero.
   */
  inline ImageRegionType GetNativeLargestRegion() const;

  /**
   * \return The spacing of the native image, which
   * is, by default, LOD level zero.
   */
  inline SpacingType GetNativeSpacing() const;

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
  inline SpacingType GetSpacing();

  /** */
  inline PointType GetOrigin();

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /** */
  void SpacingChanged( const core::SpacingType& );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** Constructor */
  AbstractImageModel( QObject* parent =NULL );

  /** */
  inline
    otb::ImageMetadataInterfaceBase::ConstPointer
    GetMetaDataInterface() const;

  /** */
  virtual void virtual_SetCurrentLod( CountType lod ) =0;

  //
  // AbstractModel overrides.

  /** */
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

//
// Private attributes.
private:
  /**
   */
  int m_Id;

  /**
   */
  CountType m_CurrentLod;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'core'

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

namespace core
{

/*****************************************************************************/
int
AbstractImageModel
::GetId() const
{
  return m_Id;
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
ImageRegionType
AbstractImageModel
::GetLodLargestRegion() const
{
  return ToImageBase()->GetLargestPossibleRegion();
}

/*****************************************************************************/
inline
ImageRegionType
AbstractImageModel
::GetNativeLargestRegion() const
{
  return m_NativeLargestRegion;
}

/*****************************************************************************/
inline
SpacingType
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
SpacingType 
AbstractImageModel
::GetSpacing()
{
  return ToImageBase()->GetSpacing();
}

/*****************************************************************************/
inline
PointType
AbstractImageModel
::GetOrigin()
{
  return ToImageBase()->GetOrigin();
}

} // end namespace 'core'

} // end namespace 'mvd'

#endif // __mvdAbstractImageModel_h
