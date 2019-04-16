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

#include "otbWrapperOutputProcessXMLParameter.h"

#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "itksys/SystemTools.hxx"

namespace otb
{
namespace Wrapper
{
OutputProcessXMLParameter::OutputProcessXMLParameter()
  : m_Node(nullptr)
  , m_Appli()
{
  this->SetKey("outxml");
  this->SetName("Save parameters to XML");
  this->SetDescription("Save application parameters to an XML file.");
  this->SetMandatory(false);
  this->SetActive(false);
  this->SetRole(Role_Output);
}

OutputProcessXMLParameter::~OutputProcessXMLParameter()
{

}

std::string
OutputProcessXMLParameter::pixelTypeToString(ImagePixelType pixType)
{
  std::string type;

  switch( pixType )
    {
    case ImagePixelType_uint8:
    {
    type = "uint8";
    break;
    }
    case ImagePixelType_int16:
    {
    type = "int16";
    break;
    }
    case ImagePixelType_uint16:
    {
    type = "uint16";
    break;
    }
    case ImagePixelType_int32:
    {
    type = "int32";
    break;
    }
    case ImagePixelType_uint32:
    {
    type = "uint32";
    break;
    }
    case ImagePixelType_float:
    {
    type = "float";
    break;
    }
    case ImagePixelType_double:
    {
    type = "double";
    break;
    }
    default:
    {
    type = "float";
    break;
    }
    }
  return type;
}

TiXmlElement* OutputProcessXMLParameter::AddChildNodeTo(TiXmlElement *parent, std::string name, std::string value)
{
  TiXmlElement * n_Node = new TiXmlElement( name.c_str() );
  parent->LinkEndChild( n_Node );
  if(!value.empty())
    {
      TiXmlText * nv_NodeValue = new TiXmlText( value.c_str() );
      n_Node->LinkEndChild( nv_NodeValue );
    }
  return n_Node;
}

void
OutputProcessXMLParameter::Write(Application::Pointer app)
{
  // Check if the filename is not empty
  if(m_FileName.empty())
    itkExceptionMacro("The XML output FileName is empty, please set the filename via the method SetFileName");

  // Check that the right extension is given : expected .xml */
  if (itksys::SystemTools::GetFilenameLastExtension(m_FileName) != ".xml")
    {
    itkExceptionMacro(<<itksys::SystemTools::GetFilenameLastExtension(m_FileName)
                      <<" is a wrong Extension FileName : Expected .xml");
    }

  // start creating XML file
  TiXmlDocument doc;

  TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
  doc.LinkEndChild( decl );

  TiXmlElement * n_OTB = new TiXmlElement( "OTB");
  doc.LinkEndChild( n_OTB );

  std::string version = OTB_VERSION_STRING;

  AddChildNodeTo(n_OTB, "version", version);

  // Parse application
  TiXmlElement *n_App = ParseApplication(app);
  n_OTB->LinkEndChild(n_App);

  // Finally, write xml contents to file
  doc.SaveFile( m_FileName.c_str() );
}

TiXmlElement*
OutputProcessXMLParameter::ParseApplication(Application::Pointer app)
{
  m_Appli = app;
  TiXmlElement * n_App = new TiXmlElement("application");
  m_Node = n_App;

  AddChildNodeTo(n_App, "name", app->GetName());
  AddChildNodeTo(n_App, "descr", app->GetDescription());


  TiXmlElement *n_AppDoc;
  n_AppDoc = AddChildNodeTo(n_App, "doc");
  AddChildNodeTo(n_AppDoc, "name", app->GetDocName());
  AddChildNodeTo(n_AppDoc, "longdescr", app->GetDocLongDescription());
  AddChildNodeTo(n_AppDoc, "authors", app->GetDocAuthors());
  AddChildNodeTo(n_AppDoc, "limitations", app->GetDocLimitations());
  AddChildNodeTo(n_AppDoc, "seealso", app->GetDocSeeAlso());

  TiXmlElement *n_DocTags;
  n_DocTags = AddChildNodeTo(n_AppDoc, "tags");
  std::vector<std::string> docTagList = app->GetDocTags();
  std::vector<std::string>::iterator tagIt;
  for(tagIt = docTagList.begin(); tagIt!= docTagList.end(); ++tagIt)
    {
      std::string tag = *tagIt;
      AddChildNodeTo(n_DocTags, "tag", tag);
    }

  // recursive call to ParseGroup(), starting with "" (i.e. GetParameterList())
  this->ParseGroup(std::string(""));

  // reset temporary members
  m_Appli = nullptr;
  m_Node = nullptr;
  return n_App;
}

void
OutputProcessXMLParameter::ParseGroup(const std::string& group)
{
  std::string prefix(group);
  ParameterGroup::Pointer paramGroup = m_Appli->GetParameterList();
  if (!group.empty())
    {
    prefix += '.';
    Parameter* rawParam = paramGroup->GetParameterByKey(group);
    ParameterGroup* rawParamAsGroup = dynamic_cast<ParameterGroup*>(rawParam);
    if (rawParamAsGroup)
      {
      paramGroup = rawParamAsGroup;
      }
    else
      {
      itkExceptionMacro("Function ParseGroup() expected a group parameter for key "<<group);
      }
    }

  std::vector<std::string> paramList = paramGroup->GetParametersKeys(false);

  // Iterate through parameters
  for (std::vector<std::string>::const_iterator it = paramList.begin(); it!= paramList.end(); ++it)
    {
      std::string key = prefix + *it;
      Parameter *param = paramGroup->GetParameterByKey(*it);
      std::string paramName = param->GetName();
      ParameterType type = m_Appli->GetParameterType(key);
      std::string typeAsString = paramGroup->GetParameterTypeAsString(type);

      // if param is a Group, inspect this group with a recursive call
      if (type == ParameterType_Group)
        {
        this->ParseGroup(key);
        }
      else
       {
       bool paramExists = m_Appli->HasUserValue(key) &&
                          m_Appli->IsParameterEnabled(key) &&
                          m_Appli->GetParameterRole(key) == Role_Input;
       if ( type == ParameterType_OutputProcessXML )
         {
           paramExists = false;
         }

        if(type  == ParameterType_RAM)
        {
          paramExists = true;
        }
       // if parameter doesn't have any value then skip it
       if (paramExists)
        {
         std::vector<std::string> values;
         std::string value;
         bool hasValueList = false;
         std::string pixelTypeAsString;
         if (type == ParameterType_OutputImage)
          {
           OutputImageParameter *imgParam = dynamic_cast<OutputImageParameter *>(param);
           
           if(imgParam!=nullptr)
             {
             value = imgParam->GetFileName();
             ImagePixelType pixType = imgParam->GetPixelType();
             pixelTypeAsString = pixelTypeToString(pixType);
             }
         }
         else if( type == ParameterType_InputImageList || type == ParameterType_InputFilenameList ||
                  type == ParameterType_InputVectorDataList || type == ParameterType_StringList ||
                  type == ParameterType_ListView )
           {
           values = m_Appli->GetParameterStringList(key);
           hasValueList = true;
           }
         else if (type == ParameterType_Int || type == ParameterType_Radius || type == ParameterType_RAM )
           {
           value = m_Appli->GetParameterAsString(key);
           }
         else if(type == ParameterType_Float)
           {
           std::ostringstream oss;
           oss << std::setprecision(std::numeric_limits<float>::digits10+1);
           oss << m_Appli->GetParameterFloat( key );
           value = oss.str();
           }
         else if ( type == ParameterType_String || type == ParameterType_InputFilename ||
                   type == ParameterType_Directory || type == ParameterType_InputImage ||
                   type == ParameterType_InputVectorData || type == ParameterType_Choice ||
                   type == ParameterType_OutputVectorData || type == ParameterType_OutputFilename ||
                   type == ParameterType_Bool)
           {
           value = m_Appli->GetParameterString(key);
           }
         else if(key == "rand")
           {
           std::ostringstream strm;
           strm << m_Appli->GetParameterInt("rand");
           value = strm.str();
           }
         else if (type == ParameterType_InputProcessXML)
           {
             continue;
           }
         //get only file name
    /*
      if(type == ParameterType_InputFilename || type == ParameterType_InputImage ||
      type == ParameterType_InputVectorData ||
      type == ParameterType_OutputVectorData || type == ParameterType_OutputFilename)
           {
           unsigned found = value.find_last_of("/\\");
           //std::cerr << " path: " << value.substr(0,found) << '\n';
           value = value.substr(found+1);
           }
           else
           if(type == ParameterType_InputImageList || type == ParameterType_InputFilenameList ||
           type == ParameterType_InputVectorDataList)
           {
           std::vector<std::string>::iterator strIt;
           for(strIt = values.begin(); strIt != values.end(); ++strIt)
                    {
                    std::string val = *strIt;
                    unsigned found = val.find_last_of("/\\");
                    *strIt = val.substr(found+1);
                    }
                    }
  */
         //parameter node in xml
         TiXmlElement * n_Parameter = new TiXmlElement("parameter");

         const char * mandatory = "false";

         if( param->GetMandatory() )
           mandatory = "true";

         n_Parameter->SetAttribute("mandatory", mandatory);

         //setting parameter key as child node in parameter
         AddChildNodeTo(n_Parameter, "key", key);
         AddChildNodeTo(n_Parameter, "type", typeAsString);
         AddChildNodeTo(n_Parameter, "name", paramName);

         if(type == ParameterType_OutputImage)
           {
           AddChildNodeTo(n_Parameter, "pixtype", pixelTypeAsString);
           }
         if(!hasValueList)
           {
             AddChildNodeTo(n_Parameter, "value", value);
           }
         else
           {
             TiXmlElement *n_Values = AddChildNodeTo(n_Parameter, "values");
             std::vector<std::string>::iterator strIt;
             for(strIt = values.begin(); strIt != values.end(); ++strIt)
              {
                AddChildNodeTo(n_Values, "value",*strIt);
              }
           }
         m_Node->LinkEndChild(n_Parameter);
         }
       // dig into Choice parameter
       if (type == ParameterType_Choice)
        {
        std::string choiceGroup(key);
        choiceGroup += '.';
        choiceGroup += m_Appli->GetParameterString(key);
        this->ParseGroup(choiceGroup);
        }
      }
    }
}

std::string OutputProcessXMLParameter::MakeCommandLine(Application::Pointer application)
{
  OutputProcessXMLParameter::Pointer outXMLParam = OutputProcessXMLParameter::New();
  TiXmlElement* XMLAppElement = outXMLParam->ParseApplication(application);

  // Create command line from the XML document
  TiXmlElement *     pName, *pParam;
  std::ostringstream cmdLine;

  cmdLine << "";

  if (XMLAppElement)
  {
    pName = XMLAppElement->FirstChildElement("name");

    cmdLine << "otbcli_" << pName->GetText();
#ifdef _WIN32
    cmdLine << ".bat";
#endif
    cmdLine << " ";

    // Parse application parameters
    pParam = XMLAppElement->FirstChildElement("parameter");

    while (pParam)
    {
      // Get parameter key
      cmdLine << "-";
      cmdLine << pParam->FirstChildElement("key")->GetText();
      cmdLine << " ";

      // Some parameters can have multiple values. Test it and handle this
      // specific case
      TiXmlElement* values = pParam->FirstChildElement("values");

      if (values)
      {
        // Loop over value
        TiXmlElement* pValue = values->FirstChildElement("value");
        while (pValue)
        {
          cmdLine << pValue->GetText();
          cmdLine << " ";

          pValue = pValue->NextSiblingElement(); // iteration over multiple values
        }
      }
      else
      {
        // Get parameter value
        cmdLine << pParam->FirstChildElement("value")->GetText();
        cmdLine << " ";

        // In case of OutputImageparameter we need to report output pixel type
        TiXmlElement* pPixType = pParam->FirstChildElement("pixtype");

        if (pPixType)
        {
          cmdLine << pPixType->GetText();
          cmdLine << " ";
        }
      }

      pParam = pParam->NextSiblingElement(); // iteration over parameters
    }
  }

  return cmdLine.str();
}


} //end namespace wrapper

} //end namespace otb
