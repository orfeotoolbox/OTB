/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbExtendedFilenameHelper.h"

#include <cassert>
#include <ostream>
#include <stdexcept>
#include <string>

#include "otbStringUtils.h"

namespace otb
{

void
ExtendedFilenameHelper
::SetExtendedFileName(const char *extFname)
{
  if (extFname == nullptr)
  {
    itkGenericExceptionMacro( << "Filename is NULL" );
  }

  this->SetExtendedFileName(std::string(extFname));
}

void
ExtendedFilenameHelper
::SetExtendedFileName(const std::string& extFname)
{
  this->m_ExtendedFileName = extFname;
  this->m_OptionMap.clear();
  std::vector<std::string> tmp1;
  std::vector<std::string> tmp2;
  if (!m_ExtendedFileName.empty())
    {
    boost::split(tmp1, m_ExtendedFileName, boost::is_any_of("?"), boost::token_compress_on);
    this->SetSimpleFileName(tmp1[0]);
    if (tmp1.size()>1)
      {
      boost::split(tmp2, tmp1[1], boost::is_any_of("&"), boost::token_compress_on);
      for (unsigned int i=0; i<tmp2.size(); i++)
      if (!tmp2[i].empty())
        {
        std::vector<std::string> tmp;
        boost::split(tmp, tmp2[i], boost::is_any_of("="), boost::token_compress_on);
        if (tmp.size()>1)
          {
          if (!tmp[1].empty())
            {
            if (m_OptionMap[tmp[0]].empty())
              {
              m_OptionMap[tmp[0]]=tmp[1];
              }
            else
              {
              itkWarningMacro("Duplicated option detected: " << tmp[0] << ". Using value " << tmp[1] << ".");
              }
            }
          else
            itkGenericExceptionMacro( << "Value for option '" << tmp[0] << "' is not set.");
          }
        }
      }
    }
}

const ExtendedFilenameHelper::OptionMapType &
ExtendedFilenameHelper
::GetOptionMap(void) const
{
  return this->m_OptionMap;
}

/*-------------------- GenericBandRange ----------------------*/

ExtendedFilenameHelper::GenericBandRange
::GenericBandRange(int a)
  : std::pair<int,int>(a,a)
  {
  }

ExtendedFilenameHelper::GenericBandRange
::GenericBandRange(const std::pair<int,int>& a)
  : std::pair<int,int>(a)
  {
    if (a.second>=0 && a.second < a.first)
      {
      throw std::range_error("Invalid range");
      }
  }

ExtendedFilenameHelper::GenericBandRange
::GenericBandRange(int a,int b)
  : std::pair<int,int>(a,b)
  {
    if (b>=0 && b < a)
      {
      throw std::range_error("Invalid range");
      }
  }

bool
ExtendedFilenameHelper::GenericBandRange
::SetString(const std::string& str, size_t start , size_t size)
  {
  assert(start < str.size());
  bool ret = true;
  if (size == 0)
    {
    first = 0;
    second = 0;
    return false;
    }
  size_t end = str.size();
  if (size != std::string::npos && (start+size) <= str.size())
    {
    end = start + size;
    }
  size_t pos = str.find(':',start);
  if (pos != std::string::npos && pos<end)
    {
    // range of values
    if (pos > start)
      {
      try
        {
        first = boost::lexical_cast<int>(str.c_str()+start, pos-start);
        }
      catch(boost::bad_lexical_cast &)
        {
        ret = false;
        }
      }
    else
      {
      first = 0;
      }
    if (end > pos + 1)
      {
      try
        {
        second = boost::lexical_cast<int>(str.c_str()+ pos + 1, end - pos - 1);
        }
      catch(boost::bad_lexical_cast &)
        {
        ret = false;
        }
      }
    else
      {
      second = 0;
      }
    }
  else
    {
    // single value
    try
      {
      first = boost::lexical_cast<int>(str.c_str()+start, end-start);
      second = first;
      }
    catch(boost::bad_lexical_cast &)
      {
      ret = false;
      }
    }
  return ret;
  }

void
ExtendedFilenameHelper::GenericBandRange
::Print(std::ostream& os)
  {
  if (this->first)
    {
    os << this->first;
    }
  if (this->first != this->second)
    {
    if (this->first || this->second)
      {
      os << ":";
      }
    if (this->second)
      {
      os << this->second;
      }
    }
  }

std::vector<ExtendedFilenameHelper::GenericBandRange>
ExtendedFilenameHelper
::GetGenericBandRange(const std::string &bandRange) const
{
  //Parse string to return vector of band range
  std::vector<ExtendedFilenameHelper::GenericBandRange> vBands;
  size_t start = 0;
  size_t pos;
  if (!bandRange.empty())
    {
    while (start != std::string::npos)
      {
      pos = bandRange.find(',',start);
      if (pos > start)
        {
        ExtendedFilenameHelper::GenericBandRange range;
        size_t size = (pos == std::string::npos ? pos : pos - start);

        bool ret = range.SetString(bandRange, start, size);
        if (ret) vBands.push_back(range);
        }
      if (pos != std::string::npos) pos++;
      start = pos;
      }
    }
  return vBands;
}

bool
ExtendedFilenameHelper
::ResolveBandRange(const std::string &bandRange, const unsigned int &nbBands, std::vector<unsigned int> &output) const
{
  output.clear();
  std::vector<ExtendedFilenameHelper::GenericBandRange> bandRangeList = this->GetGenericBandRange(bandRange);
  for (unsigned int i=0 ; i<bandRangeList.size() ; i++)
    {
    int a = bandRangeList[i].first;
    int b = bandRangeList[i].second;
    if (a<0) a+= nbBands+1;
    if (b<0) b+= nbBands+1;
    if (a==0) a=1;
    if (b==0) b=nbBands;

    if (1<=a && a<=b && b<=(int)nbBands)
      {
      for (unsigned int k=a ; k <= (unsigned int)b ; k++)
        {
        output.push_back((int)k -1);
        }
      }
    else
      {
      // Invalid range wrt. the given number of bands
      itkExceptionMacro("Invalid band number.");
      return false;
      }
    }
  return true;
}

} // end namespace otb
