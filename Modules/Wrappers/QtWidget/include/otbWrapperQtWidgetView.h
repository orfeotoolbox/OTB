/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperQtWidgetView_h
#define otbWrapperQtWidgetView_h

#include <QtWidgets>
#include "otbWrapperApplication.h"
#include "otbWrapperQtWidgetModel.h"
#include "OTBQtWidgetExport.h"

namespace otb
{
namespace Wrapper
{

/**
 * \class QtWidgetView
 *
 * \ingroup OTBQtWidget
 *
 * \brief WIP.
 */
class OTBQtWidget_EXPORT QtWidgetView :
    public QWidget
{
  Q_OBJECT

  Q_PROPERTY( bool isClosable
	      READ IsClosable
	      WRITE SetClosable );

public:

  static char const * const OBJECT_NAME;

  /** \brief Constructor. */
  QtWidgetView( const otb::Wrapper::Application::Pointer & otbApp,
		QWidget* parent,
		Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  ~QtWidgetView() override;

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
  //~ otb::Wrapper::Application::ConstPointer GetApplication() const
  //~ {
    //~ return otb::ConstCast< otb::Wrapper::Application >(
      //~ m_Application
    //~ );
  //~ }

  inline bool IsClosable() const;

public slots:

  void UnhandledException(QString message);

signals:
  void QuitSignal();
  void ExecuteAndWriteOutput();
  void Stop();

protected:

  bool IsRunning();

  virtual QWidget* CreateInputWidgets();

  // QWidget overloads.
  void closeEvent( QCloseEvent * event ) override;

protected:

  /** Html section for 'Done' icon */
  std::string m_IconPathDone;

  /** Html section for 'Failed' icon */
  std::string m_IconPathFailed;

protected slots:

  void OnExecButtonClicked();

  void OnExceptionRaised( QString what );

// Private methods.
private:

  QtWidgetView(const QtWidgetView&) = delete;
  void operator=(const QtWidgetView&) = delete;

  QWidget* CreateFooter();

  QWidget* CreateDoc();


// Private attributes.
private:

  otb::Wrapper::QtWidgetModel* m_Model;

  QPushButton* m_ExecButton;
  QPushButton* m_QuitButton;
  QShortcut* m_QuitShortcut;
  QLabel* m_Message;
  QTextEdit *m_LogText;
  QTabWidget *m_TabWidget;

  bool m_IsClosable : 1;

  bool m_IsRunning;

private slots:
  void UpdateMessageAfterExecution(int status);
  void UpdateMessageAfterApplicationReady(bool val);

  /**
   */
  inline void OnProgressReportBegin();

  /**
   */
  inline void OnProgressReportEnd( int status );

  /**
   */
  inline void SetClosable( bool );

};

} // end namespace 'Wrapper'

} // end namespace 'otb'

/*****************************************************************************/
/* INLINE SECTION                                                            */

namespace otb
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
::OnProgressReportEnd( int )
{
  SetClosable( true );
}

} // end namespace 'Wrapper'

} // end namespace 'otb'

#endif
