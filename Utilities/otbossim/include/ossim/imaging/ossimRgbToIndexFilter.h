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
// $Id: ossimRgbToIndexFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimRgbToIndexFilter_HEADER
#define ossimRgbToIndexFilter_HEADER
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimRgbLutDataObject.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <vector>

class ossimImageData;

/*!
 * It will map the input data to an 8-bit index table.  The table
 * <pre>
 *
 * Example keyword list:
 *
 * type:  ossimRgbToIndexFilter
 *
 * If you want to load from a file then just do:
 *
 * lut.filename: <full path to lut file>
 *
 * If you have the lut table in line then it must look like this:
 *
 * lut.entry0:  204 102 1
 * lut.entry1:  255 204 153
 * lut.entry2:  51 204 204
 * lut.number_of_entries:  3
 * lut.type:  ossimRgbLutDataObject
 *
 * </pre>
 */
class ossimRgbToIndexFilter : public ossimImageSourceFilter
{
public:
   /*!
    * Initializes the min value to 0 and the max value to 4000.
    */
   ossimRgbToIndexFilter();
   ossimRgbToIndexFilter(ossimImageSource* inputSource,
                           const ossimRgbLutDataObject& rgbLut);
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);
   
   virtual ossim_uint32 getNumberOfOutputBands() const;
   
   virtual ossimScalarType getOutputScalarType() const;
   
   void setLut(ossimRgbLutDataObject& lut);

   void setLut(const ossimFilename& file);

   virtual void disableSource();
   
   virtual double getNullPixelValue(ossim_uint32 band=0)const;

   virtual double getMinPixelValue(ossim_uint32 band=0)const;

   virtual double getMaxPixelValue(ossim_uint32 band=0)const;
   
   virtual void initialize();

   /*!
    * Saves the state of this object.
    */
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;

   /*!
    * Loads the state of this object.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);
protected:
   virtual ~ossimRgbToIndexFilter();
   void allocate();
   
   virtual ossimRefPtr<ossimImageData> convertInputTile(ossimRefPtr<ossimImageData>& tile);

   ossimRefPtr<ossimRgbLutDataObject>       theLut;
   ossimRefPtr<ossimImageData> theTile;
   
TYPE_DATA
};

#endif
