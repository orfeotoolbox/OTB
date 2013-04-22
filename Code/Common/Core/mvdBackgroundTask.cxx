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

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractWorker.h"

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
::BackgroundTask( AbstractWorker* worker, QObject* parent ) :
  QThread( parent ),
  m_Worker( worker )
{
  // Change thread affinity and ownership of managed worker.
  assert( m_Worker->parent()==NULL );
  m_Worker->moveToThread( this );
  m_Worker->setParent( this );


  // Start.
  QObject::connect(
    this, SIGNAL( started() ),
    // to:
    worker, SLOT( Do() )
  );

  // Stop.
  QObject::connect(
    worker, SIGNAL( Finished() ),
    // to:
    this, SLOT( quit() )
  );


  // Auto-destroy this background task.
  QObject::connect(
    this, SIGNAL( finished() ),
    // to:
    this, SLOT( deleteLater() )
  );

  // Auto-destroy worker instance.
  QObject::connect(
    this, SIGNAL( finished() ),
    // to:
    worker, SLOT( deleteLater() )
  );
}

/*******************************************************************************/
BackgroundTask
::~BackgroundTask()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
