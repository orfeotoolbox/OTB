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

#ifndef mvdDropLineEdit_h
#define mvdDropLineEdit_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"


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
#include "OTBMonteverdiGUIExport.h"
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
// class DropLineEdit;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class DropLineEdit
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT DropLineEdit : public QLineEdit
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** \brief Constructor. */
  DropLineEdit(QWidget* p = 0);

  /** \brief Constructor. */
  DropLineEdit(const QString& contents, QWidget* p = 0);

  /** \brief Destructor. */
  ~DropLineEdit() override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

  //
  // Protected methods.
protected:
  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dropEvent(QDropEvent* event) override;

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
  // Ui::DropLineEdit* m_UI;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // mvdDropLineEdit_h
