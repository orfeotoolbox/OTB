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
// $Id: ossimBrightnessMatch.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimBrightnessMatch_HEADER
#define ossimBrightnessMatch_HEADER
#include <ossim/imaging/ossimImageSourceFilter.h>
#include <ossim/imaging/ossimBrightnessContrastSource.h>

class ossimBrightnessMatch : public ossimImageSourceFilter
{
public:
   ossimBrightnessMatch();


   virtual ~ossimBrightnessMatch();

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
   double          theTargetBrightness;
   double          theInputBrightness;
   ossimRefPtr<ossimImageData> theNormTile;

   ossimRefPtr<ossimBrightnessContrastSource> theBrightnessContrastSource;
   
   ossimRefPtr<ossimImageData> runBrightnessMatch();

   void computeInputBrightness();
TYPE_DATA
};

#endif
