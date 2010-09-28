//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#include <ossim/elevation/ossimSrtmElevationDatabase.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <sstream>
#include <iomanip>
#include <cstdlib> /* for abs(int) */

static ossimTrace traceDebug("ossimSrtmElevationDatabase:debug");

RTTI_DEF1(ossimSrtmElevationDatabase, "ossimSrtmElevationDatabase", ossimElevationCellDatabase);

double ossimSrtmElevationDatabase::getHeightAboveMSL(const ossimGpt& gpt)
{
   if(isSourceEnabled())
   {
      ossimRefPtr<ossimElevCellHandler> handler = getOrCreateCellHandler(gpt);
      if(handler.valid())
      {
         return handler->getHeightAboveMSL(gpt); // still need to shift
      }
   }
   return ossim::nan();
}

double ossimSrtmElevationDatabase::getHeightAboveEllipsoid(const ossimGpt& gpt)
{
   double h = getHeightAboveMSL(gpt);
   if(h != ossim::nan())
   {
      h += getOffsetFromEllipsoid(gpt);
   }
   
   return h;
}
bool ossimSrtmElevationDatabase::open(const ossimString& connectionString)
{
   bool result = false;
   ossimFilename file = ossimFilename(connectionString);
   m_connectionString = connectionString;
   result = openSrtmDirectory(file);
   
   return result;
}

bool ossimSrtmElevationDatabase::openSrtmDirectory(const ossimFilename& dir)
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimSrtmElevationDatabase::open: Entered....\n";
   }
   if(!m_geoid.valid())
   {
      m_geoid = ossimGeoidManager::instance()->findGeoidByShortName("geoid1996", false);
   }
   
   //---
   // Sample format:
   //            dir
   //         |          |
   //    N35W121.hgt N35W121.hgt
   //---
   ossim_uint32 count = 0;
   ossim_uint32 maxCount = 25;
   if (!dir.isDir())
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimSrtmElevationDatabase::open: Not a directory, leaving ... " << dir << "\n";
      }
      return false;
   }
   
   ossimDirectory od(dir);
   if (od.isOpened() == false)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "ossimSrtmElevationDatabase::open: Unable to open directory " << dir << ", Leaving ... \n";
      }
      return false;
   }
   
   ossimFilename f;
   //ossimSrtmSupportData sd;
   ossimRefPtr<ossimSrtmHandler> handler = new ossimSrtmHandler;
   if(od.getFirst(f, ossimDirectory::OSSIM_DIR_FILES))
   {
      do
      {
         ++count;
         if(handler->open(f))
         {
            m_meanSpacing = handler->getMeanSpacingMeters();
            if(traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "ossimSrtmElevationDatabase::open: Found file " << f << "\n";
               if(!m_geoid.valid())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG) << "ossimSrtmElevationDatabase::open: Unable to load goeid grid 1996 for SRTM database\n";
               }
            }
            return true;
         }
      }while((od.getNext(f)) &&(count < maxCount));
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "ossimSrtmElevationDatabase::open: Leaving ... \n";
   }
   return false;
}

void ossimSrtmElevationDatabase::createRelativePath(ossimFilename& file, const ossimGpt& gpt)const
{
   int ilat =  static_cast<int>(floor(gpt.latd()));
   if (ilat < 0)
   {
      file = "S";
   }
   else
   {
      file = "N";
   }
   
   ilat = abs(ilat);
   std::ostringstream  os1;
   
   os1 << std::setfill('0') << std::setw(2) <<ilat;
   
   file += os1.str().c_str();
   
   int ilon = static_cast<int>(floor(gpt.lond()));
   
   if (ilon < 0)
   {
      file += "W";
   }
   else
   {
      file += "E";
   }
   
   ilon = abs(ilon);
   std::ostringstream  os2;
   os2 << std::setfill('0') << std::setw(3) << ilon;
   
   file += os2.str().c_str();
   file.setExtension("hgt");
}

bool ossimSrtmElevationDatabase::loadState(const ossimKeywordlist& kwl, const char* prefix )
{
   bool result = ossimElevationDatabase::loadState(kwl, prefix);
   if(result)
   {
      if(!m_connectionString.empty()&&ossimFilename(m_connectionString).exists())
      {
         result = open(m_connectionString);
      }
      else
      {
         // can't open the connection because it does not exists or empty
         result = false;
      }
   }
   
   return result;
}

bool ossimSrtmElevationDatabase::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   bool result = ossimElevationDatabase::saveState(kwl, prefix);
   
   return result;
}

ossimRefPtr<ossimElevCellHandler>
ossimSrtmElevationDatabase::createCell(const ossimGpt& gpt)
{

  ossimRefPtr<ossimElevCellHandler> result = 0;
  ossimFilename f;
  createFullPath(f, gpt);

  if(f.exists())
  {
     ossimRefPtr<ossimSrtmHandler> h = new ossimSrtmHandler();
     if (h->open(f, m_memoryMapCellsFlag))
     {
        result = h.get();
     }
  }

  return result;
}
