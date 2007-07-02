#ifndef ossimSrtmFilename_HEADER
#define ossimSrtmFilename_HEADER
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>

class ossimSrtmFilename
{
public:
   ossimSrtmFilename(const ossimFilename& srtmFilename="");

   bool setFilename(const ossimFilename& srtmFilename);
   ossimGpt ul()const;
   ossimGpt ur()const;
   ossimGpt lr()const;
   ossimGpt ll()const;
   
protected:
   ossimFilename theFilename;

   ossim_float64 theSouthwestLongitude;
   ossim_float64 theSouthwestLatitude;
};

#endif
