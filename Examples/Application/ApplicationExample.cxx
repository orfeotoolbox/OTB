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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {QB_Suburb.png}
//    OUTPUTS: {ApplicationExample.png}
//  Software Guide : EndCommandLineArgs


//  Software Guide : BeginLatex
// This example illustrates the creation of an application.
// A new application is a class, which derives from \doxygen{otb}{Wrapper::Application} class.
// We start by including the needed header files.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
//  Software Guide : EndCodeSnippet

namespace otb
{

//  Software Guide : BeginLatex
//  Application class is defined in Wrapper namespace.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
namespace Wrapper
{
//  Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  ExampleApplication class is derived from Application class.
//
//  Software Guide : EndLatex

//  Software Guide : BeginCodeSnippet
class ExampleApplication : public Application
//  Software Guide : EndCodeSnippet
{
public:
  //  Software Guide : BeginLatex
  // Class declaration is followed by \code{ITK} public types for the class, the superclass and
  // smart pointers.
  // Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  typedef ExampleApplication Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //  Following macros are necessary to respect ITK object factory mechanisms. Please report
  //  to \ref{sec:FilterConventions} for additional information.
  //  Software Guide : EndLatex

  //  Software Guide : BeginCodeSnippet
  itkNewMacro(Self)
  ;

  itkTypeMacro(ExampleApplication, otb::Application)
  ;
  //  Software Guide : EndCodeSnippet


private:
  //  Software Guide : BeginLatex
  //  \doxygen{otb}{Application} relies on three main private methods: \code{DoInit()}, \code{DoUpdate()}, and \code{DoExecute()}.
  //  Section \ref{sec:appArchitecture} gives a description a these methods.
  //  Software Guide : EndLatex


  //  Software Guide : BeginLatex
  //  \code{DoInit()} method contains class information and description, parameter set up, and example values.
  //  Software Guide : EndLatex

  void DoInit()
  {

    // Software Guide : BeginLatex
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
    // Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    SetName("Example");
    SetDescription("This application opens in image and save it. "
      "(it includes Latex snippets in order to generate "
      "software guide documentation)");

    SetDocName("Example");
    SetDocLongDescription("The purpose of this application is "
      "to present parameters types,"
      " and Application class framework. "
      "It is used to generate Software guide documentation"
      " for Application chapter example.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
    //  Software Guide : EndCodeSnippet


    // Software Guide : BeginLatex
    // \code{AddDocTag()} method categorize the application using relevant tags.
    // \code{Code/ApplicationEngine/otbWrapperTags.h} contains some predefined tags defined in \code{Tags} namespace.
    // Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    AddDocTag(Tags::Analysis);
    AddDocTag("Test");
    //  Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    // Application parameters declaration is done using \code{AddParameter()} method.
    // \code{AddParameter()} requires Parameter type, its name and description.
    // \doxygen{otb}{Wrapper::Application} class contains methods to set parameters characteristics.
    // Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    AddParameter(ParameterType_Empty, "boolean", "Boolean");
    MandatoryOff("boolean");
    AddParameter(ParameterType_Int, "int", "Integer");
    MandatoryOff("int");
    SetDefaultParameterInt("int", 1);
    SetMinimumParameterIntValue("int", 0);
    SetMaximumParameterIntValue("int", 10);
    AddParameter(ParameterType_Float, "float", "Float");
    MandatoryOff("float");
    SetDefaultParameterFloat("float", 0.2);
    SetMinimumParameterFloatValue("float", -1.0);
    SetMaximumParameterFloatValue("float", 15.0);
    AddParameter(ParameterType_String, "string", "String");
    MandatoryOff("string");
    AddParameter(ParameterType_Filename, "filename", "File name");
    MandatoryOff("filename");
    AddParameter(ParameterType_Directory, "directory", "Directory name");
    MandatoryOff("directory");

    AddParameter(ParameterType_Choice, "choice", "Choice");
    AddChoice("choice.choice1", "Choice 1");
    AddChoice("choice.choice2", "Choice 2");
    AddChoice("choice.choice3", "Choice 3");
    AddParameter(ParameterType_Float, "choice.choice1.floatchoice1"
                 , "Float of choice1");
    SetDefaultParameterFloat("choice.choice1.floatchoice1", 0.125);
    AddParameter(ParameterType_Float, "choice.choice3.floatchoice3"
                 , "Float of choice3");
    SetDefaultParameterFloat("choice.choice3.floatchoice3", 5.0);

    AddParameter(ParameterType_Group, "ingroup", "Input Group");
    MandatoryOff("ingroup");
    AddParameter(ParameterType_Int, "ingroup.integer", "Integer of Group");
    MandatoryOff("ingroup.integer");
    AddParameter(ParameterType_Group, "ingroup.images", "Input Images Group");
    AddParameter(ParameterType_InputImage, "ingroup.images.inputimage"
                 , "Input Image");
    MandatoryOff("ingroup.images.inputimage");
    AddParameter(ParameterType_Group, "outgroup", "Output Group");
    MandatoryOff("outgroup");
    AddParameter(ParameterType_OutputImage, "outgroup.outputimage"
                 , "Output Image");
    MandatoryOff("outgroup.outputimage");
    AddParameter(ParameterType_InputImageList, "il", "Input image list");
    MandatoryOff("il");

    AddParameter(ParameterType_ListView, "cl", "Output Image channels");
    AddChoice("cl.choice1", "cl.choice1");
    AddChoice("cl.choice2", "cl.choice2");
    MandatoryOff("cl");

    AddParameter(ParameterType_RAM, "ram", "Available RAM");
    SetDefaultParameterInt("ram", 256);
    MandatoryOff("ram");

    AddParameter(ParameterType_ComplexInputImage, "cin", "Input Complex Image");
    AddParameter(ParameterType_ComplexOutputImage, "cout", "Output Complex Image");
    MandatoryOff("cin");
    MandatoryOff("cout");
    //  Software Guide : EndCodeSnippet

    // Software Guide : BeginLatex
    // An example commandline is automatically generated. Method \code{SetDocExampleParameterValue()} is
    // used to set parameters. Dataset should be located in  \code{OTB-Data/Examples} directory.
    // Software Guide : EndLatex

    //  Software Guide : BeginCodeSnippet
    SetDocExampleParameterValue("boolean", "true");
    SetDocExampleParameterValue("in", "QB_Suburb.png");
    SetDocExampleParameterValue("out", "Application_Example.png");
    //  Software Guide : EndCodeSnippet
  }

  // Software Guide : BeginLatex
  // \code{DoUpdateParameters()} is called as soon as a parameter value change. Section \ref{sec:appDoUpdateParameters}
  // gives a complete description of this method.
  // Software Guide : EndLatex
  //  Software Guide :BeginCodeSnippet
  void DoUpdateParameters()
  {
  }
  //  Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // \code{DoExecute()} contains the application core. Section \ref{sec:appDoExecute}
  // gives a complete description of this method.
  // Software Guide : EndLatex
  //  Software Guide :BeginCodeSnippet
  void DoExecute()
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    int paramInt = GetParameterInt("int");
    otbAppLogDEBUG( << paramInt <<std::endl );
    int paramFloat = GetParameterFloat("float");
    otbAppLogINFO( << paramFloat );

    SetParameterOutputImage("out", inImage);
  }
  //  Software Guide :EndCodeSnippet


};
}
}

// Software Guide : BeginLatex
// Finally \code{OTB\_APPLICATION\_EXPORT} is called.
// Software Guide : EndLatex
//  Software Guide :BeginCodeSnippet
OTB_APPLICATION_EXPORT(otb::Wrapper::ExampleApplication)
//  Software Guide :EndCodeSnippet







