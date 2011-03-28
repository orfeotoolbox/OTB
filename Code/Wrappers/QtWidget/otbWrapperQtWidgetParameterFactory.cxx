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


//#include "otbWrapperQtWidgetNumericalParameterFactory.h"
#include "otbWrapperQtWidgetEmptyParameterFactory.h"
//#include "otbWrapperQtWidgetStringParameterFactory.h"
//#include "otbWrapperQtWidgetChoiceParameterFactory.h"

namespace otb
{
namespace Wrapper
{

QtWidgetParameterFactory::QtWidgetParameterFactory()
{

}


QtWidgetParameterFactory::~QtWidgetParameterFactory()
{

}

QWidget*
QtWidgetParameterFactory::CreateQtWidget( Parameter* param )
{

  QWidget* widget = 0;

  if (!widget)
    widget = QtWidgetIntParameterFactory::CreateQtWidget( param );

  //  if (!widget)
  //    widget = QtWidgetFloatParameterFactory::CreateQtWidget( param );

  if (!widget)
    widget = QtWidgetEmptyParameterFactory::CreateQtWidget( param );

  //  if (!widget)
  //   widget = QtWidgetStringParameterFactory::CreateQtWidget( param );

  //if (!widget)
  //  widget = QtWidgetChoiceParameterFactory::CreateQtWidget( param );

  return 0;
}


}
}

#endif
