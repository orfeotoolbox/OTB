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

#ifndef otbImageMetadata_h
#define otbImageMetadata_h

#include "otbBandMetadata.h"
//#include "otbGeometryMetadata.h"
#include "otbMetaDataKey.h"
#include "OTBMetadataExport.h"

#include <boost/any.hpp>
#include <vector>
#include <string>
#include <ctime>
#include <bitset>
#include <map>
#include <unordered_map>
//~ #include <stdexcept>

namespace otb
{

/** \class ImageMetadataBase
 *
 *  \brief Metadata hybrid dictionary
 *
 *  Contains a dict of geometry parameters, several typed dictionaries for
 *  double, string, and other types. The different metadatas are retrieved
 *  with enum classes ( one for each type stored in the dictionary).
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT ImageMetadataBase
{
public:
  /** type of dictionary used internally */

  template <class TKey, class TVal>
  using DictType = std::map<TKey, TVal>;

  using Keywordlist = std::unordered_map<std::string, std::string>;

  /** Stack of geometry parameters, sorted by decreasing priority
   *  Cases with corresponding enum values:
   *    - projected
   *        * MDGeom::ProjectionWKT   -> WKT (string)
   *        * MDGeom::ProjectionEPSG  -> EPSG (integer)
   *        * MDGeom::ProjectionProj  -> proj4 (string)
   *    - sensor
   *        * MDGeom::RPC             -> RPCParam
   *        * MDGeom::SAR             -> SARParam
   *        * MDGeom::Adjustment      -> optional adjustment param (TBD)
   *        * MDGeom::GCP             -> GCPParam
   *        * MDGeom::SensorGeometry  -> other sensor model represented by a std::string (need a factory to deal with it)
   *    - epipolar ?
   *        + epipolar rectification grid
   *
   * Corner cases we may encounter:
   *   - sensor refinement: [RPC, GCP, adjustment]
   *   - piece-wise sensor model: [compound_RPC] ( handled as MDGeom::SensorGeometry)
   *   - raw georeferencing: [GCP]
   *   - Fine registration : [WKT, adjustment grid]
   */
  DictType<MDGeom, boost::any> GeometryKeys;

  DictType<MDNum, double> NumericKeys;

  DictType<MDStr, std::string> StringKeys;

  DictType<MDL1D, MetaData::LUT1D> LUT1DKeys;

  DictType<MDL2D, MetaData::LUT2D> LUT2DKeys;

  DictType<MDTime, MetaData::Time> TimeKeys;

  DictType<std::string, std::string> ExtraKeys;

  // TODO : iterators ?

  // -------------------- Geom utility function ----------------------------

  /** Read-only accessor to geometric keys */
  const boost::any & operator[](const MDGeom& key) const;
  
  /** Setter for geometric keys */
  void Add(const MDGeom& key, const boost::any &value);
  
  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(const MDGeom& key);
  
  /** Test if a key is available */
  bool Has(const MDGeom& key) const;

  // -------------------- Double utility function ----------------------------

  /** Read-only accessor to numeric keys */
  const double & operator[](const MDNum& key) const;
  
  /** Setter for numeric keys */
  void Add(const MDNum& key, const double &value);
  
  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(const MDNum& key);
  
  /** Test if a key is available */
  bool Has(const MDNum& key) const;
  
  // -------------------- String utility function ----------------------------

  /** Read-only accessor to string keys */
  const std::string & operator[](const MDStr& key) const;
  
  /** Setter for string keys */
  void Add(const MDStr& key, const std::string &value);
  
  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(const MDStr& key);
  
  /** Test if a key is available */
  bool Has(const MDStr& key) const;
  
  // -------------------- LUT1D utility function ----------------------------

  /** Read-only accessor to 1D LUT keys */
  const MetaData::LUT1D & operator[](const MDL1D& key) const;
  
  /** Setter for 1D LUT keys */
  void Add(const MDL1D& key, const MetaData::LUT1D &value);
  
  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(const MDL1D& key);
  
  /** Test if a key is available */
  bool Has(const MDL1D& key) const;
  
  // -------------------- 2D LUT utility function ----------------------------

  /** Read-only accessor to 2D LUT keys */
  const MetaData::LUT2D & operator[](const MDL2D& key) const;
  
  /** Setter for 2D LUT keys */
  void Add(const MDL2D& key, const MetaData::LUT2D &value);
  
  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(const MDL2D& key);
  
  /** Test if a key is available */
  bool Has(const MDL2D& key) const;
  
  // -------------------- Time utility function ----------------------------

  /** Read-only accessor to time keys */
  const MetaData::Time & operator[](const MDTime& key) const;
  
  /** Setter for time keys */
  void Add(const MDTime& key, const MetaData::Time &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(const MDTime& key);

  /** Test if a key is available */
  bool Has(const MDTime& key) const;

  // -------------------- Extra keys utility function --------------------------

  /** Read-only accessor to extra keys */
  const std::string & operator[](const std::string & key) const;

  /** Setter for extra keys */
  void Add(const std::string& key, const std::string &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(const std::string& key);

  /** Test if a key is available */
  bool Has(const std::string& key) const;

  // printing
  Keywordlist ToKeywordlist() const;
  
  std::string ToJSON(bool multiline=false) const;
};


/** \class ImageMetadata
 *
 * \brief Generic class containing image metadatas used in OTB (first try)
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT ImageMetadata: public ImageMetadataBase
{
public:
  /** Band-specific metadatas */
  std::vector<ImageMetadataBase> Bands;

  // utility functions
  /** Extract metadata from a subset of the bands */
  ImageMetadata slice(int start, int end);

  /** concatenate with an other ImageMetadata */
  void append(const ImageMetadata& );

  /** if all bands share the same value of a key, put it at top level */
  void compact();
};


/** \class ImageMetadataOLD
 *
 * \brief Generic class containing image metadatas used in OTB (first try)
 *
 * \ingroup OTBMetadata
 */
struct OTBMetadata_EXPORT ImageMetadataOLD
{
// ------------------------------ static part ----------------------------------

  std::string ProjectionRef;

  std::string GCPProjection;

  std::vector<OTB_GCP> GCPs;

  //~ // TODO : remove if not used
  //~ double GeoTransform[6] = {0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
//~ 
  //~ // TODO : remove if not used
  //~ double PixelSizeX = 0.0;
  //~ double PixelSizeY = 0.0;
//~ 
  //~ // TODO : remove if not used
  //~ /** Corners coordinate */
  //~ double ULX = 0.0;
  //~ double ULY = 0.0;
  //~ double URX = 0.0;
  //~ double URY = 0.0;
  //~ double LLX = 0.0;
  //~ double LLY = 0.0;
  //~ double LRX = 0.0;
  //~ double LRY = 0.0;

  /** Tile Hint -> MDNum */
  //~ unsigned int TileHintX = 0;
  //~ unsigned int TileHintY = 0;

  //~ int DataType = 0;
  
  /** Band-specific metadatas */
  std::vector<BandMetadata> Bands;

  // pas conseillé, recursion plus couteuse à contrôler dans le code
  //~ vector<ImageMetadata> Childs;
  // ------------------------------ dynamic part ---------------------------------
  
  // TODO: check the concept, maybe use boost/type_erasure with properties ostreamable, ...
  /** Sensor model geometry */
  std::vector<boost::any> SensorGeometry;

  std::map<MDNum, double> NumericKeys;

  std::map<MDStr, std::string> StringKeys;

  std::map<MDL1D, MetaData::LUT1D > LUT1DKeys;

  std::map<MDL2D, MetaData::LUT2D > LUT2DKeys;

  std::map<MDTime, MetaData::Time> TimeKeys;

  //~ std::map<string,string> Extras;

  // ------------------------ access functions ---------------------------------
  double & operator[](const MDNum& key)
    {
    return NumericKeys.at(key);
    }

  std::string & operator[](const MDStr& key)
    {
    return StringKeys.at(key);
    }
  // TODO: prefer using map.at ? YES

  // TODO : add missing access functions
};

extern OTBMetadata_EXPORT std::ostream& operator<<(std::ostream& os, const otb::ImageMetadataBase& imd);

extern OTBMetadata_EXPORT std::ostream& operator<<(std::ostream& os, const otb::ImageMetadata& imd);

} // end namespace otb

#endif
