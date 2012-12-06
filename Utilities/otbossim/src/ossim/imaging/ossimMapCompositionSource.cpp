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
// $Id: ossimMapCompositionSource.cpp 21631 2012-09-06 18:10:55Z dburken $

#include <ossim/imaging/ossimMapCompositionSource.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/imaging/ossimImageDataHelper.h>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/imaging/ossimAnnotationFontObject.h>
#include <ossim/imaging/ossimAnnotationLineObject.h>
#include <ossim/imaging/ossimImageGeometry.h>
#include <ossim/font/ossimFontFactoryRegistry.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimColorProperty.h>
#include <ossim/base/ossimContainerProperty.h>
#include <ossim/base/ossimFontProperty.h>
#include <ossim/base/ossimBooleanProperty.h>
#include <ossim/base/ossimTextProperty.h>
#include <ossim/base/ossimNumericProperty.h>
#include <ossim/base/ossimStringProperty.h>
#include <sstream>

using namespace std;

static const char* GRID_TYPE_ENUM_NAMES[]     = {"none", "line", "reseaux"};
static const char* VIEW_WIDTH_KW              = "view_width";
static const char* VIEW_HEIGHT_KW             = "view_height";
static const char* METER_GRID_SPACING_X_KW    = "meter_grid_spacing_x";
static const char* METER_GRID_SPACING_Y_KW    = "meter_grid_spacing_y";
static const char* GEO_GRID_SPACING_LON_KW    = "geo_grid_spacing_lon";
static const char* GEO_GRID_SPACING_LAT_KW    = "geo_grid_spacing_lat";
static const char* METER_GRID_TYPE_KW         = "meter_grid_type";
static const char* GEO_GRID_TYPE_KW           = "geo_grid_type";
static const char* TOP_BORDER_LENGTH_KW       = "top_border_length";
static const char* BOTTOM_BORDER_LENGTH_KW    = "bottom_border_length";
static const char* LEFT_BORDER_LENGTH_KW      = "top_border_length";
static const char* RIGHT_BORDER_LENGTH_KW     = "right_border_length";
static const char* BORDER_COLOR_KW            = "border_color";
static const char* GEO_GRID_COLOR_KW          = "geo_grid_color";
static const char* METER_GRID_COLOR_KW        = "meter_grid_color";
static const char* TITLE_STRING_KW            = "title_string";
static const char* TITLE_COLOR_KW             = "title_color";

static const char* TOP_GEO_LABEL_COLOR_KW     = "top_geo_label_color";
static const char* BOTTOM_GEO_LABEL_COLOR_KW  = "bottom_geo_label_color";
static const char* LEFT_GEO_LABEL_COLOR_KW    = "left_geo_label_color";
static const char* RIGHT_GEO_LABEL_COLOR_KW   = "right_geo_label_color";

static const char* TOP_METER_LABEL_COLOR_KW     = "top_meter_label_color";
static const char* BOTTOM_METER_LABEL_COLOR_KW  = "bottom_meter_label_color";
static const char* LEFT_METER_LABEL_COLOR_KW    = "left_meter_label_color";
static const char* RIGHT_METER_LABEL_COLOR_KW   = "right_meter_label_color";

static const char* TOP_GEO_LABEL_FORMAT_KW    = "top_geo_label_format";
static const char* BOTTOM_GEO_LABEL_FORMAT_KW = "bottom_geo_label_format";
static const char* LEFT_GEO_LABEL_FORMAT_KW   = "left_geo_label_format";
static const char* RIGHT_GEO_LABEL_FORMAT_KW  = "right_geo_label_format";

static const char* TOP_GEO_LABEL_FLAG_KW      = "top_geo_label_flag";
static const char* BOTTOM_GEO_LABEL_FLAG_KW   = "bottom_geo_label_flag";
static const char* LEFT_GEO_LABEL_FLAG_KW     = "left_geo_label_flag";
static const char* RIGHT_GEO_LABEL_FLAG_KW    = "right_geo_label_flag";

static const char* TOP_METER_LABEL_FLAG_KW    = "top_meter_label_flag";
static const char* BOTTOM_METER_LABEL_FLAG_KW = "bottom_meter_label_flag";
static const char* LEFT_METER_LABEL_FLAG_KW   = "left_meter_label_flag";
static const char* RIGHT_METER_LABEL_FLAG_KW  = "right_meter_label_flag";

static const char* TOP_GEO_LABEL_FONT_KW      = "top_geo_label_font";
static const char* BOTTOM_GEO_LABEL_FONT_KW   = "bottom_geo_label_font";
static const char* LEFT_GEO_LABEL_FONT_KW     = "left_geo_label_font";
static const char* RIGHT_GEO_LABEL_FONT_KW    = "right_geo_label_font";
static const char* TOP_METER_LABEL_FONT_KW    = "top_meter_label_font";
static const char* BOTTOM_METER_LABEL_FONT_KW = "bottom_meter_label_font";
static const char* LEFT_METER_LABEL_FONT_KW   = "left_meter_label_font";
static const char* RIGHT_METER_LABEL_FONT_KW  = "right_meter_label_font";
static const char* TITLE_FONT_KW              = "title_font";

static ossimTrace traceDebug("ossimMapCompositionSource:debug");

RTTI_DEF1(ossimMapCompositionSource, "ossimMapCompositionSource", ossimAnnotationSource);
ossimMapCompositionSource::ossimMapCompositionSource()
   :ossimAnnotationSource(),
    theMeterGridType(OSSIM_GRID_NONE),
    theGeographicGridType(OSSIM_GRID_LINE),
    theTopBorderLength(200),
    theBottomBorderLength(200),
    theLeftBorderLength(300),
    theRightBorderLength(300),
    theBorderColor(255,255,255),
    theGeographicGridColor(255,255,255),
    theMeterGridColor(255,255,255),
    theTitleString(""),
    theTitleFont(NULL),
    theTitleColor(1, 1, 1),
    theTopGeographicLabelColor(1, 1, 1),
    theBottomGeographicLabelColor(1, 1, 1),
    theLeftGeographicLabelColor(1, 1, 1),
    theRightGeographicLabelColor(1, 1, 1),
    theTopMeterLabelColor(1, 1, 1),
    theBottomMeterLabelColor(1, 1, 1),
    theLeftMeterLabelColor(1, 1, 1),
    theRightMeterLabelColor(1, 1, 1),
    theGeographicTopLabelFont(NULL),
    theGeographicBottomLabelFont(NULL),
    theGeographicLeftLabelFont(NULL),
    theGeographicRightLabelFont(NULL),
    theMeterTopLabelFont(NULL),
    theMeterBottomLabelFont(NULL),
    theMeterLeftLabelFont(NULL),
    theMeterRightLabelFont(NULL),
    theTopGeographicFormat("dd@mm'ss\".ssssC"),
    theBottomGeographicFormat("dd@mm'ss\".ssssC"),
    theLeftGeographicFormat("dd@mm'ss\".ssssC"),
    theRightGeographicFormat("dd@mm'ss\".ssssC"),
    theTopGeographicLabelFlag(true),
    theBottomGeographicLabelFlag(true),
    theLeftGeographicLabelFlag(true),
    theRightGeographicLabelFlag(true),
    theTopGeographicTickFlag(true),
    theBottomGeographicTickFlag(true),
    theLeftGeographicTickFlag(true),
    theRightGeographicTickFlag(true),
    theTopMeterLabelFlag(false),
    theBottomMeterLabelFlag(false),
    theLeftMeterLabelFlag(false),
    theRightMeterLabelFlag(false),
    theTopMeterTickFlag(false),
    theBottomMeterTickFlag(false),
    theLeftMeterTickFlag(false),
    theRightMeterTickFlag(false),
    theGeographicSpacing(1.0, 1.0),
    theMeterSpacing(3600*30, 3600*30)
{
   theViewWidthHeight = ossimIpt(-1,-1);
   vector<ossimFontInformation> info;
   ossimFontFactoryRegistry::instance()->getFontInformation(info);

   if(info.size())
   {
      theGeographicTopLabelFont = ossimFontFactoryRegistry::instance()->createFont(info[0]);
      theGeographicTopLabelFontInfo = info[0];
      theGeographicTopLabelFontInfo.thePointSize = ossimIpt(12,12);

      if(theGeographicTopLabelFont.valid())
      {
         theGeographicBottomLabelFont     = (ossimFont*)theGeographicTopLabelFont->dup();
         theGeographicBottomLabelFontInfo = theGeographicTopLabelFontInfo;
         theGeographicLeftLabelFont       = (ossimFont*)theGeographicTopLabelFont->dup();
         theGeographicLeftLabelFontInfo   = theGeographicTopLabelFontInfo;
         theGeographicRightLabelFont      = (ossimFont*)theGeographicTopLabelFont->dup();
         theGeographicRightLabelFontInfo  = theGeographicTopLabelFontInfo;

         theMeterTopLabelFont             = (ossimFont*)theGeographicTopLabelFont->dup();
         theMeterTopLabelFontInfo         = theGeographicTopLabelFontInfo;
         theMeterBottomLabelFont          = (ossimFont*)theGeographicTopLabelFont->dup();
         theMeterBottomLabelFontInfo      = theGeographicTopLabelFontInfo;
         theMeterLeftLabelFont            = (ossimFont*)theGeographicTopLabelFont->dup();
         theMeterLeftLabelFontInfo        = theGeographicTopLabelFontInfo;
         theMeterRightLabelFont           = (ossimFont*)theGeographicTopLabelFont->dup();
         theMeterRightLabelFontInfo       = theGeographicTopLabelFontInfo;
         
         theTitleFont                     = (ossimFont*)theGeographicTopLabelFont->dup();
         theTitleFontInfo                 = theGeographicTopLabelFontInfo;
         theTitleFontInfo.thePointSize    = ossimIpt(48, 48);
      }
   }
}

ossimMapCompositionSource::~ossimMapCompositionSource()
{
   theGeographicTopLabelFont = 0;
   theGeographicBottomLabelFont = 0;
   theGeographicLeftLabelFont = 0;
   theGeographicRightLabelFont = 0;
   theMeterTopLabelFont = 0;
   theMeterBottomLabelFont = 0;
   theMeterLeftLabelFont = 0;

   theMeterRightLabelFont = 0;
   theTitleFont = 0;
   deleteFixedAnnotations();
}

ossimRefPtr<ossimImageData> ossimMapCompositionSource::getTile(const ossimIrect& tileRect,
                                                               ossim_uint32 resLevel)
{
   if(!theTile)
   {
      allocate(tileRect);
      if(!theTile)
      {
         if(theInputConnection)
         {
            return theInputConnection->getTile(tileRect, resLevel);
         }
      }
   }
   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();
   
   if(theInputConnection)
   {
      ossimRefPtr<ossimImageData> tile = theInputConnection->getTile(tileRect,
                                                                     resLevel);

      if(tile.valid())
      {
         if(tile->getScalarType() != OSSIM_UCHAR)
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << "ossimMapCompositionSource::getTile\n"
               << "Error input is not uchar" << endl;
            return tile;
         }
         else
         {
            if(tile->getBuf()&&(tile->getDataObjectStatus() != OSSIM_EMPTY))
            {
               int band = 0;
               int outBands = theTile->getNumberOfBands();
               int minBand = std::min((long)theTile->getNumberOfBands(),
                                      (long)tile->getNumberOfBands());
               for(band = 0; band < minBand; ++band)
               {
                  theTile->loadBand(tile->getBuf(band),
                                    tile->getImageRectangle(),
                                    band);
               }
               // copy last band of input tile to all output
               // bands if the input tile did not have enough bands.
               //
               for(;band < outBands;++band)
               {
                  theTile->loadBand(tile->getBuf(minBand-1),
                                    tile->getImageRectangle(),
                                    band);
               }
            }
	    drawBorders();
	    drawAnnotations(theTile);
         }
      }
   }
   theTile->validate();
   return theTile;
}

void ossimMapCompositionSource::setGeographicLabelFormat(const ossimString format)
{
   if( (format == theTopGeographicFormat)&&
       (format == theBottomGeographicFormat)&&
       (format == theLeftGeographicFormat)&&
       (format == theRightGeographicFormat))
   {
      return;
   }
   theTopGeographicFormat    = format;
   theBottomGeographicFormat = format;
   theLeftGeographicFormat   = format;
   theRightGeographicFormat  = format;
}

void ossimMapCompositionSource::setTopGeographicLabelFormat(const ossimString& format)
{
   if(format == theTopGeographicFormat)
   {
      return;
   }
   theTopGeographicFormat = format;
}

void ossimMapCompositionSource::setBottomGeographicLabelFormat(const ossimString& format)
{
   if(format == theBottomGeographicFormat)
   {
      return;
   }
   theBottomGeographicFormat = format;

   // need to reset the fonts
//   layoutAnnotations();
}

void ossimMapCompositionSource::setLeftGeographicLabelFormat(const ossimString& format)
{
   if(format == theLeftGeographicFormat)
   {
      return;
   }
   theLeftGeographicFormat = format;

   // need to reset the fonts
//   layoutAnnotations();
}

void ossimMapCompositionSource::setRightGeographicLabelFormat(const ossimString& format)
{
   if(format == theRightGeographicFormat)
   {
      return;
   }
   theRightGeographicFormat = format;

}

void ossimMapCompositionSource::setTitleFont(const ossimFontInformation& fontInfo)
{
   if(theTitleFontInfo == fontInfo)
   {
      return;
   }
   ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(fontInfo);
   if(font)
   {
 
      theTitleFont     = font;
      theTitleFontInfo = fontInfo;      
   }
}

void ossimMapCompositionSource::setGeographicTopLabelFont(const ossimFontInformation& fontInfo)
{
   const char* MODULE = "ossimMapCompositionSource::setGeographicTopLabelFont";
   
   if(traceDebug())
   {
      CLOG << "Entering...." <<endl;
   }
   if(theGeographicTopLabelFontInfo == fontInfo)
   {
      if(traceDebug())
      {
         CLOG << "Font info the same exiting...." <<endl
              << theGeographicTopLabelFontInfo << endl;
      }
      
      return;
   }
   ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(fontInfo);

   if(font)
   {
      if(traceDebug())
      {
         CLOG << "Previous font info " <<endl
              << theGeographicTopLabelFont.get() << endl;
      }
      theGeographicTopLabelFont = font;
      theGeographicTopLabelFontInfo = fontInfo;

      if(traceDebug())
      {
         CLOG << "New font info " <<endl
              << theGeographicTopLabelFont.get() << endl;
      }      
   }
}

void ossimMapCompositionSource::setGeographicBottomLabelFont(const ossimFontInformation& fontInfo)
{
   if(theGeographicBottomLabelFontInfo == fontInfo)
   {
      return;
   }
   ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(fontInfo);

   if(font)
   {
      theGeographicBottomLabelFont     = font;
      theGeographicBottomLabelFontInfo = fontInfo;

   }
}

void ossimMapCompositionSource::setGeographicLeftLabelFont(const ossimFontInformation& fontInfo)
{
   if(theGeographicRightLabelFontInfo == fontInfo)
   {
      return;
   }
   ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(fontInfo);

   if(font)
   {
      theGeographicLeftLabelFont     = font;
      theGeographicLeftLabelFontInfo = fontInfo;

   }
}

void ossimMapCompositionSource::setGeographicRightLabelFont(const ossimFontInformation& fontInfo)
{
   if(theGeographicRightLabelFontInfo == fontInfo)
   {
      return;
   }
   
   ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(fontInfo);

   if(font)
   {
      theGeographicRightLabelFont = font;
      theGeographicRightLabelFontInfo = fontInfo;
   }
}


void ossimMapCompositionSource::setMeterTopLabelFont(const ossimFontInformation& fontInfo)
{
   if(theMeterTopLabelFontInfo == fontInfo)
   {
      return;
   }
   ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(fontInfo);

   if(font)
   {
      theMeterTopLabelFont     = font;
      theMeterTopLabelFontInfo = fontInfo;
   }
}

void ossimMapCompositionSource::setMeterBottomLabelFont(const ossimFontInformation& fontInfo)
{
   if(theMeterBottomLabelFontInfo == fontInfo)
   {
      return;
   }
   ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(fontInfo);

   if(font)
   {
      theMeterBottomLabelFont     = font;
      theMeterBottomLabelFontInfo = fontInfo;
   }
}

void ossimMapCompositionSource::setMeterLeftLabelFont(const ossimFontInformation& fontInfo)
{
   if(theMeterLeftLabelFontInfo == fontInfo)
   {
      return;
   }
   ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(fontInfo);

   if(font)
   {
      theMeterLeftLabelFont     = font;
      theMeterLeftLabelFontInfo = fontInfo;
   }
}

void ossimMapCompositionSource::setMeterRightLabelFont(const ossimFontInformation& fontInfo)
{
   if(theMeterRightLabelFontInfo == fontInfo)
   {
      return;
   }
   ossimFont* font = ossimFontFactoryRegistry::instance()->createFont(fontInfo);

   if(font)
   {
      theMeterRightLabelFont     = font;
      theMeterRightLabelFontInfo = fontInfo;
   }
}

ossimIrect ossimMapCompositionSource::getBoundingRect(ossim_uint32 resLevel)const
{
   ossimIrect result;

   result.makeNan();
   if(theInputConnection)
   {
      ossimIrect inputRect  = getViewingRect();

      if(!inputRect.hasNans())
      {
         ossimDpt decimation;
         getDecimationFactor(resLevel,
                             decimation);
         int leftBorderLength   = theLeftBorderLength;
         int rightBorderLength  = theRightBorderLength;
         int topBorderLength    = theTopBorderLength;
         int bottomBorderLength = theBottomBorderLength;

         ossimIpt p1(inputRect.ul().x - leftBorderLength,
                     inputRect.ul().y - topBorderLength);
         
         ossimIpt p2(inputRect.ur().x + rightBorderLength,
                     inputRect.ur().y - topBorderLength);
         
         ossimIpt p3(inputRect.lr().x + rightBorderLength,
                     inputRect.lr().y + bottomBorderLength);
         
         ossimIpt p4(inputRect.ll().x - leftBorderLength,
                     inputRect.ll().y + bottomBorderLength);

         result = ossimIrect(p1, p2, p3, p4);
         
         if(!decimation.hasNans())
         {
            result*=decimation;
         }
      }
   }
   return result;
}

void ossimMapCompositionSource::layoutAnnotations()
{
   deleteFixedAnnotations();

   addTitle();
   addGridLabels();
   if(theGeographicGridType == OSSIM_GRID_LINE)
   {
      addGeographicGridLines();
   }
   else if(theGeographicGridType == OSSIM_GRID_RESEAUX)
   {
      addGeographicGridReseaux();
   }
   
   if(theMeterGridType == OSSIM_GRID_LINE)
   {
      addMeterGridLines();
   }
   else if(theMeterGridType == OSSIM_GRID_RESEAUX)
   {
      addMeterGridReseaux();
   }
}


void ossimMapCompositionSource::initialize()
{
   ossimAnnotationSource::initialize();   

   theTile = 0;
   if(theInputConnection)
   {
      computeBorderRects();
      layoutAnnotations();
   }
   else 
   {
      deleteFixedAnnotations();
   }
}

void ossimMapCompositionSource::computeBorderRects()
{
   if(theInputConnection)
   {
      ossimIrect inputRect = getViewingRect();;
      
      theTopBorder = ossimIrect(inputRect.ul().x - theLeftBorderLength,
                                inputRect.ul().y - theTopBorderLength,
                                inputRect.ur().x + theRightBorderLength,
                                inputRect.ur().y);
      
      theBottomBorder = ossimIrect(inputRect.ll().x - theLeftBorderLength,
                                   inputRect.ll().y,
                                   inputRect.lr().x + theRightBorderLength,
                                   inputRect.lr().y + theBottomBorderLength);
      
      theLeftBorder   = ossimIrect(theTopBorder.ul().x,
                                   theTopBorder.ll().y,
                                   inputRect.ll().x,
                                   inputRect.ll().y);
      
      theRightBorder   = ossimIrect(inputRect.ur().x,
                                    inputRect.ur().y,
                                    theBottomBorder.lr().x,
                                    inputRect.lr().y);
   }
   else
   {
      theTopBorder.makeNan();
      theBottomBorder.makeNan();
      theLeftBorder.makeNan();
      theRightBorder.makeNan();
   }
      
}

void ossimMapCompositionSource::drawBorders()
{
   if(theTile.valid())
   {
      ossimIrect top    = theTopBorder;
      ossimIrect bottom = theBottomBorder;
      ossimIrect left   = theLeftBorder;
      ossimIrect right  = theRightBorder;

//       if(resLevel)
//       {
//          ossimDpt decimation;
//          getDecimationFactor(resLevel,
//                              decimation);

//          if(!decimation.hasNans())
//          {
//             top*=decimation;
//             bottom*=decimation;
//             left*=decimation;
//             right*=decimation;            
//          }
//       }
      ossimIrect tileRect = theTile->getImageRectangle();
      
      ossimImageDataHelper helper(theTile.get());
      
      if(top.intersects(tileRect))
      {
         helper.fill(theBorderColor,
                     top.clipToRect(tileRect),false);
      }
      if(bottom.intersects(tileRect))
      {
         helper.fill(theBorderColor,
                     bottom.clipToRect(tileRect),false);
      }
      if(left.intersects(tileRect))
      {
         helper.fill(theBorderColor,
                     left.clipToRect(tileRect),false);
      }
      if(right.intersects(tileRect))
      {
         helper.fill(theBorderColor,
                     right.clipToRect(tileRect),false);
      }
   }
}

void ossimMapCompositionSource::addGridLabels()
{
   addGeographicTopGridLabels();
   addGeographicBottomGridLabels();
   addGeographicLeftGridLabels();
   addGeographicRightGridLabels();

   addMeterGridLabels();
}

void ossimMapCompositionSource::addGeographicTopGridLabels()
{
   const char* MODULE = "ossimMapCompositionSource::addGeographicTopGridLabels";
   if(traceDebug())
   {
      CLOG << "Entering...." << endl;
   }
   if((!theTopGeographicLabelFlag)&&(!theTopGeographicTickFlag))
   {
      if(traceDebug())
      {
         CLOG << "Exiting, flags are false...." << endl;
      }
      return;
   }
   
   if(theInputConnection)
   { 
      const ossimMapProjection* mapProj = inputMapProjection();
      ossimGpt gpt[4];
      ossimDrect rect = getViewingRect();

      if(rect.hasNans()) return;
      mapProj->lineSampleToWorld(rect.ul(), gpt[0]);
      mapProj->lineSampleToWorld(rect.ur(), gpt[1]);
      mapProj->lineSampleToWorld(rect.lr(), gpt[2]);
      mapProj->lineSampleToWorld(rect.ll(), gpt[3]);
      
      ossimDrect grect(gpt[0], gpt[1], gpt[2], gpt[3], OSSIM_RIGHT_HANDED);

      if(grect.hasNans()) return;
      ossimDpt ulLatLon(((int)((grect.ul().x-theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.ul().y+theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);
      
      ossimDpt lrLatLon(((int)((grect.lr().x+theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.lr().y-theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);

      while(ulLatLon.lat > 90)
      {
         ulLatLon.lat -= theGeographicSpacing.lat;
      }
      while(ulLatLon.lon < -180)
      {
         ulLatLon.lon += theGeographicSpacing.lon;
      }
      while(lrLatLon.lat < -90)
      {
         ulLatLon.lat += theGeographicSpacing.lat;
      }
      while(lrLatLon.lon > 180)
      {
         ulLatLon.lon -= theGeographicSpacing.lon;
      }
      
      ossimDrect latLonSpacing(ulLatLon,
                               lrLatLon,
                               OSSIM_RIGHT_HANDED);

      if(traceDebug())
      {
         CLOG << "labeling bounding geo rect = " << latLonSpacing << endl;
      }
      // do the top side labels.
      for(double lon = latLonSpacing.ul().x; lon <= latLonSpacing.lr().x; lon+=theGeographicSpacing.x)
      {
         ossimDpt tipt; // top
         ossimDpt bipt; // bottom
         
         ossimGpt tgpt(latLonSpacing.ul().y,
                       lon,
                       0.0,
                       gpt[0].datum());

         ossimGpt bgpt(latLonSpacing.lr().y,
                       lon,
                       0.0,
                       gpt[0].datum());
         
         mapProj->worldToLineSample(tgpt, tipt);
         mapProj->worldToLineSample(bgpt, bipt);

         
         if((!tipt.hasNans()&& !bipt.hasNans()) && rect.clip(tipt, bipt))
         {
            ossimIpt rounded(tipt);
            if( (rounded.x >= rect.ul().x)&&
                (rounded.x <= rect.lr().x))
            {
               if(theTopGeographicLabelFlag)
               {
                  ossimDms dms(tgpt.lond(), false);
               
                  ossimString dmsString = dms.toString(theTopGeographicFormat.c_str());
                  ossimAnnotationFontObject* lonLabel = 
                     new ossimAnnotationFontObject(ossimIpt(0,0), dmsString);
                  lonLabel->setFont(theGeographicTopLabelFont.get());
                  lonLabel->setGeometryInformation(theGeographicTopLabelFontInfo);
                  lonLabel->computeBoundingRect();
                  ossimDrect boundsD;
                  lonLabel->getBoundingRect(boundsD);
                  
                  ossimIpt center( rounded.x, ossim::round<int>(rect.ul().y-(boundsD.height()/2)));
               
                  lonLabel->setColor(theTopGeographicLabelColor.getR(), theTopGeographicLabelColor.getG(), theTopGeographicLabelColor.getB());
                  lonLabel->setCenterPosition(center);
                  lonLabel->computeBoundingRect();

                  ossimDrect labelRect;
                  lonLabel->getBoundingRect(labelRect);
                  
                  if(rect.intersects(labelRect))
                  {
                     ossimIrect intersection = rect.clipToRect(labelRect);
                     center.y -= ossim::round<int>(rect.ul().y - intersection.lr().y);
                  }

                  center.y -= 24; // this will need to be the tick height later;
                  lonLabel->setCenterPosition(center);
                  lonLabel->computeBoundingRect();
                  
                  addFixedAnnotation(lonLabel);
               }
               if(theTopGeographicTickFlag)
               {
                  ossimAnnotationLineObject* lineLabel = new ossimAnnotationLineObject
                     (ossimIpt(rounded.x,ossim::round<int>(rect.ul().y-24)),
                      ossimIpt(rounded.x, ossim::round<int>(rect.ul().y)));
                  lineLabel->setColor(theTopGeographicLabelColor.getR(), theTopGeographicLabelColor.getG(), theTopGeographicLabelColor.getB());
                  addFixedAnnotation(lineLabel);
               }
            }
         }
      }
   }
   if(traceDebug())
   {
      CLOG << "Exiting...." << endl;
   }
}

void ossimMapCompositionSource::addGeographicBottomGridLabels()
{
   if((!theBottomGeographicLabelFlag)&&(!theBottomGeographicTickFlag))
   {
      return;
   }

   const ossimMapProjection* mapProj = inputMapProjection();
   if(mapProj)
   { 
      ossimGpt gpt[4];
      ossimDrect rect = getViewingRect();

      if(rect.hasNans()) return;
      mapProj->lineSampleToWorld(rect.ul(), gpt[0]);
      mapProj->lineSampleToWorld(rect.ur(), gpt[1]);
      mapProj->lineSampleToWorld(rect.lr(), gpt[2]);
      mapProj->lineSampleToWorld(rect.ll(), gpt[3]);
      
      ossimDrect grect(gpt[0], gpt[1], gpt[2], gpt[3], OSSIM_RIGHT_HANDED);

      if(grect.hasNans()) return;
      ossimDpt ulLatLon(((int)((grect.ul().x-theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.ul().y+theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);
      
      ossimDpt lrLatLon(((int)((grect.lr().x+theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.lr().y-theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);

      while(ulLatLon.lat > 90)
      {
         ulLatLon.lat -= theGeographicSpacing.lat;
      }
      while(ulLatLon.lon < -180)
      {
         ulLatLon.lon += theGeographicSpacing.lon;
      }
      while(lrLatLon.lat < -90)
      {
         ulLatLon.lat += theGeographicSpacing.lat;
      }
      while(lrLatLon.lon > 180)
      {
         ulLatLon.lon -= theGeographicSpacing.lon;
      }
      
      ossimDrect latLonSpacing(ulLatLon,
                               lrLatLon,
                               OSSIM_RIGHT_HANDED);

      // do the top side labels.
      for(double lon = latLonSpacing.ul().x; lon <= latLonSpacing.lr().x; lon+=theGeographicSpacing.x)
      {
         ossimDpt tipt; // top
         ossimDpt bipt; // bottom
         
         ossimGpt tgpt(latLonSpacing.ul().y,
                       lon,
                       0.0,
                       gpt[0].datum());

         ossimGpt bgpt(latLonSpacing.lr().y,
                       lon,
                       0.0,
                       gpt[0].datum());
         
         mapProj->worldToLineSample(tgpt, tipt);
         mapProj->worldToLineSample(bgpt, bipt);

         if(rect.clip(tipt, bipt))
         {
            ossimIpt rounded(bipt);
            if( (rounded.x >= rect.ul().x)&&
                (rounded.x <= rect.lr().x))
            {
               if(theBottomGeographicLabelFlag)
               {
                  ossimDms dms(bgpt.lond(), false);
                  
                  ossimString dmsString = dms.toString(theBottomGeographicFormat.c_str());
                  ossimAnnotationFontObject* lonLabel = new ossimAnnotationFontObject(ossimIpt(0,0),
                                                                                      dmsString);
                  lonLabel->setGeometryInformation(theGeographicBottomLabelFontInfo);
                  lonLabel->setFont(theGeographicBottomLabelFont.get());
//                  lonLabel->setFont(theGeographicBottomLabelFont->dup());
                  
                  lonLabel->computeBoundingRect();
                  ossimDrect boundsD;
                  lonLabel->getBoundingRect(boundsD);
                  ossimIrect bounds = boundsD;
                  
                  ossimIpt center( rounded.x,
                                   ossim::round<int>(rect.lr().y+(bounds.height()/2)));
                  
                  lonLabel->setColor(theBottomGeographicLabelColor.getR(), theBottomGeographicLabelColor.getG(), theBottomGeographicLabelColor.getB());
                  lonLabel->setCenterPosition(center);
                  lonLabel->computeBoundingRect();
                  lonLabel->getBoundingRect(boundsD);
                  bounds = boundsD;

                  ossimDrect labelRect;
                  lonLabel->getBoundingRect(labelRect);
                  
                  if(rect.intersects(labelRect))
                  {
                     ossimIrect intersection = rect.clipToRect(labelRect);
                     center.y += ossim::round<int>(rect.lr().y - intersection.ul().y);
                  }

                  center.y += 24; // this will need to be the tick height later;
                  lonLabel->setCenterPosition(center);
                  lonLabel->computeBoundingRect();


                  addFixedAnnotation(lonLabel);
               }
               if(theBottomGeographicTickFlag)
               {
                  ossimAnnotationLineObject* lineLabel = new ossimAnnotationLineObject(ossimIpt(rounded.x,
                                                                                                ossim::round<int>(rect.lr().y+24)),
                                                                                       ossimIpt(rounded.x,
                                                                                                ossim::round<int>(rect.lr().y)));
                  lineLabel->setColor(theBottomGeographicLabelColor.getR(), theBottomGeographicLabelColor.getG(), theBottomGeographicLabelColor.getB());
                  addFixedAnnotation(lineLabel);
               }
            }         
         }
      }
   }   
}

void ossimMapCompositionSource::addGeographicLeftGridLabels()
{
   if((!theLeftGeographicLabelFlag)&&(!theLeftGeographicTickFlag))
   {
      return;
   }
   
   const ossimMapProjection* mapProj = inputMapProjection();
   if(mapProj)
   { 
      ossimGpt gpt[4];
      ossimDrect rect = getViewingRect();

      if(rect.hasNans()) return;
      mapProj->lineSampleToWorld(rect.ul(), gpt[0]);
      mapProj->lineSampleToWorld(rect.ur(), gpt[1]);
      mapProj->lineSampleToWorld(rect.lr(), gpt[2]);
      mapProj->lineSampleToWorld(rect.ll(), gpt[3]);
      
      ossimDrect grect(gpt[0],
                       gpt[1],
                       gpt[2],
                       gpt[3],
                       OSSIM_RIGHT_HANDED);

      if(grect.hasNans()) return;
      ossimDpt ulLatLon(((int)((grect.ul().x-theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.ul().y+theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);
      
      ossimDpt lrLatLon(((int)((grect.lr().x+theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.lr().y-theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);

      while(ulLatLon.lat > 90)
      {
         ulLatLon.lat -= theGeographicSpacing.lat;
      }
      while(ulLatLon.lon < -180)
      {
         ulLatLon.lon += theGeographicSpacing.lon;
      }
      while(lrLatLon.lat < -90)
      {
         ulLatLon.lat += theGeographicSpacing.lat;
      }
      while(lrLatLon.lon > 180)
      {
         ulLatLon.lon -= theGeographicSpacing.lon;
      }
      
      ossimDrect latLonSpacing(ulLatLon,
                               lrLatLon,
                               OSSIM_RIGHT_HANDED);
      // do the left side labels
      for(double lat = latLonSpacing.ul().y; lat >= latLonSpacing.lr().y; lat-=theGeographicSpacing.y)
      {
         ossimDpt lipt;
         ossimDpt ript;
         ossimGpt lgpt(lat,
                       latLonSpacing.ul().x,
                       0.0,
                       gpt[0].datum());
         ossimGpt rgpt(lat,
                       latLonSpacing.ur().x,
                       0.0,
                       gpt[0].datum());

         
         mapProj->worldToLineSample(lgpt, lipt);
         mapProj->worldToLineSample(rgpt, ript);
         if(rect.clip(lipt, ript))
         {
            ossimIpt rounded(lipt);
            if( (rounded.y >= rect.ul().y)&&
                (rounded.y <= rect.lr().y))
            {
               if(theLeftGeographicLabelFlag)
               {
                  ossimDms dms(lgpt.latd());
                  
                  ossimString dmsString = dms.toString(theLeftGeographicFormat.c_str());
                  ossimAnnotationFontObject* latLabel = new ossimAnnotationFontObject(ossimIpt(0,0),
                                                                                      dmsString);
                  latLabel->setGeometryInformation(theGeographicLeftLabelFontInfo);
                  latLabel->setFont(theGeographicLeftLabelFont.get());
//                  latLabel->setFont(theGeographicLeftLabelFont->dup());
                  
                  latLabel->computeBoundingRect();
                  ossimDrect boundsD;
                  latLabel->getBoundingRect(boundsD);
                  ossimIrect bounds = boundsD;
                  
                  ossimIpt center( ossim::round<int>((rect.ul().x - (bounds.width()/2))),
                                   rounded.y);
                  
                  latLabel->setColor(theLeftGeographicLabelColor.getR(), theLeftGeographicLabelColor.getG(), theLeftGeographicLabelColor.getB());
                  latLabel->setCenterPosition(center);
                  latLabel->computeBoundingRect();
                  latLabel->getBoundingRect(boundsD);
                  bounds = boundsD;

                  ossimDrect labelRect;
                  latLabel->getBoundingRect(labelRect);
                  
                  if(rect.intersects(labelRect))
                  {
                     ossimIrect intersection = rect.clipToRect(labelRect);
                     center.x -= ossim::round<int>(rect.ul().x - intersection.ur().x);
                  }

                  center.x -= 24; // this will need to be the tick height later;
                  latLabel->setCenterPosition(center);
                  latLabel->computeBoundingRect();


                  
                  addFixedAnnotation(latLabel);
               }
               
               if(theLeftGeographicTickFlag)
               {
                  ossimAnnotationLineObject* lineLabel = new ossimAnnotationLineObject(ossimIpt(ossim::round<int>(rect.ul().x),
                                                                                                rounded.y),
                                                                                       ossimIpt(ossim::round<int>(rect.ul().x-23),
                                                                                                rounded.y));
                  lineLabel->setColor(theLeftGeographicLabelColor.getR(), theLeftGeographicLabelColor.getG(), theLeftGeographicLabelColor.getB());
                  addFixedAnnotation(lineLabel);
               }
            }
         }
      }
   }
}

void ossimMapCompositionSource::addGeographicRightGridLabels()
{
   if((!theRightGeographicLabelFlag)&&(!theRightGeographicTickFlag))
   {
      return;
   }

   const ossimMapProjection* mapProj = inputMapProjection();
   if(mapProj)
   { 
      ossimGpt gpt[4];
      ossimDrect rect = getViewingRect();

      if(rect.hasNans()) return;
      mapProj->lineSampleToWorld(rect.ul(), gpt[0]);
      mapProj->lineSampleToWorld(rect.ur(), gpt[1]);
      mapProj->lineSampleToWorld(rect.lr(), gpt[2]);
      mapProj->lineSampleToWorld(rect.ll(), gpt[3]);
      
      ossimDrect grect(gpt[0],
                       gpt[1],
                       gpt[2],
                       gpt[3],
                       OSSIM_RIGHT_HANDED);

      if(grect.hasNans()) return;
      ossimDpt ulLatLon(((int)((grect.ul().x-theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.ul().y+theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);
      
      ossimDpt lrLatLon(((int)((grect.lr().x+theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.lr().y-theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);

      while(ulLatLon.lat > 90)
      {
         ulLatLon.lat -= theGeographicSpacing.lat;
      }
      while(ulLatLon.lon < -180)
      {
         ulLatLon.lon += theGeographicSpacing.lon;
      }
      while(lrLatLon.lat < -90)
      {
         ulLatLon.lat += theGeographicSpacing.lat;
      }
      while(lrLatLon.lon > 180)
      {
         ulLatLon.lon -= theGeographicSpacing.lon;
      }
      
      ossimDrect latLonSpacing(ulLatLon,
                               lrLatLon,
                               OSSIM_RIGHT_HANDED);
      // do the left side labels
      for(double lat = latLonSpacing.ul().y; lat >= latLonSpacing.lr().y; lat-=theGeographicSpacing.y)
      {
         ossimDpt lipt;
         ossimDpt ript;
         ossimGpt lgpt(lat,
                       latLonSpacing.ul().x,
                       0.0,
                       gpt[0].datum());
         ossimGpt rgpt(lat,
                       latLonSpacing.ur().x,
                       0.0,
                       gpt[0].datum());
         
         
         mapProj->worldToLineSample(lgpt, lipt);
         mapProj->worldToLineSample(rgpt, ript);
         
         if(rect.clip(lipt, ript))
         {
            ossimIpt rounded(ript);
            if( (rounded.y >= rect.ul().y)&&
                (rounded.y <= rect.lr().y))
            {
               if(theRightGeographicLabelFlag)
               {
                  ossimDms dms(rgpt.latd());
                  
                  ossimString dmsString = dms.toString(theRightGeographicFormat.c_str());
                  ossimAnnotationFontObject* latLabel = new ossimAnnotationFontObject(ossimIpt(0,0),
                                                                                      dmsString);
                  latLabel->setGeometryInformation(theGeographicRightLabelFontInfo);
                  latLabel->setFont(theGeographicRightLabelFont.get());
//                  latLabel->setFont(theGeographicRightLabelFont->dup());
                  latLabel->computeBoundingRect();
                  ossimDrect boundsD;
                  latLabel->getBoundingRect(boundsD);
                  ossimIrect bounds = boundsD;
                  
                  ossimIpt center( ossim::round<int>((rect.ur().x + (bounds.width()/2))),
                                   rounded.y);
               
                  latLabel->setColor(theRightGeographicLabelColor.getR(), theRightGeographicLabelColor.getG(), theRightGeographicLabelColor.getB());
                  latLabel->setCenterPosition(center);
                  latLabel->computeBoundingRect();
                  latLabel->getBoundingRect(boundsD);
                  bounds = boundsD;

                  ossimDrect labelRect;
                  latLabel->getBoundingRect(labelRect);
                  
                  if(rect.intersects(labelRect))
                  {
                     ossimIrect intersection = rect.clipToRect(labelRect);
                     center.x -= ossim::round<int>(rect.ur().x - intersection.ul().x);
                  }

                  center.x += 24; // this will need to be the tick height later;
                  latLabel->setCenterPosition(center);
                  latLabel->computeBoundingRect();
                  

                  addFixedAnnotation(latLabel);
               }
               
               if(theRightGeographicTickFlag)
               {
                  ossimAnnotationLineObject* lineLabel = new ossimAnnotationLineObject(ossimIpt(ossim::round<int>(rect.lr().x),
                                                                                                rounded.y),
                                                                                       ossimIpt(ossim::round<int>(rect.lr().x+23),
                                                                                                rounded.y));
                  
                  lineLabel->setColor(theRightGeographicLabelColor.getR(), theRightGeographicLabelColor.getG(), theRightGeographicLabelColor.getB());
                  addFixedAnnotation(lineLabel);
               }
            }
         }
      }
   }
}

void ossimMapCompositionSource::addGeographicGridLines()
{
   const ossimMapProjection* mapProj = inputMapProjection();
   if(mapProj)
   { 
      ossimGpt gpt[4];
      ossimDrect rect = getViewingRect();

      if(rect.hasNans()) return;
      mapProj->lineSampleToWorld(rect.ul(), gpt[0]);
      mapProj->lineSampleToWorld(rect.ur(), gpt[1]);
      mapProj->lineSampleToWorld(rect.lr(), gpt[2]);
      mapProj->lineSampleToWorld(rect.ll(), gpt[3]);
      
      ossimDrect grect(gpt[0],
                       gpt[1],
                       gpt[2],
                       gpt[3],
                       OSSIM_RIGHT_HANDED);

      if(grect.hasNans()) return;
      ossimDpt ulLatLon(((int)((grect.ul().x-theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.ul().y+theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);
      
      ossimDpt lrLatLon(((int)((grect.lr().x+theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.lr().y-theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);

      while(ulLatLon.lat > 90)
      {
         ulLatLon.lat -= theGeographicSpacing.lat;
      }
      while(ulLatLon.lon < -180)
      {
         ulLatLon.lon += theGeographicSpacing.lon;
      }
      while(lrLatLon.lat < -90)
      {
         ulLatLon.lat += theGeographicSpacing.lat;
      }
      while(lrLatLon.lon > 180)
      {
         ulLatLon.lon -= theGeographicSpacing.lon;
      }
      
      ossimDrect latLonSpacing(ulLatLon,
                               lrLatLon,
                               OSSIM_RIGHT_HANDED);


      for(double lon = latLonSpacing.ul().x; lon <= latLonSpacing.lr().x; lon+=theGeographicSpacing.x)
      {
         ossimDpt tipt; // top
         ossimDpt bipt; // bottom
         
         ossimGpt tgpt(latLonSpacing.ul().y,
                       lon,
                       0.0,
                       gpt[0].datum());

         ossimGpt bgpt(latLonSpacing.lr().y,
                       lon,
                       0.0,
                       gpt[0].datum());
         
         mapProj->worldToLineSample(tgpt, tipt);
         mapProj->worldToLineSample(bgpt, bipt);

         if(rect.clip(tipt, bipt))
         {
            ossimAnnotationLineObject* line = new ossimAnnotationLineObject(tipt, bipt);
            line->setColor(theGeographicGridColor.getR(),
                           theGeographicGridColor.getG(),
                           theGeographicGridColor.getB());
            addFixedAnnotation(line);
         }
      }
      // do the left side labels
      for(double lat = latLonSpacing.ul().y; lat >= latLonSpacing.lr().y; lat-=theGeographicSpacing.y)
      {
         ossimDpt lipt;
         ossimDpt ript;
         ossimGpt lgpt(lat,
                       latLonSpacing.ul().x,
                       0.0,
                       gpt[0].datum());
         ossimGpt rgpt(lat,
                       latLonSpacing.ur().x,
                       0.0,
                       gpt[0].datum());

         mapProj->worldToLineSample(lgpt, lipt);
         mapProj->worldToLineSample(rgpt, ript);
         
         if(rect.clip(lipt, ript))
         {
            ossimAnnotationLineObject* line = new ossimAnnotationLineObject(lipt,
                                                                            ript);
         
            line->setColor(theGeographicGridColor.getR(),
                                theGeographicGridColor.getG(),
                                theGeographicGridColor.getB());
         
            addFixedAnnotation(line);
         }
      }
   }   
}

void ossimMapCompositionSource::addGeographicGridReseaux()
{
   const ossimMapProjection* mapProj = inputMapProjection();
   if(mapProj)
   { 
      ossimGpt gptArray[4];
      ossimDrect rect = getViewingRect();
      
      mapProj->lineSampleToWorld(rect.ul(), gptArray[0]);
      mapProj->lineSampleToWorld(rect.ur(), gptArray[1]);
      mapProj->lineSampleToWorld(rect.lr(), gptArray[2]);
      mapProj->lineSampleToWorld(rect.ll(), gptArray[3]);
      
      ossimDrect grect(gptArray[0],
                       gptArray[1],
                       gptArray[2],
                       gptArray[3],
                       OSSIM_RIGHT_HANDED);

      ossimDpt ulLatLon(((int)((grect.ul().x-theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.ul().y+theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);
      
      ossimDpt lrLatLon(((int)((grect.lr().x+theGeographicSpacing.x)/theGeographicSpacing.x))*theGeographicSpacing.x,
                        ((int)((grect.lr().y-theGeographicSpacing.y)/theGeographicSpacing.y))*theGeographicSpacing.y);
      
      ossimDrect latLonSpacing(ulLatLon,
                               lrLatLon,
                               OSSIM_RIGHT_HANDED);

      ossimDpt ipt; // top
      for(double lat = latLonSpacing.ul().y; lat >= latLonSpacing.lr().y; lat-=theGeographicSpacing.y)
      {
         for(double lon = latLonSpacing.ul().x; lon <= latLonSpacing.lr().x; lon+=theGeographicSpacing.x)
         {
            
            ossimGpt gpt(lat,
                         lon,
                         0.0,
                         gptArray[0].datum());
            
            mapProj->worldToLineSample(gpt, ipt);

            ossimIpt rounded(ipt);
            if(rect.pointWithin(ossimDpt(rounded)))
            {
               ossimIpt horizontalStart(rounded.x-10, rounded.y);
               ossimIpt horizontalEnd(rounded.x+10, rounded.y);

               ossimIpt verticalStart(rounded.x, rounded.y-10);
               ossimIpt verticalEnd(rounded.x, rounded.y+10);
               
               ossimAnnotationLineObject* line1 = new ossimAnnotationLineObject(horizontalStart, horizontalEnd);
               ossimAnnotationLineObject* line2 = new ossimAnnotationLineObject(verticalStart,verticalEnd);

               line1->setColor(theGeographicGridColor.getR(),
                               theGeographicGridColor.getG(),
                               theGeographicGridColor.getB());
               line2->setColor(theGeographicGridColor.getR(),
                               theGeographicGridColor.getG(),
                               theGeographicGridColor.getB());
               addFixedAnnotation(line1);
               addFixedAnnotation(line2);
            }  
         }
      }
   }   
}

void ossimMapCompositionSource::addMeterGridLines()
{
   const ossimMapProjection* mapProj = inputMapProjection();
   if(mapProj)
   { 
      ossimDpt dptArray[4];
      ossimDrect rect = getViewingRect();
      
      mapProj->lineSampleToEastingNorthing(rect.ul(), dptArray[0]);
      mapProj->lineSampleToEastingNorthing(rect.ur(), dptArray[1]);
      mapProj->lineSampleToEastingNorthing(rect.lr(), dptArray[2]);
      mapProj->lineSampleToEastingNorthing(rect.ll(), dptArray[3]);
      
      ossimDrect drect(dptArray[0], dptArray[1], dptArray[2], dptArray[3], OSSIM_RIGHT_HANDED);
      
      ossimDpt ulMeter(((int)((drect.ul().x-theMeterSpacing.x)/theMeterSpacing.x))*theMeterSpacing.x,
                       ((int)((drect.ul().y+theMeterSpacing.y)/theMeterSpacing.y))*theMeterSpacing.y);
      
      ossimDpt lrMeter(((int)((drect.lr().x+theMeterSpacing.x)/theMeterSpacing.x))*theMeterSpacing.x,
                        ((int)((drect.lr().y-theMeterSpacing.y)/theMeterSpacing.y))*theMeterSpacing.y);
      
      ossimDrect meterSpacing(ulMeter,
                              lrMeter,
                              OSSIM_RIGHT_HANDED);

      for(double horizontal = meterSpacing.ul().x;
          horizontal <= meterSpacing.lr().x;
          horizontal+=theMeterSpacing.x)
      {
         ossimDpt tipt; // top
         ossimDpt bipt; // bottom

         ossimDpt tdpt(horizontal, meterSpacing.ul().y);

         ossimDpt bdpt(horizontal, meterSpacing.lr().y);
         
         mapProj->eastingNorthingToLineSample(tdpt, tipt);
         mapProj->eastingNorthingToLineSample(bdpt, bipt);
         
         if(rect.clip(tipt, bipt))
         {
            ossimAnnotationLineObject* line = new ossimAnnotationLineObject(tipt, bipt);
            line->setColor(theMeterGridColor.getR(),
                           theMeterGridColor.getG(),
                           theMeterGridColor.getB());
            addFixedAnnotation(line);
         }
      }
      // do the left side labels
      for(double vertical = meterSpacing.ul().y;
          vertical >= meterSpacing.lr().y;
          vertical-=theMeterSpacing.y)
      {
         ossimDpt lipt;
         ossimDpt ript;
         ossimDpt ldpt(meterSpacing.ul().x, vertical);
         ossimDpt rdpt(meterSpacing.ur().x, vertical);

         mapProj->eastingNorthingToLineSample(ldpt,  lipt);
         mapProj->eastingNorthingToLineSample(rdpt,  ript);
         
         if(rect.clip(lipt, ript))
         {
            ossimAnnotationLineObject* line = new ossimAnnotationLineObject(lipt, ript);
         
            line->setColor(theMeterGridColor.getR(),
                           theMeterGridColor.getG(),
                           theMeterGridColor.getB());
            
            addFixedAnnotation(line);
         }
      }
   }
}

void ossimMapCompositionSource::addMeterGridLabels()
{
   const ossimMapProjection* mapProj = inputMapProjection();
   if(mapProj)
   {
      ossimDpt dptArray[4];
      ossimDrect rect = getViewingRect();
      
      mapProj->lineSampleToEastingNorthing(rect.ul(), dptArray[0]);
      mapProj->lineSampleToEastingNorthing(rect.ur(), dptArray[1]);
      mapProj->lineSampleToEastingNorthing(rect.lr(), dptArray[2]);
      mapProj->lineSampleToEastingNorthing(rect.ll(), dptArray[3]);
      
      ossimDrect drect(dptArray[0],
                       dptArray[1],
                       dptArray[2],
                       dptArray[3],
                       OSSIM_RIGHT_HANDED);
      
      ossimDpt ulMeter(((int)((drect.ul().x-theMeterSpacing.x)/theMeterSpacing.x))*theMeterSpacing.x,
                       ((int)((drect.ul().y+theMeterSpacing.y)/theMeterSpacing.y))*theMeterSpacing.y);
      
      ossimDpt lrMeter(((int)((drect.lr().x+theMeterSpacing.x)/theMeterSpacing.x))*theMeterSpacing.x,
                        ((int)((drect.lr().y-theMeterSpacing.y)/theMeterSpacing.y))*theMeterSpacing.y);
      
      ossimDrect meterSpacing(ulMeter,
                              lrMeter,
                              OSSIM_RIGHT_HANDED);
      
      ossimDrect labelRect;

      // Since we support overlapping grid lables.  We will shift
      // the meter label if there exists a gegraphic label.  To
      // do this we need to compute an extra shift length based
      // on the font type on each of the sides.  We could actually make this
      // shift adjustable later????
      //
      long extraTopDelta     = 0;
      long extraBottomDelta  = 0;
      long extraLeftDelta    = 0;
      long extraRightDelta   = 0;
         
      if(theTopGeographicLabelFlag&&theGeographicTopLabelFont.valid())
      {  
         ossimDms dms(180, false);
         ossimString dmsString = dms.toString(theTopGeographicFormat.c_str());
         
         theGeographicTopLabelFont->setString(dmsString);
         ossimIrect box;
         
         theGeographicTopLabelFont->getBoundingBox(box);
         extraTopDelta = box.height();
      }
      if(theBottomGeographicLabelFlag&&theGeographicBottomLabelFont.valid())
      {  
         ossimDms dms(180, false);
         ossimString dmsString = dms.toString(theBottomGeographicFormat.c_str());
         theGeographicBottomLabelFont->setString(dmsString);
         ossimIrect box;
         
         theGeographicBottomLabelFont->getBoundingBox(box);
         extraBottomDelta = box.height();
      }
      if(theLeftGeographicLabelFlag&&theGeographicLeftLabelFont.valid())
      {  
         ossimDms dms(90.0, true);
         ossimString dmsString = dms.toString(theLeftGeographicFormat.c_str());
         theGeographicLeftLabelFont->setString(dmsString);
         ossimIrect box;
         
         theGeographicLeftLabelFont->getBoundingBox(box);
         extraLeftDelta = box.width();
      }
      if(theRightGeographicLabelFlag&&theGeographicRightLabelFont.valid())
      {  
         ossimDms dms(90.0, true);
         ossimString dmsString = dms.toString(theRightGeographicFormat.c_str());
         
         theGeographicRightLabelFont->setString(dmsString);
         ossimIrect box;
         
         theGeographicRightLabelFont->getBoundingBox(box);
         extraRightDelta = box.width();
      }

      if(theTopMeterLabelFlag || theBottomMeterLabelFlag)
      {
         for(double horizontal = meterSpacing.ul().x;
             horizontal <= meterSpacing.lr().x;
             horizontal+=theMeterSpacing.x)
         {
            ossimDpt tipt; // top
            ossimDpt bipt; // bottom
            
            ossimDpt tdpt(horizontal,
                          meterSpacing.ul().y);
            
            ossimDpt bdpt(horizontal,
                          meterSpacing.lr().y);
            
            mapProj->eastingNorthingToLineSample(tdpt, tipt);
            mapProj->eastingNorthingToLineSample(bdpt, bipt);
            
            rect.clip(tipt, bipt);
            
            tipt = ossimIpt(tipt);
            bipt = ossimIpt(bipt);
            
            if(theTopMeterLabelFlag&&rect.clip(tipt, bipt))
            {
               ossimAnnotationFontObject* topLabel = new ossimAnnotationFontObject(ossimIpt(0,0),
                                                                                   ossimString::toString(horizontal).c_str());
               
               
               topLabel->setFont(theMeterTopLabelFont.get());
               
               topLabel->setGeometryInformation(theMeterTopLabelFontInfo);
               topLabel->computeBoundingRect();
               ossimDrect boundsTop;
               topLabel->getBoundingRect(boundsTop);
               
               ossimIpt centerTop(ossim::round<int>(tipt.x),
                                  ossim::round<int>(rect.ul().y-(boundsTop.height()/2)));
               
               topLabel->setColor(theTopMeterLabelColor.getR(),
                                  theTopMeterLabelColor.getG(),
                                  theTopMeterLabelColor.getB());
               
               
               topLabel->setCenterPosition(centerTop);
               topLabel->computeBoundingRect();
               
               topLabel->getBoundingRect(labelRect);
               
               if(rect.intersects(labelRect))
               {
                  ossimIrect intersection = rect.clipToRect(labelRect);
                  centerTop.y -= ossim::round<int>(rect.ul().y - intersection.lr().y);
               }
               
               centerTop.y -= (24+extraTopDelta); // this will need to be the tick height later;
               topLabel->setCenterPosition(centerTop);
               topLabel->computeBoundingRect();
               addFixedAnnotation(topLabel);

               if(theTopMeterTickFlag)
               {
                  ossimAnnotationLineObject* lineLabel = new ossimAnnotationLineObject(ossimIpt(ossim::round<int>(tipt.x),
                                                                                                ossim::round<int>(tipt.y-24)),
                                                                                       ossimIpt(ossim::round<int>(tipt.x),
                                                                                                ossim::round<int>(tipt.y)));
                  
                  lineLabel->setColor(theTopMeterLabelColor.getR(), theTopMeterLabelColor.getG(), theTopMeterLabelColor.getB());
                  addFixedAnnotation(lineLabel);
               }
            }
         // ------------------------- BOTTOM ----------------------------
            
            if(theBottomMeterLabelFlag&&rect.pointWithin(bipt))
            {
               ossimAnnotationFontObject* bottomLabel = new ossimAnnotationFontObject(ossimIpt(0,0),
                                                                                      ossimString::toString(horizontal).c_str());
               
               
               bottomLabel->setFont(theMeterBottomLabelFont.get());
               
               bottomLabel->setGeometryInformation(theMeterBottomLabelFontInfo);
               bottomLabel->computeBoundingRect();
               ossimDrect boundsBottom;
               bottomLabel->getBoundingRect(boundsBottom);
               
               ossimIpt centerBottom(ossim::round<int>(bipt.x),
                                     ossim::round<int>(rect.lr().y+(boundsBottom.height()/2)));
               
               bottomLabel->setColor(theBottomMeterLabelColor.getR(),
                                     theBottomMeterLabelColor.getG(),
                                     theBottomMeterLabelColor.getB());
               
               
               bottomLabel->setCenterPosition(centerBottom);
               bottomLabel->computeBoundingRect();
               
               bottomLabel->getBoundingRect(labelRect);
               
               if(rect.intersects(labelRect))
               {
                  ossimIrect intersection = rect.clipToRect(labelRect);
                  centerBottom.y += ossim::round<int>(rect.lr().y - intersection.ul().y);
               }
               
               centerBottom.y += (24+extraBottomDelta); // this will need to be the tick height later;
               bottomLabel->setCenterPosition(centerBottom);
               bottomLabel->computeBoundingRect();
               addFixedAnnotation(bottomLabel);
               if(theBottomMeterTickFlag)
               {
                  ossimAnnotationLineObject* lineLabel = new ossimAnnotationLineObject(ossimIpt(ossim::round<int>(bipt.x),
                                                                                                ossim::round<int>(bipt.y+24)),
                                                                                       ossimIpt(ossim::round<int>(bipt.x),
                                                                                                ossim::round<int>(bipt.y)));
                  
                  lineLabel->setColor(theBottomMeterLabelColor.getR(), theBottomMeterLabelColor.getG(), theBottomMeterLabelColor.getB());
                  addFixedAnnotation(lineLabel);
               }
            }            
         }
      }
      if(theLeftMeterLabelFlag || theRightMeterLabelFlag)
      {
         for(double vertical = meterSpacing.ul().y;
             vertical >= meterSpacing.lr().y;
             vertical-=theMeterSpacing.y)
         {
            ossimDpt lipt;
            ossimDpt ript;
            ossimDpt ldpt(meterSpacing.ul().x,
                          vertical);
            ossimDpt rdpt(meterSpacing.ur().x,
                          vertical);
            
            mapProj->eastingNorthingToLineSample(ldpt, lipt);
            mapProj->eastingNorthingToLineSample(rdpt, ript);
            
            if(rect.clip(lipt, ript))
            {
               //------------------------------------------------ Left label ----------------------------------------------
               ossimAnnotationFontObject* leftLabel = new ossimAnnotationFontObject(ossimIpt(0,0),
                                                                                    ossimString::toString(vertical).c_str());
               
               
               leftLabel->setFont(theMeterLeftLabelFont.get());
               
               leftLabel->setGeometryInformation(theMeterLeftLabelFontInfo);
               leftLabel->computeBoundingRect();
               ossimDrect boundsLeft;
               leftLabel->getBoundingRect(boundsLeft);
               
               ossimIpt centerLeft(ossim::round<int>(rect.ul().x-(boundsLeft.width()/2)),
                                   ossim::round<int>(lipt.y));
                                  
               
               leftLabel->setColor(theLeftMeterLabelColor.getR(),
                                   theLeftMeterLabelColor.getG(),
                                   theLeftMeterLabelColor.getB());
               
               
               leftLabel->setCenterPosition(centerLeft);
               leftLabel->computeBoundingRect();
               
               leftLabel->getBoundingRect(labelRect);
               
               if(rect.intersects(labelRect))
               {
                  ossimIrect intersection = rect.clipToRect(labelRect);
                  centerLeft.x -= ossim::round<int>(intersection.lr().x - rect.ul().x);
               }
               
               centerLeft.x -= (24+extraLeftDelta); // this will need to be the tick height later;
               leftLabel->setCenterPosition(centerLeft);
               leftLabel->computeBoundingRect();
               addFixedAnnotation(leftLabel);
               if(theLeftMeterTickFlag)
               {
                  ossimAnnotationLineObject* lineLabel = new ossimAnnotationLineObject(ossimIpt(ossim::round<int>(lipt.x-24),
                                                                                                ossim::round<int>(lipt.y)),
                                                                                       ossimIpt(ossim::round<int>(lipt.x),
                                                                                                ossim::round<int>(lipt.y)));
                  
                  lineLabel->setColor(theLeftMeterLabelColor.getR(), theLeftMeterLabelColor.getG(), theLeftMeterLabelColor.getB());
                  addFixedAnnotation(lineLabel);
               }

               
               //--------------------------------RIGHT-----------------------------------------------------------
               ossimAnnotationFontObject* rightLabel = new ossimAnnotationFontObject(ossimIpt(0,0),
                                                                                     ossimString::toString(vertical).c_str());
               
               
               rightLabel->setFont(theMeterRightLabelFont.get());
               
               rightLabel->setGeometryInformation(theMeterRightLabelFontInfo);
               rightLabel->computeBoundingRect();
               ossimDrect boundsRight;
               rightLabel->getBoundingRect(boundsRight);
               
               ossimIpt centerRight(ossim::round<int>(rect.ur().x+(boundsRight.width()/2)),
                                    ossim::round<int>(ript.y));
                                  
               
               rightLabel->setColor(theRightMeterLabelColor.getR(),
                                    theRightMeterLabelColor.getG(),
                                    theRightMeterLabelColor.getB());
               
               
               rightLabel->setCenterPosition(centerRight);
               rightLabel->computeBoundingRect();
               
               rightLabel->getBoundingRect(labelRect);
               
               if(rect.intersects(labelRect))
               {
                  ossimIrect intersection = rect.clipToRect(labelRect);
                  centerRight.x += ossim::round<int>(intersection.ul().x - rect.ur().x);
               }
               
               centerRight.x += (24+extraRightDelta); // this will need to be the tick height later;
               rightLabel->setCenterPosition(centerRight);
               rightLabel->computeBoundingRect();
               addFixedAnnotation(rightLabel);
               if(theRightMeterTickFlag)
               {
                  ossimAnnotationLineObject* lineLabel = new ossimAnnotationLineObject(ossimIpt(ossim::round<int>(ript.x+24),
                                                                                                ossim::round<int>(ript.y)),
                                                                                       ossimIpt(ossim::round<int>(ript.x),
                                                                                                ossim::round<int>(ript.y)));
                  
                  lineLabel->setColor(theRightMeterLabelColor.getR(), theRightMeterLabelColor.getG(), theRightMeterLabelColor.getB());
                  addFixedAnnotation(lineLabel);
               }
            }
         }
      }
   }
}


void ossimMapCompositionSource::addMeterGridReseaux()
{
   const ossimMapProjection* mapProj = inputMapProjection();
   if(mapProj)
   { 
      ossimDpt dpt[4];
      ossimDrect rect = getViewingRect();
      
      mapProj->lineSampleToEastingNorthing(rect.ul(), dpt[0]);
      mapProj->lineSampleToEastingNorthing(rect.ur(), dpt[1]);
      mapProj->lineSampleToEastingNorthing(rect.lr(), dpt[2]);
      mapProj->lineSampleToEastingNorthing(rect.ll(), dpt[3]);
      
      ossimDrect drect(dpt[0],
                       dpt[1],
                       dpt[2],
                       dpt[3],
                       OSSIM_RIGHT_HANDED);

      ossimDpt ulMeter(((int)((drect.ul().x-theMeterSpacing.x)/theMeterSpacing.x))*theMeterSpacing.x,
                       ((int)((drect.ul().y+theMeterSpacing.y)/theMeterSpacing.y))*theMeterSpacing.y);
      
      ossimDpt lrMeter(((int)((drect.lr().x+theMeterSpacing.x)/theMeterSpacing.x))*theMeterSpacing.x,
                        ((int)((drect.lr().y-theMeterSpacing.y)/theMeterSpacing.y))*theMeterSpacing.y);

      ossimDrect meterSpacing(ulMeter,
                              lrMeter,
                              OSSIM_RIGHT_HANDED);

      ossimDpt ipt; // top

      for(double vertical = meterSpacing.ul().y; vertical >= meterSpacing.lr().y; vertical-=theMeterSpacing.y)
      {
         for(double horizontal = meterSpacing.ul().x;  horizontal <= meterSpacing.lr().x; horizontal+=theMeterSpacing.x)
         {            
            ossimDpt dpt(horizontal,
                         vertical);
            
            mapProj->eastingNorthingToLineSample(dpt, ipt);

            ossimIpt rounded(ipt);
            if(rect.pointWithin(ossimDpt(rounded)))
            {
               ossimIpt horizontalStart(rounded.x-10, rounded.y);
               ossimIpt horizontalEnd(rounded.x+10, rounded.y);

               ossimIpt verticalStart(rounded.x, rounded.y-10);
               ossimIpt verticalEnd(rounded.x, rounded.y+10);
               
               ossimAnnotationLineObject* line1 = new ossimAnnotationLineObject(horizontalStart, horizontalEnd);
               ossimAnnotationLineObject* line2 = new ossimAnnotationLineObject(verticalStart,verticalEnd);

               line1->setColor(theMeterGridColor.getR(),
                               theMeterGridColor.getG(),
                               theMeterGridColor.getB());

               line2->setColor(theMeterGridColor.getR(),
                               theMeterGridColor.getG(),
                               theMeterGridColor.getB());
               addFixedAnnotation(line1);
               addFixedAnnotation(line2);
            }  
         }
      }
   }   
}

void ossimMapCompositionSource::addTitle()
{
   ossimAnnotationFontObject* title = new ossimAnnotationFontObject(ossimIpt(0,0),
                                                                    theTitleString);
   title->setGeometryInformation(theTitleFontInfo);
   title->setFont(theTitleFont.get());
   title->setColor(theTitleColor.getR(),
                   theTitleColor.getG(),
                   theTitleColor.getB());
   title->setCenterPosition(theTopBorder.midPoint());
   title->computeBoundingRect();

   addFixedAnnotation(title);
}

ossimIrect ossimMapCompositionSource::getViewingRect()const
{
   ossimIrect result;
   result.makeNan();

   if(theInputConnection)
   {
      result = theInputConnection->getBoundingRect();

      if(!result.hasNans())
      {
         ossimIpt mid = result.midPoint();
         ossim_int32 w = result.width();
         ossim_int32 h = result.width();

         if(theViewWidthHeight.x >-1)
         {
            w = theViewWidthHeight.x;
         }
         if(theViewWidthHeight.y >-1)
         {
            h = theViewWidthHeight.y;
         }
         ossimIpt ul(mid.x - w/2,
                     mid.y - h/2);
         
         result = ossimIrect(ul.x,
                             ul.y,
                             ul.x + w-1,
                             ul.y + h -1);
      }
   }

   return result;
}

void ossimMapCompositionSource::drawAnnotations(
   ossimRefPtr<ossimImageData> tile)
{
   if (!theImage)
   {
      return;
   }
      
   theImage->setCurrentImageData(theTile);
   
   if(theImage->getImageData().valid())
   {
      ossimAnnotationSource::AnnotationObjectListType::iterator object = theFixedAnnotationList.begin();
      while(object != theFixedAnnotationList.end())
      {
         if((*object).valid())
         {
            (*object)->draw(*theImage);
         }
         ++object;
      }      
   }
   
   ossimAnnotationSource::drawAnnotations(tile);
}

void ossimMapCompositionSource::deleteFixedAnnotations()
{
   theFixedAnnotationList.clear();
}


void ossimMapCompositionSource::setProperty(ossimRefPtr<ossimProperty> property)
{
   if(!property.valid()) return;
   
   ossimRefPtr<ossimProperty>        tempProp     = 0;
   ossimFontProperty*    fontProp     = 0;
   ossimColorProperty*   colorProp    = 0;
   ossimBooleanProperty* booleanProp  = 0;
   
   if(property->getName() == "Geographic label properties")
   {
      ossimContainerProperty* containerProp = PTR_CAST(ossimContainerProperty,
                                                       property.get());
      if(containerProp)
      {
         // first set the font format
         //
         tempProp = containerProp->getProperty("Top font");
         fontProp = PTR_CAST(ossimFontProperty, tempProp.get());
         if(fontProp)
         {
            setGeographicTopLabelFont(fontProp->getFontInformation());
         }
         tempProp = containerProp->getProperty("Bottom font");
         fontProp = PTR_CAST(ossimFontProperty, tempProp.get());
         if(fontProp)
         {
            setGeographicBottomLabelFont(fontProp->getFontInformation());
         }
         tempProp = containerProp->getProperty("Left font");
         fontProp = PTR_CAST(ossimFontProperty, tempProp.get());
         if(fontProp)
         {
            setGeographicLeftLabelFont(fontProp->getFontInformation());
         }
         tempProp = containerProp->getProperty("Right font");
         fontProp = PTR_CAST(ossimFontProperty, tempProp.get());
         if(fontProp)
         {
            setGeographicRightLabelFont(fontProp->getFontInformation());
         }

         // set color property
         tempProp = containerProp->getProperty("Top color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(colorProp)
         {
            setTopGeographicLabelColor(colorProp->getColor());
         }
         tempProp = containerProp->getProperty("Bottom color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(colorProp)
         {
            setBottomGeographicLabelColor(colorProp->getColor());
         }
         tempProp = containerProp->getProperty("Left color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(colorProp)
         {
            setLeftGeographicLabelColor(colorProp->getColor());
         }
         tempProp = containerProp->getProperty("Right color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(colorProp)
         {
            setRightGeographicLabelColor(colorProp->getColor());
         }

         // set enable flags
         tempProp = containerProp->getProperty("Top enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setTopGeographicLabelFlag(booleanProp->getBoolean());
         }

         tempProp = containerProp->getProperty("Bottom enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setBottomGeographicLabelFlag(booleanProp->getBoolean());
         }

         tempProp = containerProp->getProperty("Left enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setLeftGeographicLabelFlag(booleanProp->getBoolean());
         }

         tempProp = containerProp->getProperty("Right enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setRightGeographicLabelFlag(booleanProp->getBoolean());
         }
         
         tempProp = containerProp->getProperty("Top tick enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setTopGeographicTickMarkFlag(booleanProp->getBoolean());
         }
         tempProp = containerProp->getProperty("Bottom tick enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setBottomGeographicTickMarkFlag(booleanProp->getBoolean());
         }
         tempProp = containerProp->getProperty("Left tick enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setLeftGeographicTickMarkFlag(booleanProp->getBoolean());
         }
         tempProp = containerProp->getProperty("Right tick enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setRightGeographicTickMarkFlag(booleanProp->getBoolean());
         }
      }
   }
   else if(property->getName() == "Geographic grid properties")
   {
      ossimContainerProperty* containerProp = PTR_CAST(ossimContainerProperty,
                                                       property.get());
      if(containerProp)
      {
         tempProp = containerProp->getProperty("Grid type");
         if(tempProp.valid())
         {
            ossimString value = tempProp->valueToString();

            if(value == "none")
            {
               setGeographicGridType(OSSIM_GRID_NONE);
            }
            else if(value == "line")
            {
               setGeographicGridType(OSSIM_GRID_LINE);
            }
            else if(value == "reseaux")
            {
               setGeographicGridType(OSSIM_GRID_RESEAUX);
            }
         }
         tempProp = containerProp->getProperty("Horizontal spacing");
         if(tempProp.valid())
         {
            setGeographicSpacingLon(tempProp->valueToString().toDouble());
         }

         tempProp = containerProp->getProperty("Vertical spacing");
         if(tempProp.valid())
         {
            setGeographicSpacingLat(tempProp->valueToString().toDouble());
         }

         tempProp = containerProp->getProperty("Color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(tempProp.valid())
         {
            setGeographicGridColor(colorProp->getColor());
         }
      }
   }
   else if(property->getName() == "Meter label properties")
   {
      ossimContainerProperty* containerProp = PTR_CAST(ossimContainerProperty,
                                                       property.get());
      if(containerProp)
      {
         // first set the font format
         //
         tempProp = containerProp->getProperty("Top font");
         fontProp = PTR_CAST(ossimFontProperty, tempProp.get());
         if(fontProp)
         {
            setMeterTopLabelFont(fontProp->getFontInformation());
         }
         tempProp = containerProp->getProperty("Bottom font");
         fontProp = PTR_CAST(ossimFontProperty, tempProp.get());
         if(fontProp)
         {
            setMeterBottomLabelFont(fontProp->getFontInformation());
         }
         tempProp = containerProp->getProperty("Left font");
         fontProp = PTR_CAST(ossimFontProperty, tempProp.get());
         if(fontProp)
         {
            setMeterLeftLabelFont(fontProp->getFontInformation());
         }
         tempProp = containerProp->getProperty("Right font");
         fontProp = PTR_CAST(ossimFontProperty, tempProp.get());
         if(fontProp)
         {
            setMeterRightLabelFont(fontProp->getFontInformation());
         }

         // set color property
         tempProp = containerProp->getProperty("Top color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(colorProp)
         {
            setTopMeterLabelColor(colorProp->getColor());
         }
         tempProp = containerProp->getProperty("Bottom color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(colorProp)
         {
            setBottomMeterLabelColor(colorProp->getColor());
         }
         tempProp = containerProp->getProperty("Left color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(colorProp)
         {
            setLeftMeterLabelColor(colorProp->getColor());
         }
         tempProp = containerProp->getProperty("Right color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(colorProp)
         {
            setRightMeterLabelColor(colorProp->getColor());
         }

         // set enable flags
         tempProp = containerProp->getProperty("Top enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setTopMeterLabelFlag(booleanProp->getBoolean());
         }

         tempProp = containerProp->getProperty("Bottom enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setBottomMeterLabelFlag(booleanProp->getBoolean());
         }

         tempProp = containerProp->getProperty("Left enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setLeftMeterLabelFlag(booleanProp->getBoolean());
         }

         tempProp = containerProp->getProperty("Right enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setRightMeterLabelFlag(booleanProp->getBoolean());
         }
         
         tempProp = containerProp->getProperty("Top tick enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setTopMeterTickMarkFlag(booleanProp->getBoolean());
         }
         tempProp = containerProp->getProperty("Bottom tick enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setBottomMeterTickMarkFlag(booleanProp->getBoolean());
         }
         tempProp = containerProp->getProperty("Left tick enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setLeftMeterTickMarkFlag(booleanProp->getBoolean());
         }
         tempProp = containerProp->getProperty("Right tick enabled");
         booleanProp = PTR_CAST(ossimBooleanProperty,
                                tempProp.get());
         if(booleanProp)
         {
            setRightMeterTickMarkFlag(booleanProp->getBoolean());
         }
      }
   }
   else if(property->getName() == "Meter grid properties")
   {
      ossimContainerProperty* containerProp = PTR_CAST(ossimContainerProperty,
                                                       property.get());
      if(containerProp)
      {
         tempProp = containerProp->getProperty("Grid type");
         if(tempProp.valid())
         {
            ossimString value = tempProp->valueToString();

            if(value == "none")
            {
               setMeterGridType(OSSIM_GRID_NONE);
            }
            else if(value == "line")
            {
               setMeterGridType(OSSIM_GRID_LINE);
            }
            else if(value == "reseaux")
            {
               setMeterGridType(OSSIM_GRID_RESEAUX);
            }
         }
         tempProp = containerProp->getProperty("Horizontal spacing");
         if(tempProp.valid())
         {
            setMeterSpacingX(tempProp->valueToString().toDouble());
         }

         tempProp = containerProp->getProperty("Vertical spacing");
         if(tempProp.valid())
         {
            setMeterSpacingY(tempProp->valueToString().toDouble());
         }

         tempProp = containerProp->getProperty("Color");
         colorProp = PTR_CAST(ossimColorProperty, tempProp.get());
         if(tempProp.valid())
         {
            setMeterGridColor(colorProp->getColor());
         }
      }
   }
   else if(property->getName() == "Title properties")
   {
      ossimContainerProperty* container = PTR_CAST(ossimContainerProperty,
                                                   property.get());
      if(container)
      {
         ossimProperty* titleProp = container->getProperty("Title").get();
         if(titleProp)
         {
            setTitle(titleProp->valueToString());
         }
         ossimFontProperty* fontProp = PTR_CAST(ossimFontProperty,
                                                container->getProperty("Font").get());
         ossimColorProperty* colorProp = PTR_CAST(ossimColorProperty,
                                                  container->getProperty("Color").get());

         if(fontProp)
         {
            setTitleFont(fontProp->getFontInformation());
         }
         if(colorProp)
         {
            setTitleColor(colorProp->getColor());
         }
      }
   }
   else if(property->getName() == "Viewport properties")
   {
      ossimContainerProperty* container = PTR_CAST(ossimContainerProperty,
                                                   property.get());
      if(container)
      {
         int w = -1;
         int h = -1;
         tempProp = container->getProperty("Width");
         if(tempProp.valid())
         {
            w = tempProp->valueToString().toInt32();
         }
         tempProp = container->getProperty("Height");
         if(tempProp.valid())
         {
            h = tempProp->valueToString().toInt32();
         }
         setViewWidthHeight(ossimIpt(w, h));
      }
   }
   else if(property->getName() == "Border color")
   {
      colorProp = PTR_CAST(ossimColorProperty,
                           property.get());
      if(colorProp)
      {
         setBorderColor(colorProp->getColor());
      }
   }
   else if(property->getName() == "Border size")
   {
      ossimContainerProperty* container = PTR_CAST(ossimContainerProperty,
                                                   property.get());
      if(container)
      {
         tempProp = container->getProperty("Top");
         if(tempProp.valid())
         {
            setTopBorderLength(tempProp->valueToString().toInt32());
            
         }
         tempProp = container->getProperty("Bottom");
         if(tempProp.valid())
         {
            setBottomBorderLength(tempProp->valueToString().toInt32());
            
         }
         tempProp = container->getProperty("Left");
         if(tempProp.valid())
         {
            setLeftBorderLength(tempProp->valueToString().toInt32());
            
         }
         tempProp = container->getProperty("Right");
         if(tempProp.valid())
         {
            setRightBorderLength(tempProp->valueToString().toInt32());
         }
      }
   }
   else
   {
      ossimAnnotationSource::setProperty(property);
   }
}

ossimRefPtr<ossimProperty> ossimMapCompositionSource::getProperty(const ossimString& name)const
{
   if(name == "Viewport properties")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);

      container->addChild(new ossimNumericProperty("Width",
                                                   ossimString::toString(theViewWidthHeight.x),
                                                   -1,1.0/DBL_EPSILON));
      container->addChild(new ossimNumericProperty("Height",
                                                   ossimString::toString(theViewWidthHeight.y),
                                                   -1,1.0/DBL_EPSILON));
      container->setCacheRefreshBit();
      return container;
   }
   else if(name == "Geographic label properties")
   {

      ossimContainerProperty* container = new ossimContainerProperty(name);
      
      container->addChild(new ossimFontProperty("Top font",
                                                theGeographicTopLabelFontInfo));
      container->addChild(new ossimColorProperty("Top color",
                                                 theTopGeographicLabelColor));
      container->addChild(new ossimBooleanProperty("Top enabled",
                                                   theTopGeographicLabelFlag));
      container->addChild(new ossimBooleanProperty("Top tick enabled",
                                                   theTopGeographicTickFlag));
      
      container->addChild(new ossimFontProperty("Bottom font",
                                                theGeographicBottomLabelFontInfo));
      container->addChild(new ossimColorProperty("Bottom color",
                                                 theBottomGeographicLabelColor));
      container->addChild(new ossimBooleanProperty("Bottom enabled",
                                                   theBottomGeographicLabelFlag));
      container->addChild(new ossimBooleanProperty("Bottom tick enabled",
                                                   theBottomGeographicTickFlag));
      
      container->addChild(new ossimFontProperty("Left font",
                                                theGeographicLeftLabelFontInfo));
      container->addChild(new ossimColorProperty("Left color",
                                                 theLeftGeographicLabelColor));
      container->addChild(new ossimBooleanProperty("Left enabled",
                                                   theLeftGeographicLabelFlag));
      container->addChild(new ossimBooleanProperty("Left tick enabled",
                                                   theLeftGeographicTickFlag));
      
      container->addChild(new ossimFontProperty("Right font",
                                                theGeographicRightLabelFontInfo));
      container->addChild(new ossimColorProperty("Right color",
                                                 theRightGeographicLabelColor));
      container->addChild(new ossimBooleanProperty("Right enabled",
                                                   theRightGeographicLabelFlag));
      container->addChild(new ossimBooleanProperty("Right tick enabled",
                                                   theRightGeographicTickFlag));
      container->setCacheRefreshBit();
      return container;
   }
   else if(name == "Geographic grid properties")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      std::vector<ossimString> gridTypeConstraints;
      ossimString value = GRID_TYPE_ENUM_NAMES[(int)theGeographicGridType];

      gridTypeConstraints.push_back("none");
      gridTypeConstraints.push_back("line");
      gridTypeConstraints.push_back("reseaux");
      
      container->addChild(new ossimStringProperty("Grid type",
                                                  value,
                                                  false,
                                                  gridTypeConstraints));
      container->addChild(new ossimColorProperty("Color",
                                                 theGeographicGridColor));

      container->addChild(new ossimNumericProperty("Horizontal spacing",
                                                   ossimString::toString(theGeographicSpacing.x),
                                                   0.0, 180.0));

      container->addChild(new ossimNumericProperty("Vertical spacing",
                                                   ossimString::toString(theGeographicSpacing.y),
                                                   0.0, 180.0));
                                                   
      container->setCacheRefreshBit();
      return container;
   }
   else if(name == "Meter label properties")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      
      container->addChild(new ossimFontProperty("Top font",
                                                theMeterTopLabelFontInfo));
      container->addChild(new ossimColorProperty("Top color",
                                                 theTopMeterLabelColor));
      container->addChild(new ossimBooleanProperty("Top enabled",
                                                   theTopMeterLabelFlag));
      container->addChild(new ossimBooleanProperty("Top tick enabled",
                                                   theTopMeterTickFlag));
      
      container->addChild(new ossimFontProperty("Bottom font",
                                                theMeterBottomLabelFontInfo));
      container->addChild(new ossimColorProperty("Bottom color",
                                                 theBottomMeterLabelColor));
      container->addChild(new ossimBooleanProperty("Bottom enabled",
                                                   theBottomMeterLabelFlag));
      container->addChild(new ossimBooleanProperty("Bottom tick enabled",
                                                   theBottomMeterTickFlag));
      
      container->addChild(new ossimFontProperty("Left font",
                                                theMeterLeftLabelFontInfo));
      container->addChild(new ossimColorProperty("Left color",
                                                 theLeftMeterLabelColor));
      container->addChild(new ossimBooleanProperty("Left enabled",
                                                   theLeftMeterLabelFlag));
      container->addChild(new ossimBooleanProperty("Left tick enabled",
                                                   theLeftMeterTickFlag));
      
      container->addChild(new ossimFontProperty("Right font",
                                                theMeterRightLabelFontInfo));
      container->addChild(new ossimColorProperty("Right color",
                                                 theRightMeterLabelColor));
      container->addChild(new ossimBooleanProperty("Right enabled",
                                                   theRightMeterLabelFlag));
      container->addChild(new ossimBooleanProperty("Right tick enabled",
                                                   theRightMeterTickFlag));
      
      container->setCacheRefreshBit();
      return container;
   }
   else if(name == "Meter grid properties")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      std::vector<ossimString> gridTypeConstraints;
      ossimString value = GRID_TYPE_ENUM_NAMES[(int)theMeterGridType];

      gridTypeConstraints.push_back("none");
      gridTypeConstraints.push_back("line");
      gridTypeConstraints.push_back("reseaux");
      
      container->addChild(new ossimStringProperty("Grid type",
                                                  value,
                                                  false,
                                                  gridTypeConstraints));
      container->addChild(new ossimColorProperty("Color",
                                                 theMeterGridColor));
      
      container->addChild(new ossimNumericProperty("Horizontal spacing",
                                                   ossimString::toString(theMeterSpacing.x),
                                                   0.0, 1.0/DBL_EPSILON));

      container->addChild(new ossimNumericProperty("Vertical spacing",
                                                   ossimString::toString(theMeterSpacing.y),
                                                   0.0, 1.0/DBL_EPSILON));
      container->setCacheRefreshBit();
      return container;
   }
   else if(name == "Title properties")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      container->addChild(new ossimTextProperty("Title",
                                                theTitleString));
      container->addChild(new ossimFontProperty("Font",
                                                theTitleFontInfo));
      container->addChild(new ossimColorProperty("Color",
                                                 theTitleColor));
      
      container->setCacheRefreshBit();
      return container;
   }
   else if(name == "Border size")
   {
      ossimContainerProperty* container = new ossimContainerProperty(name);
      
      container->addChild(new ossimNumericProperty("Top",
                                                   ossimString::toString(theTopBorderLength),
                                                   0,10000));
      container->addChild(new ossimNumericProperty("Bottom",
                                                   ossimString::toString(theBottomBorderLength),
                                                   0,10000));
      container->addChild(new ossimNumericProperty("Left",
                                                   ossimString::toString(theLeftBorderLength),
                                                   0,10000));
      container->addChild(new ossimNumericProperty("Right",
                                                   ossimString::toString(theRightBorderLength),
                                                   0,10000));
      container->setCacheRefreshBit();
      return container;
   }
   else if(name == "Border color")
   {
      ossimColorProperty* colorProp = new ossimColorProperty(name,
                                                             theBorderColor);
      colorProp->setCacheRefreshBit();
      
      return colorProp;
   }
   else if(name == "Viewport properties")
   {
      
   }
   return ossimAnnotationSource::getProperty(name);
}

void ossimMapCompositionSource::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
   ossimAnnotationSource::getPropertyNames(propertyNames);

   
   propertyNames.push_back("Title properties");
   propertyNames.push_back("Border size");
   propertyNames.push_back("Border color");
   propertyNames.push_back("Viewport properties");
   propertyNames.push_back("Geographic label properties");
   propertyNames.push_back("Geographic grid properties");
   propertyNames.push_back("Meter label properties");
   propertyNames.push_back("Meter grid properties");
}

bool ossimMapCompositionSource::saveState(ossimKeywordlist& kwl,
                                          const char* prefix)const
{
   kwl.add(prefix,
           VIEW_WIDTH_KW,
           theViewWidthHeight.x,
           true);
   
   kwl.add(prefix,
           VIEW_HEIGHT_KW,
           theViewWidthHeight.x,
           true);
   
   kwl.add(prefix,
           METER_GRID_SPACING_X_KW,
           theMeterSpacing.x,
           true);

   kwl.add(prefix,
           METER_GRID_SPACING_Y_KW,
           theMeterSpacing.y,
           true);
   
   kwl.add(prefix,
           GEO_GRID_SPACING_LON_KW,
           theGeographicSpacing.lon,
           true);

   kwl.add(prefix,
           GEO_GRID_SPACING_LAT_KW,
           theGeographicSpacing.lat,
           true);

   ossimString meterGridType = "line";
   if(theMeterGridType == OSSIM_GRID_NONE)
   {
      meterGridType = "none";
   }
   else if(theMeterGridType == OSSIM_GRID_RESEAUX)
   {
      meterGridType = "reseaux";
   }
   
   kwl.add(prefix,
           METER_GRID_TYPE_KW,
           meterGridType.c_str(),
           true);

   ossimString geoGridType = "line";
   if(theGeographicGridType == OSSIM_GRID_NONE)
   {
      geoGridType = "none";
   }
   else if(theGeographicGridType == OSSIM_GRID_RESEAUX)
   {
      geoGridType = "reseaux";
   }

   kwl.add(prefix,
           GEO_GRID_TYPE_KW,
           geoGridType.c_str(),
           true);

   kwl.add(prefix,
           TOP_BORDER_LENGTH_KW,
           theTopBorderLength,
           true);
   
   kwl.add(prefix,
           BOTTOM_BORDER_LENGTH_KW,
           theBottomBorderLength,
           true);

   kwl.add(prefix,
           LEFT_BORDER_LENGTH_KW,
           theLeftBorderLength,
           true);

   kwl.add(prefix,
           RIGHT_BORDER_LENGTH_KW,
           theRightBorderLength,
           true);

   kwl.add(prefix,
           BORDER_COLOR_KW,
           (ossimString::toString(theBorderColor.getR()) +" "+
            ossimString::toString(theBorderColor.getG()) +" "+
            ossimString::toString(theBorderColor.getB())).c_str(),
           true);

   kwl.add(prefix,
           GEO_GRID_COLOR_KW,
           (ossimString::toString(theGeographicGridColor.getR()) +" "+
            ossimString::toString(theGeographicGridColor.getG()) +" "+
            ossimString::toString(theGeographicGridColor.getB())).c_str(),
           true);

   kwl.add(prefix,
           METER_GRID_COLOR_KW,
           (ossimString::toString(theMeterGridColor.getR()) +" "+
            ossimString::toString(theMeterGridColor.getG()) +" "+
            ossimString::toString(theMeterGridColor.getB())).c_str(),
           true);

   kwl.add(prefix,
           TITLE_STRING_KW,
           theTitleString.c_str(),
           true);
   
   kwl.add(prefix,
           TITLE_COLOR_KW,
           (ossimString::toString(theTitleColor.getR()) +" "+
            ossimString::toString(theTitleColor.getG()) +" "+
            ossimString::toString(theTitleColor.getB())).c_str(),
           true);

   kwl.add(prefix,
           TOP_GEO_LABEL_COLOR_KW,
           (ossimString::toString(theTopGeographicLabelColor.getR()) +" "+
            ossimString::toString(theTopGeographicLabelColor.getG()) +" "+
            ossimString::toString(theTopGeographicLabelColor.getB())).c_str(),
           true);

   kwl.add(prefix,
           BOTTOM_GEO_LABEL_COLOR_KW,
           (ossimString::toString(theBottomGeographicLabelColor.getR()) +" "+
            ossimString::toString(theBottomGeographicLabelColor.getG()) +" "+
            ossimString::toString(theBottomGeographicLabelColor.getB())).c_str(),
           true);
   kwl.add(prefix,
           LEFT_GEO_LABEL_COLOR_KW,
           (ossimString::toString(theLeftGeographicLabelColor.getR()) +" "+
            ossimString::toString(theLeftGeographicLabelColor.getG()) +" "+
            ossimString::toString(theLeftGeographicLabelColor.getB())).c_str(),
           true);
   kwl.add(prefix,
           RIGHT_GEO_LABEL_COLOR_KW,
           (ossimString::toString(theRightGeographicLabelColor.getR()) +" "+
            ossimString::toString(theRightGeographicLabelColor.getG()) +" "+
            ossimString::toString(theRightGeographicLabelColor.getB())).c_str(),
           true);

   kwl.add(prefix,
           TOP_GEO_LABEL_FORMAT_KW,
           theTopGeographicFormat,
           true);

   kwl.add(prefix,
           BOTTOM_GEO_LABEL_FORMAT_KW,
           theBottomGeographicFormat,
           true);
   kwl.add(prefix,
           LEFT_GEO_LABEL_FORMAT_KW,
           theLeftGeographicFormat,
           true);
   kwl.add(prefix,
           RIGHT_GEO_LABEL_FORMAT_KW,
           theRightGeographicFormat,
           true);

   kwl.add(prefix,
           TOP_GEO_LABEL_FLAG_KW,
           (int)theTopGeographicLabelFlag,
           true);

   kwl.add(prefix,
           BOTTOM_GEO_LABEL_FLAG_KW,
           (int)theBottomGeographicLabelFlag,
           true);

   kwl.add(prefix,
           LEFT_GEO_LABEL_FLAG_KW,
           (int)theLeftGeographicLabelFlag,
           true);

   kwl.add(prefix,
           RIGHT_GEO_LABEL_FLAG_KW,
           (int)theRightGeographicLabelFlag,
           true);

   kwl.add(prefix,
           TOP_METER_LABEL_COLOR_KW,
           (ossimString::toString(theTopMeterLabelColor.getR()) +" "+
            ossimString::toString(theTopMeterLabelColor.getG()) +" "+
            ossimString::toString(theTopMeterLabelColor.getB())).c_str(),
           true);

   kwl.add(prefix,
           BOTTOM_METER_LABEL_COLOR_KW,
           (ossimString::toString(theBottomMeterLabelColor.getR()) +" "+
            ossimString::toString(theBottomMeterLabelColor.getG()) +" "+
            ossimString::toString(theBottomMeterLabelColor.getB())).c_str(),
           true);
   kwl.add(prefix,
           LEFT_METER_LABEL_COLOR_KW,
           (ossimString::toString(theLeftMeterLabelColor.getR()) +" "+
            ossimString::toString(theLeftMeterLabelColor.getG()) +" "+
            ossimString::toString(theLeftMeterLabelColor.getB())).c_str(),
           true);
   kwl.add(prefix,
           RIGHT_METER_LABEL_COLOR_KW,
           (ossimString::toString(theRightMeterLabelColor.getR()) +" "+
            ossimString::toString(theRightMeterLabelColor.getG()) +" "+
            ossimString::toString(theRightMeterLabelColor.getB())).c_str(),
           true);


   kwl.add(prefix,
           TOP_METER_LABEL_FLAG_KW,
           (int)theTopMeterLabelFlag,
           true);

   kwl.add(prefix,
           BOTTOM_METER_LABEL_FLAG_KW,
           (int)theBottomMeterLabelFlag,
           true);

   kwl.add(prefix,
           LEFT_METER_LABEL_FLAG_KW,
           (int)theLeftMeterLabelFlag,
           true);

   kwl.add(prefix,
           RIGHT_METER_LABEL_FLAG_KW,
           (int)theRightMeterLabelFlag,
           true);
   
   theGeographicTopLabelFontInfo.saveState(kwl, (ossimString(prefix) + ossimString(TOP_GEO_LABEL_FONT_KW) + ".").c_str());
   theGeographicBottomLabelFontInfo.saveState(kwl, (ossimString(prefix) + ossimString(BOTTOM_GEO_LABEL_FONT_KW) + ".").c_str());
   theGeographicLeftLabelFontInfo.saveState(kwl, (ossimString(prefix) + ossimString(LEFT_GEO_LABEL_FONT_KW) + ".").c_str());
   theGeographicRightLabelFontInfo.saveState(kwl, (ossimString(prefix) + ossimString(RIGHT_GEO_LABEL_FONT_KW) + ".").c_str());

   theMeterTopLabelFontInfo.saveState(kwl, (ossimString(prefix) + ossimString(TOP_METER_LABEL_FONT_KW) + ".").c_str());
   theMeterBottomLabelFontInfo.saveState(kwl, (ossimString(prefix) + ossimString(BOTTOM_METER_LABEL_FONT_KW) + ".").c_str());
   theMeterLeftLabelFontInfo.saveState(kwl, (ossimString(prefix) + ossimString(LEFT_METER_LABEL_FONT_KW) + ".").c_str());
   theMeterRightLabelFontInfo.saveState(kwl, (ossimString(prefix) + ossimString(RIGHT_METER_LABEL_FONT_KW) + ".").c_str());
   
   theTitleFontInfo.saveState(kwl, (ossimString(prefix) + ossimString(TITLE_FONT_KW) + ".").c_str());
   
           
   return ossimImageSource::saveState(kwl, prefix);
   
}

bool ossimMapCompositionSource::loadState(const ossimKeywordlist& kwl,
                                          const char* prefix)
{
   const char* viewWidth            = kwl.find(prefix, VIEW_WIDTH_KW);
   const char* viewHeight           = kwl.find(prefix, VIEW_HEIGHT_KW);
   const char* meterGridSpacingX    = kwl.find(prefix, METER_GRID_SPACING_X_KW);
   const char* meterGridSpacingY    = kwl.find(prefix, METER_GRID_SPACING_Y_KW);
   const char* geoGridSpacingLon    = kwl.find(prefix, GEO_GRID_SPACING_LON_KW);
   const char* geoGridSpacingLat    = kwl.find(prefix, GEO_GRID_SPACING_LAT_KW);
   const char* meterGridType        = kwl.find(prefix, METER_GRID_TYPE_KW);
   const char* geoGridType          = kwl.find(prefix, GEO_GRID_TYPE_KW);
   const char* topBorderLength      = kwl.find(prefix, TOP_BORDER_LENGTH_KW);
   const char* bottomBorderLength   = kwl.find(prefix, BOTTOM_BORDER_LENGTH_KW);
   const char* leftBorderLength     = kwl.find(prefix, LEFT_BORDER_LENGTH_KW);
   const char* rightBorderLength    = kwl.find(prefix, RIGHT_BORDER_LENGTH_KW);
   
   const char* borderColor          = kwl.find(prefix, BORDER_COLOR_KW);
   const char* geoGridColor         = kwl.find(prefix, GEO_GRID_COLOR_KW);
   const char* meterGridColor       = kwl.find(prefix, METER_GRID_COLOR_KW);
   const char* titleString          = kwl.find(prefix, TITLE_STRING_KW);
   const char* titleColor           = kwl.find(prefix, TITLE_COLOR_KW);

   const char* topGeoLabelColor     = kwl.find(prefix, TOP_GEO_LABEL_COLOR_KW);
   const char* bottomGeoLabelColor  = kwl.find(prefix, BOTTOM_GEO_LABEL_COLOR_KW);
   const char* leftGeoLabelColor    = kwl.find(prefix, LEFT_GEO_LABEL_COLOR_KW);
   const char* rightGeoLabelColor   = kwl.find(prefix, RIGHT_GEO_LABEL_COLOR_KW);

   const char* topMeterLabelColor     = kwl.find(prefix, TOP_METER_LABEL_COLOR_KW);
   const char* bottomMeterLabelColor  = kwl.find(prefix, BOTTOM_METER_LABEL_COLOR_KW);
   const char* leftMeterLabelColor    = kwl.find(prefix, LEFT_METER_LABEL_COLOR_KW);
   const char* rightMeterLabelColor   = kwl.find(prefix, RIGHT_METER_LABEL_COLOR_KW);
   
   const char* topGeoLabelFormat    = kwl.find(prefix, TOP_GEO_LABEL_FORMAT_KW);
   const char* bottomGeoLabelFormat = kwl.find(prefix, BOTTOM_GEO_LABEL_FORMAT_KW);
   const char* leftGeoLabelFormat   = kwl.find(prefix, LEFT_GEO_LABEL_FORMAT_KW);
   const char* rightGeoLabelFormat  = kwl.find(prefix, RIGHT_GEO_LABEL_FORMAT_KW);

   const char* topGeoLabelFlag      = kwl.find(prefix, TOP_GEO_LABEL_FLAG_KW);
   const char* bottomGeoLabelFlag   = kwl.find(prefix, BOTTOM_GEO_LABEL_FLAG_KW);
   const char* leftGeoLabelFlag     = kwl.find(prefix, LEFT_GEO_LABEL_FLAG_KW);
   const char* rightGeoLabelFlag    = kwl.find(prefix, RIGHT_GEO_LABEL_FLAG_KW);

   const char* topMeterLabelFlag    = kwl.find(prefix, TOP_METER_LABEL_FLAG_KW);
   const char* bottomMeterLabelFlag = kwl.find(prefix, BOTTOM_METER_LABEL_FLAG_KW);
   const char* leftMeterLabelFlag   = kwl.find(prefix, LEFT_METER_LABEL_FLAG_KW);
   const char* rightMeterLabelFlag  = kwl.find(prefix, RIGHT_METER_LABEL_FLAG_KW);

   deleteFixedAnnotations();
   theViewWidthHeight.makeNan();
   
   if(viewWidth)
   {
      theViewWidthHeight.x = ossimString(viewWidth).toLong();
   }
   if(viewHeight)
   {
      theViewWidthHeight.y = ossimString(viewHeight).toLong();
   }
   
   if(meterGridSpacingX)
   {
      theMeterSpacing.x = ossimString(meterGridSpacingX).toDouble();
   }

   if(meterGridSpacingY)
   {
      theMeterSpacing.y = ossimString(meterGridSpacingY).toDouble();
   }
   if(geoGridSpacingLon)
   {
      theGeographicSpacing.lon = ossimString(geoGridSpacingLon).toDouble();
   }
   if(geoGridSpacingLat)
   {
      theGeographicSpacing.lat = ossimString(geoGridSpacingLat).toDouble();
   }
   if(meterGridType)
   {
      ossimString type = ossimString(meterGridType).trim().downcase();
      if(type == "reseaux")
      {
         theMeterGridType = OSSIM_GRID_RESEAUX;
      }
      else if(type == "line")
      {
         theMeterGridType = OSSIM_GRID_LINE;
      }
      else
      {
         theMeterGridType = OSSIM_GRID_NONE;
      }
   }

   if(geoGridType)
   {
      ossimString type = ossimString(geoGridType).trim().downcase();
      if(type == "reseaux")
      {
         theGeographicGridType = OSSIM_GRID_RESEAUX;
      }
      else if(type == "line")
      {
         theGeographicGridType = OSSIM_GRID_LINE;
      }
      else
      {
         theGeographicGridType = OSSIM_GRID_NONE;
      }
   }

   if(topBorderLength)
   {
      theTopBorderLength = ossimString(topBorderLength).toULong();
   }

   if(bottomBorderLength)
   {
      theBottomBorderLength = ossimString(bottomBorderLength).toULong();
   }
   if(leftBorderLength)
   {
      theLeftBorderLength = ossimString(leftBorderLength).toULong();
   }
   if(rightBorderLength)
   {
      theRightBorderLength = ossimString(rightBorderLength).toULong();
   }

   if(borderColor)
   {
      istringstream in(borderColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;

      theBorderColor = ossimRgbVector((ossim_uint8)r.toUInt32(), 
                                      (ossim_uint8)g.toUInt32(), 
                                      (ossim_uint8)b.toUInt32());
   }

   if(geoGridColor)
   {
      istringstream in(geoGridColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      theGeographicGridColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }

   if(meterGridColor)
   {
      istringstream in(meterGridColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      theMeterGridColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());      
   }

   if(titleString)
   {
      theTitleString = titleString;
   }
   
   if(titleColor)
   {
      istringstream in(titleColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      
      theTitleColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }

   if(topGeoLabelColor)
   {
      istringstream in(topGeoLabelColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      theTopGeographicLabelColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }

   if(bottomGeoLabelColor)
   {
      istringstream in(bottomGeoLabelColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      theBottomGeographicLabelColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }

   if(leftGeoLabelColor)
   {
      istringstream in(leftGeoLabelColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;

      theLeftGeographicLabelColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }

   if(rightGeoLabelColor)
   {
      istringstream in(rightGeoLabelColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      theRightGeographicLabelColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }


/////
   if(topMeterLabelColor)
   {
      istringstream in(topMeterLabelColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      theTopMeterLabelColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }

   if(bottomMeterLabelColor)
   {
      istringstream in(bottomMeterLabelColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      theBottomMeterLabelColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }

   if(leftMeterLabelColor)
   {
      istringstream in(leftMeterLabelColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      theLeftMeterLabelColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }

   if(rightMeterLabelColor)
   {
      istringstream in(rightMeterLabelColor);

      ossimString r;
      ossimString g;
      ossimString b;
      in >> r >> g >> b;
      
      theRightMeterLabelColor = ossimRgbVector((ossim_uint8)r.toUInt32(), (ossim_uint8)g.toUInt32(), (ossim_uint8)b.toUInt32());
   }

   if(topGeoLabelFormat)
   {
      theTopGeographicFormat = topGeoLabelFormat;
   }
   
   if(bottomGeoLabelFormat)
   {
      theBottomGeographicFormat = bottomGeoLabelFormat;
   }
   
   if(leftGeoLabelFormat)
   {
      theLeftGeographicFormat = leftGeoLabelFormat;
   }
   
   if(rightGeoLabelFormat)
   {
      theRightGeographicFormat = rightGeoLabelFormat;
   }

   if(topGeoLabelFlag)
   {
      theTopGeographicLabelFlag = ossimString(topGeoLabelFlag).toBool();
   }
   if(bottomGeoLabelFlag)
   {
      theBottomGeographicLabelFlag = ossimString(bottomGeoLabelFlag).toBool();
   }
   if(leftGeoLabelFlag)
   {
      theLeftGeographicLabelFlag = ossimString(leftGeoLabelFlag).toBool();
   }
   if(rightGeoLabelFlag)
   {
      theRightGeographicLabelFlag = ossimString(rightGeoLabelFlag).toBool();
   }

   if(topMeterLabelFlag)
   {
      theTopMeterLabelFlag = ossimString(topMeterLabelFlag).toBool();
   }
   if(bottomMeterLabelFlag)
   {
      theBottomMeterLabelFlag = ossimString(bottomMeterLabelFlag).toBool();
   }
   if(leftMeterLabelFlag)
   {
      theLeftMeterLabelFlag = ossimString(leftMeterLabelFlag).toBool();
   }
   if(rightMeterLabelFlag)
   {
      theRightMeterLabelFlag = ossimString(rightMeterLabelFlag).toBool();
   }
   
   theGeographicTopLabelFontInfo.loadState(kwl, (ossimString(prefix) + ossimString(TOP_GEO_LABEL_FONT_KW) + ".").c_str());
   theGeographicBottomLabelFontInfo.loadState(kwl, (ossimString(prefix) + ossimString(BOTTOM_GEO_LABEL_FONT_KW) + ".").c_str());
   theGeographicLeftLabelFontInfo.loadState(kwl, (ossimString(prefix) + ossimString(LEFT_GEO_LABEL_FONT_KW) + ".").c_str());
   theGeographicRightLabelFontInfo.loadState(kwl, (ossimString(prefix) + ossimString(RIGHT_GEO_LABEL_FONT_KW) + ".").c_str());

   theMeterTopLabelFontInfo.loadState(kwl, (ossimString(prefix) + ossimString(TOP_METER_LABEL_FONT_KW) + ".").c_str());
   theMeterBottomLabelFontInfo.loadState(kwl, (ossimString(prefix) + ossimString(BOTTOM_METER_LABEL_FONT_KW) + ".").c_str());
   theMeterLeftLabelFontInfo.loadState(kwl, (ossimString(prefix) + ossimString(LEFT_METER_LABEL_FONT_KW) + ".").c_str());
   theMeterRightLabelFontInfo.loadState(kwl, (ossimString(prefix) + ossimString(RIGHT_METER_LABEL_FONT_KW) + ".").c_str());
   
   theTitleFontInfo.loadState(kwl, (ossimString(prefix) + ossimString(TITLE_FONT_KW) + ".").c_str());
   theTitleFont = ossimFontFactoryRegistry::instance()->createFont(theTitleFontInfo);
   theGeographicTopLabelFont = ossimFontFactoryRegistry::instance()->createFont(theGeographicTopLabelFontInfo);
   theGeographicBottomLabelFont = ossimFontFactoryRegistry::instance()->createFont(theGeographicBottomLabelFontInfo);
   theGeographicLeftLabelFont = ossimFontFactoryRegistry::instance()->createFont(theGeographicLeftLabelFontInfo);
   theGeographicRightLabelFont = ossimFontFactoryRegistry::instance()->createFont(theGeographicRightLabelFontInfo);

   theMeterTopLabelFont = ossimFontFactoryRegistry::instance()->createFont(theMeterTopLabelFontInfo);
   theMeterBottomLabelFont = ossimFontFactoryRegistry::instance()->createFont(theMeterBottomLabelFontInfo);
   theMeterLeftLabelFont = ossimFontFactoryRegistry::instance()->createFont(theMeterLeftLabelFontInfo);
   theMeterRightLabelFont = ossimFontFactoryRegistry::instance()->createFont(theMeterRightLabelFontInfo);
   
   vector<ossimFontInformation> info;

   
   if(!theTitleFont)
   {
      theTitleFont = (ossimFont*)ossimFontFactoryRegistry::instance()->getDefaultFont()->dup();
      theTitleFont->getFontInformation(info);
      theTitleFontInfo = info[0];
   }
   if(!theGeographicTopLabelFont)
   {
      info.clear();
      theGeographicTopLabelFont = (ossimFont*)ossimFontFactoryRegistry::instance()->getDefaultFont()->dup();
      theGeographicTopLabelFont->getFontInformation(info);
      theGeographicTopLabelFontInfo = info[0];
   }
   if(!theGeographicBottomLabelFont)
   {
      info.clear();
      theGeographicBottomLabelFont = (ossimFont*)ossimFontFactoryRegistry::instance()->getDefaultFont()->dup();
      theGeographicBottomLabelFont->getFontInformation(info);
      theGeographicBottomLabelFontInfo = info[0];
   }
   if(!theGeographicLeftLabelFont)
   {
      info.clear();
      theGeographicLeftLabelFont = (ossimFont*)ossimFontFactoryRegistry::instance()->getDefaultFont()->dup();
      theGeographicLeftLabelFont->getFontInformation(info);
      theGeographicLeftLabelFontInfo = info[0];
   }
   if(!theGeographicRightLabelFont)
   {
      info.clear();
      theGeographicRightLabelFont = (ossimFont*)ossimFontFactoryRegistry::instance()->getDefaultFont()->dup();
      theGeographicRightLabelFont->getFontInformation(info);
      theGeographicRightLabelFontInfo = info[0];
   }

   if(!theMeterTopLabelFont)
   {
      info.clear();
      theMeterTopLabelFont = (ossimFont*)ossimFontFactoryRegistry::instance()->getDefaultFont()->dup();
      theMeterTopLabelFont->getFontInformation(info);
      theMeterTopLabelFontInfo = info[0];
   }
   if(!theMeterBottomLabelFont)
   {
      info.clear();
      theMeterBottomLabelFont = (ossimFont*)ossimFontFactoryRegistry::instance()->getDefaultFont()->dup();
      theMeterBottomLabelFont->getFontInformation(info);
      theMeterBottomLabelFontInfo = info[0];
   }
   if(!theMeterLeftLabelFont)
   {
      info.clear();
      theMeterLeftLabelFont = (ossimFont*)ossimFontFactoryRegistry::instance()->getDefaultFont()->dup();
      theMeterLeftLabelFont->getFontInformation(info);
      theMeterLeftLabelFontInfo = info[0];
   }
   if(!theMeterRightLabelFont)
   {
      info.clear();
      theMeterRightLabelFont = (ossimFont*)ossimFontFactoryRegistry::instance()->getDefaultFont()->dup();
      theMeterRightLabelFont->getFontInformation(info);
      theMeterRightLabelFontInfo = info[0];
   }

   bool result =  ossimAnnotationSource::loadState(kwl,
                                                   prefix);
   layoutAnnotations();

   return result;
}

ossimScalarType ossimMapCompositionSource::getOutputScalarType() const
{
   return OSSIM_UCHAR;
}

ossim_uint32 ossimMapCompositionSource::getNumberOfOutputBands() const
{
   return 3;
}

ossimIpt ossimMapCompositionSource::getViewWidthHeight()const
{
   return theViewWidthHeight;
}

void ossimMapCompositionSource::getViewWidthHeight(ossimIpt& widthHeight)const
{
   widthHeight = theViewWidthHeight;
}

void ossimMapCompositionSource::setViewWidthHeight(const ossimIpt& widthHeight)
{
   theViewWidthHeight = widthHeight;
}

ossim_int32 ossimMapCompositionSource::getTopBorderLength()const
{
   return theTopBorderLength;
}

ossim_int32 ossimMapCompositionSource::getBottomBorderLength()const
{
   return theBottomBorderLength;
}

ossim_int32 ossimMapCompositionSource::getLeftBorderLength()const
{
   return theLeftBorderLength;
}

ossim_int32 ossimMapCompositionSource::getRightBorderLength()const
{
   return theRightBorderLength;
}

void ossimMapCompositionSource::setTopBorderLength(ossim_int32 length)
{
   theTopBorderLength = length;
}

void ossimMapCompositionSource::setBottomBorderLength(ossim_int32 length)
{
   theBottomBorderLength = length;
}

void ossimMapCompositionSource::setLeftBorderLength(ossim_int32 length)
{
   theLeftBorderLength = length;
}

void ossimMapCompositionSource::setRightBorderLength(ossim_int32 length)
{
   theRightBorderLength = length;
}

void ossimMapCompositionSource::setGeographicSpacingLat(double value)
{
   theGeographicSpacing.lat = value;
}

void ossimMapCompositionSource::setGeographicSpacingLon(double value)
{
   theGeographicSpacing.lon = value;
}

void ossimMapCompositionSource::setMeterSpacingX(double value)
{
   theMeterSpacing.x = value;
}

void ossimMapCompositionSource::setMeterSpacingY(double value)
{
   theMeterSpacing.y = value;
}

void ossimMapCompositionSource::setMeterSpacing(double x, double y)
{
   theMeterSpacing.x = x;
   theMeterSpacing.y = y;
}

void ossimMapCompositionSource::setGeographicSpacing(double lat, double lon)
{
   theGeographicSpacing.lat = lat;
   theGeographicSpacing.lon = lon;
}

ossimDpt ossimMapCompositionSource::getGeographicSpacing()const
{
   return theGeographicSpacing;
}

ossimDpt ossimMapCompositionSource::getMeterSpacing()const
{
   return theMeterSpacing;
}

ossimString ossimMapCompositionSource::getTopGeographicLabelFormat()const
{
   return theTopGeographicFormat;
}

ossimString ossimMapCompositionSource::getBottomGeographicLabelFormat()const
{
   return theBottomGeographicFormat;
}

ossimString ossimMapCompositionSource::getLeftGeographicLabelFormat()const
{
   return theLeftGeographicFormat;
}

ossimString ossimMapCompositionSource::getRightGeographicLabelFormat()const
{
   return theRightGeographicFormat;
}

ossimString ossimMapCompositionSource::getTitle()const
{
   return theTitleString;
}

ossimRgbVector ossimMapCompositionSource::getTitleColor()const
{
   return theTitleColor;
}

void ossimMapCompositionSource::setTitleColor(const ossimRgbVector& color)
{
   if(theTitleColor != color)
   {
      theTitleColor = color;
   }
}

void ossimMapCompositionSource::setTitle(const ossimString& s)
{
   if(theTitleString != s)
   {
      theTitleString = s;
   }
         
}

ossimFontInformation ossimMapCompositionSource::getTitleFont()const
{
   return theTitleFontInfo;
}

ossimFontInformation ossimMapCompositionSource::getGeographicTopLabelFontInfo()const
{
   return theGeographicTopLabelFontInfo;
}

ossimFontInformation ossimMapCompositionSource::getGeographicBottomLabelFontInfo()const
{
   return theGeographicBottomLabelFontInfo;
}

ossimFontInformation ossimMapCompositionSource::getGeographicLeftLabelFontInfo()const
{
   return theGeographicLeftLabelFontInfo;
}

ossimFontInformation ossimMapCompositionSource::getGeographicRightLabelFontInfo()const
{
   return theGeographicRightLabelFontInfo;
}

ossimFontInformation ossimMapCompositionSource::getMeterTopLabelFontInfo()const
{
   return theMeterTopLabelFontInfo;
}

ossimFontInformation ossimMapCompositionSource::getMeterBottomLabelFontInfo()const
{
   return theMeterBottomLabelFontInfo;
}

ossimFontInformation ossimMapCompositionSource::getMeterLeftLabelFontInfo()const
{
   return theMeterLeftLabelFontInfo;
}

ossimFontInformation ossimMapCompositionSource::getMeterRightLabelFontInfo()const
{
   return theMeterRightLabelFontInfo;
}
   
ossimRgbVector ossimMapCompositionSource::getBorderColor()const
{
   return theBorderColor;
}

void ossimMapCompositionSource::setBorderColor(const ossimRgbVector& color)
{
   theBorderColor=color;
}

ossimRgbVector ossimMapCompositionSource::getGeographicGridColor()const
{
   return theGeographicGridColor;
}

ossimRgbVector ossimMapCompositionSource::getMeterGridColor()const
{
   return theMeterGridColor;
}

ossimRgbVector ossimMapCompositionSource::getTopGeographicLabelColor()const
{
   return theTopGeographicLabelColor;
}

ossimRgbVector ossimMapCompositionSource::getBottomGeographicLabelColor()const
{
   return theBottomGeographicLabelColor;
}

ossimRgbVector ossimMapCompositionSource::getLeftGeographicLabelColor()const
{
   return theLeftGeographicLabelColor;
}

ossimRgbVector ossimMapCompositionSource::getRightGeographicLabelColor()const
{
   return theRightGeographicLabelColor;
}

void ossimMapCompositionSource::setGeographicGridColor(const ossimRgbVector& color)
{
   theGeographicGridColor = color;
}

void ossimMapCompositionSource::setTopGeographicLabelColor(const ossimRgbVector& color)
{
   theTopGeographicLabelColor = color;
}

void ossimMapCompositionSource::setBottomGeographicLabelColor(const ossimRgbVector& color)
{
   theBottomGeographicLabelColor = color;
}

void ossimMapCompositionSource::setLeftGeographicLabelColor(const ossimRgbVector& color)
{
   theLeftGeographicLabelColor = color;
}

void ossimMapCompositionSource::setRightGeographicLabelColor(const ossimRgbVector& color)
{
   theRightGeographicLabelColor = color;
}

void ossimMapCompositionSource::setGeographicLabelColor(const ossimRgbVector& color)
{
   setTopGeographicLabelColor(color);
   setBottomGeographicLabelColor(color);
   setLeftGeographicLabelColor(color);
   setRightGeographicLabelColor(color);
}

ossimRgbVector ossimMapCompositionSource::getTopMeterLabelColor()const
{
   return theTopMeterLabelColor;
}

ossimRgbVector ossimMapCompositionSource::getBottomMeterLabelColor()const
{
   return theBottomMeterLabelColor;
}

ossimRgbVector ossimMapCompositionSource::getLeftMeterLabelColor()const
{
   return theLeftMeterLabelColor;
}

ossimRgbVector ossimMapCompositionSource::getRightMeterLabelColor()const
{
   return theRightMeterLabelColor;
}
   
void ossimMapCompositionSource::setTopMeterLabelColor(const ossimRgbVector& color)
{
   theTopMeterLabelColor = color;
}

void ossimMapCompositionSource::setBottomMeterLabelColor(const ossimRgbVector& color)
{
   theBottomMeterLabelColor = color;
}

void ossimMapCompositionSource::setLeftMeterLabelColor(const ossimRgbVector& color)
{
   theLeftMeterLabelColor = color;
}

void ossimMapCompositionSource::setRightMeterLabelColor(const ossimRgbVector& color)
{
   theRightMeterLabelColor = color;
}

void ossimMapCompositionSource::setMeterLabelColor(const ossimRgbVector& color)
{
   setTopMeterLabelColor(color);
   setBottomMeterLabelColor(color);
   setLeftMeterLabelColor(color);
   setRightMeterLabelColor(color);
}
   
void ossimMapCompositionSource::setMeterGridColor(const ossimRgbVector& color)
{
   theMeterGridColor = color;
}

void ossimMapCompositionSource::setMeterGridType(ossimGridLineType gridType)
{
   theMeterGridType = gridType;
}

void ossimMapCompositionSource::setGeographicGridType(ossimGridLineType gridType)
{
   theGeographicGridType = gridType;
}

void ossimMapCompositionSource::setBorderColor(ossimRgbVector& color)
{
   theBorderColor = color;
}

ossimMapCompositionSource::ossimGridLineType ossimMapCompositionSource::getGeographicGridType()const
{
   return theGeographicGridType;
}

ossimMapCompositionSource::ossimGridLineType ossimMapCompositionSource::getMeterGridType()const
{
   return theMeterGridType;
}
 
void ossimMapCompositionSource::setGeographicTickMarkFlag(bool flag)
{
   setTopGeographicTickMarkFlag(flag);
   setBottomGeographicTickMarkFlag(flag);
   setLeftGeographicTickMarkFlag(flag);
   setRightGeographicTickMarkFlag(flag);
}

void ossimMapCompositionSource::setTopGeographicTickMarkFlag(bool flag)
{
   theTopGeographicTickFlag = flag;
}

void ossimMapCompositionSource::setBottomGeographicTickMarkFlag(bool flag)
{
   theBottomGeographicTickFlag = flag;
}

void ossimMapCompositionSource::setLeftGeographicTickMarkFlag(bool flag)
{
   theLeftGeographicTickFlag = flag;
}

void ossimMapCompositionSource::setRightGeographicTickMarkFlag(bool flag)
{
   theRightGeographicTickFlag = flag;
}

void ossimMapCompositionSource::setTopGeographicLabelFlag(bool flag)
{
   theTopGeographicLabelFlag = flag;
}

void ossimMapCompositionSource::setBottomGeographicLabelFlag(bool flag)
{
   theBottomGeographicLabelFlag = flag;
}

void ossimMapCompositionSource::setLeftGeographicLabelFlag(bool flag)
{
   theLeftGeographicLabelFlag = flag;
}

void ossimMapCompositionSource::setRightGeographicLabelFlag(bool flag)
{
   theRightGeographicLabelFlag = flag;
}

bool ossimMapCompositionSource::getTopGeographicTickMarkFlag()const
{
   return theTopGeographicTickFlag;
}

bool ossimMapCompositionSource::getBottomGeographicTickMarkFlag()const
{
   return theBottomGeographicTickFlag;
}

bool ossimMapCompositionSource::getLeftGeographicTickMarkFlag()const
{
   return theLeftGeographicTickFlag;
}

bool ossimMapCompositionSource::getRightGeographicTickMarkFlag()const
{
   return theRightGeographicTickFlag;
}

bool ossimMapCompositionSource::getTopGeographicLabelFlag()const
{
   return theTopGeographicLabelFlag;
}

bool ossimMapCompositionSource::getBottomGeographicLabelFlag()const
{
   return theBottomGeographicLabelFlag;
}

bool ossimMapCompositionSource::getLeftGeographicLabelFlag()const
{
   return theLeftGeographicLabelFlag;
}

bool ossimMapCompositionSource::getRightGeographicLabelFlag()const
{
   return theRightGeographicLabelFlag;
}

void ossimMapCompositionSource::setMeterTickMarkFlag(bool flag)
{
   setTopMeterTickMarkFlag(flag);
   setBottomMeterTickMarkFlag(flag);
   setLeftMeterTickMarkFlag(flag);
   setRightMeterTickMarkFlag(flag);
}

void ossimMapCompositionSource::setTopMeterTickMarkFlag(bool flag)
{
   theTopMeterTickFlag = flag;
}

void ossimMapCompositionSource::setBottomMeterTickMarkFlag(bool flag)
{
   theBottomMeterTickFlag = flag;
}

void ossimMapCompositionSource::setLeftMeterTickMarkFlag(bool flag)
{
   theLeftMeterTickFlag = flag;
}

void ossimMapCompositionSource::setRightMeterTickMarkFlag(bool flag)
{
   theRightMeterTickFlag = flag;
}

void ossimMapCompositionSource::setTopMeterLabelFlag(bool flag)
{
   theTopMeterLabelFlag = flag;
}

void ossimMapCompositionSource::setBottomMeterLabelFlag(bool flag)
{
   theBottomMeterLabelFlag = flag;
}

void ossimMapCompositionSource::setLeftMeterLabelFlag(bool flag)
{
   theLeftMeterLabelFlag = flag;
}

void ossimMapCompositionSource::setRightMeterLabelFlag(bool flag)
{
   theRightMeterLabelFlag = flag;
}
   
bool ossimMapCompositionSource::getTopMeterTickMarkFlag()const
{
   return theTopMeterTickFlag;
}

bool ossimMapCompositionSource::getBottomMeterTickMarkFlag()const
{
   return theBottomMeterTickFlag;
}

bool ossimMapCompositionSource::getLeftMeterTickMarkFlag()const
{
   return theLeftMeterTickFlag;
}

bool ossimMapCompositionSource::getRightMeterTickMarkFlag()const
{
   return theRightMeterTickFlag;
}

bool ossimMapCompositionSource::getTopMeterLabelFlag()const
{
   return theTopMeterLabelFlag;
}

bool ossimMapCompositionSource::getBottomMeterLabelFlag()const
{
   return theBottomMeterLabelFlag;
}

bool ossimMapCompositionSource::getLeftMeterLabelFlag()const
{
   return theLeftMeterLabelFlag;
}

bool ossimMapCompositionSource::getRightMeterLabelFlag()const
{
   return theRightMeterLabelFlag;
}

void ossimMapCompositionSource::addFixedAnnotation(ossimAnnotationObject* obj)
{
   if(obj)
   {
      theFixedAnnotationList.push_back(obj);
   }
}

//**************************************************************************************************
//! Fetches the input connection's image geometry and verifies that it is a map projection.
//! Returns NULL if no valid projection found.
//**************************************************************************************************
const ossimMapProjection* ossimMapCompositionSource::inputMapProjection() const
{
   if (!theInputConnection)
      return 0;

   ossimRefPtr<ossimImageGeometry> inputGeom = theInputConnection->getImageGeometry();
   if (!inputGeom)
      return 0;

   return PTR_CAST(ossimMapProjection, inputGeom->getProjection());
}

