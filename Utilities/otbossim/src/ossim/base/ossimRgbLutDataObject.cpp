//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimRgbLutDataObject.cpp 13710 2008-10-14 16:27:57Z gpotts $
#include <ossim/base/ossimRgbLutDataObject.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/base/ossimNotifyContext.h>

#include <sstream>
using namespace std;

RTTI_DEF1(ossimRgbLutDataObject, "ossimRgbLutDataObject", ossimObject);

ostream& operator <<(ostream& out,
                     const ossimRgbLutDataObject& lut)
{
   for(ossim_uint32 index = 0; index < lut.theNumberOfEntries; ++index)
   {
      out << "entry" << index << " " << lut[index] << endl;
   }

   return out;
}

ossimRgbLutDataObject::ossimRgbLutDataObject(unsigned long numberOfEntries)
   :
      theLut(NULL),
      theNumberOfEntries(numberOfEntries)
{
   if(theNumberOfEntries > 0)
   {
      // allocate 256 entries for the data object;
      theLut = new ossimRgbVector[theNumberOfEntries];
   }
      
}

ossimRgbLutDataObject::ossimRgbLutDataObject(const ossimRgbLutDataObject& lut)
   :
      theLut(NULL),
      theNumberOfEntries(0)
{
   theNumberOfEntries = lut.theNumberOfEntries;
   if(theNumberOfEntries > 0)
   {
      theLut = new ossimRgbVector[theNumberOfEntries];
      for(ossim_uint32 index = 0; index < theNumberOfEntries; ++index)
      {
         theLut[index] = lut.theLut[index];
      }
   }
}

ossimRgbLutDataObject::~ossimRgbLutDataObject()
{
   if(theLut)
   {
      delete [] theLut;
      theLut = NULL;
   }
   theNumberOfEntries = 0;
}

int ossimRgbLutDataObject::findIndex(ossim_uint8 r, ossim_uint8 g, ossim_uint8 b)
{
   ossim_uint32 distance = 0x7FFFFFFF; // max 4 byte signed
   ossim_int32 result   = 0;

   if(theNumberOfEntries > 0)
   {
      for(ossim_uint32 i = 0; i < theNumberOfEntries; ++i)
      {
         ossim_uint32 rDelta = r - theLut[i].getR();
         ossim_uint32 gDelta = g - theLut[i].getG();
         ossim_uint32 bDelta = b - theLut[i].getB();

         ossim_uint32 deltaSumSquare = (rDelta*rDelta +
                                        gDelta*gDelta +
                                        bDelta*bDelta);
         if(deltaSumSquare == 0)
         {
            return static_cast<int>(i);
         }
         else if( deltaSumSquare < distance)
         {
            result = static_cast<int>(i);
            distance = deltaSumSquare;
         }
      }
   }

   return result;
}

ossimRgbLutDataObject ossimRgbLutDataObject::rotate(long numberOfElements)const
{
   if(numberOfElements < 0)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL: Negative rotation is not supported yet in ossimRgbLutDataObject::rotate" << endl;
      return *this;
   }
   ossimRgbLutDataObject lut;
   for(ossim_uint32 index = 0; index < theNumberOfEntries; ++index)
   {
      int adjustedDestinationIndex = (index+numberOfElements)%theNumberOfEntries;
      lut[adjustedDestinationIndex] = theLut[index] ;
   }

   return lut;
}

ossimRgbLutDataObject& ossimRgbLutDataObject::rotate(long numberOfElements)
{
   if(numberOfElements < 0)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL: Negative rotation is not supported yet in ossimRgbLutDataObject::rotate" << endl;
      return *this;
   }
   const ossimRgbLutDataObject* temp = this;

   *this = temp->rotate(numberOfElements);

   return *this;
}

const ossimRgbLutDataObject& ossimRgbLutDataObject::operator =(const ossimRgbLutDataObject& lut)
{
   if(theNumberOfEntries != lut.theNumberOfEntries)
   {
      delete [] theLut;
      theLut = NULL;
   }

   theNumberOfEntries = lut.theNumberOfEntries;
   if(!theLut&&(theNumberOfEntries > 0))
   {
      theLut = new ossimRgbVector[theNumberOfEntries];
   }
   for(unsigned long index = 0; index < theNumberOfEntries; ++index)
   {
      theLut[index] = lut.theLut[index];
   }

   return *this;
}

bool ossimRgbLutDataObject::operator ==(const ossimRgbLutDataObject& lut)const
{
   if(theNumberOfEntries != lut.theNumberOfEntries)
   {
      return false;
   }
   for(unsigned long index = 0; index < theNumberOfEntries; ++index)
   {
      if(theLut[index] != lut.theLut[index])
      {
         return false;
      }
   }
   return true;
}

bool ossimRgbLutDataObject::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   kwl.add(prefix,
           "type",
           getClassName(),
           true);
   kwl.add(prefix,
           "number_of_entries",
           ossimString::toString(theNumberOfEntries).c_str(),
           true);
   for(ossim_uint32 index = 0; index < theNumberOfEntries; ++index)
   {
      ossimString newPrefix = "entry";
      newPrefix += ossimString::toString(index);
      ostringstream ostr;
      ostr << (int)(theLut[index].getR())
           << " " << (int)(theLut[index].getG())
           << " " << (int)(theLut[index].getB());
      kwl.add(prefix,
              newPrefix,
              ostr.str().c_str(),
              true);
   }

   return true;
}

bool ossimRgbLutDataObject::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   const char* lutFile = kwl.find(prefix, "lut_file");
   ossimKeywordlist fileLut;
   ossimKeywordlist* tempKwl = (const_cast<ossimKeywordlist*>(&kwl));
   ossimString tempPrefix = prefix;

   // this should have been used instead of lut_file.  We will still look
   // for lut_file for backward compatibility.
   //
   if(!lutFile)
   {
      lutFile = kwl.find(prefix, "filename");
   }
   // check to see if we should open an external file
   // if so point the fileLut to the one that we use
   if(lutFile)
   {
      ossimFilename filename(lutFile);
      if(filename.exists())
      {
         fileLut.addFile(filename.c_str());
         tempKwl = &fileLut;
         tempPrefix = "";
      }
   }
 
   const char* numberOfEntries = tempKwl->find(tempPrefix, "number_of_entries");
   if(!numberOfEntries)
   {
      numberOfEntries = tempKwl->find(tempPrefix, "number_entries");
   }
   if(!numberOfEntries) return false;
   theNumberOfEntries = ossimString(numberOfEntries).toULong();

   delete [] theLut;
   theLut = new ossimRgbVector[theNumberOfEntries];

   if(tempKwl->find(tempPrefix, "entry0"))
   {
      for(ossim_uint32 index = 0; index < theNumberOfEntries; ++index)
      {
         ossimString newPrefix = "entry";
         newPrefix += ossimString::toString(index);
         ossimString v = tempKwl->find(tempPrefix, newPrefix.c_str());
         istringstream istr(v);

         ossimString r, g, b;
         istr >> r >> g >> b;
         theLut[index].setR((unsigned char)r.toInt32());
         theLut[index].setG((unsigned char)g.toInt32());
         theLut[index].setB((unsigned char)b.toInt32());
      }
   }
   else
   {
      for(ossim_uint32 index = 0; index < theNumberOfEntries; ++index)
      {
         ossimString newPrefix = "entry";
         newPrefix += ossimString::toString(index);
         
         const char* r = tempKwl->find(tempPrefix, (newPrefix+".r").c_str());
         const char* g = tempKwl->find(tempPrefix, (newPrefix+".g").c_str());
         const char* b = tempKwl->find(tempPrefix, (newPrefix+".b").c_str());
         
         if(r)
         {
            theLut[index].setR((unsigned char)ossimString(r).toLong());
         }
         if(g)
         {
            theLut[index].setG((unsigned char)ossimString(g).toLong());
         }
         if(b)
         {
            theLut[index].setB((unsigned char)ossimString(b).toLong());
         }
      }
   }
   return true;
}
