//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// Class for encapsulate projection info given a projection, datum, and
// output rectangle.
//
// NOTE:
// - Output rectangle should be relative to the center of pixels.
//   Shifts will be made for "pixel is area" internally.
//
//*******************************************************************
//  $Id: ossimMapProjectionInfo.h 11959 2007-10-31 19:22:56Z gpotts $

#ifndef ossimMapProjectionInfo_HEADER
#define ossimMapProjectionInfo_HEADER

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimString.h>

class ossimMapProjection;
class ossimImageChain;

class OSSIMDLLEXPORT ossimMapProjectionInfo : public ossimObject
{
public:

   ossimMapProjectionInfo(const ossimMapProjection* proj,
                          const ossimDrect& output_rect);

   virtual ~ossimMapProjectionInfo();

   /**
    *  Returns true on error false if ok.
    */
   bool errorStatus() const;

   /**
    * Adds tie point to keyword list.  This will be latitude, longitude if
    * projection is geographic; else, easting, northing.
    */
   void getGeom(ossimKeywordlist& kwl, const char* prefix=0)const;

   /**
    *  Returns the pixel size in meters.
    *  x = sample direction
    *  y = line direction
    */
   ossimDpt getMetersPerPixel() const;
   
   /**
    *  Returns the pixel size in U.S. Survey feet.
    *  x = sample direction
    *  y = line direction
    *
    *  NOTE:
    *  The US survey feet per meter converion (0.3048006096 feet per meter)
    *  is NOT the same as the standard feet per meter conversion
    *  (0.3048 feet per meter).
    */
   ossimDpt getUsSurveyFeetPerPixel() const;

   /**
    *  Returns the decimal degrees per pixel.
    *  x = sample direction
    *  y = line direction
    */
   ossimDpt getDecimalDegreesPerPixel() const;
   
   /**
    *  Returns the upper left easting and northing as a ossimDpt.
    *  The easting is "u" and the northing is "v". Units are in meters.
    */
   ossimDpt ulEastingNorthingPt() const;

   /**
    *  Returns the upper right easting and northing as a ossimDpt.
    *  The easting is "u" and the northing is "v". Units are in meters.
    */
   ossimDpt urEastingNorthingPt() const;

   /**
    *  Returns the lower right easting and northing as a ossimDpt.
    *  The easting is "u" and the northing is "v". Units are in meters.
    */
   ossimDpt lrEastingNorthingPt() const;

   /**
    *  Returns the lower left easting and northing as a ossimDpt.
    *  The easting is "u" and the northing is "v". Units are in meters.
    */
   ossimDpt llEastingNorthingPt() const;
   
   /**
    *  Returns the upper left easting and northing as a ossimDpt.
    *  The easting is "u" and the northing is "v".
    *  Units are in U.S. Survey feet.
    */
   ossimDpt ulEastingNorthingPtInFt() const;

   /**
    *  Returns the upper right easting and northing as a ossimDpt.
    *  The easting is "u" and the northing is "v".
    *  Units are in U.S. Survey feet.
    */
   ossimDpt urEastingNorthingPtInFt() const;

   /**
    *  Returns the lower right easting and northing as a ossimDpt.
    *  The easting is "u" and the northing is "v".
    *  Units are in U.S. Survey feet.
    */
   ossimDpt lrEastingNorthingPtInFt() const;

   /**
    *  Returns the lower left easting and northing as a ossimDpt.
    *  The easting is "u" and the northing is "v". 
    *  Units are in U.S. Survey feet.
    */
   ossimDpt llEastingNorthingPtInFt() const;
   
   /**
    * Returns the upper left ground point.
    */
   ossimGpt ulGroundPt() const;

   /**
    * Returns the upper right ground point.
    */
   ossimGpt urGroundPt() const;

   /**
    * Returns the lower right ground point.
    */
   ossimGpt lrGroundPt() const;
 
   /**
    * Returns the lower left ground point.
    */
   ossimGpt llGroundPt() const;

   /**
    * Returns the center ground point.
    */
   ossimGpt centerGroundPt() const;

   /** @return The center ground point. */
   ossimDpt centerEastingNorthingPt() const;

   /** @return The lines per image. */
   ossim_int32 linesPerImage() const;

   /** @return The pixels per line (samples). */
   ossim_int32 pixelsPerLine() const;

   /**
    * Formatted print of data members.  Used by operator <<.
    */
   std::ostream& print(std::ostream& os) const;

   /**
    *  Sets the data member "thePixelType".
    *  NOTE:
    *  PIXEL_IS_POINT (ennumerated to a "0"):
    *  Corner coordinates are relative to the center of pixel.
    *  PIXEL_IS_AREA (ennumerated to a "1"):
    *  Corner coordinates are relative to the outer edge of the pixel.
    */
   void setPixelType (ossimPixelType type);

   /**
    *  Returns data member "thePixelType".
    *  NOTE:
    *  OSSIM_PIXEL_IS_POINT (ennumerated to a "0"):
    *  Corner coordinates are relative to the center of pixel.
    *  OSSIM_PIXEL_IS_AREA (ennumerated to a "1"):
    *  Corner coordinates are relative to the outer of the pixel.
    */
   ossimPixelType  getPixelType () const;

   /**
    * Sets the data member "theOutputInfoInFeetFlag".
    */
   void setOutputFeetFlag (bool flag);

   /**
    *  Returns the data member "theOutputInfoInFeetFlag".
    *  True = feet, false = meters.
    */
   bool unitsInFeet() const;

   /**
    * Returns theImageInfoString.
    */
   ossimString getImageInfoString () const;

   /**
    *  Used to set data member "theImageInfoString".  This will be output
    *  in the "Image: " field of the README file.
    */
   void setImageInfoString (const ossimString& string);

   /**
    * Returns reference to "theProjection".
    */
   const ossimMapProjection* getProjection() const;

   //****
   // Keywords for getStateFrom/saveStateTo.
   //***
   static const char* README_IMAGE_STRING_KW;
   static const char* PIXEL_TYPE_KW;
   static const char* OUTPUT_US_FT_INFO_KW;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = 0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = 0) const;
   
private:

   // Do not permit construction with void args.
   ossimMapProjectionInfo();
   ossimMapProjectionInfo& operator=(const ossimMapProjectionInfo&);
   
   void initializeMembers(const ossimDrect& output_rect);
   
   const ossimMapProjection* theProjection;
   mutable bool              theErrorStatus;
   int                       theLinesPerImage;
   int                       thePixelsPerLine;
   ossimGpt                  theCornerGroundPt[4];
   ossimDpt                  theCornerEastingNorthingPt[4];
   ossimGpt                  theCenterGroundPt;
   ossimDpt                  theCenterEastingNorthingPt;
   ossimDrect                theBoundingRect;
   
   /**
    *   "thePixelType" defines whether the tie point coordinates are
    *   relative to the center of the pixel(which is PIXEL_IS_POINT) or
    *   the upper left corner of the pixel(PIXEL_IS_AREA).  The default
    *   pixle type is PIXEL_IS_POINT.
    */
   ossimPixelType                 thePixelType;

   /**
    *   If "theOutputInfoInFeetFlag" is true the readme file tie point
    *   info will be output in both meters and U.S. feet.
    *   NOTE:
    *   U.S. feet conversion is "meters/0.3048006096" not the standard
    *   "meters/0.3048".
    *   Reference for U.S. feet value:
    *   U.S.G.S. "Map Projections - A Working Manual" USGS Professional
    *   paper 1395 page 51.
    */
   bool                      theOutputInfoInFeetFlag;

   /**
    *   "theImageInfoString" goes in the README file "Image:" field
    *   to identify the image.
    */
   ossimString               theImageInfoString;
};

#endif /* End of #ifndef ossimMapProjectionInfo_HEADER */
