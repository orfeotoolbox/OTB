//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
//*******************************************************************
//  $Id: ossimGeoidNgs.h 11496 2007-08-06 09:18:28Z dburken $
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
   /**
    *  @return The offset from the ellipsoid to the geoid or ossim::nan()
    *  if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt&) const;

   /**
    *  @return Geoid to ellipsoid height or ossim::nan()
    *  if grid does not contain the point.
    */
   double geoidToEllipsoidHeight(double lat,
                                 double lon,
                                 double geoidHeight) const;

   /**
    *  @return Ellipsoid to geoid height or ossim::nan()
    *  if grid does not contain the point.
    */
   double ellipsoidToGeoidHeight(double lat,
                                 double lon,
                                 double ellipsoidHeight) const;
protected:
   void fixLatLon(double &lat, double &lon) const;
   double deltaHeight(double lat, double lon)const;
   
   std::vector<ossimGeoidNgsHeader> theGridVector;
   ossimString theShortName;

   TYPE_DATA
};

#endif
