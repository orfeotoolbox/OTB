//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LGPL
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimApplanixEcefModel.h,v 1.5 2005/06/29 12:00:15 gpotts Exp $
#ifndef ossimApplanixEcefModel_HEADER
#define ossimApplanixEcefModel_HEADER
#include "ossimFcsiModel.h"
#include <projections/sensor_modeling/ossimMeanRadialLensDistortion.h>
#include <base/data_types/ossimDpt3d.h>

class OSSIM_DLL ossimApplanixEcefModel : public ossimSensorModel
{
public:
   ossimApplanixEcefModel();
   ossimApplanixEcefModel(const ossimApplanixEcefModel& src);
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


   void setRollPitchHeading(double roll,
                            double pitch,
                            double heading);
   
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
   double         theRoll;
   double         thePitch;
   double         theHeading;
   ossimDpt       thePrincipalPoint;
   ossimDpt       thePixelSize;
   double         theFocalLength;
   ossimEcefPoint theEcefPlatformPosition;
   ossimRefPtr<ossimMeanRadialLensDistortion> theLensDistortion;


   ossimEcefPoint theAdjEcefPlatformPosition;
   
TYPE_DATA
};

#endif
