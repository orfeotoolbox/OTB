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
    m_Application->ExecuteAndWriteOutput();
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

signals:
  void SetApplicationReady(bool);
  void SetProgressReportBegin();
  void SetProgressReportDone();

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
};



}
}

#endif
