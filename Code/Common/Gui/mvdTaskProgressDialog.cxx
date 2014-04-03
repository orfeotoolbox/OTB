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
#include "mvdTaskProgressDialog.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractWorker.h"
#include "Core/mvdBackgroundTask.h"

namespace mvd
{

/*
  TRANSLATOR mvd::TaskProgressDialog

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
TaskProgressDialog
::TaskProgressDialog( BackgroundTask* task,
		      QWidget* parent,
		      Qt::WindowFlags flags ):
  QProgressDialog( parent, flags ),
  m_BackgroundTask( task ),
  m_Object( NULL ),
  m_Exception()
{
  // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
  //
  // Do not take ownership of background-task to leave display
  // independant of execution of background-tas.
  //
  // m_BackgroundTask->setParent( this );

  QObject::connect(
    task->GetWorker(), SIGNAL( ProgressTextChanged( const QString& ) ),
    // to:
    this, SLOT( setLabelText( const QString& ) )
  );

  QObject::connect(
    task->GetWorker(), SIGNAL( ProgressValueChanged( int ) ),
    // to:
    this, SLOT( setValue( int ) )
  );

  QObject::connect(
    task->GetWorker(), SIGNAL( ProgressRangeChanged( int, int ) ),
    // to:
    this, SLOT( setRange( int, int ) )
  );

  QObject::connect(
    task->GetWorker(),
    SIGNAL( Done( QObject* ) ),
    // to:
    this,
    SLOT( OnDone( QObject* ) )
  );

  QObject::connect(
    task->GetWorker(),
    SIGNAL( ExceptionRaised( QString ) ),
    // to:
    this,
    SLOT( OnExceptionRaised( QString ) )
  );

  // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
  //
  // Accept progress-dialog when thread has been signaled as
  // finished() and not when worker object has done or finished it's
  // job (while the thread is still running when Done() or Finished()
  // is signalled).
  QObject::connect(
    task,
    SIGNAL( finished() ),
    // to:
    this,
    SLOT( accept() )
  );

  // Keep informed When some object is destroyed.
  QObject::connect(
    m_BackgroundTask,
    SIGNAL( destroyed( QObject* ) ),
    // to:
    this,
    SLOT( OnObjectDestroyed( QObject* ) )
  );
}

/*******************************************************************************/
TaskProgressDialog
::~TaskProgressDialog()
{
  qDebug() << this << "is being destroyed.";


  // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
  //
  // Trace (for debugging purposes) if background-task thread is still
  // running when destroying this dialog.
#if ( defined( _DEBUG ) && 1 ) || 0
  if( m_BackgroundTask!=NULL &&
      m_BackgroundTask->isRunning() )
    {
    qDebug() << m_BackgroundTask << "is still running!";
    }
#endif

  qDebug() << this << "has been destroyed.";
}

/*****************************************************************************/
int
TaskProgressDialog
::Exec()
{
  m_BackgroundTask->start();

  setLabelText( m_BackgroundTask->GetWorker()->GetFirstProgressText() );

  return exec();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
TaskProgressDialog
::OnDone( QObject* result )
{
  m_Object = result;

  // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
  //
  // Accept is slotted to QThread finished() signal for correct thread
  // synchronization.
  //
  // accept();
}

/*******************************************************************************/
void
TaskProgressDialog
::OnExceptionRaised( QString what )
{
  QMessageBox::warning(
    this,
    tr( "%1 - Warning!" ).arg( PROJECT_NAME ),
    what
  );

  reject();
}

/*******************************************************************************/
void
TaskProgressDialog
::OnObjectDestroyed( QObject* object )
{
  qDebug() << this << "::OnObjectDestryed(" << object << ")";

  assert( object==m_BackgroundTask );

  if( object==m_BackgroundTask )
    {
    qDebug() << this << "forgetting" << m_BackgroundTask;

    // Forget background-task.
    m_BackgroundTask = NULL;

    // Accept QDialog te prevent locking the UI.
    accept();
    }
}

} // end namespace 'mvd'
