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
// $Id: ossimCastTileSourceFilter.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimCastTileSourceFilter_HEADER
#define ossimCastTileSourceFilter_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

class OSSIMDLLEXPORT ossimCastTileSourceFilter : public ossimImageSourceFilter
{
public:
   ossimCastTileSourceFilter(ossimImageSource* inputSource=NULL,
			     ossimScalarType scalarType=OSSIM_UCHAR);
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);
   virtual void initialize();
   virtual ossimScalarType getOutputScalarType() const;

   virtual ossimString getOutputScalarTypeString()const;

   /**
    *  Sets the output scalar type.
    */
   virtual void setOutputScalarType(ossimScalarType scalarType);
   virtual void setOutputScalarType(ossimString scalarType);
   
   /**
    *  Sets the current resolution level.  Returns true on success, false
    *  on error.
    */
   virtual double getNullPixelValue(ossim_uint32 band)const;
   virtual double getMinPixelValue(ossim_uint32 band=0)const;
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   ossimRefPtr<ossimImageData> applyCast(ossimRefPtr<ossimImageData> input);
   
   /**
    *  Method to save the state of an object to a keyword list.
    *  Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL)const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);

   /**
    *   Override base class so that a disableSource event does not
    *   reinitialize the object and enable itself.
    */
   virtual void propertyEvent(ossimPropertyEvent& event);
   virtual void refreshEvent(ossimRefreshEvent& event);   

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
     
protected:
   virtual ~ossimCastTileSourceFilter();

   void allocate(const ossimRefPtr<ossimImageData> inputTile);
   
   ossimRefPtr<ossimImageData> theTile;
   ossimScalarType             theOutputScalarType;

   template<class inType>
   void castInputToOutput(inType **inBuffer,
                          const double  *nullPix,
                          bool   inPartialFlag);
   
   template<class inType, class outType>
   void castInputToOutput(inType *inBuffer[],
                          const double  *nullInPix,
                          bool   inPartialFlag,
                          outType *outBuffer[],
                          const double  *nullOutPix,
                          ossim_uint32   numberOfBands);
TYPE_DATA
};

#endif /* #ifndef ossimCastTileSourceFilter_HEADER */
