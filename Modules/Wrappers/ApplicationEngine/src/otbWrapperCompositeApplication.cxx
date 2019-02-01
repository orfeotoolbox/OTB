/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbWrapperCompositeApplication.h"
#include "otbWrapperProxyParameter.h"
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperAddProcessToWatchEvent.h"
#include "otbWrapperParameterKey.h"

namespace otb
{
namespace Wrapper
{

CompositeApplication::CompositeApplication()
{
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
  container.App->SetLogger(this->GetLogger());
  container.App->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());
  m_AppContainer[key] = container;
  return true;
}

void
CompositeApplication
::ClearApplications()
{
  m_AppContainer.clear();
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
  return app1->GetParameterList()->ReplaceParameter(key1, proxyParam.GetPointer());
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

  // Check source
  ParameterKey pKey(localKey);
  std::string proxyKey(pKey.GetLastElement());

  // Create and setup proxy parameter
  ProxyParameter::Pointer proxyParam = ProxyParameter::New();
  ProxyParameter::ProxyTargetType target;
  target.first = app->GetParameterList();
  target.second = internalKeyCheck;
  proxyParam->SetTarget(target);
  proxyParam->SetName( name.empty() ? rawTarget->GetName() : name);
  proxyParam->SetDescription(desc.empty() ? rawTarget->GetDescription() : desc);
  proxyParam->SetKey(proxyKey);

  // Get group parameter where the proxy should be added
  Parameter::Pointer baseParam(proxyParam.GetPointer());
  ParameterGroup *parentGroup = this->GetParameterList();
  if (localKey.find('.') != std::string::npos)
    {
    Parameter::Pointer parentParam = this->GetParameterList()->GetParameterByKey(pKey.GetRoot());
    parentGroup = dynamic_cast<ParameterGroup*>(parentParam.GetPointer());
    baseParam->SetRoot(parentGroup);
    parentGroup->AddChild(baseParam);
    }
  parentGroup->AddParameter(baseParam);
  return true;
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
}

void
CompositeApplication
::UpdateInternalParameters(std::string key)
{
  GetInternalApplication(key)->UpdateParameters();
}

} // end namespace Wrapper
} // end namespace otb
