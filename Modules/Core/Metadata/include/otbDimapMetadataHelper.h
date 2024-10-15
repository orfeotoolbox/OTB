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

#ifndef otbMetadataDataHelper_h
#define otbMetadataDataHelper_h

#include "OTBMetadataExport.h"
#include "otbMetadataSupplierInterface.h"
#include "otbSpot5Metadata.h"
#include "otbDateTime.h"


namespace otb
{

/** \class DimapData
 *
 * \brief Struct containing metadata parsed from a Dimap product
 *
 * \ingroup OTBMetadata
 */
struct DimapData
{
  std::string mission;
  std::string missionIndex;

  std::string ImageID;
  std::string ProductionDate;
  std::string AcquisitionDate;
  std::string Instrument;
  std::string InstrumentIndex;
  std::string ProcessingLevel;
  std::string SpectralProcessing;

  std::vector<std::string> BandIDs;

  std::vector<double>  SunAzimuth;
  std::vector<double>  SunElevation;
  std::vector<double>  IncidenceAngle;
  std::vector<double>  ViewingAngle;
  std::vector<double>  AzimuthAngle;
  std::vector<double>  SceneOrientation;

  std::vector<double>  PhysicalBias;
  std::vector<double>  PhysicalGain;
  
  std::vector<double>  SolarIrradiance;

  // Optional 
  std::vector<double> AlongTrackIncidenceAngle;
  std::vector<double> AcrossTrackIncidenceAngle;
  std::vector<double> AlongTrackViewingAngle;
  std::vector<double> AcrossTrackViewingAngle;
  int StepCount;
  std::string softwareVersion;
  double SatAzimuth;

  // phr sensor characteristics
  std::string TimeRangeStart;
  std::string TimeRangeEnd;
  std::string LinePeriod;
  std::string SwathFirstCol;
  std::string SwathLastCol;
};

/** \class DimapMetadataHelper
 *
 * \brief Helper class to read dimap data from various sources (Dimap v1, dimap v2, Ossim geom file) and various sensors (PHR, SPOT, ...)
 * and store them in a DimapData struct
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT DimapMetadataHelper
{
public:

  DimapMetadataHelper() = default;
  
  const DimapData & GetDimapData() const
  {
    return m_Data;
  }


  /** Parse Dimap data from an Ossim geom file. This method can be used to parse geom generated with OTB <= 7.0 */
  void ParseGeom(const MetadataSupplierInterface & mds);

  /** Parse Dimap data from a Dimap v1 product */
  void ParseDimapV1(const MetadataSupplierInterface & mds, const std::string prefix);

  /** Parse Dimap data from a Dimap v2 product */
  void ParseDimapV2(const MetadataSupplierInterface & mds, const std::string & prefix = "Dimap_Document.");

  void ParseSpot5Model(const MetadataSupplierInterface & mds, Spot5Param& spot5Param, const std::string & prefix = "Dimap_Document.");

protected:

private:

  template <class T>
  void ParseVector(const MetadataSupplierInterface & mds, 
                    const std::string & prefix, 
                    const std::string & name,
                    std::vector<T> & dest) 
  {
    dest.clear();

    std::vector<std::string> mdStr;

    bool hasValue = false;
    auto value = mds.GetMetadataValue(prefix + "." + name, hasValue);

    if (hasValue)
    {
      mdStr.push_back(value);
    }
    else
    {
      hasValue = true;
      unsigned int i = 1;
      while(hasValue)
      {
        value = mds.GetMetadataValue(prefix + "_" + std::to_string(i) + "." + name, hasValue);
        if (hasValue)
          mdStr.push_back(value);
        i++;
      }
    }

    if (mdStr.empty())
    {
      otbGenericExceptionMacro(MissingMetadataException,
                          <<"Missing metadata '"<< prefix + name <<"'")
    }

    for (const auto & elem : mdStr)
    {
      try
      {
        dest.push_back(boost::lexical_cast<T>(elem));
      }
      catch (boost::bad_lexical_cast&)
      {
        otbGenericExceptionMacro(MissingMetadataException,<<"Bad metadata value for '"<<prefix + name<<"', got: "<<elem)
      }
    }

    //dest.push_back( );
  }
  
  // Non throwing version
  template <class T>
  void ParseVector(const MetadataSupplierInterface & mds, 
                    const std::string & prefix, 
                    const std::string & name,
                    std::vector<T> & dest,
                    std::vector<T> & defaultValue) 
  {
    //TODO factorize
    dest.clear();

    std::vector<std::string> mdStr;

    bool hasValue = false;
    auto value = mds.GetMetadataValue(prefix + "." + name, hasValue);

    if (hasValue)
    {
      mdStr.push_back(value);
    }
    else
    {
      hasValue = true;
      unsigned int i = 1;
      while(hasValue)
      {
        value = mds.GetMetadataValue(prefix + "_" + std::to_string(i) + "." + name, hasValue);
        if (hasValue)
          mdStr.push_back(value);
        i++;
      }
    }

    if (mdStr.empty())
    {
      dest = defaultValue;
    }

    for (const auto & elem : mdStr)
    {
      try
      {
        dest.push_back(boost::lexical_cast<T>(elem));
      }
      catch (boost::bad_lexical_cast&)
      {
        dest = defaultValue;
      }
    }
  }

  template <class T>
  T GetSingleValueFromList(const MetadataSupplierInterface & mds, 
                    const std::string & prefix, 
                    const std::string & name)
  {
    std::vector<T> vector;
    ParseVector(mds, prefix, name, vector);
    return vector[0];

  }

  double GetTime(const std::string& timeStr){
    // Time stamps are in the format: "yyyy-mm-ddThh:mm:ss.ssssss"
    const auto d = MetaData::ReadFormattedDate(timeStr);
    return d.GetSecond() + 60.0 * (
             d.GetMinute() + 60.0 * ( // Total NB of minutes
               d.GetHour() + 24.0 * ( // Total NB of hours
                 d.GetDay() - 1.0 + 365.25 * ( // Total NB of days (-1 as day is not over)
                   d.GetYear() - 2002.0
                 )
               )
             )
           );
  }

  DimapData m_Data;
};


} // end namespace otb

#endif

