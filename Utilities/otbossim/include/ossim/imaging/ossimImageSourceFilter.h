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
//  $Id: ossimImageSourceFilter.h 17932 2010-08-19 20:34:35Z dburken $
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

   /*!
    * Returns the input connection's output band list.
    * If not connected calls ossimImageSource::getOutputBandList.
    */
   virtual void getOutputBandList(std::vector<ossim_uint32>& bandList) const;
   
   /*!
    * Returns the number of bands available from the input.
    */
   virtual ossim_uint32 getNumberOfInputBands()const;   

   virtual void initialize();

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

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
   virtual ~ossimImageSourceFilter();
   ossimImageSource* theInputConnection;
TYPE_DATA
};

#endif /* #ifndef ossimImageSourceFilter_HEADER */
