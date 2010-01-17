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
// $Id: ossimMapCompositionSource.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimMapCompositionSource_HEADER
#define ossimMapCompositionSource_HEADER
#include <ossim/imaging/ossimAnnotationSource.h>
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimPolyArea2d.h>
#include <ossim/font/ossimFont.h>
#include <ossim/base/ossimFontInformation.h>

class ossimFont;
class ossimMapProjection;
class ossimU8ImageData;

class ossimMapCompositionSource : public ossimAnnotationSource
{
public:
   enum ossimGridLineType
   {
      OSSIM_GRID_NONE    = 0,
      OSSIM_GRID_LINE    = 1,
      OSSIM_GRID_RESEAUX = 2
   };

   ossimMapCompositionSource();
   
   virtual ossimScalarType getOutputScalarType() const;
   virtual ossim_uint32    getNumberOfOutputBands() const;
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);


   ossimIpt getViewWidthHeight()const;

   void getViewWidthHeight(ossimIpt& widthHeight)const;
   
   void setViewWidthHeight(const ossimIpt& widthHeight);
   
   // all borders here
   virtual ossim_int32 getTopBorderLength()const;
   virtual ossim_int32 getBottomBorderLength()const;
   virtual ossim_int32 getLeftBorderLength()const;
   virtual ossim_int32 getRightBorderLength()const;

   virtual void setTopBorderLength(ossim_int32 length);
   virtual void setBottomBorderLength(ossim_int32 length);
   virtual void setLeftBorderLength(ossim_int32 length);
   virtual void setRightBorderLength(ossim_int32 length);

   //all spacing here
   void setGeographicSpacingLat(double value);
   void setGeographicSpacingLon(double value);
   void setMeterSpacingX(double value);
   void setMeterSpacingY(double value);
   void setMeterSpacing(double x, double y);
   void setGeographicSpacing(double lat, double lon);

   ossimDpt getGeographicSpacing()const;
   ossimDpt getMeterSpacing()const;
   
   // All geographic formatting here
   virtual ossimString getTopGeographicLabelFormat()const;
   virtual ossimString getBottomGeographicLabelFormat()const;
   virtual ossimString getLeftGeographicLabelFormat()const;
   virtual ossimString getRightGeographicLabelFormat()const;

   virtual void setGeographicLabelFormat(const ossimString format);
   virtual void setTopGeographicLabelFormat(const ossimString& format);
   virtual void setBottomGeographicLabelFormat(const ossimString& format);
   virtual void setLeftGeographicLabelFormat(const ossimString& format);
   virtual void setRightGeographicLabelFormat(const ossimString& format);
   
   // All font funtions here.
   virtual ossimString    getTitle()const;
   virtual ossimRgbVector getTitleColor()const;

   virtual void setTitleFont(const ossimFontInformation& fontInfo);
   virtual void setTitleColor(const ossimRgbVector& color);
   virtual void setTitle(const ossimString& s);
   virtual void setGeographicTopLabelFont(const ossimFontInformation& fontInfo);
   virtual void setGeographicBottomLabelFont(const ossimFontInformation& fontInfo);
   virtual void setGeographicLeftLabelFont(const ossimFontInformation& fontInfo);
   virtual void setGeographicRightLabelFont(const ossimFontInformation& fontInfo);

   virtual void setMeterTopLabelFont(const ossimFontInformation& fontInfo);
   virtual void setMeterBottomLabelFont(const ossimFontInformation& fontInfo);
   virtual void setMeterLeftLabelFont(const ossimFontInformation& fontInfo);
   virtual void setMeterRightLabelFont(const ossimFontInformation& fontInfo);

   virtual ossimFontInformation getTitleFont()const;
   virtual ossimFontInformation getGeographicTopLabelFontInfo()const;
   virtual ossimFontInformation getGeographicBottomLabelFontInfo()const;
   virtual ossimFontInformation getGeographicLeftLabelFontInfo()const;
   virtual ossimFontInformation getGeographicRightLabelFontInfo()const;
   virtual ossimFontInformation getMeterTopLabelFontInfo()const;
   virtual ossimFontInformation getMeterBottomLabelFontInfo()const;
   virtual ossimFontInformation getMeterLeftLabelFontInfo()const;
   virtual ossimFontInformation getMeterRightLabelFontInfo()const;

   // All color funtions here
   virtual ossimRgbVector getBorderColor()const;
   virtual void setBorderColor(const ossimRgbVector& color);

   virtual ossimRgbVector getGeographicGridColor()const;
   virtual ossimRgbVector getMeterGridColor()const;
   virtual ossimRgbVector getTopGeographicLabelColor()const;
   virtual ossimRgbVector getBottomGeographicLabelColor()const;
   virtual ossimRgbVector getLeftGeographicLabelColor()const;
   virtual ossimRgbVector getRightGeographicLabelColor()const;

   virtual void setGeographicGridColor(const ossimRgbVector& color);
   virtual void setTopGeographicLabelColor(const ossimRgbVector& color);
   virtual void setBottomGeographicLabelColor(const ossimRgbVector& color);
   virtual void setLeftGeographicLabelColor(const ossimRgbVector& color);
   virtual void setRightGeographicLabelColor(const ossimRgbVector& color);
   virtual void setGeographicLabelColor(const ossimRgbVector& color);

   virtual ossimRgbVector getTopMeterLabelColor()const;
   virtual ossimRgbVector getBottomMeterLabelColor()const;
   virtual ossimRgbVector getLeftMeterLabelColor()const;
   virtual ossimRgbVector getRightMeterLabelColor()const;
   
   virtual void setTopMeterLabelColor(const ossimRgbVector& color);
   virtual void setBottomMeterLabelColor(const ossimRgbVector& color);
   virtual void setLeftMeterLabelColor(const ossimRgbVector& color);
   virtual void setRightMeterLabelColor(const ossimRgbVector& color);
   virtual void setMeterLabelColor(const ossimRgbVector& color);
   virtual void setMeterGridColor(const ossimRgbVector& color);

   // All grid types here
   virtual void setMeterGridType(ossimGridLineType gridType);
   virtual void setGeographicGridType(ossimGridLineType gridType);
   virtual void setBorderColor(ossimRgbVector& color);

   virtual ossimGridLineType getGeographicGridType()const;
   virtual ossimGridLineType getMeterGridType()const;
   
   // all grid flags here
   void setGeographicTickMarkFlag(bool flag);
   void setTopGeographicTickMarkFlag(bool flag);
   void setBottomGeographicTickMarkFlag(bool flag);
   void setLeftGeographicTickMarkFlag(bool flag);
   void setRightGeographicTickMarkFlag(bool flag);
   void setTopGeographicLabelFlag(bool flag);
   void setBottomGeographicLabelFlag(bool flag);
   void setLeftGeographicLabelFlag(bool flag);
   void setRightGeographicLabelFlag(bool flag);

   virtual bool getTopGeographicTickMarkFlag()const;
   virtual bool getBottomGeographicTickMarkFlag()const;
   virtual bool getLeftGeographicTickMarkFlag()const;
   virtual bool getRightGeographicTickMarkFlag()const;
   virtual bool getTopGeographicLabelFlag()const;
   virtual bool getBottomGeographicLabelFlag()const;
   virtual bool getLeftGeographicLabelFlag()const;
   virtual bool getRightGeographicLabelFlag()const;

   void setMeterTickMarkFlag(bool flag);
   virtual void setTopMeterTickMarkFlag(bool flag);
   virtual void setBottomMeterTickMarkFlag(bool flag);
   virtual void setLeftMeterTickMarkFlag(bool flag);
   virtual void setRightMeterTickMarkFlag(bool flag);
   virtual void setTopMeterLabelFlag(bool flag);
   virtual void setBottomMeterLabelFlag(bool flag);
   virtual void setLeftMeterLabelFlag(bool flag);
   virtual void setRightMeterLabelFlag(bool flag);
   
   virtual bool getTopMeterTickMarkFlag()const;
   virtual bool getBottomMeterTickMarkFlag()const;
   virtual bool getLeftMeterTickMarkFlag()const;
   virtual bool getRightMeterTickMarkFlag()const;
   virtual bool getTopMeterLabelFlag()const;
   virtual bool getBottomMeterLabelFlag()const;
   virtual bool getLeftMeterLabelFlag()const;
   virtual bool getRightMeterLabelFlag()const;
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;

   virtual void initialize();

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

protected:
   virtual ~ossimMapCompositionSource();
   ossimIpt            theViewWidthHeight;
   ossimGridLineType   theMeterGridType;
   ossimGridLineType   theGeographicGridType;
   
   ossim_uint32        theTopBorderLength;
   ossim_uint32        theBottomBorderLength;
   ossim_uint32        theLeftBorderLength;
   ossim_uint32        theRightBorderLength;
   
   ossimRgbVector      theBorderColor;

   ossimRgbVector      theGeographicGridColor;
   ossimRgbVector      theMeterGridColor;

   // title info
   //
   ossimString          theTitleString;
   ossimRefPtr<ossimFont>           theTitleFont;
   ossimFontInformation theTitleFontInfo;
   ossimRgbVector       theTitleColor;
   
   // grid label colors
   //
   ossimRgbVector      theTopGeographicLabelColor;
   ossimRgbVector      theBottomGeographicLabelColor;
   ossimRgbVector      theLeftGeographicLabelColor;
   ossimRgbVector      theRightGeographicLabelColor;
   ossimRgbVector      theTopMeterLabelColor;
   ossimRgbVector      theBottomMeterLabelColor;
   ossimRgbVector      theLeftMeterLabelColor;
   ossimRgbVector      theRightMeterLabelColor;

   // grid label font
   //
   ossimFontInformation theGeographicTopLabelFontInfo;
   ossimRefPtr<ossimFont>           theGeographicTopLabelFont;
   ossimFontInformation theGeographicBottomLabelFontInfo;
   ossimRefPtr<ossimFont>           theGeographicBottomLabelFont;
   ossimFontInformation theGeographicLeftLabelFontInfo;
   ossimRefPtr<ossimFont>           theGeographicLeftLabelFont;
   ossimFontInformation theGeographicRightLabelFontInfo;
   ossimRefPtr<ossimFont>           theGeographicRightLabelFont;

   ossimFontInformation theMeterTopLabelFontInfo;
   ossimRefPtr<ossimFont>           theMeterTopLabelFont;
   ossimFontInformation theMeterBottomLabelFontInfo;
   ossimRefPtr<ossimFont>           theMeterBottomLabelFont;
   ossimFontInformation theMeterLeftLabelFontInfo;
   ossimRefPtr<ossimFont>           theMeterLeftLabelFont;
   ossimFontInformation theMeterRightLabelFontInfo;
   ossimRefPtr<ossimFont>           theMeterRightLabelFont;
   
   // grid label dms format strings
   //
   ossimString         theTopGeographicFormat;
   ossimString         theBottomGeographicFormat;
   ossimString         theLeftGeographicFormat;
   ossimString         theRightGeographicFormat;

   // grid label flag
   //
   bool                theTopGeographicLabelFlag;
   bool                theBottomGeographicLabelFlag;
   bool                theLeftGeographicLabelFlag;
   bool                theRightGeographicLabelFlag;

   bool                theTopGeographicTickFlag;
   bool                theBottomGeographicTickFlag;
   bool                theLeftGeographicTickFlag;
   bool                theRightGeographicTickFlag;

   bool                theTopMeterLabelFlag;
   bool                theBottomMeterLabelFlag;
   bool                theLeftMeterLabelFlag;
   bool                theRightMeterLabelFlag;

   bool                theTopMeterTickFlag;
   bool                theBottomMeterTickFlag;
   bool                theLeftMeterTickFlag;
   bool                theRightMeterTickFlag;
   
   
   ossimIrect          theTopBorder;
   ossimIrect          theBottomBorder;
   ossimIrect          theLeftBorder;
   ossimIrect          theRightBorder;

   // geo-tick spacing in lat lon
   ossimDpt            theGeographicSpacing;

   //  tick spacing in meter units
   ossimDpt            theMeterSpacing;
   
   ossimAnnotationSource::AnnotationObjectListType theFixedAnnotationList;

   /*!
    * Override base classes drawAnnotations so we can layout
    * any fixed annotations first.
    */
   virtual void drawAnnotations(ossimRefPtr<ossimImageData> tile);
   
   virtual void computeBorderRects();
   virtual void drawBorders();
   virtual void addGridLabels();
   
   virtual void addGeographicTopGridLabels();
   virtual void addGeographicBottomGridLabels();
   virtual void addGeographicLeftGridLabels();
   virtual void addGeographicRightGridLabels();

   virtual void addGeographicGridLines();
   virtual void addGeographicGridReseaux();
   
   virtual void addMeterGridLabels();
   
   virtual void addMeterGridLines();
   virtual void addMeterGridReseaux();

   
   virtual void addTitle();
   
   virtual void layoutAnnotations();

   virtual void addFixedAnnotation(ossimAnnotationObject* obj);
      
   virtual void deleteFixedAnnotations();
   
   ossimIrect getViewingRect()const;
   
   //! Fetches the input connection's image geometry and verifies that it is a map projection.
   //! Returns NULL if no valid projection found.
   const ossimMapProjection* inputMapProjection() const;

// For RTTI
TYPE_DATA
};
#endif /* #ifndef ossimMapCompositionSource_HEADER */

