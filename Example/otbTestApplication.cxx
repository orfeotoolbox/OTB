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
  typedef TestApplication                      Self;
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

    std::cout << ">>>>>>>>>>>>>> TestApplication" << std::endl;
  }

  virtual ~TestApplication()
  {
    std::cout << ">>>>>>>>>>>>>> ~TestApplication" << std::endl;
  }

  void DoCreateParameters()
  {
    std::cout << "TestApplication::DoCreateParameters" << std::endl;
    AddParameter(ParameterType_Empty, "boolean", "Boolean");
    AddParameter(ParameterType_Int, "int", "Integer");
    AddParameter(ParameterType_Float, "float", "Float");
    AddParameter(ParameterType_String, "string", "String");
    AddParameter(ParameterType_Filename, "filename", "File name");
    AddParameter(ParameterType_Directory, "directory", "Directory name");
    AddParameter(ParameterType_InputImage, "inputimage", "Input Image");
    AddParameter(ParameterType_InputComplexImage, "inputcompleximage", "Input Complex Image");
    AddParameter(ParameterType_InputVectorData, "inputvectordata", "Input Vector Data");
    AddParameter(ParameterType_OutputImage, "outputimage", "Output Image");
    AddParameter(ParameterType_OutputVectorData, "outputvectordata", "Output Vector Data");
    AddParameter(ParameterType_Radius, "radius", "Radius");
    //AddParameter(ParameterType_Group, "group", "Group");
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
