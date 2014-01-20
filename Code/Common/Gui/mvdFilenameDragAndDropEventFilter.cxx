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
#include "mvdFilenameDragAndDropEventFilter.h"


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

namespace mvd
{

/*
  TRANSLATOR mvd::FilenameDragAndDropEventFilter

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*****************************************************************************/
FilenameDragAndDropEventFilter
::FilenameDragAndDropEventFilter( QObject* parent  ) :
  AbstractDragAndDropEventFilter( parent )
{
}

/*****************************************************************************/
FilenameDragAndDropEventFilter
::~FilenameDragAndDropEventFilter()
{
}

/*****************************************************************************/
bool
FilenameDragAndDropEventFilter
::DragEnterEvent( QObject* object, QDragEnterEvent* event )
{
  //
  // Bypass event its MIME data does not contain not URL(s).
  if( !event->mimeData()->hasUrls() )
    return false;

  //
  // Bypass event if MIME data URL(s) are not all local filenames.
  typedef QList< QUrl > QUrlList;

  QUrlList urls( event->mimeData()->urls() );

  for( QUrlList::const_iterator it( urls.begin() );
       it!=urls.end();
       ++it )
    {
#if QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
    if( !it->scheme().compare( "file", Qt::CaseInsensitive ) )
#else // QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
    if( !it->isLocalFile() )
#endif  // QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
      {
      return false;
      }
    }

  //
  // Accept event if its MIME data contains some URL(s) and they are
  // all local filenames.
  event->acceptProposedAction();

  //
  // Eatup event.
  return true;
}

/*****************************************************************************/
bool
FilenameDragAndDropEventFilter
::DragLeaveEvent( QObject* object, QDragLeaveEvent* event )
{
  //
  // Nothing to do: bypass event & let default behaviour occur.
  return false;
}

/*****************************************************************************/
bool
FilenameDragAndDropEventFilter
::DragMoveEvent( QObject* object, QDragMoveEvent* event )
{
  //
  // Nothing to do: bypass event & let default behaviour occur.
  return false;
}

/*****************************************************************************/
bool
FilenameDragAndDropEventFilter
::DropEvent( QObject* object, QDropEvent* event )
{
  assert( event!=NULL );
  assert( event->mimeData()!=NULL );

  //
  // Bypass event its MIME data does not contain not URL(s).
  if( !event->mimeData()->hasUrls() )
    return false;

  //
  // Bypass event if MIME data URL(s) are not all local filenames.
  typedef QList< QUrl > QUrlList;

  QUrlList urls( event->mimeData()->urls() );

  for( QUrlList::const_iterator it( urls.begin() );
       it!=urls.end();
       ++it )
    {
#if QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
    if( !it->scheme().compare( "file", Qt::CaseInsensitive ) )
#else // QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
    if( !it->isLocalFile() )
#endif  // QT_VERSION < QT_VERSION_CHECK( 4, 8, 0 )
      {
      qWarning() << "Dropped URL is not a local filename." << *it;
      }
    else
      {
      emit FilenameDropped( it->toLocalFile() );
      }
    }
  
  //
  // Eatup event.
  return true;
}

/*****************************************************************************/
/* SLOTS                                                                     */
/*****************************************************************************/

} // end namespace 'mvd'
