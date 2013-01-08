/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdMainWindow_h
#define __mvdMainWindow_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"

//
// Some constants.
#define VIDEO_COLOR_SETUP_DOCK "videoColorSetupDock"

//
// External class pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal class pre-declaration.
class AbstractModel;

namespace Ui
{
class MainWindow;
}

/** \class MainWindow
 *
 */
class Monteverdi2_EXPORT MainWindow
  : public QMainWindow
{
  Q_OBJECT;

//
// Public types.
public:

//
// Public methods.
public:
  /** Constructor */
  MainWindow( QWidget* Parent =0, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~MainWindow();

//
// SIGNALS.
signals:
  void largestPossibleRegionChanged(const ImageRegionType& largestRegion);

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

//
// Private SLOTS.
private:
  /** */
  void Initialize();

  /** */
  void InitializeDockWidgets();

  /** */
  inline
    const QDockWidget*
    GetVideoColorSetupDock() const
  {
    return const_cast< const MainWindow* >( this )->GetVideoColorSetupDock();
  }

  /** */
  inline
    QDockWidget*
    GetVideoColorSetupDock()
  {
    return findChild< QDockWidget* >( VIDEO_COLOR_SETUP_DOCK );  
  }

//
// Private methods.
private:
  /**
   */
  Ui::MainWindow* m_UI;

//
// Private attributes.
private slots:
  void on_action_Open_activated();
  void on_action_About_activated();

  void onSelectedModelChanged( const AbstractModel* );
};

} // end namespace 'mvd'

#endif // __MainWindow_h
