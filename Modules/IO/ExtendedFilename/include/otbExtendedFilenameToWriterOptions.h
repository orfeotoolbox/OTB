/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2018 CS Systemes d'Information (CS SI)
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

#ifndef otbExtendedFilenameToWriterOptions_h
#define otbExtendedFilenameToWriterOptions_h

#include "otbExtendedFilenameHelper.h"
#include "otbGDALImageIO.h"
#include <string>

namespace otb
{

/** \class ExtendedFilenameToWriterOptions
 *  \brief Converts an extended filename to writer options.
 *
 * Available options for extended file name are:
 * - &writegeom=ON : to activate the creation of an external geom file
 * - &gdal:co:<KEY>=<VALUE> : the gdal creation option <KEY>
 * - streaming modes
 * - box
 * See http://wiki.orfeo-toolbox.org/index.php/ExtendedFileName
 *
 *  \sa ImageFileWriter
 *
 * \ingroup OTBExtendedFilename
 */

class ITK_EXPORT ExtendedFilenameToWriterOptions : public ExtendedFilenameHelper
{
public:
  /** Standard class typedefs. */
  typedef ExtendedFilenameToWriterOptions Self;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;
  typedef ExtendedFilenameHelper          Superclass;

  itkTypeMacro(ExtendedFilenameToWriterOptions, otb::ExtendedFilenameHelper);
  itkNewMacro(Self);

  typedef Superclass                     FNameHelperType;
  typedef FNameHelperType::OptionMapType MapType;
  typedef MapType::iterator              MapIteratorType;

  /** The writer option structure. */
  typedef GDALImageIO::GDALCreationOptionsType GDALCOType;


  struct OptionType
  {
    std::pair<bool, std::string> simpleFileName;
    std::pair<bool, bool>        writeGEOMFile;
    std::pair<bool, bool>        writeRPCTags;
    std::pair<bool, bool>        multiWrite;
    std::pair<bool, GDALCOType>  gdalCreationOptions;
    std::pair<bool, std::string> streamingType;
    std::pair<bool, std::string> streamingSizeMode;
    std::pair<bool, double>      streamingSizeValue;
    std::pair<bool, std::string> box;
    std::pair<bool, std::string> bandRange;
    std::vector<std::string> optionList;
  };

  typedef std::pair<int, double> NoDataPairType;
  typedef std::vector<NoDataPairType> NoDataListType;
  NoDataListType                      m_NoDataList;
  bool                                has_noDataValue;

  /** \deprecated const char* overload of SetExtendedFileName is deprecated, use std::string instead */
  void SetExtendedFileName(const char* extFname) override;

  void SetExtendedFileName(const std::string& extFname) override;

  /* Get Methods */
  bool           SimpleFileNameIsSet() const;
  bool           NoDataValueIsSet() const;
  bool           WriteGEOMFileIsSet() const;
  bool           WriteRPCTagsIsSet() const;
  bool           GetMultiWrite() const;
  NoDataListType GetNoDataList() const
  {
    return m_NoDataList;
  }

  bool        GetWriteGEOMFile() const;
  bool        GetWriteRPCTags() const;
  bool        gdalCreationOptionsIsSet() const;
  GDALCOType  GetgdalCreationOptions() const;
  bool        StreamingTypeIsSet() const;
  std::string GetStreamingType() const;
  bool        StreamingSizeModeIsSet() const;
  std::string GetStreamingSizeMode() const;
  bool        StreamingSizeValueIsSet() const;
  double      GetStreamingSizeValue() const;
  std::string GetBandRange() const;

  bool        BoxIsSet() const;
  std::string GetBox() const;

  /** Test if band range extended filename is set */
  bool BandRangeIsSet() const;

protected:
  ExtendedFilenameToWriterOptions();
  ~ExtendedFilenameToWriterOptions() override
  {
  }

private:
  ExtendedFilenameToWriterOptions(const Self&) = delete;
  void operator=(const Self&) = delete;

  OptionType m_Options;
};
} // end namespace otb

#endif // otbExtendedFilenameToWriterOptions_h
