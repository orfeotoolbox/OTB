//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRpfTocEntry.cpp 18052 2010-09-06 14:33:08Z dburken $

#include <istream>
#include <ostream>
#include <iterator>

#include <ossim/support_data/ossimRpfTocEntry.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimErrorCodes.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/projection/ossimEquDistCylProjection.h>
#include <ossim/projection/ossimAzimEquDistProjection.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/support_data/ossimRpfFrameEntry.h>

std::ostream& operator <<(std::ostream& out,
                          const ossimRpfTocEntry& data)
{
   data.print(out);
   return out;
}

ossimRpfTocEntry::ossimRpfTocEntry()
{
   
}

ossimErrorCode ossimRpfTocEntry::parseStream(
   std::istream &in, ossimByteOrder byteOrder)
{
   ossimErrorCode result = ossimErrorCodes::OSSIM_OK;
   
   result = theBoundaryInformation.parseStream(in, byteOrder);
   if(result == ossimErrorCodes::OSSIM_OK)
   {
      allocateFrameEntryArray();
   }

   return result;
}

std::ostream& ossimRpfTocEntry::print(std::ostream& out,
                                      const std::string& prefix) const
{
   theBoundaryInformation.print(out, prefix);

   std::vector< std::vector<ossimRpfFrameEntry> >::const_iterator frameEntry =
      theFrameEntryArray.begin();

   while( frameEntry != theFrameEntryArray.end() )
   {
      std::vector<ossimRpfFrameEntry>::const_iterator i =
         (*frameEntry).begin();
      while ( i != (*frameEntry).end() ) 
      {
         (*i).print(out, prefix);
         ++i;
      }
      ++frameEntry;
   }
   return out;
}

ossim_uint32 ossimRpfTocEntry::getNumberOfFramesHorizontal()const
{
   return theBoundaryInformation.getNumberOfFramesHorizontal();
}

ossim_uint32 ossimRpfTocEntry::getNumberOfFramesVertical()const
{
   return theBoundaryInformation.getNumberOfFramesVertical();
}

ossim_uint32 ossimRpfTocEntry::getNumberOfLines() const
{
   return  theBoundaryInformation.getNumberOfFramesVertical() * 1536;
}

ossim_uint32 ossimRpfTocEntry::getNumberOfSamples() const
{
   return theBoundaryInformation.getNumberOfFramesHorizontal() * 1536;
}

ossim_uint32 ossimRpfTocEntry::getNumberOfBands() const
{
   ossim_uint32 result = 0;
   ossimString productType = getProductType().trim().upcase();
   if( productType == "CIB" )
   {
      result = 1;
   }
   else if ( productType == "CADRG" )
   {
      result = 3;
   }
   return result;
}

void ossimRpfTocEntry::getBoundingRect(ossimIrect& rect) const
{
   rect = ossimIrect(0, 0, getNumberOfSamples()-1, getNumberOfLines()-1); 
}

const ossimRpfBoundaryRectRecord& ossimRpfTocEntry::getBoundaryInformation() const
{
   return theBoundaryInformation;
}

void ossimRpfTocEntry::setEntry(const ossimRpfFrameEntry& entry,
                                long row,
                                long col)
{
   if(row < (long)theFrameEntryArray.size() && row >= 0)
   {
      if(col < (long)theFrameEntryArray[row].size() && col >= 0)
      {
         theFrameEntryArray[row][col] = entry;
      }
   }
}

bool ossimRpfTocEntry::getEntry(long row,
                                long col,
                                ossimRpfFrameEntry& result)const
{
   if(row < (long)theFrameEntryArray.size() && row >= 0)
   {
      if(col < (long)theFrameEntryArray[row].size() && col >= 0)
      {
         result = theFrameEntryArray[row][col];
      }
      else
      {
         return false;
      }
   }
   else
   {
      return false;
   }
   
   return true;
}

ossimString ossimRpfTocEntry::getProductType() const
{
   return theBoundaryInformation.getProductType();
}

/*!
 * If there is an entry and all the files don't exist we will return
 * true.
 */
bool ossimRpfTocEntry::isEmpty()const
{
   long rows = (long)theFrameEntryArray.size();
   long cols = 0;
   if(rows > 0)
   {
      cols = (long)theFrameEntryArray[0].size();
      for(long rowIndex = 0; rowIndex < rows; ++ rowIndex)
      {
         for(long colIndex = 0; colIndex < cols; ++colIndex)
         {
            if(theFrameEntryArray[rowIndex][colIndex].exists())
            {
               return false;
            }
         }
      }
   }

   return true;
}

ossimRefPtr<ossimImageGeometry> ossimRpfTocEntry::getImageGeometry() const
{
   ossimRefPtr<ossimImageGeometry> geom =  new ossimImageGeometry;

   ossimRpfBoundaryRectRecord boundaryInfo = getBoundaryInformation();

   ossimGpt ul(boundaryInfo.getCoverage().getUlLat(),
               boundaryInfo.getCoverage().getUlLon());
   
   ossim_uint32 lines = getNumberOfLines();
   ossim_uint32 samps = getNumberOfSamples();
   
   // Pixel scale:
   ossimDpt scale;
   getScale(scale);
   
   // Tie point - Shifted to point:
   ossimGpt tie( (ul.latd() - (scale.y/2.0)), (ul.lond() + (scale.x/2.0)), 0.0 );
   
   // Origin - Use the center of the image aligning to tie point.
   // ossimGpt origin((ul.latd()+lr.latd())*.5, (ul.lond()+lr.lond())*.5, 0.0);
   ossimGpt origin( tie.latd() - ((lines/2)*scale.y),
                    tie.lond() + ((samps/2)*scale.x) );

   int z = boundaryInfo.getZone();
   
   if (z == 74) z--; // Fix J to a zone.
   if (z > 64) z -= 64; // Below the equator
   else z -= 48; // Above the equator

   ossimRefPtr<ossimMapProjection> mapProj = 0;
   if ( z != 9 )
   {
      mapProj = new ossimEquDistCylProjection;
   }
   else
   {
      mapProj = new ossimAzimEquDistProjection;
   }

   // Set the origin:
   mapProj->setOrigin(origin);
   
   // Set the scale:
   mapProj->setDecimalDegreesPerPixel(scale);

   // Set the tie:
   mapProj->setUlTiePoints(tie);

   // Give projection to the geometry object.
   geom->setProjection( mapProj.get() );

   return geom;
}

void ossimRpfTocEntry::getScale(ossimDpt& scale) const
{
   scale.x = theBoundaryInformation.getCoverage().getHorizontalInterval();
   scale.y = theBoundaryInformation.getCoverage().getVerticalInterval();
}

void ossimRpfTocEntry::allocateFrameEntryArray()
{
   theFrameEntryArray.resize(theBoundaryInformation.getNumberOfFramesVertical());
   ossim_uint32 horizontalSize = theBoundaryInformation.getNumberOfFramesHorizontal();

   for(ossim_uint32 index = 0; index < theFrameEntryArray.size(); index++)
   {
      theFrameEntryArray[index].resize(horizontalSize);
   }
}
