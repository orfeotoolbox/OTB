//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LGPL
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimApplanixUtmModel.h,v 1.1 2005/09/06 16:20:04 gpotts Exp $
#ifndef ossimApplanixUtmModel_HEADER
#define ossimApplanixUtmModel_HEADER
#include "ossimFcsiModel.h"
#include <projections/sensor_modeling/ossimMeanRadialLensDistortion.h>
#include <base/data_types/ossimDpt3d.h>

class OSSIM_DLL ossimApplanixUtmModel : public ossimSensorModel
{
public:
   ossimApplanixUtmModel();
   ossimApplanixUtmModel(const ossimApplanixUtmModel& src);
   virtual ossimObject* dup()const;
   
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

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
