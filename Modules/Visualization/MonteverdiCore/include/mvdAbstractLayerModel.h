/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __mvdAbstractLayerModel_h
#define __mvdAbstractLayerModel_h


//
// Configuration include.
//// Included at first position before any other ones.
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "ConfigureMonteverdi.h"
#endif //tag=QT4-boost-compatibility

#include "OTBMonteverdiCoreExport.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "mvdAbstractModel.h"
#include "mvdTypes.h"
#include "mvdVisibleInterface.h"
#endif //tag=QT4-boost-compatibility

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


//
// Global types.

/**
 */
enum SpatialReferenceType
{
  SRT_UNKNOWN =0,
  SRT_CARTO,
  SRT_GEO,
  SRT_SENSOR,
};

//
// Global functions.

/**
 */
OTBMonteverdiCore_EXPORT SpatialReferenceType GetSpatialReferenceType( const std::string & filename );

/**
 */
OTBMonteverdiCore_EXPORT SpatialReferenceType GetSpatialReferenceType( const std::string & wkt, bool hasKwl );

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class AbstractLayerModel
 *
 * \ingroup OTBMonteverdiCore
 *
 * \brief WIP.
 */
class OTBMonteverdiCore_EXPORT AbstractLayerModel :
    public AbstractModel,
    public VisibleInterface
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Destructor. */
  virtual ~AbstractLayerModel();

  /**
   */
  SpatialReferenceType GetSpatialReferenceType() const;

  /**
   */
  std::string GetWkt() const;

  /**
   */
  std::string GetAuthorityCode( bool ) const;

  /**
   */
  void ToWgs84( const PointType &, PointType & wgs84, double & alt ) const;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void VisibilityChanged( bool =true );
  void VisibilityChanged( AbstractLayerModel *, bool );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /** \brief Constructor. */
  AbstractLayerModel( QObject* p =NULL );

  /**
   */
  bool HasKwl() const;

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  /**
   */
  virtual std::string virtual_GetWkt() const =0;

  /**
   */
  virtual bool virtual_HasKwl() const;

  /**
   */
  virtual void virtual_ToWgs84( const PointType &, PointType & wgs84, double & alt ) const =0;

  //
  // VisibleInterface overloads.
  virtual void virtual_SignalVisibilityChanged( bool );

//
// Private attributes.
private:

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdAbstractLayerModel_h
