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

#include "otbWrapperInputProcessXMLParameter.h"

#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperComplexInputImageParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperComplexOutputImageParameter.h"
#include "otbWrapperRAMParameter.h"
#include "itksys/SystemTools.hxx"
#include "otbMacro.h"

namespace otb
{
namespace Wrapper
{
InputProcessXMLParameter::InputProcessXMLParameter()
{
  this->SetKey("inxml");
  this->SetName("Load otb application from xml file");
  this->SetDescription("Load otb application from xml file");
  this->SetMandatory(false);
  this->SetActive(false);
  this->SetRole(Role_Input);
}

InputProcessXMLParameter::~InputProcessXMLParameter()
{

}

bool
InputProcessXMLParameter::SetFileName(std::string value)
{
  // Check if the filename is not empty
  if(!value.empty())
    {
    // Check that the right extension is given : expected .xml
    if (itksys::SystemTools::GetFilenameLastExtension(value) == ".xml")
      {
      if (itksys::SystemTools::FileExists(value,true))
        {
        this->SetValue(value);
        return true;
        }
      }
    }
  return false;
}

void
InputProcessXMLParameter::SetValue(const std::string value)
{
  itkDebugMacro("setting member m_FileName to " << value);
  this->m_FileName = value;
  SetActive(true);
  this->Modified();
}

ImagePixelType
InputProcessXMLParameter::GetPixelTypeFromString(std::string strType)
{
  if(strType == "uint8")
    {
    return ImagePixelType_uint8;
    }
  else if (strType == "int16")
    {
    return ImagePixelType_int16;
    }
  else if (strType == "uint16")
    {
    return ImagePixelType_uint16;
    }
  else if(strType == "int32")
    {
    return ImagePixelType_int32;
    }
  else if(strType == "uint32")
    {
    return ImagePixelType_uint32;
    }
  else if(strType == "float")
    {
    return ImagePixelType_float;
    }
  else if(strType == "double")
    {
    return ImagePixelType_double;
    }
  else
    {
    /*by default return float. Eg: if strType is empty because of no pixtype
    node in xml which was the case earlier
    */
    return ImagePixelType_float;
    }
}

void
InputProcessXMLParameter::otbAppLogInfo(Application::Pointer app, std::string info)
{
  app->GetLogger()->Write(itk::LoggerBase::INFO, info );
}

/* copied from Utilities/tinyXMLlib/tinyxml.cpp. Must have a FIX inside tinyxml.cpp */
FILE*
InputProcessXMLParameter::TiXmlFOpen( const char* filename, const char* mode )
{
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
  FILE* fp = 0;
  errno_t err = fopen_s( &fp, filename, mode );
  if ( !err && fp )
    return fp;
  return 0;
  #else
  return fopen( filename, mode );
  #endif
}

const std::string
InputProcessXMLParameter::GetChildNodeTextOf(TiXmlElement *parentElement, std::string key)
{
  std::string value="";

  if(parentElement)
    {
    TiXmlElement* childElement = nullptr;
    childElement = parentElement->FirstChildElement(key.c_str());

    //same as childElement->GetText() does but that call is failing if there is
    //no such node.
    //but the below code works and is a replacement for GetText()
    if(childElement)
      {
      const TiXmlNode* child = childElement->FirstChild();
      if ( child )
        {
        const TiXmlText* childText = child->ToText();
        if ( childText )
          {
          value = childText->Value();
          }
        }
      }
    }
  return value;
}

int
InputProcessXMLParameter::Read(Application::Pointer this_)
{
  // Open the xml file
  TiXmlDocument doc;

  //Use itksys::SystemTools::FOpen() and close it below because
  //TiXmlDocument::TiXmlFileOpen( ) is not exposed from tinyXML library. Even
  //though its available in the TiXmlDocument::SaveFile().
  FILE* fp =  itksys::SystemTools::Fopen(m_FileName, "rb");

  if (!doc.LoadFile(fp , TIXML_ENCODING_UTF8))
    {
    fclose(fp);
    itkExceptionMacro(<< "Can't open file " << m_FileName);
    }

  TiXmlHandle handle(&doc);

  TiXmlElement *n_OTB;
  n_OTB = handle.FirstChild("OTB").Element();

  if(!n_OTB)
  {
    std::string info = "Input XML file " + std::string(this->GetFileName()) + " is invalid.";
    //this->otbAppLogInfo(app,info);
  }

  std::string otb_Version;
  otb_Version = GetChildNodeTextOf(n_OTB,"version");

  if(otb_Version != OTB_VERSION_STRING)
    otbMsgDebugMacro( << "Input XML was generated with a different version of OTB (" <<
                       otb_Version << ") and current version is OTB (" << OTB_VERSION_STRING << ")" );

  /*
  std::string otb_Version, otb_Build, otb_Platform;
  otb_Version = this_->GetChildNodeTextOf(n_OTB,"version");
  otb_Build = GetChildNodeTextOf(n_OTB, "build");
  otb_Platform = this_->GetChildNodeTextOf(n_OTB, "platform");
  */

  int ret = 0;

  TiXmlElement *n_AppNode   = n_OTB->FirstChildElement("application");

  std::string app_Name;
  app_Name = GetChildNodeTextOf(n_AppNode, "name");
  /*
  AddMetaData("appname", app_Name);

  app_Descr = this_->GetChildNodeTextOf(n_AppNode, "descr");
  AddMetaData("appdescr", app_Descr);

  TiXmlElement* n_Doc    = n_AppNode->FirstChildElement("doc");

  std::string doc_Name, doc_Descr, doc_Author, doc_Limitation, doc_SeeAlso;

  doc_Name = this_->GetChildNodeTextOf(n_Doc, "name");
  AddMetaData("docname", doc_Name);

  doc_Descr = this_->GetChildNodeTextOf(n_Doc, "longdescr");
  AddMetaData("doclongdescr", doc_Descr);

  doc_Author = this_->GetChildNodeTextOf(n_Doc, "authors");
  AddMetaData("docauthors", doc_Author);

  doc_Limitation = this_->GetChildNodeTextOf(n_Doc, "limitations");
  AddMetaData("doclimitations", doc_Limitation);

  doc_SeeAlso = this_->GetChildNodeTextOf(n_Doc, "seealso");
  AddMetaData("docseealso", doc_SeeAlso);
  */


  if(this_->GetName() != app_Name)
    {
      //hopefully shouldn't reach here ...
    /*
    std::string message = "Input XML was generated for a different application( "
        + app_Name + ") while application loaded is:" + this_->GetName();
    */
    itkWarningMacro( << "Input XML was generated for a different application( " <<
                       app_Name << ") while application loaded is:" <<this_->GetName());

              fclose(fp);
    return -1;
    }

  ParameterGroup::Pointer paramGroup = this_->GetParameterList();

  // Iterate through the parameter list
  for( TiXmlElement* n_Parameter = n_AppNode->FirstChildElement("parameter"); n_Parameter != nullptr;
       n_Parameter = n_Parameter->NextSiblingElement() )
    {
    std::string key,typeAsString, value, paramName;
    std::vector<std::string> values;
    key = GetChildNodeTextOf(n_Parameter, "key");
    typeAsString = GetChildNodeTextOf(n_Parameter, "type");
    value = GetChildNodeTextOf(n_Parameter, "value");
    paramName = GetChildNodeTextOf(n_Parameter, "name");
    ParameterType type = paramGroup->GetParameterTypeFromString(typeAsString);

    Parameter* param = this_->GetParameterByKey(key);

    // std::cout
    //   << "'" << paramName << "'\t"
    //   << key << "\t"
    //   << typeAsString << "\t"
    //   << type << "\t"
    //   << ParameterType_OutputImage << "\t"
    //   << param->HasUserValue() << "\t"
    //   << value << std::endl;

    param->SetUserValue(true);
    param->SetActive(true);

    TiXmlElement* n_Values = nullptr;
    n_Values = n_Parameter->FirstChildElement("values");
    if(n_Values)
      {
      for(TiXmlElement* n_Value = n_Values->FirstChildElement("value"); n_Value != nullptr;
	  n_Value = n_Value->NextSiblingElement())
	{
	values.push_back(n_Value->GetText());
	}
      }

    if (type == ParameterType_OutputFilename || type == ParameterType_OutputVectorData ||
        type == ParameterType_String || type == ParameterType_Choice ||
        type == ParameterType_RAM || type == ParameterType_Bool)
      {
      this_->SetParameterString(key, value);
      }
    else if( type == ParameterType_OutputImage )
      {
      assert( dynamic_cast< OutputImageParameter * >( param )==param );

      OutputImageParameter * outImageParam =
	dynamic_cast< OutputImageParameter * >( param );

      assert( outImageParam!=nullptr );

      outImageParam->SetFileName( value );

      std::string pixelType(
	GetChildNodeTextOf(n_Parameter, "pixtype" )
      );

      if( pixelType.empty() )
	std::runtime_error( "Invalid pixel type (empty string)." );

      outImageParam->SetPixelType(
	GetPixelTypeFromString( pixelType )
      );
      }
    else if (type == ParameterType_ComplexOutputImage)
      {
      ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*>(param);
      if(paramDown!=nullptr)
	paramDown->SetFileName(value);
      }
    else if (type == ParameterType_Directory)
      {
      DirectoryParameter* paramDown = dynamic_cast<DirectoryParameter*>(param);
      if(paramDown!=nullptr)
	paramDown->SetValue(value);
      }
    else if (type == ParameterType_InputFilename)
      {
      InputFilenameParameter* paramDown = dynamic_cast<InputFilenameParameter*>(param);
      if(paramDown!=nullptr)
	paramDown->SetValue(value);
      }
    else if (type == ParameterType_InputImage)
      {
      if(itksys::SystemTools::FileExists(value))
	{
	InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
	if(paramDown!=nullptr)
	  {
	  paramDown->SetFromFileName(value);
	  if (!paramDown->SetFromFileName(value))
	    {
	    ret= -1;
	    }
	  }
	}
      else
	{
	otbMsgDevMacro( << "InputImageFile saved in InputXML does not exists" );
	}
      }
    else if (type == ParameterType_ComplexInputImage)
      {
      if(itksys::SystemTools::FileExists(value))
	{
	ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*>(param);
	if(paramDown!=nullptr)
	  paramDown->SetFromFileName(value);
	}
      }
    else if (dynamic_cast<InputVectorDataParameter*>(param))
      {
      if(itksys::SystemTools::FileExists(value))
	{
	InputVectorDataParameter* paramDown = dynamic_cast<InputVectorDataParameter*>(param);
	if ( !paramDown->SetFromFileName(value) )
	  {
	  ret = -1;
	  }
	}
      }
    else if (dynamic_cast<InputImageListParameter*>(param))
      {
      InputImageListParameter* paramDown = dynamic_cast<InputImageListParameter*>(param);
      paramDown->SetListFromFileName(values);
      }
    else if (dynamic_cast<InputVectorDataListParameter*>(param))
      {
      InputVectorDataListParameter* paramDown = dynamic_cast<InputVectorDataListParameter*>(param);
      paramDown->SetListFromFileName( values );
      }
    else if (dynamic_cast<InputFilenameListParameter*>(param))
      {
      InputFilenameListParameter* paramDown = dynamic_cast<InputFilenameListParameter*>(param);
      paramDown->SetListFromFileName( values );
      }
    else if (type == ParameterType_Radius || type == ParameterType_Int ||
	     typeAsString == "rand" )
      {
      int intValue;
      std::stringstream(value) >> intValue;
      this_->SetParameterInt(key, intValue);
      }
    else if (type == ParameterType_Float)
      {
      float floatValue;
      std::stringstream(value) >> floatValue;
      this_->SetParameterFloat(key, floatValue);
      }
    else if (type == ParameterType_Empty)
      {
      bool emptyValue = false;
      if( value == "true")
	{
	emptyValue = true;
	}
      this_->SetParameterEmpty(key, emptyValue);
      }
    else if (type == ParameterType_StringList || type == ParameterType_ListView)
      {
      if(values.empty())
	itkWarningMacro(<< key << " has null values");

      this_->SetParameterStringList(key, values);
      }

    // Call UpdateParameters after each parameter is set
    this_->UpdateParameters();
    } //end updateFromXML
  //choice also comes as setint and setstring why??

  ret = 0; //resetting return to zero, we don't use it anyway for now.

  fclose(fp);

  return ret;
}


} //end namespace wrapper

} //end namespace otb
