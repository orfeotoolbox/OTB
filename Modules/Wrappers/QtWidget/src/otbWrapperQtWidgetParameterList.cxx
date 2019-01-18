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

#include "otbWrapperParameterList.h"
#include "otbWrapperQtWidgetListEditItemModel.h"
#include "otbWrapperQtWidgetListEditWidget.h"
#include "otbWrapperQtWidgetParameterList.h"


namespace otb
{

namespace Wrapper
{


/*****************************************************************************/
QtWidgetParameterList
::QtWidgetParameterList( AbstractParameterList * param, QtWidgetModel * m , QWidget * parent) :
  QtWidgetParameterBase( param, m , parent)
{
  assert( m!=nullptr );

  QObject::connect(
    this, &QtWidgetParameterList::NotifyUpdate ,
    m, &QtWidgetModel::NotifyUpdate
  );
}

/*****************************************************************************/
QtWidgetParameterList
::~QtWidgetParameterList()
{
}

/*****************************************************************************/
void
QtWidgetParameterList
::DoUpdateGUI()
{
}

/*****************************************************************************/
void
QtWidgetParameterList
::DoCreateWidget()
{
  //
  // List-edit widget.
  assert( dynamic_cast< StringListInterface * >( GetParam() )!=nullptr );

  ListEditWidget * widget = new ListEditWidget(
    dynamic_cast< StringListInterface * >( GetParam() ),
    this
  );

  //
  // Global Layout
  QGridLayout * gLayout = new QGridLayout;

  gLayout->setSpacing( 1 );
  gLayout->setContentsMargins( 2, 2, 2, 2 );

  gLayout->addWidget( widget );

  setLayout( gLayout );

  //
  // Connections (Update UserValue flag).
  QObject::connect(
    widget, &ListEditWidget::ValueChanged,
    this, [=] () { emit ParameterChanged( GetParam()->GetKey() ); }
  );

  // Connections (Update app parameters).
  QObject::connect(
    widget, &ListEditWidget::Updated,
    this, &QtWidgetParameterList::NotifyUpdate
  );
}

}

}
