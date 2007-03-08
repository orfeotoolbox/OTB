//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
//
// Description:
// 
// Image handler class for a Shuttle Radar Topography Mission (SRTM) file.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmTileSource.cpp 10275 2007-01-15 16:39:53Z dburken $

#include <ossim/imaging/ossimSrtmTileSource.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimSrtmSupportData.h>

RTTI_DEF1(ossimSrtmTileSource,
               "ossimSrtmTileSource",
               ossimGeneralRasterTileSource)

static ossimTrace traceDebug("ossimSrtmTileSource:debug");

ossimSrtmTileSource::ossimSrtmTileSource()
   :
   ossimGeneralRasterTileSource(),
   theSrtmSupportData()
{
}

ossimSrtmTileSource::~ossimSrtmTileSource()
{
}

bool ossimSrtmTileSource::open()
{
   static const char MODULE[] = "ossimSrtmTileSource::open";
   if (traceDebug())
   {
      CLOG << " Entered..." << std::endl
           << "srtm " << theImageFile << std::endl;
   }
   
   if(isOpen())
   {
      close();
   }

   //---
   // Set the scan for min max flag to true.  This will only scan if not found
   // in a omd file so this is a one time hit typically taken when building
   // reduced res sets.
   //---
   if (!theSrtmSupportData.setFilename(theImageFile, true))
   {
      if (traceDebug()) CLOG << " Unable to set filename"<< std::endl;
      
      return false;
   }

   ossimKeywordlist kwl;
   theSrtmSupportData.saveState(kwl);
   
   ossimGeneralRasterInfo generalRasterInfo;
   bool result = generalRasterInfo.loadState(kwl);
   
   theMetaData.loadState(kwl);
   
   if(result)
   {
       result =  ossimGeneralRasterTileSource::open(generalRasterInfo);
   }
   
   if (traceDebug())
   {
      CLOG << " Exited with " << (result?"true":"false") << " ..."
           << std::endl;
   }
   
   return result;
}
   
bool ossimSrtmTileSource::getImageGeometry(ossimKeywordlist& kwl,
                                              const char* prefix)
{
   // Check for override for an external geometry file, or a previous save.
   if(ossimImageHandler::getImageGeometry(kwl, prefix))
   {
      return true;
   }

   if (theSrtmSupportData.getImageGeometry(kwl, prefix))
   {
      // Capture for next time...
      setImageGeometry(kwl);
      return true;
   }

   return false;
}

bool ossimSrtmTileSource::saveState(ossimKeywordlist& kwl,
                                    const char* prefix)const
{
   if (theSrtmSupportData.saveState(kwl, prefix))
   {
      return ossimImageHandler::saveState(kwl,prefix);
   }
   return false;
}

bool ossimSrtmTileSource::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   if (theSrtmSupportData.loadState(kwl, prefix))
   {
      setFilename(theSrtmSupportData.getFilename());
      if (open())
      {
         // Must call to pick up id for connections.
         return ossimImageSource::loadState(kwl, prefix);
      }
   }

   return false;
}

ossimString ossimSrtmTileSource::getShortName() const
{
   return ossimString("SRTM");
}
   
ossimString ossimSrtmTileSource::getLongName() const
{
   return ossimString("SRTM reader");
}

ossimString  ossimSrtmTileSource::className() const
{
   return ossimString("ossimSrtmTileSource");
}
 
