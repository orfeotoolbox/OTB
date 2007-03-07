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
// Author: Garrett Potts (gpotts@imagelinks)
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimRgbLutDataObject.h,v 1.12 2004/08/07 20:40:02 dburken Exp $
#ifndef ossimRgbLutDataObject_HEADER
#define ossimRgbLutDataObject_HEADER
#include "base/data_types/color_space/ossimRgbVector.h"
#include "base/common/ossimObject.h"
#include <iostream>
class OSSIMDLLEXPORT ossimKeywordlist;

class OSSIMDLLEXPORT ossimRgbLutDataObject : public ossimObject
{
public:
   friend std::ostream& operator <<(std::ostream& out,
                                    const ossimRgbLutDataObject& lut);
   /*!
    * Will allocate 256 values for the color table
    */
   ossimRgbLutDataObject(unsigned long numberOfEntries=0);
   ossimRgbLutDataObject(const ossimRgbLutDataObject& lut);
   virtual ~ossimRgbLutDataObject();
   const ossimRgbVector& operator[](unsigned int index)const
      {
         return index<theNumberOfEntries?theLut[index]:theLut[theNumberOfEntries-1];
      }
   ossimRgbVector& operator[](unsigned int index)
      {
         return index<theNumberOfEntries?theLut[index]:theLut[theNumberOfEntries-1];
      }
   const ossimRgbVector& operator[](int index)const
      {
         return index<(long)theNumberOfEntries?theLut[index]:theLut[theNumberOfEntries-1];
      }
   ossimRgbVector& operator[](int index)
      {
         return index<(long)theNumberOfEntries?theLut[index]:theLut[theNumberOfEntries-1];
      }
   const ossimRgbVector& operator[](double normalizedIndex)const
      {
         int index = int(normalizedIndex*theNumberOfEntries+.5);
         if (index < 0)
         {
            index = 0;
         }
         return (*this)[index];
      }
   ossimRgbVector& operator[](double normalizedIndex)
      {
         int index = int(normalizedIndex*theNumberOfEntries+.5);
         if (index < 0)
         {
            index = 0;
         }
         return (*this)[index];
      }
   long getNumberOfEntries()const{return theNumberOfEntries;}
   int findIndex(const ossimRgbVector& rgb)
      {
         return findIndex(rgb.getR(),
                          rgb.getG(),
                          rgb.getB());
      }
   int findIndex(ossim_uint8 r, ossim_uint8 g, ossim_uint8 b);
   ossimRgbLutDataObject& rotate(long numberOfElements=1);
   ossimRgbLutDataObject  rotate(long numberOfElements=1)const;
   
   const ossimRgbLutDataObject& operator =(const ossimRgbLutDataObject& lut);
   bool operator ==(const ossimRgbLutDataObject& lut)const;

   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
protected:
   ossimRgbVector *theLut;
   ossim_uint32 theNumberOfEntries;

TYPE_DATA
};

#endif
