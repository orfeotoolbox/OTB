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

#ifndef mvdWrapperQtWidgetView_h
#define mvdWrapperQtWidgetView_h

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
#include <QShortcut>

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperQtWidgetView.h"
#include "otbWrapperQtWidgetModel.h"
#include "OTBMonteverdiGUIExport.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"

namespace mvd
{
namespace Wrapper
{

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ApplicationLauncher
 *
 * \ingroup OTBMonteverdiGUI
 *
 * \brief WIP.
 */

class OTBMonteverdiGUI_EXPORT QtWidgetView : public otb::Wrapper::QtWidgetView
{
  Q_OBJECT

public:
  static char const* const OBJECT_NAME;

  /** \brief Constructor. */
  QtWidgetView(const otb::Wrapper::Application::Pointer& otbApp, QWidget* p = nullptr, Qt::WindowFlags flags = 0);

  /** \brief Destructor. */
  ~QtWidgetView() override;

  bool BeforeExecuteButtonClicked() override;

protected:
  QWidget* CreateInputWidgets() override;

private:
  QtWidgetView(const QtWidgetView&) = delete;
  void operator=(const QtWidgetView&) = delete;

  void SetupParameterWidgets(QWidget* widget);

  void SetupFileSelectionWidget(QWidget*);

private Q_SLOTS:

  // slot in charge of emitting a signal to the catalogue mainWindow.
  // when received, the main application need to get the output
  // image filename{s} set by the user in this OTB application (if any).
  void OnApplicationExecutionDone(int);

  inline void OnFileSelectionWidgetAdded0(QWidget*);

  inline void OnFileSelectionWidgetAdded1(QWidget*);
};

} // end namespace 'Wrapper'

} // end namespace 'mvd'

namespace mvd
{

namespace Wrapper
{

inline void QtWidgetView::OnFileSelectionWidgetAdded0(QWidget* widget)
{
  SetupFileSelectionWidget(widget);
}

inline void QtWidgetView::OnFileSelectionWidgetAdded1(QWidget* widget)
{
  SetupFileSelectionWidget(widget);
}

} // end namespace 'Wrapper'

} // end namespace 'mvd'

#endif
