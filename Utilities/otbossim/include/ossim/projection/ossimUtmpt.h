//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
// This is the Universal Transverse Mercator (UTM) point.  Allows
// easy onversion between different coordinates.
//*******************************************************************
//  $Id: ossimUtmpt.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimUtmpt_HEADER
#define ossimUtmpt_HEADER

#include <ossim/base/ossimConstants.h>

class ossimGpt;
class ossimEcefPoint;
class ossimUpspt;
class ossimDatum;

class OSSIMDLLEXPORT ossimUtmpt
{
public:
   ossimUtmpt(const ossimGpt &aPt);
   ossimUtmpt(const ossimEcefPoint &aPt);
   ossimUtmpt(long         zone,
              char         hemisphere,  // N or S
              double       easting,
              double       northing,
              const ossimDatum* datum);
   
   // ossimUtmpt(const ossimUpspt &aPt);

   /**
    * Return the zone for this utm point.
    */
   long zone() const { return theZone; }

   /**
    * Return the hemisphere ( Northern or Southern).
    * We store this as a char value N or S.  We store it
    * like this because we are interfacing to geotrans
    * and geotrans stores their values in this manner.
    */
   char hemisphere() const { return theHemisphere; }

   /**
    * return the easting value in meters.
    */
   double easting() const { return theEasting; }

   /**
    * return the northing value in meters.
    */
   double northing() const { return theNorthing; }

   /**
    * return the reference datum for this point.
    */
   const ossimDatum* datum() const { return theDatum; }

   /**
    * Initializes this point to utm coordinates for the ground point.
    *
    * @param aPt Ground point to initialize from.
    */
   void convertFromGround(const ossimGpt &aPt);

   /**
    * Initializes the ground point from the utm coordinates of this point.
    *
    * @param aPt Ground point to initialize.
    */
   void convertToGround(ossimGpt &aPt);
   
private:
   long               theZone;
   char               theHemisphere;
   double             theEasting;
   double             theNorthing;
   const ossimDatum*  theDatum;
};

#endif
