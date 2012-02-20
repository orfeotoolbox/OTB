//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Base class for all map projections.
// 
//*******************************************************************
//  $Id: ossimMapProjection.h 20133 2011-10-12 19:03:47Z oscarkramer $

#ifndef ossimMapProjection_HEADER
#define ossimMapProjection_HEADER 1

#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimKeyword.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimDrect.h>
#include <iostream>

#include <ossim/base/ossimMatrix4x4.h>

class ossimKeywordlist;

class OSSIMDLLEXPORT ossimMapProjection : public ossimProjection
{
public:
   ossimMapProjection(const ossimEllipsoid& ellipsoid = ossimEllipsoid(),
                      const ossimGpt& origin=ossimGpt());

   ossimMapProjection(const ossimMapProjection& src);

   virtual ossimGpt origin()const;

   /**
    * All map projections will convert the world coordinate to an easting
    * northing (Meters).
    */
   virtual ossimDpt forward(const ossimGpt &worldPoint) const = 0;

   /**
    * Will take a point in meters and convert it to ground.
    */
   virtual ossimGpt inverse(const ossimDpt &projectedPoint)const = 0;

   virtual ossimDpt worldToLineSample(const ossimGpt &worldPoint)const;
   virtual void     worldToLineSample(const ossimGpt &worldPoint,
                                      ossimDpt&       lineSample)const;
   
   virtual ossimGpt lineSampleToWorld(const ossimDpt &projectedPoint)const;
   virtual void     lineSampleToWorld(const ossimDpt &projectedPoint,
                                      ossimGpt& gpt)const;
   /**
    * This is the pure virtual that projects the image point to the given
    * elevation above ellipsoid, thereby bypassing reference to a DEM. Useful
    * for projections that are sensitive to elevation.
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   heightAboveEllipsoid,
                                        ossimGpt&       worldPt) const;

   virtual void lineSampleToEastingNorthing(const ossimDpt& liineSample,
                                            ossimDpt& eastingNorthing)const;

   virtual void eastingNorthingToLineSample(const ossimDpt& eastingNorthing,
                                            ossimDpt&       lineSample)const;

   virtual void eastingNorthingToWorld(const ossimDpt& eastingNorthing,
                                       ossimGpt&       worldPt)const;

   
   /** @return The false easting. */
   virtual double getFalseEasting() const;

   /** @return The false northing. */
   virtual double getFalseNorthing() const;
   
   /**
    * Derived classes should implement as needed.
    * This implementation returns 0.0.
    * 
    * @return The first standard parallel.
    */
   virtual double getStandardParallel1() const;

   /**
    * Derived classes should implement as needed.
    * This implementation returns 0.0.
    * 
    * @return The second standard parallel.
    */
   virtual double getStandardParallel2() const;
   
   virtual void update();

   virtual void setPcsCode(ossim_uint32 pcsCode);

   //! Returns the EPSG PCS code or 32767 if the projection is a custom (non-EPSG) projection
   virtual ossim_uint32 getPcsCode()const;

   /**
    *  Returns the projection name.
    */
   virtual ossimString getProjectionName() const;
   
   /**
    * ACCESS METHODS: 
    */
   virtual double getA() const;
   virtual double getB() const;
   virtual double getF() const;
   
   virtual ossimDpt getMetersPerPixel() const;

   /**
    * Returns decimal degrees per pixel as an ossimDpt with
    * "x" representing longitude and "y" representing latitude.
    */
   virtual const ossimDpt& getDecimalDegreesPerPixel() const;
   
   virtual const ossimDpt&   getUlEastingNorthing() const;
   virtual const ossimGpt&   getUlGpt() const;
   virtual const ossimDatum* getDatum() const;
   const ossimEllipsoid&     getEllipsoid() const { return theEllipsoid; }
   const ossimGpt& getOrigin() const;
   virtual bool isGeographic()const;

   /**
    * Applies scale to theDeltaLonPerPixel, theDeltaLatPerPixel and
    * theMetersPerPixel data members (eg: theDeltaLonPerPixel *= scale.x).
    *
    * @param scale Multiplier to be applied to theDeltaLonPerPixel,
    * theDeltaLatPerPixel and theMetersPerPixel
    *
    * @param recenterTiePoint If true the will adjust the tie point by
    * shifting the original tie to the upper left corner, applying scale,
    * then shifting back by half of either the new
    * theDeltaLat/lon or theMetersPerPixel depending on if projection
    * isGeographic.
    */
   virtual void applyScale(const ossimDpt& scale, bool recenterTiePoint);
   
   /**
    * SET METHODS: 
    */
   virtual void setEllipsoid(const ossimEllipsoid& ellipsoid);
   virtual void setAB(double a, double b);

   /**
    * Sets theDatum to datum.
    * Sets theEllipsiod to datum's ellipsiod.
    * Calls update().
    */
   virtual void setDatum(const ossimDatum* datum);

   /**
    * Sets theOrigin to origin.
    * Sets theDatum to origin's datum.
    * Sets theEllipsiod to datum's ellipsiod.
    * Calls update().
    *
    * @param origin The projection origin.
    */
   virtual void setOrigin(const ossimGpt& origin);
   
   virtual void setMetersPerPixel(const ossimDpt& gsd);
   virtual void setDecimalDegreesPerPixel(const ossimDpt& gsd);
   virtual void setUlTiePoints(const ossimGpt& gpt);
   virtual void setUlTiePoints(const ossimDpt& eastingNorthing);
   virtual void setUlEastingNorthing(const  ossimDpt& ulEastingNorthing);
   virtual void setUlGpt(const  ossimGpt& ulGpt);
   virtual void assign( const  ossimProjection &aProjection);
   
   /**
    * Method to save the state of an object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;

   /**
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /**
    * Prints data members to stream.  Returns stream&.
    */
   virtual std::ostream& print(std::ostream& out) const;

   //! Compares this to arg projection and returns TRUE if the same. 
   //! NOTE: As currently implemented in OSSIM, map projections also contain image geometry 
   //! information like tiepoint and scale. This operator is only concerned with the map 
   //! specification and ignores image geometry differences.
   virtual bool operator==(const ossimProjection& projection) const;

   //! Computes the approximate resolution in degrees/pixel
   virtual void computeDegreesPerPixel();

   
   /**
    * This will go from the ground point and give
    * you an approximate meters per pixel. the Delta Lat
    * and delta lon will be in degrees.
    */
   virtual void computeMetersPerPixel();

   void setMatrix(double rotation,
                  const ossimDpt& scale,
                  const ossimDpt& translation);
   
   void setMatrixScale(const ossimDpt& scale);
   
   void setMatrixRotation(double rotation);

   void setMatrixTranslation(const ossimDpt& translation);

   /**
    * Utility method to snap the tie point to some multiple.
    *
    * This is used to overcome inaccuracies in formats like NITF's rpc tag
    * witch has a four digit decimal degree precision for the lat/lon offset.
    *
    * @param multiple Value to snap tie point to like 1.0.
    *
    * @param unitType OSSIM_METERS or OSSIM_DEGREES.
    *
    * EXAMPLE1:
    * projection:    ossimUtmProjection
    * tie point is:  551634.88, 4492036.16
    * multiple:      1.0
    * unitType:      OSSIM_METERS
    * new tie point: 551635.00, 4492036.00
    *
    * EXAMPLE2:
    * projection:    ossimEquDistCylProjection
    * tie point is:  -90.3246343      42.0297589
    * multiple:      0.0001
    * unitType:      OSSIM_DEGREES
    * new tie point: -90.3246 42.0298
    */
   void snapTiePointTo(ossim_float64 multiple, ossimUnitType unitType);
   void snapTiePointToOrigin();
                      
   void setElevationLookupFlag(bool flag);
   bool getElevationLookupFlag()const;
   ossimUnitType getModelTransformUnitType()const
   {
      return theModelTransformUnitType;
   }
   void setModelTransformUnitType(ossimUnitType unit)
   {
      theModelTransformUnitType = unit;
   }
   bool hasModelTransform()const
   {
      return (theModelTransformUnitType != OSSIM_UNIT_UNKNOWN);
   }

   /**
    * @brief Implementation of pure virtual
    * ossimProjection::isAffectedByElevation method.
    * @return false.
    */
   virtual bool isAffectedByElevation() const { return false; }
   
   void setProjectionUnits(ossimUnitType units) { theProjectionUnits = units; }
   ossimUnitType getProjectionUnits() const { return theProjectionUnits; }
   
   virtual bool isEqualTo(const ossimObject& obj, ossimCompareType compareType = OSSIM_COMPARE_FULL)const;


protected:
   
   virtual ~ossimMapProjection();

   //---
   // If theModelTransform is set this updates:
   // theDegreesPerPixel
   // theMetersPerPixel
   // theUlEastingNorthing
   // theUlGpt
   //---
   void updateFromTransform();

   /**
    * This method verifies that the projection parameters match the current
    * pcs code.  If not this will set the pcs code to 0.
    *
    * @return true if match, false if not.
    */
   //bool verifyPcsCodeMatches() const;
   
   ossimEllipsoid   theEllipsoid;
   ossimGpt         theOrigin;

   /**
    * This is only set if we want to have built in datum shifting
    */
   const ossimDatum* theDatum;
   
   /**
    * Holds the number of meters per pixel.
    * x = easting
    * y = northing
    */
   ossimDpt          theMetersPerPixel;

   /**
    * Hold the decimal degrees per pixel.
    * x = longitude
    * y = latitude
    */
   ossimDpt          theDegreesPerPixel;

   /** Hold tie point in decimal degrees. */
   ossimGpt          theUlGpt;

   /**
    * Hold tie point as easting northing.
    * x = easting,
    * y = northing
    */
   ossimDpt          theUlEastingNorthing;

   /**
    * Hold the false easting northing.
    * x = false easting,
    * y = false northing
    */
   ossimDpt          theFalseEastingNorthing;

   //! Projection Coordinate System(PCS) code. Mutable because they may update automatically
   //! after the projection is initialized, even in a const method. Normally set to 0 until the
   //! corresponding EPSG code can be determined. If the projection is NOT represented by any
   //! EPSG code, then the PCS is set to 32767.
   mutable ossim_uint32      thePcsCode;

   bool              theElevationLookupFlag;

   // Will always be a 4x4 matrix.
   // note:  only the first 2 dimensions will be used.
   // if the size is 0 then it will not be used
   //
   ossimMatrix4x4 theModelTransform; // goes from image to model
   ossimMatrix4x4 theInverseModelTransform; //goes from model back to image

   // Output Units of the transform
   //
   ossimUnitType theModelTransformUnitType;

   //! Linear units of the projection as indicated in the projection's specification:
   ossimUnitType theProjectionUnits;

TYPE_DATA
};


#endif
