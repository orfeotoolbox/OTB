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

#include "otbXMLMetadataSupplier.h"

namespace otb
{
XMLMetadataSupplier::XMLMetadataSupplier(const std::string & fileName)
  : m_FileName(fileName)
{
  CPLXMLNode* psNode = CPLParseXMLFile(m_FileName.c_str());
  if(psNode != nullptr)
  {
    //if(std::string(psNode->pszValue) == "?xml")
//    if(EQUAL(psNode->pszValue, "?xml"))
//      psNode = psNode->psNext;
    m_MetadataDic = ReadXMLToList(psNode, m_MetadataDic);
  }
  else
  {
    otbLogMacro(Warning, <<"Unable to parse XML file " << fileName);
    m_MetadataDic = nullptr;
  }
  CPLDestroyXMLNode(psNode);
}

const char * XMLMetadataSupplier::GetMetadataValue(const char * path, int band) const
{
  return CSLFetchNameValue(m_MetadataDic, path);
}

std::string XMLMetadataSupplier::GetResourceFile(std::string) const
{
  return m_FileName;
}

// This method originates from a work by GDAL in the class GDALMDReaderBase.
char** XMLMetadataSupplier::ReadXMLToList(CPLXMLNode* psNode, char** papszList,
                                          const char* pszName)
{
  if(nullptr == psNode)
    return papszList;

  if (psNode->eType == CXT_Text)
  {
    papszList = CSLAddNameValue(papszList, pszName, psNode->pszValue);
  }

  if (psNode->eType == CXT_Element)
  {
    int nAddIndex = 0;
    bool bReset = false;
    for(CPLXMLNode* psChildNode = psNode->psChild; nullptr != psChildNode;
        psChildNode = psChildNode->psNext)
    {
      if (psChildNode->eType == CXT_Element)
      {
        // check name duplicates
        if(nullptr != psChildNode->psNext)
        {
          if(bReset)
          {
            bReset = false;
            nAddIndex = 0;
          }

          if(EQUAL(psChildNode->pszValue, psChildNode->psNext->pszValue))
          {
            nAddIndex++;
          }
          else
          { // the name changed
            if(nAddIndex > 0)
            {
              bReset = true;
              nAddIndex++;
            }
          }
        }
        else
        {
          if(bReset)
          {
            bReset = false;
            nAddIndex = 0;
          }
          if(nAddIndex > 0)
          {
            nAddIndex++;
          }
        }
        char szName[512];
        if(nAddIndex > 0)
        {
          CPLsnprintf( szName, 511, "%s_%d", psChildNode->pszValue,
                       nAddIndex);
        }
        else
        {
          CPLStrlcpy(szName, psChildNode->pszValue, 511);
        }
        char szNameNew[512];
        if(CPLStrnlen( pszName, 511 ) > 0) //if no prefix just set name to node name
        {
          CPLsnprintf( szNameNew, 511, "%s.%s", pszName, szName );
        }
        else
        {
          CPLsnprintf( szNameNew, 511, "%s.%s", psNode->pszValue, szName );
        }
        papszList = ReadXMLToList(psChildNode, papszList, szNameNew);
      }
      else if( psChildNode->eType == CXT_Attribute )
      {
        papszList = CSLAddNameValue(papszList,
                                    CPLSPrintf("%s.%s", pszName, psChildNode->pszValue),
                                    psChildNode->psChild->pszValue);
      }
      else
      {
        // Text nodes should always have name
        if(EQUAL(pszName, ""))
        {
          papszList = ReadXMLToList(psChildNode, papszList, psNode->pszValue);
        }
        else
        {
          papszList = ReadXMLToList(psChildNode, papszList, pszName);
        }
      }
    }
  }

  // proceed next only on top level
  if(nullptr != psNode->psNext && EQUAL(pszName, ""))
  {
    papszList = ReadXMLToList(psNode->psNext, papszList, pszName);
  }

  return papszList;
}

std::string XMLMetadataSupplier::PrintSelf()
{
  std::ostringstream oss;
  oss << "XMLMetadataSupplier: " << this->m_FileName << '\n';
  for (char ** string = this->m_MetadataDic; *string != nullptr ; ++string)
    oss << *string << '\n';
  return oss.str();
}


} // end namespace otb
