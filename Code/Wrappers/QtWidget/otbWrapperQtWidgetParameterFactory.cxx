/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbWrapperQtWidgetParameterFactory.h"

#include "otbWrapperParameter.h"
#include "otbWrapperQtWidgetModel.h"

#include "otbWrapperQtWidgetParameterBase.h"

#include "otbWrapperQtWidgetEmptyParameter.h"
#include "otbWrapperQtWidgetIntParameter.h"
#include "otbWrapperQtWidgetFloatParameter.h"
#include "otbWrapperQtWidgetStringParameter.h"
#include "otbWrapperQtWidgetChoiceParameter.h"
#include "otbWrapperQtWidgetInputImageParameter.h"
#include "otbWrapperQtWidgetOutputImageParameter.h"
#include "otbWrapperQtWidgetParameterGroup.h"

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
    return dynamic_cast<TParameterType *>(param) != 0;
  }

  static QtWidgetParameterBase* Create( Parameter* param, QtWidgetModel* model )
  {
    QtWidgetParameterBase* widget = 0;
    TParameterType* specificParam = dynamic_cast<TParameterType *>(param);

    if (specificParam)
      {
      widget = new TQtWidget(specificParam, model);
      }
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
QtWidgetParameterFactory::CreateQtWidget( Parameter* param, QtWidgetModel* model )
{
  QtWidgetParameterBase* widget = 0;

#define CREATEWIDGET( ParameterType, WidgetType ) \
  else if ( QtWidgetParameterGenericFactory<ParameterType,  WidgetType>::CanCreate(param) ) \
    { \
    widget = QtWidgetParameterGenericFactory<ParameterType,  WidgetType>::Create(param, model); \
    }

  if (0) {}
  CREATEWIDGET(EmptyParameter,       QtWidgetEmptyParameter)
  CREATEWIDGET(IntParameter,         QtWidgetIntParameter)
  CREATEWIDGET(FloatParameter,       QtWidgetFloatParameter)
  CREATEWIDGET(StringParameter,      QtWidgetStringParameter)
  CREATEWIDGET(ChoiceParameter,      QtWidgetChoiceParameter)
  CREATEWIDGET(InputImageParameter,  QtWidgetInputImageParameter)
  CREATEWIDGET(OutputImageParameter, QtWidgetOutputImageParameter)
  CREATEWIDGET(EmptyParameter,       QtWidgetEmptyParameter)
  CREATEWIDGET(ParameterGroup,       QtWidgetParameterGroup)

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
