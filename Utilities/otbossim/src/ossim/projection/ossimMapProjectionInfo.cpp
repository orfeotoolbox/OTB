//*******************************************************************
// Copyright (C) 2001 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
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
//  $Id: ossimMapProjectionInfo.cpp 13025 2008-06-13 17:06:30Z sbortman $

#include <cstdlib>
#include <fstream>
#include <time.h>

using namespace std;

#include <ossim/projection/ossimMapProjectionInfo.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimUnitTypeLut.h>

//***
// Static trace for debugging.
//***
static ossimTrace traceDebug("ossimMapProjectionInfo::debug");

//***
// Keywords for getStateFrom/saveStateTo.
//***
const char* ossimMapProjectionInfo::OUTPUT_US_FT_INFO_KW =
"viewinfo.output_readme_in_us_ft_flag";

const char* ossimMapProjectionInfo::PIXEL_TYPE_KW = "viewinfo.pixel_type";

const char* ossimMapProjectionInfo::README_IMAGE_STRING_KW =
"viewinfo.readme_image_string";

ossimMapProjectionInfo::ossimMapProjectionInfo(const ossimMapProjection* proj,
                                               const ossimDrect& output_rect)
   :
      theProjection               (proj),
      theErrorStatus              (false),
      theLinesPerImage            (0),
      thePixelsPerLine            (0),
      theCornerGroundPt           (),
      theCornerEastingNorthingPt  (),
      theCenterGroundPt           (),
      theCenterEastingNorthingPt  (0.0, 0.0),
      thePixelType                (OSSIM_PIXEL_IS_POINT),
      theOutputInfoInFeetFlag     (false),
      theImageInfoString          ()
{
   if (!theProjection)
   {
      theErrorStatus = true;
      ossimNotify(ossimNotifyLevel_FATAL)
         << "FATAL ossimMapProjectionInfo::ossimMapProjectionInfo: "
         << "Null projection pointer passed to constructor!"
         << "\nError status has been set.  Returning..."
         << std::endl;
      return;
   }
   
   initializeMembers(output_rect);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimMapProjectionInfo::ossimMapProjectionInfo:\n"
         << "output_rect:  " << output_rect << "\n"
         << *this << std::endl;
   }
}

ossimMapProjectionInfo::~ossimMapProjectionInfo()
{
}

bool ossimMapProjectionInfo::errorStatus() const
{
   return theErrorStatus;
}

void ossimMapProjectionInfo::initializeMembers(const ossimDrect& rect)
{
   theBoundingRect = rect;
   theLinesPerImage  = ossim::round<int>(rect.height());
   thePixelsPerLine  = ossim::round<int>(rect.width());

   theProjection->lineSampleToWorld(rect.ul(),
                                    theCornerGroundPt[0]);
   
   theProjection->lineSampleToWorld(rect.ur(),
                                    theCornerGroundPt[1]);
   
   theProjection->lineSampleToWorld(rect.lr(), 
                                    theCornerGroundPt[2]);
   
   
   theProjection->lineSampleToWorld(rect.ll(), 
                                    theCornerGroundPt[3]);

   theProjection->lineSampleToEastingNorthing(rect.ul(),
                                              theCornerEastingNorthingPt[0]);

   theProjection->lineSampleToEastingNorthing(rect.ur(), 
                                              theCornerEastingNorthingPt[1]);
   
   theProjection->lineSampleToEastingNorthing(rect.lr(),
                                              theCornerEastingNorthingPt[2]);
   
   theProjection->lineSampleToEastingNorthing(rect.ll(), 
                                              theCornerEastingNorthingPt[3]);
   
   theCenterEastingNorthingPt.x = (theCornerEastingNorthingPt[0].x +
                                   theCornerEastingNorthingPt[1].x +
                                   theCornerEastingNorthingPt[2].x +
                                   theCornerEastingNorthingPt[3].x) / 4.0;
   
   theCenterEastingNorthingPt.y = (theCornerEastingNorthingPt[0].y +
                                   theCornerEastingNorthingPt[1].y +
                                   theCornerEastingNorthingPt[2].y +
                                   theCornerEastingNorthingPt[3].y) / 4.0;
   
   theCenterGroundPt = theProjection->inverse(theCenterEastingNorthingPt);
}


void ossimMapProjectionInfo::getGeom(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   theProjection->saveState(kwl, prefix);

   if(theProjection->isGeographic())
   {
      ossimGpt gpt = ulGroundPt();
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_XY_KW,
              ossimDpt(gpt).toString().c_str(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_UNITS_KW,
              ossimUnitTypeLut::instance()->getEntryString(OSSIM_DEGREES),
              true);
   }
   else
   {
      ossimDpt dpt = ulEastingNorthingPt();
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_XY_KW,
              dpt.toString().c_str(),
              true);
      kwl.add(prefix,
              ossimKeywordNames::TIE_POINT_UNITS_KW,
              ossimUnitTypeLut::instance()->getEntryString(OSSIM_METERS),
              true);
   }
}

std::ostream& ossimMapProjectionInfo::print(std::ostream& os) const
{
   if (!os)
   {
      return os;
   }

   os << setiosflags(ios::left)
      << setiosflags(ios::fixed)
      << "ossimMapProjectionInfo Data Members:\n"
      << "Projection name:  " << theProjection->getProjectionName()
      << setw(30) << "\nOutput pixel type:"
      << ((getPixelType() == OSSIM_PIXEL_IS_POINT) ? "pixel is point" :
          "pixel is area")
      << setw(30) << "\nMeters per pixel:"
      << getMetersPerPixel()
      << setw(30) << "\nUS survey feet per pixel:"
      << getUsSurveyFeetPerPixel()
      << setw(30) << "\nDecimal degrees per pixel:"
      << getDecimalDegreesPerPixel()
      << setw(30) << "\nNumber of lines:"
      << theLinesPerImage
      << setw(30) << "\nNumber of pixels:"
      << thePixelsPerLine
      << setw(30) << "\nUpper left ground point:"
      << theCornerGroundPt[0]
      << setw(30) << "\nUpper right ground point:"
      << theCornerGroundPt[1]
      << setw(30) << "\nLower right ground point:"
      << theCornerGroundPt[2]
      << setw(30) << "\nLower left ground point:"
      << theCornerGroundPt[3]
      << setw(30) << "\nUpper left easting_northing:"
      << theCornerEastingNorthingPt[0]
      << setw(30) << "\nUpper right easting_northing:"
      << theCornerEastingNorthingPt[1]
      << setw(30) << "\nLower right easting_northing:"
      << theCornerEastingNorthingPt[2]
      << setw(30) << "\nLower left easting_northing:"
      << theCornerEastingNorthingPt[3]
      << setw(30) << "\nCenter ground point:"
      << theCenterGroundPt
      << setw(30) << "\nCenter easting_northing:"
      << theCenterEastingNorthingPt
      << "\nMap Projection dump:";
   theProjection->print(os);

   return os;
}

void ossimMapProjectionInfo::setPixelType (ossimPixelType type)
{
   thePixelType = type;
}

ossimPixelType ossimMapProjectionInfo::getPixelType () const
{
   return thePixelType;
}

void ossimMapProjectionInfo::setOutputFeetFlag (bool flag)
{
   theOutputInfoInFeetFlag = flag;
}

bool ossimMapProjectionInfo::unitsInFeet() const
{
   return theOutputInfoInFeetFlag;
}

ossimString ossimMapProjectionInfo::getImageInfoString () const
{
   return theImageInfoString;
}

void ossimMapProjectionInfo::setImageInfoString (const ossimString& string)
{
   theImageInfoString = string;
}

const ossimMapProjection* ossimMapProjectionInfo::getProjection() const
{
   return theProjection;
}

ossimDpt ossimMapProjectionInfo::ulEastingNorthingPt( ) const
{
   if (getPixelType() == OSSIM_PIXEL_IS_AREA)
   {
      ossimDpt mpp = getMetersPerPixel();
      ossimDpt pt;
      pt.x = theCornerEastingNorthingPt[0].x - (mpp.x / 2.0);
      pt.y = theCornerEastingNorthingPt[0].y + (mpp.y / 2.0);
      return pt;
   }
   else
   {
      return theCornerEastingNorthingPt[0];
   }
}

ossimDpt ossimMapProjectionInfo::urEastingNorthingPt( ) const
{
   if (getPixelType() == OSSIM_PIXEL_IS_AREA)
   {
      ossimDpt mpp = getMetersPerPixel();
      ossimDpt pt;
      pt.x = theCornerEastingNorthingPt[1].x + (mpp.x / 2.0);
      pt.y = theCornerEastingNorthingPt[1].y + (mpp.y / 2.0);
      return pt;      
   }
   else
   {
      return theCornerEastingNorthingPt[1];
   }
}

ossimDpt ossimMapProjectionInfo::lrEastingNorthingPt( ) const
{
   if (getPixelType() == OSSIM_PIXEL_IS_AREA)
   {
      ossimDpt mpp = getMetersPerPixel();
      ossimDpt pt;
      pt.x = theCornerEastingNorthingPt[2].x + (mpp.x / 2.0);
      pt.y = theCornerEastingNorthingPt[2].y - (mpp.y / 2.0);
      return pt;      
   }
   else
   {
      return theCornerEastingNorthingPt[2];
   }
}

ossimDpt ossimMapProjectionInfo::llEastingNorthingPt( ) const
{
   if (getPixelType() == OSSIM_PIXEL_IS_AREA)
   {
      ossimDpt mpp = getMetersPerPixel();
      ossimDpt pt;
      pt.x = theCornerEastingNorthingPt[3].x - (mpp.x / 2.0);
      pt.y = theCornerEastingNorthingPt[3].y - (mpp.y / 2.0);
      return pt;      
   }
   else
   {
      return theCornerEastingNorthingPt[3];
   }
}

ossimGpt ossimMapProjectionInfo::ulGroundPt( ) const
{
   if (getPixelType() == OSSIM_PIXEL_IS_AREA)
   {
      ossimDpt ddpp = getDecimalDegreesPerPixel();
      ossimGpt gpt;
      gpt.latd(theCornerGroundPt[0].latd() + (ddpp.y / 2.0));
      gpt.lond(theCornerGroundPt[0].lond() - (ddpp.x / 2.0));
      return gpt;
   }
   else
   {
      return theCornerGroundPt[0];
   } 
}

ossimGpt ossimMapProjectionInfo::urGroundPt( ) const
{
   if (getPixelType() == OSSIM_PIXEL_IS_AREA)
   {
      ossimDpt ddpp = getDecimalDegreesPerPixel();
      ossimGpt gpt;
      gpt.latd(theCornerGroundPt[1].latd() + (ddpp.y / 2.0));
      gpt.lond(theCornerGroundPt[1].lond() + (ddpp.x / 2.0));
      return gpt;
   }
   else
   {
      return theCornerGroundPt[1];
   }
}

ossimGpt ossimMapProjectionInfo::lrGroundPt( ) const
{
   if (getPixelType() == OSSIM_PIXEL_IS_AREA)
   {
      ossimDpt ddpp = getDecimalDegreesPerPixel();
      ossimGpt gpt;
      gpt.latd(theCornerGroundPt[2].latd() - (ddpp.y / 2.0));
      gpt.lond(theCornerGroundPt[2].lond() + (ddpp.x / 2.0));
      return gpt;
   }
   else
   {
      return theCornerGroundPt[2];
   }
}

ossimGpt ossimMapProjectionInfo::llGroundPt( ) const
{
   if (getPixelType() == OSSIM_PIXEL_IS_AREA)
   {
      ossimDpt ddpp = getDecimalDegreesPerPixel();
      ossimGpt gpt;
      gpt.latd(theCornerGroundPt[3].latd() - (ddpp.y / 2.0));
      gpt.lond(theCornerGroundPt[3].lond() - (ddpp.x / 2.0));
      return gpt;
   }
   else
   {
      return theCornerGroundPt[3];
   }   
}

ossimGpt ossimMapProjectionInfo::centerGroundPt( ) const
{
   // Center is simply center, no shift for pixel is area...
   return theCenterGroundPt;
}

ossimDpt ossimMapProjectionInfo::centerEastingNorthingPt() const
{
   return theCenterEastingNorthingPt;
}

ossim_int32 ossimMapProjectionInfo::linesPerImage() const
{
   return theLinesPerImage;
}

ossim_int32 ossimMapProjectionInfo::pixelsPerLine() const
{
   return thePixelsPerLine;
}

bool ossimMapProjectionInfo::loadState(const ossimKeywordlist& kwl,
                                       const char* )
{
   const char* lookupReturn = kwl.find(README_IMAGE_STRING_KW);

   //***
   // "theImageInfoString" goes in the README file "Image:" field and can
   // be used to identify the image.
   //***
   if (lookupReturn) theImageInfoString = lookupReturn;
   
   lookupReturn = kwl.find(ossimKeywordNames::PIXEL_TYPE_KW);

   if (lookupReturn)
   {
      ossimString tmp = lookupReturn;

      tmp.downcase();

      if (tmp.contains("area"))
      {
         thePixelType = OSSIM_PIXEL_IS_AREA;
      }
      else
      {
         thePixelType = OSSIM_PIXEL_IS_POINT;
      }
   }

   lookupReturn = kwl.find(OUTPUT_US_FT_INFO_KW);

   if (lookupReturn)
   {
      int tmp = atoi(lookupReturn);

      if (tmp)
      {
         theOutputInfoInFeetFlag = true;
      }
      else
      {
         theOutputInfoInFeetFlag = false;
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimMapProjectionInfo::loadState:"
         << "\ntheImageInfoString:       " << theImageInfoString
         << "\nthePixelType:             " << int(thePixelType)
         << "\ntheOutputInfoInFeetFlag:  " << theOutputInfoInFeetFlag
         << endl;
   }

   return true;
}

bool ossimMapProjectionInfo::saveState(ossimKeywordlist& kwl,
                                       const char* ) const 
{
   kwl.add(README_IMAGE_STRING_KW,
           theImageInfoString.chars());
   
   ossimString tmp;
   
   if (thePixelType == OSSIM_PIXEL_IS_POINT)
   {
      tmp = "point";
   }
   else
   {
      tmp = "area";
   }
   
   kwl.add(PIXEL_TYPE_KW,
           tmp.chars());

   kwl.add(OUTPUT_US_FT_INFO_KW,
           int(theOutputInfoInFeetFlag));
   
   return true;
}

ossimDpt ossimMapProjectionInfo::ulEastingNorthingPtInFt() const
{
   ossimDpt pt = ulEastingNorthingPt();
   
   pt.x = ossim::mtrs2usft(pt.x);
   
   pt.y = ossim::mtrs2usft(pt.y);
   
   return pt;
}

ossimDpt ossimMapProjectionInfo::urEastingNorthingPtInFt() const
{
   ossimDpt pt = urEastingNorthingPt();
   pt.x = ossim::mtrs2usft(pt.x);
   pt.y = ossim::mtrs2usft(pt.y);
   return pt;
}

ossimDpt ossimMapProjectionInfo::lrEastingNorthingPtInFt() const
{
   ossimDpt pt = lrEastingNorthingPt();
   pt.x = ossim::mtrs2usft(pt.x);
   pt.y = ossim::mtrs2usft(pt.y);
   return pt;
}

ossimDpt ossimMapProjectionInfo::llEastingNorthingPtInFt() const
{
   ossimDpt pt = llEastingNorthingPt();
   pt.x = ossim::mtrs2usft(pt.x);
   pt.y = ossim::mtrs2usft(pt.y);
   return pt;
}

ossimDpt ossimMapProjectionInfo::getMetersPerPixel() const
{
   return theProjection->getMetersPerPixel();
}

ossimDpt ossimMapProjectionInfo::getUsSurveyFeetPerPixel() const
{
   ossimDpt pt = getMetersPerPixel();
   
   pt.x = ossim::mtrs2usft(pt.x);
   pt.y = ossim::mtrs2usft(pt.y);

   return pt;
}

ossimDpt ossimMapProjectionInfo::getDecimalDegreesPerPixel() const
{
   return theProjection->getDecimalDegreesPerPixel();
}
