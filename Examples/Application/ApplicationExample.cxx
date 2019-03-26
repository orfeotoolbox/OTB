/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

// This example illustrates the creation of an application.
// A new application is a class, which derives from \subdoxygen{otb}{Wrapper}{Application} class.
// We start by including the needed header files.

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{

//  Application class is defined in Wrapper namespace.

namespace Wrapper
{


//  ExampleApplication class is derived from Application class.

class ApplicationExample : public Application
{
public:
  // Class declaration is followed by \code{ITK} public types for the class, the superclass and
  // smart pointers.

  typedef ApplicationExample            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  //  Following macros are necessary to respect ITK object factory mechanisms. Please report
  //  to \ref{sec:FilterConventions} for additional information.

  itkNewMacro(Self);
  itkTypeMacro(ExampleApplication, otb::Application);


private:
  //  \doxygen{otb}{Application} relies on three main private methods: \code{DoInit()}, \code{DoUpdate()}, and \code{DoExecute()}.
  //  Section \ref{sec:appArchitecture} gives a description a these methods.


  //  \code{DoInit()} method contains class information and description, parameter set up, and example values.

  void DoInit() override
  {

    // Application name and description are set using following methods :
    // \begin{description}
    // \item[\code{SetName()}] Name of the application.
    // \item[\code{SetDescription()}] Set the short description of the class.
    // \item[\code{SetDocName()}] Set long name of the application (that can be displayed \dots).
    // \item[\code{SetDocLongDescription()}] This methods is used to describe the class.
    // \item[\code{SetDocLimitations()}] Set known limitations (threading, invalid pixel type \dots) or bugs.
    // \item[\code{SetDocAuthors()}] Set the application Authors. Author List. Format : "John Doe, Winnie the Pooh" \dots
    // \item[\code{SetDocSeeAlso()}] If the application is related to one another, it can be mentioned.
    // \end{description}

    SetName("Example");
    SetDescription(
        "This application opens an image and save it. "
        "Pay attention, it includes Latex snippets in order to generate "
        "software guide documentation");

    SetDocName("Example");
    SetDocLongDescription(
        "The purpose of this application is "
        "to present parameters types,"
        " and Application class framework. "
        "It is used to generate Software guide documentation"
        " for Application chapter example.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");


    // \code{AddDocTag()} method categorize the application using relevant tags.
    // The header file \code{otbWrapperTags.h} in OTB sources contains some predefined tags defined in \code{Tags} namespace.

    AddDocTag(Tags::Analysis);
    AddDocTag("Test");

    // Application parameters declaration is done using \code{AddParameter()} method.
    // \code{AddParameter()} requires the input parameter type
    // (ParameterType\_InputImage, ParameterType\_Int, ParameterType\_Float), its name and description.
    // \subdoxygen{otb}{Wrapper}{Application} class contains methods to set parameters characteristics.

    AddParameter(ParameterType_InputImage, "in", "Input Image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");

    AddParameter(ParameterType_Bool, "param1", "Example of boolean parameter");
    MandatoryOff("param1");

    AddParameter(ParameterType_Int, "param2", "Example of integer parameter");
    MandatoryOff("param2");
    SetDefaultParameterInt("param2", 1);
    SetMinimumParameterIntValue("param2", 0);
    SetMaximumParameterIntValue("param2", 10);

    AddParameter(ParameterType_Float, "param3", "Example of float parameter");
    MandatoryOff("param3");
    SetDefaultParameterFloat("param3", 0.2);
    SetMinimumParameterFloatValue("param3", -1.0);
    SetMaximumParameterFloatValue("param3", 15.0);

    AddParameter(ParameterType_String, "param4", "Example of string parameter");
    MandatoryOff("param4");

    AddParameter(ParameterType_InputFilename, "param5", "Example of filename");
    MandatoryOff("param5");

    AddParameter(ParameterType_Directory, "param6", "Example of directory name");
    MandatoryOff("param6");

    AddParameter(ParameterType_Choice, "inchoice", "Example of choice parameter");
    AddChoice("inchoice.choice1", "Choice 1");
    AddChoice("inchoice.choice2", "Choice 2");
    AddChoice("inchoice.choice3", "Choice 3");

    AddParameter(ParameterType_Float, "inchoice.choice1.floatchoice1", "Example of float parameter for choice1");
    SetDefaultParameterFloat("inchoice.choice1.floatchoice1", 0.125);

    AddParameter(ParameterType_Float, "inchoice.choice3.floatchoice3", "Example of float parameter for choice3");
    SetDefaultParameterFloat("inchoice.choice3.floatchoice3", 5.0);

    AddParameter(ParameterType_Group, "ingroup", "Input group");
    MandatoryOff("ingroup");
    AddParameter(ParameterType_Int, "ingroup.valint", "Example of integer parameter for group");
    MandatoryOff("ingroup.valint");
    AddParameter(ParameterType_Group, "ingroup.images", "Input Images group");
    AddParameter(ParameterType_InputImage, "ingroup.images.inputimage", "Input Image");
    MandatoryOff("ingroup.images.inputimage");

    AddParameter(ParameterType_Group, "outgroup", "Output group");
    MandatoryOff("outgroup");
    AddParameter(ParameterType_OutputImage, "outgroup.outputimage", "Output Image");
    MandatoryOff("outgroup.outputimage");
    AddParameter(ParameterType_InputImageList, "il", "Input image list");
    MandatoryOff("il");

    AddParameter(ParameterType_ListView, "cl", "Output image channels");
    AddChoice("cl.choice1", "cl.choice1");
    AddChoice("cl.choice2", "cl.choice2");
    MandatoryOff("cl");

    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");

    // An example of command-line is automatically generated. Method \code{SetDocExampleParameterValue()} is
    // used to set parameters. Dataset should be located in  \code{OTB-Data/Examples} directory.

    SetDocExampleParameterValue("boolean", "true");
    SetDocExampleParameterValue("in", "QB_Suburb.png");
    SetDocExampleParameterValue("out", "Application_Example.png");
  }

  // \code{DoUpdateParameters()} is called as soon as a parameter value change. Section \ref{sec:appDoUpdateParameters}
  // gives a complete description of this method.
  //  Software Guide :BeginCodeSnippet
  void DoUpdateParameters() override
  {
  }

  // \code{DoExecute()} contains the application core. Section \ref{sec:appDoExecute}
  // gives a complete description of this method.
  //  Software Guide :BeginCodeSnippet
  void DoExecute() override
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    int paramInt = GetParameterInt("param2");
    otbAppLogDEBUG(<< paramInt << std::endl);
    int paramFloat = GetParameterFloat("param3");
    otbAppLogINFO(<< paramFloat);

    SetParameterOutputImage("out", inImage);
  }
  //  Software Guide :EndCodeSnippet
};
} // namespace Wrapper
} // namespace otb

// Finally \code{OTB\_APPLICATION\_EXPORT} is called:
OTB_APPLICATION_EXPORT(otb::Wrapper::ApplicationExample)
