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

#ifndef otbImageMetadata_h
#define otbImageMetadata_h

#include "otbGeometryMetadata.h"
#include "otbSARMetadata.h"
#include "otbMetaDataKey.h"
#include "OTBMetadataExport.h"
#include "otbSpot5Metadata.h"
#include "otbMacro.h"

#include <boost/any.hpp>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

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

  DictType<MDTime, MetaData::TimePoint> TimeKeys;

  DictType<std::string, std::string> ExtraKeys;

  // Constructor
  ImageMetadataBase() = default;
  ImageMetadataBase(DictType<MDGeom, boost::any> geometryKeys,
                    DictType<MDNum, double> numericKeys,
                    DictType<MDStr, std::string> stringKeys,
                    DictType<MDL1D, MetaData::LUT1D> lut1DKeys,
                    DictType<MDL2D, MetaData::LUT2D> lut2DKeys,
                    DictType<MDTime, MetaData::TimePoint> timeKeys,
                    DictType<std::string, std::string> extraKeys);

  // -------------------- Geom utility function ----------------------------

  /** Read-only accessor to geometric keys */
  const boost::any & operator[](MDGeom key) const;

  const Projection::GCPParam & GetGCPParam() const;
  const Projection::RPCParam & GetRPCParam() const;
  const SARParam & GetSARParam() const;
  const Spot5Param & GetSpot5Param() const;

  std::string GetProjectedGeometry() const;

  std::string GetProjectionWKT() const;

  std::string GetProjectionProj() const;

  /** Setter for geometric keys */
  void Add(MDGeom key, const boost::any &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(MDGeom key);

  size_t RemoveSensorGeometry();

  size_t RemoveProjectedGeometry();

  /** Test if a key is available */
  bool Has(MDGeom key) const;

  bool HasSensorGeometry() const;

  bool HasProjectedGeometry() const;

  // -------------------- Double utility function ----------------------------

  /** Read-only accessor to numeric keys */
  const double & operator[](MDNum key) const;

  /** Setter for numeric keys */
  void Add(MDNum key, const double &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(MDNum key);

  /** Test if a key is available */
  bool Has(MDNum key) const;

  /** Return the list of valid keys */
  std::string GetKeyListNum() const;

  // -------------------- String utility function ----------------------------

  /** Read-only accessor to string keys */
  const std::string & operator[](MDStr key) const;

  /** Setter for string keys */
  void Add(MDStr key, const std::string &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(MDStr key);

  /** Test if a key is available */
  bool Has(MDStr key) const;

  /** Return the list of valid keys */
  std::string GetKeyListStr() const;

  // -------------------- LUT1D utility function ----------------------------

  /** Read-only accessor to 1D LUT keys */
  const MetaData::LUT1D & operator[](MDL1D key) const;

  /** Setter for 1D LUT keys */
  void Add(MDL1D key, const MetaData::LUT1D &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(MDL1D key);

  /** Test if a key is available */
  bool Has(MDL1D key) const;

  /** Return the list of valid keys */
  std::string GetKeyListL1D() const;

  // -------------------- 2D LUT utility function ----------------------------

  /** Read-only accessor to 2D LUT keys */
  const MetaData::LUT2D & operator[](MDL2D key) const;

  /** Setter for 2D LUT keys */
  void Add(MDL2D key, const MetaData::LUT2D &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(MDL2D key);

  /** Test if a key is available */
  bool Has(MDL2D key) const;

  /** Return the list of valid keys */
//  std::string GetKeyListL2D() const;

  // -------------------- Time utility function ----------------------------

  /** Read-only accessor to time keys */
  const MetaData::TimePoint & operator[](MDTime key) const;

  /** Setter for time keys */
  void Add(MDTime key, const MetaData::TimePoint &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(MDTime key);

  /** Test if a key is available */
  bool Has(MDTime key) const;

  /** Return the list of valid keys */
  std::string GetKeyListTime() const;

  // -------------------- Extra keys utility function --------------------------

  /** Read-only accessor to extra keys */
  const std::string & operator[](const std::string & key) const;

  /** Setter for extra keys */
  void Add(const std::string& key, const std::string &value);

  /** Remove a key from the dictionary (even if the key is already missing) */
  size_t Remove(const std::string& key);

  /** Test if a key is available */
  bool Has(const std::string& key) const;

  // -------------------- Other --------------------------

  /** Fill a KeywordList with the metadata */
  void ToKeywordlist(Keywordlist&) const;

  /** Format the metadata to JSON */
  std::string ToJSON(bool multiline=false) const;

  /** Import metadata from a string keywordlist.
   * Will skip MDGeom::SensorGeometry, MDGeom::RPC and MDGeom::GCP.
   * Returns True if all keywords were parsed correctly.
   */
  bool FromKeywordlist(const Keywordlist&);

  /** Merge with another ImageMetadataBase
   * If a key exists in both ImageMetadataBase, keeps the value of this ImageMetadataBase. */
  void Fuse(const ImageMetadataBase& );

  /** Return the band that should be displayed by default, using the information contained in the
   *  input ImageMetadata (MDNum::RedDisplayChannel, GreenDisplayChannel and BlueDisplayChannel)
   *  the first channel being indexed as 0.
   *  If no information is available the default order (0,1,2) is returned instead */
  std::vector<unsigned int> GetDefaultDisplay() const;

  /** Return the number of elements in the ImageMetadataBase */
  int GetSize() const;

};


/** \class ImageMetadata
 *
 * \brief Generic class containing image metadata used in OTB
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT ImageMetadata: public ImageMetadataBase
{
public:
  /** Metadata object as a vector of Keywordlist */
  using KeywordlistVector = std::vector<ImageMetadata::Keywordlist>;

  /** Band-specific metadatas */
  using ImageMetadataBandsType = std::vector<ImageMetadataBase>;
  ImageMetadataBandsType Bands;

  // Constructor
  ImageMetadata() = default;
  ImageMetadata(DictType<MDGeom, boost::any> geometryKeys,
                DictType<MDNum, double> numericKeys,
                DictType<MDStr, std::string> stringKeys,
                DictType<MDL1D, MetaData::LUT1D> lut1DKeys,
                DictType<MDL2D, MetaData::LUT2D> lut2DKeys,
                DictType<MDTime, MetaData::TimePoint> timeKeys,
                DictType<std::string, std::string> extraKeys,
                ImageMetadataBandsType bands);

  // utility functions
  /** Extract metadata from a subset of the bands */
  ImageMetadata slice(int start, int end) const;

  /** concatenate with an other ImageMetadata
   * If a key exists in both ImageMetadata, keeps the value of this ImageMetadata.*/
  void append(const ImageMetadata& );

  /** if all bands share the same value of a key, put it at top level */
  void compact();

  /** merge with another ImageMetadata
   * If a key exists in both ImageMetadata, keeps the value of this ImageMetadata.
   * */
  void Merge(const ImageMetadata& );

  /** Append the Metadata to a vector of KeywordList.
   *  The first KeywordList contains the metadata common to all the bands.
   *  The following KeywordList contains the metadata of the bands.
   */
  void AppendToKeywordlists(KeywordlistVector&) const;

  /** Append the bands of the Metadata to a vector of KeywordList.
   *  Each KeywordList contain the metadata of a band.
   */
  void AppendToBandKeywordlists(KeywordlistVector&) const;

  /** Import metadata from a vector of keywordlist.
   *  The first KeywordList contains the metadata common to all the bands.
   *  The following KeywordList contains the metadata of the bands.
   *  Will skip MDGeom::SensorGeometry.
   *  Returns True if all keywords were parsed correctly.
   */
  bool FromKeywordlists(const KeywordlistVector&);

  /** Setter for numeric keys on each band*/
  using ImageMetadataBase::Add;
  void Add(MDNum , const MetaDataKey::VariableLengthVectorType);

  /** Getter for numeric keys on each band*/
  itk::VariableLengthVector<double> GetAsVector(MDNum key) const;

  /** test whether the metadata corresponding to key is present on each band */
  bool HasBandMetadata(MDNum key) const;

  /** test whether the metadata corresponding to key is present on each band */
  bool HasBandMetadata(MDL1D key) const;

  /** Return a vector containing the name of each band of the ImageMetadata or an empty
   * vector if at least one band name is missing */
  std::vector<std::string> GetBandNames() const;

  /** Return a vector containing the name of each band of the ImageMetadata or an empty
   * vector if at least one band name is missing */
  std::vector<std::string> GetEnhancedBandNames() const;

  /** Return the number of elements in the ImageMetadata */
  int GetSize() const;
};

extern OTBMetadata_EXPORT std::ostream& operator<<(std::ostream& os, const otb::ImageMetadataBase& imd);

extern OTBMetadata_EXPORT std::ostream& operator<<(std::ostream& os, const otb::ImageMetadata& imd);

OTBMetadata_EXPORT bool HasOpticalSensorMetadata(const ImageMetadata & imd);
OTBMetadata_EXPORT bool HasSARSensorMetadata(const ImageMetadata & imd);

OTBMetadata_EXPORT void WriteImageMetadataToGeomFile(const ImageMetadata & imd, const std::string & filename);

/** Comparison test */
bool HasSameRPCModel(const ImageMetadataBase& a, const ImageMetadataBase& b);
bool HasSameSARModel(const ImageMetadataBase& a, const ImageMetadataBase& b);
bool HasSameSensorModel(const ImageMetadataBase& a, const ImageMetadataBase& b);

} // end namespace otb

#endif
