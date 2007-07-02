//----------------------------------------------------------------------------
// Copyright (C) 2004 David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// Utility class to partition up a rectangle.
//
// $Id: ossimRectanglePartitioner.cpp 9094 2006-06-13 19:12:40Z dburken $
//----------------------------------------------------------------------------

#include <ossim/base/ossimRectanglePartitioner.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
using namespace std;

static ossimTrace traceDebug("ossimRectanglePartitioner:degug");

ossimRectanglePartitioner::ossimRectanglePartitioner()
{
}

ossimRectanglePartitioner::~ossimRectanglePartitioner()
{
}

void ossimRectanglePartitioner::binaryPartition(
   const ossimIrect& inputRectangle,
   vector<ossimIrect>& result,
   ossim_uint64 maxSizeInBytes,
   ossim_uint32 bands,
   ossim_uint32 bytesPerPixel,
   ossim_uint32 internalOverlapPixels) const
{
   // Clear the result rect for starters.
   result.clear();

   // Some sanity checks.
   if (maxSizeInBytes == 0)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Waning:  0 passed for max size in bytes.  Returning..."
         << endl;
      return;
   }
   if (bands == 0)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Waning:  0 passed for number of bands.  Returning..."
         << endl;
      return;
   }
   if (bytesPerPixel == 0)
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Waning:  0 passed for bytes per pixel.  Returning..."
         << endl;
      return;
   }
   if (inputRectangle.hasNans())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "Waning:  Input rectangle has nans in it!  Returning..."
         << endl;
      return;
   }
   if ( (maxSizeInBytes / (bands*bytesPerPixel)) < 4)
   {
      // Come on now you have to have at least four pixels.
      ossimNotify(ossimNotifyLevel_WARN)
         << "Waning:  Max size in bytes too small.  Returning..."
         << endl;
      return;
   }
   // End of sanity checks...

   // Check the size... We could already be there.
   if (getSize(inputRectangle, bands, bytesPerPixel) <= maxSizeInBytes)
   {
      result.push_back(inputRectangle);

      if (traceDebug())
      {
         trace(inputRectangle, result, maxSizeInBytes, bands, bytesPerPixel);
      }
      
      return;
   }   

   // OK, find the rectangle size that gets that will fit the max size.
   ossimIrect tileRect = inputRectangle;
   
   splitUntilLessThanMax(tileRect,
                         maxSizeInBytes,
                         bands,
                         bytesPerPixel,
                         internalOverlapPixels);

   ossim_int32 input_width  = static_cast<ossim_int32>(inputRectangle.width());
   ossim_int32 input_height = static_cast<ossim_int32>(inputRectangle.height());

   ossim_int32 tile_width   = static_cast<ossim_int32>(tileRect.width());
   ossim_int32 tile_height  = static_cast<ossim_int32>(tileRect.height());

   ossim_int32 tiles_wide   = (input_width % tile_width) ?
      ( (input_width/tile_width)+1) : (input_width/tile_width);
   ossim_int32 tiles_high   = (input_height % tile_height) ?
      ( (input_height/tile_height)+1) : (input_height/tile_height);

   ossim_int32 y_start = inputRectangle.ul().y;
   ossim_int32 y_stop  = y_start + tile_height - 1 + internalOverlapPixels;

   for (ossim_int32 y = 0; y < tiles_high; ++y)
   {
      // Clip to bottom if needed.
      if (y_stop > inputRectangle.lr().y)
      {
         y_stop = inputRectangle.lr().y;
      }
      
      ossim_int32 x_start = inputRectangle.ul().x;
      ossim_int32 x_stop  = x_start + tile_width - 1 + internalOverlapPixels;
      
      for (ossim_int32 x = 0; x < tiles_wide; ++x)
      {
         // Clip to right edge is needed.
         if (x_stop > inputRectangle.lr().x)
         {
            x_stop = inputRectangle.lr().x;
         }

         ossimIrect r(x_start, y_start, x_stop, y_stop);

         // Add it to the result.
         result.push_back(r);
         
         if( 0 == x )
         {
            x_start += tile_width - internalOverlapPixels;
         }
         else
         {
            x_start += tile_width;
         }
         x_stop  += tile_width;
         
      } // End of tiles_wide loop.
      
      if( 0 == y )
      {
         y_start += tile_height - internalOverlapPixels;
      }
      else
      {
         y_start += tile_height;
      }
      y_stop  += tile_height;
      
   } // End of tiles_high loop.
   
   if (traceDebug())
   {
      trace(inputRectangle, result, maxSizeInBytes, bands, bytesPerPixel);
   }
}

void ossimRectanglePartitioner::splitUntilLessThanMax(
   ossimIrect& rect,
   ossim_uint64 maxSizeInBytes,
   ossim_uint32 bands,
   ossim_uint32 bytesPerPixel,
   ossim_uint32 internalOverlapPixels) const
{
   do
   {
      splitRect(rect);

   } while ( getSize(rect,
                     bands,
                     bytesPerPixel,
                     internalOverlapPixels) > maxSizeInBytes );
}

void ossimRectanglePartitioner::splitRect(ossimIrect& rect) const
{
   ossim_int32 width  = static_cast<ossim_int32>(rect.width());
   ossim_int32 height = static_cast<ossim_int32>(rect.height());
   ossim_int32 new_width;
   ossim_int32 new_height;
   
   if (height > width)
   {
      new_width  = width;
      new_height = (height % 2) ? ( (height/2) + 1 ) : (height/2);
   }
   else
   {
      new_width  = (width % 2) ? ( (width/2) + 1) : (width/2);
      new_height = height;
   }
   
   rect = ossimIrect(0, 0, new_width-1, new_height-1);
}

ossim_uint64 ossimRectanglePartitioner::getSize(
   const ossimIrect& rect,
   ossim_uint32 bands,
   ossim_uint32 bytesPerPixel,
   ossim_uint32 internalOverlapPixels) const
{
   return( ( rect.width()  + 2 * internalOverlapPixels ) *
           ( rect.height() + 2 * internalOverlapPixels ) *
           bands * bytesPerPixel );
}

void ossimRectanglePartitioner::trace(const ossimIrect& r,
                                      const std::vector<ossimIrect>& v,
                                      ossim_uint64 maxSizeInBytes,
                                      ossim_uint32 bands,
                                      ossim_uint32 bytesPerPixel) const
{
   ossimNotify(ossimNotifyLevel_DEBUG)
      << "ossimRectanglePartitioner DEBUG:"
      << "\nInput rectangle:            " << r
      << "\nInput rectangle byte size:  " << getSize(r, bands, bytesPerPixel)
      << "\nTile max size in bytes:     " << maxSizeInBytes
      << "\nbands:                      " << bands
      << "\nbytesPerPixel:              " << bytesPerPixel
      << "\nNumber of output tiles:     " << v.size()
      << "\nTiled rectangles:\n";

   int index = 0;
   vector<ossimIrect>::const_iterator i = v.begin();
   while(i != v.end())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "tile[" << index << "]:  " << *i
         << "\nsize in bytes:  " << getSize(*i, bands, bytesPerPixel)
         << endl;
      ++i;
      ++index;
   }
}
