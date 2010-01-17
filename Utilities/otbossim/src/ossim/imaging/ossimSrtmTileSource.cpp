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
// $Id: ossimSrtmTileSource.cpp 15801 2009-10-23 20:04:16Z gpotts $

#include <ossim/imaging/ossimSrtmTileSource.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimSrtmSupportData.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

RTTI_DEF1(ossimSrtmTileSource,
               "ossimSrtmTileSource",
               ossimGeneralRasterTileSource)

static ossimTrace traceDebug("ossimSrtmTileSource:debug");

ossimSrtmTileSource::ossimSrtmTileSource()
   :
   ossimGeneralRasterTileSource(),
   m_SrtmSupportData()
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
   if (!m_SrtmSupportData.setFilename(theImageFile, true))
   {
      if (traceDebug()) CLOG << " Unable to set filename"<< std::endl;
      
      return false;
   }

   ossimKeywordlist kwl;
   m_SrtmSupportData.saveState(kwl);
   
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
   
//**************************************************************************************************
//! Returns the image geometry object associated with this tile source or NULL if non defined.
//! The geometry contains full-to-local image transform as well as projection (image-to-world)
//**************************************************************************************************
ossimImageGeometry* ossimSrtmTileSource::getInternalImageGeometry()
{
   // Check for override for an external geometry file, or a previous save.
   if(!theGeometry.valid())
      theGeometry = new ossimImageGeometry();
   ossimKeywordlist kwl;
   if (m_SrtmSupportData.getImageGeometry(kwl))
   {
      // Capture for next time.
      ossimProjection* proj = ossimProjectionFactoryRegistry::instance()->createProjection(kwl);
      if (proj)
      {
         theGeometry->setProjection(proj);
         return theGeometry.get();
      }
   }

   return 0;
}

bool ossimSrtmTileSource::saveState(ossimKeywordlist& kwl,
                                    const char* prefix)const
{
   if (m_SrtmSupportData.saveState(kwl, prefix))
   {
      return ossimImageHandler::saveState(kwl,prefix);
   }
   return false;
}

bool ossimSrtmTileSource::loadState(const ossimKeywordlist& kwl,
                                    const char* prefix)
{
   if (m_SrtmSupportData.loadState(kwl, prefix))
   {
      setFilename(m_SrtmSupportData.getFilename());
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
 
