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

// Wrappers
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperAddProcessToWatchEvent.h"
#include "otbWrapperProxyParameter.h"
#include "otbWrapperParameterKey.h"
#include "itkStdStreamLogOutput.h"

namespace otb
{
namespace Wrapper
{

class CompositeTrain : public Application
{
public:
  /** Standard class typedefs. */
  typedef CompositeTrain            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(CompositeTrain, otb::Application);

  /** Filters typedef */
  typedef itk::MemberCommand< Self >        AddProcessCommandType;

  typedef struct
    {
    Application::Pointer App;
    std::string Desc;
    } InternalApplication;

  typedef std::map<std::string, InternalApplication> InternalAppContainer;

protected:
  void LinkWatchers(itk::Object * itkNotUsed(caller), const itk::EventObject & event)
    {
    if (typeid(AddProcessToWatchEvent) == typeid( event ))
      {
      this->InvokeEvent(event);
      }
    }

  CompositeTrain()
    {
    m_LogOutput = itk::StdStreamLogOutput::New();
    m_LogOutput->SetStream(m_Oss);
    m_AddProcessCommand = AddProcessCommandType::New();
    m_AddProcessCommand->SetCallbackFunction(this, &CompositeTrain::LinkWatchers);
    }

private:

  InternalAppContainer m_AppContainer;

  /**
   * Method to instanciate and register a new internal application
   * \param appType Type of the application to instanciate
   * \param key Identifier associated to the created application
   * \param desc Description of the internal application
   */
  bool AddApplication(std::string appType, std::string key, std::string desc)
    {
    if (m_AppContainer.count(key))
      {
      otbAppLogWARNING("The requested identifier for internal application is already used ("<<key<<")");
      return false;
      }
    InternalAppContainer container;
    container.App = ApplicationRegistry::CreateApplication(appType);
    container.Desc = desc;
    // Setup logger
    container.App->GetLogger()->AddLogOutput(m_LogOutput);
    container.App->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
    container.App->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());
    m_AppContainer[key] = container;
    return true;
    }

  bool Connect(Application *app1, std::string key1, Application *app2, std::string key2)
  {
    Parameter* rawParam1 = app1->GetParameterByKey(key1, false);
    if (dynamic_cast<ProxyParameter*>(rawParam1))
      {
      otbAppLogWARNING("First parameter is already connected !");
      return false;
      }
    ProxyParameter::Pointer proxyParam = ProxyParameter::New();
    ProxyParameter::ProxyTargetType target;
    target.first = app2->GetParameterList();
    target.second = key2;
    proxyParam->SetTarget(target);
    proxyParam->SetName(rawParam1->GetName());
    proxyParam->SetDescription(rawParam1->GetDescription());

    return app1->GetParameterList()->SetParameter(proxyParam.GetPointer(),key1);
  }

  bool Connect(std::string key1, std::string key2)
  {
    size_t pos1 = key1.find('.');
    size_t pos2 = key2.find('.');
    Application *app1 = this;
    Application *app2 = this;
    std::string key1Check(key1);
    std::string key2Check(key2);

    if (pos1 != std::string::npos && m_AppContainer.count(key1.substr(0,pos1)))
      {
      app1 = m_AppContainer[key1.substr(0,pos1)].App;
      key1Check = key1.substr(pos1+1);
      }
    if (pos2 != std::string::npos && m_AppContainer.count(key2.substr(0,pos2)))
      {
      app2 = m_AppContainer[key2.substr(0,pos2)].App;
      key2Check = key2.substr(pos2+1);
      }

    return this->Connect(app1, key1Check, app2, key2Check);
  }

  void DoExecuteInternal(std::string key)
    {
    otbAppLogINFO(<< m_AppContainer[key].Desc <<"...");
    m_AppContainer[key].App->Execute();
    otbAppLogINFO(<< "\n" << m_Oss.str());
    m_Oss.str(std::string(""));
    }

  void DoInit() ITK_OVERRIDE
  {
    SetName("CompositeTrain");
    SetDescription("Composite application for classifier training.");
    SetDocName("Composite Train");
    SetDocLongDescription(" TODO");
    SetDocLimitations(" TODO");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    this->AddApplication("PolygonClassStatistics", "polystat","Polygon analysis");

    this->AddApplication("SampleSelection", "select", "Sample selection");

    // share parameters with PolygonClassStatistics
    this->GetParameterList()->AddParameter(m_PolygonAnalysis->GetParameterByKey("in"));
    this->GetParameterList()->AddParameter(m_PolygonAnalysis->GetParameterByKey("vec"));
    //this->GetParameterList()->AddParameter(m_PolygonAnalysis->GetParameterByKey("field"));
    this->GetParameterList()->AddParameter(m_PolygonAnalysis->GetParameterByKey("layer"));

    AddParameter(ParameterType_Group, "sample", "Training and validation samples parameters");
    SetParameterDescription("sample",
                          "This group of parameters allows you to set training and validation sample lists parameters.");

    this->Connect("sample.vfn", "polystat.field");

    // share parameters with SampleSelection
    this->GetParameterList()->AddParameter(m_SampleSelection->GetParameterByKey("out"));
    this->GetParameterList()->AddParameter(m_SampleSelection->GetParameterByKey("strategy"));

    m_PolygonAnalysis->GetLogger()->AddLogOutput(m_LogOutput);
    m_PolygonAnalysis->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
    m_SampleSelection->GetLogger()->AddLogOutput(m_LogOutput);
    m_SampleSelection->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);

    // Progress
    // Add the callback to be added when a AddProcessToWatch event is invoked
    m_PolygonAnalysis->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());
    m_SampleSelection->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());

    // Doc example parameter settings
    SetDocExampleParameterValue("io.in", "clLabeledImageQB123_1.tif");
    SetDocExampleParameterValue("io.out", "clLabeledImageQB123_1_CMR_r2_nodl_10_undl_7.tif");
    SetDocExampleParameterValue("ip.radius", "2");
    SetDocExampleParameterValue("ip.suvbool", "true");
    SetDocExampleParameterValue("ip.nodatalabel", "10");
    SetDocExampleParameterValue("ip.undecidedlabel", "7");
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // copy common values
    // "in" -> "SampleSelection.in"
    // "vec" -> "SampleSelection.in"
    // "field" -> "SampleSelection.in"
    // "layer" -> "SampleSelection.in"
    m_SampleSelection->SetParameterString("in",this->GetParameterString("in"));
    m_SampleSelection->SetParameterString("vec",this->GetParameterString("vec"));
    m_SampleSelection->SetParameterString("field",this->GetParameterString("sample.vfn"));
    m_SampleSelection->SetParameterInt("layer",this->GetParameterInt("layer"));

    if (HasValue("out"))
      {
      // TODO : get dirname, and prepare name of temporary output
      m_PolygonAnalysis->SetParameterString("out", std::string("foo.xml"));
      m_SampleSelection->SetParameterString("instats", std::string("foo.xml"));
      }

    // DoUpdateParameters on sub-application
    m_PolygonAnalysis->UpdateParameters();
    m_SampleSelection->UpdateParameters();
  }

  void DoExecute() ITK_OVERRIDE
  {
    this->DoExecuteInternal("polystat");
    this->DoExecuteInternal("select");
  }// END DoExecute()

  Application::Pointer m_PolygonAnalysis;

  Application::Pointer m_SampleSelection;

  itk::StdStreamLogOutput::Pointer  m_LogOutput;

  std::ostringstream m_Oss;

  AddProcessCommandType::Pointer    m_AddProcessCommand;

}; // END class CompositeTrain


}// END namespace wrapper
}// END namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::CompositeTrain)
