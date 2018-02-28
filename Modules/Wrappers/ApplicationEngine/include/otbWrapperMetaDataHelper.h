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

#ifndef otbWrapperMetaDataHelper_h
#define otbWrapperMetaDataHelper_h

#include "otbMetaDataKey.h"
#include "otbImageKeywordlist.h"
#include "otbVectorDataKeywordlist.h"
#include "itkMetaDataDictionary.h"
#include "OTBApplicationEngineExport.h"

namespace otb
{
namespace Wrapper
{

/**
 * \class MetaDataHelper
 *
 * \brief Small helper to manipulate itk::MetaDataDictionary
 *
 * This class is only here to instanciate the different template function that
 * allow to get/set values in MetaDataDictionary. All functions are static.
 * 
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT MetaDataHelper
{
public:
  MetaDataHelper();
  virtual ~MetaDataHelper();

  typedef enum
  {
    MDType_STRING,
    MDType_INT,
    MDType_DOUBLE,
    MDType_GCP,
    MDType_VECTOR,
    MDType_IMAGEKWL,
    MDType_VECTORDATAKWL,
    MDType_BOOLVECTOR
  } MDType;

  static MDType GetType(const std::string &val);

  static std::string GetString(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  static void SetString(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const std::string &val);

  static unsigned int GetInt(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  static void SetInt(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    unsigned int val);

  static double GetDouble(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  static void SetDouble(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    double val);

  static otb::OTB_GCP GetGCP(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  static void SetGCP(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::OTB_GCP &val);

  static otb::MetaDataKey::VectorType GetVector(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  static void SetVector(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::MetaDataKey::VectorType &val);

  static otb::ImageKeywordlist GetImageKWL(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  static void SetImageKWL(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::ImageKeywordlist &val);

  static otb::VectorDataKeywordlist GetVectorDataKWL(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  static void SetVectorDataKWL(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::VectorDataKeywordlist &val);

  static otb::MetaDataKey::BoolVectorType GetBoolVector(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  static void SetBoolVector(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::MetaDataKey::BoolVectorType &val);
};

} // end of namespace Wrapper
} // end of namespace otb

#endif
