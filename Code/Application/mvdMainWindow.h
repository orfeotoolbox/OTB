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

public:

  /** Constructor */
  MainWindow( QWidget* Parent =0, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~MainWindow();

  /** Method to get the instance*/
  inline otb::QGLImageWidget* GetGLImageWidget()
  {
    return dynamic_cast< ::otb::QGLImageWidget* >( centralWidget() );
  }

  /** Method to get the instance*/
  inline const otb::QGLImageWidget* GetGLImageWidget() const
  {
    return const_cast< const MainWindow* >( this )->GetGLImageWidget();
  }

protected:

private:
  void Initialize();

  Ui::MainWindow* m_UI;

private slots:
  void on_action_Open_activated();
};

} // end namespace 'mvd'

#endif // __MainWindow_h
