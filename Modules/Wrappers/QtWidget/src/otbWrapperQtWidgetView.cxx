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

#include <functional>

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

char const * const
QtWidgetView
::OBJECT_NAME = "otb::Wrapper::QtWidgetView";

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
QtWidgetView::QtWidgetView( const otb::Wrapper::Application::Pointer & otbApp,
		QWidget* parent,
		Qt::WindowFlags flags ) :
  QWidget( parent, flags ),
  m_Model( NULL ),
  m_IsClosable( true ),
  m_IsRunning(false)
{
  setObjectName( QtWidgetView::OBJECT_NAME );

  m_Model = new otb::Wrapper::QtWidgetModel( otbApp );

  QObject::connect(
    m_Model, &QtWidgetModel::SetProgressReportBegin,
    this, &QtWidgetView::OnProgressReportBegin
  );

  QObject::connect(
    m_Model, &QtWidgetModel::SetProgressReportDone,
    this, &QtWidgetView::OnProgressReportEnd
  );
}

QtWidgetView::~QtWidgetView()
{
  delete m_Model;
  m_Model = NULL;
}

void QtWidgetView::CreateGui()
{
  QVBoxLayout *mainLayout = new QVBoxLayout;
  this->setLayout(mainLayout);
  mainLayout->addWidget(CreateInputWidgets());
}

QWidget* QtWidgetView::CreateInputWidgets()
{
  return otb::Wrapper::QtWidgetParameterFactory::CreateQtWidget(
      m_Model->GetApplication()->GetParameterList(),
      m_Model,
      this);
}


void QtWidgetView::closeEvent( QCloseEvent * e )
{
  assert( e!=NULL );

  if( !IsClosable() )
    {
    assert( GetModel()->GetApplication() );

    QMessageBox::warning(
      this,
      tr( "Warning!" ),
      tr( "OTB-Application '%1' cannot be closed while running!")
      .arg( GetModel()->GetApplication()->GetDocName() )
    );

    e->ignore();

    return;
    }

  QWidget::closeEvent( e );

  emit QuitSignal();

  deleteLater();
}

bool QtWidgetView::IsRunning() const
{
  return m_IsRunning;
}

QtWidgetModel* QtWidgetView::GetModel() const
{
return m_Model;
}

bool QtWidgetView::IsClosable() const
{
  return m_IsClosable;
}

void QtWidgetView::SetClosable( bool enabled )
{
  m_IsClosable = enabled;

  setEnabled( true );
}

void QtWidgetView::OnProgressReportBegin()
{
  SetClosable( false );
}

void QtWidgetView::OnProgressReportEnd( int )
{
  SetClosable( true );
}

void QtWidgetView::Disable()
{
  qInfo() << "QtWidgetView::Disable()";
  // Disable all widgets to make sure parameters are not updated when the application is running in another thread
  // Save their enabled state for
  m_EnabledState.clear();
  for (QWidget* w : this->findChildren<QWidget*>())
  {
    m_EnabledState[w] = w->isEnabled();
    qInfo() << w << w->isEnabled();
  }

  for (QWidget* w : this->findChildren<QWidget*>())
  {
    w->setEnabled(false);
  }
  this->setEnabled(false);
}

void QtWidgetView::Enable()
{
  qInfo() << "QtWidgetView::Enable()";
  this->setEnabled(true);
  for (QWidget* w : this->findChildren<QWidget*>())
  {
    qInfo() << "enabling " << w << m_EnabledState[w];
    w->setEnabled(m_EnabledState[w]);
  }
}

} // end of namespace Wrapper
} // end of namespace otb

