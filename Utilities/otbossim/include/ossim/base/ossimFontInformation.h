//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  See top level LICENSE.txt.
//
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimFontInformation.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimFontInformation_HEADER
#define ossimFontInformation_HEADER

#include <iosfwd>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimIpt.h>

class ossimKeywordlist;

class OSSIMDLLEXPORT ossimFontInformation
{
public:
   friend std::ostream& operator << (std::ostream& out,
                                     const ossimFontInformation& rhs);

   ossimFontInformation();

   ossimFontInformation(const ossimString& family,
                        const ossimString& style,
                        const ossimIpt&    pointSize,
                        bool               fixedFlag,
                        const ossimDpt&    scale=ossimDpt(1.0,1.0),
                        double             rotation = 0.0,
                        const ossimDpt&    shear=ossimDpt(0.0, 0.0));

   ossimFontInformation(const ossimFontInformation& rhs);

   bool isFixed()const;

   /**
    * Saves the current state of this object.
    *
    * For keywords see loadState:
    */
   bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

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

   bool operator ==(const ossimFontInformation& rhs)const;

   bool operator !=(const ossimFontInformation& rhs)const;

   const ossimFontInformation& operator =(const ossimFontInformation& rhs);
   
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
