/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef mvdKeymapDialog_h
#define mvdKeymapDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

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
class KeymapDialog;
}

/** \class KeymapDialog
 *
 * \ingroup OTBMonteverdiGUI
 *
 */
class OTBMonteverdiGUI_EXPORT KeymapDialog : public QDialog
{
  Q_OBJECT;

  //
  // Public methods.
public:
  /** Constructor */
  KeymapDialog(QWidget* p = NULL, Qt::WindowFlags flags = 0);

  /** Destructor */
  ~KeymapDialog() override;

  //
  // SIGNALS.
signals:

  //
  // Protected methods.
protected:
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
   */
  Ui::KeymapDialog* m_UI;

  //
  // SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // mvdKeymapDialog_h
