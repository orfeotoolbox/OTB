//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
//*******************************************************************
//  $Id: ossimGeoidNgs.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimGeoidNgs_HEADER
#define ossimGeoidNgs_HEADER

#include <ossim/base/ossimGeoid.h>
#include <ossim/base/ossimGeoidNgsHeader.h>

class OSSIMDLLEXPORT ossimGeoidNgs : public ossimGeoid
{

public:
   ossimGeoidNgs();
   ossimGeoidNgs(const ossimFilename& dir,
                 ossimByteOrder byteOrder = OSSIM_LITTLE_ENDIAN);
   virtual ~ossimGeoidNgs();

   virtual ossimString getShortName()const;
   
   virtual bool open(const ossimFilename& dir,
                     ossimByteOrder byteOrder = OSSIM_LITTLE_ENDIAN);

   virtual bool addFile(const ossimFilename& file,
                        ossimByteOrder byteOrder = OSSIM_LITTLE_ENDIAN);
   /*!
    *  Returns the offset from the ellipsoid to the geoid.
    *  Returns OSSIM_DBL_NAN if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt&) const;
   
   double geoidToEllipsoidHeight(double lat,
                           double lon,
                           double geoidHeight);
   
   double ellipsoidToGeoidHeight(double lat,
                                 double lon,
                                 double ellipsoidHeight);
protected:
   void fixLatLon(double &lat, double &lon) const;
   double deltaHeight(double lat, double lon)const;
   
   std::vector<ossimGeoidNgsHeader> theGridVector;
   ossimString theShortName;

   TYPE_DATA
};

#endif
