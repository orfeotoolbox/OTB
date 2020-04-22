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

QtWidgetSimpleProgressReport::QtWidgetSimpleProgressReport(QWidget* parent) : itk::QtProgressBar(parent), m_CurrentProcess()
{
}

void QtWidgetSimpleProgressReport::SetModel(QtWidgetModel* model)
{
  connect(model, &QtWidgetModel::SetProgressReportBegin, this, &QtWidgetSimpleProgressReport::show);
  connect(model, &QtWidgetModel::SetProgressReportDone, this, &QtWidgetSimpleProgressReport::Init);
  connect(this, &QtWidgetSimpleProgressReport::AddNewProcessToReport, this, &QtWidgetSimpleProgressReport::ReportProcess);

  m_AddProcessCommand = AddProcessCommandType::New();
  m_AddProcessCommand->SetCallbackFunction(this, &QtWidgetSimpleProgressReport::ProcessEvent);

  connect(this, &itk::QtProgressBar::SetValueChanged, this, &itk::QtProgressBar::setValue);
  connect(model, &QtWidgetModel::SetProgressReportDone, this, &itk::QtProgressBar::reset);

  model->GetApplication()->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());
}

QtWidgetSimpleProgressReport::~QtWidgetSimpleProgressReport()
{
}

void QtWidgetSimpleProgressReport::ProcessEvent(itk::Object* itkNotUsed(caller), const itk::EventObject& event2)
{
  if (typeid(otb::Wrapper::AddProcessToWatchEvent) == typeid(event2))
  {
    const AddProcessToWatchEvent* eventToWatch = dynamic_cast<const AddProcessToWatchEvent*>(&event2);

    if (eventToWatch)
    {
      m_CurrentProcess     = eventToWatch->GetProcess();
      m_CurrentDescription = eventToWatch->GetProcessDescription();
      Q_EMIT AddNewProcessToReport();
    }
  }
}

void QtWidgetSimpleProgressReport::ReportProcess()
{
  this->Observe(m_CurrentProcess);
  Q_EMIT SetText(QString::fromStdString(m_CurrentDescription));
}

void QtWidgetSimpleProgressReport::Init()
{
  this->setValue(0);
  Q_EMIT SetText(QString(""));
}
}
}
