//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See LICENSE.txt file in the top level directory.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimAnnotationObject.cpp 9094 2006-06-13 19:12:40Z dburken $
#include <ossim/imaging/ossimAnnotationObject.h>

RTTI_DEF1(ossimAnnotationObject, "ossimAnnotationObject", ossimObject)

ossim_uint32 ossimAnnotationObject::theIdCounter = 0;

ossimAnnotationObject::~ossimAnnotationObject()
{}

void ossimAnnotationObject::restIdCounter()
{
  theIdCounter = 0;
}

ossim_uint32 ossimAnnotationObject::getNextId()
{
  ossim_uint32 id = theIdCounter;
  ++theIdCounter;
  return id;
}

ossimAnnotationObject::ossimAnnotationObject(ossim_uint8 r,
                                             ossim_uint8 g,
                                             ossim_uint8 b,
                                             ossim_uint8 thickness)
   :theRed(r),
    theGreen(g),
    theBlue(b),
    theThickness(thickness)
{
  theId = getNextId();
}

void ossimAnnotationObject::setColor(ossim_uint8 r,
                                     ossim_uint8 g,
                                     ossim_uint8 b)
{
   theRed    = r;
   theGreen = g;
   theBlue   = b;
}

void ossimAnnotationObject::setThickness(ossim_uint8 thickness)
{
   theThickness = thickness;
}

void ossimAnnotationObject::getColor(ossim_uint8 &r,
                                     ossim_uint8 &g,
                                     ossim_uint8 &b)const
{
   r = theRed;
   g = theGreen;
   b = theBlue;
}

ossim_uint8 ossimAnnotationObject::getThickness()const
{
   return theThickness;
}

ossim_uint32 ossimAnnotationObject::getId()const
{
  return theId;
}

ossim_uint32 ossimAnnotationObject::setId()
{
  theId = getNextId();
  return getId();
}

bool ossimAnnotationObject::saveState(ossimKeywordlist& kwl,
                                      const char* prefix) const
{
   kwl.add(prefix, "red",       theRed);
   kwl.add(prefix, "green",     theGreen);
   kwl.add(prefix, "blue",      theBlue);
   kwl.add(prefix, "thickness", theThickness);
   
   return ossimObject::saveState(kwl, prefix);
}

bool ossimAnnotationObject::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
{
   const char* red       = kwl.find(prefix, "red");
   const char* green     = kwl.find(prefix, "green");
   const char* blue      = kwl.find(prefix, "blue");
   const char* thickness = kwl.find(prefix, "thickness");

   if(red)
   {
      theRed = (ossim_uint8)ossimString(red).toLong();
   }
   if(green)
   {
      theGreen = (ossim_uint8)ossimString(green).toLong();
   }
   if(blue)
   {
      theBlue = (ossim_uint8)ossimString(blue).toLong();
   }
   if(thickness)
   {
      theThickness = (ossim_uint8)ossimString(thickness).toLong();
   }
   
   return ossimObject::loadState(kwl, prefix);
}

bool ossimAnnotationObject::isPointWithin(const ossimDpt& /* imagePoint */) const
{
   return false;
}

ossimString ossimAnnotationObject::getName()const
{
   return theName;
}

void ossimAnnotationObject::setName(const ossimString& name)
{
   theName = name;
}

ossimDrect ossimAnnotationObject::getBoundingRect()const
{
   ossimDrect rect;
   getBoundingRect(rect);
   return rect;
}

void ossimAnnotationObject::applyScale(const ossimDpt& scale)
{
   applyScale(scale.x,
              scale.y);
}

std::ostream& ossimAnnotationObject::print(ostream& out) const
{
   
   out << setw(15)<<setiosflags(ios::left)<<"Red:" << (long)theRed   << endl
       <<  setw(15)<<setiosflags(ios::left)<<"Green:" << (long)theGreen << endl
       <<  setw(15)<<setiosflags(ios::left)<<"Blue" << (long)theBlue << endl
       <<  setw(15)<<setiosflags(ios::left)<<"Thickness:"<<theThickness
       << "color_red:   " << (long)theRed       << endl
       << "color_green: " << (long)theGreen     << endl
       << "color_blue:  " << (long)theBlue      << endl
       << "thickness:   " << theThickness << endl;
   return out;
}
