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
#include <QtGui>

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
#include "Core/mvdTypes.h"

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

class Monteverdi2_EXPORT QtWidgetView :
    public QWidget
{
  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/
  
  Q_OBJECT

  Q_PROPERTY( bool isClosable
	      READ IsClosable
	      WRITE SetClosable );

  Q_PROPERTY( bool isStandalone
              READ IsStandalone );

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  QtWidgetView( otb::Wrapper::Application::Pointer otbApp,
                bool isStandalone,
		QWidget* parent =0,
		Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~QtWidgetView();

  /** \brief Gui Creation. */
  void CreateGui();

  /** \brief Model Accessor */
  inline otb::Wrapper::QtWidgetModel* GetModel()
  {
    return m_Model;
  }

  /**
   * \return The OTB-application pointer of this view.
   */
  otb::Wrapper::Application::ConstPointer GetApplication() const
  {
    return otb::ConstCast< otb::Wrapper::Application >(
      m_Application
    );
  }

  /**
   */
  inline bool IsClosable() const;

  /**
   */
  bool IsStandalone() const
  {
    return m_IsStandalone;
  }

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  // slot in charge of emiting a signal to the catalogue mainWindow.
  // when received, the main application need to get the output
  // image filename{s} set by the user in this OTB application (if any).
  void OnApplicationExecutionDone( int );

  // application closed
  void CloseSlot();

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  void QuitSignal();
  
  void OTBApplicationOutputImageChanged( const QString &, const QString &);

  void ExecuteAndWriteOutput();

  void ExecutionDone( int nbOutputs );

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private methods.
private:

  QtWidgetView(const QtWidgetView&); //purposely not implemented
  void operator=(const QtWidgetView&); //purposely not implemented

  QWidget* CreateFooter();

  QWidget* CreateInputWidgets();

  QWidget* CreateDoc();

  /**
   */
  void SetupParameterWidgets( QWidget* widget );

  /**
   */
  void SetupFileSelectionWidget( QWidget *, bool supportsDataset );

//
// Private attributes.

private:

  otb::Wrapper::Application::Pointer m_Application;
  otb::Wrapper::QtWidgetModel* m_Model;

  QPushButton* m_ExecButton;
  QPushButton* m_QuitButton;
  QLabel* m_Message;

  bool m_IsClosable : 1;
  bool m_IsStandalone : 1;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:

  void UpdateMessageAfterApplicationReady(bool val);

  /**
   */
  void OnExecButtonClicked();

  /**
   */
  inline void OnProgressReportBegin();

  /**
   */
  inline void OnProgressReportEnd( int status );

  /**
   */
  void OnExceptionRaised( QString what );

  /**
   */
  inline void SetClosable( bool );

  /**
   */
  inline void OnFileSelectionWidgetAdded0( QWidget * );

  /**
   */
  inline void OnFileSelectionWidgetAdded1( QWidget * );
};

} // end namespace 'Wrapper'

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace mvd
{

namespace Wrapper
{

/*****************************************************************************/
inline
bool
QtWidgetView
::IsClosable() const
{
  return m_IsClosable;
}

/*****************************************************************************/
inline
void
QtWidgetView
::SetClosable( bool enabled )
{
  m_IsClosable = enabled;

  setEnabled( true );

  if( m_QuitButton!=NULL )
    m_QuitButton->setEnabled( m_IsClosable );
}

/*******************************************************************************/
inline
void
QtWidgetView
::OnProgressReportBegin()
{
  SetClosable( false );
}

/*******************************************************************************/
inline
void
QtWidgetView
::OnProgressReportEnd( int status )
{
  SetClosable( true );
}

/*******************************************************************************/
inline
void
QtWidgetView
::OnFileSelectionWidgetAdded0( QWidget * widget )
{
  SetupFileSelectionWidget( widget, false );
}

/*******************************************************************************/
inline
void
QtWidgetView
::OnFileSelectionWidgetAdded1( QWidget * widget )
{
  SetupFileSelectionWidget( widget, true );
}

} // end namespace 'Wrapper'

} // end namespace 'mvd'

#endif
