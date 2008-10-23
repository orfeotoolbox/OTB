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
// $Id: ossimQuickbirdTiffTileSource.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <ossim/imaging/ossimQuickbirdTiffTileSource.h>
#include <ossim/support_data/ossimQuickbirdTile.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimQuickbirdTiffTileSource,
          "ossimQuickbirdTiffTileSource",
          ossimTiffTileSource);

static const ossimTrace traceDebug("ossimQuickbirdTiffTileSource:debug");

bool ossimQuickbirdTiffTileSource::open()
{
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQuickbirdTiffTileSource::open() DEBUG: entered ..."
         << std::endl;
   }
   
   ossimFilename file = theImageFile;
   file = file.replaceAllThatMatch("_R[0-9]+C[0-9]+");
   ossimQuickbirdTile tileFile;

   file.setExtension("TIL");
   if(!tileFile.open(file))
   {
      file.setExtension("til");
      if(!tileFile.open(file))
      {
         return false;
      }
   }
   
   // Call the base class open...
   if(!ossimTiffTileSource::open())
   {
      return false;
   }
   
   ossimQuickbirdTileInfo info;
   
   // try upcase...
   if(!tileFile.getInfo(info, theImageFile.file().upcase()))
   {
      // try downcase...
      if(!tileFile.getInfo(info, theImageFile.file().downcase()))
      {
         return false;
      }
   }

   if((info.theUlXOffset != OSSIM_INT_NAN) &&
      (info.theUlYOffset != OSSIM_INT_NAN))
   {
      theSubImageOffset = ossimIpt(info.theUlXOffset, info.theUlYOffset);
   }
   else
   {
      theSubImageOffset = ossimIpt(0,0);
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimQuickbirdTiffTileSource::open() DEBUG:"
         << "\nSub image offset  = " << theSubImageOffset
         << std::endl;
   }
   
   return true;
}
