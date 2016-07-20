//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimTerraSarXSarSensorModel_HEADER
#define ossimTerraSarXSarSensorModel_HEADER

#include "ossimSarSensorModel.h"
#include "otb/Noise.h"
#include "otb/SceneCoord.h"
class ossimXmlDocument;

namespace ossimplugins
{

class OSSIM_PLUGINS_DLL ossimTerraSarXSarSensorModel : public ossimSarSensorModel
{
public:

  /** Constructor */
  ossimTerraSarXSarSensorModel();

#if ! (defined(BOOST_NO_DEFAULTED_FUNCTIONS) || defined(BOOST_NO_CXX1_DEFAULTED_FUNCTIONS))

  /** Copy constructor */
  ossimTerraSarXSarSensorModel(ossimTerraSarXSarSensorModel const& m) = default;
  /** Move constructor */
  ossimTerraSarXSarSensorModel(ossimTerraSarXSarSensorModel && m) = default;

  /** Destructor */
  virtual ~ossimTerraSarXSarSensorModel() = default;
#endif

  /**Reads an annotation file associated to an image file.
   * To be used on an new instance just constructed.
   */
  virtual bool open(const ossimFilename& file) /*override*/;

  /*!
   * Returns pointer to a new instance, copy of this.
   */
  virtual ossimObject* dup() const /*override*/;

  /*!
   * Extends base-class implementation. Dumps contents of object to ostream.
   */
  virtual std::ostream& print(std::ostream& out) const /*override*/;

  /*!
   * Saves geometry KWL files.
   * \return if successful.
   */
  virtual bool saveState(ossimKeywordlist& kwl,
        const char* prefix=NULL) const /*override*/;

  /*!
   * Loads geometry KWL files.
   * \return if successful.
   */
  virtual bool loadState(ossimKeywordlist const& kwl,
        const char* prefix=NULL) /*override*/;

  /** Read product function: flavour that directly loads into C++ data.
   * References:
   * TerraSAR-X Image Product Guide
   * SNAP source code (s1tbx-io/src/main/java/org/esa/s1tbx/io/terrasarx/TerraSarXProductDirectory.java)
  */
  void readAnnotationFile(const std::string & annotationXml, const std::string & geoXml);

protected:

  TYPE_DATA;

  std::string theAzimuthTimeStart;
  std::string theAzimuthTimeStop;

  /// CalFactor (Calibration node).
  std::vector<double>      m_calFactor;
  /// PolLayer (AcquisitionInfo node).
  // TODO: add getPolLayerFromImageFile
  std::string              m_polLayerName;
  std::vector<ossimString> m_polLayerList;
  /// Noise (Noise node).
  std::vector<Noise>       m_noise;
  /// SceneCoord (SceneInfo node).
  SceneCoord               m_sceneCoord;

private:
  /** Internal function that reads annotation file.
   * \throw std::exception possibly
   */
  bool read(ossimFilename const& annotationXml);

  /** Internal function dedicated to read `OrbitStateRecords`.
   */
  std::size_t addOrbitStateVectors(ossimXmlNode const& orbitList);

  /** Internal function dedicated to read Geo Location Grid.
   */
  void readGeoLocationGrid(
        ossimXmlNode const& productRoot,
        TimeType     const& azimuthTimeStart,
        double       const  nearRangeTime);

  /** Internal function dedicated to read Azimuth Time Start and Stop
   */
  void readAzimuthTimes(
        ossimXmlNode const& sceneInfo,
        ossimXmlNode const& settings);

  ossimFilename searchGeoRefFile(ossimFilename const& file) const;

  /**
   * Searches the metadata TerraSAR file (image or xml).
   * @param[in] file image or metadata path.
   * @return metadata path.
   * @throw std::runtime_error if no metadataFile can be found.
   */
  ossimFilename findTSXLeader(ossimFilename const& file);

  ossimString const& findPolLayerName(ossimXmlNode const& node, ossimString const& xpath) const;
  std::size_t findPolLayerIdx(std::string const& polLayerName) const;
  void getNoiseAtGivenNode(ossimXmlNode const& noiseNode, ossimplugins::Noise& noise);

  /**
   * Initialize ImageNoise parameters from TerraSAR product xml file.
   * @param[in] xdoc Opened product xml file.
   * @throw std::runtime_error if data cannot be found in XML file.
   */
  void initNoise(ossimXmlDocument const& xmlDocument);
  /**
   * Initialize Calibration parameters from TerraSAR product xml file.
   * @param[in] xdoc Opened product xml file.
   * @throw std::runtime_error if data cannot be found in XML file.
   */
  void initCalibration(ossimXmlDocument const& xmlDocument);

  /**
   * Initialize InfoIncidenceAngle parameters from TerraSAR product xml file.
   * @param[in] xdoc Opened product xml file.
   * @throw std::runtime_error if data cannot be found in XML file.
   */
  void initSceneCoord(
        ossimXmlNode const& sceneCenterCoord, ossimXmlNode const& sceneInfo);
  // TODO: use a parameter
  ossimKeywordlist   theProductKwl;
};


} // end ossimplugins namespace

#endif
