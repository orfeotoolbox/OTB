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
// $Id: ossimTiffWorld.h 18693 2011-01-17 18:49:15Z dburken $
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
   
   bool open(const ossimFilename& file,
             ossimPixelType ptype,
             ossimUnitType unit);
   
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
   
   //! Converts world file parameters into x, y scale (for use in affine transform) 
   const ossimDpt& getScale() const { return theComputedScale; }
   
   //! Converts world file parameters into RH rotation in radians (for use in affine transform) 
   double getRotation() const { return theComputedRotation; }
   
   //! Provides access to the translation (for use in affine transform) 
   const ossimDpt& getTranslation() const { return theTranslation; }

protected:
   double theXform1;     
   double theXform2;   
   double theXform3;   
   double theXform4;   
   ossimDpt theTranslation;

   ossimPixelType thePixelType;
   ossimUnitType  theUnit;
   ossimDpt theComputedScale;
   double   theComputedRotation;  //!< Radians
};

#endif
