//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Utility class to encapsulate parsing TerraSAR-X product.xml
// file.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimTerraSarProductDoc_HEADER
#define ossimTerraSarProductDoc_HEADER 1

#include <list>
#include <string>
#include <vector>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimRefPtr.h>

// Forward class declarations outside of namespace ossimplugins.
class ossimDpt;
class ossimGpt;
class ossimIpt;
class ossimXmlDocument;
class ossimXmlNode;
class ossimString;

namespace ossimplugins
{
   // Forward class declarations inside of namespace ossimplugins.
   class CivilDateTime;
   class PlatformPosition;
   class RefPoint;
   class SensorParams;
   class Noise;
   class SceneCoord;

   /** @brief Class to encapsulate parsing TerraSAR product xml file. */
   class ossimTerraSarProductDoc
   {
   public:
      /** @brief default constructor */
      ossimTerraSarProductDoc();
      
      /** @brief destructor */
      ~ossimTerraSarProductDoc();
      
      /**
       * @brief Checks for node /level1Product/generalHeader/mission containing
       * "TSX-1".
       * 
       * @return true if present, false if not.
       */
      bool isTerraSarX(const ossimXmlDocument* xdoc) const;
      
      /**
       * @brief Method to initialize PlatformPosition object from
       * TerraSAR  product xml file.
       * @param xdoc Opened product xml file.
       * @param pos Pointer to PlatformPosition object.
       * @return true on success, false on error.
       */
      bool initPlatformPosition(const ossimXmlDocument* xdoc,
                                PlatformPosition* pos) const;
      
      /**
       * @brief Method to initialize SensorParams object from
       * TerraSAR product xml file.
       * @param xdoc Opened product.xml file.
       * @param sp Pointer to SensorParams object.
       * @return true on success, false on error.
       */
      bool initSensorParams(const ossimXmlDocument* xdoc,
                            SensorParams* sp) const;
      
      /**
       * @brief Method to initialize image size from
       * TerraSAR product xml file.
       * @param xdoc Opened product.xml file.
       * @param imageSize The point to initialize.
       * @return true on success, false on error.
       */
      bool initImageSize(const ossimXmlDocument* xdoc,
                         ossimIpt& imageSize) const;
      
      /**
       * @brief Method to initialize gsd from
       * TerraSAR product xml file.
       * @param xdoc Opened product.xml file.
       * @param gsd The point to initialize.
       * @return true on success, false on error.
       */
      bool initGsd(const ossimXmlDocument* xdoc,
                   ossimDpt& gsd) const;
      
      /**
       * @brief Method to initialize image tie points from
       * TerraSAR product xml file.
       * @param xdoc Opened product.xml file.
       * @param imageSize The point to initialize.
       * @return true on success, false on error.
       */
      bool initTiePoints(const ossimXmlDocument* xdoc,
                         std::list<ossimGpt>& gcp,
                         std::list<ossimDpt>& icp) const;
      
      bool isProductGeoreferenced(const ossimXmlDocument* xdoc) const;
      
      bool getMission(const ossimXmlDocument* xdoc,
                      ossimString& s) const;
      
      bool geNumberOfLayers(const ossimXmlDocument* xdoc,
                      ossimString& s) const;

      bool getImageFile(const ossimXmlDocument* xdoc,
                        ossimString& s) const;
      
      bool getSceneId(const ossimXmlDocument* xdoc,
                      ossimString& s) const;
      
      bool getProjection(const ossimXmlDocument* xdoc,
                         ossimString& s) const;
      
      bool getReferencePoint(const ossimXmlDocument* xdoc,
                             ossimString& s) const;
      
      bool getImageDataStrartWith(const ossimXmlDocument* xdoc,
                                  ossimString& s) const;
      
      bool getOrbitDirection(const ossimXmlDocument* xdoc,
                             ossimString& s) const;
      
      bool getLookDirection(const ossimXmlDocument* xdoc,
                            ossimString& s) const;

      bool getGenerationTime(const ossimXmlDocument* xdoc,
                            ossimString& s) const;

      /** one based??? */
      bool getRangeGateFirstPixel(const ossimXmlDocument* xdoc,
                                  ossimString& s) const;
      
      /** one based??? */
      bool getRangeGateLastPixel(const ossimXmlDocument* xdoc,
                                 ossimString& s) const;
      
      bool getSceneCenterAzimuthTime(const ossimXmlDocument* xdoc,
                                     ossimString& s) const;
      
      bool getSceneCenterRangeTime(const ossimXmlDocument* xdoc,
                                   ossimString& s) const;
      
      /** one based */
      bool getSceneCenterRefColumn(const ossimXmlDocument* xdoc,
                                   ossimString& s) const;
      
      /** one based */
      bool getSceneCenterRefRow(const ossimXmlDocument* xdoc,
                                ossimString& s) const;
      
      /** latitude */
      bool getSceneCenterLat(const ossimXmlDocument* xdoc,
                             ossimString& s) const;
      
      /** longitude */
      bool getSceneCenterLon(const ossimXmlDocument* xdoc,
                             ossimString& s) const;
      
      bool getSceneAverageHeight(const ossimXmlDocument* xdoc,
                                 ossimString& s) const;
      
      bool getRadarCenterFrequency(const ossimXmlDocument* xdoc,
                                   ossimString& s) const;
      
      bool getAzimuthStartTime(const ossimXmlDocument* xdoc,
                               ossimString& s) const;
      
      bool getAzimuthStopTime(const ossimXmlDocument* xdoc,
                              ossimString& s) const;
      
      bool getRangeFirstPixelTime(const ossimXmlDocument* xdoc,
                              ossimString& s) const;

      bool getRangeLastPixelTime(const ossimXmlDocument* xdoc,
                              ossimString& s) const;

      bool getCommonPrf(const ossimXmlDocument* xdoc, ossimString& s) const;
      
      /** range sampling rate */
      bool getCommonRsf(const ossimXmlDocument* xdoc, ossimString& s) const;
      
      bool getNumberOfRangeLooks(const ossimXmlDocument* xdoc,
                                 ossimString& s) const;
      
      bool getNumberOfAzimuthLooks(const ossimXmlDocument* xdoc,
                                   ossimString& s) const;
      
      bool getNumberOfColumns(const ossimXmlDocument* xdoc,
                              ossimString& s) const;
      
      bool getNumberOfRows(const ossimXmlDocument* xdoc,
                           ossimString& s) const;
      
      bool getColumnSpacing(const ossimXmlDocument* xdoc,
                            ossimString& s) const;
      
      bool getRowSpacing(const ossimXmlDocument* xdoc,
                         ossimString& s) const;
      
      bool getProjectedSpacingSlantRange(const ossimXmlDocument* xdoc,
                                         ossimString& s) const;

      bool getProjectedSpacingAzimuth(const ossimXmlDocument* xdoc,
                                      ossimString& s) const;

      bool getGeodeticTerrainHeight(const ossimXmlDocument* xdoc,
                                    ossimString& s) const;

      bool getProductType(const ossimXmlDocument* xdoc,
                                    ossimString& s) const;

      bool getProductVariant(const ossimXmlDocument* xdoc,
                             ossimString& s) const;

      bool getRadiometricCorrection(const ossimXmlDocument* xdoc,
                                    ossimString& s) const;
      bool getImagingMode(const ossimXmlDocument* xdoc,
                                    ossimString& s) const;
      bool getAcquisitionSensor(const ossimXmlDocument* xdoc,
                                    ossimString& s) const;
      bool getPolarisationMode(const ossimXmlDocument* xdoc,
                                     ossimString& s) const;
      bool getPolLayerList(const ossimXmlDocument* xdoc,
                                    std::vector<ossimString>& s) const;
      bool getCalFactor(const ossimXmlDocument* xdoc,
                                    std::vector<ossimString>& s) const;
      bool getRadarFrequency(const ossimXmlDocument* xdoc,
                                    ossimString& s) const;
/*      bool getCenterIncidenceAngle(const ossimXmlDocument* xdoc,
                                    ossimString& s) const;
      bool getCornerIncidenceAngles(const ossimXmlDocument* xdoc,
                                    std::vector<ossimString>& s) const;
*/
      /**
       * @brief Method to initialize IncidenceAngles object from
       * TerraSAR  product xml file.
       * @param xdoc Opened product xml file.
       * @param pos Pointer to PlatformPosition object.
       * @return true on success, false on error.
             */
      bool initSceneCoord(const ossimXmlDocument* xdoc,
                              SceneCoord* pos) const;
      
      /**
       * @brief Method to initialize Noise object from
       * TerraSAR  product xml file.
       * @param xdoc Opened product xml file.
       * @param pos Pointer to PlatformPosition object.
       * @return true on success, false on error.
       */
      bool initNoise(const ossimXmlDocument* xdoc,
                                Noise* pos) const;

/*
      bool getImageNoiseTimeUTC(const ossimXmlDocument* xdoc, ossimString& s) const;
      bool getImageNoiseValidityRangeMin(const ossimXmlDocument* xdoc, ossimString& s) const;
      bool getImageNoiseValidityRangeMax(const ossimXmlDocument* xdoc, ossimString& s) const;
      bool getImageNoiseReferencePoint(const ossimXmlDocument* xdoc, ossimString& s) const;
      bool getImageNoisePolynomialCoefficient( const ossimXmlDocument* xdoc, std::vector<double>& polynomialCoefficient) const;

*/

   };

} // End: namespace ossimplugins

#endif /* matches: #ifndef ossimTerraSarProductDoc_HEADER */
