//*******************************************************************
// Copyright (C) 2002 ImageLinks Inc. 
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kathy Minear
//
// Description:
//
// Gamma remapper.
//
//*************************************************************************
// $Id: ossimGammaRemapper.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimGammaRemapper_HEADER
#define ossimGammaRemapper_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimGammaRemapper : public ossimImageSourceFilter
{
public:

   ossimGammaRemapper(ossimObject* owner=NULL);
   ossimGammaRemapper(ossimImageSource* inputSource);
   ossimGammaRemapper(ossimObject* owner, ossimImageSource* inputSource);


   virtual ossimString getShortName()const;

   virtual void initialize();

   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tile_rect,
                                   ossim_uint32 resLevel=0);
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual ostream& print(ostream& os) const;
   friend ostream& operator<< (ostream& os,  const ossimGammaRemapper& hr);

   void setMinMaxPixelValues(const vector<double>& v_min,
                             const vector<double>& v_max);

   virtual void enableSource();
   
protected:
   virtual ~ossimGammaRemapper();

   /*!
    * Method to set unset the enable flag.
    */

   void verifyEnabled();

   ossimRefPtr<ossimImageData> theTile;
   double*         theBuffer;
   vector<double>  theMinPixelValue;
   vector<double>  theMaxPixelValue;
   vector<double>  theGamma;
   bool            theUserDisabledFlag;

   TYPE_DATA
};

#endif /* #ifndef ossimGammaRemapper_HEADER */
