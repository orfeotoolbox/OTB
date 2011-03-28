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
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperStringParameter.h"
#include "otbWrapperQtWidgetParameterGroup.h"

int otbWrapperQtWidgetParameterFactory(int argc, char* argv[])
{
  QApplication app(argc, argv);

  otb::Wrapper::QtWidgetParameterFactory::Pointer factory = otb::Wrapper::QtWidgetParameterFactory::New();

  otb::Wrapper::IntParameter::Pointer   intParam = otb::Wrapper::IntParameter::New();
  otb::Wrapper::FloatParameter::Pointer floatParam = otb::Wrapper::FloatParameter::New();
  otb::Wrapper::EmptyParameter::Pointer emptyParam = otb::Wrapper::EmptyParameter::New();
  otb::Wrapper::ChoiceParameter::Pointer choiceParam = otb::Wrapper::ChoiceParameter::New();

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

  choiceParam->SetName("Choice parameter");
  choiceParam->SetDescription("This is an choice parameter");
  choiceParam->SetKey("choice");
  choiceParam->AddChoice("choice1", 0);
  choiceParam->AddChoice("choice2", 0);
  choiceParam->AddChoice("choice3", 0);

  QWidget * intWidget   = factory->CreateQtWidget(intParam);
  QWidget * floatWidget = factory->CreateQtWidget(floatParam);
  QWidget * emptyWidget = factory->CreateQtWidget(emptyParam);
  QWidget * choiceWidget = factory->CreateQtWidget(choiceParam);

  if(intWidget)
    {
    intWidget->show();
    floatWidget->show();
    emptyWidget->show();
    choiceWidget->show();

    return app.exec();
    return EXIT_SUCCESS;
    }

  return EXIT_FAILURE;
}



int otbWrapperQtWidgetParameterGroup(int argc, char* argv[])
{
  QApplication app(argc, argv);

  otb::Wrapper::QtWidgetParameterFactory::Pointer factory = otb::Wrapper::QtWidgetParameterFactory::New();

  otb::Wrapper::IntParameter::Pointer   intParam = otb::Wrapper::IntParameter::New();
  otb::Wrapper::FloatParameter::Pointer floatParam = otb::Wrapper::FloatParameter::New();
  otb::Wrapper::EmptyParameter::Pointer emptyParam = otb::Wrapper::EmptyParameter::New();
  otb::Wrapper::ChoiceParameter::Pointer choiceParam = otb::Wrapper::ChoiceParameter::New();
  otb::Wrapper::StringParameter::Pointer stringParam = otb::Wrapper::StringParameter::New();

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

  choiceParam->SetName("Choice parameter");
  choiceParam->SetDescription("This is an choice parameter");
  choiceParam->SetKey("choice");
  choiceParam->AddChoice("choice1", 0);
  choiceParam->AddChoice("choice2", 0);
  choiceParam->AddChoice("choice3", 0);

  stringParam->SetName("String parameter");
  stringParam->SetDescription("This is a string parameter");
  stringParam->SetKey("string");
  stringParam->SetValue("test value");

  otb::Wrapper::ParameterList list;
  list.push_back((otb::Wrapper::Parameter*)intParam);
  list.push_back((otb::Wrapper::Parameter*)floatParam);
  list.push_back((otb::Wrapper::Parameter*)emptyParam);
  list.push_back((otb::Wrapper::Parameter*)choiceParam);
  list.push_back((otb::Wrapper::Parameter*)stringParam);

  QWidget * group = new otb::Wrapper::QtWidgetParameterGroup(list);

  if(group)
    {
    group->show();
    return app.exec();
    // return EXIT_SUCCESS;
    }

  return EXIT_FAILURE;
}
