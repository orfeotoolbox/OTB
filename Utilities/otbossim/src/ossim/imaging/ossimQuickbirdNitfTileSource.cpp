//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
//*******************************************************************
//  $Id: ossimQuickbirdNitfTileSource.cpp 13330 2008-07-28 18:04:40Z dburken $
#include <ossim/imaging/ossimQuickbirdNitfTileSource.h>
#include <ossim/support_data/ossimQuickbirdTile.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimNitfImageHeader.h>

RTTI_DEF1(ossimQuickbirdNitfTileSource, "ossimQuickbirdNitfTileSource", ossimNitfTileSource);

static const ossimTrace traceDebug("ossimQuickbirdNitfTileSource:debug");

bool ossimQuickbirdNitfTileSource::open()
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQuickbirdNitfTileSource::open(file) DEBUG: entered ..."
         << std::endl;
   }
   
   ossimFilename file = theImageFile;
   file = file.replaceAllThatMatch("_R[0-9]+C[0-9]+");
   ossimQuickbirdTile tileFile;
   bool openedTileFile = false;
   file.setExtension("TIL");
   
   if(!tileFile.open(file))
   {
      file.setExtension("TIL");
      if(tileFile.open(file))
      {
         openedTileFile = true;
      }
      else
      {
         file.setExtension("til");
         if(tileFile.open(file))
         {
            openedTileFile = true;
         }
      }
   }
   else
   {
      openedTileFile = true;
   }
   
   if(openedTileFile)
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimQuickbirdNitfTileSource::open(file) DEBUG:"
            << "\nOpened tile file" << std::endl;
      }

      // Call the base class open...
      if(!ossimNitfTileSource::open())
      {
         return false;
      }
      
      ossimQuickbirdTileInfo info;
      ossimIrect tempBounds = getCurrentImageHeader()->getImageRect();
      
      
      tempBounds = ossimIrect(0,
                              0,
                              tempBounds.width() - 1,
                              tempBounds.height() - 1);
      
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimQuickbirdNitfTileSource::open(file) DEBUG:"
            << "\nheader rectangle = " << tempBounds << std::endl;
      }
      
      ossimIpt ulPt;
      ossimIpt urPt;
      ossimIpt lrPt;
      ossimIpt llPt;
      
      if(tileFile.getInfo(info, theImageFile.file().upcase()))
      {
         ulPt.makeNan();
         urPt.makeNan();
         lrPt.makeNan();
         llPt.makeNan();
         
         if((info.theUlXOffset != OSSIM_INT_NAN) &&
            (info.theUlYOffset != OSSIM_INT_NAN))
         {
            theSubImageOffset = ossimIpt(info.theUlXOffset, info.theUlYOffset);
         }
         else
         {
            theSubImageOffset = ossimIpt(0,0);
         }
          
         ulPt = theSubImageOffset;
         
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimQuickbirdNitfTileSource::open(file) DEBUG:"
               << "\nSub image offset  = " << theSubImageOffset
               << std::endl;
         }
         
         if((info.theUrXOffset != OSSIM_INT_NAN) &&
            (info.theUrYOffset != OSSIM_INT_NAN))
         {
            urPt = ossimIpt(info.theUrXOffset,
                            info.theUrYOffset);
         }
         else
         {
            urPt = tempBounds.ur() + ulPt;
         }
         
         if((info.theLrXOffset != OSSIM_INT_NAN) &&
            (info.theLrYOffset != OSSIM_INT_NAN))
         {
            lrPt = ossimIpt(info.theLrXOffset,
                            info.theLrYOffset);
         }
         else
         {
            lrPt = tempBounds.lr() + ulPt;
         }
         
         if((info.theLlXOffset != OSSIM_INT_NAN) &&
            (info.theLlYOffset != OSSIM_INT_NAN))
         {
            llPt = ossimIpt(info.theLlXOffset,
                            info.theLlYOffset);
         }
         else
         {
            llPt = tempBounds.ll() + ulPt;
         }
      }

      //---
      // Set the base class image rectangle in case a sub image offset was
      // picked up here.
      //---
      setBoundingRectangle(ossimIrect(ulPt, urPt, lrPt, llPt));
      
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimQuickbirdNitfTileSource::open(file) DEBUG:"
            << "\nUl = " << ulPt
            << "\nUr = " << urPt
            << "\nLr = " << lrPt
            << "\nLl = " << llPt
            << "\ntheImageRect:  " << getImageRectangle(0)
            << "\nExiting..." 
            << std::endl;
      }
   }
   else
   {
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimQuickbirdNitfTileSource::open(file) DEBUG"
            << "Not opened..."
            << std::endl;
      }
   }
   
   return openedTileFile;
}
