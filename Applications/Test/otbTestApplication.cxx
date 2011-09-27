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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{

class TestApplication : public Application
{
public:
  /** Standard class typedefs. */
  typedef TestApplication               Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(TestApplication, otb::Application);

private:
  TestApplication()
  {
    SetName("TestApplication");
    SetDescription("This application demonstrate the use of all parameters types");
  }

  virtual ~TestApplication()
  {
  }

  void DoCreateParameters()
  {
    std::cout << "TestApplication::DoCreateParameters" << std::endl;
    AddParameter(ParameterType_Empty, "boolean", "Boolean");
    AddParameter(ParameterType_Int, "int", "Integer");
    MandatoryOff("int");
    AddParameter(ParameterType_Float, "float", "Float");
    MandatoryOff("float");
    AddParameter(ParameterType_String, "string", "String");
    AddParameter(ParameterType_Filename, "filename", "File name");
    AddParameter(ParameterType_Directory, "directory", "Directory name");


    AddParameter(ParameterType_Choice, "choice", "Choice");
    AddChoice("choice.choice1", "Choice 1");
    AddChoice("choice.choice2", "Choice 2");
    AddChoice("choice.choice3", "Choice 3");
    AddParameter(ParameterType_Float,  "choice.choice1.floatchoice1", "Float of choice1");
    SetParameterFloat("choice.choice1.floatchoice1",   0.125);
//  Test a choice without associated parameter
//    AddParameter(ParameterType_Float,  "choice.choice2.floatchoice2", "Float of choice2");
//    SetParameterFloat("choice.choice2.floatchoice2",   1.0);
    AddParameter(ParameterType_Float,  "choice.choice3.floatchoice3", "Float of choice3");
    SetParameterFloat("choice.choice3.floatchoice3",   5.0);


    AddParameter(ParameterType_Group, "ingroup", "Input Group");
    MandatoryOff("ingroup");
    AddParameter(ParameterType_Float,  "ingroup.integer", "Integer of Group");
    AddParameter(ParameterType_Group, "ingroup.images", "Input Images Group");
    AddParameter(ParameterType_InputImage, "ingroup.images.inputimage", "Input Image");
    //AddParameter(ParameterType_InputComplexImage, "ingroup.images.inputcompleximage", "Input Complex Image");
    //AddParameter(ParameterType_InputVectorData, "ingroup.inputvectordata", "Input Vector Data");

    AddParameter(ParameterType_Group, "outgroup", "Output Group");
    AddParameter(ParameterType_OutputImage, "outgroup.outputimage", "Output Image");
    //AddParameter(ParameterType_OutputVectorData, "outgroup.outputvectordata", "Output Vector Data");

    AddParameter(ParameterType_Radius, "radius", "Radius");
  }

  void DoUpdateParameters()
  {
    std::cout << "TestApplication::DoUpdateParameters" << std::endl;
  }

  void DoExecute()
  {
    std::cout << "TestApplication::DoExecute" << std::endl;
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TestApplication)
