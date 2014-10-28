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
#include "otbWrapperApplicationHtmlDocGenerator.h"

#include <stdio.h>
#include "otbMacro.h"
#include "otbWrapperChoiceParameter.h"

namespace otb
{
namespace Wrapper
{

#define otbDocHtmlTitleMacro( value )                                       \
  oss << "</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\"><p align=\"center\" style=\" margin-top:16px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:x-large; font-weight:600;\"><span style=\" font-size:x-large;\">"; \
  oss << value;                                                         \
  oss << "</span></p>";

#define otbDocHtmlTitle1Macro( value )                                  \
  oss << "<p style=\" margin-top:14px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:large; font-weight:600;\"><span style=\" font-size:large;\">"; \
  oss << value;                                                         \
  oss << "</span></p>";

#define otbDocHtmlTitle2Macro( value )                                  \
  oss << "<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:medium; font-weight:600;\"><span style=\" font-size:medium;\">"; \
  oss << value;                                                         \
  oss << "</span></p>";

#define otbDocHtmlBodyMacro( value )                                         \
  oss << "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"; \
  oss << value;                                                         \
  oss << "</p>";

#define otbDocHtmlBodyCodeMacro( value )                                         \
  oss << "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Courier New, courier';\">"; \
  oss << value;                                                         \
  oss << "</p>";

#define otbDocHtmlParamMacro( type, param, showKey )                             \
  oss << "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Courier New, courier'; font-weight:600;\"; >"; \
  oss << "["<<type<<"] "<< param->GetName();                            \
  if( showKey == true &&  !std::string(param->GetKey()).empty() )       \
    {                                                                   \
    oss << " (-"<< param->GetKey()<<")";                                \
    }                                                                   \
  oss << ": </span>";                                                   \
  if( std::string(param->GetDescription()).size()!=0 )                  \
    {                                                                   \
    oss << param->GetDescription();                                     \
    }                                                                   \
  oss << "</p>";


ApplicationHtmlDocGenerator::ApplicationHtmlDocGenerator()
{
}

ApplicationHtmlDocGenerator::~ApplicationHtmlDocGenerator()
{
}

void
ApplicationHtmlDocGenerator::GenerateDoc( const Application::Pointer app, std::string & val, const bool showKey )
{
  std::ostringstream oss;

  oss << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">";
  oss << "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">";
  oss << "p, li { white-space: pre-wrap; }";
  oss << "</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">";

  otbDocHtmlTitleMacro( app->GetDocName() );

  otbDocHtmlTitle1Macro( "Brief Description" );
  otbDocHtmlBodyMacro( app->GetDescription() );

  otbDocHtmlTitle1Macro( "Tags" );
  std::string tagList;
  if ( app->GetDocTags().size() > 0 )
    {
    for (unsigned int i = 0; i < app->GetDocTags().size() - 1; i++)
      {
      tagList.append( app->GetDocTags()[i] ).append(", ");
      }
    tagList.append( app->GetDocTags()[app->GetDocTags().size() - 1]);
    otbDocHtmlBodyMacro( tagList );
    }
  else
    {
    otbDocHtmlBodyMacro( "None" );
    }

  otbDocHtmlTitle1Macro("Long Description");
  otbDocHtmlBodyMacro( app->GetDocLongDescription() );

  otbDocHtmlTitle1Macro("Parameters");
  oss << "<ul>";
  std::string paramDocs("");
  ApplicationHtmlDocGenerator::GetDocParameters( app, paramDocs, showKey );
  oss<<paramDocs;
  oss<<"</ul>";

  otbDocHtmlTitle1Macro( "Limitations");
  otbDocHtmlBodyMacro( app->GetDocLimitations() );

  otbDocHtmlTitle1Macro( "Authors" );
  otbDocHtmlBodyMacro( app->GetDocAuthors() );

  otbDocHtmlTitle1Macro( "See also" );
  otbDocHtmlBodyMacro( app->GetDocSeeAlso() );

  otbDocHtmlTitle1Macro( "Example of use" );
  if( showKey == true )
    {
    oss << "<ul>";
    otbDocHtmlBodyMacro( "<li>Values for GUIs:</li>" );
    }
  otbDocHtmlBodyMacro( app->GetHtmlExample() );
  if( showKey == true )
    {
    otbDocHtmlBodyMacro( "<li>Command line to execute:</li>" );
    otbDocHtmlBodyCodeMacro( app->GetCLExample() );

    oss << "</ul>";
    }
  oss << "</body></html>";

  val = oss.str();

  // Replace "\n" string with <br/>
  itksys::SystemTools::ReplaceString( val, "\n", "<br/>");
}

void
ApplicationHtmlDocGenerator::GenerateDoc(const Application::Pointer app, const std::string& filename, const bool showKey)
{
  std::string doc;

  ApplicationHtmlDocGenerator::GenerateDoc( app, doc, showKey );

  std::ofstream ofs(filename.c_str());
  if (!ofs.is_open())
    {
    fprintf(stderr, "Error, can't open file");
    itkGenericExceptionMacro( << "Error, can't open file "<<filename<<".");
    }
  ofs << doc;
  ofs.close();
}

void ApplicationHtmlDocGenerator::GetDocParameters( const Application::Pointer app, std::string & val, const bool showKey)
{
  std::ostringstream oss;
  const std::vector<std::string> appKeyList = app->GetParametersKeys( false );
  const unsigned int nbOfParam = appKeyList.size();

  std::string paramDocs("");
  if( nbOfParam == 0)
    {
    val = "None";
    }
  else
    {
   for( unsigned int i=0; i<nbOfParam; i++ )
     {
     const std::string key(appKeyList[i]);
     Parameter::Pointer param =  app->GetParameterByKey( key );
     if( app->GetParameterType(key) ==  ParameterType_Group)
       {
       oss << "<li>";
       otbDocHtmlParamMacro( "group", param, showKey );
       std::string grDoc;
       ApplicationHtmlDocGenerator::GetDocParameterGroup( app, grDoc, key, showKey );
       oss<<grDoc;
       oss<<"</li><br />";
       }
     else if( app->GetParameterType(key) ==  ParameterType_Choice )
       {
       oss << "<li>";
       otbDocHtmlParamMacro( "choice", param, showKey );
       std::string grDoc;
       ApplicationHtmlDocGenerator::GetDocParameterChoice(app, grDoc, key, showKey);
       oss<<grDoc;
       oss<<"</li><br />";
       }
     else
       {
       oss << "<li>";
       otbDocHtmlParamMacro("param", param, showKey );
       oss << "</li>";
       }
     }
    }

   val = oss.str();
}

void ApplicationHtmlDocGenerator::GetDocParameterGroup( const Application::Pointer app, std::string & val, const std::string & key, const bool showKey )
{
  Parameter * paramGr  = app->GetParameterByKey( key );
  if( !dynamic_cast<ParameterGroup *>(paramGr))
    {
    itkGenericExceptionMacro("Invalid parameter type for key "<<key<<", wait for ParameterGroup...");
    }

  ParameterGroup * group = dynamic_cast<ParameterGroup *>(paramGr);
  const std::vector<std::string> appKeyList = group->GetParametersKeys( false );
  unsigned int nbOfParam = appKeyList.size();
  std::ostringstream oss;
  oss<<"<ul>";

  for( unsigned int i=0; i<nbOfParam; i++ )
    {
    const std::string fullKey(std::string(key).append(".").append(appKeyList[i]));
    Parameter::Pointer param =  app->GetParameterByKey( fullKey );
    if( app->GetParameterType(fullKey) ==  ParameterType_Group)
      {
      oss<<"<li>";
       otbDocHtmlParamMacro( "group", param, showKey );
      std::string grDoc;
      ApplicationHtmlDocGenerator::GetDocParameterGroup( app, grDoc, fullKey, showKey );
      oss<<grDoc;
      oss<<"</li>";
      }
    else if( app->GetParameterType(fullKey) ==  ParameterType_Choice )
      {
      oss<<"<li>";
      otbDocHtmlParamMacro( "choice", param, showKey );
      std::string grDoc;
      ApplicationHtmlDocGenerator::GetDocParameterChoice(app, grDoc, fullKey, showKey );
      oss<<grDoc;
      oss<<"</li>";
      }
    else
      {
      oss << "<li>";
      otbDocHtmlParamMacro( "param", param, showKey );
      oss <<"</li>";
      }
    }
  oss<<"</ul>";
  val.append(oss.str());
}


void ApplicationHtmlDocGenerator::GetDocParameterChoice( const Application::Pointer app, std::string & val, const std::string & key, const bool showKey )
{
  Parameter * paramCh  = app->GetParameterByKey( key );
  if( !dynamic_cast<ChoiceParameter *>(paramCh))
    {
    itkGenericExceptionMacro("Invalid parameter type for key "<<key<<", wait for ChoiceParameter...");
    }
  ChoiceParameter * choice = dynamic_cast<ChoiceParameter *>(paramCh);
  const std::vector<std::string> appKeyList = choice->GetChoiceKeys();
  unsigned int nbOfParam = choice->GetNbChoices();
  std::ostringstream oss;
  oss<<"<ul>";

  for( unsigned int i=0; i<nbOfParam; i++ )
    {
    const std::string fullKey(std::string(key).append(".").append(appKeyList[i]));
    ParameterGroup * group = choice->GetChoiceParameterGroupByIndex(i);
    std::string grDoc;

    oss << "<li>";
    otbDocHtmlParamMacro( "group", group, showKey );
    ApplicationHtmlDocGenerator::GetDocParameterGroup( app, grDoc, fullKey, showKey );
    oss<<grDoc;
    oss<<"</li>";
    }
  oss<<"</ul>";
  val.append(oss.str());
}

}
}

