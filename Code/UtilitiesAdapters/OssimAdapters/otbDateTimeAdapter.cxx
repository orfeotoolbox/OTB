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
  if (m_LocalTm != NULL)
    {
    delete m_LocalTm;
    }
}

int
DateTimeAdapter::GetYear()
{
  return m_LocalTm->getYear();
}

int
DateTimeAdapter::GetMonth()
{
  return m_LocalTm->getMonth();
}

int
DateTimeAdapter::GetDay()
{
  return m_LocalTm->getDay();
}

int
DateTimeAdapter::GetHour()
{
  return m_LocalTm->getHour();
}

int
DateTimeAdapter::GetMinute()
{
  return m_LocalTm->getMin();
}

double
DateTimeAdapter::GetSeconds()
{
  return static_cast<double>(m_LocalTm->getSec())
    + m_LocalTm->getFractionalSecond();
}

bool
DateTimeAdapter::SetFromIso8601(const std::string &date)
{
  return m_LocalTm->setIso8601(date);
}

double
DateTimeAdapter::GetDeltaInSeconds(const DateTimeAdapter *pastDate)
{
  return m_LocalTm->deltaInSeconds(*(pastDate->m_LocalTm));
}


} // namespace otb
