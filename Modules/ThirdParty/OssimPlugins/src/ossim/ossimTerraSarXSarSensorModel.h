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

  //Not implemented yet
  /** References
   * TerraSAR-X Image Product Guide
   * SNAP source code (s1tbx-io/src/main/java/org/esa/s1tbx/io/terrasarx/TerraSarXProductDirectory.java)
  */
  void readAnnotationFile(const std::string & annotationXml, const std::string & geoXml);

protected:
  /*
  std::string theProductType;
  std::string theMode;
  std::string theSwath;
  std::string thePolarisation;
  */
};

} // end namespace

#endif
