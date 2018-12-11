/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperInputFilenameListParameter.h"

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
  void DoInit() override
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
    AddParameter(ParameterType_Bool, "boolean", "Boolean");
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

    AddParameter( ParameterType_InputFilenameList, "fl", "Input filename list" );
    MandatoryOff( "fl" );

    AddParameter( ParameterType_InputVectorDataList, "vdl", "Input vector-data list" );
    MandatoryOff( "vdl" );

    AddParameter( ParameterType_StringList, "sl", "Input string list" );
    MandatoryOff( "sl" );

    AddParameter(ParameterType_ListView,  "cl", "Output Image channels");
    AddChoice("cl.choice1", "Choice1");
    AddChoice("cl.choice2", "Choice2");
    MandatoryOff("cl");

    AddParameter(ParameterType_ComplexInputImage,  "cin", "Input Complex Image");
    AddParameter(ParameterType_ComplexOutputImage,  "cout", "Output Complex Image");

    // Doc example parameter settings
    SetDocExampleParameterValue("boolean", "true");
    SetDocExampleParameterValue("filename", "myFilename.foo");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {}

  void DoExecute() override
  {
    if( HasValue("il") && IsParameterEnabled("il") )
      {
      FloatVectorImageListType * imgList = GetParameterImageList( "il" );
      SetParameterOutputImage(
        "outgroup.outputimage",
        imgList->GetNthElement( 0 )
      );
      }
    else if (HasValue("ingroup.images.inputimage") && IsParameterEnabled("ingroup") )
      {
      SetParameterOutputImage("outgroup.outputimage",
        GetParameterImage("ingroup.images.inputimage"));
      }
    else
      {
      otbAppLogFATAL("Waiting at least one input image");
      }

    SetParameterComplexOutputImage(
      "cout",
      GetParameterComplexImage( "cin" )
    );

    PrintStrings( "fl" );
  }

private:
  void PrintStrings( const std::string & key ) const
  {
    if( !IsParameterEnabled(key) )
      return;

    const Parameter * p = GetParameterByKey( key );
    assert( p!=nullptr );
    const StringListInterface * sli =
      dynamic_cast< const StringListInterface * >(p);
    assert( sli!=nullptr );

    StringListInterface::StringVector strings;

    sli->GetStrings( strings );

    std::cout << "{" << std::endl;
    {
      for( auto s : strings )
        std::cout << "'" << s << "'" << std::endl;
    }
    std::cout << "}"<< std::endl;
  }
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT( otb::Wrapper::TestApplication )
