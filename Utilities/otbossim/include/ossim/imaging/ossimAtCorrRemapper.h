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
// Takes in DNs for any number of bands
// Converts DNs to Radiance at the satellite values Lsat
// Converts Lsat to Surface Reflectance values
//
//*************************************************************************
// $Id: ossimAtCorrRemapper.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimAtCorrRemapper_HEADER
#define ossimAtCorrRemapper_HEADER

#include <ossim/imaging/ossimImageSourceFilter.h>

class ossimAtCorrRemapper : public ossimImageSourceFilter
{
public:

   ossimAtCorrRemapper(ossimObject* owner = NULL,
                       ossimImageSource* inputSource =  NULL,
                       const ossimString& sensorType = "");


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
   friend ostream& operator << (ostream& os,  const ossimAtCorrRemapper& hr);

   vector<double> getNormMinPixelValues() const;
   vector<double> getNormMaxPixelValues() const;

   void getNormMinPixelValues(vector<double>& v) const;
   void getNormMaxPixelValues(vector<double>& v) const;

   const ossimString& getSensorType() const;

   void setSensorType(const ossimString& sensorType);

protected:
   virtual ~ossimAtCorrRemapper();

   /*!
    * Method to set unset the enable flag.
    */
   void verifyEnabled();
   virtual void interpolate(const ossimDpt& pt,
                            int band,
                            double& a,
                            double& b,
                            double& c)const;
                            
   ossimRefPtr<ossimImageData> theTile;
   double*            theSurfaceReflectance;
   bool               theUseInterpolationFlag;
   vector<double>     theMinPixelValue;
   vector<double>     theMaxPixelValue;
   vector<double>     theXaArray;
   vector<double>     theXbArray;
   vector<double>     theXcArray;
   vector<double>     theBiasArray;
   vector<double>     theGainArray;
   vector<double>     theCalCoefArray;
   vector<double>     theBandWidthArray;
   ossimString        theSensorType;

TYPE_DATA
};

#endif /* #ifndef ossimAtCorrRemapper_HEADER */
