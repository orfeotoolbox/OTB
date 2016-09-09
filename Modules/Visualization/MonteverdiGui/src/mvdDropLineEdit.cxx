/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mvdDropLineEdit.h"


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
  TRANSLATOR mvd::DropLineEdit

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
DropLineEdit
::DropLineEdit( QWidget* p ) :
  QLineEdit( p )
{
  setReadOnly( true );
  setAcceptDrops( true );
}

/*******************************************************************************/
DropLineEdit
::DropLineEdit( const QString& contents, QWidget* p ) :
  QLineEdit( contents, p )
{
  setReadOnly( true );
  setAcceptDrops( true );
}

/*******************************************************************************/
DropLineEdit
::~DropLineEdit()
{
}

/*******************************************************************************/
void
DropLineEdit
::dragEnterEvent( QDragEnterEvent* e )
{
  assert( e!=NULL );

  // Accepts all proposed actions: check will be made in
  // dragMoveEvent().
  e->acceptProposedAction();
}

/*******************************************************************************/
void 
DropLineEdit
::dragMoveEvent( QDragMoveEvent* e )
{
  assert( e!=NULL );

  /*
  if( ( e->mimeData()->hasText() &&
	QUrl( e->mimeData()->text() ).isLocalFile() ) ||
      ( e->mimeData()->hasFormat(
	"application/x-qabstractitemmodeldatalist" ) ) )
  */
  if( e->mimeData()->hasText() ||
      e->mimeData()->hasFormat(
	"application/x-qabstractitemmodeldatalist" ) )
    {
    e->acceptProposedAction();
    }
  else
    {
    e->ignore();
    }
}

/*******************************************************************************/
void
DropLineEdit
::dropEvent( QDropEvent *e )
{
  assert( e!=NULL );

  if( e->mimeData()->hasText() )
    {
    QUrl url( e->mimeData()->text() );

    /*
    if( url.isLocalFile() )
      {
    */
    setText( url.toLocalFile() );
    /*
      }
    */
    }
  else if( e->mimeData()->hasFormat(
	     "application/x-qabstractitemmodeldatalist" ) )
    {
    // TODO: use  simpler mime type
    setText(
      e->mimeData()->data( "application/x-qabstractitemmodeldatalist" )
    );
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
