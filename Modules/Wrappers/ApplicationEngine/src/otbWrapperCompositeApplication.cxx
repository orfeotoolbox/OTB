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

#include "otbWrapperCompositeApplication.h"
#include "otbWrapperProxyParameter.h"
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperAddProcessToWatchEvent.h"

namespace otb
{
namespace Wrapper
{

CompositeApplication::CompositeApplication()
{
  m_LogOutput = itk::StdStreamLogOutput::New();
  m_LogOutput->SetStream(m_Oss);
  m_AddProcessCommand = AddProcessCommandType::New();
  m_AddProcessCommand->SetCallbackFunction(this, &CompositeApplication::LinkWatchers);
}

CompositeApplication::~CompositeApplication()
{
}

void
CompositeApplication
::LinkWatchers(itk::Object * itkNotUsed(caller), const itk::EventObject & event)
{
  if (typeid(AddProcessToWatchEvent) == typeid( event ))
    {
    this->InvokeEvent(event);
    }
}

bool
CompositeApplication
::AddApplication(std::string appType, std::string key, std::string desc)
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

bool
CompositeApplication
::Connect(std::string fromKey, std::string toKey)
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

bool
CompositeApplication
::ShareParameter(std::string localKey,
                 std::string internalKey,
                 std::string name,
                 std::string desc)
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

Application*
CompositeApplication
::DecodeKey(std::string &key)
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

Application*
CompositeApplication
::GetInternalApplication(std::string id)
{
  if (!m_AppContainer.count(id))
    otbAppLogFATAL("Unknown internal application : "<<id);
  return m_AppContainer[id].App;
}

std::string
CompositeApplication
::GetInternalAppDescription(std::string id)
{
  if (!m_AppContainer.count(id))
    otbAppLogFATAL("Unknown internal application : "<<id);
  return m_AppContainer[id].Desc;
}

void
CompositeApplication
::ExecuteInternal(std::string key)
{
  otbAppLogINFO(<< GetInternalAppDescription(key) <<"...");
  GetInternalApplication(key)->Execute();
  otbAppLogINFO(<< "\n" << m_Oss.str());
  m_Oss.str(std::string(""));
}

void
CompositeApplication
::UpdateInternalParameters(std::string key)
{
  GetInternalApplication(key)->UpdateParameters();
}

} // end namespace Wrapper
} // end namespace otb
