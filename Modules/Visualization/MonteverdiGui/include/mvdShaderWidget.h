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

#ifndef mvdShaderWidget_h
#define mvdShaderWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"
#include "mvdImageSettings.h"

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
#include "mvdImageSettingsInterface.h"


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
class ShaderWidget;
};

class ImageSettings;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ShaderWidget
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Widget template skeleton to copy-paste when adding a new
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT ShaderWidget : public QWidget, public ImageSettingsInterface
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** \brief Constructor. */
  ShaderWidget(QWidget* p = NULL, Qt::WindowFlags flags = 0);

  /** \brief Destructor. */
  ~ShaderWidget() override;

  /**
   */
  void SetGLSLEnabled(bool);
  /**
   */
  void SetGLSL140Enabled(bool);

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public Q_SLOTS:

  void GrayscaleActivated(bool status);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
Q_SIGNALS:
  void SettingsChanged();

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
  void SetEffectVisible(const Effect& effect, bool visible);

  /** */
  void virtual_SetSettings(ImageSettings*) override;

  /**
   * Set the ComboBox effects item for the corresponding image settings.
   * This will append or remove effects (lut) depending if settings
   * correspond to grayscale image or not.
   * \param imageSettings Settings of the current image.
   */
  void UpdateComboBoxEffectItems(ImageSettings* imageSettings);

  //
  // Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::ShaderWidget* m_UI;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private Q_SLOTS:
  void on_effectComboBox_currentIndexChanged(const QString&);
  void on_sizeSpinBox_valueChanged(int);
  void on_valueLineEdit_editingFinished();
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{
} // end namespace 'mvd'

#endif // mvdShaderWidget_h
