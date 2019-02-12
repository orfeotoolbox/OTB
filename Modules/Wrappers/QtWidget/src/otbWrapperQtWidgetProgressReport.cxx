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


#include "otbWrapperQtWidgetProgressReport.h"
#include "otbWrapperOutputImageParameter.h"
#include "itksys/SystemTools.hxx"

#include "otbWrapperAddProcessToWatchEvent.h"

namespace otb
{
namespace Wrapper
{

QtWidgetProgressReport::QtWidgetProgressReport(QtWidgetModel * model, QWidget * parent)
  : QWidget(parent)
  , m_CurrentProcess()
{
  m_Model = model;
  connect(model, &QtWidgetModel::SetProgressReportBegin, this, &QtWidgetProgressReport::show );
  connect(model, &QtWidgetModel::SetProgressReportDone, this, &QtWidgetProgressReport::close );
  connect(model, &QtWidgetModel::SetProgressReportDone, this, &QtWidgetProgressReport::RemoveLayout );
  connect(this, &QtWidgetProgressReport::AddNewProcessToReport, this, &QtWidgetProgressReport::ReportProcess );

  m_Layout = new QVBoxLayout;
  this->setLayout(m_Layout);

  m_AddProcessCommand = AddProcessCommandType::New();
  m_AddProcessCommand->SetCallbackFunction( this, &QtWidgetProgressReport::ProcessEvent );

  this->show();
}

QtWidgetProgressReport::~QtWidgetProgressReport()
{
}

void QtWidgetProgressReport::SetApplication(Application::Pointer app)
{
  m_Application = app;
  m_Application->AddObserver( AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer() );
}

void
QtWidgetProgressReport::ProcessEvent( itk::Object * itkNotUsed(caller),
                                      const itk::EventObject & ev )
{
  if( typeid( otb::Wrapper::AddProcessToWatchEvent ) == typeid( ev ) )
    {
    const AddProcessToWatchEvent* eventToWatch = dynamic_cast< const  AddProcessToWatchEvent*> ( &ev );

    if(eventToWatch)
      {
      m_CurrentProcess = eventToWatch->GetProcess();
      m_CurrentDescription =  eventToWatch->GetProcessDescription();
      emit AddNewProcessToReport();
      }
    }
}

void QtWidgetProgressReport::ReportProcess ( )
{
  // Build the widget containing the QtProgressBar for the current
  // process

  // Create a itk::QtProgressBar, observing the event ProgressEvent
  itk::QtProgressBar * bar =  new itk::QtProgressBar(this);
  connect( bar, &itk::QtProgressBar::SetValueChanged, bar, &itk::QtProgressBar::setValue );
  connect( m_Model, &QtWidgetModel::SetProgressReportDone, bar, &itk::QtProgressBar::reset );
  bar->Observe(m_CurrentProcess);

  // label
  QLabel *label = new QLabel(QString(m_CurrentDescription.c_str()), this);

  // Build the layout and store the pointers
  m_Layout->addWidget(label);
  m_Layout->addWidget(bar);
}

void QtWidgetProgressReport::RemoveLayout()
{
  // Remove the children of the layout (progress bar widgets)
  QLayoutItem *child;
  while ((child = this->layout()->takeAt(0)) != nullptr)
    {
    delete child->widget();
    delete child;
    }
}

}
}
