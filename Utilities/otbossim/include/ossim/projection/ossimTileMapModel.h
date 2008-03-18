//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION: Contains declaration of TileMap sensor model class.
//
// SOFTWARE HISTORY:
//   23JAN2008  Jordi Inglada, CNES
//              Initial coding.
//
//*****************************************************************************
//  $Id: ossimTileMapModel.h 10384 2007-01-26 20:32:12Z gpotts $

#ifndef ossimTileMapModel_HEADER
#define ossimTileMapModel_HEADER

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
//* CLASS:  ossimTileMapModel
//*
//*******************************************************************************
class OSSIMDLLEXPORT ossimTileMapModel : public ossimSensorModel
{
public:
   /*!
    * CONSTRUCTORS:
    */
   ossimTileMapModel();
   ossimTileMapModel(const ossimFfL7& head);
   ossimTileMapModel(const ossimFilename& init_file);
   ossimTileMapModel(const ossimKeywordlist& geom_kwl);
   ossimTileMapModel(const ossimTileMapModel& rhs);
   
   virtual ~ossimTileMapModel();

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
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const { return 0; } // TBR
   
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
  virtual void worldToLineSample(const ossimGpt& ground_point,
				 ossimDpt&       img_pt) const;
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
  virtual void lineSampleToWorld(const ossimDpt& image_point,
                                        ossimGpt&       worldPoint) const;

  /*!
   * Set/Get the depth of the quadtree decomposition
   *
*/
  void setDepth(unsigned char depth){
    qDepth = depth;
  }
  unsigned char getDepth()
  {
    return qDepth;
  }
   
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

   unsigned char qDepth;
};

#endif
