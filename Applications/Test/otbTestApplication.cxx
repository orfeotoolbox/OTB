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
  void DoInit()
  {
    SetName("TestApplication");
    SetDescription("This application helps developers to test parameters types");


    SetDocName("Test");
    SetDocLongDescription("The purpose of this application is to test parameters types.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag("Test");

    //std::cout << "TestApplication::DoInit" << std::endl;
    AddParameter(ParameterType_Empty, "boolean", "Boolean");
    AddParameter(ParameterType_Int, "int", "Integer");
    MandatoryOff("int");
    AddParameter(ParameterType_Float, "float", "Float");
    MandatoryOff("float");
    AddParameter(ParameterType_String, "string", "String");
    AddParameter(ParameterType_InputFilename, "filename", "File name");
    AddParameter(ParameterType_OutputFilename, "outfilename", "Output Filename");
    AddParameter(ParameterType_Directory, "directory", "Directory name");


    AddParameter(ParameterType_Choice, "choice", "Choice");
    AddChoice("choice.choice1", "Choice 1");
    AddChoice("choice.choice2", "Choice 2");
    AddChoice("choice.choice3", "Choice 3");
    AddParameter(ParameterType_Float,  "choice.choice1.floatchoice1", "Float of choice1");
    SetDefaultParameterFloat("choice.choice1.floatchoice1",   0.125);
    AddParameter(ParameterType_Float,  "choice.choice3.floatchoice3", "Float of choice3");
    SetDefaultParameterFloat("choice.choice3.floatchoice3",   5.0);


    AddParameter(ParameterType_Group, "ingroup", "Input Group");
    MandatoryOff("ingroup");
    AddParameter(ParameterType_Float,  "ingroup.integer", "Integer of Group");
    AddParameter(ParameterType_Group, "ingroup.images", "Input Images Group");
    AddParameter(ParameterType_InputImage, "ingroup.images.inputimage", "Input Image");

    AddParameter(ParameterType_Group, "outgroup", "Output Group");
    AddParameter(ParameterType_OutputImage, "outgroup.outputimage", "Output Image");

    AddParameter(ParameterType_InputImageList,  "il",   "Input image list");
    MandatoryOff("il");

    AddParameter(ParameterType_ListView,  "cl", "Output Image channels");
    AddChoice("cl.choice1", "cl.choice1");
    AddChoice("cl.choice2", "cl.choice2");
    MandatoryOff("cl");

    AddParameter(ParameterType_ComplexInputImage,  "cin", "Input Complex Image");
    AddParameter(ParameterType_ComplexOutputImage,  "cout", "Output Complex Image");

    // Doc example parameter settings
    SetDocExampleParameterValue("boolean", "true");
    SetDocExampleParameterValue("filename", "myFilename.foo");
  }

  void DoUpdateParameters()
  {
    //std::cout << "TestApplication::DoUpdateParameters" << std::endl;
  }

  void DoExecute()
  {
    //std::cout << "TestApplication::DoExecute" << std::endl;
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TestApplication)
