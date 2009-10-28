//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimNBandToIndexFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimNBandToIndexFilter_HEADER
#define ossimNBandToIndexFilter_HEADER
#include <ossim/base/ossimNBandLutDataObject.h>
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
 * type:  ossimNBandToIndexFilter
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
 * lut.type:  ossimNBandLutDataObject
 *
 * </pre>
 */
class OSSIM_DLL ossimNBandToIndexFilter : public ossimImageSourceFilter
{
public:
   /*!
    * Initializes the min value to 0 and the max value to 4000.
    */
   ossimNBandToIndexFilter();
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& origin,
                                               ossim_uint32 resLevel=0);
   
   virtual ossim_uint32 getNumberOfOutputBands() const;
   
   virtual ossimScalarType getOutputScalarType() const;
   
   void setLut(ossimNBandLutDataObject& lut);

   void setLut(const ossimFilename& file);

   virtual bool isSourceEnabled()const;
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

   /**
    * Will set the property whose name matches the argument
    * "property->getName()".
    *
    * @param property Object containing property to set.
    */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);

   /**
    * @param name Name of property to return.
    * 
    * @returns A pointer to a property object which matches "name".  Returns
    * NULL if no match is found.
    */
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;

   /**
    * Pushes this's names onto the list of property names.
    *
    * @param propertyNames array to add this's property names to.
    */
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
protected:
   virtual ~ossimNBandToIndexFilter();
   void allocate();

   
   virtual ossimRefPtr<ossimImageData> convertInputTile(ossimImageData* inputTile);

   template <class T> ossimRefPtr<ossimImageData> convertOutputTileTemplate(T inputDummy,
                                                                            ossimImageData* inputTile);
   template <class T1, class T2> ossimRefPtr<ossimImageData> convertInputTileToOutputTemplate(T1 inputDummy,
                                                                                              T2 outputDummy,
                                                                                              ossimImageData* inputTile);
   
   ossimRefPtr<ossimNBandLutDataObject> theLut;
   ossimRefPtr<ossimImageData>          theTile;
   ossimFilename                        theLutFilename;
   bool                                 theKeepQuantizedValueFlag;
   std::vector<ossimNBandLutDataObject::LUT_ENTRY_TYPE> theMinValues;
   std::vector<ossimNBandLutDataObject::LUT_ENTRY_TYPE> theMaxValues;
   std::vector<ossimNBandLutDataObject::LUT_ENTRY_TYPE> theNullValues;
TYPE_DATA
};

#endif
