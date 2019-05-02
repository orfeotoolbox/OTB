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

#ifndef otbWrapperQtWidgetModel_h
#define otbWrapperQtWidgetModel_h

#include <QtWidgets>
#include <QTimer>
#include "otbWrapperApplication.h"
#include "otbQtLogOutput.h"

namespace otb
{

namespace Wrapper
{

class OTBQtWidget_EXPORT AppliThread : public QThread
{
 Q_OBJECT

 public:
   AppliThread(Application::Pointer app) : m_Application(app)
   {
    }

  ~AppliThread() override;

  /** Ask the running application to stop */
  void Stop()
  {
    m_Application->Stop();
  }

signals:
  /**
   * \brief Signal emitted when the OTB-application has finished.
   *
   * \param status 0 The result status of the OTB-application.
   */
  void ApplicationExecutionDone( int status =0 );

  /**
   * \brief Signal emitted when an exception has been raised by an
   * otb::Application and is caught by this calling QThread.
   *
   * \param what The std::exception::what() which is forwarded to listeners.
   */
  void ExceptionRaised( QString what );

protected:
  void run() override;

private:
  AppliThread(const AppliThread&) = delete;
  void operator=(const AppliThread&) = delete;

  Application::Pointer m_Application;
};


/** \class QtWidgetModel
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetModel : public QObject
{
  Q_OBJECT
public:
  QtWidgetModel(Application* app);

  ~QtWidgetModel() override;

  Application* GetApplication()
  {
    return m_Application;
  }

  QtLogOutput::Pointer GetLogOutput()
  {
    return m_LogOutput;
  }

  bool IsRunning() const;

  void Stop();

  /** Logger warning message sender */
  void SendLogWARNING( const std::string & mes );
  /** Logger info message sender */
  void SendLogINFO( const std::string & mes );
  /** Logger debug message sender */
  void SendLogDEBUG( const std::string & mes );

  /** Used by inxml when forcing xml parse flag to update widget data via UpdateGui */
  void UpdateAllWidgets()
  {
    GetApplication()->ForceInXMLParseFlag();
    emit UpdateGui();
  }

signals:
  void SetApplicationReady(bool);

  void SetProgressReportBegin();

  /**
   * \brief Signal emitted when execution otb::Application has finished.
   *
   * \param status The result status of the otb::application (-1 when
   * an exception has occurred).
   */
  void SetProgressReportDone( int status =0 );

  /**
   * \brief Signal emitted when an exception has been raised by an
   * otb::Application and is caught by the calling QThread.
   *
   * \param what The std::exception::what() which is forwarded to listeners.
   */
  void ExceptionRaised( QString what );

  void UpdateGui();

public slots:
  /**
   * \brief Slots called every time one of the widget needs to be
   * updated (e.g. by specialized parameter widgets).
   *
   */
  void NotifyUpdate();

  /**
   * \brief Slot called when execution is requested (e.g. by
   * otb::Wrapper::QtWidgetView).
   */
  void ExecuteAndWriteOutputSlot();

private slots:
  /**
   * \brief
   *
   * \param status
   */
  void OnApplicationExecutionDone( int status );

private:
  QtWidgetModel(const QtWidgetModel&) = delete;
  void operator=(const QtWidgetModel&) = delete;

  QtLogOutput::Pointer  m_LogOutput;

  AppliThread* m_taskAppli;

  bool m_IsRunning;

public:
  Application::Pointer m_Application;
};


}
}

#endif
