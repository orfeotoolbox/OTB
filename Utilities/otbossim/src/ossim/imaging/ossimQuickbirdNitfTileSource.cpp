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
//  $Id: ossimQuickbirdNitfTileSource.cpp 21631 2012-09-06 18:10:55Z dburken $
#include <ossim/imaging/ossimQuickbirdNitfTileSource.h>
#include <ossim/support_data/ossimQuickbirdTile.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossim2dTo2dShiftTransform.h>
#include <ossim/imaging/ossimImageGeometry.h>
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
      ossimDpt shift;
      if(tileFile.getInfo(info, theImageFile.file().upcase()))
      {
         ulPt.makeNan();
         urPt.makeNan();
         lrPt.makeNan();
         llPt.makeNan();
         
         if((info.theUlXOffset != OSSIM_INT_NAN) &&
            (info.theUlYOffset != OSSIM_INT_NAN))
         {
            shift = ossimIpt(info.theUlXOffset, info.theUlYOffset);
         }
         else
         {
            shift = ossimIpt(0,0);
         }
         m_transform = new ossim2dTo2dShiftTransform(shift);
      }
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

ossimRefPtr<ossimImageGeometry> ossimQuickbirdNitfTileSource::getImageGeometry()
{
   ossimRefPtr<ossimImageGeometry> result = ossimImageHandler::getImageGeometry();
   if( result.valid() )
   {
      if(!result->getTransform())
      {
         result->setTransform(m_transform.get());
      }
   }
   return result;
}
