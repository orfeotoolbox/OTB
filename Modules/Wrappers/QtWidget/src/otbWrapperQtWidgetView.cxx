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

#include "otbWrapperQtWidgetView.h"

#include "otbWrapperQtWidgetParameterGroup.h"
#include "otbWrapperQtWidgetParameterFactory.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperQtWidgetSimpleProgressReport.h"

namespace otb
{
namespace Wrapper
{
/*
  TRANSLATOR mvd::Wrapper::QtWidgetView

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */

char const* const QtWidgetView::OBJECT_NAME = "otb::Wrapper::QtWidgetView";

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
QtWidgetView::QtWidgetView(const otb::Wrapper::Application::Pointer& otbApp, QWidget* parent, Qt::WindowFlags flags)
  : QWidget(parent, flags), m_Model(NULL), m_IsRunning(false)
{
  setObjectName(QtWidgetView::OBJECT_NAME);
  m_Model = new otb::Wrapper::QtWidgetModel(otbApp);
}

QtWidgetView::~QtWidgetView()
{
  delete m_Model;
  m_Model = NULL;
}

void QtWidgetView::CreateGui()
{
  QVBoxLayout* mainLayout = new QVBoxLayout;
  this->setLayout(mainLayout);
  mainLayout->addWidget(CreateInputWidgets());
}

QWidget* QtWidgetView::CreateInputWidgets()
{
  return otb::Wrapper::QtWidgetParameterFactory::CreateQtWidget(m_Model->GetApplication()->GetParameterList(), m_Model, this);
}

bool QtWidgetView::IsRunning() const
{
  return m_IsRunning;
}

QtWidgetModel* QtWidgetView::GetModel() const
{
  return m_Model;
}

void QtWidgetView::Disable()
{
  // Disable all widgets to make sure parameters are not updated when the application is running in another thread
  for (QWidget* w : this->findChildren<QWidget*>())
  {
    w->setEnabled(false);
  }
  this->setEnabled(false);
}

void QtWidgetView::Enable()
{
  // Reset all widgets of the view to their previous enabled state
  this->setEnabled(true);
  for (QWidget* w : this->findChildren<QWidget*>())
  {
    w->setEnabled(true);
  }
  // Resync widgets enabled state with parameter enabled flag
  this->GetModel()->NotifyUpdate();
}

} // end of namespace Wrapper
} // end of namespace otb
