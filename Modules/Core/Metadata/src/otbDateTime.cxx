/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbDateTime.h"
#include "otbStringUtilities.h"

namespace otb
{
namespace MetaData
{

double ratio_(Duration const& lhs, Duration const& rhs)
{
  return (lhs.TotalNanoseconds() / rhs.TotalNanoseconds());
}

TimeType ReadFormattedDate(const std::string & dateStr, const std::string & format)
{
  MetaData::TimeType outputDate;
  std::stringstream ss;
  auto facet = new boost::posix_time::time_input_facet(format);
  ss.imbue(std::locale(std::locale(), facet));
  ss << dateStr;
  ss >> outputDate;
  return outputDate;
}

DurationType seconds(double input)
{
  //return boost::posix_time::precise_duration(input * 1e9);
  return DurationType::Seconds(input);
}

Duration Abs(Duration d)
{
  if(d.m_Duration.is_negative())
    d.m_Duration = d.m_Duration.invert_sign();
  return d;
}

}
}
