/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdMainWindow_h
#define __mvdMainWindow_h

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)
#include <QtGui>

//
// OTB includes (sorted by alphabetic order)
#include "otbQGLImageWidget.h"

//
// Monteverdi includes (sorted by alphabetic order)

 namespace otb 
 {
 class QGLImageWidget;
 }

//
// Class pre-declaration.
namespace Ui
{
class MainWindow;
}





namespace mvd
{
/** \class MainWindow
 *
 */
class MainWindow
  : public QMainWindow
{
  Q_OBJECT;

//
//
public:

/** Constructor */
  MainWindow( QWidget* Parent =0, Qt::WindowFlags flags =0 );

/** Destructor */
  virtual ~MainWindow();

  /** Method to get the instance*/
  otb::QGLImageWidget* GetGLImageWidget()
  {
    return m_QGLWidget;
  }

//
//
protected:

//
//
private:
  Ui::MainWindow* m_UI;

  otb::QGLImageWidget * m_QGLWidget;

//
// SLOTS
private slots:
  void on_action_Open_activated();
};

} // end namespace 'mvd'

#endif // __MainWindow_h
