/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbApplicationWithNoData_h
#define otbApplicationWithNoData_h

#include "OTBApplicationEngineExport.h"
#include "otbWrapperApplication.h"
#include <sstream>

namespace otb
{

namespace Wrapper
{

/**
 * Helper _decorator_ parent class for applications that need to inject `&nodata=` in metadata.
 *
 * This decorator (for applications) provides a factorized way to define applications that receive
 * an optional "nodata" parameter that will be used in their output images.
 *
 * Input-nodata values are expected to be extracted from the input images, not set from the
 * application.
 *
 * This class provides two services:
 * - `DoInit_NoData()` meant to be called from `DoInit()` specialisation.
 * - `AddNodataInMetadataThroughExtendedFilename()` meant to be called from
 *   `DoExecute()` specialisation.
 *
 * \author Luc Hermitte
 * \copyright CS Group
 * \ingroup OTBApplicationEngine
 */
template <typename TNoData, typename TApplication = Application>
class OTBApplicationEngine_EXPORT_TEMPLATE ApplicationWithNoData : public TApplication
{
protected:
  ApplicationWithNoData(
      std::string nodata_key           = "nodata",
      TNoData     nodata_default_value = -32768)
  : m_nodata_key(std::move(nodata_key))
  , m_nodata_default_value(nodata_default_value)
  {}

  /**
   * Registers a `-nodata` parameter.
   */
  void DoInit_NoData()
  {
    this->template AddParameter<parameter_type_v<TNoData>>(
        m_nodata_key,
        "NoData value",
        "Output cells with no data are filled with this value (optional " + std::to_string(m_nodata_default_value) + " by default)"
    );
    parameter_type<TNoData>::set_default(
        *this,
        m_nodata_key,
        m_nodata_default_value);
    this->MandatoryOff(m_nodata_key);
  }

  TNoData GetParameterNodata() const
  {
    return parameter_type<TNoData>::get(*this, m_nodata_key);
  }

  /**
   * Appends `&nodata={value}` at the end of the extended filename to force nodata to be added in metadata.
   */
  void AddNodataInMetadataThroughExtendedFilename(
      TNoData            nodata,
      std::string const& out_param = "out")
  {
    auto const origin_FileName = this->GetParameterString(out_param);
    std::ostringstream oss;
    oss << origin_FileName;

    // Check if FileName is extended (with the ? character)
    // If not extended then override the FileName
    auto const extension_start = origin_FileName.find('?');
    if (extension_start == std::string::npos && !origin_FileName.empty())
      oss << '?';
    else
    {
      auto const nodata_start = origin_FileName.find("&nodata=");
      if (nodata_start != std::string::npos && nodata_start > extension_start)
      {
        // Let's trust the end-user. Even if the value doesn't match
        otbLogMacro(Warning,
                    << "Trusting the nodata value required in extended filename. "
                    << "User specified parameter -" << m_nodata_key << "=" << nodata << " won't be propagated to image metadata");
        return;
      }
    }
    oss << "&nodata=" << nodata;
    // Set the new FileName with extended options
    this->SetParameterString(out_param, oss.str());
  }

private:
  std::string m_nodata_key;
  TNoData     m_nodata_default_value = nan;
};

} //end namespace Wrapper

} //end namespace otb

#endif // otbApplicationWithNoData_h
