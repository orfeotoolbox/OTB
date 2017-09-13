/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
::QtWidgetParameterList( AbstractParameterList * param, QtWidgetModel * m ) :
  QtWidgetParameterBase( param, m )
{
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
  ListEditWidget * widget = new ListEditWidget();

  assert( widget->GetItemModel() );
  assert( dynamic_cast< StringListInterface * >( GetParam() )!=nullptr );

  widget->GetItemModel()->SetStringList(
    dynamic_cast< StringListInterface * >( GetParam() )
  );

  //
  // Global Layout
  QGridLayout * layout = new QGridLayout();

  layout->setSpacing( 1 );
  layout->setContentsMargins( 2, 2, 2, 2 );

  layout->addWidget( widget );

  setLayout( layout );
}

}

}
