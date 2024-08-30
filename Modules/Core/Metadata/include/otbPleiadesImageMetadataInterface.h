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

#ifndef otbPleiadesImageMetadataInterface_h
#define otbPleiadesImageMetadataInterface_h

#include "otbOpticalImageMetadataInterface.h"
#include <string>
#include "otbDateTime.h"

namespace otb
{
/** \class PleiadesImageMetadataInterface
 *
 * \brief Creation of an "otb" PleiadesImageMetadataInterface that gets metadata.
 *
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT PleiadesImageMetadataInterface : public OpticalImageMetadataInterface
{
public:
  typedef PleiadesImageMetadataInterface Self;
  typedef OpticalImageMetadataInterface  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PleiadesImageMetadataInterface, OpticalImageMetadataInterface);

  typedef Superclass::ImageType                ImageType;
  typedef Superclass::MetaDataDictionaryType   MetaDataDictionaryType;
  typedef Superclass::VectorType               VectorType;
  typedef Superclass::VariableLengthVectorType VariableLengthVectorType;

  void Parse(ImageMetadata &) override;

protected:
  PleiadesImageMetadataInterface();
  ~PleiadesImageMetadataInterface() = default;

private:
  PleiadesImageMetadataInterface(const Self&) = delete;
  void operator=(const Self&) = delete;

  void FetchTabulatedPhysicalGain(const MetaData::TimePoint & date, ImageMetadata& imd);
  void FetchSolarIrradiance(const std::vector<double> & dimapSolarIrradiance, ImageMetadata& imd);

  void FetchSatAngles(const std::vector<double> & incidenceAngles,
                      const std::vector<double> & alongTrackIncidenceAngles,
                      const std::vector<double> & axrossTrackIncidenceAngles,
                      const std::vector<double> & sceneOrientation,
                      ImageMetadata & imd);

  /** Vector that contains the filter function value in 6S format (step of 0.0025 micro m).
     * There values a computed by 6S. */
  void FetchSpectralSensitivity(const std::string & sensorId, ImageMetadata &imd);
};


namespace MetaData
{
namespace PleiadesUtils
{
  const std::string IMAGE_ID_KEY = "ImageID";
  const std::string TIME_RANGE_START_KEY = "TimeRangeStart";
  const std::string TIME_RANGE_END_KEY = "TimeRangeEnd";
  const std::string LINE_PERIOD_KEY = "LinePeriod";
  const std::string SWATH_FIRST_COL_KEY = "SwathFirstCol";
  const std::string SWATH_LAST_COL_KEY = "SwathLastCol";

  /** This struct defines additional metadata used to enhance pansharpening of phr products */
  struct SensorModelCharacteristics
  {
    std::string imageID;
    MetaData::TimePoint timeRangeStart;
    MetaData::TimePoint timeRangeEnd;
    double  linePeriod;
    int     swathFirstCol;
    int     swathLastCol;
  };

  bool HasSensorModelCharacteristics(const ImageMetadata &);

  /** extract sensor model characteristics from ImageMetadata */ 
  SensorModelCharacteristics GetSensorModelCharacteristics(const ImageMetadata &);

} // end namespace PleiadesUtils
} // end namespace MetaData



} // end namespace otb

#endif
