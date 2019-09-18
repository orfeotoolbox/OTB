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

#include "otbProgressReporterManager.h"
#include "otbWrapperAddProcessToWatchEvent.h"

namespace otb
{

ProgressReporterManager::ProgressReporterManager()
{
  // Add the callback to be added when a AddProcessToWatch event is invoked
  m_AddProcessCommand = AddProcessCommandType::New();
  m_AddProcessCommand->SetCallbackFunction(this, &ProgressReporterManager::LinkWatchers);
}

ProgressReporterManager::~ProgressReporterManager()
{
  this->DeleteWatcherList();
}

void ProgressReporterManager::DeleteWatcherList()
{
  m_WatcherList.clear();
}

void ProgressReporterManager::LinkWatchers(itk::Object* itkNotUsed(caller), const itk::EventObject& event)
{
  if (typeid(otb::Wrapper::AddProcessToWatchEvent) == typeid(event))
  {
    const Wrapper::AddProcessToWatchEvent* eventToWatch = dynamic_cast<const Wrapper::AddProcessToWatchEvent*>(&event);

    auto watch = std::make_unique<WatcherType>(eventToWatch->GetProcess(), eventToWatch->GetProcessDescription());
    watch->SetCallback(m_Callback);
    m_WatcherList.push_back(std::move(watch));
  }
}
}
