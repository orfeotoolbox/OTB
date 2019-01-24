/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
 * \namespace MetaDataHelper
 *
 * \brief Contains small helper functions to manipulate itk::MetaDataDictionary
 *
 * These functions are only here to instanciate the different template function that
 * allow to get/set values in MetaDataDictionary.
 *
 */
namespace MetaDataHelper
{

  enum class MDType
  {
    String,
    Int,
    Double,
    GCP,
    Vector,
    ImageKWL,
    VectorDataKWL,
    BoolVector
  };

  OTBApplicationEngine_EXPORT MDType GetType(const std::string &val);

  OTBApplicationEngine_EXPORT std::string GetString(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  OTBApplicationEngine_EXPORT void SetString(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const std::string &val);

  OTBApplicationEngine_EXPORT unsigned int GetInt(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  OTBApplicationEngine_EXPORT void SetInt(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    unsigned int val);

  OTBApplicationEngine_EXPORT double GetDouble(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  OTBApplicationEngine_EXPORT void SetDouble(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    double val);

  OTBApplicationEngine_EXPORT otb::OTB_GCP GetGCP(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  OTBApplicationEngine_EXPORT void SetGCP(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::OTB_GCP &val);

  OTBApplicationEngine_EXPORT otb::MetaDataKey::VectorType GetVector(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  OTBApplicationEngine_EXPORT void SetVector(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::MetaDataKey::VectorType &val);

  OTBApplicationEngine_EXPORT otb::ImageKeywordlist GetImageKWL(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  OTBApplicationEngine_EXPORT void SetImageKWL(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::ImageKeywordlist &val);

  OTBApplicationEngine_EXPORT otb::VectorDataKeywordlist GetVectorDataKWL(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  OTBApplicationEngine_EXPORT void SetVectorDataKWL(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::VectorDataKeywordlist &val);

  OTBApplicationEngine_EXPORT otb::MetaDataKey::BoolVectorType GetBoolVector(
    const itk::MetaDataDictionary &dict,
    const std::string &key);
  OTBApplicationEngine_EXPORT void SetBoolVector(
    itk::MetaDataDictionary &dict,
    const std::string &key,
    const otb::MetaDataKey::BoolVectorType &val);

} // end of namespace MetaDataHelper

} // end of namespace Wrapper
} // end of namespace otb

#endif
