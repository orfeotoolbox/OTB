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
#ifndef otbWrapperQtWidgetView_h
#define otbWrapperQtWidgetView_h

#include <QtGui>
#include <QObject>
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

private slots:
  void UpdateMessageAfterExecuteClicked();
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
  QLabel* m_Message;
};


}
}

#endif
