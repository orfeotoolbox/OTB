//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Oscar Kramer <okramer@imagelinks.com>
//          ossim port by David Burken <dburken@imagelinks.com>
//
// Description:  
//
// Contains declaration of class ossimSpot5Model.
// 
//*****************************************************************************
// $Id: ossimSpot5Model.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimSpot5Model_HEADER
#define ossimSpot5Model_HEADER

#include <iostream>
using namespace std;

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimMatrix3x3.h>

class ossimSpotDimapSupportData;

class OSSIMDLLEXPORT ossimSpot5Model : public ossimSensorModel
{
public:
   /*!
    * CONSTRUCTORS:
    */
   ossimSpot5Model();
   ossimSpot5Model(ossimSpotDimapSupportData* sd);
   ossimSpot5Model(const ossimFilename& init_file);
   ossimSpot5Model(const ossimKeywordlist& geom_kwl);
   ossimSpot5Model(const ossimSpot5Model& rhs);

   virtual ~ossimSpot5Model();

   enum AdjustParamIndex
   {
      ROLL_OFFSET = 0,
      PITCH_OFFSET,
      YAW_OFFSET,
      ROLL_RATE,
      PITCH_RATE,
      YAW_RATE,
      FOCAL_LEN_OFFSET,
      NUM_ADJUSTABLE_PARAMS // not an index
   };   

   /*!
    * Returns pointer to a new instance, copy of this.
    * Not implemented yet!  Returns NULL...
    */
   virtual ossimObject* dup() const;

   /*!
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;

   /*!
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=NULL) const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=NULL);

   /*!
    * Writes a template of geom keywords processed by loadState and saveState
    * to output stream.
    */
   static void writeGeomTemplate(ostream& os);

   /*!
    * Given an image point and height, initializes worldPoint.
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const ossim_float64& heightEllipsoid,
                                        ossimGpt& worldPoint) const;
   
   /*!
    * Given an image point, returns a ray originating at some arbitrarily high
    * point (ideally at the sensor position) and pointing towards the target.
    */
   virtual void imagingRay(const ossimDpt& image_point,
                           ossimEcefRay&   image_ray) const;

   /*!
    * Following a change to the adjustable parameter set, this virtual
    * is called to permit instances to compute derived quantities after
    * parameter change.
    */
   virtual void updateModel();

   /*!
    * ossimOptimizableProjection
    */
   inline virtual bool useForward()const {return false;} //!image to ground faster
   virtual bool setupOptimizer(const ossimString& init_file); //!uses file path to init model

   bool initFromMetadata(ossimSpotDimapSupportData* sd);
protected:

   /*!
    * Sets adjustables to default values.
    */
   void initAdjustableParameters();
   
   void loadGeometry(FILE*);
   void loadSupportData();
   //void computeSatToOrbRotation(ossim_float64 t)const;
   void computeSatToOrbRotation(NEWMAT::Matrix& result, ossim_float64 t)const;

/*    virtual ossimDpt extrapolate (const ossimGpt& gp) const; */
/*    virtual ossimGpt extrapolate (const ossimDpt& ip, */
/* 				 const double& height=ossim::nan()) const; */

   ossimRefPtr<ossimSpotDimapSupportData> theSupportData;

   //---
   // Image constant parameters:
   //---
   ossimFilename  theMetaDataFile;
   ossim_float64  theIllumAzimuth;  
   ossim_float64  theIllumElevation;
   ossim_float64  thePositionError;
   ossim_float64  theRefImagingTime;

   /** relative to full image */
   ossim_float64  theRefImagingTimeLine;
   
   ossim_float64  theLineSamplingPeriod;
   ossimDpt       theSpotSubImageOffset;
//   mutable NEWMAT::Matrix theSatToOrbRotation;
//   mutable NEWMAT::Matrix theOrbToEcfRotation;

   //---
   // Adjustable parameters:
   //---
   ossim_float64  theRollOffset;      // degrees
   ossim_float64  thePitchOffset;     // degrees
   ossim_float64  theYawOffset;       // degrees
   ossim_float64  theRollRate;        // degrees/sec
   ossim_float64  thePitchRate;       // degrees/sec
   ossim_float64  theYawRate;         // degrees/sec
   ossim_float64  theFocalLenOffset;  // percent deviation from nominal

TYPE_DATA
};

#endif /* #ifndef ossimSpot5Model_HEADER */
