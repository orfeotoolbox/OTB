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

#ifndef otbWrapperQtWidgetSimpleProgressReport_h
#define otbWrapperQtWidgetSimpleProgressReport_h

#include <QtWidgets>
#include "otbWrapperQtWidgetModel.h"
#include "itkProcessObject.h"
#include "itkQtProgressBar.h"
#include <string>


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetSimpleProgressReport
 * \brief Create a widget reporting the progress of the application
 *        process.
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetSimpleProgressReport : public itk::QtProgressBar
{
  Q_OBJECT
public:
  QtWidgetSimpleProgressReport(QWidget* parent);
  ~QtWidgetSimpleProgressReport() override;

  void SetModel(QtWidgetModel* model);

  typedef itk::MemberCommand<QtWidgetSimpleProgressReport> AddProcessCommandType;

  void ProcessEvent(itk::Object* caller, const itk::EventObject& event);

public slots:
  void Init();
  void ReportProcess();

signals:
  void AddNewProcessToReport();
  void SetText(QString);

private:
  QtWidgetSimpleProgressReport(const QtWidgetSimpleProgressReport&) = delete;
  void operator=(const QtWidgetSimpleProgressReport&) = delete;

  AddProcessCommandType::Pointer m_AddProcessCommand;
  itk::ProcessObject*            m_CurrentProcess;
  std::string                    m_CurrentDescription;
};
}
}

#endif
