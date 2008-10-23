//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
//
// Description:
//
// Contains class declaration for OssimDatum.  This is a class wrapper
// for Geotrans datum.  For more thorough description of each function
// look at the datum.h file.
//*******************************************************************
//  $Id: ossimDatum.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimDatum_HEADER
#define ossimDatum_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEllipsoid.h>

class OSSIMDLLEXPORT ossimDatum
{
public:
   ossimDatum(const ossimString &code, const ossimString &name,
              const ossimEllipsoid* anEllipsoid,
              ossim_float64 sigmaX, ossim_float64 sigmaY, ossim_float64 sigmaZ,
              ossim_float64 westLongitude, ossim_float64 eastLongitude,
              ossim_float64 southLatitude, ossim_float64 northLatitude)
      :theCode(code),
       theName(name),
       theEllipsoid(anEllipsoid),
       theSigmaX(sigmaX),
       theSigmaY(sigmaY),
       theSigmaZ(sigmaZ),
       theWestLongitude(westLongitude),
       theEastLongitude(eastLongitude),
       theSouthLatitude(southLatitude),
       theNorthLatitude(northLatitude)
      {};
   virtual ~ossimDatum(){};

   // Argument holds the source point and datum.  Returns another
   // point with this datum.
   //
   virtual ossimGpt shift(const ossimGpt    &aPt)const=0;
   //utility functions to shift to and from the standard coordinates.
   //Users should use the shift instead!!!
   //
   virtual ossimGpt       shiftToWgs84(const ossimGpt &aPt)const = 0;
   virtual ossimGpt       shiftFromWgs84(const ossimGpt &aPt)const = 0;
   

   virtual bool  isTheSameAs(const ossimDatum *aDatum)const
      {return this == aDatum;}
   virtual const ossimString& code()const{return theCode;}
   virtual const ossimString& name()const{return theName;}
   virtual const ossimEllipsoid* ellipsoid()const{return theEllipsoid;}
   virtual ossim_float64 sigmaX()const{return theSigmaX;}
   virtual ossim_float64 sigmaY()const{return theSigmaY;}
   virtual ossim_float64 sigmaZ()const{return theSigmaZ;}

   virtual ossim_float64 westLongitude()const{return theWestLongitude;}
   virtual ossim_float64 eastLongitude()const{return theEastLongitude;}
   virtual ossim_float64 southLatitude()const{return theSouthLatitude;}
   virtual ossim_float64 northLatitude()const{return theNorthLatitude;}
   
   virtual ossim_float64 param1()const=0;
   virtual ossim_float64 param2()const=0;
   virtual ossim_float64 param3()const=0;
   virtual ossim_float64 param4()const=0;
   virtual ossim_float64 param5()const=0;
   virtual ossim_float64 param6()const=0;
   virtual ossim_float64 param7()const=0;

   virtual bool isInside(ossim_float64 latitude, ossim_float64 longitude)const
      {
         return ((theSouthLatitude <= latitude) &&
                 (latitude <= theNorthLatitude) &&
                 (theWestLongitude <= longitude) &&
                 (longitude <= theEastLongitude));
      }

   bool operator ==(const ossimDatum& rhs)const
      {
         return ( (theCode == rhs.theCode)&&
                  (theName == rhs.theName)&&
                  (*theEllipsoid == *rhs.theEllipsoid)&&
                  (theSigmaX == rhs.theSigmaX)&&
                  (theSigmaY == rhs.theSigmaY)&&
                  (theSigmaZ == rhs.theSigmaZ)&&
                  (theWestLongitude == rhs.theWestLongitude)&&
                  (theEastLongitude == rhs.theEastLongitude)&&
                  (theSouthLatitude == rhs.theSouthLatitude)&&
                  (theNorthLatitude == rhs.theNorthLatitude));
      }
protected:
   
   /*!
    * This is directly from Geotrans:
    * Begin Molodensky_Shift
    * This function shifts geodetic coordinates using the Molodensky method.
    *
    *    a         : Semi-major axis of source ellipsoid in meters  (input)
    *    da        : Destination a minus source a                   (input)
    *    f         : Flattening of source ellipsoid                 (input)
    *    df        : Destination f minus source f                   (input)
    *    dx        : X coordinate shift in meters                   (input)
    *    dy        : Y coordinate shift in meters                   (input)
    *    dz        : Z coordinate shift in meters                   (input)
    */
   virtual void    molodenskyShift( double a,
                                    double da,
                                    double f,
                                    double df,
                                    double dx,
                                    double dy,
                                    double dz,
                                    double Lat_in,
                                    double Lon_in,
                                    double Hgt_in,
                                   double &Lat_out,
                                   double &Lon_out,
                                   double &Hgt_out)const;
   bool withinMolodenskyRange(const ossimGpt& pt)const
      {
         double lat = pt.latd();
         
         return ((lat < 89.75) && (lat > -89.75));
      }

private:
   ossimString           theCode;
   ossimString           theName;
   const ossimEllipsoid *theEllipsoid;
   
   ossim_float64        theSigmaX;
   ossim_float64        theSigmaY;
   ossim_float64        theSigmaZ;

   ossim_float64        theWestLongitude;
   ossim_float64        theEastLongitude;
   ossim_float64        theSouthLatitude;
   ossim_float64        theNorthLatitude;   
};

#endif
