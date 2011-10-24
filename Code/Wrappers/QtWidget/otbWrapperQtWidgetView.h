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
#include <QObject>
#include "otbWrapperApplication.h"
#include "otbWrapperQtWidgetModel.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetView : public QWidget
{
  Q_OBJECT
public:
  QtWidgetView(Application* app);
  virtual ~QtWidgetView();

  void CreateGui();

  QtWidgetModel* GetModel()
  {
    return m_Model;
  }

public slots:
  void CloseSlot();

signals:
  void QuitSignal();
  

private:
  QtWidgetView(const QtWidgetView&); //purposely not implemented
  void operator=(const QtWidgetView&); //purposely not implemented

  QWidget* CreateFooter();

  QWidget* CreateInputWidgets();

  QWidget* CreateDoc();

  /** Add the parameter description in the flux of the documentation tab. */
  void SetDocParameters( std::string & val );

  /** generate the documentation associated to a group.*/
  void GetDocParameterGroup( std::string & val, const std::string & key);

  Application::Pointer m_Application;

  QtWidgetModel* m_Model;

  QPushButton* m_ExecButton;
  QPushButton* m_QuitButton;
};


}
}

#endif
