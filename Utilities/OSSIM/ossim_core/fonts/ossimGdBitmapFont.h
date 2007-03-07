//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: 
// 
//********************************************************************
// $Id: ossimGdBitmapFont.h,v 1.8 2005/09/09 19:23:58 gpotts Exp $
#ifndef ossimGdBitmapFont_HEADER
#define ossimGdBitmapFont_HEADER
#include "fonts/ossimFont.h"

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
