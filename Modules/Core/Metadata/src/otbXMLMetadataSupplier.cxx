/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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

#include "otbXMLMetadataSupplier.h"
#include <unordered_set>

namespace otb
{
XMLMetadataSupplier::XMLMetadataSupplier(const std::string & fileName)
  : m_FileName(fileName)
{
  CPLXMLNode* psNode = CPLParseXMLFile(m_FileName.c_str());
  if(psNode != nullptr)
    m_MetadataDic = ReadXMLToList(psNode, m_MetadataDic);
  else
  {
    otbLogMacro(Warning, <<"Unable to parse XML file " << fileName);
    m_MetadataDic = nullptr;
  }
  CPLDestroyXMLNode(psNode);
}

std::string XMLMetadataSupplier::GetMetadataValue(std::string const& path, bool& hasValue, int /*band*/) const
{
  const char * ret = CSLFetchNameValue(m_MetadataDic, path.c_str());
  if (ret)
    hasValue = true;
  else
  {
    hasValue = false;
    ret = "";
  }
  return std::string(ret);
}

std::string XMLMetadataSupplier::GetFirstMetadataValue(std::string const& path, bool& hasValue) const
{
  // Search for the  first joker
  std::size_t found = path.find("_#");
  // Looking for the keys corresponding to the part of the path before the first joker
  std::vector<std::string> values = this->FetchPartialNameValueMultiple(m_MetadataDic, path.substr(0, found).c_str());
  // Position of the beginning of the path after the joker
  std::size_t start = found + 2;

  // While a joker is found
  while(found != std::string::npos)
  {
    // Look for the next joker
    found = path.find("_#", start);
    // Look for the keys corresponding to the part of the path between the two jokers
    values = this->FetchPartialNameValueMultiple(values, path.substr(start, found));
    start = found + 2;
  }

  if ((values.size() != 0) && (!values[0].empty()))
  {
    hasValue = true;
    std::string ret = values[0];
    ret = ret.substr(ret.find('=') + 1);
    // Return the value part
    return ret;
  }
  else
  {
    hasValue = false;
    return "";
  }
}

std::string XMLMetadataSupplier::GetResourceFile(std::string const& /*s*/) const
{
  return m_FileName;
}


/**
 * AddXMLNameValueToList() is a GDAL function non exposed from API.
 * The original code source is available here
 * https://github.com/OSGeo/gdal/blob/13323826f941afbd8a4f188312a3cec20e487290/gcore/gdal_mdreader.cpp#L297
 */
char** XMLMetadataSupplier::AddXMLNameValueToList(char** papszList,
                                               const char *pszName,
                                               const char *pszValue)
{
    return CSLAddNameValue(papszList, pszName, pszValue);
}

/**
 * ReadXMLToListFirstPass() is a GDAL function non exposed from API.
 * The original code source is available here
 * https://github.com/OSGeo/gdal/blob/13323826f941afbd8a4f188312a3cec20e487290/gcore/gdal_mdreader.cpp#L307
 */
void XMLMetadataSupplier::ReadXMLToListFirstPass(const CPLXMLNode* psNode,
                                              std::map<std::string, int>& oMapCountKeysFull,
                                              const std::string& osPrefixFull)
{
    if(nullptr == psNode)
        return;
    if (psNode->eType == CXT_Element)
    {
        std::string osNewPrefixFull;
        for(const CPLXMLNode* psChildNode = psNode->psChild;
            nullptr != psChildNode;
            psChildNode = psChildNode->psNext)
        {
            if (psChildNode->eType == CXT_Element)
            {
                osNewPrefixFull = !osPrefixFull.empty() ? osPrefixFull : std::string(psNode->pszValue);
                osNewPrefixFull += '.';
                osNewPrefixFull += psChildNode->pszValue;
                osNewPrefixFull += CPLSPrintf("_%d", ++oMapCountKeysFull[osNewPrefixFull]);

                ReadXMLToListFirstPass(psChildNode, oMapCountKeysFull, osNewPrefixFull);
            }
        }
    }

    // proceed next only on top level

    if(nullptr != psNode->psNext && osPrefixFull.empty())
    {
         ReadXMLToListFirstPass(psNode->psNext, oMapCountKeysFull, osPrefixFull);
    }
}

/**
 * ReadXMLToList()is a GDAL function non exposed from API.
 * The original code source is available here
 * https://github.com/OSGeo/gdal/blob/13323826f941afbd8a4f188312a3cec20e487290/gcore/gdal_mdreader.cpp#L343
 */
char** XMLMetadataSupplier::ReadXMLToList(const CPLXMLNode* psNode,
                                       char** papszList,
                                       const std::map<std::string, int>& oMapCountKeysFullRef,
                                       std::map<std::string, int>& oMapCountKeysFull,
                                       std::map<std::string, int>& oMapCountKeys,
                                       const std::string& osPrefix,
                                       const std::string& osPrefixFull)
{
    if(nullptr == psNode)
        return papszList;

    if (psNode->eType == CXT_Text)
    {
        papszList = AddXMLNameValueToList(papszList, osPrefix.c_str(), psNode->pszValue);
    }

    if (psNode->eType == CXT_Element)
    {
        std::string osNewPrefix;
        std::string osNewPrefixFull;
        for(const CPLXMLNode* psChildNode = psNode->psChild;
            nullptr != psChildNode;
            psChildNode = psChildNode->psNext)
        {
            if (psChildNode->eType == CXT_Element)
            {
                osNewPrefixFull = !osPrefixFull.empty() ? osPrefixFull : std::string(psNode->pszValue);
                osNewPrefixFull += '.';
                osNewPrefixFull += psChildNode->pszValue;

                const auto oIter = oMapCountKeysFullRef.find(osNewPrefixFull);
                CPLAssert(oIter != oMapCountKeysFullRef.end());
                osNewPrefixFull += CPLSPrintf("_%d", ++oMapCountKeysFull[osNewPrefixFull]);

                osNewPrefix = !osPrefix.empty() ? osPrefix : std::string(psNode->pszValue);
                osNewPrefix += '.';
                osNewPrefix += psChildNode->pszValue;
                const int nIndex = ++oMapCountKeys[osNewPrefix];
                const bool bMultipleInstances = oIter->second >= 2;
                if( bMultipleInstances )
                {
                    osNewPrefix += CPLSPrintf("_%d", nIndex);
                }
                papszList = ReadXMLToList(psChildNode, papszList,
                                          oMapCountKeysFullRef,
                                          oMapCountKeysFull,
                                          oMapCountKeys,
                                          osNewPrefix, osNewPrefixFull);
            }
            else if( psChildNode->eType == CXT_Attribute )
            {
                papszList = AddXMLNameValueToList(papszList,
                                                  CPLSPrintf("%s.%s", osPrefix.c_str(), psChildNode->pszValue),
                                                  psChildNode->psChild->pszValue);
            }
            else
            {
                // Text nodes should always have name
                if(osPrefix.empty())
                {
                    papszList = ReadXMLToList(psChildNode, papszList,
                                              oMapCountKeysFullRef,
                                              oMapCountKeysFull,
                                              oMapCountKeys,
                                              psNode->pszValue, psNode->pszValue);
                }
                else
                {
                    papszList = ReadXMLToList(psChildNode, papszList,
                                              oMapCountKeysFullRef,
                                              oMapCountKeysFull,
                                              oMapCountKeys,
                                              osPrefix.c_str(), osNewPrefixFull.c_str());
                }
            }
        }
    }

    // proceed next only on top level

    if(nullptr != psNode->psNext && osPrefix.empty())
    {
         papszList = ReadXMLToList(psNode->psNext, papszList,
                                   oMapCountKeysFullRef,
                                   oMapCountKeysFull,
                                   oMapCountKeys,
                                   osPrefix, osPrefixFull);
    }

    return papszList;
}


/**
 * ReadXMLToList() is a GDAL function non exposed from API.
 * The original code source is available here
 * https://github.com/OSGeo/gdal/blob/13323826f941afbd8a4f188312a3cec20e487290/gcore/gdal_mdreader.cpp#L439
 */
char** XMLMetadataSupplier::ReadXMLToList(CPLXMLNode* psNode, char** papszList,
                                       const char* pszName)
{
    std::map<std::string, int> oMapCountKeysFullRef;
    ReadXMLToListFirstPass(psNode, oMapCountKeysFullRef, pszName);
    std::map<std::string, int> oMapCountKeysFull;
    std::map<std::string, int> oMapCountKeys;
    return ReadXMLToList(psNode, papszList,
                         oMapCountKeysFullRef,
                         oMapCountKeysFull,
                         oMapCountKeys,
                         pszName, pszName);
}

std::vector<std::string> XMLMetadataSupplier::FetchPartialNameValueMultiple(char** papszStrList,
		                                                                    const char *pszName) const
{
  std::vector<std::string> retStringVector;
  if( papszStrList == nullptr || pszName == nullptr )
    return retStringVector;

  for( ; *papszStrList != nullptr ; ++papszStrList )
    if( strstr(*papszStrList, pszName) )
      retStringVector.push_back(*papszStrList);
  return retStringVector;
}

std::vector<std::string> XMLMetadataSupplier::FetchPartialNameValueMultiple(const std::vector<std::string> &StringVector,
		                                                                    const std::string &Name) const
{
  std::vector<std::string> retStringVector;
  if( StringVector.size() == 0 || Name.empty() )
    return retStringVector;

  for( const auto& val : StringVector )
    if( val.find(Name) != std::string::npos )
      retStringVector.push_back(val);
  return retStringVector;
}

std::vector<std::string> XMLMetadataSupplier::GetAllStartWith(char** papszStrList,
                                                                  const char *pszName) const
{
  std::vector<std::string> retStringVector;
  if( papszStrList == nullptr || pszName == nullptr )
    return retStringVector;

  for( ; *papszStrList != nullptr ; ++papszStrList )
    if( strstr(*papszStrList, pszName) == *papszStrList)
      retStringVector.push_back(*papszStrList);
  return retStringVector;
}

int XMLMetadataSupplier::GetNbBands() const
{
  return 0;
}

unsigned int XMLMetadataSupplier::GetNumberOf(std::string const & path) const
{
  std::unordered_set<int> idx;
  for(auto const& key : GetAllStartWith(m_MetadataDic, path.c_str()))
  {
    auto after = key.substr(path.size());
    if(after.size() > 0 && after.compare(0, 1, "_") == 0)
    {
      auto sub = after.substr(1, after.find("."));
      idx.insert(std::stoi(sub));
    }
    else if(after.size() > 0 && after.compare(0, 1, ".") == 0)
      idx.insert(0);
  }
  return idx.size();
}
unsigned int XMLMetadataSupplier::GetAttributId(std::string const& path, std::string const& value) const
{
  // Search for the joker
  std::size_t found = path.find("_#");
  // Looking for the keys corresponding to the part of the path before the joker
  std::vector<std::string> values = this->FetchPartialNameValueMultiple(m_MetadataDic, path.substr(0, found).c_str());
  // Position of the beginning of the path after the joker
  std::size_t start = found + 2;
  // Looking for the keys corresponding to the part of the path after the joker
  values = this->FetchPartialNameValueMultiple(values, path.substr(start));

  if ((values.size() == 0) || (values[0].empty()))
    return 0;

  for(auto const& s: values)
  {
    auto val = s.substr(s.find('=') + 1);
    if (val == value)
    {
      val = s.substr(found + 1, s.find(".", found) - found - 1);
      return std::stoi(val);
    }
  }
  return 0;
}

std::string XMLMetadataSupplier::PrintSelf() const
{
  std::ostringstream oss;
  oss << "XMLMetadataSupplier: " << this->m_FileName << '\n';
  for (char ** string = this->m_MetadataDic; *string != nullptr ; ++string)
    oss << *string << '\n';
  return oss.str();
}

} // end namespace otb
