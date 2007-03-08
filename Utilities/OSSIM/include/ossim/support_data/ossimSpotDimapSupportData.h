//*******************************************************************
// Copyright (C) 2003 ImageLinks Inc.  All rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//
// Author:  Oscar Kramer (ossim port by D. Burken)
//
// Description:  
//
// Contains declaration of class ossimSpotDimapSupportData.
// 
//*****************************************************************************
// $Id: ossimSpotDimapSupportData.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimSpotDimapSupportData_HEADER
#define ossimSpotDimapSupportData_HEADER

#include <vector>
#include <iostream>

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

class ossimKeywordlist;
// class ossimRefPtr;
class ossimXmlDocument;

class ossimSpotDimapSupportData : public ossimObject,
                                  public ossimErrorStatusInterface
{
public:
   ossimSpotDimapSupportData();
   ossimSpotDimapSupportData(const ossimSpotDimapSupportData& rhs);
   ossimSpotDimapSupportData(const ossimFilename& dimapFile,
                             bool  processSwir=false);

   virtual ossimObject* dup()const;
   virtual ~ossimSpotDimapSupportData();

   void clearFields();
   bool loadXmlFile(const ossimFilename& file,
                    bool processSwir=false);

   ossimString   getAcquisitionDate()                     const;
   ossimString   getImageID()                             const;
   ossimFilename getMetadataFile()                        const;
   void          getSunAzimuth(ossim_float64& az)         const;
   void          getSunElevation(ossim_float64& el)       const;
   void          getImageSize(ossimDpt& sz)               const; 
   void          getLineSamplingPeriod(ossim_float64& pe) const;
   void          getIncidenceAngle(ossim_float64& ia)     const;
   ossim_uint32  getNumberOfBands()                       const;
   bool          isStarTrackerUsed()                      const;
   bool          isSwirDataUsed()                         const;
   
   //---
   // Image center point:
   //---
   void getRefGroundPoint(ossimGpt& gp)     const;
   void getRefImagePoint(ossimDpt& rp)      const;
   void getRefLineTime(ossim_float64& rt)   const;

   ossimDrect getImageRect()const;
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
   // Pixel Pointing/Mirror tilt  Angles in RADIANS:
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
   void  printInfo (ostream& os) const;

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0)const;
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
private:
   void getLagrangeInterpolation(const ossim_float64& t,
                                 const vector<ossimDpt3d>& V,
                                 const vector<ossim_float64>& T,
                                 ossimDpt3d& li )const;

   void getBilinearInterpolation(const ossim_float64& t,
                                 const vector<ossimDpt3d>& V,
                                 const vector<ossim_float64>& T,
                                 ossimDpt3d& li )const;

   void getInterpolatedLookAngle(const ossim_float64& p,
                                 const vector<ossim_float64>& angles,
                                 ossim_float64& la) const;
   
   ossim_float64 convertTimeStamp(const ossimString& time_stamp) const;

   void convertTimeStamp(const ossimString& time_stamp,
                         ossim_float64& ti) const;
   
   ossimString                 theImageID;
   ossimFilename               theMetadataFile;
   ossim_float64               theSunAzimuth;  
   ossim_float64               theSunElevation;
   ossimDpt                    theImageSize;
   ossimGpt                    theRefGroundPoint;
   ossimDpt                    theRefImagePoint;
   ossimDpt                    theSubImageOffset;
   ossim_float64               theRefLineTime;
   ossim_float64               theLineSamplingPeriod;
   vector<ossim_float64>       thePixelLookAngleX;
   vector<ossim_float64>       thePixelLookAngleY;
   vector<ossimDpt3d>          theAttitudeSamples; // x=pitch, y=roll, z=yaw
   vector<ossim_float64>       theAttSampTimes;
   vector<ossimDpt3d>          thePosEcfSamples;
   vector<ossimDpt3d>          theVelEcfSamples;
   vector<ossim_float64>       theEphSampTimes;
   bool                        theStarTrackerUsed;
   bool                        theSwirDataFlag;
   ossim_uint32                theNumBands;
   ossimString                 theAcquisitionDate;
   ossim_float64               theIncidenceAngle;

   //---
   // Corner points:
   //---
   ossimGpt theUlCorner;
   ossimGpt theUrCorner;
   ossimGpt theLrCorner;
   ossimGpt theLlCorner;

   //---
   // Geoposition Points:
   //---
   vector <ossimDpt> theGeoPosImagePoints;
   vector <ossimGpt> theGeoPosGroundPoints;

   ossimGpt createGround(const ossimString& s)const;
   ossimDpt createDpt(const ossimString& s)const;

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
};

#endif /* #ifndef ossimSpotDimapSupportData_HEADER */
