/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef ossimFormosatDimapSupportData_HEADER
#define ossimFormosatDimapSupportData_HEADER 1

#include <ossimPluginConstants.h>

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEcefPoint.h>

#include <vector>

class ossimKeywordlist;
// class ossimRefPtr;
class ossimXmlDocument;

class OSSIM_PLUGINS_DLL ossimFormosatDimapSupportData :
   public ossimObject, public ossimErrorStatusInterface
{
public:

   /** metadata.dim format version */
   enum ossimFormosatMetadataVersion
   {
      OSSIM_FORMOSAT_METADATA_VERSION_UNKNOWN = 0,
      OSSIM_FORMOSAT_METADATA_VERSION_1_0 = 1,
      OSSIM_FORMOSAT_METADATA_VERSION_1_1 = 2
   };

   ossimFormosatDimapSupportData();


   ossimFormosatDimapSupportData(const ossimFormosatDimapSupportData& rhs);
   ossimFormosatDimapSupportData(const ossimFilename& dimapFile);

   virtual ossimObject* dup()const;

   void clearFields();
   bool loadXmlFile(const ossimFilename& file);

   ossimString   getSensorID()                                 const;
   ossimString   getMetadataVersionString()                    const;
   ossimString   getAcquisitionDate()                          const;
   ossimString   getProductionDate()                           const;
   ossimString   getSoftwareVersion()                          const;
   ossimString   getImageID()                                  const;
   ossimString   getInstrument()                               const;
   ossim_uint32  getInstrumentIndex()                          const;
   ossimFilename getMetadataFile()                             const;
   void          getSunAzimuth(ossim_float64& az)              const;
   void          getSunElevation(ossim_float64& el)            const;
   void          getSatAzimuth(ossim_float64& az)              const;
   void          getImageSize(ossimDpt& sz)                    const;
   void          getLineSamplingPeriod(ossim_float64& pe)      const;
   void          getIncidenceAngle(ossim_float64& ia)          const;
   void          getViewingAngle(ossim_float64& va)            const;
   void          getViewingAngleAlongTrack(ossim_float64& va)  const;
   void          getViewingAngleAcrossTrack(ossim_float64& va) const;
   void          getSceneOrientation(ossim_float64& so)        const;
   ossim_uint32  getNumberOfBands()                            const;
   bool          isStarTrackerUsed()                           const;
   bool          isSwirDataUsed()                              const;

   //---
   // Image center point:
   //---

   /** Center of frame, sub image if there is one. */
   void getRefGroundPoint(ossimGpt& gp)         const;

   /** zero base center point */
   void getRefImagePoint(ossimDpt& rp)          const;

   void getRefLineTime(ossim_float64& rt)       const;

   /** relative to full frame. */
   void getRefLineTimeLine(ossim_float64& rtl)  const;

   /** Zero based image rectangle, sub image if there is one. */
   void getImageRect(ossimDrect& rect)const;

   //---
   // Sub image offset:
   //---
   void getSubImageOffset(ossimDpt& offset) const;

   //---
   // Ephemeris (m & m/s):
   //---
   void getPositionEcf(ossim_uint32 sample, ossimEcefPoint& pe) const;
   void getPositionEcf(const ossim_float64& time, ossimEcefPoint& pe) const;
   void getVelocityEcf(ossim_uint32 sample, ossimEcefPoint& ve) const;
   void getVelocityEcf(const ossim_float64& time, ossimEcefPoint& ve) const;
   void getEphSampTime(ossim_uint32 sample, ossim_float64& et) const;

   ossim_uint32 getNumEphSamples() const;

   //---
   // Attitude Angles in RADIANS:
   //---
   void getAttitude(ossim_uint32 sample, ossimDpt3d& at)  const;
   void getAttitude(const ossim_float64& time, ossimDpt3d& at) const;
   void getAttSampTime(ossim_uint32 sample, ossim_float64& at)  const;
   ossim_uint32 getNumAttSamples() const;

   //---
   // Polynomial instrument look angle in RADIANS:
   //---
   void getPixelLookAngleX (ossim_uint32 sample, ossim_float64& pa) const;
   void getPixelLookAngleX (const ossim_float64& sample,
                        	    ossim_float64& pa) const;
   void getPixelLookAngleY (ossim_uint32 sample, ossim_float64& pa) const;
   void getPixelLookAngleY (const ossim_float64& sample,
                     	       ossim_float64& pa) const;

   //---
   // Geoposition points provided in the file (most likely just corner points):
   //---
   ossim_uint32 getNumGeoPosPoints() const;
   void getGeoPosPoint (ossim_uint32 point, ossimDpt& ip, ossimGpt& gp) const;

   //---
   // Corner points:
   //---
   void getUlCorner(ossimGpt& pt) const;
   void getUrCorner(ossimGpt& pt) const;
   void getLrCorner(ossimGpt& pt) const;
   void getLlCorner(ossimGpt& pt) const;

   //---
   // Convenient method to print important image info:
   //---
   void  printInfo (std::ostream& os) const;

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
protected:
   virtual ~ossimFormosatDimapSupportData();

private:
   void getLagrangeInterpolation(const ossim_float64& t,
                                 const std::vector<ossimDpt3d>& V,
                                 const std::vector<ossim_float64>& T,
                                 ossimDpt3d& li )const;

   void getBilinearInterpolation(const ossim_float64& t,
                                 const std::vector<ossimDpt3d>& V,
                                 const std::vector<ossim_float64>& T,
                                 ossimDpt3d& li )const;

   void getInterpolatedLookAngle(const ossim_float64& p,
                                 const std::vector<ossim_float64>& angles,
                                 ossim_float64& la) const;

   ossim_float64 convertTimeStamp(const ossimString& time_stamp) const;

   void convertTimeStamp(const ossimString& time_stamp,
                         ossim_float64& ti) const;

   /**
    * Initializes theMetadataVersion.
    * @return true on success, false if not found.
    */
   bool initMetadataVersion(ossimRefPtr<ossimXmlDocument> xmlDocument);

   /**
    * Initializes theImageId.
    * @return true on success, false if not found.
    */
   bool initImageId(ossimRefPtr<ossimXmlDocument> xmlDocument);

   /**
    * From xml section:
    * /Dimap_Document/Dataset_Sources/Source_Information/Scene_Source
    *
    * Initializes:
    * theSunAzimuth
    * theSunElevation
    * theSatAzimuth;
    * theIncidenceAngle
    * @return true on success, false if not found.
    */
   bool initSceneSource(ossimRefPtr<ossimXmlDocument> xmlDocument);

   /**
    * Frame points:
    *
    * From xml section:
    * /Dimap_Document/Dataset_Frame/
    *
    * Initializes:
    * theRefGroundPoint
    * theViewingAngle
    * theViewingAngleAlongTrack
    * theViewingAngleAcrossTrack
    *
    * Note that the theRefImagePoint will be the zero based center of the
    * frame.
    * @return true on success, false if not found.
    */
   bool initFramePoints(ossimRefPtr<ossimXmlDocument> xmlDocument);

   ossimString                 theSensorID;
   ossimFormosatMetadataVersion    theMetadataVersion;
   ossimString                 theImageID;
   ossimFilename               theMetadataFile;
   ossimString                 theProductionDate;
   ossimString                 theSoftwareVersion;
   ossimString                 theInstrument;
   ossim_uint32                theInstrumentIndex;

   /*
    * From xml section:
    * /Dimap_Document/Dataset_Sources/Source_Information/
    * Scene_Source
    */
   ossim_float64               theSunAzimuth;
   ossim_float64               theSunElevation;
   ossim_float64               theSatAzimuth;
   ossim_float64               theIncidenceAngle;
   ossim_float64               theViewingAngle;
   ossim_float64               theViewingAngleAlongTrack;
   ossim_float64               theViewingAngleAcrossTrack;
   ossim_float64               theSceneOrientation;   
   
   ossimDpt                    theImageSize;

   /** Center of frame on ground, if sub image it's the center of that. */
   ossimGpt                    theRefGroundPoint;

   /** Zero based center of frame. */
   ossimDpt                    theRefImagePoint;

   ossimDpt                    theSubImageOffset;

   /** relative to full image */
   ossim_float64               theRefLineTime;

   /** relative to full image */
   ossim_float64               theRefLineTimeLine;

   ossim_float64               theLineSamplingPeriod;

   /** holds the size of thePixelLookAngleX/Y */
   std::vector<ossim_float64>  thePolynomialLookAngleX;
   std::vector<ossim_float64>  thePolynomialLookAngleY;
   std::vector<ossimDpt3d>     theAttitudeSamples; // x=pitch, y=roll, z=yaw
   std::vector<ossim_float64>  theAttSampTimes;
   std::vector<ossimDpt3d>     thePosEcfSamples;
   std::vector<ossimDpt3d>     theVelEcfSamples;
   std::vector<ossim_float64>  theEphSampTimes;
   bool                        theStarTrackerUsed;
   ossim_uint32                theNumBands;
   ossimString                 theAcquisitionDate;
   

   //---
   // Corner points:
   //---
   //ossimGpt theUlCorner;
   //ossimGpt theUrCorner;
   //ossimGpt theLrCorner;
   //ossimGpt theLlCorner;
   std::vector <ossimDpt> theFrameVertexPosImagePoints;
   std::vector <ossimGpt> theFrameVertexPosGroundPoints;


   //---
   // Geoposition Points:
   //---
   std::vector <ossimDpt> theGeoPosImagePoints;
   std::vector <ossimGpt> theGeoPosGroundPoints;

   ossimGpt createGround(const ossimString& s)const;
   ossimDpt createDpt(const ossimString& s)const;

   /** callibration information for radiometric corrections*/

   std::vector<ossim_float64> thePhysicalBias;
   std::vector<ossim_float64> thePhysicalGain;

   std::vector<ossim_float64> theSolarIrradiance;

   /**
    * Private parse methods called by loadXml.
    *
    * These "parse" methods were made simply to split the loadXmlMethod
    * into pieces.  This was an attempt to troubleshoot what was thought to
    * be a compiler issue, which turned out to be changed metadata and bad
    * error handling within this code.  Since there were lots of debug
    * statements added I am committing this to cvs.
    * DRB 16 Oct. 2005
    */
   bool parsePart1(ossimRefPtr<ossimXmlDocument> xmlDocument);
   bool parsePart2(ossimRefPtr<ossimXmlDocument> xmlDocument);
   bool parsePart3(ossimRefPtr<ossimXmlDocument> xmlDocument);
   bool parsePart4(ossimRefPtr<ossimXmlDocument> xmlDocument);
};

#endif /* #ifndef ossimFormosatDimapSupportData_HEADER */
