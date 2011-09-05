//*************************************************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
// Description: Image handler used for tiled Quickbird imagery. 
//
//*************************************************************************************************
//  $Id: ossimQbTileFilesHandler.cpp 2814 2011-07-05 13:40:16Z oscar.kramer $

#include <algorithm>

#include <ossim/imaging/ossimQbTileFilesHandler.h>
#include <ossim/imaging/ossimImageHandlerRegistry.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRegExp.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimPreferences.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimPolygon.h>
#include <ossim/base/ossimStdOutProgress.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/imaging/ossimTiffOverviewBuilder.h>
#include <ossim/imaging/ossimTiffTileSource.h>
#include <ossim/support_data/ossimQuickbirdTile.h>
#include <ossim/projection/ossimQuickbirdRpcModel.h>

RTTI_DEF1(ossimQbTileFilesHandler, "ossimQbTileFilesHandler", ossimTiledImageHandler)

// Static trace for debugging
static ossimTrace traceDebug("ossimQbTileFilesHandler:debug");

//*************************************************************************************************
//!  Constructor (default):
//*************************************************************************************************
ossimQbTileFilesHandler::ossimQbTileFilesHandler()
{
}

//*************************************************************************************************
//! Destructor:
//*************************************************************************************************
ossimQbTileFilesHandler::~ossimQbTileFilesHandler()
{
}

//*************************************************************************************************
//! Opens the image file given entry index. This class does not support multiple entries.
//! @param imageFile File to open.
//! @param entryIndex
//! @return false if entry index other than 0 specified.
//*************************************************************************************************
bool ossimQbTileFilesHandler::open()
{
   static const char* MODULE = "ossimQbTileFilesHandler::open() -- ";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_INFO)
         <<MODULE << "Entering...\nimage: " << theImageFile << "\n";
   }
   
   // Test for extension: image.til
   ossimString ext = theImageFile.ext().downcase();
   if ( ext == "ovr" )
   {
      return false; // Don't try to open overviews.
   }
   
   ossimRegExp regExp("^[t|T][i|I][l|L]");
   if ( regExp.find( ext.c_str() ) )
   {
      if (!theImageFile.exists())
      {
         return false;
      }
   }
      
   // Use ossimQuickbirdTile object for parsing the TIL file and fetching the data structure 
   // containing tile-file info:
   if (traceDebug())
      ossimNotify(ossimNotifyLevel_INFO)<<MODULE<<"OPENING ossimQuickbirdTile..."<<std::endl;
   
   ossimQuickbirdTile qbt;
   bool success = qbt.open(theImageFile);
   if (!success)
      return false;
   
   // Populate our own data structure given the QBT map:
   ossimFilename image_dir (theImageFile.path());
   const ossimQuickbirdTile::TileMap& qbtTileMap = qbt.getMap();

   m_fullImgRect.makeNan();
   ossimIrect subImageRect;
   ossimQuickbirdTile::TileMap::const_iterator qbt_iter = qbtTileMap.begin();
   ossimImageHandlerRegistry* factory = ossimImageHandlerRegistry::instance();
   while (qbt_iter != qbtTileMap.end())
   {
      ossimQuickbirdTileInfo info ((*qbt_iter).second);
      ossimTileFile tilefile;
      tilefile.tileFilename = (*qbt_iter).second.theFilename;
      tilefile.tileFilename.setPath(image_dir);
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_INFO)<<MODULE<<"Using factory to open <"
            <<tilefile.tileFilename<<">"<<std::endl;
      }

      // Now open up the handlers for this tile-file:
      tilefile.imageHandler = factory->open(tilefile.tileFilename);
      if (tilefile.imageHandler.valid())
      {
         // Valid handler, populate remaining items of interest and push it on our list...
         // Fetch image coordinates in full-view:
         subImageRect.set_ulx((*qbt_iter).second.theUlXOffset);
         subImageRect.set_uly((*qbt_iter).second.theUlYOffset);
         subImageRect.set_lrx((*qbt_iter).second.theLrXOffset);
         subImageRect.set_lry((*qbt_iter).second.theLrYOffset);
         tilefile.subImageRects.push_back(subImageRect);

         // Update bounding image rectangle:
         m_fullImgRect = m_fullImgRect.combine(subImageRect);
         m_tileFiles.push_back(tilefile);
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_INFO)<<MODULE<<"Pushed <"<<tilefile.tileFilename<<"> on"
               "to tile-files list."<<std::endl;
         }
      }
      else
      {
         ossimNotify(ossimNotifyLevel_WARN)<<MODULE<<"TIL file listed <"
            <<tilefile.tileFilename<<"> as one of the tiles, but the file could not be opened. "
            <<"This is being ignored but may cause a problem downstream."<<std::endl;
      }
   
      ++qbt_iter;   
   }

   // Need to check the full-image rect for validity. May need to explore the .IMD file if this 
   // was not properly initialized:
   if (m_fullImgRect.hasNans())
   {
      if (!computeImageRects())
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_INFO)<<MODULE<<"Could not establish image rectangles."
               <<std::endl;
         }
         return false;
      }
   }

   if (traceDebug())
      ossimNotify(ossimNotifyLevel_INFO)<<MODULE<<"Finished loading tiles."<<std::endl;

   bool open_good = false;
   if ((m_tileFiles.size() != 0) && m_tileFiles[0].imageHandler.valid())
   {
      completeOpen();
      open_good = true;
      
      if (traceDebug())
         ossimNotify(ossimNotifyLevel_INFO)<<MODULE<<"Open successful."<<std::endl;
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_INFO)<<MODULE<<"Returning with <"
         <<ossimString::toString(open_good)<<">"<<std::endl;
   }
   return open_good;
}

//*************************************************************************************************
//! Fetch the tile-files common RPC data file and create the projection for this image
//*************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimQbTileFilesHandler::getImageGeometry()
{
   // Try external geom first:
   theGeometry = getExternalImageGeometry();
   if (theGeometry.valid())
      return theGeometry;  // We should return here.
   
   // The dataset is expected to have an RPC model associated with it:
   ossimRefPtr<ossimQuickbirdRpcModel> model = new ossimQuickbirdRpcModel(this);
   if (!model->getErrorStatus())
   {
      theGeometry = new ossimImageGeometry;
      theGeometry->setProjection(model.get());
      
      // Set image things the geometry object should know about.
      initImageParameters( theGeometry.get() );

      return theGeometry;
   }
   return ossimRefPtr<ossimImageGeometry>();
}

//*************************************************************************************************
//! Initializes tile image rects by considering adjacent row/col offsets. Called when TIL
//! doesn't contain the info. Returns true if successful.
//*************************************************************************************************
bool ossimQbTileFilesHandler::computeImageRects()
{
   try
   {
      ossimQuickbirdMetaData oqmd;
      if (!oqmd.open(theImageFile))
         throw 1;

      ossimIpt image_size (oqmd.getImageSize());
      if (image_size.hasNans())
         throw 2;

      ossimIpt full_image_lr (image_size.x-1, image_size.y-1);
      m_fullImgRect.set_ulx(0);
      m_fullImgRect.set_uly(0);
      m_fullImgRect.set_lr(full_image_lr);

      // Now consider the subrects for each tile. It is assumed that all tiles (except right
      // and bottom) will be of uniform size. The TIL file should have at least provided the row
      // and column offsets for each tile into the full image. This is equivalent to the UL corner.
      // It is presumed that the TIL failed to provide the LR corner since this method was called.
      int dx=0, dy=0, i=0;
      int num_tiles = (int) m_tileFiles.size();
      while (((dx == 0) || (dy == 0)) && (i < num_tiles-1))
      {
         if (dx == 0)
            dx = m_tileFiles[i+1].subImageRects[0].ul().x - m_tileFiles[i].subImageRects[0].ul().x;
         if (dy == 0)
            dy = m_tileFiles[i+1].subImageRects[0].ul().y - m_tileFiles[i].subImageRects[0].ul().y;
         ++i;
      }

      // We should have the nominal tile size now. Check this is so:
      if ((dx == 0) || (dy == 0))
      {
         // Prior scheme failed for establishing the tile dimensions. Final resort is to open the first
         // available tile file and query its dimensions:
         if (num_tiles == 0)
            throw 3;
         if (m_tileFiles[0].imageHandler.valid())
         {
            // we have an open handler, so just query it:
            dy = m_tileFiles[0].imageHandler->getNumberOfLines();
            dx = m_tileFiles[0].imageHandler->getNumberOfSamples();
         }
         else
         {
            // Struck out, just assume the tile is square:
            if ((dx == 0) && (dy == 0))
               throw 4;
            if (dy == 0)
               dy = dx;
            else
               dx = dy;
         }
      }

      // Loop over all tiles to properly assign their image rectangles:
      for (i=0; i<num_tiles; i++)
      {
         ossimIpt ul (m_tileFiles[i].subImageRects[0].ul());
         ossimIpt lr (ul.x + dx - 1, ul.y + dy - 1);

         // Check to make sure we don't exceed extents of full image:
         if (lr.x > full_image_lr.x)
            lr.x = full_image_lr.x;
         if (lr.y > full_image_lr.y)
            lr.y = full_image_lr.y;

         m_tileFiles[i].subImageRects[0].set_lr(lr);
      }
   }
   
   catch (...)
   {
      // Could not establish the rectangles:
      return false;
   }

   return true;
}

