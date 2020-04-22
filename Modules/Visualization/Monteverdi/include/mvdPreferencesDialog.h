/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef mvdPreferencesDialog_h
#define mvdPreferencesDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

#include "OTBMonteverdiExport.h"
/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

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
 * \ingroup OTBMonteverdi
 *
 */
class OTBMonteverdi_EXPORT PreferencesDialog : public QDialog
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** Constructor */
  PreferencesDialog(QWidget* p = NULL, Qt::WindowFlags flags = 0);

  /** Destructor */
  ~PreferencesDialog() override;

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // SIGNALS.
Q_SIGNALS:

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

  bool m_ResultsDirModified : 1;
  bool m_GeoidFileModified : 1;
  bool m_SrtmDirModified : 1;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:

  /**
   * \brief Qt auto-connected slot which is called when datasetPathButton menu
   * action is pushed.
   */
  void on_buttonBox_accepted();


  void on_srtmCheckbox_toggled(bool);
  void on_geoidCheckbox_toggled(bool);

  void on_srtmButton_clicked();
  void on_geoidButton_clicked();

  void on_srtmLineEdit_textChanged(const QString&);
  void on_geoidLineEdit_textChanged(const QString&);

  void on_resultDirButton_clicked();
};

} // end namespace 'mvd'


/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // mvdAboutDialog_h
