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
#ifndef __mvdPreferencesDialog_h
#define __mvdPreferencesDialog_h

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
class PreferencesDialog;
}


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class PreferencesDialog
 *
 * \ingroup Monteverdi
 *
 */
class Monteverdi_EXPORT PreferencesDialog :
    public QDialog
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Constructor */
  PreferencesDialog( QWidget* p =NULL, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~PreferencesDialog();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// SIGNALS.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

//
// Private attributes.
private:
  /**
   */
  Ui::PreferencesDialog* m_UI;

  bool m_ResultsDirModified: 1;
  bool m_GeoidFileModified: 1;
  bool m_SrtmDirModified: 1;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:

  /**
   * \brief Qt auto-connected slot which is called when datasetPathButton menu
   * action is pushed.
   */
  void on_buttonBox_accepted();


  void on_srtmCheckbox_toggled( bool );
  void on_geoidCheckbox_toggled( bool );

  void on_srtmButton_clicked();
  void on_geoidButton_clicked();

  void on_srtmLineEdit_textChanged( const QString & );
  void on_geoidLineEdit_textChanged( const QString & );

  void on_resultDirButton_clicked();
};

} // end namespace 'mvd'


/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdAboutDialog_h
