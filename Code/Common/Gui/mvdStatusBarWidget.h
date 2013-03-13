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
#ifndef __mvdStatusBarWidget_h
#define __mvdStatusBarWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


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
class StatusBarWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class StatusBarWidget
 *
 * \brief Status Bar widget
 * widget class.
 */
class Monteverdi2_EXPORT StatusBarWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  StatusBarWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~StatusBarWidget();

  /** 
    * accessors to statusBar widgets 
    */
  QLineEdit * GetCurrentPixelIndexWidget();
  QLabel *    GetCurrentPixelPhysicalWidget();
  QLabel *    GetCurrentPixelGeographicWidget();
  QLabel *    GetCurrentPixelRadioWidget();
  QLineEdit * GetCurrentScaleWidget();

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

//
// Private attributes.
private:

  /**
   * \brief uic generated.
   */
  Ui::StatusBarWidget* m_UI;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
};

/*****************************************************************************/
/* INLINE SECTION                                                            */
/*****************************************************************************/

} // end namespace 'mvd'

#endif // __mvdStatusBarWidget_h
