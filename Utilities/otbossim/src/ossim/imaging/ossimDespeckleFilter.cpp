//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Oscar Kramer
//
//*******************************************************************
//  $Id: ossimDespeckleFilter.cpp 2644 2011-05-26 15:20:11Z oscar.kramer $

#include <ossim/imaging/ossimDespeckleFilter.h>
#include <ossim/imaging/ossimImageData.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNumericProperty.h>

static const ossimKeyword FILTER_RADIUS_KW = ossimKeyword("filter_radius", "");

RTTI_DEF1(ossimDespeckleFilter, "ossimDespeckleFilter", ossimImageSourceFilter);

ossimDespeckleFilter::ossimDespeckleFilter()
   : ossimImageSourceFilter(),
     theTile(NULL),
     theFilterRadius(1)
{
}

ossimDespeckleFilter::ossimDespeckleFilter(ossimImageSource* inputSource, ossim_uint32 filter_radius)
   : ossimImageSourceFilter(inputSource),
     theTile(NULL),
     theFilterRadius(filter_radius)
{
   initialize();
}

ossimDespeckleFilter::~ossimDespeckleFilter()
{
   theTile = 0;
}

ossimRefPtr<ossimImageData> 
ossimDespeckleFilter::getTile(const ossimIrect& tileRect,ossim_uint32 resLevel)
{
   if (!theInputConnection) 
      return NULL;

   if (!isSourceEnabled())
      return theInputConnection->getTile(tileRect, resLevel);
   
   if (!theTile.valid())
   {
      allocate();
      if (!theTile.valid())  // Throw exception???
         return theInputConnection->getTile(tileRect, resLevel);
   }
   
   // Fetch input tile with kernel margins:
   ossimIrect requestRect(tileRect.ul().x - theFilterRadius,
                          tileRect.ul().y - theFilterRadius,
                          tileRect.lr().x + theFilterRadius,
                          tileRect.lr().y + theFilterRadius);
   ossimRefPtr<ossimImageData> inputTile = theInputConnection->getTile(requestRect, resLevel);
   if (!inputTile.valid())
      return NULL;

   ossim_uint32 w  = tileRect.width();
   ossim_uint32 h  = tileRect.height();
   ossim_uint32 tw = theTile->getWidth();
   ossim_uint32 th = theTile->getHeight();
   theTile->setWidth(w);
   theTile->setHeight(h);
   if((w*h)!=(tw*th))
      theTile->initialize();
      
   theTile->makeBlank();
   theTile->setOrigin(tileRect.ul());
   
   ossimDataObjectStatus status = inputTile->getDataObjectStatus();
   if ((status == OSSIM_NULL) || (status == OSSIM_EMPTY))
      return inputTile;

   switch(inputTile->getScalarType())
   {
   case OSSIM_UCHAR:
      despeckle(static_cast<ossim_uint8>(0), inputTile);
      break;
   
   case OSSIM_USHORT16:
   case OSSIM_USHORT11:
      despeckle(static_cast<ossim_uint16>(0), inputTile);
      break;

   case OSSIM_SSHORT16:
      despeckle(static_cast<ossim_sint16>(0), inputTile);
      break;

   case OSSIM_FLOAT:
   case OSSIM_NORMALIZED_FLOAT:
      despeckle(static_cast<float>(0), inputTile);
      break;

   case OSSIM_DOUBLE:
   case OSSIM_NORMALIZED_DOUBLE:
      despeckle(static_cast<double>(0), inputTile);
      break;

   default:
      theTile->loadTile(inputTile.get());
   }
   
   theTile->validate();
   return theTile;
}
   
template <class T>
void ossimDespeckleFilter::despeckle(T /* dummyVariable */, ossimRefPtr<ossimImageData> inputTile)
{
   ossimIpt inUL  (inputTile->getImageRectangle().ul());
   ossimIpt inLR  (inputTile->getImageRectangle().lr());
   ossimIpt outUL (theTile->getImageRectangle().ul());
   ossimIpt outLR (theTile->getImageRectangle().lr());
   long inWidth   = inputTile->getWidth();
   long outWidth  = theTile->getWidth();
   long num_bands = theTile->getNumberOfBands();
   
      // Loop over all bands first:
   for(long b = 0; b < num_bands; ++b)
   {                  
      const T* inbuf = (const T*) inputTile->getBuf(b);
      T* outBuf = (T*) theTile->getBuf(b);
      T null_pixel = (T) inputTile->getNullPix(b);

      for (long y=outUL.y; y<=outLR.y; y++)
      {
         for (long x=outUL.x; x<=outLR.x; x++)
         {
            bool found_valid = false;
            long idx = (y - inUL.y)*inWidth + x - inUL.x; // index to input buffer
            long odx = (y-outUL.y)*outWidth + x - outUL.x;// index to output buffer
          
            T pixel = inbuf[idx];
            if (pixel != null_pixel)
            {
               // Inside loop over input "kernel" pixels centered at x, y:
               for (long iy=-theFilterRadius; (iy<=theFilterRadius) && !found_valid; iy++)
               {
                  // Compute row number in image space and skip this row if outside input tile:
                  long row = y + iy;
                  if ((row < inUL.y) || (row > inLR.y))
                     continue;

                  for (long ix=-theFilterRadius; (ix<=theFilterRadius) && !found_valid; ix++)
                  {
                     // Compute column number in image space and skip this col if outside input tile:
                     long col = x + ix;
                     if ((col < inUL.x) || (col > inLR.x))
                        continue;

                     // Compute offset into input buffer for the neighbor pixel. If we aren't at the 
                     // kernel center, consider if valid neighbor was found:
                     idx = (row - inUL.y)*inWidth + col - inUL.x;
                     if ( ((iy != 0) || (ix != 0)) && (inbuf[idx] != null_pixel) )
                        found_valid = true;
                  }
               }
            }

            // Finished scanning neighborhood. Save output to tile buffer:
            if (found_valid)
               outBuf[odx] = pixel;
            else
               outBuf[odx] = null_pixel;
         }
      }
   }
}

void ossimDespeckleFilter::initialize()
{
   ossimImageSourceFilter::initialize();
   theTile = NULL;
}

void ossimDespeckleFilter::allocate()
{
   if(theInputConnection)
   {
      theTile = ossimImageDataFactory::instance()->create(this, theInputConnection);
      theTile->initialize();
   }
}

bool ossimDespeckleFilter::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   kwl.add(prefix, FILTER_RADIUS_KW, theFilterRadius, true);
   return ossimImageSourceFilter::saveState(kwl, prefix);
}


bool ossimDespeckleFilter::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   const char* lookup = kwl.find(prefix, FILTER_RADIUS_KW);
   if (lookup)
      theFilterRadius = ossimString(lookup).toInt32();

   return ossimImageSourceFilter::loadState(kwl, prefix);
}

void ossimDespeckleFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
   ossimString name = property->getName();
   if(name == "Filter Radius")
      theFilterRadius = property->valueToString().toInt32();
   else
      ossimImageSourceFilter::setProperty(property);
}

ossimRefPtr<ossimProperty> ossimDespeckleFilter::getProperty(const ossimString& name)const
{
   if(name == "Filter Radius")
   {
      ossimProperty* prop = new ossimNumericProperty(name, ossimString::toString(theFilterRadius), 1, 16.0);
      prop->setCacheRefreshBit();
      return prop;
   }
   return ossimImageSourceFilter::getProperty(name);
}

void ossimDespeckleFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimImageSourceFilter::getPropertyNames(propertyNames);
   propertyNames.push_back("Filter Radius");
}

