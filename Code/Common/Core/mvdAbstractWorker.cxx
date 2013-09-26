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
#include "mvdAbstractWorker.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdI18nCoreApplication.h"

namespace mvd
{
/*
  TRANSLATOR mvd::AbstractWorker

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
AbstractWorker
::AbstractWorker( QObject* parent ) :
  QObject( parent )
{
}

/*******************************************************************************/
AbstractWorker
::~AbstractWorker()
{
}

/*****************************************************************************/
void
AbstractWorker
::Do() throw()
{
  QObject* result = NULL;

  try
    {
    // Just do it and get resulting QObject.
    result = virtual_Do();

    // Access application.
    const QCoreApplication* app = QCoreApplication::instance();
    assert( app!=NULL );

    // Move object into application's main thread.
    //
    // We can only push to another thread,
    // so thread affinity must be set here,
    // and not in the slot that receives the object.
    if( result->thread()!=app->thread() )
      result->moveToThread( app->thread() );

    // Emit task/job has been correctly done.
    emit Done( result );
    }
  catch( std::exception& exc )
    {
    // Delete allocated object.
    delete result;
    result = NULL;

    // Emit clone of caught exception.
    emit ExceptionRaised( FromStdString( exc.what() ) );
    }

  // Emit task/job has finished.
  emit Finished();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
