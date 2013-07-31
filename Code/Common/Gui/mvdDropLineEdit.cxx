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
#include "Gui/mvdDropLineEdit.h"


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
::DropLineEdit( QWidget* parent ) :
  QLineEdit( parent )
{
  setReadOnly( true );
  setAcceptDrops( true );
}

/*******************************************************************************/
DropLineEdit
::DropLineEdit( const QString& contents, QWidget* parent ) :
  QLineEdit( contents, parent )
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
::dragEnterEvent( QDragEnterEvent* event )
{
  assert( event!=NULL );

  // Accepts all proposed actions: check will be made in
  // dragMoveEvent().
  event->acceptProposedAction();
}

/*******************************************************************************/
void 
DropLineEdit
::dragMoveEvent( QDragMoveEvent* event )
{
  assert( event!=NULL );

  if( ( event->mimeData()->hasText() &&
	QUrl( event->mimeData()->text() ).isLocalFile() ) ||
      ( event->mimeData()->hasFormat(
	"application/x-qabstractitemmodeldatalist" ) ) )
    {
    event->acceptProposedAction();
    }
  else
    {
    event->ignore();
    }
}

/*******************************************************************************/
void
DropLineEdit
::dropEvent( QDropEvent *event )
{
  assert( event!=NULL );

  if( event->mimeData()->hasText() )
    {
    QUrl url( event->mimeData()->text() );
    
    if( url.isLocalFile() )
      {
      setText( url.toLocalFile() );
      }
    }
  else if( event->mimeData()->hasFormat(
	     "application/x-qabstractitemmodeldatalist" ) )
    {
    // TODO: use  simpler mime type
    setText(
      event->mimeData()->data( "application/x-qabstractitemmodeldatalist" )
    );
    }
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
