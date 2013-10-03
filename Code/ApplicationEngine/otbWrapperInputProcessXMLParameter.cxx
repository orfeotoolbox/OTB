
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

void
InputProcessXMLParameter::otbAppLogInfo(Application::Pointer app, std::string info)
{
  app->GetLogger()->Write(itk::LoggerBase::INFO, info );
}

int
InputProcessXMLParameter::Read(Application::Pointer this_)
{

  // Check if the filename is not empty
  if(m_FileName.empty())
    itkExceptionMacro(<<"The XML input FileName is empty, please set the filename via the method SetFileName");

  // Check that the right extension is given : expected .xml
  if (itksys::SystemTools::GetFilenameLastExtension(m_FileName) != ".xml")
    {
    itkExceptionMacro(<<itksys::SystemTools::GetFilenameLastExtension(m_FileName)
                      <<" is a wrong Extension FileName : Expected .xml");
    }

  // Open the xml file
  TiXmlDocument doc;

  FILE* fp = TiXmlFOpen( m_FileName.c_str (), "rb" );

  if (!doc.LoadFile(fp , TIXML_ENCODING_UTF8))
    {
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

  /*
  std::string otb_Version, otb_Build, otb_Platform;
  otb_Version = this_->GetChildNodeTextOf(n_OTB,"version");
  otb_Build = GetChildNodeTextOf(n_OTB, "build");
  otb_Platform = this_->GetChildNodeTextOf(n_OTB, "platform");
  */

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
//      itkExceptionMacro( << "Input XML was generated for a different application( " <<
    //                  app_Name << ") while application loaded is:" <<this_->GetName());
  //    return -1;
    std::string message = "Input XML was generated for a different application( "
        + app_Name + ") while application loaded is:" + this_->GetName();
    std::cerr << message << "\n\n";
    return -1;
    }

  ParameterGroup::Pointer paramGroup = this_->GetParameterList();

  // Iterate through the parameter list
  for( TiXmlElement* n_Parameter = n_AppNode->FirstChildElement("parameter"); n_Parameter != NULL;
       n_Parameter = n_Parameter->NextSiblingElement() )
    {
    std::string key,typeAsString, value, paramName;
    std::vector<std::string> values;
    key = GetChildNodeTextOf(n_Parameter, "key");
    typeAsString = GetChildNodeTextOf(n_Parameter, "type");
    value = GetChildNodeTextOf(n_Parameter, "value");
    paramName = GetChildNodeTextOf(n_Parameter, "name");
    ParameterType type = paramGroup->GetParameterTypeFromString(typeAsString);

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
    if ( type == ParameterType_InputFilename || type == ParameterType_OutputFilename ||
         type == ParameterType_Directory ||  type == ParameterType_InputImage ||
         type == ParameterType_ComplexInputImage || type == ParameterType_InputVectorData ||
         type == ParameterType_OutputImage || type == ParameterType_ComplexOutputImage ||
         type == ParameterType_OutputVectorData || type == ParameterType_String ||
         type == ParameterType_Choice)
      {
      if(this_->IsUseXMLValue(key))
        {
        this_->SetParameterString(key, value);
        }
      else
        {
        std::string userValue = this_->GetParameterString(key);
        if(!userValue.empty())
          {
          this_->SetParameterString(key, userValue);
          }
        }
      }
    else if (type == ParameterType_Radius || type == ParameterType_Int ||
             type == ParameterType_RAM || typeAsString == "rand" )
      {
      int intValue;
      std::stringstream(value) >> intValue;
      if(this_->IsUseXMLValue(key))
        {
        this_->SetParameterInt(key, intValue);
        }
      else
        {
        int userValue = 0;
        userValue = this_->GetParameterInt(key);
        if(userValue != 0)
          {
          this_->SetParameterInt(key,userValue);
          }
        }
      }
    else if (type == ParameterType_Float)
      {
      float floatValue;
      std::stringstream(value) >> floatValue;
      if(this_->IsUseXMLValue(key))
        {
        this_->SetParameterFloat(key, floatValue);
        }
      else
        {
        float userValue = 0;
        userValue = this_->GetParameterFloat(key);
        if(userValue != 0)
          {
          this_->SetParameterFloat(key,userValue);
          }
        }
      }
    else if (type == ParameterType_Empty)
      {
      bool emptyValue = false;
      // std::stringstream(value) >> floatValue;
      if( value == "true")
        {
        emptyValue = true;
        }
      if(this_->IsUseXMLValue(key))
        {
        this_->SetParameterEmpty(key, emptyValue);
        }
      else
        {
        bool userValue = this_->GetParameterEmpty(key);
        this_->SetParameterEmpty(key,userValue);
        }
      }
    else if (type == ParameterType_InputFilenameList || type == ParameterType_InputImageList ||
             type == ParameterType_InputVectorDataList || type == ParameterType_StringList ||
             type == ParameterType_ListView)
      {
      if(values.empty())
        itkWarningMacro(<< key << " has null values");

      if(this_->IsUseXMLValue(key))
        {
        this_->SetParameterStringList(key, values);
        }
      else
        {
        std::vector<std::string> userValues;
        userValues = this_->GetParameterStringList(key);

        if(userValues.size() != 0)
          {

          this_->SetParameterStringList(key,userValues);
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
