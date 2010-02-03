//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: 
// 
//********************************************************************
// $Id: ossimGdBitmapFont.cpp 15833 2009-10-29 01:41:53Z eshirschorn $
#include <ossim/font/ossimGdBitmapFont.h>
#include "string.h"

RTTI_DEF1(ossimGdBitmapFont, "ossimGdBitmapFont", ossimFont);

ossimGdBitmapFont::ossimGdBitmapFont(const ossimString& familyName,
                                     const ossimString& styleName,
                                     const ossimGdFont* gdFontPtr)
   :ossimFont(familyName,
              styleName,
              (int)gdFontPtr->w,
              (int)gdFontPtr->h),
    theGdFontPtr(gdFontPtr),
    theBufferSize(0),
    theOutputBuffer(NULL)
{
}

ossimGdBitmapFont::ossimGdBitmapFont(const ossimGdBitmapFont& rhs)
   :ossimFont(rhs),
    theGdFontPtr(rhs.theGdFontPtr),
    theBufferSize(rhs.theBufferSize),
    theOutputBuffer(NULL)    
{
   if(theBufferSize)
   {
      theOutputBuffer = new ossim_uint8[theBufferSize];
      memcpy(theOutputBuffer, rhs.theOutputBuffer, theBufferSize);
   }
}

ossimGdBitmapFont::~ossimGdBitmapFont()
{
   if(theOutputBuffer)
   {
      delete [] theOutputBuffer;
      theOutputBuffer = NULL;
   }
}

void ossimGdBitmapFont::getBoundingBox(ossimIrect& box)
{
   if(theStringToRasterize.size())
   {
      box = ossimIrect(0,0,
                       (int)ceil(theStringToRasterize.size()*theHorizontalPixelSize*theHorizontalScale)-1,
                       (int)ceil(theVerticalPixelSize*theVerticalScale)-1);
   }
   else
   {
      box = ossimIrect(0,
                       0,
                       0,
                       0);
   }

   // I currently don't support rotations and other affine operations so I will comment this
   // bounding box test out and use the above.
   //
#if 0
   if(theGdFontPtr)
   {
      if((theRotation == 0.0)&&
         (theHorizontalScale == 1.0)&&
         (theVerticalScale   == 1.0))
      {
         box = ossimIrect(0,0, theGdFontPtr->w-1, theGdFontPtr->h-1);
      }
      else
      {
         ossimIrect localBox(0,0, theGdFontPtr->w-1, theGdFontPtr->h-1);
         double ulx, uly;
         double llx, lly;
         double lrx, lry;
         double urx, ury;
         
         double min_x, min_y, max_x, max_y;

         ulx = 0.0;
         uly = 0.0;

         llx = (theAffineTransform[0][0]*localBox.ll().x +
                theAffineTransform[0][1]*localBox.ll().y);
         lly = theAffineTransform[1][0]*localBox.ll().x +
               theAffineTransform[1][1]*localBox.ll().y;
         
         lrx = (theAffineTransform[0][0]*localBox.lr().x +
                theAffineTransform[0][1]*localBox.lr().y);
         lry = theAffineTransform[1][0]*localBox.lr().x +
               theAffineTransform[1][1]*localBox.lr().y;
         urx = (theAffineTransform[0][0]*localBox.ur().x +
                theAffineTransform[0][1]*localBox.ur().y);
         ury = theAffineTransform[1][0]*localBox.ur().x +
               theAffineTransform[1][1]*localBox.ur().y;

         
         min_x = std::min(ulx, std::min(llx, (std::min(lrx, urx))));
         max_x = std::max(ulx, std::max(llx, (std::max(lrx, urx))));

         min_y = std::min(uly, std::min(lly, (std::min(lry, ury))));
         max_y = std::max(uly, std::max(lly, (std::max(lry, ury))));


         
         box = ossimIrect((int)floor(min_x), (int)floor(min_y),
                          (int)ceil(max_x), (int)ceil(max_y));
      }
   }
#endif
}

const ossim_uint8* ossimGdBitmapFont::rasterize()
{
   // make sure the buffer is allocated for the output string
   allocateBuffer();

   if(theOutputBuffer)
   {
      memset(theOutputBuffer, 0, theBufferSize);
      rasterizeNormal();
   }

   return theOutputBuffer;
}

void ossimGdBitmapFont::rasterizeNormal()
{
   ossimIrect outBox;
   ossimIrect inBox(0,0,
                    (ossim_int32)theStringToRasterize.length()*theGdFontPtr->w-1,
                    theGdFontPtr->h-1);
   
   getBoundingBox(outBox);

   
   long charOffset = 0;
   long bufOffset  = 0;
   long inW    = inBox.width();
   long inH    = inBox.height();
   long outW = outBox.width();
   long outH = outBox.height();
   ossim_uint8* inBuf = new ossim_uint8[inBox.width()*inBox.height()];
      
   for(std::string::size_type character = 0;
       character < theStringToRasterize.size();
       ++character)
   {
      charOffset = theStringToRasterize[character];
      
      // we need to shift the offset to a positive value.
      // if it goes negative we just add 256 to the value.
      if(charOffset < 0)
      {
         charOffset += 256;
      }

      // which col do we start on
      bufOffset = (long)character*theGdFontPtr->w;
      
      // get the starting offset to the bitmap
      charOffset  = charOffset*theGdFontPtr->w*theGdFontPtr->h;
      
      for(long row = 0; row < (theGdFontPtr->h); ++row)
      {         
         for(long col = 0; col < theGdFontPtr->w; ++col)
         {
            if(theGdFontPtr->data[charOffset])
            {
               inBuf[bufOffset+col] = 255;
            }
            ++charOffset;
         }
         bufOffset += inW;
      }
   }

   if((inW == outW)&&(inH==outH))
   {
      memcpy(theOutputBuffer, inBuf, inW*inH);
   }
   else
   {
      // now lets expand or contract the input buf to the output buf.
      long *xLut = new long[outW];
      long *yLut = new long[outH];
      long i = 0;
      for(i = 0; i < outW; ++i)
      {
         xLut[i] = (int)(i*((double)inW/(double)outW));
      }
      for(i = 0; i < outH; ++i)
      {
         yLut[i] = (int)(i*((double)inH/(double)outH));
      }
      
      ossim_uint8* outBuf = theOutputBuffer;
      for(long row= 0; row < outH; ++row)
      {
         long inYOffset = yLut[row]*inW;
         for(long col=0; col < outW; ++col)
         {
            *outBuf = inBuf[inYOffset + xLut[col]];
            ++outBuf;
         }
      }
      delete [] xLut;
      delete [] yLut;
   }

   delete [] inBuf;
}

void ossimGdBitmapFont::allocateBuffer()
{   
   ossimIrect box;
   
   getBoundingBox(box);
   long bufferSize = box.width()*box.height();
   if(bufferSize != theBufferSize)
   {
      theBufferSize = bufferSize;
      
      if(theOutputBuffer)
      {
         delete theOutputBuffer;
         theOutputBuffer = NULL;
      }
      if(!theOutputBuffer&&theBufferSize)
      {
         theOutputBuffer = new ossim_uint8[theBufferSize];
            
      }
   }   
}

void ossimGdBitmapFont::getFixedSizes(vector<ossimIpt>& sizes)const
{
   sizes.push_back(ossimIpt(theHorizontalPixelSize,
                            theVerticalPixelSize));
}
