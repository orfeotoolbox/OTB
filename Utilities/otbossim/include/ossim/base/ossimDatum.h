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
//  $Id: ossimDatum.h 19793 2011-06-30 13:26:56Z gpotts $
#ifndef ossimDatum_HEADER
#define ossimDatum_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimObject.h>

class OSSIMDLLEXPORT ossimDatum : public ossimObject
{
   friend class ossimDatumFactory;

public:
   /**
    *   @param alpha_code     new OSSIM/Geotrans datum code.                (input)
    *   @param name           Name of the new datum                         (input)
    *   @param SigmaX         Standard error in X in meters                 (input)
    *   @param SigmaY         Standard error in Y in meters                 (input)
    *   @param SigmaZ         Standard error in Z in meters                 (input)
    *   @param southLatitude  Southern edge of validity rectangle in radians(input)
    *   @param northLatitude  Northern edge of validity rectangle in radians(input)
    *   @param westLongitude  Western edge of validity rectangle in radians (input)
    *   @param eastLongitude  Eastern edge of validity rectangle in radians (input)
    */
    ossimDatum(const ossimString &alpha_code, const ossimString &name,
              const ossimEllipsoid* anEllipsoid,
              ossim_float64 sigmaX, ossim_float64 sigmaY, ossim_float64 sigmaZ,
              ossim_float64 westLongitude, ossim_float64 eastLongitude,
              ossim_float64 southLatitude, ossim_float64 northLatitude);

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
   virtual ossim_uint32 epsgCode()const{return theEpsgCode;}
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

   bool operator==(const ossimDatum& rhs) const;

   bool operator!=(const ossimDatum& rhs)const
   {
      return (!(*this == rhs));
   }
   virtual bool isEqualTo(const ossimObject& obj, ossimCompareType compareType=OSSIM_COMPARE_FULL)const;

protected:
   //! Only friend ossimDatumFactory is permitted to delete
   virtual ~ossimDatum(){};

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

   
protected:
   ossimString           theCode;
   ossim_uint32          theEpsgCode;
   ossimString           theName;
   const ossimEllipsoid *theEllipsoid;
   
   ossim_float64        theSigmaX;
   ossim_float64        theSigmaY;
   ossim_float64        theSigmaZ;

   ossim_float64        theWestLongitude;
   ossim_float64        theEastLongitude;
   ossim_float64        theSouthLatitude;
   ossim_float64        theNorthLatitude; 
   
TYPE_DATA;
};

#endif
