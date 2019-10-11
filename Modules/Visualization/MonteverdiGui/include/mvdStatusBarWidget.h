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

#ifndef mvdStatusBarWidget_h
#define mvdStatusBarWidget_h

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
namespace Ui
{
class StatusBarWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class StatusBarWidget
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief Status Bar widget
 * widget class.
 */
class OTBMonteverdiGUI_EXPORT StatusBarWidget : public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

  //
  // Public methods.
public:
  /** \brief Constructor. */
  StatusBarWidget(QWidget* p = NULL, Qt::WindowFlags flags = 0);

  /** \brief Destructor. */
  ~StatusBarWidget() override;

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

  //
  // Public SLOTS.
public slots:

  /**
   */
  void SetPixelIndex(const IndexType&, bool isInside);

  /**
   */
  void SetText(const QString&);

  /**
   */
  void SetScale(double sx, double sy);

  /**
   */
  void SetGLSLEnabled(bool);

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

  //
  // Signals.
signals:
  /**
   */
  void ScaleChanged(double);
  /**
   */
  void PixelIndexChanged(const IndexType&);

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
  static QString ZoomLevel(double scale);

  //
  // Private attributes.
private:
  /**
   * \brief uic generated.
   */
  Ui::StatusBarWidget* m_UI;

  /**
   * \brief Change the scale when scaleLineEdit is pressed or editing is finished with change.
   */
  void ChangeScale();

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

  //
  // Slots.
private slots:
  /**
   */
  // void SetPixelIndexText( const QString& );
  /**
   */
  void on_scaleLineEdit_editingFinished();
  /**
   */
  void on_pixelIndexLineEdit_returnPressed();
  /**
   */
  void on_scaleLineEdit_returnPressed();
};

} // end namespace 'mvd'

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
#include "mvdAlgorithm.h"

namespace mvd
{

} // end namespace 'mvd'

#endif // mvdStatusBarWidget_h
