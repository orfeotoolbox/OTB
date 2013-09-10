
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

      if(childElement)
	{
	  value = childElement->GetText();
	}
      // value = parent->FirstChildElement(key)->GetText();
    }
  return value;
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
    itkExceptionMacro( << "Input XML file " << this->GetFileName() << " is invalid.");

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

  //  GetChildNodeTextOf(n_Tags, "tag");
 
  /*
  app->otbAppLogINFO(<< "Application Name : " << app_Name );
  otbAppLogINFO(<< "Description      : " << app_Descr )
  otbAppLogINFO(<< "OTB Version      : " << otb_Version);
  otbAppLogINFO(<< "Operating system : " << otb_Platform);

  otbAppLogINFO(<< "Doc Name        : " << doc_Name );
  otbAppLogINFO(<< "Doc Description : " << doc_Descr )
  otbAppLogINFO(<< "Author          : " << doc_Author);
  otbAppLogINFO(<< "Limitation      : " << doc_Limitation);
  otbAppLogINFO(<< "Related         : " << doc_Related);
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
	  std::string userValue = app->GetParameterString(key);
	  if(userValue.empty())
	    app->SetParameterString(key, value);
        }
      else if (type == "Radius" || type == "Int" || type == "RAM" )
        {
	  int intValue;
	  std::stringstream(value) >> intValue; 
	  int userValue = 0;
	  userValue = app->GetParameterInt(key);
	  if(userValue == 0)
	    app->SetParameterInt(key,intValue);
        }
      else if (type == "Float")
        {
	  int floatValue;
	  std::stringstream(value) >> floatValue; 
	  float userValue = 0;
	  userValue = app->GetParameterFloat(key);
	  if( userValue == 0 )
	    app->SetParameterFloat(key, floatValue);
        }
      else if (type == "InputFilenameList" || type == "InputImageList" ||
	       type =="InputVectorDataList" || type == "StringList" || 
	       type == "ListView")
        {
	  if(values.empty())
	    itkExceptionMacro(<< key << " has null values");

	  std::vector<std::string> userValues;
	  userValues = app->GetParameterStringList(key);

    //skip = (skip == true) ? true: ( (userValues.size() == 0) ? true : false )
	  if( userValues.size() == 0 )
    {
	    app->SetParameterStringList(key,values);
    }
  }
      //choice also comes as setint and setstring why??
    }
    return 0;
  }
} //end namespace wrapper

} //end namespace otb
