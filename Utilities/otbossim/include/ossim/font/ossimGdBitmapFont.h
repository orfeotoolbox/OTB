//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// Description: 
// 
//********************************************************************
// $Id: ossimGdBitmapFont.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimGdBitmapFont_HEADER
#define ossimGdBitmapFont_HEADER
#include <ossim/font/ossimFont.h>

struct ossimGdFont
{
	/* # of characters in font */
	int nchars;
	/* First character is numbered... (usually 32 = space) */
	int offset;
	/* Character width and height */
	int w;
	int h;
	/* Font data; array of characters, one row after another.
		Easily included in code, also easily loaded from
		data files. */
	char *data;
};
typedef ossimGdFont* ossimGdFontPtr;

/*!
 *  This is a wrapper for the gd library fonts.
 *  Note that this class will currently only support scaling.  It will not support
 *  any other affine operation.
 */
class ossimGdBitmapFont : public ossimFont
{
public:
   
   ossimGdBitmapFont(const ossimString& familyName,
                     const ossimString& styleName,
                     const ossimGdFont* gdFontPtr);

   ossimGdBitmapFont(const ossimGdBitmapFont& rhs);
   virtual ~ossimGdBitmapFont();

   virtual ossimObject* dup()const
      {
         return new ossimGdBitmapFont(*this);
      }
   virtual const ossim_uint8* getBuf()const
      {
         return theOutputBuffer;
      }
    /*!
    * Will return an internal buffer for rasterizing and will
    */
   virtual const ossim_uint8* rasterize();
   virtual void getBoundingBox(ossimIrect& box);
   bool isFixed()const
      {
         return true;
      }
   
   virtual void getFixedSizes(vector<ossimIpt>& sizes)const;

protected:
   const ossimGdFont* theGdFontPtr;

   long         theBufferSize;
   ossim_uint8* theOutputBuffer;

   /*!
    * This will take into consideration the point size along
    * the horizontal and vertical and also the scale
    * values.  So the actual scale will be:
    *
    * Scale*(pointSize/fixedSize);
    */
   void getActualScale(ossimDpt& scales)const;

   void rasterizeNormal();
   void allocateBuffer();

TYPE_DATA
};

#endif
