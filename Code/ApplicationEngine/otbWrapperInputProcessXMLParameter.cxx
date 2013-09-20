
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
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperComplexInputImageParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperComplexOutputImageParameter.h"
#include "otbWrapperRAMParameter.h"

#include "itksys/SystemTools.hxx"

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

std::string
InputProcessXMLParameter::pixelTypeToString(ImagePixelType pixType)
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
    }
  return type;
}

const std::string InputProcessXMLParameter::GetChildNodeTextOf(TiXmlElement *parentElement, std::string key)
{
  std::string value="";
  
  if(parentElement)
    {
      TiXmlElement* childElement = 0;
      childElement = parentElement->FirstChildElement(key.c_str());
    
      //same as childElement->GetText() does but that call is failing if there is no such node.
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

void
InputProcessXMLParameter::otbAppLogInfo(Application::Pointer app, std::string info)
{
  app->GetLogger()->Write(itk::LoggerBase::INFO, info );
}

int
InputProcessXMLParameter::Read(Application::Pointer app)
{
  // Check if the filename is not empty
  if(m_FileName.empty())
    itkExceptionMacro(<<"The XML input FileName is empty, please set the filename via the method SetFileName");

  // Check that the right extension is given : expected .xml */
  if (itksys::SystemTools::GetFilenameLastExtension(m_FileName) != ".xml")
    {
    itkExceptionMacro(<<itksys::SystemTools::GetFilenameLastExtension(m_FileName)
                      <<" is a wrong Extension FileName : Expected .xml");
    }

  // Open the xml file
  TiXmlDocument doc(m_FileName.c_str());
  if (!doc.LoadFile())
    {
    itkExceptionMacro(<<"Can't open file "<<m_FileName);
    }
  
  TiXmlHandle handle(&doc);

  TiXmlElement *n_OTB;
  n_OTB = handle.FirstChild("OTB").Element();
 
  if(!n_OTB)
  {
    std::string info = "Input XML file " + std::string(this->GetFileName()) + " is invalid.";
    this->otbAppLogInfo(app,info);
  }

  std::string otb_Version, otb_Build, otb_Platform;
  otb_Version = GetChildNodeTextOf(n_OTB,"version");
  otb_Build = GetChildNodeTextOf(n_OTB, "build");
  otb_Platform = GetChildNodeTextOf(n_OTB, "platform");
  
  TiXmlElement* n_App    = n_OTB->FirstChildElement("application");
  std::string app_Name, app_Descr;
  app_Name = GetChildNodeTextOf(n_App, "name");
  app_Descr = GetChildNodeTextOf(n_App, "descr");
  
  TiXmlElement* n_Doc    = n_App->FirstChildElement("doc");
  std::string doc_Name, doc_Descr, doc_Author, doc_Limitation, doc_Related;
  doc_Name = GetChildNodeTextOf(n_Doc, "name");
  doc_Descr = GetChildNodeTextOf(n_Doc, "descr");
  doc_Author = GetChildNodeTextOf(n_Doc, "author");
  doc_Limitation = GetChildNodeTextOf(n_Doc, "limitation");
  doc_Related = GetChildNodeTextOf(n_Doc, "related");

  TiXmlElement* n_Tags    = n_Doc->FirstChildElement("tags");
  std::vector<std::string> doc_TagList;

  //GetChildNodeTextOf(n_Tags, "tag");
 
  /*
  FIXME removed temporarily
  this->otbAppLogInfo(app, "Application Name : " + app_Name);
  this->otbAppLogInfo(app, "Description      : " + app_Descr);
  this->otbAppLogInfo(app, "OTB Version      : " + otb_Version);
  this->otbAppLogInfo(app, "Operating system : " + otb_Platform);

  this->otbAppLogInfo(app, "Doc Name        : " + doc_Name);
  this->otbAppLogInfo(app, "Doc Description : " + doc_Descr);
  this->otbAppLogInfo(app, "Author          : " + doc_Author);
  this->otbAppLogInfo(app, "Limitation      : " + doc_Limitation);
  this->otbAppLogInfo(app, "Related         : " + doc_Related);
  */

  if(app->GetName() != app_Name)
    {
      //hopefully shouldn't reach here ...
      itkExceptionMacro( << "Input XML was generated for a different application( " <<
                      app_Name << ") while application loaded is:" << app->GetName());
      return -1;
    }
  
  // Iterate through the tree to get all the stats
  for( TiXmlElement* n_Parameter = n_App->FirstChildElement("parameter"); n_Parameter != NULL;
       n_Parameter = n_Parameter->NextSiblingElement() )
    {
      std::string key, type, value;
      std::vector<std::string> values;
      key = GetChildNodeTextOf(n_Parameter, "key");
      type = GetChildNodeTextOf(n_Parameter, "type");
      value = GetChildNodeTextOf(n_Parameter, "value");
            
      TiXmlElement* n_Values = NULL;
      n_Values = n_Parameter->FirstChildElement("values");
      if(n_Values)
       {
         for(TiXmlElement* n_Value = n_Values->FirstChildElement("value"); n_Value != NULL;
             n_Value = n_Value->NextSiblingElement())
           {
             values.push_back(n_Value->GetText());
           }
       }
      if ( type == "InputFilename" || type == "OutputFilename" ||
          type == "Directory" ||  type == "InputImage" ||
          type == "ComplexInputImage" || type == "InputVectorData" ||
          type == "OutputImage" || type == "ComplexOutputImage" ||
          type ==  "OutputVectorData" || type == "String" ||
          type == "Choice")
       {
         if(app->IsUseXMLValue(key))
           {
             app->SetParameterString(key, value);
           }
         else
           {
             std::string userValue = app->GetParameterString(key);
             if(!userValue.empty())
              {
                app->SetParameterString(key, userValue);
              }
           }
       }
      else if (type == "Radius" || type == "Int" || type == "RAM" || type == "rand" )
       {
         int intValue;
         std::stringstream(value) >> intValue;
         if(app->IsUseXMLValue(key))
           {
             app->SetParameterInt(key, intValue);
           }
         else
           {
             int userValue = 0;
             userValue = app->GetParameterInt(key);
             if(userValue != 0)
              {
                app->SetParameterInt(key,userValue);
              }
           }
       }
      else if (type == "Float")
        {
         float floatValue;
         std::stringstream(value) >> floatValue;
         if(app->IsUseXMLValue(key))
           {
             app->SetParameterFloat(key, floatValue);
           }
         else
           {
             float userValue = 0;
             userValue = app->GetParameterFloat(key);
             if(userValue != 0)
              {
                app->SetParameterFloat(key,userValue);
              }
           }
        }
      else if (type == "Empty")
        {
         bool emptyValue = false;
         // std::stringstream(value) >> floatValue;
         if( value == "true")
           {
             emptyValue = true;
           }
         if(app->IsUseXMLValue(key))
           {
             app->SetParameterEmpty(key, emptyValue);
           }
         else
           {
             bool userValue = app->GetParameterEmpty(key);
             app->SetParameterEmpty(key,userValue);
           }
        }
      else if (type == "InputFilenameList" || type == "InputImageList" ||
              type =="InputVectorDataList" || type == "StringList" ||
              type == "ListView")
        {
         if(values.empty())
           itkWarningMacro(<< key << " has null values");
         
         if(app->IsUseXMLValue(key))
           {
             app->SetParameterStringList(key, values);
           }
         else
           {
             std::vector<std::string> userValues;
             userValues = app->GetParameterStringList(key);
             
             if(userValues.size() != 0)
              {
                app->SetParameterStringList(key,userValues);
              }
           }
         //skip = (skip == true) ? true: ( (userValues.size() == 0) ? true : false )
       }
      //choice also comes as setint and setstring why??
    }
  return 0;
}
} //end namespace wrapper
  
} //end namespace otb
