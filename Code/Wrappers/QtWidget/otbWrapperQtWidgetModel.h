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
    // Prepare the progressbar window 
    QWidget * progWin = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout;  

    std::vector< QProgressBar * > barListIntern, barListWriter;
    std::vector< QLabel * > labelListIntern, labelListWriter;
    if( m_Application->GetInternalProcessList().size() != m_Application->GetInternalProcessListName().size())
      {
        itkGenericExceptionMacro ("Internal process list and list name size mismatch...")
      }

    for(unsigned int ii=0; ii<m_Application->GetInternalProcessList().size(); ii++)
      {
        QLabel *label = new QLabel(QString(m_Application->GetInternalProcessListName()[ii].c_str()));
        QProgressBar * bar = new QProgressBar();
        layout->addWidget(label);
        layout->addWidget(bar);
        barListIntern.push_back(bar);
        labelListIntern.push_back(label);
      }

    unsigned int nbOutput = 0;
    std::vector<std::string> paramList = m_Application->GetParametersKeys(true);
    for (std::vector<std::string>::const_iterator it = paramList.begin();
         it != paramList.end();
         ++it)
      {
        if ( m_Application->GetParameterType(*it) == ParameterType_OutputImage)
          {
            itk::OStringStream oss;
            oss << "Writer "<<nbOutput;
            QLabel *label = new QLabel(QString(oss.str().c_str()));
            QProgressBar * bar = new QProgressBar();
            layout->addWidget(label);
            layout->addWidget(bar);
            barListWriter.push_back(bar);
            labelListWriter.push_back(label);
            nbOutput++;
          }
      }

    progWin->setLayout(layout);
    progWin->show();

    double curWriterProgress = 0;
    unsigned int curWriter = 0;
    while( m_Application->GetExecuteAndWriteOutputDone() == false )
      {
          itk::OStringStream oss;
        oss.str("");

        // Internal DoExecute process watcher
        std::vector<double> progCount = m_Application->GetDoExecuteProgress();
        for(unsigned int i=0; i<progCount.size(); i++)
          {
            //oss<< "doexe "<<i<<": "<<m_Application->GetDoExecuteProgress()[i] <<std::flush;
            barListIntern[i]->setValue( static_cast<int>(progCount[i]*100 ));
            progWin->update();
          }
      
        // Writer watcher
        if(  nbOutput > 0)
          {
            double curProg = m_Application->GetExecuteProgress();
            if( curProg > -1 )
              {
                if( curWriterProgress > curProg )
                  {
                    curWriter++;
                  }
                //std::cout<<"progress writer: "<<curProg<<"."<<std::flush;
                barListWriter[curWriter]->setValue( static_cast<int>(curProg*100) );
                curWriterProgress = curProg;
                progWin->update();
              }
          }

        this->msleep(500);
      }
    progWin->close();
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
