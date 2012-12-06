//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken
// 
// Copied from ossimQuickbirdTiffTileSource written by Garrett Potts.
//
// Description:
//
// Class definition for specialized image handler to pick up offsets from
// Quick Bird ".TIL" files.
//
//----------------------------------------------------------------------------
// $Id: ossimQuickbirdTiffTileSource.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimQuickbirdTiffTileSource.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/support_data/ossimQuickbirdTile.h>
#include <ossim/projection/ossimQuickbirdRpcModel.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimQuickbirdTiffTileSource,
          "ossimQuickbirdTiffTileSource",
          ossimTiffTileSource);

static const ossimTrace traceDebug("ossimQuickbirdTiffTileSource:debug");

//*************************************************************************************************
// Checks for presence of required info before flagging successful open
//*************************************************************************************************
bool ossimQuickbirdTiffTileSource::open()
{
   static const char M[] = "ossimQuickbirdTiffTileSource::open()";
   
   bool result = false;
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " ENTERED ..." << std::endl;
   }

   m_tileInfoFilename = theImageFile.noExtension().replaceAllThatMatch("_R[0-9]+C[0-9]+");
   
   // QB is recognized by the presence of a tile info file. Fail if not present:
   m_tileInfoFilename.setExtension("TIL");

   if ( !m_tileInfoFilename.exists() )
   {
      m_tileInfoFilename.setExtension("til");
   }

   if ( m_tileInfoFilename.exists() )
   {
      // Call the base class open...
      result = ossimTiffTileSource::open();
   }

   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " result=" << (result?"true\n":"false\n");
   }
   
   return result;
}

//*************************************************************************************************
// I know what I am. I am a Quickbird TIFF image. I may be standalone or tiled with row-column
// designation in the file name. My projection information is available (possibly) in an 
// accompanying RPC file and/or internal TIF tag, and my transform info (sub-image offset) is 
// available in a TILE info file (*.til) or internal TIF tag. Therefore, I (and not some outside
// factory) am best qualified for establishing my image geometry.
//*************************************************************************************************
ossimRefPtr<ossimImageGeometry> ossimQuickbirdTiffTileSource::getImageGeometry()
{
   if ( !theGeometry )
   {
      //---
      // Check factory for external geom:
      //---
      theGeometry = getExternalImageGeometry();
      
      if ( !theGeometry )
      {
         theGeometry = new ossimImageGeometry();
         
         // Fetch the tile info for this particular image:
         if ( m_tileInfoFilename.size() )
         {
            ossimQuickbirdTile tileFile;
            if ( tileFile.open(m_tileInfoFilename) ) 
            {
               ossimQuickbirdTileInfo info;
               bool infoStatus = tileFile.getInfo(info, theImageFile.file().upcase());
               if ( !infoStatus )
               {
                  infoStatus = tileFile.getInfo(info, theImageFile.file().downcase());
               }

               if ( infoStatus )
               {
                  // Establish sub-image offset (shift) for this tile:
                  ossimDpt shift(0,0);
                  if ((info.theUlXOffset != OSSIM_INT_NAN) && (info.theUlYOffset != OSSIM_INT_NAN))
                     shift = ossimIpt(info.theUlXOffset, info.theUlYOffset);
                  
                  if(traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG)
                        << "ossimQuickbirdTiffTileSource::open() DEBUG:"
                        << "\nSub image offset  = " << shift << std::endl;
                  }

                  // Create the transform and set it in the geometry object:
                  ossimRefPtr<ossim2dTo2dTransform> transform =
                     new ossim2dTo2dShiftTransform(shift);

                  theGeometry->setTransform(transform.get());
   
                  // Next is the projection part of the image geometry. This should be available
                  // as an external RPC file or internal RPC's in the tiff file. Otherwise use
                  // the map projection specified in the 
                  // tiff file:
                  theGeometry->setProjection(0);
                  
                  ossimRefPtr<ossimQuickbirdRpcModel> model = new ossimQuickbirdRpcModel;
                  if (model->parseFile(theImageFile))
                  {
                     theGeometry->setProjection(model.get());
                  }
                  else
                  {
                     // Last resort to a projection factory:
                     ossimRefPtr<ossimProjection> proj = 
                        ossimProjectionFactoryRegistry::instance()->createProjection(this);
                     if (proj.valid()) theGeometry->setProjection(proj.get());
                  }
               }

               // Set image things the geometry object should know about.
               initImageParameters( theGeometry.get() );
            }
         }
      }
   }

   return theGeometry;
}


