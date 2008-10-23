//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
// 
// Shuttle Radar Topography Mission (SRTM) factory to return an
// ossimSrtmElevSource given a ground point.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmFactory.cpp 12982 2008-06-04 01:12:46Z dburken $

#include <cstdlib> /* abs() */
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ossim/elevation/ossimSrtmFactory.h>
#include <ossim/elevation/ossimSrtmHandler.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimStreamFactoryRegistry.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/support_data/ossimSrtmFilename.h>
#include <ossim/support_data/ossimSrtmSupportData.h>
#include <ossim/elevation/ossimElevationShapeIdx.h>

static ossimTrace traceDebug ("ossimSrtmFactory:debug");

RTTI_DEF1(ossimSrtmFactory, "ossimSrtmFactory", ossimElevSourceFactory)

ossimSrtmFactory::ossimSrtmFactory()
   : ossimElevSourceFactory()
{}

ossimSrtmFactory::ossimSrtmFactory(const ossimFilename& dir)
   : ossimElevSourceFactory()
{
   theDirectory = dir;
}
      
ossimSrtmFactory::~ossimSrtmFactory()
{}

ossimElevSource* ossimSrtmFactory::getNewElevSource(const ossimGpt& gpt) const
{
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimSrtmFactory::getNewElevSource: Entered..."
         << std::endl;
   }

   ossimElevManager* mgr = ossimElevManager::instance();
   
   ossimElevSource* srtmPtr = NULL;

   if (theDirectory == ossimFilename::NIL)
   {
      ossimNotify(ossimNotifyLevel_FATAL)
         << "ossimSrtmFactory::getNewElevSource: "
         << "SRTM directory has not been set!"
         << "\nReturning null elevation source..."
         << std::endl;
      
      return srtmPtr;
   }

   //---
   // Build up a srtm file name.
   //
   // Standard for name is upper case 'N' and 'W' lower case "hgt" like:
   // N27W081.hgt
   //---
   ossimFilename srtmFileBasename;

   int ilat =  static_cast<int>(floor(gpt.latd()));
   if (ilat < 0)
   {
      srtmFileBasename = "S";
   }
   else
   {
      srtmFileBasename = "N";
   }

   ilat = abs(ilat);
   std::ostringstream  os1;
   
   os1 << std::setfill('0') << std::setw(2) <<ilat;
   
   srtmFileBasename += os1.str().c_str();

   int ilon = static_cast<int>(floor(gpt.lond()));
   
   if (ilon < 0)
   {
      srtmFileBasename += "W";
   }
   else
   {
      srtmFileBasename += "E";
   }

   ilon = abs(ilon);
   std::ostringstream  os2;
   os2 << std::setfill('0') << std::setw(3) << ilon;
   
   srtmFileBasename += os2.str().c_str();
   srtmFileBasename.setExtension(".hgt");

   ossimFilename srtmFile = theDirectory.dirCat(srtmFileBasename);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimSrtmFactory::getNewElevSource:"
         << "\nSearching for file:  " << srtmFile
         << std::endl;
   }

   ossimRefPtr<ossimIFStream> is = ossimStreamFactoryRegistry::instance()->
      createNewIFStream(srtmFile, std::ios::in | std::ios::binary);

   // Look for the file mix case, then all lower case, then all upper case.
   if (is.valid())
   {
      if(is->fail())
      {
         // Try down casing the whole thing.
         srtmFileBasename = srtmFileBasename.downcase();
         srtmFile = theDirectory.dirCat(srtmFileBasename);
         
         is =  ossimStreamFactoryRegistry::instance()->
            createNewIFStream(srtmFile, std::ios::in | std::ios::binary);      
         if (is.valid())
         {
            if(is->fail())
            {
               // OK, try upcasing the whole thing.
               srtmFileBasename = srtmFileBasename.upcase();
               srtmFile = theDirectory.dirCat(srtmFileBasename);
               is =  ossimStreamFactoryRegistry::instance()->
                  createNewIFStream(srtmFile, std::ios::in | std::ios::binary);
            }
         }
      }
   }

   if ( is.valid() && (!is->fail())&&!(mgr->isCellOpen(srtmFile)) )
   {
      is->close();
      is = 0;
      srtmPtr = new ossimSrtmHandler(srtmFile);
      if (srtmPtr->pointHasCoverage(gpt) )
      {
         return srtmPtr;
      }
      else
      {
         delete srtmPtr;
         srtmPtr = NULL;
      }
   }
   return srtmPtr;
}

void ossimSrtmFactory::createIndex()
{
   ossimDirectory dir;

   if(dir.open(theDirectory))
   {
      ossimFilename f;
      if(dir.getFirst(f, ossimDirectory::OSSIM_DIR_FILES))
      {
         ossimRefPtr<ossimElevationShapeIdx> shpIdx = 0;
         try
         {
            shpIdx = new ossimElevationShapeIdx(theDirectory, true);
         }
         catch(...)
         {
            shpIdx = 0;
            return;
         }
         
         do
         {
            if(f.contains(".hgt"))
            {
               // check and remove the gz extension if present
               //
               ossimFilename file = f.file();
               if(f.ext() == "gz")
               {
                  f = f.setExtension("");
               }
               ossimSrtmFilename srtmFile;
               if(srtmFile.setFilename(f.file()))
               {
                  ossimSrtmSupportData srtmSupport;

                  if(srtmSupport.setFilename(f, true))
                  {
                     shpIdx->add(f,
                                 srtmFile.ul().lond(),
                                 srtmFile.ll().latd(),
                                 srtmSupport.getMinPixelValue(),
                                 srtmFile.ur().lond(),
                                 srtmFile.ur().latd(),
                                 srtmSupport.getMaxPixelValue());
                  }
               }
            }
         }while(dir.getNext(f));
         shpIdx->buildQuadTree();
      }
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN) << "ossimSrtmFactory::createIndex(): Directory can't be opened for indexing." << std::endl;
   }
}
