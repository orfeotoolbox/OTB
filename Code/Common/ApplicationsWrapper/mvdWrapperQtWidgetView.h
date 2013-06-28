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
#ifndef __mvdWrapperQtWidgetView_h
#define __mvdWrapperQtWidgetView_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbWrapperApplication.h"
#include "otbWrapperQtWidgetModel.h"

//
// Monteverdi includes (sorted by alphabetic order)


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

namespace Wrapper
{

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class ApplicationLauncher
 *
 * \brief WIP.
 */

class QtWidgetView :
    public QWidget
{
  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/
  
  Q_OBJECT

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/
  
//
// Public methods.
public:

  /** \brief Constructor. */
  QtWidgetView( otb::Wrapper::Application::Pointer otbApp,
		QWidget* parent =0,
		Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~QtWidgetView();

  /** \brief Gui Creation. */
  void CreateGui();

  /** \brief Model Accessor */
  otb::Wrapper::QtWidgetModel* GetModel()
  {
    return m_Model;
  }

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  // slot in charge of emiting a signal to the catalogue mainWindow.
  // when received, the main application need to get the output
  // image filename{s} set by the user in this OTB application (if any).
  void OnApplicationExecutionDone();

  // application closed
  void CloseSlot();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  void QuitSignal();
  
  void OTBApplicationOutputImageChanged( const QString &, const QString &);

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  QtWidgetView(const QtWidgetView&); //purposely not implemented
  void operator=(const QtWidgetView&); //purposely not implemented

  QWidget* CreateFooter();

  QWidget* CreateInputWidgets();

  QWidget* CreateDoc();

  // fill the default output image parameter with default
  // value
  void FillOTBAppDefaultOutputImageParameter(QWidget * widgets);

  // method to generate an unique identifier
  const QString GenerateIdentifier();

//
// Private attributes.

private:

  otb::Wrapper::Application::Pointer m_Application;
  otb::Wrapper::QtWidgetModel* m_Model;

  QPushButton* m_ExecButton;
  QPushButton* m_QuitButton;
  QLabel*      m_Message;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  void UpdateMessageAfterExcuteClicked();
  void UpdateMessageAfterApplicationReady(bool val);
};


}
}

#endif
