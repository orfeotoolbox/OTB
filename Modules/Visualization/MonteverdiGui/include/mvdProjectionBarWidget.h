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


#ifndef mvdProjectionBarWidget_h
#define mvdProjectionBarWidget_h

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
#include <qnamespace.h>

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

namespace mvd
{
//
// Internal classes pre-declaration.
namespace Ui
{
class ProjectionBarWidget;
};

/**
 * \class ProjectionBarWidget
 * \ingroup OTBMonteverdiGUI
 * \brief ProjectionBarWidget widget class.
 *
 * This Widget allows changing the zoom level of the current selected projection.
 */
class OTBMonteverdiGUI_EXPORT ProjectionBarWidget : public QWidget
{

  Q_OBJECT;

public:
  /** \brief Constructor. */
  explicit ProjectionBarWidget(QWidget* p = NULL, Qt::WindowFlags flags = Qt::Widget);

  /** \brief Destructor. */
  ~ProjectionBarWidget() override;

public Q_SLOTS:
  void SetProjectionScale(double scale_x, double scale_y);

private:
  /**
   * \brief uic generated.
   */
  Ui::ProjectionBarWidget* m_UI;

  /**
   * emit a signal to change the scale if projectionScaleLineEdit is changed
   * and if the input text is valid.
   */
  void ChangeScale();

private Q_SLOTS:
  /**
   * When user edit the projection scale line
   */
  void on_projectionScaleLineEdit_editingFinished();
  /**
   * When user press return
   */
  void on_projectionScaleLineEdit_returnPressed();

Q_SIGNALS:
  /**
   */
  void ProjectionScaleChanged(double);
};

} // end namespace mvd


#endif // mvdProjectionBarWidget_h
