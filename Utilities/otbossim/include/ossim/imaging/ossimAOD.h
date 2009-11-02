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
// Class to compute Aerosol Optical Depth (AOD) for with atmospheric
// correction.
//
//*************************************************************************
// $Id: ossimAOD.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAOD_HEADER
#define ossimAOD_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimAOD : public ossimImageSourceFilter
{
public:

   ossimAOD(ossimObject* owner=NULL);
   ossimAOD(ossimImageSource* inputSource);
   ossimAOD(ossimObject* owner, ossimImageSource* inputSource);


   virtual ossimString getShortName() const;

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
   friend ostream& operator << (ostream& os,  const ossimAOD& hr);

   static void writeTemplate(ostream& os);

protected:
   virtual ~ossimAOD();

   void allocate();
   
   /*!
    * Method to set unset the enable flag.
    */
   void verifyEnabled();

   ossimRefPtr<ossimImageData> theTile;

   TYPE_DATA
};

#endif /* #ifndef ossimAOD_HEADER */

