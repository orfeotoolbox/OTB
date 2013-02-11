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
class HistogramModel;

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
// Public methods.
public:

  /** Destructor */
  virtual ~AbstractImageModel();

  /**
   * \return The largest possible region of the current LOD level.
   */
  inline ImageRegionType GetLodLargestRegion() const;

  /**
   * \return The largest possible region of the native image, which
   * is, by default, LOD level zero.
   */
  inline ImageRegionType GetNativeLargestRegion() const;

  /** */
  inline CountType GetNbComponents() const;

  /**
   */
  QStringList GetBandNames() const;

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

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

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
  virtual void virtual_BuildModel();

//
// Protected attributes.
protected:

  /**
   * The largest possible region of the native image, which is, by
   * default, LOD level zero.
   */
  ImageRegionType m_NativeLargestRegion;

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

//
// Private attributes.
private:
  CountType m_CurrentLod;

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
  if( GetCurrentLod()==lod )
    return;

  virtual_SetCurrentLod( lod );

  m_CurrentLod = lod;
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

} // end namespace 'mvd'

#endif // __mvdAbstractImageModel_h
