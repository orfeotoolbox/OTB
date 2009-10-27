//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author: Garrett Potts
// 
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimBrightnessMatch.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimBrightnessMatch_HEADER
#define ossimBrightnessMatch_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimBrightnessContrastSource.h>

class ossimBrightnessMatch : public ossimImageSourceFilter
{
public:
   ossimBrightnessMatch();



   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect,
                                               ossim_uint32 resLevel=0);


   virtual void initialize();
   /*---------------------- PROPERTY INTERFACE ---------------------------*/
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;
   /*--------------------- PROPERTY INTERFACE END ------------------------*/

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   virtual ~ossimBrightnessMatch();
   
   double          theTargetBrightness;
   double          theInputBrightness;
   ossimRefPtr<ossimImageData> theNormTile;

   ossimRefPtr<ossimBrightnessContrastSource> theBrightnessContrastSource;
   
   ossimRefPtr<ossimImageData> runBrightnessMatch();

   void computeInputBrightness();
TYPE_DATA
};

#endif
