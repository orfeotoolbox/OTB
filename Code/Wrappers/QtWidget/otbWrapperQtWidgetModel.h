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

class AppliThread : public QThread
{
 Q_OBJECT
 public:
  AppliThread(Application* app)
    {
      m_Application = app;
    }

  ~AppliThread()
  {
    wait();
  }

  void Execute()
  {
    // Call the signal start to begin running the program
    start();
  }

signals:
  void ApplicationExecutionDone();

protected:
  virtual void run()
  {
    try
      {
      m_Application->ExecuteAndWriteOutput();
      }
    catch(std::exception& err)
      {
      std::ostringstream message;
      message << "The following error occurred during application execution : " << err.what() << std::endl;
      m_Application->GetLogger()->Write( itk::LoggerBase::FATAL, message.str() );
      }
    catch(...)
      {
      m_Application->GetLogger()->Write( itk::LoggerBase::FATAL, "An unknown exception has been raised during application execution" );
      }
    
    emit ApplicationExecutionDone();
  }

private:
  AppliThread(const AppliThread&); //purposely not implemented
  void operator=(const AppliThread&); //purposely not implemented

  Application::Pointer m_Application;
};


/** \class
 * \brief
 */
class QtWidgetModel : public QObject
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
  void SetProgressReportDone();
  void UpdateGui();

protected slots:
  // slot called everytime one of the widget is updated
  void NotifyUpdate();
  // slot called when execution is requested
  void ExecuteAndWriteOutputSlot();
  // slot called to activate the Execute button after the application
  // is done
  void ActivateExecuteButton();

private:
  QtWidgetModel(const QtWidgetModel&); //purposely not implemented
  void operator=(const QtWidgetModel&); //purposely not implemented

  Application::Pointer m_Application;

  QtLogOutput::Pointer  m_LogOutput;
};


}
}

#endif
