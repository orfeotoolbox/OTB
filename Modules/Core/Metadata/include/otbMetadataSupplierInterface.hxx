/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMetadataSupplierInterface_hxx
#define otbMetadataSupplierInterface_hxx

#include "otbMetadataSupplierInterface.h"

namespace otb
{



template <typename T> T MetadataSupplierInterface::GetAs(T const& defaultValue, std::string const& path, int band) const
{
  bool hasValue;
  std::string ret = GetMetadataValue(path, hasValue, band);
  if (!hasValue)
  {
    return defaultValue;
  }
  boost::trim(ret);
  try
  {
    return boost::lexical_cast<T>(ret);
  }
  catch (boost::bad_lexical_cast&)
  {
    return defaultValue;
  }
}

template <typename T> T MetadataSupplierInterface::GetAs(std::string const& path, int band) const
{
  bool hasValue;
  std::string ret = GetMetadataValue(path, hasValue, band);
  if (!hasValue)
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Missing metadata '"<<path<<"'")
  }
  boost::trim(ret);
  try
  {
    return boost::lexical_cast<T>(ret);
  }
  catch (boost::bad_lexical_cast&)
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Bad metadata value for '"<<path<<"', got: "<<ret)
  }
}

template < typename T> std::vector<T> MetadataSupplierInterface::GetAsVector(std::string const& path, char sep, int size, int band) const
{
  bool hasValue;
  std::string ret = GetMetadataValue(path, hasValue, band);
  if (!hasValue)
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Missing metadata '"<<path<<"'")
  }
  string_view value(ret);
  string_view filt_value = rstrip(lstrip(value,"[ "), "] ");
  std::vector<T> output;
  typedef part_range<splitter_on_delim> range_type;
  const range_type parts = split_on(filt_value, sep);
  for (auto const& part : parts)
  {
    // TODO: check if we can use lexical_cast on a string_view
    std::string strPart = to<std::string>(part, "casting string_view to std::string");
    if (strPart.empty())
    {
      continue;
    }
    try
    {
      output.push_back(boost::lexical_cast<T>(strPart));
    }
    catch (boost::bad_lexical_cast&)
    {
      otbGenericExceptionMacro(MissingMetadataException,<<"Bad metadata vector element in '"<<path<<"', got :"<<part)
    }
  }
  if ((size >= 0) && (output.size() != (size_t)size))
  {
    otbGenericExceptionMacro(MissingMetadataException,<<"Bad number of elements in vector '"<<path<<"', expected "<<size<< ", got "<<output.size())
  }
  return output;
}

} // end namespace otb

#endif
