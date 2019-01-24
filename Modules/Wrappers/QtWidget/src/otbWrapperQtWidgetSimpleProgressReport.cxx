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


#include "otbWrapperQtWidgetSimpleProgressReport.h"
#include "otbWrapperOutputImageParameter.h"
#include "itksys/SystemTools.hxx"

#include "otbWrapperAddProcessToWatchEvent.h"

namespace otb
{
namespace Wrapper
{

QtWidgetSimpleProgressReport::QtWidgetSimpleProgressReport(QtWidgetModel * model, QWidget * parent)
  : QWidget(parent)
  , m_CurrentProcess()
{
  m_Model = model;
  connect(model, &QtWidgetModel::SetProgressReportBegin, this, &QtWidgetSimpleProgressReport::show );
  connect(model, &QtWidgetModel::SetProgressReportDone, this, &QtWidgetSimpleProgressReport::Init );
  connect(this, &QtWidgetSimpleProgressReport::AddNewProcessToReport, this, &QtWidgetSimpleProgressReport::ReportProcess );

  m_Layout = new QVBoxLayout;
  this->setLayout(m_Layout);

  m_AddProcessCommand = AddProcessCommandType::New();
  m_AddProcessCommand->SetCallbackFunction( this, &QtWidgetSimpleProgressReport::ProcessEvent );

  m_Bar =  new itk::QtProgressBar(this);

  m_Label = new QLabel("No process", this);
  m_Label->setWordWrap(true);
  connect( m_Bar, &itk::QtProgressBar::SetValueChanged, m_Bar, &itk::QtProgressBar::setValue );
  connect( m_Model, &QtWidgetModel::SetProgressReportDone, m_Bar, &itk::QtProgressBar::reset );

  m_Layout->addWidget(m_Label);
  m_Layout->addWidget(m_Bar);

  this->show();
}

QtWidgetSimpleProgressReport::~QtWidgetSimpleProgressReport()
{
}

void QtWidgetSimpleProgressReport::SetApplication(Application::Pointer app)
{
  m_Application = app;
  m_Application->AddObserver( AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer() );
}

void
QtWidgetSimpleProgressReport::ProcessEvent( itk::Object * itkNotUsed(caller),
                                      const itk::EventObject & event2 )
{
  if( typeid( otb::Wrapper::AddProcessToWatchEvent ) == typeid( event2 ) )
    {
    const AddProcessToWatchEvent* eventToWatch = dynamic_cast< const  AddProcessToWatchEvent*> ( &event2 );

    if(eventToWatch)
      {
      m_CurrentProcess = eventToWatch->GetProcess();
      m_CurrentDescription =  eventToWatch->GetProcessDescription();
      emit AddNewProcessToReport();
      }
    }
}

void QtWidgetSimpleProgressReport::ReportProcess()
{
  m_Bar->Observe(m_CurrentProcess);
  m_Label->setText(QString(m_CurrentDescription.c_str()));
}


void QtWidgetSimpleProgressReport::Init()
{
  m_Bar->setValue(0);
  m_Label->setText("No process");
}

}
}
