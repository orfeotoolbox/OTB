//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*******************************************************************
//  $Id: ossimGeoidNgs.cpp 17195 2010-04-23 17:32:18Z dburken $
#include <ossim/base/ossimGeoidNgs.h>

#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimDatumFactory.h>


#include <stdlib.h>  /* standard C general utility library */
#include <iostream>
#include <fstream>
using namespace std;

RTTI_DEF1(ossimGeoidNgs, "ossimGeoidNgs", ossimGeoid)

static ossimTrace traceDebug ("ossimGeoidNgs:debug");

ossimGeoidNgs::ossimGeoidNgs()
   :theShortName("")
{
}

ossimGeoidNgs::ossimGeoidNgs(const ossimFilename& dir,
                             ossimByteOrder /* byteOrder */)
{
   open(dir);
}


bool ossimGeoidNgs::open(const ossimFilename& dir,
                         ossimByteOrder byteOrder)
{
   if (!dir.isDir())
   {
      return false;
   }

   if(dir.isDir())
   {
      ossimGeoidNgsHeader h;

      if(dir.dirCat("g2003u01.bin").exists())
      {
         theShortName = "geoid2003";
         addFile(dir.dirCat("g2003u01.bin"), byteOrder);
      }
      if(dir.dirCat("g2003u02.bin").exists())
      {
         theShortName = "geoid2003";
         addFile(dir.dirCat("g2003u02.bin"), byteOrder);
      }
      if(dir.dirCat("g2003u03.bin").exists())
      {
         theShortName = "geoid2003";
         addFile(dir.dirCat("g2003u03.bin"), byteOrder);
      }
      if(dir.dirCat("g2003u04.bin").exists())
      {
         theShortName = "geoid2003";
         addFile(dir.dirCat("g2003u04.bin"), byteOrder);
      }
      if(dir.dirCat("g2003u05.bin").exists())
      {
         theShortName = "geoid2003";
         addFile(dir.dirCat("g2003u05.bin"), byteOrder);
      }
      if(dir.dirCat("g2003u06.bin").exists())
      {
         theShortName = "geoid2003";
         addFile(dir.dirCat("g2003u06.bin"), byteOrder);
      }

      if(dir.dirCat("g1999u01.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999u01.bin"), byteOrder);
      }
      if(dir.dirCat("g1999u02.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999u02.bin"), byteOrder);
      }
      if(dir.dirCat("g1999u03.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999u03.bin"), byteOrder);
      }
      if(dir.dirCat("g1999u04.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999u04.bin"), byteOrder);
      }
      if(dir.dirCat("g1999u05.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999u05.bin"), byteOrder);
      }
      if(dir.dirCat("g1999u06.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999u06.bin"), byteOrder);
      }
      if(dir.dirCat("g1999u07.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999u07.bin"), byteOrder);
      }
      if(dir.dirCat("g1999u08.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999u08.bin"), byteOrder);
      }
      if(dir.dirCat("g1999a01.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999a01.bin"), byteOrder);
      }
      if(dir.dirCat("g1999a02.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999a02.bin"), byteOrder);
      }
      if(dir.dirCat("g1999a03.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999a03.bin"), byteOrder);
      }
      if(dir.dirCat("g1999a04.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999a04.bin"), byteOrder);
      }
      if(dir.dirCat("g1999h01.bin"))
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999h01.bin"), byteOrder);
      }
      if(dir.dirCat("g1999p01.bin").exists())
      {
         theShortName = "geoid1999";
         addFile(dir.dirCat("g1999p01.bin"), byteOrder);
      }
   }
   else
   {
      addFile(dir, byteOrder);
   }
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "Opened geoid grids from:  " << dir.c_str() << endl;
   }
   
   return true;
}

ossimGeoidNgs::~ossimGeoidNgs()
{
}


bool ossimGeoidNgs::addFile(const ossimFilename& file, ossimByteOrder byteOrder)
{
   bool result = false;
   if(file.exists())
   {
      ossim_uint32 idx = 0;
      for(idx = 0; idx < theGridVector.size();++idx)
      {
         if(file == theGridVector[idx].filename())
         {
            result = true;
            return result;
         }
      }
      ossimGeoidNgsHeader h;
      result = h.initialize(file, byteOrder);
      if(result)
      {
         theGridVector.push_back(h);
      }
   }

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimGeoidNgs::addFile: Adding file = " << file << std::endl;
   }
   return result;
}

double ossimGeoidNgs::offsetFromEllipsoid(const ossimGpt& gpt) const
{
   ossimGpt savedGpt = gpt;
   if(ossimDatumFactory::instance()->wgs84())
   {
      savedGpt.changeDatum(ossimDatumFactory::instance()->wgs84());
   }
   double lat = savedGpt.latd();
   double lon = savedGpt.lond();
   fixLatLon(lat, lon);
   
   return deltaHeight(lat, lon); 
}

double ossimGeoidNgs::geoidToEllipsoidHeight(double lat,
                                             double lon,
                                             double geoidHeight) const
{

   fixLatLon(lat, lon);
   double delta = deltaHeight(lat, lon);
   if (!ossim::isnan(delta))
   {
      return (geoidHeight + delta);
   }
   return delta; // nan
}

double ossimGeoidNgs::ellipsoidToGeoidHeight(double lat,
                                      double lon,
                                      double ellipsoidHeight) const
{

   fixLatLon(lat, lon);
   double delta = deltaHeight(lat, lon);
   if (!ossim::isnan(delta))
   {
      return (ellipsoidHeight - delta);
   }
   return delta; // nan
}

void ossimGeoidNgs::fixLatLon(double &lat, double &lon) const
{
   if(lat < 0)
   {
      lat += 180;
   }
   if(lon < 0)
   {
      lon += 360;
   }
}

double ossimGeoidNgs::deltaHeight(double lat, double lon)const
{
   ossim_uint32 idx = 0;
   for(idx = 0; idx < theGridVector.size();++idx)
   {
      if( theGridVector[idx].pointWithin(lat, lon) )
      {
         return theGridVector[idx].getHeightDelta(lat,
                                                  lon);
      }
   }
   return ossim::nan();
}

ossimString ossimGeoidNgs::getShortName()const
{
   return theShortName;
}
