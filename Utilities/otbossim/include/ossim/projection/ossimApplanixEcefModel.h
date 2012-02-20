//*******************************************************************
// Copyright (C) 2005 Garrett Potts
//
// LGPL
// 
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimApplanixEcefModel.h 20496 2012-01-25 17:15:43Z gpotts $
#ifndef ossimApplanixEcefModel_HEADER
#define ossimApplanixEcefModel_HEADER
#include <ossim/projection/ossimFcsiModel.h>
#include <ossim/projection/ossimMeanRadialLensDistortion.h>
#include <ossim/base/ossimDpt3d.h>

class OSSIM_DLL ossimApplanixEcefModel : public ossimSensorModel
{
public:
   ossimApplanixEcefModel();
   ossimApplanixEcefModel(const ossimDrect& imageRect, // center in image space
                          const ossimGpt& platformPosition,
                          double roll,
                          double pitch,
                          double heading,
                          const ossimDpt& principalPoint, // in millimeters
                          double focalLength, // in millimeters
                          const ossimDpt& pixelSize); // in millimeters
   ossimApplanixEcefModel(const ossimApplanixEcefModel& src);
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

   virtual bool insideImage(const ossimDpt& p) const
   {
      /*          return( (p.u>=(0.0-FLT_EPSILON)) && */
      /*                  (p.u<=(double)(theImageSize.u-(1-FLT_EPSILON))) && */
      /*                  (p.v>=(0.0-FLT_EPSILON)) && */
      /*                  (p.v<=(double)(theImageSize.v-(1-FLT_EPSILON))) ); */
      // if it's close to the edge we will consider it inside the image
      //
      return theImageClipRect.pointWithin(p, theImageClipRect.width());
   }


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
   
   /*!
    * ossimOptimizableProjection
    */
//   inline virtual bool useForward()const {return true;} //!ground to image faster (you don't need DEM)
   inline virtual bool useForward()const {return false;} //!ground to image faster (you don't need DEM)
   virtual bool setupOptimizer(const ossimString& init_file); //!uses file path to init model

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
