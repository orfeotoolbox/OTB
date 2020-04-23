/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
#include "mvdAbstractModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::AbstractModelController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
AbstractModelController::AbstractModelController(QWidget* widget, QObject* p) : QObject(p), m_Widget(widget), m_Model(NULL)
{
  assert(widget != NULL);

  QObject::connect(this, SIGNAL(destroyed(QObject*)), this, SLOT(OnDestroyed(QObject*)));

  QObject::connect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(OnDestroyed(QObject*)));
}

/*******************************************************************************/
AbstractModelController::~AbstractModelController()
{
  /*
  assert( m_Model==NULL && "AbstractModelController::m_Model should be
  disconnected before destroying controller instance." );
  */
}

/*****************************************************************************/
void AbstractModelController::SetModel(AbstractModel* model)
{
  // Disconnect previously connected model and signal listeners.
  private_Disconnect(m_Model);

  // Disable GUI widget while there is no model.
  GetWidget()->setEnabled(false);

  // Clear widget from previous values.
  private_ClearWidget();

  // Forget previously disconnected model before new model is
  // connected. This is done in order to stay in a consistent internal
  // state whenever the connection is aborted (e.g. by an exception).
  m_Model = NULL;

  // Connect new model and signal listeners.
  private_Connect(model);

  // Remember newly connected model.
  m_Model = model;

  // Enable GUI widget when there is a model.
  if (m_Model != NULL)
  {
    virtual_ResetWidget(true);

    GetWidget()->setEnabled(true);
  }
}

/*****************************************************************************/
void AbstractModelController::private_Connect(AbstractModel* model)
{
  if (model == NULL)
    return;

  emit AboutToConnectModel(model);

  QObject::connect(model, SIGNAL(destroyed(QObject*)), this, SLOT(OnDestroyed(QObject*)));

  Connect(model);

  emit ModelConnected(model);
}

/*****************************************************************************/
void AbstractModelController::private_Disconnect(AbstractModel* model)
{
  if (model == NULL)
    return;

  emit AboutToDisconnectModel(model);

  Disconnect(model);

  QObject::disconnect(model, SIGNAL(destroyed(QObject*)), this, SLOT(OnDestroyed(QObject*)));

  emit ModelDisconnected(model);
}

/*****************************************************************************/
void AbstractModelController::private_ClearWidget()
{
  // Reset UI from model.
  ClearWidget();
}

/*****************************************************************************/
void AbstractModelController::ResetWidget()
{
  // Reset UI from model.
  virtual_ResetWidget(false);

#if 0
  // Signal model has been updated.
  // When resetting widget, data is read from model and set into
  // widget so, there's no need to signal back model update.
  emit ModelUpdated();
#endif
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void AbstractModelController::RefreshWidget()
{
}

/*******************************************************************************/
void AbstractModelController::OnDestroyed(QObject* object)
{
  assert(object != NULL);

  if (object == m_Widget)
  {
    // qDebug() << "Disconnect (widget) on destroy.";

    // When wrapped widget is destroyed, auto-destroy controller which
    // has no other reason to live than to wrap the widget.
    //
    // N.B.: Qt will automatically disconnect all signals from deleted
    // object.
    deleteLater();
  }
  else if (object == m_Model)
  {
#if 0
    qDebug() << "Disconnect (model" << object << ") on destroy.";

    // When model is destroyed, safely disconnect-it from controller
    // if no previous call to SetModel( NULL ) has been made from
    // client.
    //
    // N.B.: This is quite equivalent to SetModel( NULL ) but is
    // quite bit faster.
    private_Disconnect( m_Model );
#endif
  }
}

/*******************************************************************************/

} // end namespace 'mvd'
