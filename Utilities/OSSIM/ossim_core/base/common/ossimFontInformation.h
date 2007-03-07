//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt.
//
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimFontInformation.h,v 1.4 2005/12/16 14:27:02 dburken Exp $
#ifndef ossimFontInformation_HEADER
#define ossimFontInformation_HEADER
#include <iostream>
#include <base/data_types/ossimString.h>
#include <base/data_types/ossimIpt.h>

class ossimKeywordlist;

class OSSIMDLLEXPORT ossimFontInformation
{
public:
   friend std::ostream& operator << (std::ostream& out,
                                     const ossimFontInformation& rhs)
      {
         out << "Family name:  " << rhs.theFamilyName
             << "\nStyle name:   " << rhs.theStyleName
             << "\nPoint size:   " << rhs.thePointSize
             << "\nFixed flag:   " << (rhs.theFixedFlag?"true":"false")
             << std::endl;

         return out;
      }
   ossimFontInformation()
      :theFamilyName(""),
       theStyleName(""),
       thePointSize(0,0),
       theFixedFlag(false),
       theScale(1.0,1.0),
       theRotation(0.0),
       theShear(0.0,0.0)
      {
      }
   ossimFontInformation(const ossimString& family,
                        const ossimString& style,
                        const ossimIpt&    pointSize,
                        bool               fixedFlag,
                        const ossimDpt&    scale=ossimDpt(1.0,1.0),
                        double             rotation = 0.0,
                        const ossimDpt&    shear=ossimDpt(0.0, 0.0))
      :theFamilyName(family),
       theStyleName(style),
       thePointSize(pointSize),
       theFixedFlag(fixedFlag),
       theScale(scale),
       theRotation(rotation),
       theShear(shear)
      {}
   ossimFontInformation(const ossimFontInformation& rhs)
      : theFamilyName(rhs.theFamilyName),
        theStyleName(rhs.theStyleName),
        thePointSize(rhs.thePointSize),
        theFixedFlag(rhs.theFixedFlag),
        theScale(rhs.theScale),
        theRotation(rhs.theRotation),
        theShear(rhs.theShear)
      {
      }
   bool isFixed()const
      {
         return theFixedFlag;
      }

   /**
    * Saves the current state of this object.
    *
    * For keywords see loadState:
    */
   bool saveState(ossimKeywordlist& kwl,
                  const char* prefix=0)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    *
    * Keywords:
    *
    * family_name:
    * style_name:
    * fixed_flag:
    * point_size: ( x, y )
    * scale: ( x, y )
    * shear: ( x, y )
    * rotation:
    *
    * point_size_x:  (deprecated)
    * point_size_y:  (deprecated)
    * scale_x: (deprecated)
    * scale_y: (deprecated)
    * shear_x: (deprecated)
    * shear_y: (deprecated)
    */   
   bool loadState(const ossimKeywordlist& kwl,
                  const char* prefix=0);

   bool operator ==(const ossimFontInformation& rhs)const
      {
         return ( (theFamilyName == rhs.theFamilyName)&&
                  (theStyleName  == rhs.theStyleName)&&
                  (thePointSize  == rhs.thePointSize)&&
                  (theFixedFlag  == rhs.theFixedFlag)&&
                  (theScale      == rhs.theScale)&&
                  (theRotation   == rhs.theRotation)&&
                  (theShear      == rhs.theShear));
      }

   bool operator !=(const ossimFontInformation& rhs)const
      {
         return ( (theFamilyName != rhs.theFamilyName)||
                  (theStyleName  != rhs.theStyleName)||
                  (thePointSize  != rhs.thePointSize)||
                  (theFixedFlag  != rhs.theFixedFlag)||
                  (theScale      != rhs.theScale)||
                  (theRotation   != rhs.theRotation)||
                  (theShear      != rhs.theShear));
      }

   const ossimFontInformation& operator =(const ossimFontInformation& rhs)
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
   
   static const char* FAMILY_NAME_KW;
   static const char* STYLE_NAME_KW;
   static const char* POINT_SIZE_KW;   // point_size: ( x, y ) 
   static const char* POINT_SIZE_X_KW; // deprecated
   static const char* POINT_SIZE_Y_KW; // deprecated
   static const char* FIXED_FLAG_KW;
   static const char* SHEAR_KW;        // shear: ( x, y ) 
   static const char* SHEAR_X_KW;      // deprecated
   static const char* SHEAR_Y_KW;      // deprecated
   static const char* SCALE_KW;        // scale: ( x, y ) 
   static const char* SCALE_X_KW;      // deprecated
   static const char* SCALE_Y_KW;      // deprecated
   static const char* ROTATION_KW;
   
   
   ossimString    theFamilyName;
   ossimString    theStyleName;
   ossimIpt       thePointSize;
   bool           theFixedFlag;
   ossimDpt       theScale;
   double         theRotation;
   ossimDpt       theShear;
};

#endif
