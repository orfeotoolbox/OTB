//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
// 
// Description:  Class provides an elevation image source from the elevation
// manager.
//
//********************************************************************
// $Id: ossimElevImageSource.h 17108 2010-04-15 21:08:06Z dburken $

#ifndef ossimElevImageSource_HEADER
#define ossimElevImageSource_HEADER

#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimGrect.h>

class ossimElevSource;

class ossimElevImageSource : public ossimImageSource
{
public:

   ossimElevImageSource();
   ossimElevImageSource(ossimObject* owner);
   ossimElevImageSource(ossimObject* owner,
                        const ossimGpt& tie,
                        double latSpacing,  // decimal degrees
                        double lonSpacing ,
                        ossim_uint32 numberLines,
                        ossim_uint32 numberSamples);
   ossimElevImageSource(ossimObject* owner,
                        const ossimKeywordlist& kwl,
                        const char* prefix=0);
   

   /*!
    *  Returns a pointer to a tile given an origin representing the upper left
    *  corner of the tile to grab from the image.
    *  Satisfies pure virtual from TileSource class.
    */
   virtual ossimRefPtr<ossimImageData> getTile(const  ossimIrect& rect,
                                   ossim_uint32 resLevel=0);

   /*!
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection/model information to users.
    *  Returns true if geometry info is present, false if not.
    */
   virtual bool getImageGeometry(ossimKeywordlist& kwl,
                                 const char* prefix=0);
   
   /*!
    *  Returns the number of bands in the image.  Alway one for this source.
    */
   virtual ossim_uint32 getNumberOfOutputBands()const { return 1; }


   /*!
    * Returns the zero-based (relative) image rectangle for the reduced
    * resolution data set (rrds) passed in.  Note that rrds 0 is the highest
    * resolution rrds.
    */
   virtual ossimIrect getImageRectangle(ossim_uint32 reduced_res_level = 0) const; 

   /*!
    * Returns the absolute bounding rectangle of the image. The upper left
    * corner may be non-zero if this is a subimage.
    */
   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel = 0) const
      {
         return getImageRectangle(resLevel);
      }
   
   /*!
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   bool canConnectMyInputTo(ossim_int32 /* inputIndex */,
                            const ossimConnectableObject* /* object */ )const
   {      
      return false;
   }
      
   virtual void initialize();

   /*!
    *  Satisfy pure virtuals from base classes...
    */
   virtual void getDecimationFactor(ossim_uint32 resLevel, ossimDpt& result) const;
   virtual void getDecimationFactors(vector<ossimDpt>& decimations) const;
   virtual ossim_uint32 getNumberOfDecimationLevels()const;
   virtual ossim_uint32 getNumberOfInputBands() const;
   virtual ossimScalarType getOutputScalarType() const;
   virtual ossim_uint32 getTileWidth() const;
   virtual ossim_uint32 getTileHeight() const;
   virtual void changeTileSize(const ossimIpt& size);

   // access methods
   virtual ossimGpt getTiePoint()         const { return theTiePoint; }
   virtual ossim_float64 getLatitudeSpacing()  const { return theLatSpacing; }
   virtual ossim_float64 getLongitudeSpacing() const { return theLonSpacing; }
   virtual ossim_uint32  getNumberOfLines(ossim_uint32 reduced_res_level = 0) const;
   virtual ossim_uint32 getNumberOfSamples(ossim_uint32 reduced_res_level = 0) const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   
   // set methods
   virtual void setTiePoint(const ossimGpt& gpt) { theTiePoint = gpt; }
   virtual void setLatitudeSpacing(ossim_float64 sp) { theLatSpacing = sp; }
   virtual void setLongitudeSpacing(ossim_float64 sp) { theLonSpacing = sp; }
   virtual void setNumberOfLines(ossim_uint32 lines) { theNumberOfLines = lines; }
   virtual void setNumberOfSamples(ossim_uint32 samps) { theNumberOfSamps = samps;}
   virtual void setMinPixelValue(ossim_float64 min_pix);
   virtual void setMaxPixelValue(ossim_float64 max_pix);
   
protected:
   virtual ~ossimElevImageSource();

   ossimElevSource*            theElevManager;
   ossimRefPtr<ossimImageData> theTile;
   ossimGpt                    theTiePoint;      // upper left tie point
   ossim_float64               theLatSpacing;    // in decimal degrees
   ossim_float64               theLonSpacing;    // in decimal degrees
   ossim_uint32                theNumberOfLines;
   ossim_uint32                theNumberOfSamps;
   
TYPE_DATA
};

#endif
