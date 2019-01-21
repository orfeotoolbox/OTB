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

#ifndef otbWrapperQtWidgetProgressReport_h
#define otbWrapperQtWidgetProgressReport_h

#include <QtWidgets>
#include "otbWrapperQtWidgetModel.h"
#include "itkQtProgressBar.h"
#include "itkProcessObject.h"
#include <string>


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetProgressReport
 * \brief Create a widget reporting the progress of the application
 *        process.
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetProgressReport : public QWidget
{
  Q_OBJECT
public:
  QtWidgetProgressReport(QtWidgetModel * model, QWidget * parent);
  ~QtWidgetProgressReport() override;

  void SetApplication(Application::Pointer app);

  typedef itk::MemberCommand< QtWidgetProgressReport >  AddProcessCommandType;

  void ProcessEvent( itk::Object * caller, const itk::EventObject & event );

  std::string GetCurrentDescription()
  {
    return m_CurrentDescription;
  }

/*
  itk::ProcessObject* GetCurrentProcess()
  {
    return m_CurrentProcess;
  }

  QtWidgetModel * GetModel()
  {
    m_Model;
  }

  QVBoxLayout * GetLayout()
  {
    return m_Layout;
  }
*/
public slots:
  void RemoveLayout();
  /*virtual */void ReportProcess();

  signals:
  void AddNewProcessToReport();

private:
  QtWidgetProgressReport(const QtWidgetProgressReport&) = delete;
  void operator=(const QtWidgetProgressReport&) = delete;

  Application::Pointer              m_Application;
  QtWidgetModel *                   m_Model;
  QVBoxLayout *                     m_Layout;

  AddProcessCommandType::Pointer    m_AddProcessCommand;
  itk::ProcessObject*               m_CurrentProcess;
  std::string                       m_CurrentDescription;
};

}
}

#endif
