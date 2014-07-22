/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbWrapperQtWidgetModel_h
#define __otbWrapperQtWidgetModel_h

#include <QtGui>
#include "otbWrapperApplication.h"
#include "otbQtLogOutput.h"

namespace otb
{

namespace Wrapper
{

class ITK_ABI_EXPORT AppliThread : public QThread
{
 Q_OBJECT

 public:
  inline AppliThread(Application* app)
    {
      m_Application = app;
    }

  virtual ~AppliThread();

  inline void Execute()
  {
    // Call the signal start to begin running the program
    start();
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
  virtual void run();

private:
  AppliThread(const AppliThread&); //purposely not implemented
  void operator=(const AppliThread&); //purposely not implemented

  Application::Pointer m_Application;
};


/** \class
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetModel : public QObject
{
  Q_OBJECT
public:
  QtWidgetModel(Application* app);

  virtual ~QtWidgetModel();

  Application* GetApplication()
  {
    return m_Application;
  }

  QtLogOutput::Pointer GetLogOutput()
  {
    return m_LogOutput;
  }

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
   * an exception has occured).
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

protected slots:
  /**
   * \brief Slot called when execution is requested (e.g. by
   * otb::Wrapper::QtWidgetView).
   *
   * This slot is protected so it can only be called via Qt
   * signal/slot mechanism and not directly by extern caller.
   */
  void ExecuteAndWriteOutputSlot();

  /**
   * \brief Slots called everytime one of the widget needs to be
   * updated (e.g. by specialized parameter widgets).
   *
   * This slot is protected so it can only be called via Qt
   * signal/slot mechanism and not directly by extern caller.
   */
  void NotifyUpdate();

private slots:
  /**
   * \brief
   *
   * \param status
   */
  void OnApplicationExecutionDone( int status );

private:
  QtWidgetModel(const QtWidgetModel&); //purposely not implemented
  void operator=(const QtWidgetModel&); //purposely not implemented

  Application::Pointer m_Application;

  QtLogOutput::Pointer  m_LogOutput;
};


}
}

#endif
