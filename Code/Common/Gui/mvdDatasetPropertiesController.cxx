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
#include "Gui/mvdDatasetPropertiesController.h"


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

//
#include "Gui/mvdDatasetPropertiesWidget.h"

#include "Core/mvdVectorImageModel.h"
#include "Core/mvdDatasetModel.h"

namespace mvd
{
/*
  TRANSLATOR mvd::DatasetPropertiesController

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
DatasetPropertiesController
::DatasetPropertiesController( DatasetPropertiesWidget* widget, QObject* parent ) :
  AbstractModelController( widget, parent )
{
}

/*******************************************************************************/
DatasetPropertiesController
::~DatasetPropertiesController()
{
}

/*******************************************************************************/
void
DatasetPropertiesController
::Connect( AbstractModel* model )
{
  // nothing to do
}

/*******************************************************************************/
void
DatasetPropertiesController
::Disconnect( AbstractModel* model )
{
  // nothing to do 
}

/*******************************************************************************/
void
DatasetPropertiesController
::ResetWidget()
{
  qDebug() << this << "::ResetWidget()";

  // Reset widget.
  ResetDatasetTree();
  
  // 
  // No Signal to emit
  //emit ModelUpdated();
}

/*******************************************************************************/
void
DatasetPropertiesController
::ResetDatasetTree()
{
  //
  // Access widget.
  DatasetPropertiesWidget* widget = GetWidget< DatasetPropertiesWidget >();

  //
  // Access model.
  DatasetModel* model = GetModel< DatasetModel >();
  assert( model!=NULL );

  // Block this controller's signals to prevent display refreshes
  // but let let widget(s) signal their changes so linked values
  // will be correctly updated.
  this->blockSignals( true );
  {
  // Block widget's signals...
  //...but force call to valueChanged() slot to force refresh.
  widget->blockSignals( true );
  {
  // TODO: Fill in widget.
  widget->FillDatasetProperties( model->QueryDatasetProperties() );
  }
  widget->blockSignals( false );
  }
  this->blockSignals( false );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
