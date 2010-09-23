//*******************************************************************
//
// License:  See top LICENSE.txt file.
//
// Author: Garrett Potts
//*******************************************************************
//  $Id: ossimStatePlaneProjectionInfo.h 17815 2010-08-03 13:23:14Z dburken $
#ifndef ossimStatePlaneProjectionInfo_HEADER
#define ossimStatePlaneProjectionInfo_HEADER

#include <ostream>

#include <ossim/base/ossimConstants.h> /* for OSSIM_DLL macro */
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimString.h>

class ossimDatum;
class ossimMapProjection;
class ossimKeywordlist;

class OSSIM_DLL ossimStatePlaneProjectionInfo
{

   friend std::ostream& operator<<(std::ostream&,
                                   const ossimStatePlaneProjectionInfo&);

public:

   ossimStatePlaneProjectionInfo(const std::string&  name,
                                 int                 pcsCode,
                                 const std::string&  projCode,
                                 const std::string&  param1,
                                 const std::string&  param2,
                                 const std::string&  param3,
                                 const std::string&  param4,
                                 double              falseEast,
                                 double              falseNorth,
                                 const std::string&  units,
                                 const std::string&  datumName);
  
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

   /** Not stored as string; hence, returned by value. */
   ossimString        units()                 const;

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
                                   const char* prefix=0)const;
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
   
   ossimUnitType        theUnits;        // "us_survey_ft" "feet" or "meters"
};

#endif /* #ifndef ossimStatePlaneProjectionInfo_HEADER */
