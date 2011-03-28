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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperQtWidgetParameterFactory.h"
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperNumericalParameter.h"

int otbWrapperQtWidgetParameterFactory(int argc, char* argv[])
{
  QApplication app(argc, argv);

  otb::Wrapper::QtWidgetParameterFactory::Pointer factory = otb::Wrapper::QtWidgetParameterFactory::New();

  otb::Wrapper::IntParameter::Pointer   intParam = otb::Wrapper::IntParameter::New();
  otb::Wrapper::FloatParameter::Pointer floatParam = otb::Wrapper::FloatParameter::New();
  otb::Wrapper::EmptyParameter::Pointer emptyParam = otb::Wrapper::EmptyParameter::New();

  intParam->SetName("Int parameter");
  intParam->SetDescription("This is an int parameter");
  intParam->SetKey("int");
  intParam->SetDefaultValue(10);
  intParam->SetValue(5);
  intParam->SetMinimumValue(-10);
  intParam->SetMaximumValue(10);

  floatParam->SetName("Float parameter");
  floatParam->SetDescription("This is an float parameter");
  floatParam->SetKey("float");
  floatParam->SetDefaultValue(0.567);
  floatParam->SetValue(0.21);
  floatParam->SetMinimumValue(-3.75);
  floatParam->SetMaximumValue(4.97);

  emptyParam->SetName("Empty parameter");
  emptyParam->SetDescription("This is an empty parameter");
  emptyParam->SetKey("empty");


  QWidget * intWidget = factory->CreateQtWidget(intParam);
  QWidget * floatWidget = factory->CreateQtWidget(floatParam);
  QWidget * emptyWidget = factory->CreateQtWidget(emptyParam);

  if(intWidget)
    {
    intWidget->show();
    floatWidget->show();
    emptyWidget->show();

    return EXIT_SUCCESS;
    }

  return EXIT_FAILURE;
}
