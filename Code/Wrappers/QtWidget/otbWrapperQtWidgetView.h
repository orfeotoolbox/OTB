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
#ifndef __otbWrapperQtWidgetView_h
#define __otbWrapperQtWidgetView_h

#include <QtGui>
#include "otbWrapperApplication.h"
#include "otbWrapperQtWidgetModel.h"
//#include "otbWrapperEventsListener.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
  class QtWidgetView : public QObject /* QWidget*/ /*, public EventsListener<std::string>*/
{
  Q_OBJECT
public:
  QtWidgetView(Application* app);
  virtual ~QtWidgetView();

  void CreateGui();

  QWidget * GetMainWindow()
  {
    return m_MainWindow;
  }

public slots:
  void ExecuteAndWriteOutputSlot();

private:
  QtWidgetView(const QtWidgetView&); //purposely not implemented
  void operator=(const QtWidgetView&); //purposely not implemented

  QWidget* CreateHeader();

  QWidget* CreateFooter();

  QWidget* CreateInputWidgets();

  Application::Pointer m_Application;

  QtWidgetModel* m_Model;

  QWidget * m_MainWindow;

  QPushButton* m_ExecButton;
  QPushButton* m_QuitButton;
  QWidget * m_ProgressWindow;
};


}
}

#endif
