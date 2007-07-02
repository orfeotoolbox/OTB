//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top LICENSE.txt file.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimStatePlaneProjectionInfo.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimStatePlaneProjectionInfo_HEADER
#define ossimStatePlaneProjectionInfo_HEADER

#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimKeywordlist.h>

class ossimDatum;
class ossimMapProjection;

class OSSIMDLLEXPORT ossimStatePlaneProjectionInfo
{

   friend ostream& operator<<(ostream&, const ossimStatePlaneProjectionInfo&);

public:

   ossimStatePlaneProjectionInfo(char*  name,
                                 int    pcsCode,
                                 char*  projCode,
                                 char*  param1,
                                 char*  param2,
                                 char*  param3,
                                 char*  param4,
                                 double falseEast,
                                 double falseNorth,
                                 char*   units);
  
   ossimStatePlaneProjectionInfo(char*  name,
                                 int    pcsCode,
                                 char*  projCode,
                                 char*  param1,
                                 char*  param2,
                                 double param3,
                                 double param4,
                                 double falseEast,
                                 double falseNorth,
                                 char*  units);
   
   int                code()                  const;
   const ossimString& name()                  const;
   const ossimString& projName()              const;
   double             originLat()             const;
   double             originLon()             const;
   const ossimDatum*  datum()                 const;
   ossimGpt           origin()                const;
   double             parallel1()             const;
   double             parallel2()             const;
   double             falseEasting()          const;
   double             falseNorthing()         const;
   double             falseEastingInMeters()  const;
   double             falseNorthingInMeters() const;
   double             scaleFactor()           const;
   const ossimString& units()                 const;

   /**
    * 
    * @return ossimUnitType
    */
   ossimUnitType getUnitType() const;

   /**
    * Checks parameters of projection against this.
    *
    * @return true if projection parameters match this, false if not.
    */
   bool matchesProjection(const ossimMapProjection* proj) const;
   
   bool               isSameCode( int Code )  const;

   void populateProjectionKeywords(ossimKeywordlist& kwl,
                                   const char* prefix=NULL)const;
private:
   
   int                  thePcsCode;       // The Geotiff code 
   ossimString          theName;          // Descriptive PCS name
   
   /*!
    * ossimLambertConformalConicProjection or
    * ossimTransMercatorProjection
    */
   ossimString          theProjectionName;
   
   const ossimDatum*    theDatum; // Either NAD83 or NAD27  based on name
   
   ossimDms             theOriginLat;
   ossimDms             theOriginLon;
   ossimGpt             theOrigin;
   
   double               theParameter3;      // parallel 1   or   Scale factor
   double               theParameter4;      // parallel 2   or      0
   
   double               theFalseEasting;
   double               theFalseNorthing;
   
   double               theScaleFactor;
   
   ossimString          theUnits;        // "ft" or "m"
};

#endif /* #ifndef ossimStatePlaneProjectionInfo_HEADER */
