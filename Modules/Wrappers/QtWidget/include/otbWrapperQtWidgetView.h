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

#ifndef otbWrapperQtWidgetView_h
#define otbWrapperQtWidgetView_h

#include <QtWidgets>
#include <QObject>
#include <QShortcut>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperQtWidgetModel.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetView
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetView : public QWidget
{
  Q_OBJECT
public:
  QtWidgetView(Application* app);
  ~QtWidgetView() ITK_OVERRIDE;

  void CreateGui();

  QtWidgetModel* GetModel()
  {
    return m_Model;
  }

public slots:
  void CloseSlot();
  void UnhandledException(QString message);
  void OnExceptionRaised(QString message);

private slots:
  void UpdateMessageAfterExecuteClicked();
  void UpdateMessageAfterExecution(int status);
  void UpdateMessageAfterApplicationReady(bool val);

signals:
  void QuitSignal();


private:
  QtWidgetView(const QtWidgetView&); //purposely not implemented
  void operator=(const QtWidgetView&); //purposely not implemented

  QWidget* CreateFooter();

  QWidget* CreateInputWidgets();

  QWidget* CreateDoc();

  Application::Pointer m_Application;

  QtWidgetModel* m_Model;

  QTextEdit *m_LogText;
  QTabWidget *m_TabWidget;

  QPushButton* m_ExecButton;
  QPushButton* m_QuitButton;
  QShortcut* m_QuitShortcut;
  QLabel* m_Message;
};


}
}

#endif
