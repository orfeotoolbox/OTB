//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimFontInformation.cpp 9094 2006-06-13 19:12:40Z dburken $

#include <iostream>
#include <sstream>
#include <ossim/base/ossimFontInformation.h>
#include <ossim/base/ossimKeywordlist.h>

const char* ossimFontInformation::FAMILY_NAME_KW  = "family_name";
const char* ossimFontInformation::STYLE_NAME_KW   = "style_name";
const char* ossimFontInformation::POINT_SIZE_KW   = "point_size";
const char* ossimFontInformation::POINT_SIZE_X_KW = "point_size_x";
const char* ossimFontInformation::POINT_SIZE_Y_KW = "point_size_y";
const char* ossimFontInformation::FIXED_FLAG_KW   = "fixed_flag";
const char* ossimFontInformation::SHEAR_KW        = "shear";
const char* ossimFontInformation::SHEAR_X_KW      = "shear_x";
const char* ossimFontInformation::SHEAR_Y_KW      = "shear_y";
const char* ossimFontInformation::SCALE_KW        = "scale";
const char* ossimFontInformation::SCALE_X_KW      = "scale_x";
const char* ossimFontInformation::SCALE_Y_KW      = "scale_y";
const char* ossimFontInformation::ROTATION_KW     = "rotation";

std::ostream& operator << (std::ostream& out,
                           const ossimFontInformation& rhs)
{
   out << "Family name:  " << rhs.theFamilyName
       << "\nStyle name:   " << rhs.theStyleName
       << "\nPoint size:   " << rhs.thePointSize
       << "\nFixed flag:   " << (rhs.theFixedFlag?"true":"false")
       << std::endl;
   
   return out;
}

ossimFontInformation::ossimFontInformation()
   :theFamilyName(""),
    theStyleName(""),
    thePointSize(0,0),
    theFixedFlag(false),
    theScale(1.0,1.0),
    theRotation(0.0),
    theShear(0.0,0.0)
{
}

ossimFontInformation::ossimFontInformation(const ossimString& family,
                                           const ossimString& style,
                                           const ossimIpt&    pointSize,
                                           bool               fixedFlag,
                                           const ossimDpt&    scale,
                                           double             rotation,
                                           const ossimDpt&    shear)
   :theFamilyName(family),
    theStyleName(style),
    thePointSize(pointSize),
    theFixedFlag(fixedFlag),
    theScale(scale),
    theRotation(rotation),
    theShear(shear)
{}

ossimFontInformation::ossimFontInformation(const ossimFontInformation& rhs)
   : theFamilyName(rhs.theFamilyName),
     theStyleName(rhs.theStyleName),
     thePointSize(rhs.thePointSize),
     theFixedFlag(rhs.theFixedFlag),
     theScale(rhs.theScale),
     theRotation(rhs.theRotation),
     theShear(rhs.theShear)
{
}

bool ossimFontInformation::isFixed()const
{
   return theFixedFlag;
}

bool ossimFontInformation::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   kwl.add(prefix,
           FAMILY_NAME_KW,
           theFamilyName,
           true);
   kwl.add(prefix,
           STYLE_NAME_KW,
           theStyleName,
           true);

   kwl.add(prefix,
           POINT_SIZE_KW,
           thePointSize.toString().c_str(),
           true);
      
   kwl.add(prefix,
           SHEAR_KW,
           theShear.toString().c_str(),
           true);

   kwl.add(prefix,
           SCALE_KW,
           theScale.toString().c_str(),
           true);

#if 0
   kwl.add(prefix,
           POINT_SIZE_X_KW,
           thePointSize.x,
           true);
   kwl.add(prefix,
           POINT_SIZE_Y_KW,
           thePointSize.y,
           true);
   kwl.add(prefix,
           SHEAR_Y_KW,
           theShear.y,
           true);
   kwl.add(prefix,
           SCALE_X_KW,
           theScale.x,
           true);
   kwl.add(prefix,
           SCALE_X_KW,
           theScale.x,
           true);
   kwl.add(prefix,
           SCALE_Y_KW,
           theScale.y,
           true);
#endif
   
   kwl.add(prefix,
           FIXED_FLAG_KW,
           (int)theFixedFlag,
           true);
   kwl.add(prefix,
           ROTATION_KW,
           theRotation,
           true);
   

   return true;
}
   
bool ossimFontInformation::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   bool result = true;
   
   const char* family_name = kwl.find(prefix, FAMILY_NAME_KW);
   const char* style_name  = kwl.find(prefix, STYLE_NAME_KW);
   const char* fixed       = kwl.find(prefix, FIXED_FLAG_KW);
   const char* rotation    = kwl.find(prefix, ROTATION_KW);

   if(family_name)
   {
      theFamilyName = family_name;
   }
   else
   {
      result = false;
   }

   if(style_name)
   {
      theStyleName = style_name;
   }
   else
   {
      result = false;
   }

   // Look for point_size:
   const char* lookup;
   lookup = kwl.find(prefix, POINT_SIZE_KW);
   if (lookup)
   {
      std::istringstream is(lookup);
      is >> thePointSize;
   }
   else // backwards compat...
   {
      const char* point_x = kwl.find(prefix, POINT_SIZE_X_KW);
      const char* point_y = kwl.find(prefix, POINT_SIZE_Y_KW);
      if((point_x)&&(point_y))
      {
         thePointSize = ossimIpt(ossimString(point_x).toLong(),
                                 ossimString(point_y).toLong());
      }
      else
      {
         result = false;
      }
   }

   // Look for shear:
   lookup = kwl.find(prefix, SHEAR_KW);
   if (lookup)
   {
      std::istringstream is(lookup);
      is >> theShear;
   }
   else
   {
      const char* shear_x  = kwl.find(prefix, SHEAR_X_KW);
      if(shear_x)
      {
         theShear.x = ossimString(shear_x).toDouble();
      }
      else
      {
         result = false;
      }
      const char* shear_y  = kwl.find(prefix, SHEAR_Y_KW);
      if(shear_y)
      {
         theShear.y = ossimString(shear_y).toDouble();
      }
      else
      {
         result = false;
      }
   }

   // Look for scale:
   lookup = kwl.find(prefix, SCALE_KW);
   if (lookup)
   {
      std::istringstream is(lookup);
      is >> theScale;
   }
   else
   {
      const char* scale_x  = kwl.find(prefix, SCALE_X_KW);
      if(scale_x)
      {
         theScale.x = ossimString(scale_x).toDouble();
      }
      else
      {
         result = false;
      }
      const char* scale_y  = kwl.find(prefix, SCALE_Y_KW);
      if(scale_y)
      {
         theScale.y = ossimString(scale_y).toDouble();
      }
      else
      {
         result = false;
      }
   }

   if(fixed)
   {
      theFixedFlag = ossimString(fixed).toBool();
   }

   if(rotation)
   {
      theRotation = ossimString(rotation).toDouble();
   }
   else
   {
      result = false;
   }
   
   return result;
}

bool ossimFontInformation::operator==(const ossimFontInformation& rhs)const
{
   return ( (theFamilyName == rhs.theFamilyName)&&
            (theStyleName  == rhs.theStyleName)&&
            (thePointSize  == rhs.thePointSize)&&
            (theFixedFlag  == rhs.theFixedFlag)&&
            (theScale      == rhs.theScale)&&
            (theRotation   == rhs.theRotation)&&
            (theShear      == rhs.theShear));
}

bool ossimFontInformation::operator !=(const ossimFontInformation& rhs)const
{
   return ( (theFamilyName != rhs.theFamilyName)||
            (theStyleName  != rhs.theStyleName)||
            (thePointSize  != rhs.thePointSize)||
            (theFixedFlag  != rhs.theFixedFlag)||
            (theScale      != rhs.theScale)||
            (theRotation   != rhs.theRotation)||
            (theShear      != rhs.theShear));
}

const ossimFontInformation& ossimFontInformation::operator=(
   const ossimFontInformation& rhs)
{
   theFamilyName  = rhs.theFamilyName;
   theStyleName   = rhs.theStyleName;
   thePointSize   = rhs.thePointSize;
   theFixedFlag   = rhs.theFixedFlag;
   theScale       = rhs.theScale;
   theRotation    = rhs.theRotation;
   theShear       = rhs.theShear;
   
   return *this;
}
