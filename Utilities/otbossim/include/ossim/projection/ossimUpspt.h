//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Garrett Potts(gpotts@imagelinks.com)
//
// Description:
//
// This is the Universal Polar Stereographic (UPS) point.  Allows
// easy onversion between different coordinates.
//*******************************************************************
//  $Id: ossimUpspt.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimUpspt_HEADER
#define ossimUpspt_HEADER

class ossimUtmpt;
class ossimGpt;
class ossimEcefPoint;
class ossimDatum;

class ossimUpspt
{
public:
   ossimUpspt(const ossimGpt &aPt);
   ossimUpspt(const ossimEcefPoint &aPt);

   char hemisphere()const{return theHemisphere;}
   double easting()const {return theEasting;}
   double northing()const{return theNorthing;}
   const ossimDatum* datum()const{return theDatum;}
   
private:
   char    theHemisphere;
   double  theEasting;
   double  theNorthing;
   ossimDatum  *theDatum;

   void convertFromGeodetic(const ossimGpt &aPt);
};

#endif
