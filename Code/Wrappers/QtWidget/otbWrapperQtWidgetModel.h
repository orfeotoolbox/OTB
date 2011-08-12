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
//#include "otbWrapperEventsListener.h"

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
    std::cout<<"ruuuuuuuuuun done"<<std::endl;
  }

private:
  AppliThread(const AppliThread&); //purposely not implemented
  void operator=(const AppliThread&); //purposely not implemented

  Application::Pointer m_Application;

};

class WatchThread : public QThread
{
 Q_OBJECT
 public:
  WatchThread(Application* app)
    {
      m_Application = app;
    }
virtual ~WatchThread(){};
  void run()
  {
    while( m_Application->GetExecuteAndWriteOutputDone() == false )
      {
        itk::OStringStream oss;
        oss.str("");

        // Internal DoExecute process watcher
        std::vector<double> progCount = m_Application->GetDoExecuteProgress();
        for(unsigned int i=0; i<progCount.size(); i++)
          {
            oss<< "doexe "<<i<<": "<<m_Application->GetDoExecuteProgress()[i] <<"\n";
          }

        // Writer watcher
        double curProg = m_Application->GetExecuteProgress();
        if( curProg > -1 )
          {
            oss<< "exe "<<curProg<<"  ";
          }

        std::cout<<oss.str()<<std::flush;
        this->msleep(500);
      }
    
   std::cout<<"waaaaaaatch done"<<std::endl;
  }

private:
  WatchThread(const WatchThread&); //purposely not implemented
  void operator=(const WatchThread&); //purposely not implemented

  Application::Pointer m_Application;

};


/** \class
 * \brief
 */
  class QtWidgetModel : public QObject/*, public EventsListener<std::string>*/
{
  Q_OBJECT
public:
  QtWidgetModel(Application* app);

  virtual ~QtWidgetModel();

  Application* GetApplication()
  {
    return m_Application;
  }

protected slots:
  // slot called everytime one of the widget is updated
  void NotifyUpdate();

  // slot called when execution is requested
  void ExecuteAndWriteOutput();

  //void Notify();

private:
  QtWidgetModel(const QtWidgetModel&); //purposely not implemented
  void operator=(const QtWidgetModel&); //purposely not implemented

  Application::Pointer m_Application;
};


}
}

#endif
