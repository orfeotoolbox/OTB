//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION: Contains declaration of LandSat sensor model class.
//
// SOFTWARE HISTORY:
//   01AUG2002  Oscar Kramer, ImageLinks
//              Initial coding.
//   21NOV2002  Oscar Kramer, ImageLinks
//              Modified selection of adjustable parameters..
//
//*****************************************************************************
//  $Id: ossimLandSatModel.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimLandSatModel_HEADER
#define ossimLandSatModel_HEADER

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/projection/ossimMapProjection.h>
#include <ossim/base/ossimIpt.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefRay.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <iostream>

class ossimFfL7;
class ossimString;
class ossimMapProjection;

//******************************************************************************
//*
//* CLASS:  ossimLandSatModel
//*
//*******************************************************************************
class OSSIMDLLEXPORT ossimLandSatModel : public ossimSensorModel
{
public:
   /*!
    * CONSTRUCTORS:
    */
   ossimLandSatModel();
   ossimLandSatModel(const ossimFfL7& head);
   ossimLandSatModel(const ossimFilename& init_file);
   ossimLandSatModel(const ossimKeywordlist& geom_kwl);
   ossimLandSatModel(const ossimLandSatModel& rhs);
   virtual ossimObject* dup()const;

   enum ProjectionType
   {
      UNKNOWN_PROJECTION = 0,
      UTM_MAP,
      UTM_ORBIT,
      SOM_MAP,
      SOM_ORBIT
   };
   
   enum AdjustParamIndex
   {
      INTRACK_OFFSET = 0,
      CRTRACK_OFFSET,
      LINE_GSD_CORR,
      SAMP_GSD_CORR,
      ROLL_OFFSET,
      YAW_OFFSET,
      YAW_RATE,
      MAP_ROTATION,
      NUM_ADJUSTABLE_PARAMS // not an index
   };
   
   
   /*!
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;
   
   /*!
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   /*!
    * Writes a template of geom keywords processed by loadState and saveState
    * to output stream.
    */
   static void writeGeomTemplate(ostream& os);
   
   //***
   // Overrides base class pure virtual.
   //***
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
   
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

protected:

   virtual ~ossimLandSatModel();
   /*!
    * Initializes the model given a fast format header.
    */
   void initFromHeader(const ossimFfL7& head);
   
   virtual void initAdjustableParameters();
   void initMapProjection();

   //***
   // Image constant parameters:
   //***
   double           theIllumAzimuth;  
   double           theIllumElevation;
   double           theOrbitAltitude;
   double           theOrbitInclination;
   int              theMapZone;
   ossimDpt         theMapOffset;
   int              theWrsPathNumber;
   int              theWrsRowNumber;    
   double           theMeridianalAngle;   
   double           thePositionError;

   ProjectionType       theProjectionType;
   ossimRefPtr<ossimMapProjection>  theMapProjection;

   double           theMapAzimAngle; 
   double           theMapAzimCos;    
   double           theMapAzimSin;
   double           theMap2IcRotAngle;
   double           theMap2IcRotCos;
   double           theMap2IcRotSin;
   
   //***
   // Adjustable parameters:
   //***
   double           theIntrackOffset;
   double           theCrtrackOffset;
   double           theLineGsdCorr;   
   double           theSampGsdCorr;   
   double           theRollOffset;
   double           theYawOffset; 
   double           theYawRate; 
   double           theMapRotation; 

   //***
   // Quantities derived from the adjustable parameters:
   //***
   NEWMAT::Matrix   theRollRotMat;
   
   TYPE_DATA
};

#endif
