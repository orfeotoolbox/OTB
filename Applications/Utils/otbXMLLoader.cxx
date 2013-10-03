
#include <iostream>
#include <sstream>

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperInputProcessXMLParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"
#include "otbTinyXML.h"
namespace otb
{
namespace Wrapper
{
class XMLLoader : public Application
{
public:

  typedef XMLLoader Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(Self, otb::Application);

private:

  void DoInit()
  {
    this->SetHaveInXML(false);
    this->SetHaveOutXML(false);

    SetName("XMLLoader");
    SetDescription("XML Loader reads xml created from the execution of otb application and executes it");

    SetDocName("XMLLoader");
    SetDocLongDescription("XML Loader reads xml created from the execution of otb application and executes it.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag("xml");
    AddDocTag("application");

    AddParameter(ParameterType_InputFilename, "in", "Input XML File");
    AddParameter(ParameterType_Empty, "info", "Display information about application from the given input xml");
    SetDocExampleParameterValue("in", "application.xml");
    SetDocExampleParameterValue("info", "true");
    MandatoryOn("in");

  }

  void DoUpdateParameters()
  {

/*
    std::vector<std::string> tagList;
    std::vector<std::string> paramList = GetParametersKeys(true);


/*
///   std::cerr << paramList.size() << "psize\n";
    for (std::vector<std::string>::const_iterator it = paramList.begin(); it != paramList.end(); ++it)
    {
      std::string key = *it;

      std::cerr << "key1=" << key << "\n";
    }
*/
//    inXMLParam->GetMetaData();
//    ClearParameterList();
    //  std::cerr << "before read in xmloader1\n";


    std::string inputFilename = GetParameterString("in");

    //  UpdateParameterListFromXML(inputFilename);

    //UpdateMetaDataFromXML(inputFilename);

    //  AddParameter(ParameterType_InputProcessXML, key, inXMLParam->GetName());
    //  inXMLParam = dynamic_cast<InputProcessXMLParameter*>(GetParameterByKey(key));

    //SetParameterString(key,inputFilename);
//    inXMLParam->Read(this);
    //  inXMLParam->UpdateMetaData(this);


    //  AddParameter(inXMLParam);
//    AddInXMLParameter();
/*

*/

    //AddParameter(ParameterType_InputProcessXML, key, inXMLParam->GetName());
    //  AddOutXMLParameter();
//   m_MetaData = inXMLParam->GetMetaData();
//  //inXMLParam->PrintMetaData();
    //    tagList = inXMLParam->GetDocTagList(); //save it


//ReloadApplication();

// Init();


//    ReloadApplication();
//    SetDocTags(tagList); //good we saved before. else its gone after init.

//    OutputProcessXMLParameter::Pointer outXMLParam = dynamic_cast<OutputProcessXMLParameter*>(GetParameterByKey("inxml"));
//outXMLParam->SetFileName("/home/rashad/newout.xml");

  }

  void DoExecute()
  {

//    std::cerr << GetName() << ":nn\n";

/*
    std::vector<std::string> paramList = GetParametersKeys(true);
//   std::cerr << paramList.size() << "psize\n";
    for (std::vector<std::string>::const_iterator it = paramList.begin(); it != paramList.end(); ++it)
    {
      std::string key = *it;

      std::cerr << "key1=" << key << "\n";
    }

//    this->Execute();
*/

  }

/*
void UpdateParameterList()
{

  ClearParameterList();
  AddInXMLParameter();
  AddOutXMLParameter();


    std::string key = "inxml";
    std::string inputFilename = GetParameterString(key);
//    InputProcessXMLParameter::Pointer inXMLParam = dynamic_cast<InputProcessXMLParameter*>(GetParameterByKey(key));
//    InputProcessXMLParameter::Pointer inXMLParam = InputProcessXMLParameter::New();

//  ReadMetaData();
//  std::string app_Name = GetMetaDataByKey("appname");
  ParameterGroup::Pointer paramGroup = GetParameterList();

    std::cerr << "in updateparamlist in xmloader1\n";

    SetParameterString(key, inputFilename);


    TiXmlDocument doc(inputFilename.c_str());

//  FILE* fp = inXMLParam->TiXmlFOpen( inputFilename.c_str (), "rb" );

  if(!doc.LoadFile())
    std::cerr << "error loading xml\n";

  TiXmlHandle handle(&doc);

    std::cerr << "in updateparamlist in xmloader2\n";


  TiXmlElement *n_OTB = handle.FirstChild("OTB").Element();

    std::cerr << "in updateparamlist in xmloader--1\n";

  TiXmlElement *n_AppNode   = n_OTB->FirstChildElement("application");

    std::cerr << "in updateparamlist in xmloader3\n";

  for( TiXmlElement* n_Parameter = n_AppNode->FirstChildElement("parameter"); n_Parameter != NULL;
       n_Parameter = n_Parameter->NextSiblingElement() )
    {
    std::string key,typeAsString, value, paramName;
    std::vector<std::string> values;
    key = inXMLParam->GetChildNodeTextOf(n_Parameter, "key");
    typeAsString = inXMLParam->GetChildNodeTextOf(n_Parameter, "type");
    value = inXMLParam->GetChildNodeTextOf(n_Parameter, "value");
    paramName = inXMLParam->GetChildNodeTextOf(n_Parameter, "name");
    ParameterType type = paramGroup->GetParameterTypeFromString(typeAsString);

    if(key == "ram")
      {
      this->AddRAMParameter();
      }

    std::cerr << "kk=" << key << "\n";
    this->AddParameter(type, key, paramName);
    }
  fclose(fp);

    std::cerr << "in updateparamlist in xmloader991\n";


}

/*
  void ReloadApplication()
  {
   /*
   Application::Pointer app = ApplicationRegistry::CreateApplication(appName);
   if (app.IsNull())
     {
     std::cerr << "ERROR: Could not find application \"" << appName << "\"" << std::endl;
     }
     app = NULL;
   */

//Init();
    //SetHaveInXML(false);

/*
std::cerr << "reload\n";

   ClearParameterList();
   AddInXMLParameter();
   AddOutXMLParameter();


    std::vector<std::string> paramList = GetParametersKeys(true);
   std::cerr << paramList.size() << "psize\n";
    for (std::vector<std::string>::const_iterator it = paramList.begin(); it != paramList.end(); ++it)
      {
      Parameter::Pointer param = Parameter::New();
      param->SetKey(*it);
      }

//PrintM(m_MetaData);
//this->SetHaveInXML(false);
//AddInXMLParameter();

  }


  MetaDataType m_MetaData;
*/






};

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::XMLLoader)
