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
// $Id: ossimIndexToRgbLutFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimIndexToRgbLutFilter_HEADER
#define ossimIndexToRgbLutFilter_HEADER
#include <ossim/base/ossimRgbVector.h>
#include <ossim/base/ossimRgbLutDataObject.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <vector>

class ossimImageData;

/**
 * The output is an 8-bit pseudo color of the input
 * data.  This class implements a generic way to color code
 * any form of input data.  It will use theMinValue
 * theMaxValue to color code the input data.  Note
 * this class only operates on one input band since
 * it assumes the input is an index.  It also uses
 * an ossimRgbLutDataObject that is an array of color
 * data or ossimRgbVector data.  It will convert
 * the input into normalized form and then map
 * it uniformly to the color in the RGB Lut (Lookup Table).
 * <pre>
 *
 * Note:  You can tell this filter if it should use the
 *        nearest entry for the output color or linearly
 *        interpolate the entries for smooth coloring.
 *
 * Example keyword list:
 *
 * type:  ossimIndexToRgbLutFilter
 * // if the max_value and min_value is given it will use these for normalizing the
 * // input data to be between 0 and 1.  If not given it will query the min max
 * // from the input
 *
 * max_value:  2164.000000000000000
 * min_value:  -3.000000000000000
 *
 * // it owns an ossimRgbLutDataObject and has prefix value lut.
 *
 * // the value can be nearest or linear.  This describes how the
 * // color value is computed.  If it falls bewteen 2 color values
 * // for example, 1.4 it will go to entries 1 and 2 and interpolate
 * // if linear is set else it will return the color at entry 1 if
 * // nearest is set.
 * 
 * interpolation_type:  linear 
 *
 * // the rest of the keywords is the lookup table.
 * // NOTE: you do not have to have the same number of entries as you do
 * //       input range indices.  Just know that the lowest value is mapped
 * //       to 0 and the highest value is mapped to number of entires -1.
 *
 * If you want to load from a file then just do:
 *
 * lut.lut_file: <full path to lut file>
 *
 * If you have the lut table in line then it must look like this:
 *
 * lut.entry0.b:  204
 * lut.entry0.g:  102
 * lut.entry0.r:  1
 * lut.entry1.b:  255
 * lut.entry1.g:  204
 * lut.entry1.r:  153
 * lut.entry2.b:  204
 * lut.entry2.g:  204
 * lut.entry2.r:  51
 * lut.entry3.b:  255
 * lut.entry3.g:  1
 * lut.entry3.r:  1
 * lut.entry4.b:  204
 * lut.entry4.g:  1
 * lut.entry4.r:  204
 * lut.entry5.b:  204
 * lut.entry5.g:  51
 * lut.entry5.r:  255
 * lut.entry6.b:  51
 * lut.entry6.g:  1
 * lut.entry6.r:  102
 * lut.entry7.b:  255
 * lut.entry7.g:  255
 * lut.entry7.r:  255
 * lut.number_of_entries:  8
 * lut.type:  ossimRgbLutDataObject
 *
 *
 * 
 * </pre>
 */
class OSSIM_DLL ossimIndexToRgbLutFilter : public ossimImageSourceFilter
{
public:
   enum ossimIndexToRgbLutFilterInterpolationType
   {
      ossimIndexToRgbLutFilter_NEAREST = 0,
      ossimIndexToRgbLutFilter_LINEAR  = 1
   };
   /**
    * Initializes the min value to 0 and the max value to 4000.
    */
   ossimIndexToRgbLutFilter();
   ossimIndexToRgbLutFilter(ossimImageSource* inputSource,
                           const ossimRgbLutDataObject& rgbLut,
                           double minValue,
                           double maxValue,
                           ossimIndexToRgbLutFilterInterpolationType interpolationType);
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);
   
   ossim_uint32 getNumberOfOutputBands() const;
   
   virtual ossimScalarType getOutputScalarType() const;
   
   void           setLut(ossimRgbLutDataObject& lut);
   void           setLut(const ossimFilename& file);
   
   ossimFilename  getLutFile()const;
   
   double         getMinValue()const;
   double         getMaxValue()const;
   
   void           setMinValue(double value);
   void           setMaxValue(double value);
   
   virtual double getNullPixelValue()const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   ossimIndexToRgbLutFilterInterpolationType getInterpolationType()const;
   void setInterpolationType(ossimIndexToRgbLutFilterInterpolationType type);

   virtual void initialize();

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
   /**
    * Saves the state of this object.
    *
    * Keywords are:    min_value 
    *                  max_value
    *                  lut.<lut keywords> -> look at the
    *                  ossimRgbLutDataObject for the keywords that
    *                                         follow after the "." .
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;

   /**
    * Loads the state of this object.
    *
    * looks for keywords:
    *                  min_value 
    *                  max_value
    *                  lut.<lut keywords> -> look at the
    *                  ossimRgbLutDataObject for the keywords that
    *                                         follow after the "." .
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);
protected:
   virtual ~ossimIndexToRgbLutFilter();

   /**
    * Called on first getTile, will initialize all data needed.
    */
   void allocate();
   
   ossimRefPtr<ossimRgbLutDataObject> theLut;
   double theMinValue;
   double theMaxValue;
   double theMinMaxDeltaLength;
   bool theMinValueOverride;
   bool theMaxValueOverride;
      
   ossimIndexToRgbLutFilterInterpolationType theInterpolationType;
   ossimRefPtr<ossimImageData> theTile;
   ossimFilename   theLutFile;
   
   virtual void normalizeValue(double value,
                               double& normalizedValue);
   virtual void getColorNormIndex(double index, ossimRgbVector& result);
   virtual void getColor(double index, ossimRgbVector& result);
   virtual ossimRefPtr<ossimImageData> convertInputTile(const ossimRefPtr<ossimImageData>& tile);
   /**
    * Precomputed from the theMaxValue - theMinValue
    */
   
TYPE_DATA
};

#endif /* #ifndef ossimIndexToRgbLutFilter_HEADER */
