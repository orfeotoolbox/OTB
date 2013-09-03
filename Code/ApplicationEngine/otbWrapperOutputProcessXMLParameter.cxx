
#include "otbWrapperOutputProcessXMLParameter.h"

#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"
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
  this->SetKey("xml");
  this->SetName("Save process to xml file");
  this->SetDescription("Save process to xml file");
  this->SetMandatory(false);
  this->SetActive(false);
  //  this->SetRole(Role_Input); //rashad FIXME
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

  
  // Write the XML file
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
  n_AppDoc = AddChildNodeTo(n_Application, "doc");
  AddChildNodeTo(n_AppDoc, "name", app->GetDocName());
  AddChildNodeTo(n_AppDoc, "descr", app->GetDocLongDescription());
  AddChildNodeTo(n_AppDoc, "author", app->GetDocAuthors());
  AddChildNodeTo(n_AppDoc, "limitation", app->GetDocLimitations());
  AddChildNodeTo(n_AppDoc, "related", app->GetSeeAlso());
  AddChildNodeTo(n_AppDoc, "tags", app->GetTags());

  //AddChildNodeTo(n_AppDoc, "example", app->GetDocExampleString());

  ParameterGroup::Pointer paramGroup = app->GetParameterList();

  std::vector<std::string> paramList = paramGroup->GetParametersKeys(true);

  
  // Iterate through parameters
  for (std::vector<std::string>::const_iterator it = paramList.begin(); it!= paramList.end(); ++it)
    {
      std::string key = *it;
      Parameter *param = paramGroup->GetParameterByKey(key);
      ParameterType type = app->GetParameterType(key);

      // if param is a Group, dont do anything, ParamGroup dont have values
      if (type != ParameterType_Group)
      {
	bool paramExists = app->HasValue(key);
	if ( key == "xml" )
	  paramExists = false;

	// if parameter doesn't have any value then skip it
       if (paramExists)
        {
	  std::vector<std::string> values;
	  std::string value;
	  bool hasValueList = false;
	  if (type == ParameterType_OutputImage)
          {
	    OutputImageParameter *imgParam = dynamic_cast<OutputImageParameter *>(param);
	    value = imgParam->GetFileName();
	    ImagePixelType pixType = imgParam->GetPixelType();
	    std::string ptype = pixelTypeToString(pixType); //FIXME save this

	  }
	  else
	    if( type == ParameterType_InputImageList || type == ParameterType_InputFilenameList ||
		type == ParameterType_InputVectorDataList || type == ParameterType_StringList )
	      {
		values = app->GetParameterStringList(key);
		hasValueList = true;
		//std::cerr << "its list type!!:" <<  values[0] << "\n\n\n ";
		
	      }
	  else
	    if (type == ParameterType_Float || type == ParameterType_Int || 
		type == ParameterType_Radius || type == ParameterType_RAM ) 
	      {
		value = app->GetParameterAsString(key);
	      }
	    else
	      if ( type == ParameterType_String || type == ParameterType_InputFilename ||
		   type == ParameterType_Directory || type == ParameterType_InputImage ||
		   type == ParameterType_ComplexInputImage || type == ParameterType_InputVectorData ||
		   type == ParameterType_Choice || type == ParameterType_ListView ||
		   type == ParameterType_OutputVectorData || type == ParameterType_OutputFilename)
		{
		  value = app->GetParameterString(key);
		}

		//parameter node in xml
		TiXmlElement * n_Parameter = new TiXmlElement("Parameter");

		const char * mandatory = "false";

		if( param->GetMandatory() )
		  mandatory = "true";

		n_Parameter->SetAttribute("mandatory", mandatory);

		//setting parameter key as child node in parameter
		TiXmlElement *n_ParamKey = new TiXmlElement( "key" );
		TiXmlText * nv_ParamKey = new TiXmlText( key.c_str() );
		n_ParamKey->LinkEndChild( nv_ParamKey );
		n_Parameter->LinkEndChild( n_ParamKey );

		//setting parameter value as child node in parameter
		TiXmlElement *n_ParamValue = new TiXmlElement( "value" );
     
		if(!hasValueList)
		  {
		    TiXmlText * nv_ParamValue = new TiXmlText( value.c_str() );
		    n_ParamValue->LinkEndChild( nv_ParamValue );
		    n_Parameter->LinkEndChild(n_ParamValue);
		  }
		else
		  {
		    TiXmlElement * n_Values = new TiXmlElement("values");
		    std::vector<std::string>::iterator strIt;
		    for(strIt = values.begin(); strIt != values.end(); ++strIt)
		      {
			TiXmlElement * n_ValuesValue = new TiXmlElement("value");
			const char *value = (*strIt).c_str();
			TiXmlText *nv_ValuesValue = new TiXmlText(value);
			n_ValuesValue->LinkEndChild(nv_ValuesValue);
			n_Values->LinkEndChild(n_ValuesValue);
		      }
		    n_Parameter->LinkEndChild(n_Values);
		  }

		n_App->LinkEndChild( n_Parameter ); 
	}
      }
    }
  
  // Finally, write xml contents to file

  doc.SaveFile( m_FileName.c_str() );
 
}

} //end namespace wrapper

} //end namespace otb
