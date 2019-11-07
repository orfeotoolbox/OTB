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

#include "otbDateTimeAdapter.h"

#include "ossim/base/ossimDate.h"

namespace otb
{

DateTimeAdapter::DateTimeAdapter()
{
  m_LocalTm = new ossimLocalTm(0);
}

DateTimeAdapter::~DateTimeAdapter()
{
  if (m_LocalTm != nullptr)
  {
    delete m_LocalTm;
  }
}

int DateTimeAdapter::GetYear()
{
  return m_LocalTm->getYear();
}

int DateTimeAdapter::GetMonth()
{
  return m_LocalTm->getMonth();
}

int DateTimeAdapter::GetDay()
{
  return m_LocalTm->getDay();
}

int DateTimeAdapter::GetHour()
{
  return m_LocalTm->getHour();
}

int DateTimeAdapter::GetMinute()
{
  return m_LocalTm->getMin();
}

double DateTimeAdapter::GetSeconds() const
{
  return static_cast<double>(m_LocalTm->getSec()) + m_LocalTm->getFractionalSecond();
}

bool DateTimeAdapter::SetFromIso8601(const std::string& date)
{
  return m_LocalTm->setIso8601(date);
}

double DateTimeAdapter::GetDeltaInSeconds(const DateTimeAdapter* pastDate)
{
  return (this->GetSeconds() - pastDate->GetSeconds());
}


} // namespace otb
