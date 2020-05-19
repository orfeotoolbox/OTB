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

#ifndef otbXMLMetadataSupplier_h
#define otbXMLMetadataSupplier_h

#include "OTBMetadataExport.h"
#include "otb_tinyxml.h"
#include "otbMetadataSupplierInterface.h"
#include "otbStringUtilities.h"


namespace otb
{

/** \class MetadataSupplierInterface
 *
 * \brief Base class to access metadata information in files/images
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT XMLMetadataSupplier : public MetadataSupplierInterface
{
public:

  XMLMetadataSupplier(const std::string & fileName, char separator = '.') : m_FileName(fileName), 
                                                                            m_Separator(separator), 
                                                                            m_Handle(nullptr)
  {
    m_Doc = TiXmlDocument(m_FileName);
    
    if (!m_Doc.LoadFile())
    {std::cout <<"yo" << std::endl;
      otbGenericExceptionMacro(MissingMetadataException,<<"Can't open file " << m_FileName)
    }
    
     m_Handle =TiXmlHandle( &m_Doc );
    
  }


  std::string GetResourceFile() const override
  {
    return m_FileName;
  }

  /** Get the metadata value corresponding to a given path (meaning of this path
   * depends on the specific implementation. Returns NULL when path is not found
   * If band >= 0, the metadata value is looked in the specified band*/
  const char * GetMetadataValue(const char * path, int band=-1) const override
  {
    auto split = split_on(path, m_Separator);
    //std::cout << m_Doc << std::endl;
    
    
    TiXmlHandle handle = m_Handle;
    std::cout <<handle.FirstChild().ToElement() << std::endl;
    for (auto part : split)
    {
      std::cout << part << std::endl;
     // std::string aa = part;
      handle = handle.FirstChild(std::string(part.data(), part.size()));
      std::cout << handle.ToElement() << std::endl;
      
    }
    auto element = handle.ToElement();
    if (!element)
    {
      otbGenericExceptionMacro(MissingMetadataException,<<"Cannot find " << path)
    }
    
    std::cout << element << element->Attribute( "SUN_ELEVATION") << std::endl;
    
    
    return "";
  }
  
private:
  std::string m_FileName;
  char m_Separator;
  TiXmlDocument m_Doc;
  TiXmlHandle   m_Handle;
};

} // end namespace otb

#endif

