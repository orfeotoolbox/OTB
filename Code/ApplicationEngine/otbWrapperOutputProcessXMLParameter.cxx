
#include "otbWrapperOutputProcessXMLParameter.h"

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
OutputProcessXMLParameter::OutputProcessXMLParameter()
{
  this->SetKey("outxml");
  this->SetName("Save otb application to xml file");
  this->SetDescription("Save otb application to xml file");
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
    case ImagePixelType_int32:
    {
    type = "int32";
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

  std::string version = "3.18";
  std::string platform = "Linux";
  std::string build = "18-05-2013";

  AddChildNodeTo(n_OTB, "version", version);
  AddChildNodeTo(n_OTB, "build", build);
  AddChildNodeTo(n_OTB, "platform", platform);

  TiXmlElement *n_App;
  n_App = AddChildNodeTo(n_OTB, "application");
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
  ParameterGroup::Pointer paramGroup = app->GetParameterList();

  std::vector<std::string> paramList = paramGroup->GetParametersKeys(true);


  // Iterate through parameters
  for (std::vector<std::string>::const_iterator it = paramList.begin(); it!= paramList.end(); ++it)
    {
      std::string key = *it;
      Parameter *param = paramGroup->GetParameterByKey(key);
      std::string paramName = param->GetName();
      ParameterType type = app->GetParameterType(key);
      std::string typeAsString = paramGroup->GetParameterTypeAsString(type);

      // if param is a Group, dont do anything, ParamGroup dont have values
      if (type != ParameterType_Group)
      {
       bool paramExists = app->HasUserValue(key) && app->IsParameterEnabled(key);
       if ( key == "outxml" )
         {
           paramExists = false;
         }
       if (type == ParameterType_Empty && app->HasUserValue(key))
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
           value = imgParam->GetFileName();
           ImagePixelType pixType = imgParam->GetPixelType();
           pixelTypeAsString = pixelTypeToString(pixType); //FIXME save this
         }
         else if( type == ParameterType_InputImageList || type == ParameterType_InputFilenameList ||
                  type == ParameterType_InputVectorDataList || type == ParameterType_StringList )
           {
           values = app->GetParameterStringList(key);
           hasValueList = true;
           }
         else if (type == ParameterType_Int || type == ParameterType_Radius || type == ParameterType_RAM )
           {
           value = app->GetParameterAsString(key);
           }
         else if(type == ParameterType_Float)
           {
           std::ostringstream oss;
           oss << std::setprecision(10);
           oss << app->GetParameterFloat( key );
           value = oss.str();
           }
         else if ( type == ParameterType_String || type == ParameterType_InputFilename ||
                   type == ParameterType_Directory || type == ParameterType_InputImage ||
                   type == ParameterType_ComplexInputImage || type == ParameterType_InputVectorData ||
                   type == ParameterType_Choice || type == ParameterType_ListView ||
                   type == ParameterType_OutputVectorData || type == ParameterType_OutputFilename)
           {
           value = app->GetParameterString(key);
           }
         else if(key == "rand")
           {
           std::ostringstream strm;
           strm << app->GetParameterInt("rand");
           value = strm.str();
           }
         else if (typeAsString == "Empty")
           {
           EmptyParameter* eParam = dynamic_cast<EmptyParameter *> (param);
           value = "false";
           if( eParam->GetActive() )
             {
             value = "true";
             }
           }

         //get only file name
    /*
      if(type == ParameterType_InputFilename || type == ParameterType_InputImage ||
      type == ParameterType_ComplexInputImage || type == ParameterType_InputVectorData ||
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

         if(type == ParameterType_OutputImage || type == ParameterType_ComplexOutputImage)
           {
           AddChildNodeTo(n_Parameter, "type", pixelTypeAsString);
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
         n_App->LinkEndChild(n_Parameter);
       }
      }
    }

  // Finally, write xml contents to file
  doc.SaveFile( m_FileName.c_str() );

}

} //end namespace wrapper

} //end namespace otb
