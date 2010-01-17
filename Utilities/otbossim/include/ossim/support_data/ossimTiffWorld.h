//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Ken Melero
// 
// Description: Container class for a tiff world file data.
//
//********************************************************************
// $Id: ossimTiffWorld.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimTiffWorld_HEADER
#define ossimTiffWorld_HEADER

#include <iostream>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDpt.h>
using namespace std;

//***************************************************************************
// CLASS:  ossimTiffWorld
//***************************************************************************
class OSSIMDLLEXPORT ossimTiffWorld
{
public:
   ossimTiffWorld();
   ossimTiffWorld(const char* source, 
		  ossimPixelType ptype = OSSIM_PIXEL_IS_POINT,
		  ossimUnitType  unit = OSSIM_METERS);
   
   ~ossimTiffWorld();

  bool saveToOssimGeom(ossimKeywordlist& kwl, const char* prefix=NULL)const;
  bool loadFromOssimGeom(const ossimKeywordlist& kwl, const char* prefix=NULL);

  std::ostream& print(std::ostream& out) const;

  friend OSSIMDLLEXPORT std::ostream& operator<<(std::ostream& out,
                                                 const ossimTiffWorld& obj);

   /*!
    * transformed.x =
    * imagePoint.x*theX_scale + imagePoint.y*the3rdValue + theTranslateX
    * 
    * transformed.y =
    * imagePoint.x*the2ndValue + imagePoint.y*theY_scale + theTranslateY
    */
   void forward(const ossimDpt& imagePoint,
                ossimDpt& transformedPoint);
   
   double theXScale;     // xscale
   double the2ndValue;   // rotation term
   double the3rdValue;   // rotaion term
   double theYScale;     // yscale
   double theTranslateX; // 
   double theTranslateY; //

protected:
   ossimPixelType thePixelType;
   ossimUnitType  theUnit;
};

#endif
