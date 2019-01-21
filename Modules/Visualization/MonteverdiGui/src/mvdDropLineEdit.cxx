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
