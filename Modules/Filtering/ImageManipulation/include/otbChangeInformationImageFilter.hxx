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

#ifndef otbChangeInformationImageFilter_hxx
#define otbChangeInformationImageFilter_hxx

#include "otbChangeInformationImageFilter.h"
#include "itkMetaDataObject.h"

namespace otb
{

template <typename TInputImage>
void ChangeInformationImageFilter<TInputImage>::SetChangeMetaData(const char* keyname, bool flag)
{
  std::string key(keyname);
  if (!key.empty())
  {
    if (flag)
    {
      m_ChangedKeys.insert(key);
    }
    else
    {
      std::set<std::string>::iterator pos = m_ChangedKeys.find(key);
      if (pos != m_ChangedKeys.end())
      {
        m_ChangedKeys.erase(pos);
      }
    }
  }
}

template <typename TInputImage>
bool ChangeInformationImageFilter<TInputImage>::GetChangeMetaData(const char* keyname)
{
  std::string key(keyname);
  if (!key.empty())
  {
    if (m_ChangedKeys.find(key) != m_ChangedKeys.end())
    {
      return true;
    }
  }
  return false;
}

template <typename TInputImage>
template <typename T>
void ChangeInformationImageFilter<TInputImage>::SetOutputMetaData(const char* keyname, const T* value)
{
  std::string key(keyname);
  if (!key.empty())
  {
    // enable this key for metadata change
    m_ChangedKeys.insert(key);
    itk::MetaDataDictionary& dict = this->GetMetaDataDictionary();
    if (value == nullptr)
    {
      // Remove meta-data from dictionary
      dict.Erase(key);
    }
    else
    {
      // Set metadata in dictionary
      const T& valueRef = (*value);
      itk::EncapsulateMetaData<T>(dict, key, valueRef);
    }
  }
}

template <typename TInputImage>
void ChangeInformationImageFilter<TInputImage>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  // Process the metadatas to be changed
  itk::MetaDataDictionary&        dict       = this->GetMetaDataDictionary();
  itk::MetaDataDictionary&        outputDict = this->GetOutput()->GetMetaDataDictionary();
  std::set<std::string>::iterator it         = m_ChangedKeys.begin();
  for (; it != m_ChangedKeys.end(); ++it)
  {
    if (dict.HasKey(*it))
    {
      // Replace metadata in output dictionary
      outputDict[*it] = dict[*it];
    }
    else
    {
      // Remove metadata from output dictionary
      outputDict.Erase(*it);
    }
  }
}

} // End of namespace OTB

#endif
