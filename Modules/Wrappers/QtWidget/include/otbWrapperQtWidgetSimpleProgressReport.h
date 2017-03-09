/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperQtWidgetModel.h"
#include "itkProcessObject.h"
#include "itkQtProgressBar.h"
#endif //tag=QT4-boost-compatibility


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
class OTBQtWidget_EXPORT QtWidgetSimpleProgressReport : public QWidget
{
  Q_OBJECT
public:
  QtWidgetSimpleProgressReport(QtWidgetModel * model);
  ~QtWidgetSimpleProgressReport() ITK_OVERRIDE;

  void SetApplication(Application::Pointer app);

  typedef itk::MemberCommand< QtWidgetSimpleProgressReport >  AddProcessCommandType;

  void ProcessEvent( itk::Object * caller, const itk::EventObject & event );

public slots:
  void Init();
  void ReportProcess();

  signals:
  void AddNewProcessToReport();

private:
  QtWidgetSimpleProgressReport(const QtWidgetSimpleProgressReport&); //purposely not implemented
  void operator=(const QtWidgetSimpleProgressReport&); //purposely not
                                                       //implemented

  Application::Pointer              m_Application;
  QtWidgetModel *                   m_Model;
  QVBoxLayout *                     m_Layout;

  AddProcessCommandType::Pointer    m_AddProcessCommand;
  itk::ProcessObject*               m_CurrentProcess;
  std::string                       m_CurrentDescription;

  itk::QtProgressBar * m_Bar;
  QLabel * m_Label;

};

}
}

#endif
