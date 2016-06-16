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
#ifndef otbChangeInformationImageFilter_txx
#define otbChangeInformationImageFilter_txx

#include "otbChangeInformationImageFilter.h"
#include "itkMetaDataObject.h"

namespace otb
{

template< typename TInputImage >
void
ChangeInformationImageFilter<TInputImage>
::SetChangeMetaData(const char *keyname, bool flag)
{
  std::string key(keyname);
  if (! key.empty())
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

template< typename TInputImage >
bool
ChangeInformationImageFilter<TInputImage>
::GetChangeMetaData(const char *keyname)
{
  std::string key(keyname);
  if (! key.empty())
    {
    if (m_ChangedKeys.find(key) != m_ChangedKeys.end())
      {
      return true;
      }
    }
  return false;
}

template< typename TInputImage >
template<typename T>
void
ChangeInformationImageFilter<TInputImage>
::SetOutputMetaData(const char *keyname, const T * value)
{
  std::string key(keyname);
  if (! key.empty())
    {
    // enable this key for metadata change
    m_ChangedKeys.insert(key);
    itk::MetaDataDictionary &dict = this->GetMetaDataDictionary();
    if (value == ITK_NULLPTR)
      {
      // Remove meta-data from dictionary
      this->RemoveKeyFromDictionary(dict,key);
      }
    else
      {
      // Set metadata in dictionary
      const T &valueRef = (*value);
      itk::EncapsulateMetaData<T>(dict,key,valueRef);
      }
    }
}

template< typename TInputImage >
void
ChangeInformationImageFilter<TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  // Process the metadatas to be changed
  itk::MetaDataDictionary &dict = this->GetMetaDataDictionary();
  itk::MetaDataDictionary &outputDict = this->GetOutput()->GetMetaDataDictionary();
  std::set<std::string>::iterator it = m_ChangedKeys.begin();
  for ( ; it != m_ChangedKeys.end() ; ++it)
    {
    if (dict.HasKey(*it))
      {
      // Replace metadata in output dictionary
      outputDict[*it] = dict[*it];
      }
    else
      {
      // Remove metadata from output dictionary
      this->RemoveKeyFromDictionary(outputDict,*it);
      }
    }
}

template< typename TInputImage >
bool
ChangeInformationImageFilter<TInputImage>
::RemoveKeyFromDictionary(itk::MetaDataDictionary & dict, const std::string & key)
{
  std::vector<std::string> keyList = dict.GetKeys();
  std::vector<std::string>::iterator pos = keyList.begin();
  while (pos != keyList.end())
    {
    if (key.compare(*pos) == 0)
      {
      break;
      }
    ++pos;
    }
  if (pos != keyList.end())
    {
    itk::MetaDataDictionary copyDict;
    keyList.erase(pos);
    pos = keyList.begin();
    for ( ; pos != keyList.end();++pos)
      {
      copyDict.Set(*pos, const_cast<itk::MetaDataObjectBase*>(dict.Get(*pos)));
      }
    dict = copyDict;
    return true;
    }
  return false;
}

} // End of namespace OTB

#endif
