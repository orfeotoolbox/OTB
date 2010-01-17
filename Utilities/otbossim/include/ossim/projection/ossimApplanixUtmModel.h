//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LGPL
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimApplanixUtmModel.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimApplanixUtmModel_HEADER
#define ossimApplanixUtmModel_HEADER
#include <ossim/projection/ossimFcsiModel.h>
#include <ossim/projection/ossimMeanRadialLensDistortion.h>
#include <ossim/base/ossimDpt3d.h>

class OSSIM_DLL ossimApplanixUtmModel : public ossimSensorModel
{
public:
   ossimApplanixUtmModel();
   ossimApplanixUtmModel(const ossimApplanixUtmModel& src);
   virtual ossimObject* dup()const;
   
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

   void lineSampleToWorld(const ossimDpt& image_point,
                          ossimGpt&       gpt) const;
  
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
   virtual void worldToLineSample(const ossimGpt& world_point,
                                  ossimDpt&       image_point) const;
   
   virtual void updateModel();

   void setPrincipalPoint(ossimDpt principalPoint);

   void setPixelSize(const ossimDpt& pixelSize);
   void setImageRect(const ossimDrect& rect);
   void setFocalLength(double focalLength);
   void setPlatformPosition(const ossimGpt& gpt);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   virtual void initAdjustableParameters();
   
   /*!
    * ossimOptimizableProjection
    */
//   inline virtual bool useForward()const {return false;} 
   inline virtual bool useForward()const {return true;} //!ground to image faster (you don't need DEM) //TBC
   virtual bool setupOptimizer(const ossimString& init_file); //!uses file path to init model

protected:
   NEWMAT::Matrix theCompositeMatrix;
   NEWMAT::Matrix theCompositeMatrixInverse;
   double         theOmega;
   double         thePhi;
   double         theKappa;
   double         theBoreSightTx;
   double         theBoreSightTy;
   double         theBoreSightTz;
   ossimDpt       thePrincipalPoint;
   ossimDpt       thePixelSize;
   double         theFocalLength;
   ossimEcefPoint theEcefPlatformPosition;
   ossimGpt       thePlatformPosition;
   ossim_int32    theUtmZone;
   ossim_int8     theUtmHemisphere;
   ossimDpt3d     theUtmPlatformPosition;
   ossimEcefVector theShiftValues;
   
   ossimRefPtr<ossimMeanRadialLensDistortion> theLensDistortion;
   ossimEcefPoint theAdjEcefPlatformPosition;

TYPE_DATA
};

#endif
