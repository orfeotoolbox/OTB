//*******************************************************************
// Copyright (C) 2012  Centre National Etudes Spatiales
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// Author : Mickael Savinaud (mickael.savinaud@c-s.fr)
//
// Description:
//
// Contains declaration of class ossimPleiadesDimapSupportData
//
//*****************************************************************************
#ifndef ossimPleiadesDimapSupportData_HEADER
#define ossimPleiadesDimapSupportData_HEADER

#include <ossimPluginConstants.h>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimString.h>

#include <ossim/base/ossimErrorStatusInterface.h>

#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimFilename.h>

#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimGpt.h>


#include <vector>
#include <iostream>


class ossimKeywordlist;
class ossimXmlDocument;

namespace ossimplugins
{
   class OSSIM_PLUGINS_DLL ossimPleiadesDimapSupportData : public ossimObject,
      public ossimErrorStatusInterface
      {
      public:
         enum ossimPleiadesDIMAPVersion
         {
            OSSIM_PLEIADES_UNKNOWN,
            OSSIM_PLEIADES_DIMAPv1,
            OSSIM_PLEIADES_DIMAPv2
         };

         /** metadata subprofile type */
         enum ossimPleiadesMetadataSubProfile
         {
            OSSIM_PLEIADES_METADATA_SUBPROFILE_UNKNOWN = 0,
            OSSIM_PLEIADES_METADATA_SUBPROFILE_PRODUCT = 1,
            OSSIM_PLEIADES_METADATA_SUBPROFILE_RPC = 2
         };

         ossimPleiadesDimapSupportData();

         virtual ~ossimPleiadesDimapSupportData();

         void clearFields();

         bool parseXmlFile(const ossimFilename& file);

         //---
         // Convenient method to print important image info:
         //---
         void  printInfo (ostream& os) const;

         /**
          * Method to save the state of the object to a keyword list.
          * Return true if ok or false on error.
          */
         virtual bool saveState(ossimKeywordlist& kwl,
                                const char* prefix = 0)const;
         /**
          * Method to the load (recreate) the state of the object from a keyword
          * list.  Return true if ok or false on error.
          */
         virtual bool loadState(const ossimKeywordlist& kwl,
                                const char* prefix = 0);

         ossimString   getSensorID()                            const;
         ossimString   getProcessingLevel()                     const;
         ossimString   getAcquisitionDate()                     const;
         ossimString   getProductionDate()                      const;
         ossimString   getImageID()                             const;
         ossimString   getInstrument()                          const;
         ossimString   getInstrumentIndex()                     const;

         ossim_uint32  getNumberOfBands()                       const;
         void          getSunAzimuth(std::vector<ossim_float64>& az)         const;
         void          getSunElevation(std::vector<ossim_float64>& el)       const;
         void          getImageSize(ossimIpt& sz)               const;

         void          getIncidenceAngle(std::vector<ossim_float64>& ia)     const;
         void          getViewingAngle(std::vector<ossim_float64>& va)       const;

         //Along and across track incidence angle
         void getAcrossTrackIncidenceAngle(std::vector<ossim_float64>& act) const;
         void getAlongTrackIncidenceAngle(std::vector<ossim_float64>& alt) const;

         //---
         // Corner points:
         //---
         void getUlCorner(ossimGpt& pt) const;
         void getUrCorner(ossimGpt& pt) const;
         void getLrCorner(ossimGpt& pt) const;
         void getLlCorner(ossimGpt& pt) const;

         //---
         // Image center point:
         //---

         /** Center of frame, sub image if there is one. */
         void getRefGroundPoint(ossimGpt& gp)         const;

         /** zero base center point */
         void getRefImagePoint(ossimDpt& rp)          const;

         /** Zero based image rectangle, sub image if there is one. */
         void getImageRect(ossimDrect& rect)const;

         bool allMetadataRead(){return (theProductIsOk && theRpcIsOk);};

         std::vector<double> getLineNumCoeff() const {return theLineNumCoeff;};
         std::vector<double> getLineDenCoeff() const {return theLineDenCoeff;};
         std::vector<double> getSampNumCoeff() const {return theSampNumCoeff;};
         std::vector<double> getSampDenCoeff() const {return theSampDenCoeff;};

         ossim_int32 getLineOffset() const {return theLineOffset;};
         ossim_int32 getSampOffset() const {return theSampOffset;};
         double getLatOffset()       const {return theLatOffset;};
         double getLonOffset()       const {return theLonOffset;};
         double getHeightOffset()    const {return theHeightOffset;};
         double getLineScale()       const {return theLineScale;};
         double getSampScale()       const {return theSampScale;};
         double getLatScale()        const {return theLatScale;};
         double getLonScale()        const {return theLonScale;};
         double getHeightScale()     const {return theHeightScale;};
         
         
         ossimString    getTimeRangeStart() const {return theTimeRangeStart;};
         ossimString    getTimeRangeEnd()   const {return theTimeRangeEnd;};
         ossim_float64  getLinePeriod()     const {return theLinePeriod;};
         ossim_int32    getSwathFirstCol()  const {return theSwathFirstCol;};
         ossim_int32    getSwathLastCol()   const {return theSwathLastCol;}; 


      private:
         ossimPleiadesDIMAPVersion theDIMAPVersion;
         ossimPleiadesMetadataSubProfile theMetadataSubProfile; // only for DIMAPv2
         bool theProductIsOk;
         bool theRpcIsOk;
         ossimString theXmlDocumentRoot;

         ossimString                 theSensorID;
         ossimString                 theImageID;
         ossimString                 theProductionDate;
         ossimString                 theAcquisitionDate;
         ossimString                 theInstrument;
         ossimString                 theInstrumentIndex;
         ossimString                 theProcessingLevelString;
         ossimString                 theSpectralProcessingString;

         std::vector<ossim_float64>  theSunAzimuth;
         std::vector<ossim_float64>  theSunElevation;
         std::vector<ossim_float64>  theIncidenceAngle;
         std::vector<ossim_float64>  theViewingAngle;
         std::vector<ossim_float64>  theAzimuthAngle;

         std::vector<ossim_float64> theAlongTrackIncidenceAngle;
         std::vector<ossim_float64> theAcrossTrackIncidenceAngle;
   
         ossimIpt                    theImageSize;
         ossimIpt                    theTileSize;
         ossim_uint32                theNumberOfMegaTilesInRow;
         ossim_uint32                theNumberOfMegaTilesInCol;
         ossim_uint32                theNumberOfMegaTiles;
         ossimBooleanProperty        theMultiDataFile;

         ossim_uint32                theNumBands;
         std::vector<ossimString>    theBandOrder;

         /** Calibration information for radiometric corrections*/
         std::vector<ossim_float64> thePhysicalBias;
         std::vector<ossim_float64> thePhysicalGain;
         std::vector<ossim_float64> theSolarIrradiance;

         //---
         // Corner points:
         //---
         ossimGpt theUlCorner;
         ossimGpt theUrCorner;
         ossimGpt theLrCorner;
         ossimGpt theLlCorner;

         // RPC parameters
         ossimString theSpecId;
         double    theErrBias;
         double    theErrBiasX;
         double    theErrBiasY;
         double    theErrRand;
         double    theLineOffset;
         double    theSampOffset;
         double    theLatOffset;
         double    theLonOffset;
         double    theHeightOffset;
         double    theLineScale;
         double    theSampScale;
         double    theLatScale;
         double    theLonScale;
         double    theHeightScale;

         std::vector<double> theLineNumCoeff;
         std::vector<double> theLineDenCoeff;
         std::vector<double> theSampNumCoeff;
         std::vector<double> theSampDenCoeff;
         
         // Additional metadata used to enhance pansharpening
         ossimString    theTimeRangeStart;
         ossimString    theTimeRangeEnd;
         ossim_float64  theLinePeriod;
         ossim_int32    theSwathFirstCol;
         ossim_int32    theSwathLastCol;
         


         //--- TODO MSD Check if it is necessary to keep that
         /** Center of frame on ground, if sub image it's the center of that. */
         ossimGpt                    theRefGroundPoint;

         /** Zero based center of frame. */
         ossimDpt                    theRefImagePoint;


         ossimGpt createGround(const ossimString& s)const;
         ossimDpt createDpt(const ossimString& s)const;
         ossimIpt createIpt(const ossimString& s)const;
         //---

         bool parseRPCMetadata(ossimRefPtr<ossimXmlDocument> xmlDocument);

         bool parseMetadataIdentificationDIMAPv1(ossimRefPtr<ossimXmlDocument> xmlDocument);
         bool parseMetadataIdentificationDIMAPv2(ossimRefPtr<ossimXmlDocument> xmlDocument);

         /**
          * Dataset Identification:
          *
          * From xml section:
          * /Dimap_Document/Dataset_Identification/
          *
          * NOT YET USED
          *
          * @return true.
          */
         bool parseDatasetIdentification(ossimRefPtr<ossimXmlDocument> xmlDocument);

         /**
          * Dataset Content:
          *
          * From xml section:
          * /Dimap_Document/Dataset_Content/
          *
          * Initializes:
          * theRefGroundPoint
          * theUlCorner
          * theUrCorner
          * theLrCorner
          * theLlCorner
          *
          * Note that the theRefGroundPoint will be the zero based center of
          * the whole mega image.
          * Note that the corners will be the corners of the whole mega image.
          * @return true on success, false if not found.
          */
         bool parseDatasetContent(ossimRefPtr<ossimXmlDocument> xmlDocument);

         /**
          * Product Information:
          *
          * From xml section:
          * /Dimap_Document/Product_Information/
          *
          * Initializes:
          * theImageID
          * theProductionDate
          *
          * @return true on success, false if not found.
          */
         bool parseProductInformation(ossimRefPtr<ossimXmlDocument> xmlDocument);

         /**
          * Coordinate Reference System:
          *
          * From xml section:
          * /Dimap_Document/Coordinate_Reference_System/
          *
          * NOT YET USED
          *
          * @return true.
          */
         bool parseCoordinateReferenceSystem(ossimRefPtr<ossimXmlDocument> xmlDocument);

         /**
          * Geoposition:
          *
          * From xml section:
          * /Dimap_Document/Geoposition/
          *
          * NOT YET USED
          *
          * @return true.
          */
         bool parseGeoposition(ossimRefPtr<ossimXmlDocument> xmlDocument);

         /**
          * Processing Information:
          *
          * From xml section:
          * /Dimap_Document/Processing_Information/
          *
          * Initializes:
          * theProcessingLevel
          * theSpectralProcessing
          *
          * @return true on success, false if not found.
          */
         bool parseProcessingInformation(ossimRefPtr<ossimXmlDocument> xmlDocument);

         bool parseRasterData(ossimRefPtr<ossimXmlDocument> xmlDocument);

         bool parseRadiometricData(ossimRefPtr<ossimXmlDocument> xmlDocument);

         bool parseGeometricData(ossimRefPtr<ossimXmlDocument> xmlDocument);

         /**
          * QualityAssessment:
          *
          * From xml section:
          * /Dimap_Document/Quality_Assessment/
          *
          * NOT YET USED
          *
          * @return true.
          */
         bool parseQualityAssessment(ossimRefPtr<ossimXmlDocument> xmlDocument);

         bool parseDatasetSources(ossimRefPtr<ossimXmlDocument> xmlDocument);



      };
}
#endif /* #ifndef ossimPleiadesDimapSupportData_HEADER */

