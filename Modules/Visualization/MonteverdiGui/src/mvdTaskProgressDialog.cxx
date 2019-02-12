/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
#include "mvdAbstractWorker.h"
#include "mvdBackgroundTask.h"

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
		      QWidget* p,
		      Qt::WindowFlags flags ):
  QProgressDialog( p, flags ),
  m_BackgroundTask( task ),
  m_Object( NULL ),
  m_Exception()
{
  // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
  //
  // Do not take ownership of background-task to leave display
  // independent of execution of background-tas.
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
  // qDebug() << this << "is being destroyed.";


  // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
  //
  // Trace (for debugging purposes) if background-task thread is still
  // running when destroying this dialog.
#if ( defined( OTB_DEBUG ) && 1 ) || 0
  if( m_BackgroundTask!=NULL &&
      m_BackgroundTask->isRunning() )
    {
    qDebug() << m_BackgroundTask << "is still running!";
    }
#endif

  // qDebug() << this << "has been destroyed.";
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
::OnDone( QObject* res )
{
  m_Object = res;

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
    this->parentWidget(),
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
  // qDebug() << this << "::OnObjectDestryed(" << object << ")";

  assert( object==m_BackgroundTask );

  if( object==m_BackgroundTask )
    {
    // qDebug() << this << "forgetting" << m_BackgroundTask;

    // Forget background-task.
    m_BackgroundTask = NULL;

    // Accept QDialog te prevent locking the UI.
    accept();
    }
}

} // end namespace 'mvd'
