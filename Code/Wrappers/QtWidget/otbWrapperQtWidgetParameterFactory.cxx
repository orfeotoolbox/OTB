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

#include "otbWrapperQtWidgetEmptyParameter.h"
#include "otbWrapperQtWidgetIntParameter.h"
#include "otbWrapperQtWidgetFloatParameter.h"
#include "otbWrapperQtWidgetStringParameter.h"
#include "otbWrapperQtWidgetChoiceParameter.h"
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

  static QWidget* Create( Parameter* param, QtWidgetModel* model )
  {
    QWidget* widget = 0;
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

QWidget*
QtWidgetParameterFactory::CreateQtWidget( Parameter* param, QtWidgetModel* model )
{
  QWidget* widget = 0;

  typedef QtWidgetParameterGenericFactory<EmptyParameter,  QtWidgetEmptyParameter>   EmptyWidgetFactory;
  typedef QtWidgetParameterGenericFactory<IntParameter,    QtWidgetIntParameter>     IntWidgetFactory;
  typedef QtWidgetParameterGenericFactory<FloatParameter,  QtWidgetFloatParameter>   FloatWidgetFactory;
  typedef QtWidgetParameterGenericFactory<StringParameter, QtWidgetStringParameter>  StringWidgetFactory;
  typedef QtWidgetParameterGenericFactory<ChoiceParameter, QtWidgetChoiceParameter>  ChoiceWidgetFactory;
  typedef QtWidgetParameterGenericFactory<ParameterGroup,  QtWidgetParameterGroup>   GroupWidgetFactory;

  if ( EmptyWidgetFactory::CanCreate(param) )
    {
    widget = EmptyWidgetFactory::Create(param, model);
    }
  else if ( IntWidgetFactory::CanCreate(param) )
    {
    widget = IntWidgetFactory::Create(param, model);
    }
  else if ( FloatWidgetFactory::CanCreate(param) )
    {
    widget = FloatWidgetFactory::Create(param, model);
    }
  else if ( ChoiceWidgetFactory::CanCreate(param) )
    {
    widget = ChoiceWidgetFactory::Create(param, model);
    }
  else if ( StringWidgetFactory::CanCreate(param) )
    {
    widget = StringWidgetFactory::Create(param, model);
    }
  else if ( GroupWidgetFactory::CanCreate(param) )
    {
    widget = GroupWidgetFactory::Create(param, model);
    }

  return widget;
}


}
}
