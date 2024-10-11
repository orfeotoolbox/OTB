/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOGRExtendedFilenameToOptions_h
#define otbOGRExtendedFilenameToOptions_h

#include <unordered_map>
#include "otbExtendedFilenameHelper.h"

namespace otb
{

/** \class OGRExtendedFilenameToOptions
 *  \brief This class aim at processing GDAL option that can be pass through
 * extended filename.
 * \ingroup OTBExtendedFilename
 * \ingroup OTBGdalAdapters
 *
 */
#include "OTBGdalAdaptersExport.h"

class OTBGdalAdapters_EXPORT OGRExtendedFilenameToOptions : public ExtendedFilenameHelper
{
public:
  /** Standard class typedefs. */
  typedef OGRExtendedFilenameToOptions  Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef ExtendedFilenameHelper        Superclass;

  typedef Superclass::OptionMapType     OptionMapType;
  typedef OptionMapType::const_iterator ConstMapIteratorType;
  typedef std::vector<std::string>      GDALOptionType;

  itkTypeMacro(OGRExtendedFilenameToOptions, otb::ExtendedFilenameHelper);
  itkNewMacro(Self);


  /** The creation option structure. */
  struct OpenOptionType
  {
    GDALOptionType gdalOptions;
    // std::unordered_map< std::string , bool > availableOptions;
  };

  struct CreationOptionType
  {
    GDALOptionType gdalOptions;
    // std::unordered_map< std::string , bool > availableOptions;
  };

  struct LayerOptionType
  {
    std::unordered_map<std::string, std::string> gdalOptions;
  };

  /** \deprecated const char* overload of SetExtendedFileName is deprecated, use std::string instead */
  void SetExtendedFileName(const char* extFname) override;

  void SetExtendedFileName(const std::string& extFname) override;

  /** Get the GDAL option for type operation */
  GDALOptionType GetGDALOptions(const std::string& type) const;

  /** Get the deffierent GDAL options*/
  GDALOptionType GetGDALOpenOptions() const;
  GDALOptionType GetGDALCreationOptions() const;
  GDALOptionType GetGDALLayerOptions() const;

  bool SimpleFileNameIsSet() const;
  bool HasGDALLayerOption() const;

  /** Set GDAL layer option through a vector of string */
  void SetGDALLayerOptions(const GDALOptionType& options);

  /** Add GDAL layer option to existing one */
  void AddGDALLayerOptions(const GDALOptionType& options);

  /** Constructor that return a pointer to an OGRExtendedFilename with
   * GDAL layer option as options
   */
  static Pointer GetGDALLayerOptionsHelper(const GDALOptionType& options);

protected:
  OGRExtendedFilenameToOptions();
  OGRExtendedFilenameToOptions(const GDALOptionType& options);
  ~OGRExtendedFilenameToOptions() override{};

private:
  OGRExtendedFilenameToOptions(const Self&) = delete;
  void operator=(const Self&) = delete;

  OpenOptionType     m_OpenOptions;
  CreationOptionType m_CreationOptions;
  LayerOptionType    m_LayerOptions;
  bool               m_HasFileName;
};

} // end namespace otb

#endif // otbOGRExtendedFilenameToOptions_h
