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
#include <QMainWindow>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

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

//
//
protected:

//
//
private:
  Ui::MainWindow* m_UI;

//
// SLOTS
private slots:
  void on_action_Open_activated();
};

} // end namespace 'mvd'

#endif // __MainWindow_h
