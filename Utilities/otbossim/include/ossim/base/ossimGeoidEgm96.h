//*******************************************************************
// 
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  David Burken
//
// Description:
//
// Contains class definition for ossimGeoidEgm96 which is
// "Earth Gravity Model 1996".
//
//*******************************************************************
//  $Id: ossimGeoidEgm96.h 14799 2009-06-30 08:54:44Z dburken $

#ifndef ossimGeoidEgm96_HEADER
#define ossimGeoidEgm96_HEADER

#include <ossim/base/ossimGeoid.h>
#include <vector>

#define GEOID_NO_ERROR              0x0000
#define GEOID_FILE_OPEN_ERROR       0x0001
#define GEOID_INITIALIZE_ERROR      0x0002
#define GEOID_NOT_INITIALIZED_ERROR 0x0004
#define GEOID_LAT_ERROR             0x0008
#define GEOID_LON_ERROR             0x0010

class ossimGpt;

class OSSIMDLLEXPORT ossimGeoidEgm96 : public ossimGeoid
{

public:
   ossimGeoidEgm96();
   ossimGeoidEgm96(const ossimFilename& grid_file,
                   ossimByteOrder byteOrder=OSSIM_BIG_ENDIAN);
   
   virtual ~ossimGeoidEgm96();

   virtual bool open(const ossimFilename& grid_file,
                     ossimByteOrder byteOrder=OSSIM_BIG_ENDIAN);

   virtual ossimString getShortName()const;
   
   /**
    *  @return The offset from the ellipsoid to the geoid or ossim::nan()
    *  (IEEE NAN) if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt& gpt) const;

   double geoidToEllipsoidHeight(double lat,
                                 double lon,
                                 double geoidHeight) const;
   
   double ellipsoidToGeoidHeight(double lat,
                           double lon,
                           double ellipsoidHeight) const;

protected:

   std::vector<float> theGeoidHeightBuffer;
   mutable float* theGeoidHeightBufferPtr;
   TYPE_DATA
};

#endif
