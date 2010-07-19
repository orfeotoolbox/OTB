//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts
// 
//********************************************************************
// $Id: ossimBilinearProjection.h 17497 2010-06-02 01:43:43Z gpotts $
#ifndef ossimBilinearProjection_HEADER
#define ossimBilinearProjection_HEADER

#include <vector>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/projection/ossimOptimizableProjection.h>
#include <ossim/projection/ossimProjection.h>
#include <ossim/base/ossimLeastSquaresBilin.h>

class OSSIM_DLL ossimBilinearProjection : public ossimProjection,
                                          public ossimOptimizableProjection
     
{
public:
   ossimBilinearProjection();
   ossimBilinearProjection(const ossimBilinearProjection& rhs);
   ossimBilinearProjection(const ossimDpt& ul,
                           const ossimDpt& ur,
                           const ossimDpt& lr,
                           const ossimDpt& ll,
                           const ossimGpt& ulg,
                           const ossimGpt& urg,
                           const ossimGpt& lrg,
                           const ossimGpt& llg);
   virtual ossimObject *dup()const;
   virtual ossimGpt origin()const;
   
   virtual void worldToLineSample(const ossimGpt& worldPoint,
                                  ossimDpt&       lineSampPt) const;
   /*!
    * METHOD: lineSampleToWorld()
    * Performs the inverse projection from line, sample to ground (world):
    */
   virtual void lineSampleToWorld(const ossimDpt& lineSampPt,
                                  ossimGpt&       worldPt) const;
   
   /*!
    * METHOD: lineSampleHeightToWorld
    * This is the pure virtual that projects the image point to the given
    * elevation above ellipsoid, thereby bypassing reference to a DEM. Useful
    * for projections that are sensitive to elevation (such as sensor models).
    */
   virtual void lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                        const double&   heightAboveEllipsoid,
                                        ossimGpt&       worldPt) const;
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   virtual bool operator==(const ossimProjection& projection) const;
   virtual ossimDpt getMetersPerPixel() const;

   virtual std::ostream& print(std::ostream& out) const;
   
   /*!
    * METHOD: setTiePoints
    * add any number of tie points and calculate bilinear fit
    * returns the ground error variance (=RMS^2), in meters^2
    */
   virtual ossim_float64 setTiePoints(const std::vector<ossimDpt>& lsPt, 
                                      const std::vector<ossimGpt>& geoPt);
   
   /*
    * optimizable interface
    */
   virtual bool setupOptimizer(const ossimString& setup);
   
   inline virtual bool useForward()const {return false;}
   //! better go from image to ground, also means that errors variance are in squared meters
   
   virtual ossim_uint32 degreesOfFreedom()const;
   virtual double optimizeFit(const ossimTieGptSet& tieSet, double* targetVariance=0);
   
   /**
    * @brief Implementation of pure virtual
    * ossimProjection::isAffectedByElevation method.
    * @return false.
    */
   virtual bool isAffectedByElevation() const { return false; } 
   
   //! Access method for tie point information
   void getTiePoints(std::vector<ossimDpt>& lsPt, std::vector<ossimGpt>& geoPt) const;

protected:
   virtual ~ossimBilinearProjection();
   void initializeBilinear();

   /**
    * Checks theLineSamplePt for nans.
    * @return true if any point has a nan.
    * @note If theLineSamplePt size is 0 this returns false.
    */
   bool dPtsHaveNan() const;
   
   /**
    * Checks theGeographicPt for nans.
    * @return true if any point has a nan.
    * @note If theLineSamplePt size is 0 this returns false.
    */
   bool gPtsHaveNan() const;

   /**
    * @return The mid point of theLineSamplePts.
    */
   ossimDpt midLineSamplePt() const;
   
   std::vector<ossimDpt>  theLineSamplePt;
   std::vector<ossimGpt>  theGeographicPt;
   ossimLeastSquaresBilin theLatFit;
   ossimLeastSquaresBilin theLonFit;
   ossimLeastSquaresBilin theXFit;
   ossimLeastSquaresBilin theYFit;

   bool theInterpolationPointsHaveNanFlag;
   bool theInverseSupportedFlag;

TYPE_DATA   
};

#endif /* #ifndef ossimBilinearProjection_HEADER */
