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

#include "otbWrapperQtWidgetParameterFactory.h"

#include "otbWrapperParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperStringListParameter.h"

#include "otbWrapperQtWidgetChoiceParameter.h"
#include "otbWrapperQtWidgetComplexInputImageParameter.h"
#include "otbWrapperQtWidgetComplexOutputImageParameter.h"
#include "otbWrapperQtWidgetDirectoryParameter.h"
#include "otbWrapperQtWidgetFloatParameter.h"
#include "otbWrapperQtWidgetIntParameter.h"
#include "otbWrapperQtWidgetInputFilenameParameter.h"
#include "otbWrapperQtWidgetInputFilenameListParameter.h"
#include "otbWrapperQtWidgetInputImageParameter.h"
#include "otbWrapperQtWidgetInputImageListParameter.h"
#include "otbWrapperQtWidgetInputProcessXMLParameter.h"
#include "otbWrapperQtWidgetInputVectorDataListParameter.h"
#include "otbWrapperQtWidgetInputVectorDataParameter.h"
#include "otbWrapperQtWidgetListViewParameter.h"
#include "otbWrapperQtWidgetModel.h"
#include "otbWrapperQtWidgetOutputFilenameParameter.h"
#include "otbWrapperQtWidgetOutputImageParameter.h"
#include "otbWrapperQtWidgetOutputProcessXMLParameter.h"
#include "otbWrapperQtWidgetOutputVectorDataParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#include "otbWrapperQtWidgetParameterGroup.h"
#include "otbWrapperQtWidgetRAMParameter.h"
#include "otbWrapperQtWidgetStringParameter.h"
#include "otbWrapperQtWidgetStringListParameter.h"
#include "otbWrapperQtWidgetBoolParameter.h"


namespace otb
{


namespace Wrapper
{


template <class TParameterType, class TQtWidget>
class QtWidgetParameterGenericFactory
{
public:

  static bool CanCreate( Parameter* param )
  {
    return dynamic_cast<TParameterType *>(param) != nullptr;
  }

  static QtWidgetParameterBase* Create( Parameter* param, QtWidgetModel* model, QWidget * parent )
  {
    QtWidgetParameterBase * widget = nullptr;
    TParameterType * specificParam = dynamic_cast< TParameterType * >( param );

    // Code should break if param is not a TParameterType and not be silent!
    assert( specificParam!=nullptr );

    widget = new TQtWidget( specificParam, model, parent);

    return widget;
  }
};

QtWidgetParameterFactory::QtWidgetParameterFactory()
{
}

QtWidgetParameterFactory::~QtWidgetParameterFactory()
{
}

QtWidgetParameterBase*
QtWidgetParameterFactory::CreateQtWidget( Parameter* param, QtWidgetModel* model, QWidget * parent )
{
  QtWidgetParameterBase* widget = nullptr;

#define CREATEWIDGET( ParameterType, WidgetType ) \
  else if ( QtWidgetParameterGenericFactory<ParameterType,  WidgetType>::CanCreate(param) ) \
    { \
    widget = QtWidgetParameterGenericFactory<ParameterType,  WidgetType>::Create(param, model, parent); \
    }

  if (0) {}
  CREATEWIDGET(IntParameter,            QtWidgetIntParameter)
  CREATEWIDGET(FloatParameter,          QtWidgetFloatParameter)
  CREATEWIDGET(InputFilenameParameter,       QtWidgetInputFilenameParameter)
  CREATEWIDGET(OutputFilenameParameter,       QtWidgetOutputFilenameParameter)
  CREATEWIDGET(DirectoryParameter,      QtWidgetDirectoryParameter)
  CREATEWIDGET(StringParameter,         QtWidgetStringParameter)
  CREATEWIDGET(StringListParameter,     QtWidgetStringListParameter)
  CREATEWIDGET(ChoiceParameter,         QtWidgetChoiceParameter)
  CREATEWIDGET(ListViewParameter,       QtWidgetListViewParameter)
  CREATEWIDGET(InputImageParameter,     QtWidgetInputImageParameter)
  CREATEWIDGET(ComplexInputImageParameter,     QtWidgetComplexInputImageParameter)
  CREATEWIDGET(ComplexOutputImageParameter,     QtWidgetComplexOutputImageParameter)
  CREATEWIDGET(InputImageListParameter, QtWidgetInputImageListParameter)
  CREATEWIDGET(InputFilenameListParameter, QtWidgetInputFilenameListParameter)
  CREATEWIDGET(InputVectorDataListParameter, QtWidgetInputVectorDataListParameter)
  CREATEWIDGET(InputVectorDataParameter, QtWidgetInputVectorDataParameter)
  CREATEWIDGET(OutputImageParameter,    QtWidgetOutputImageParameter)
  CREATEWIDGET(OutputVectorDataParameter, QtWidgetOutputVectorDataParameter)
  CREATEWIDGET(BoolParameter,          QtWidgetBoolParameter)
  CREATEWIDGET(ParameterGroup,          QtWidgetParameterGroup)
  CREATEWIDGET(RAMParameter,            QtWidgetRAMParameter)
  CREATEWIDGET(OutputProcessXMLParameter,        QtWidgetOutputProcessXMLParameter)
  CREATEWIDGET(InputProcessXMLParameter,        QtWidgetInputProcessXMLParameter)
#undef CREATEWIDGET

  if (widget)
    {
    widget->CreateWidget();
    widget->UpdateGUI();
    }

  return widget;


}


}
}
