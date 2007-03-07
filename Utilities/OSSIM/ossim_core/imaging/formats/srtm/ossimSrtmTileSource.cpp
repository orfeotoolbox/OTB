//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// Image handler class for a Shuttle Radar Topography Mission (SRTM) file.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmTileSource.cpp,v 1.4 2005/06/15 14:15:22 gpotts Exp $

#include <imaging/formats/srtm/ossimSrtmTileSource.h>
#include <base/data_types/ossimDirectory.h>
#include <base/common/ossimTrace.h>
#include <support_data/srtm/ossimSrtmSupportData.h>

RTTI_DEF1_INST(ossimSrtmTileSource,
               "ossimSrtmTileSource",
               ossimGeneralRasterTileSource)

static ossimTrace traceDebug("ossimSrtmTileSource:debug");


//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimSrtmTileSource::ossimSrtmTileSource()
   :
   ossimGeneralRasterTileSource(),
   theSrtmSupportData()
{
}

//*******************************************************************
// Public Constructor:
//*******************************************************************
ossimSrtmTileSource::ossimSrtmTileSource(const ossimKeywordlist& kwl,
                                               const char* prefix)
   :
   ossimGeneralRasterTileSource(),
   theSrtmSupportData()
{
   if (loadState(kwl, prefix) == false)
   {
      theErrorStatus = ossimErrorCodes::OSSIM_ERROR;
   }
}

//*******************************************************************
// Destructor:
//*******************************************************************
ossimSrtmTileSource::~ossimSrtmTileSource()
{
}

bool ossimSrtmTileSource::open()
{
   static const char MODULE[] = "ossimSrtmTileSource::open";
   
   if (traceDebug())
   {
      CLOG << " Entered..." << endl
           << " trying to open file " << theImageFile << std::endl;
   }
   
   if (!theSrtmSupportData.setFilename(theImageFile, true))
   {
      if (traceDebug()) CLOG << " Unable to set filename"<< std::endl;
      
      return false;
   }

   bool result =  ossimGeneralRasterTileSource::open();
   
   if (traceDebug()) CLOG << " Exited with " << (result?"true":"false") << " ..."<< std::endl;
   
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

      if (ossimGeneralRasterTileSource::open())
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
 
