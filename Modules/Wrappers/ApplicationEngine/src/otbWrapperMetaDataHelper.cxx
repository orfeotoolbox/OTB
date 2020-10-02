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

#include "otbWrapperMetaDataHelper.h"
#include "itkMetaDataObject.h"

namespace otb
{
namespace Wrapper
{
namespace MetaDataHelper
{

MDType GetType(const std::string& val)
{
  MDType                    ret(MDType::String);
  otb::MetaDataKey::KeyType kt = otb::MetaDataKey::GetKeyType(val);
  switch (kt)
  {
  case otb::MetaDataKey::TSTRING:
    ret = MDType::String;
    break;
  case otb::MetaDataKey::TENTIER:
    ret = MDType::Int;
    break;
  case otb::MetaDataKey::TDOUBLE:
    ret = MDType::Double;
    break;
  case otb::MetaDataKey::TOTB_GCP:
    ret = MDType::GCP;
    break;
  case otb::MetaDataKey::TVECTOR:
    ret = MDType::Vector;
    break;
  case otb::MetaDataKey::TOSSIMKEYWORDLIST:
    ret = MDType::ImageKWL;
    break;
  case otb::MetaDataKey::TVECTORDATAKEYWORDLIST:
    ret = MDType::VectorDataKWL;
    break;
  case otb::MetaDataKey::TBOOLVECTOR:
    ret = MDType::BoolVector;
    break;
  default:
    break;
  }
  return ret;
}

std::string GetString(const itk::MetaDataDictionary& dict, const std::string& key)
{
  std::string ret;
  itk::ExposeMetaData<std::string>(dict, key, ret);
  return ret;
}

void SetString(itk::MetaDataDictionary& dict, const std::string& key, const std::string& val)
{
  itk::EncapsulateMetaData<std::string>(dict, key, val);
}

unsigned int GetInt(const itk::MetaDataDictionary& dict, const std::string& key)
{
  unsigned int ret = 0;
  itk::ExposeMetaData<unsigned int>(dict, key, ret);
  return ret;
}

void SetInt(itk::MetaDataDictionary& dict, const std::string& key, unsigned int val)
{
  itk::EncapsulateMetaData<unsigned int>(dict, key, val);
}

double GetDouble(const itk::MetaDataDictionary& dict, const std::string& key)
{
  double ret = 0.0;
  itk::ExposeMetaData<double>(dict, key, ret);
  return ret;
}

void SetDouble(itk::MetaDataDictionary& dict, const std::string& key, double val)
{
  itk::EncapsulateMetaData<double>(dict, key, val);
}

otb::OTB_GCP GetGCP(const itk::MetaDataDictionary& dict, const std::string& key)
{
  otb::OTB_GCP ret;
  itk::ExposeMetaData<otb::OTB_GCP>(dict, key, ret);
  return ret;
}

void SetGCP(itk::MetaDataDictionary& dict, const std::string& key, const otb::OTB_GCP& val)
{
  itk::EncapsulateMetaData<otb::OTB_GCP>(dict, key, val);
}

otb::MetaDataKey::VectorType GetVector(const itk::MetaDataDictionary& dict, const std::string& key)
{
  otb::MetaDataKey::VectorType ret;
  itk::ExposeMetaData<otb::MetaDataKey::VectorType>(dict, key, ret);
  return ret;
}

void SetVector(itk::MetaDataDictionary& dict, const std::string& key, const otb::MetaDataKey::VectorType& val)
{
  itk::EncapsulateMetaData<otb::MetaDataKey::VectorType>(dict, key, val);
}

otb::ImageKeywordlist GetImageKWL(const itk::MetaDataDictionary& dict, const std::string& key)
{
  otb::ImageKeywordlist ret;
  itk::ExposeMetaData<otb::ImageKeywordlist>(dict, key, ret);
  return ret;
}

void SetImageKWL(itk::MetaDataDictionary& dict, const std::string& key, const otb::ImageKeywordlist& val)
{
  itk::EncapsulateMetaData<otb::ImageKeywordlist>(dict, key, val);
}

otb::VectorDataKeywordlist GetVectorDataKWL(const itk::MetaDataDictionary& dict, const std::string& key)
{
  otb::VectorDataKeywordlist ret;
  itk::ExposeMetaData<otb::VectorDataKeywordlist>(dict, key, ret);
  return ret;
}

void SetVectorDataKWL(itk::MetaDataDictionary& dict, const std::string& key, const otb::VectorDataKeywordlist& val)
{
  itk::EncapsulateMetaData<otb::VectorDataKeywordlist>(dict, key, val);
}

otb::MetaDataKey::BoolVectorType GetBoolVector(const itk::MetaDataDictionary& dict, const std::string& key)
{
  otb::MetaDataKey::BoolVectorType ret;
  itk::ExposeMetaData<otb::MetaDataKey::BoolVectorType>(dict, key, ret);
  return ret;
}

void SetBoolVector(itk::MetaDataDictionary& dict, const std::string& key, const otb::MetaDataKey::BoolVectorType& val)
{
  itk::EncapsulateMetaData<otb::MetaDataKey::BoolVectorType>(dict, key, val);
}

} // end of namespace MetaDataHelper
} // end of namespace Wrapper
} // end of namespace otb
