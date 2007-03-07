//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
//
// Description: This class parses a Digital Ortho Quarter Quad (DOQQ)
//              header.
//
//********************************************************************
// $Id: ossimDoqq.h,v 1.3 2005/11/08 18:26:57 gpotts Exp $

#ifndef ossimDoqq_HEADER
#define ossimDoqq_HEADER

#include <iostream>

#include <base/common/ossimConstants.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimFilename.h>
#include <base/common/ossimReferenced.h>

class OSSIM_DLL ossimDoqq : public ossimReferenced
{
public:
   ossimDoqq(ossimFilename file);
   ~ossimDoqq();
   
   enum ossimErrorStatus
   {
      OSSIM_OK    = 0,
      OSSIM_ERROR = 1
   };

   enum
   {
      NUMBER_OF_RECORDS       = 4,
      QUADRANGLE_NAME_OFFSET  = 0,
      QUADRANT_OFFSET         = 38,
      NATION_OFFSET           = 40,
      STATE_OFFSET            = 44,
      GSD_X_OFFSET            = 59,
      GSD_Y_OFFSET            = 59,
      DATA_ORDER_OFFSET       = 142,      
      LINE_OFFSET             = 145,
      SAMPLE_OFFSET           = 151,
      PROJECTION_OFFSET       = 196,
      RGB_OFFSET              = 157,
      UL_EASTING_OFFSET       = 288,
      UL_NORTHING_OFFSET      = 312,
      UTM_ZONE_OFFSET         = 199,
      DATUM_OFFSET            = 168,

      QUADRANGLE_NAME_SIZE    = 37,
      QUADRANT_SIZE           = 2,
      NATION_SIZE             = 4,
      STATE_SIZE              = 2,
      DATA_ORDER_SIZE         = 3,
      LINE_SIZE               = 8,
      SAMPLE_SIZE             = 8,  
      PROJECTION_SIZE         = 3,
      RGB_SIZE                = 3,
      MARKING_U_SIZE          = 5,
      MARKING_V_SIZE          = 5,
      UL_EASTING_SIZE         = 24,
      UL_NORTHING_SIZE        = 24,
      UTM_ZONE_SIZE           = 6,
      DATUM_SIZE              = 2,
      RADIOMETRY_SIZE         = 3,
      GSD_SIZE              = 12
      
   };

   ossimString theProjection;
   ossimString theDatum;
   ossimString theQuadName;
   ossimString theQuad;
   ossimString theNation;
   ossimString theState;
   ossimString theDataOrder;
   ossimString theMarkU;
   ossimString theMarkV;
   
   ossim_int32 theUtmZone;
   ossim_int32 theLine;
   ossim_int32 theSample;
   ossim_int32 theRgb;

   ossimString theAcqYear;
   ossimString theAcqMonth;
   ossimString theAcqDay;
   
   ossim_float64   theEasting;
   ossim_float64   theNorthing;
   ossim_float64   theUN;
   ossim_float64   theUE;
   ossim_float64   theLN;
   ossim_float64   theLE;
   ossimDpt        theGsd;
   ossim_int32     theHeaderSize;
   ossim_int32     theRecordSize;

   friend std::ostream& operator<<(std::ostream& os, const ossimDoqq& doq);
   ossim_int32 errorStatus() const { return theErrorStatus; }
   
private:
   // prevent use
   ossimDoqq();

   ossimErrorStatus        theErrorStatus;
   
   ossim_float64 convertStr(const char str[]) const;

   void ldstr_v1(std::istream& in);
   void ldstr_v2(std::istream& in);
};

#endif
