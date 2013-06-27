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
#ifndef __mvdPreferencesDialog_h
#define __mvdPreferencesDialog_h

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
class PreferencesDialog;
}


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class PreferencesDialog
 *
 */
class Monteverdi2_EXPORT PreferencesDialog :
    public QDialog
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** Constructor */
  PreferencesDialog( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

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

  bool m_ElevationSetupModified;
  bool m_ResultsDirModified;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:

  /**
   * \brief Qt auto-connected slot which is called when datasetPathButton menu
   * action is activated.
   */
  void on_buttonBox_accepted();


  void on_srtmCheckbox_clicked();

  void on_geoidCheckbox_clicked();

  void on_srtmButton_clicked();

  void on_geoidButton_clicked();

  void on_resultDirButton_clicked();


};

} // end namespace 'mvd'


/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // __mvdAboutDialog_h
