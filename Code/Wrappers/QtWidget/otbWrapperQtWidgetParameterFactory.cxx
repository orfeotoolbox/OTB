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
#include "otbWrapperQtWidgetDoubleParameter.h"
#include "otbWrapperQtWidgetStringParameter.h"
#include "otbWrapperQtWidgetStringListParameter.h"
#include "otbWrapperQtWidgetChoiceParameter.h"
#include "otbWrapperQtWidgetListViewParameter.h"
#include "otbWrapperQtWidgetInputImageParameter.h"
#include "otbWrapperQtWidgetComplexInputImageParameter.h"
#include "otbWrapperQtWidgetComplexOutputImageParameter.h"
#include "otbWrapperQtWidgetInputImageListParameter.h"
#include "otbWrapperQtWidgetOutputImageParameter.h"
#include "otbWrapperQtWidgetOutputVectorDataParameter.h"
#include "otbWrapperQtWidgetInputFilenameParameter.h"
#include "otbWrapperQtWidgetInputFilenameListParameter.h"
#include "otbWrapperQtWidgetOutputFilenameParameter.h"
#include "otbWrapperQtWidgetDirectoryParameter.h"
#include "otbWrapperQtWidgetParameterGroup.h"
#include "otbWrapperQtWidgetInputVectorDataListParameter.h"
#include "otbWrapperQtWidgetInputVectorDataParameter.h"
#include "otbWrapperQtWidgetRAMParameter.h"

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
  CREATEWIDGET(EmptyParameter,          QtWidgetEmptyParameter)
  CREATEWIDGET(IntParameter,            QtWidgetIntParameter)
  CREATEWIDGET(FloatParameter,          QtWidgetFloatParameter)
  CREATEWIDGET(DoubleParameter,         QtWidgetDoubleParameter)
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
  CREATEWIDGET(EmptyParameter,          QtWidgetEmptyParameter)
  CREATEWIDGET(ParameterGroup,          QtWidgetParameterGroup)
  CREATEWIDGET(RAMParameter,            QtWidgetRAMParameter)

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
