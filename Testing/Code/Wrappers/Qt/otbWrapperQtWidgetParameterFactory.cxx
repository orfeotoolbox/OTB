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
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperStringParameter.h"

#include "otbWrapperQtWidgetModel.h"

#include "otbWrapperQtWidgetParameterGroup.h"

namespace otb
{
namespace WrapperTest
{

class ITK_EXPORT StubApplication : public otb::Wrapper::Application
{
public:
  /** Standard class typedefs. */
  typedef StubApplication                      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Self, otb::Application);

protected:
  StubApplication()
  {
  }

  virtual ~StubApplication()
  {
  }

  void DoInit()
  {
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
  }
};

}
}


int otbWrapperQtWidgetParameterFactory(int argc, char* argv[])
{
  QApplication app(argc, argv);
  otb::WrapperTest::StubApplication::Pointer application = otb::WrapperTest::StubApplication::New();
  otb::Wrapper::QtWidgetModel* model = new otb::Wrapper::QtWidgetModel(application);
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
  choiceParam->SetDescription("This is a choice parameter");
  choiceParam->SetKey("choice");
  choiceParam->AddChoice("choice1", "Choice 1");
  choiceParam->AddChoice("choice2", "Choice 2");
  choiceParam->AddChoice("choice3", "Choice 3");

  QWidget * intWidget   = factory->CreateQtWidget(intParam, model);
  QWidget * floatWidget = factory->CreateQtWidget(floatParam, model);
  QWidget * emptyWidget = factory->CreateQtWidget(emptyParam, model);
  QWidget * choiceWidget = factory->CreateQtWidget(choiceParam, model);

  if(intWidget)
    {
    intWidget->show();
    floatWidget->show();
    emptyWidget->show();
    choiceWidget->show();

    //return app.exec();
    return EXIT_SUCCESS;
    }

  return EXIT_FAILURE;
}



int otbWrapperQtWidgetParameterGroup(int argc, char* argv[])
{
  QApplication app(argc, argv);

  otb::WrapperTest::StubApplication::Pointer application = otb::WrapperTest::StubApplication::New();
  otb::Wrapper::QtWidgetModel* model = new otb::Wrapper::QtWidgetModel(application);

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
  choiceParam->AddChoice("choice1", "Choice 1");
  choiceParam->AddChoice("choice2", "Choice 2");
  choiceParam->AddChoice("choice3", "Choice 3");

  stringParam->SetName("String parameter");
  stringParam->SetDescription("This is a string parameter");
  stringParam->SetKey("string");
  stringParam->SetValue("test value");

  otb::Wrapper::ParameterGroup::Pointer list = otb::Wrapper::ParameterGroup::New();
  list->AddParameter(otb::Wrapper::Parameter::Pointer(intParam.GetPointer()));
  list->AddParameter(otb::Wrapper::Parameter::Pointer(floatParam.GetPointer()));
  list->AddParameter(otb::Wrapper::Parameter::Pointer(emptyParam.GetPointer()));
  list->AddParameter(otb::Wrapper::Parameter::Pointer(choiceParam.GetPointer()));
  list->AddParameter(otb::Wrapper::Parameter::Pointer(stringParam.GetPointer()));

  otb::Wrapper::IntParameter::Pointer   intParam2 = otb::Wrapper::IntParameter::New();
  otb::Wrapper::FloatParameter::Pointer floatParam2 = otb::Wrapper::FloatParameter::New();
  otb::Wrapper::EmptyParameter::Pointer emptyParam2 = otb::Wrapper::EmptyParameter::New();
  otb::Wrapper::ChoiceParameter::Pointer choiceParam2 = otb::Wrapper::ChoiceParameter::New();
  otb::Wrapper::StringParameter::Pointer stringParam2 = otb::Wrapper::StringParameter::New();
  intParam2->SetName("Int parameter");
  intParam2->SetDescription("This is an int parameter");
  intParam2->SetKey("int2");
  intParam2->SetDefaultValue(10);
  intParam2->SetValue(5);
  intParam2->SetMinimumValue(-10);
  intParam2->SetMaximumValue(10);

  floatParam2->SetName("Float parameter");
  floatParam2->SetDescription("This is an float parameter");
  floatParam2->SetKey("float2");
  floatParam2->SetDefaultValue(0.567);
  floatParam2->SetValue(0.21);
  floatParam2->SetMinimumValue(-3.75);
  floatParam2->SetMaximumValue(4.97);

  emptyParam2->SetName("Empty parameter");
  emptyParam2->SetDescription("This is an empty parameter");
  emptyParam2->SetKey("empty2");

  choiceParam2->SetName("Choice parameter");
  choiceParam2->SetDescription("This is an choice parameter");
  choiceParam2->SetKey("choice2");
  choiceParam2->AddChoice("choice1", "Choice 1");
  choiceParam2->AddChoice("choice2", "Choice 2");
  choiceParam2->AddChoice("choice3", "Choice 3");

  stringParam2->SetName("String parameter");
  stringParam2->SetDescription("This is a string parameter");
  stringParam2->SetKey("string2");
  stringParam2->SetValue("test value");

  otb::Wrapper::ParameterGroup::Pointer group2 = otb::Wrapper::ParameterGroup::New();
  group2->SetName("Group parameter");
  group2->SetDescription("This is a group parameter");
  group2->SetKey("group2");

  group2->AddParameter(otb::Wrapper::Parameter::Pointer(intParam2.GetPointer()));
  group2->AddParameter(otb::Wrapper::Parameter::Pointer(floatParam2.GetPointer()));
  group2->AddParameter(otb::Wrapper::Parameter::Pointer(emptyParam2.GetPointer()));
  group2->AddParameter(otb::Wrapper::Parameter::Pointer(choiceParam2.GetPointer()));
  group2->AddParameter(otb::Wrapper::Parameter::Pointer(stringParam2.GetPointer()));
  group2->AddParameter(otb::Wrapper::Parameter::Pointer(list.GetPointer()));

  QWidget * group = new otb::Wrapper::QtWidgetParameterGroup(group2, model);

  if(group)
    {
    group->show();
    //return app.exec();
    return EXIT_SUCCESS;
    }

  return EXIT_FAILURE;
}
