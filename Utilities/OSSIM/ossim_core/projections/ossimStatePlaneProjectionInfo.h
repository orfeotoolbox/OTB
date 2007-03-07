//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//*******************************************************************
//  $Id: ossimStatePlaneProjectionInfo.h,v 1.5 2004/06/09 15:35:33 dburken Exp $
#ifndef ossimStatePlaneProjectionInfo_HEADER
#define ossimStatePlaneProjectionInfo_HEADER

#include "base/data_types/ossimDms.h"
#include "base/data_types/ossimGpt.h"
#include "base/data_types/ossimString.h"
#include "base/data_types/ossimKeywordlist.h"

class  ossimDatum;

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
