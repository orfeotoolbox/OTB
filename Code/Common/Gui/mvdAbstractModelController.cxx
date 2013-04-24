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
#include "mvdAbstractModelController.h"

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
#include "Core/mvdAbstractModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::AbstractModelController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
AbstractModelController
::AbstractModelController( QWidget* widget, QObject* parent ):
  QObject( parent ),
  m_Widget( widget ),
  m_Model( NULL )
{
  assert( widget!=NULL );

  QObject::connect(
    this, SIGNAL( destroyed( QObject* ) ),
    this, SLOT( OnDestroyed( QObject* ) )
  );

  QObject::connect(
    widget, SIGNAL( destroyed( QObject* ) ),
    this, SLOT( OnDestroyed( QObject* ) )
  );
}

/*******************************************************************************/
AbstractModelController
::~AbstractModelController()
{
  /*
  assert( m_Model==NULL && "AbstractModelController::m_Model should be
  disconnected before destroying controller instance." );
  */
}

/*****************************************************************************/
void
AbstractModelController
::SetModel( AbstractModel* model )
{
  // Disconnect previously connected model and signal listeners.
  private_Disconnect( m_Model );

  // Disable GUI widget while there is no model.
  GetWidget()->setEnabled( false );

  // Forget previously disconnected model before new model is
  // connected. This is done in order to stay in a consistent internal
  // state whenever the connection is aborted (e.g. by an exception).
  m_Model = NULL;

  // Connect new model and signal listeners.
  private_Connect( model );

  // Remember newly connected model.
  m_Model = model;

  // Enable GUI widget when there is a model.
  if( m_Model!=NULL )
    {
    private_ResetWidget();

    GetWidget()->setEnabled( true );
    }
}

/*****************************************************************************/
void
AbstractModelController
::private_Connect( AbstractModel* model )
{
  if( model==NULL )
    return;

  emit AboutToConnectModel( model );

  QObject::connect(
    model, SIGNAL( destroyed( QObject* ) ),
    this, SLOT( OnDestroyed( QObject* ) )
  );

  Connect( model );

  emit ModelConnected( model );
}

/*****************************************************************************/
void
AbstractModelController
::private_Disconnect( AbstractModel* model )
{
  if( model==NULL )
    return;

  emit AboutToDisconnectModel( model );

  Disconnect( model );

  QObject::disconnect(
    model, SIGNAL( destroyed( QObject* ) ),
    this, SLOT( OnDestroyed( QObject* ) )
  );

  emit ModelDisconnected( model );
}

/*****************************************************************************/
void
AbstractModelController
::private_ResetWidget()
{
  // Reset UI from model.
  ResetWidget();

  // Signal model has been updated.
  emit ModelUpdated();
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
AbstractModelController
::OnDestroyed( QObject* object )
{
  /*
  qDebug() << object;
  qDebug() << this;
  qDebug() << m_Widget;
  qDebug() << m_Model;
  */

  assert( object!=NULL );

  if( object==m_Widget )
    {
    // qDebug() << "Disconnect (widget) on destroy.";

    // When wrapped widget is destroyed, auto-destroy controller which
    // has no other reason to live than to wrap the widget.
    //
    // N.B.: Qt will automatically disconnect all signals from deleted
    // object.
    deleteLater();
    }
  else if( object==m_Model )
    {
    // qDebug() << "Disconnect (model) on destroy.";

    // When model is destroyed, safely disconnect-it from controller
    // if no previous call to SetModel( NULL ) has been made from
    // client.
    //
    // N.B.: This is quite equivalent to SetModel( NULL ) but is
    // quite bit faster.
    private_Disconnect( m_Model );
    }
}

/*******************************************************************************/

} // end namespace 'mvd'
