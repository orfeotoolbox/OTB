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
#include "mvdBackgroundTask.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include <cassert>
#include <stdexcept>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractWorker.h"
#include "Core/mvdAlgorithm.h"

namespace mvd
{
/*
  TRANSLATOR mvd::BackgroundTask

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
} // end of anonymous namespace.

/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
BackgroundTask
::BackgroundTask( AbstractWorker* worker, bool autoDestroy, QObject* parent ) :
  QThread( parent ),
  m_Worker( worker )
{
  // Check worker argument.
  assert( m_Worker!=NULL );
  assert( m_Worker->parent()==NULL );

  if( m_Worker->parent()!=NULL )
    throw std::invalid_argument(
      ToStdString(
        tr(
          "Worker must not be parented in order to be moved to another thread."
        )
      )
    );

  // Change thread affinity and take ownership of managed worker.
  m_Worker->moveToThread( this );

  // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
  //
  // Do not re-parent worker to background-task thread here since it
  // will generate a 'WARNG> QObject::setParent: Cannot set parent,
  // new parent is in a different thread' warning message from Qt.
  //
  // m_Worker->setParent( this );

  // When thread is started, start doing worker job.
  QObject::connect(
    this, SIGNAL( started() ),
    // to:
    worker, SLOT( Do() )
  );

  // When worker has finished its job, quit thread.
  QObject::connect(
    worker, SIGNAL( Finished() ),
    // to:
    this, SLOT( quit() )
  );

  // Keep informed When some object is destroyed.
  QObject::connect(
    worker,
    SIGNAL( destroyed( QObject* ) ),
    // to:
    this,
    SLOT( OnObjectDestroyed( QObject* ) )
  );

  // Conditionaly auto-destroy this background task.
  if( autoDestroy )
    {
    // When thread has finished.
    QObject::connect(
      this, SIGNAL( finished() ),
      // to:
      this, SLOT( deleteLater() )
    );

    // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
    //
    // NEVER deleteLater() (i.e. asynchronously) worker instance.
    //
    // QObject::connect(
    //   this, SIGNAL( finished() ),
    //   // to:
    //   worker, SLOT( deleteLater() )
    // );
    }
}

/*******************************************************************************/
BackgroundTask
::~BackgroundTask()
{
  // Trace.
  qDebug() << this << "is being detroyed.";

  // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
  //
  // Warns if this background-task thread is still running while
  // destroying instance.
  //
  // Thread cannot be forced to quit() & wait() nor terminate()
  // because it would emit signal while execution of instance
  // destructor. This could be DANGEROUS.
  assert( isFinished() );

  if( isRunning() )
    qWarning() << this << "is being destroyed while still running!";

  // MANTIS-921 (http://bugs.orfeo-toolbox.org/view.php?id=921).
  //
  // Synchronously delete owned worker instance when this
  // background-task is being destroyed.
  delete m_Worker;
  m_Worker = NULL;

  // Trace.
  qDebug() << this << "has been destroyed.";
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
BackgroundTask
::OnObjectDestroyed( QObject* object )
{
  // NOTE: Won't be called if deleteLater() is used!?

  qDebug() << this << "::OnObjectDestryed(" << object << ")";

  //
  // If worker instance is destroyed before this instance, simply
  // forget worker instance.
  assert( object==m_Worker );

  if( object==m_Worker )
    {
    qDebug() << this << "forgetting" << m_Worker;

    m_Worker = NULL;
    }
}

} // end namespace 'mvd'
