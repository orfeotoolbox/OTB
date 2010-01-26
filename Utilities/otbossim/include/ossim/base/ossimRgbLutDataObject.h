//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRgbLutDataObject.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimRgbLutDataObject_HEADER
#define ossimRgbLutDataObject_HEADER
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimObject.h>
#include <iosfwd>

class ossimKeywordlist;

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
   virtual ossimObject* dup()const
   {
      return new ossimRgbLutDataObject(*this);
   }
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
   virtual ~ossimRgbLutDataObject();

   ossimRgbVector *theLut;
   ossim_uint32 theNumberOfEntries;

TYPE_DATA
};

#endif
