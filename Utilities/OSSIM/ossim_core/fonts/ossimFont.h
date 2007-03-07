//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: 
// 
//********************************************************************
// $Id: ossimFont.h,v 1.17 2005/09/09 19:23:58 gpotts Exp $
#ifndef ossimFont_HEADER
#define ossimFont_HEADER
#include "base/data_types/ossimIpt.h"
#include "base/data_types/ossimIrect.h"
#include "base/data_types/ossimString.h"
#include "imaging/ossimRgbImage.h"
#include "base/common/ossimConstants.h"
#include "matrix/newmat.h"
#include "base/common/ossimObject.h"
#include "base/common/ossimErrorStatusInterface.h"
#include "base/common/ossimFontInformation.h"

#define OSSIM_FONT_DEFAULT_SIZE_X 12
#define OSSIM_FONT_DEFAULT_SIZE_Y 12

class ossimFont : public ossimObject, public ossimErrorStatusInterface
{
public:
   ossimFont()
      :theStringToRasterize(""),
       theFamilyName(""),
       theStyleName(""),
       theRotation(0),
       theHorizontalShear(0.0),
       theVerticalShear(0.0),
       theHorizontalScale(1.0),
       theVerticalScale(1.0),
       theHorizontalPixelSize(8),
       theVerticalPixelSize(8),
       theHorizontalDeviceUnits(72),
       theVerticalDeviceUnits(72),
       theAffineTransform(2,2)
      {
         theAffineTransform << 1.0 << 0.0
                            << 0.0 << 1.0;
      }
   
   ossimFont(const ossimString& familyName,
             const ossimString& styleName,
             int                horizontalPixelSize,
             int                verticalPixelSize,
             int                horizontalDeviceUnits=72,// 72 dpi is default,
             int                verticalDeviceUnits=72)// 72 dpi is default
      :theStringToRasterize(""),
       theFamilyName(familyName),
       theStyleName(styleName),
       theRotation(0),
       theHorizontalShear(0.0),
       theVerticalShear(0.0),
       theHorizontalScale(1.0),
       theVerticalScale(1.0),
       theHorizontalPixelSize(horizontalPixelSize),
       theVerticalPixelSize(verticalPixelSize),
       theHorizontalDeviceUnits(horizontalDeviceUnits),
       theVerticalDeviceUnits(horizontalDeviceUnits),
       theAffineTransform(2,2)
      {
         theAffineTransform << 1.0 << 0.0
                            << 0.0 << 1.0;
      }

   ossimFont(const ossimFont& rhs)
      {
         theStringToRasterize      = rhs.theStringToRasterize;
         theFamilyName             = rhs.theFamilyName;
         theStyleName              = rhs.theStyleName;
         theRotation               = rhs.theRotation;
         theHorizontalShear        = rhs.theHorizontalShear;
         theVerticalShear          = rhs.theVerticalShear;
         theHorizontalScale        = rhs.theHorizontalScale;
         theVerticalScale          = rhs.theVerticalScale;
         theHorizontalPixelSize    = rhs.theHorizontalPixelSize;
         theHorizontalDeviceUnits  = rhs.theHorizontalDeviceUnits;
         theVerticalDeviceUnits    = rhs.theVerticalDeviceUnits;
         theAffineTransform        = rhs.theAffineTransform;
         theClippingBox            = rhs.theClippingBox;
         theLocalOrigin            = rhs.theLocalOrigin;
         theAffineTransform        = rhs.theAffineTransform;
      }
   virtual ~ossimFont()
      {}
   virtual ossimObject* dup()const=0;

   /*!
    * Returns true if this font is a fixed size font
    */
   virtual bool isFixed()const=0;

   /*!
    * You can get a list of fixed sizes supported by
    * this font.
    */
   virtual void getFixedSizes(vector<ossimIpt>& sizes)const=0;

   /*!
    * Will return the previously rasterized buffer.
    */
   virtual const ossim_uint8* getBuf()const=0;
   
   /*!
    * Will return an internal buffer for rasterizing and will
    * also set the width, and height arguments of the returned buffer.
    */
   virtual const ossim_uint8* rasterize()=0;

   virtual const ossim_uint8* rasterize(const ossimString& s)
      {
         setString(s);
         return rasterize();
      }

   virtual void setString(const ossimString& s)
      {
         theStringToRasterize = s;
      }
   
   /*!
    * Will return the bounding box of the last string that was set
    * to be rasterized in pixel space
    */
   virtual void getBoundingBox(ossimIrect& box)=0;

   /*!
    * Specifies if its helvetica, times new roman ... etc.
    */ 
   virtual ossimString getFamilyName()const
      {
         return theFamilyName;
      }
   
   /*!
    * Specifies if its bold or italic .. etc
    */
   virtual ossimString getStyleName()const
      {
         return theStyleName;
      }

   /*!
    * Allow us to rotate a font.
    */
    virtual void setRotation(double rotationInDegrees);

    virtual double getRotation()const
      {
         return theRotation;
      }
   
   virtual void getRotation(double& rotation)const
      {
         rotation = theRotation;
      }

   virtual void setHorizontalVerticalShear(double horizontalShear,
                                           double verticalShear)
      {
         theVerticalShear   = verticalShear;
         theHorizontalShear = horizontalShear;
         computeMatrix();
      }
   virtual void setVerticalShear(double verticalShear)
      {
         setHorizontalVerticalShear(theHorizontalShear,
                                    verticalShear);
      }

   virtual void setHorizontalShear(double horizontalShear)
      {
         setHorizontalVerticalShear(horizontalShear,
                                    theVerticalShear);
      }

   virtual double getVerticalShear()const
      {
         return theVerticalShear;
      }

   virtual double getHorizontalShear()const
      {
         return theHorizontalShear;
      }

   /*!
    * Sets the scale of the font. I'll have code in place to perform
    * reflections.  This can be specified with a - scale along the direction
    * you wish to reflect.
    */
   virtual void setScale(double horizontal,
                         double vertical);
   
   virtual void getScale(double& horizontal,
                         double& vertical)
      {
         horizontal = theHorizontalScale;
         vertical   = theVerticalScale;
      }

   virtual void setPixelSize(int horizontalPixelSize,
                             int verticalPixelSize)
      {
         theHorizontalPixelSize = horizontalPixelSize;
         theVerticalPixelSize   = verticalPixelSize;
      }

   virtual void setPixelSize(int pixelSize)
      {
         setPixelSize(pixelSize, pixelSize);
      }
   
   virtual void getPixelSize(int& horizontal,
                             int& vertical)
      {
         horizontal = theHorizontalPixelSize;
         vertical   = theVerticalPixelSize;
      }

   virtual long getHorizontalPixelSize()const
      {
         return theHorizontalPixelSize;
      }
   virtual ossimIpt getPixelSize()const
      {
         return ossimIpt(theHorizontalPixelSize,
                         theVerticalPixelSize);
      }
   virtual long getVerticalPixelSize()const
      {
         return theVerticalPixelSize;
      }

   virtual void setDeviceDpi(int horizontalDeviceUnits,
                             int verticalDeviceUnits)
      {
         theHorizontalDeviceUnits = horizontalDeviceUnits;
         theVerticalDeviceUnits   = verticalDeviceUnits;
      }
   
   /*!
    * Will clip the clipping box with the bounding box of the string and this
    * is the viewable information.
    *
    * If the clipping box is set then this should be used to get the dimensions
    * of the buffer.
    */
   virtual void getBufferRect(ossimIrect& bufRect)
      {
         getBoundingClipBox(bufRect);
      }
   
   virtual ossimIrect getBufferRect()
      {
         return getBoundingClipBox();
      }
   
   virtual void getBoundingClipBox(ossimIrect& clippedBox);
   
   virtual ossimIrect getBoundingClipBox();
   
   virtual void setClippingBox(const ossimIrect& clipBox=ossimIrect(0,0,0,0))
      {
         theClippingBox = clipBox;
      }
   
   virtual ossimIrect getClippingBox()const
      {
         return theClippingBox;
      }

   bool isIdentityTransform()const
      {
         return ( (theAffineTransform[0][0] == 1.0) && (theAffineTransform[1][0] == 0.0)&&
                  (theAffineTransform[0][1] == 0.0) && (theAffineTransform[1][1] == 1.0));
      }
   virtual void getFontInformation(vector<ossimFontInformation>& fontInformationList)const
      {
         fontInformationList.push_back(ossimFontInformation(getFamilyName(),
                                                            getStyleName(),
                                                            getPixelSize(),
                                                            isFixed()));         
      }

protected:
   ossimString theStringToRasterize;
   ossimString theFamilyName;
   ossimString theStyleName;
   double      theRotation;
   double      theHorizontalShear;
   double      theVerticalShear;
   double      theHorizontalScale;
   double      theVerticalScale;
   int         theHorizontalPixelSize;
   int         theVerticalPixelSize;
   int         theHorizontalDeviceUnits;
   int         theVerticalDeviceUnits;
   ossimIrect  theClippingBox;
   /*!
    * During the compution of the global bounding rect we will
    * Make sure that we save off the upper left since its not really 0,0.
    * We will make sure that the bounding rect is shifted to 0,0 but the
    * shift will be saved.  That way we can render the shape correctly
    * later.
    */
   mutable ossimIpt    theLocalOrigin;
   
   NEWMAT::Matrix  theAffineTransform;


   void computeMatrix();

TYPE_DATA
};

#endif
