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
//  $Id: ossimMapProjection.h 11805 2007-10-05 14:54:28Z dburken $

#ifndef ossimMapProjection_HEADER
#define ossimMapProjection_HEADER

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
   virtual ~ossimMapProjection();

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

   virtual void setPcsCode(ossim_uint16 pcsCode);
   virtual ossim_uint16 getPcsCode()const;
   
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
   virtual ossimDpt getDecimalDegreesPerPixel() const;
   
   virtual const ossimDpt&   getUlEastingNorthing() const;
   virtual const ossimGpt&   getUlGpt() const;
   virtual const ossimDatum* getDatum() const;

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
   virtual void setDatum(const ossimDatum* datum);
   virtual void setOrigin(const ossimGpt& origin);
   virtual void setMetersPerPixel(const ossimDpt& gsd);
   virtual void setDecimalDegreesPerPixel(const ossimDpt& gsd);
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

   virtual bool operator==(const ossimProjection& projection) const;

   /**
    * This will go from the ground point and give
    * you an approximate lat and lon per pixel. the Delta Lat
    * and delta lon will be in degrees.
    */
   virtual void computeDegreesPerPixel(const ossimGpt& ground,
                                       const ossimDpt& metersPerPixel,
                                       double &deltaLat,
                                       double &deltaLon);

   
   /**
    * This will go from the ground point and give
    * you an approximate meters per pixel. the Delta Lat
    * and delta lon will be in degrees.
    */
   virtual void computeMetersPerPixel(const ossimGpt& center,
                                      double deltaDegreesPerPixelLat,
                                      double deltaDegreesPerPixelLon,
                                      ossimDpt &metersPerPixel);

   /**
    * This will go from its internal origin of latitude and longitude and give
    * you an approximate meters per pixel. the Delta Lat
    * and delta lon will be in degrees.
    */
   virtual void computeMetersPerPixel(double deltaDegreesPerPixelLat,
                                      double deltaDegreesPerPixelLon,
                                      ossimDpt &metersPerPixel);

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
   
protected:

   /**
    * This method verifies that the projection parameters match the current
    * pcs code.  If not this will set the pcs code to 0.
    *
    * @return true if match, false if not.
    */
   bool verifyPcsCodeMatches();
   
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

   /** Projection Coordinate System(PCS) code. */
   ossim_uint16      thePcsCode;

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
TYPE_DATA
};


#endif
