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
#ifndef __mvdMultiResolutionPyramidWidget_h
#define __mvdMultiResolutionPyramidWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbGDALOverviewsBuilder.h"

//
// Monteverdi includes (sorted by alphabetic order)


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
namespace Ui
{
class MultiResolutionPyramidWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class MultiResolutionPyramidWidget
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class Monteverdi_EXPORT MultiResolutionPyramidWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  MultiResolutionPyramidWidget( QWidget * parent =NULL,
				Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~MultiResolutionPyramidWidget();

  /**
   */
  void SetBuilder( const otb::GDALOverviewsBuilder::Pointer & );

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Protected attributes.
protected:

//
// Private methods.
private:
  /**
   */
  void ClearResolutions();
  /**
   */
  void SetResolutions();

//
// Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::MultiResolutionPyramidWidget * m_UI;
  /**
   */
  otb::GDALOverviewsBuilder::Pointer m_GDALOverviewsBuilder;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  void on_baseSpinBox_valueChanged( int );
  void on_levelsSpinBox_valueChanged( int );
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdMultiResolutionPyramidWidget_h
