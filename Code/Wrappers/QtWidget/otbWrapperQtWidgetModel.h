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
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperEventsSender.h"
#include "otbWrapperEvent.h"

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

  virtual ~AppliThread(){};
  void run()
  {
    m_Application->ExecuteAndWriteOutput();
 }

private:
  AppliThread(const AppliThread&); //purposely not implemented
  void operator=(const AppliThread&); //purposely not implemented

  Application::Pointer m_Application;
  QWidget * m_ProgressWindow;
};


/** \class
 * \brief
 */
class QtWidgetModel : public QObject, public EventsSender<std::string>
{
  Q_OBJECT
public:
  QtWidgetModel(Application* app);

  virtual ~QtWidgetModel();

  Application* GetApplication()
  {
    return m_Application;
  }

  // slot called when execution is requested
  void ExecuteAndWriteOutput();

protected slots:
  // slot called everytime one of the widget is updated
  void NotifyUpdate();

private:
  QtWidgetModel(const QtWidgetModel&); //purposely not implemented
  void operator=(const QtWidgetModel&); //purposely not implemented

  Application::Pointer m_Application;
  QWidget * m_ProgressWindow;
};



}
}

#endif
