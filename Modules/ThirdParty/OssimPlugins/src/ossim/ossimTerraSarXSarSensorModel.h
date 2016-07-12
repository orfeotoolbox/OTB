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

namespace ossimplugins
{

class OSSIM_PLUGINS_DLL ossimTerraSarXSarSensorModel : public ossimSarSensorModel
{
public:

#if ! (defined(BOOST_NO_DEFAULTED_FUNCTIONS) || defined(BOOST_NO_CXX1_DEFAULTED_FUNCTIONS))
  /** Constructor */
  ossimTerraSarXSarSensorModel() = default;

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

  /** Read product function: flavour that fills KWL.
   * References:
   * TerraSAR-X Image Product Guide
   * SNAP source code (s1tbx-io/src/main/java/org/esa/s1tbx/io/terrasarx/TerraSarXProductDirectory.java)
   */
  bool readProduct(ossimFilename const &productXmlFile);

protected:

  TYPE_DATA;

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

  ossimFilename searchGeoRefFile(ossimFilename const& file) const;

  // TODO: use a parameter
  ossimKeywordlist   theProductKwl;
};

} // end ossimplugins namespace

#endif
