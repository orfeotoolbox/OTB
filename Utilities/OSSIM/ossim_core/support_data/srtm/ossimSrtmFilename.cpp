#include "ossimSrtmFilename.h"
#include <base/data_types/ossimRegExp.h>

ossimSrtmFilename::ossimSrtmFilename(const ossimFilename& srtmFilename)
{
   setFilename(srtmFilename);
}

ossimGpt ossimSrtmFilename::ul()const
{
   ossimGpt result;
   
   if((theSouthwestLongitude==OSSIM_DBL_NAN)||
      (theSouthwestLatitude==OSSIM_DBL_NAN))
   {
      result.makeNan();
   }
   else
   {
      result.latd(theSouthwestLatitude+1.0);
      result.lond(theSouthwestLongitude);
   }
   
   return result;
}

ossimGpt ossimSrtmFilename::ur()const
{
   ossimGpt result;
   
   if((theSouthwestLongitude==OSSIM_DBL_NAN)||
      (theSouthwestLatitude==OSSIM_DBL_NAN))
   {
      result.makeNan();
   }
   else
   {
      result.latd(theSouthwestLatitude+1.0);
      result.lond(theSouthwestLongitude+1.0);
   }
   
   return result;
}

ossimGpt ossimSrtmFilename::lr()const
{
   ossimGpt result;
   
   if((theSouthwestLongitude==OSSIM_DBL_NAN)||
      (theSouthwestLatitude==OSSIM_DBL_NAN))
   {
      result.makeNan();
   }
   else
   {
      result.latd(theSouthwestLatitude);
      result.lond(theSouthwestLongitude+1.0);
   }
   
   return result;
}

ossimGpt ossimSrtmFilename::ll()const
{
   ossimGpt result;
   
   if((theSouthwestLongitude==OSSIM_DBL_NAN)||
      (theSouthwestLatitude==OSSIM_DBL_NAN))
   {
      result.makeNan();
   }
   else
   {
      result.latd(theSouthwestLatitude);
      result.lond(theSouthwestLongitude);
   }
   
   return result;
}


bool ossimSrtmFilename::setFilename(const ossimFilename& srtmFilename)
{
   theFilename = srtmFilename;
   theSouthwestLongitude = OSSIM_DBL_NAN;
   theSouthwestLatitude  = OSSIM_DBL_NAN;

   if(srtmFilename == "") return false;

   ossimFilename f = theFilename.fileNoExtension();
   ossimString regularExp1 = "[N|S][0-9][0-9][E|W][0-9][0-9][0-9]";
   ossimString regularExp2 = "[E|W][0-9][0-9][0-9][N|S][0-9][0-9]";
   ossimRegExp regEx;
   bool latLonOrderFlag = true;
   bool foundFlag = false;
   f = f.upcase();

   regEx.compile(regularExp1.c_str());
   foundFlag = regEx.find(f.c_str());
   if(!foundFlag)
   {
      regEx.compile(regularExp2.c_str());
      foundFlag = regEx.find(f.c_str());
      if(foundFlag)
      {
         latLonOrderFlag = false;
         f = ossimFilename(ossimString(f.begin()+regEx.start(),
                                       f.begin()+regEx.end()));
      }
   }
   if(foundFlag)
   {
      f = ossimFilename(ossimString(f.begin()+regEx.start(),
                                    f.begin()+regEx.end()));
   }
   if (f.size() != 7)
   {
      return false;
   }
   ossimString s;

   if(latLonOrderFlag)
   {
      s.push_back(f[1]);
      s.push_back(f[2]);
      theSouthwestLatitude = s.toDouble();
      // Get the latitude.
      if (f[0] == 'S')
      {
         theSouthwestLatitude *= -1;
      }
      else if (f[0] != 'N')
      {
         return false; // Must be either 's' or 'n'.
      }
      // Get the longitude.
      s.clear();
      s.push_back(f[4]);
      s.push_back(f[5]);
      s.push_back(f[6]);
      theSouthwestLongitude = s.toDouble();
      if (f[3] == 'W')
      {
      theSouthwestLongitude *= -1;
      }
      else if (f[3] != 'E')
      {
         return false; // Must be either 'e' or 'w'.
      }
   }
   else
   {
      // Get the longitude.
      s.clear();
      s.push_back(f[1]);
      s.push_back(f[2]);
      s.push_back(f[3]);
      theSouthwestLongitude = s.toDouble();
      if (f[0] == 'W')
      {
      theSouthwestLongitude *= -1;
      }
      else if (f[0] != 'E')
      {
         return false; // Must be either 'e' or 'w'.
      }
      s.clear();
      
      s.push_back(f[5]);
      s.push_back(f[6]);
      theSouthwestLatitude = s.toDouble();
      // Get the latitude.
      if (f[4] == 'S')
      {
         theSouthwestLatitude *= -1;
      }
      else if (f[4] != 'N')
      {
         return false; // Must be either 's' or 'n'.
      }
   }
   
   return true;
}
