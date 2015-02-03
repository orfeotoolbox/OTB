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
#include "mvdLayerStackController.h"


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
#include "Core/mvdStackedLayerModel.h"
#include "Gui/mvdLayerStackItemModel.h"
#include "Gui/mvdLayerStackWidget.h"

namespace mvd
{
/*
  TRANSLATOR mvd::LayerStackController

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
LayerStackController
::LayerStackController( LayerStackWidget * widget, QObject * parent ) :
  AbstractModelController( widget, parent )
{
}

/*******************************************************************************/
LayerStackController
::~LayerStackController()
{
}

/*******************************************************************************/
void
LayerStackController
::ClearWidget()
{
}

/*******************************************************************************/
void
LayerStackController
::Connect( AbstractModel * model )
{
  assert( model==qobject_cast< StackedLayerModel * >( model ) );


  QObject::connect(
    model,
    SIGNAL( CurrentChanged( size_t ) ),
    // to:
    this,
    SLOT( OnStackedLayerCurrentChanged( size_t ) )
  );


  LayerStackWidget * widget = GetWidget< LayerStackWidget >();
  assert( widget!=NULL );

  assert( widget->GetItemModel()!=NULL );

  widget->GetItemModel()->SetStack(
    qobject_cast< StackedLayerModel * >( model )
  );


#if 0
  QObject::connect(
    widget,
    SIGNAL( CurrentChanged( int ) ),
    // to:
    this,
    SLOT( OnCurrentChanged( int ) )
  );
#else
  QObject::connect(
    widget,
    SIGNAL( SelectionChanged( int ) ),
    // to:
    this,
    SLOT( OnSelectionChanged( int ) )
  );
#endif
}

/*******************************************************************************/
void
LayerStackController
::Disconnect( AbstractModel * model )
{
  // assert( model==qobject_cast< StackedLayerModel * >( model ) );


  QObject::disconnect(
    model,
    SIGNAL( CurrentChanged( size_t ) ),
    // to:
    this,
    SLOT( OnStackedLayerCurrentChanged( size_t ) )
  );


  LayerStackWidget * widget = GetWidget< LayerStackWidget >();
  assert( widget!=NULL );

  assert( widget->GetItemModel()!=NULL );

  widget->GetItemModel()->SetStack( NULL );


#if 0
  QObject::disconnect(
    widget,
    SIGNAL( CurrentChanged( int ) ),
    // to:
    this,
    SLOT( OnCurrentChanged( int ) )
  );
#else
  QObject::disconnect(
    widget,
    SIGNAL( SelectionChanged( int ) ),
    // to:
    this,
    SLOT( OnSelectionChanged( int ) )
  );
#endif
}

/*******************************************************************************/
void
LayerStackController
::ResetWidget()
{
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
LayerStackController
::OnCurrentChanged( int index )
{
  qDebug() << this << "::OnCurrentChanged(" << index << ")";

  assert( GetModel()==GetModel< StackedLayerModel >() );
  StackedLayerModel * model = GetModel< StackedLayerModel >();
  assert( model!=NULL );

  model->SetCurrent( index );
}

/*******************************************************************************/
void
LayerStackController
::OnSelectionChanged( int index )
{
  qDebug() << this << "::OnSelectionChanged(" << index << ")";

  assert( GetModel()==GetModel< StackedLayerModel >() );
  StackedLayerModel * model = GetModel< StackedLayerModel >();
  assert( model!=NULL );

  model->SetCurrent( index );
}

/*******************************************************************************/
void
LayerStackController
::OnStackedLayerCurrentChanged( size_t index )
{
  qDebug() << this << "::OnStackedLayerCurrentChanged(" << index << ")";

  LayerStackWidget * widget = GetWidget< LayerStackWidget >();
  assert( widget!=NULL );

  assert( widget->GetItemModel()!=NULL );

  bool signalsBlocked = widget->blockSignals( true );
  {
  widget->SetCurrent( index );
  }
  widget->blockSignals( signalsBlocked );
}

} // end namespace 'mvd'
