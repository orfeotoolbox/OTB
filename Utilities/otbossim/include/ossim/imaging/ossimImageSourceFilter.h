//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
// Description:
//
//*******************************************************************
//  $Id: ossimImageSourceFilter.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimImageSourceFilter_HEADER
#define ossimImageSourceFilter_HEADER
#include <ossim/imaging/ossimImageSource.h>
#include <ossim/base/ossimConnectableObjectListener.h>
#include <ossim/base/ossimConnectionEvent.h>

class OSSIMDLLEXPORT ossimImageSourceFilter : public ossimImageSource,
     public ossimConnectableObjectListener
{
public:
   ossimImageSourceFilter(ossimObject* owner=NULL);
   ossimImageSourceFilter(ossimImageSource* inputSource);
   ossimImageSourceFilter(ossimObject* owner,
                          ossimImageSource* inputSource);
   virtual ~ossimImageSourceFilter();

   virtual ossimIrect getBoundingRect(ossim_uint32 resLevel=0)const;
   virtual void getDecimationFactor(ossim_uint32 resLevel,
                                    ossimDpt& result)const;
   virtual void getDecimationFactors(vector<ossimDpt>& decimations)const;
   virtual ossim_uint32 getNumberOfDecimationLevels()const;

   /*!
    * Returns the input connection's output band list.
    * If not connected calls ossimImageSource::getOutputBandList.
    */
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const;
   
   /*!
    * Returns the number of bands available from the input.
    */
   virtual ossim_uint32 getNumberOfInputBands()const;   

   virtual ossimScalarType getOutputScalarType() const;
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& rect,
                                               ossim_uint32 resLevel=0);

   /*!
    * ordering specifies how the vertices should be arranged.
    * valid image vertices is basically the tightly fit convex hull
    * of the image.  Usually an image has NULL values and are
    * internally not upright rectangular.  This can cause
    * problems some spatial filters.
    *
    * The default implementation is to return the bounding rect.
    */
   virtual void getValidImageVertices(vector<ossimIpt>& validVertices,
                                      ossimVertexOrdering ordering=OSSIM_CLOCKWISE_ORDER,
                                      ossim_uint32 resLevel=0)const;
   
   virtual ossim_uint32 getTileWidth() const;
   
   virtual ossim_uint32 getTileHeight() const;

   virtual void initialize();

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   /*!
    * getNullPixelValue
    * Returns the null pixel value of input connection if connected.
    * Returns OSSIM_DBL_NAN if not connected.
    */
   virtual double getNullPixelValue(ossim_uint32 band=0)const;

   /*!
    * getMinPixelValue
    * Returns the null pixel value of input connection if connected.
    * Returns OSSIM_DBL_NAN if not connected.
    */
   virtual double getMinPixelValue(ossim_uint32 band=0)const;

   /*!
    * getMaxPixelValue
    * Returns the null pixel value of input connection if connected.
    * Returns OSSIM_DBL_NAN if not connected.
    */
   virtual double getMaxPixelValue(ossim_uint32 band=0)const;

   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const;
   virtual void connectInputEvent(ossimConnectionEvent& event);
   virtual void disconnectInputEvent(ossimConnectionEvent& event);
   virtual void propertyEvent(ossimPropertyEvent& event);
   virtual void refreshEvent(ossimRefreshEvent& event);
   

   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   
protected:
   ossimImageSourceInterface* theInputConnection;
TYPE_DATA
};

#endif /* #ifndef ossimImageSourceFilter_HEADER */
