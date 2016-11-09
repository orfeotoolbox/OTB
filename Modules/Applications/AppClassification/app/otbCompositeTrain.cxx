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
#include "itksys/SystemTools.hxx"

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
    InternalApplication container;
    container.App = ApplicationRegistry::CreateApplication(appType);
    container.Desc = desc;
    // Setup logger
    container.App->GetLogger()->AddLogOutput(m_LogOutput);
    container.App->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
    container.App->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());
    m_AppContainer[key] = container;
    return true;
    }

  /**
   * Connect two existing parameters together. The first parameter will point to
   * the second parameter.
   */
  bool Connect(std::string fromKey, std::string toKey)
  {
    std::string key1(fromKey);
    std::string key2(toKey);
    Application *app1 = DecodeKey(key1);
    Application *app2 = DecodeKey(key2);

    Parameter* rawParam1 = app1->GetParameterByKey(key1, false);
    if (dynamic_cast<ProxyParameter*>(rawParam1))
      {
      otbAppLogWARNING("Parameter is already connected ! Override current connection");
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

  /**
   * Share a parameter between the composite application and an internal application
   * The local parameter is created as a proxy to the internal parameter.
   * \param localKey New parameter key in the composite application
   * \param internalKey Key to the internal parameter to expose
   * \param name Name for the local parameter, if empty the target's name is used
   * \param desc Description for the local parameter, if empty the target's description is used
   */
  bool ShareParameter(std::string localKey,
                      std::string internalKey,
                      std::string name = std::string(),
                      std::string desc = std::string())
    {
    std::string internalKeyCheck(internalKey);
    Application *app = DecodeKey(internalKeyCheck);
    Parameter* rawTarget = app->GetParameterByKey(internalKeyCheck, false);

    ProxyParameter::Pointer proxyParam = ProxyParameter::New();
    ProxyParameter::ProxyTargetType target;
    target.first = app->GetParameterList();
    target.second = internalKeyCheck;
    proxyParam->SetTarget(target);
    proxyParam->SetName( name.empty() ? rawTarget->GetName() : name);
    proxyParam->SetDescription(desc.empty() ? rawTarget->GetDescription() : desc);
    return this->GetParameterList()->SetParameter(proxyParam.GetPointer(),localKey);
    }

  /**
   * Decode a key to extract potential prefix for internal applications
   * If a valid prefix (corresponding to an internal app) is found:
   *   - prefix is removed from the input key which is altered.
   *   - the function returns a pointer to the internal application
   * If no valid prefix is found, the input key is not modified, and the
   * function returns 'this'.
   */
  Application* DecodeKey(std::string &key)
    {
    Application *ret = this;
    size_t pos = key.find('.');
    if (pos != std::string::npos && m_AppContainer.count(key.substr(0,pos)))
      {
      ret = m_AppContainer[key.substr(0,pos)].App;
      key = key.substr(pos+1);
      }
    return ret;
    }

  void ExecuteInternal(std::string key)
    {
    otbAppLogINFO(<< m_AppContainer[key].Desc <<"...");
    m_AppContainer[key].App->Execute();
    otbAppLogINFO(<< "\n" << m_Oss.str());
    m_Oss.str(std::string(""));
    }

  void UpdateParametersInternal(std::string key)
    {
    m_AppContainer[key].App->UpdateParameters();
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

    AddParameter(ParameterType_Group, "sample", "Training and validation samples parameters");
    SetParameterDescription("sample", "This group of parameters allows you to "
      "set training and validation sample lists parameters.");

    AddApplication("PolygonClassStatistics", "polystat","Polygon analysis");
    AddApplication("SampleSelection", "select", "Sample selection");

    // share parameters with PolygonClassStatistics
    ShareParameter("in","polystat.in");
    ShareParameter("vec","polystat.vec");
    ShareParameter("layer","polystat.layer");
    ShareParameter("sample.vfn", "polystat.field");

    // share parameters with SampleSelection
    ShareParameter("out", "select.out");
    ShareParameter("strat", "select.strategy");

    // synchronize some parameters together:
    Connect("select.in",    "polystat.in");
    Connect("select.vec",   "polystat.vec");
    Connect("select.field", "polystat.field");
    Connect("select.layer", "polystat.layer");
    Connect("select.instats", "polystat.out");

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
    if (HasValue("out"))
      {
      m_AppContainer["polystat"].App->SetParameterString("out", GetParameterString("out") + std::string("_stats.xml") );
      }

    // DoUpdateParameters on sub-application
    UpdateParametersInternal("polystat");
    UpdateParametersInternal("select");
  }

  void DoExecute() ITK_OVERRIDE
  {
    this->ExecuteInternal("polystat");
    this->ExecuteInternal("select");
  }// END DoExecute()

  InternalAppContainer m_AppContainer;

  itk::StdStreamLogOutput::Pointer  m_LogOutput;

  std::ostringstream m_Oss;

  AddProcessCommandType::Pointer    m_AddProcessCommand;

}; // END class CompositeTrain


}// END namespace wrapper
}// END namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::CompositeTrain)
