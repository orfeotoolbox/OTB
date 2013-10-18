//----------------------------------------------------------------------------
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Description: Utility class to encapsulate parsing RadarSat2 product.xml
// file.
//
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimRadarSat2ProductDoc_HEADER
#define ossimRadarSat2ProductDoc_HEADER 1

#include <ossimPluginConstants.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>

#include <list>
#include <vector>
#include <sstream>
#include <vector>

class ossimDpt;
class ossimGpt;
class ossimIpt;
class ossimXmlDocument;
class ossimXmlNode;
class ossimString;


using namespace std;

typedef struct
{
   double biasError;
   double randomError;
   double lineFitQuality;
   double pixelFitQuality;
   double lineOffset;
   double pixelOffset;
   double latitudeOffset;
   double longitudeOffset;
   double heightOffset;
   double lineScale;
   double pixelScale;
   double latitudeScale;
   double longitudeScale;
   double heightScale;
   vector<double> lineNumeratorCoefficients;
   vector<double> lineDenominatorCoefficients;
   vector<double> pixelNumeratorCoefficients;
   vector<double> pixelDenominatorCoefficients;
   
}RPCModel;

namespace ossimplugins
{
class PlatformPosition;
class RefPoint;
class SensorParams;
   
/** @brief Class to encapsulate parsing RadarSat2 product.xml file. */
class OSSIM_PLUGINS_DLL ossimRadarSat2ProductDoc
{
public:
   /** @brief default constructor */
   ossimRadarSat2ProductDoc();
      
   /** @brief destructor */
   ~ossimRadarSat2ProductDoc();
      
   /**
    * @brief Checks for node /product/sourceAttributes/satellite containing
    * RADARSAT-2.
    *
    * @return true if present, false if not.
    */
   bool isRadarSat2(const ossimXmlDocument* xdoc) const;
      
   RPCModel getRpcData(const ossimXmlDocument* xdoc) const;
      
   /**
    * @brief Method to initialize PlatformPosition object from
    * RadarSat "product.xml" file.
    * @param xdoc Opened product.xml file.
    * @param pos Pointer to PlatformPosition object.
    * @return true on success, false on error.
    */
   bool initPlatformPosition(const ossimXmlDocument* xdoc,
                             PlatformPosition* pos) const;
      
   /**
    * @brief Method to initialize SensorParams object from
    * RadarSat "product.xml" file.
    * @param xdoc Opened product.xml file.
    * @param sp Pointer to SensorParams object.
    * @return true on success, false on error.
    */
   bool initSensorParams(const ossimXmlDocument* xdoc,
                         SensorParams* sp) const;
      
   /**
    * @brief Method to initialize image size from
    * RadarSat "product.xml" file.
    * @param xdoc Opened product.xml file.
    * @param imageSize The point to initialize.
    * @return true on success, false on error.
    */
   bool initImageSize(const ossimXmlDocument* xdoc,
                      ossimIpt& imageSize) const;
      
   /**
    * @brief Method to initialize gsd from
    * RadarSat "product.xml" file.
    * @param xdoc Opened product.xml file.
    * @param gsd The point to initialize.
    * @return true on success, false on error.
    */
   bool initGsd(const ossimXmlDocument* xdoc,
                ossimDpt& gsd) const;
      
   /**
    * @brief Method to initialize image tie points from
    * RadarSat "product.xml" file.
    * @param xdoc Opened product.xml file.
    * @param imageSize The point to initialize.
    * @return true on success, false on error.
    */
   bool initTiePoints(const ossimXmlDocument* xdoc,
                      std::list<ossimGpt>& gcp,
                      std::list<ossimDpt>& icp) const;
   bool getSatellite(const ossimXmlDocument* xdoc,
                     ossimString& s) const;
      
   bool getSensor(const ossimXmlDocument* xdoc,
                  ossimString& s) const;

   /** /product/sourceAttributes/beamModeMnemonic */
   bool getBeamModeMnemonic(const ossimXmlDocument* xdoc,
                            ossimString& s) const;

   bool getImageId(const ossimXmlDocument* xdoc,
                   ossimString& s) const;

   bool getImageFile(const ossimXmlDocument* xdoc, ossimString& s) const;

   /** /product/sourceAttributes/radarParameters/acquisitionType */
   bool getAcquisitionType(const ossimXmlDocument* xdoc,
                           ossimString& s) const;

   bool getRadarCenterFrequency(const ossimXmlDocument* xdoc,
                                ossimString& s) const;

   bool getNominalPrf(const ossimXmlDocument* xdoc, double& prf) const;

   bool getPulseRepetitionFrequency(const ossimXmlDocument* xdoc,
                                    std::vector<ossimString>& v) const;

   bool getAntennaPointing(const ossimXmlDocument* xdoc,
                           ossimString& s) const;

   bool getAdcSamplingRate(const ossimXmlDocument* xdoc,
                           ossimString& s) const;

   bool getPassDirection(const ossimXmlDocument* xdoc,
                         ossimString& s) const;

   bool getProductType(const ossimXmlDocument* xdoc,
                       ossimString& s) const;

   bool getZeroDopplerTimeFirstLine(const ossimXmlDocument* xdoc,
                                    ossimString& s) const;

   bool getNumberOfRangeLooks(const ossimXmlDocument* xdoc,
                              ossimString& s) const;

   bool getNumberOfAzimuthLooks(const ossimXmlDocument* xdoc,
                                ossimString& s) const;

   bool getSlantRangeNearEdge(const ossimXmlDocument* xdoc,
                              ossimString& s) const;

   bool getZeroDopplerAzimuthTime(const ossimXmlDocument* xdoc,
                                  std::vector<ossimString>& v) const;

   bool getGroundRangeOrigin(const ossimXmlDocument* xdoc,
                             std::vector<ossimString>& v) const;

   bool getGroundToSlantRangeCoefficients(const ossimXmlDocument* xdoc,
                                          std::vector<ossimString>& v) const;

   bool getSemiMajorAxis(const ossimXmlDocument* xdoc,
                         ossimString& s) const;

   bool getSemiMinorAxis(const ossimXmlDocument* xdoc,
                         ossimString& s) const;

   bool getNumberOfSamplesPerLine(const ossimXmlDocument* xdoc,
                                  ossimString& s) const;

   bool getNumberOfLines(const ossimXmlDocument* xdoc,
                         ossimString& s) const;

   bool getSampledPixelSpacing(const ossimXmlDocument* xdoc,
                               ossimString& s) const;

   bool getSampledLineSpacing(const ossimXmlDocument* xdoc,
                              ossimString& s) const;

   bool getLineTimeOrdering(const ossimXmlDocument* xdoc,
                            ossimString& s) const;

   bool getPixelTimeOrdering(const ossimXmlDocument* xdoc,
                             ossimString& s) const;

   bool getGeodeticTerrainHeight(const ossimXmlDocument* xdoc,
                                 ossimString& s) const;

   bool getIncidenceAngleNearRange(const ossimXmlDocument* xdoc,
                                 ossimString& s) const;

   bool getIncidenceAngleFarRange(const ossimXmlDocument* xdoc,
                                  ossimString& s) const;
   
   bool getSatelliteHeight(const ossimXmlDocument* xdoc,
                           ossimString& s) const;
};
}

#endif /* matches: #ifndef ossimRadarSat2ProductDoc_HEADER */
