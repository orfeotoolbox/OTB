//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// 
//*******************************************************************
//  $Id$
#ifndef ossimBuckeyeSensor_HEADER
#define ossimBuckeyeSensor_HEADER
/**
 * The Buckey sensor has a set of simple orientation paramters.  For this model we are using the
 * orientation paramters found in the MetaData_GSTI.txt file typically found in the directory where the
 * imagery sits.
 *
 * There are 2 forms the class takes for keyword lists.  The first allows one to point
 * to a file containing the frame numbers and all the orientation paramters. You will still
 * need to define the Camera information and callibrartion paramters.  The pixel_size, 
 * focal_length and principal point are all in millimeters. 
 *
 * <pre>
 * EXAMPLE 1:
 *
 * type: ossimBuckeyeSensor
 * framemeta_gsti: /data/buckey/01-31@09/01-31@09/10-39-48/FrameMeta_GSTI.txt
 * frame_number: 10001
 * pixel_size: (0.0068,0.0068)
 * focal_length: 211.0211
 * principal_point: (-0.1853,1.2428)
 * smac_radial: -.2165e-3 .4230e-7 -.1652e-11 .2860e-19 .5690e-26
 * smac_decent: -.1483e-6 .1558e-6 -.1464e-18 .1233e-38
 * rect: 0 0 7239 5432
 *
 *
 * EXAMPLE 2:
 * roll: 0.074405
 * pitch: 2.849851
 * yaw: 269.517649
 * platform_position: (34.927014,69.353684,3757.12,WGE)
 * pixel_size: (0.0068,0.0068)
 * focal_length: 211.0211
 * principal_point: (-0.1853,1.2428)
 * smac_radial: -.2165e-3 .4230e-7 -.1652e-11 .2860e-19 .5690e-26
 * smac_decent: -.1483e-6 .1558e-6 -.1464e-18 .1233e-38
 * rect: 0 0 7239 5432
 */
#include <ossim/projection/ossimFcsiModel.h>
#include <ossim/base/ossimDpt3d.h>
#include "ossimSmacCallibrationSystem.h"

class OSSIM_DLL ossimBuckeyeSensor : public ossimSensorModel
{
public:
   ossimBuckeyeSensor();
   ossimBuckeyeSensor(const ossimDrect& imageRect, // center in image space
                          const ossimGpt& platformPosition,
                          double roll,
                          double pitch,
                          double heading,
                          const ossimDpt& principalPoint, // in millimeters
                          double focalLength, // in millimeters
                          const ossimDpt& pixelSize); // in millimeters
   ossimBuckeyeSensor(const ossimBuckeyeSensor& src);
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
   ossimRefPtr<ossimSmacCallibrationSystem> theLensDistortion;


   ossimEcefPoint theAdjEcefPlatformPosition;
   
TYPE_DATA
};

#endif
