//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Base class for tuple-based ossimSensorModel metric operations.
//----------------------------------------------------------------------------
// $Id: ossimSensorModelTuple.h 10375 2007-01-25 20:01:28Z dburken $
#ifndef ossimSensorModelTuple_HEADER
#define ossimSensorModelTuple_HEADER

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/elevation/ossimHgtRef.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/matrix/newmatio.h>

typedef vector<ossimDpt> DptSet_t;

class OSSIM_DLL ossimSensorModelTuple
{
public:

   enum DeriveMode
   {
      OBS_INIT =-99,
      EVALUATE =-98,
      P_WRT_X = -1,
      P_WRT_Y = -2,
      P_WRT_Z = -3
   };

   enum IntersectStatus
   {
      OP_SUCCESS      = 0,
      ERROR_PROP_FAIL = 1,
      OP_FAIL         = 2
   };

   /** @brief default constructor */
   ossimSensorModelTuple();

   /** @brief virtual destructor */
   ~ossimSensorModelTuple();

   /**
    * @brief Method to add an image to the tuple.
    */
   void addImage(ossimSensorModel* image);
   
   /**
    * @brief print method.
    */
   std::ostream& print(std::ostream& out) const;

   
   /**
    * @brief Multi-image intersection method.
    *
    * @param obs Vector of image point observations.
    *
    * @param pt Intersected ECF position of point.
    *
    * @param CE90 90% horizontal circular error.
    *
    * @param LE90 90% vertical linear error.
    *
    * @return true on success, false on error.
    */
   ossimSensorModelTuple::IntersectStatus intersect(DptSet_t obs,
                                                    ossimEcefPoint& pt,
                                                    ossim_float64& CE90,
                                                    ossim_float64& LE90);
   
   /**
    * @brief Single-image/DEM intersection method.
    *
    * @param obs Vector of image point observations.
    *
    * @param pt Intersected ECF position of point.
    *
    * @param CE90 90% horizontal circular error.
    *
    * @param LE90 90% vertical linear error.
    *
    * @return true on success, false on error.
    */
   ossimSensorModelTuple::IntersectStatus intersect(const ossimDpt& obs,
                                                    ossimEcefPoint& pt,
                                                    ossim_float64& CE90,
                                                    ossim_float64& LE90);
   
   /**
    * @brief Single-image/height intersection method.
    *
    * @param obs Vector of image point observations.
    *
    * @param heightAboveEllipsoid Desired intersection height.
    *
    * @param pt Intersected ECF position of point.
    *
    * @param CE90 90% horizontal circular error.
    *
    * @param LE90 90% vertical linear error.
    *
    * @return true on success, false on error.
    */
   ossimSensorModelTuple::IntersectStatus intersect(
      const ossimDpt& obs,
      const double& heightAboveEllipsoid,
      ossimEcefPoint& pt,
      ossim_float64& CE90,
      ossim_float64& LE90);

protected:

private:
   std::vector<ossimSensorModel*> theImages;
   
   /**
    * @brief Compute single image intersection covariance matrix.
    *
    * @param obs Vector of image point observations.
    *
    * @param ptG Current ground estimate.
    *
    * @param cRefType Current height reference type.
    *
    * @return true on success, false on error.
    */
   bool computeSingleInterCov(const ossimDpt& obs,
                              const ossimGpt& ptG,
                              HeightRefType_t cRefType,
                              NEWMAT::Matrix& covMat);
   
   /**
    * @brief Compute 90% circular/linear error (CE/LE).
    *
    * @param covMat ENU covariance matrix.
    *
    * @param posG Current ground estimate.
    *
    * @param CE90 90% circular error (ft).
    *
    * @param LE90 90% linear error (ft).
    *
    * @return true on success, false on error.
    */
   bool computeCE_LE(const NEWMAT::Matrix& covMat,
                     const ossimGpt& posG,
                     ossim_float64& CE90,
                     ossim_float64& LE90);
   
   
   /**
    * @brief Get observation equation components.
    *
    * @param img Image set index of current image.
    *
    * @param iter Current iteration.
    *
    * @param obs Range/Doppler observations.
    *
    * @param ptEst Current ground estimate.
    *
    * @param resid Range/Doppler observation residuals.
    *
    * @param B Matrix of partials of rng/Dop WRT X,Y,Z.
    *
    * @param W Weight matrix of rng/Dop.
    *
    * @param img Image set index of current image.
    */
   bool getGroundObsEqComponents(const ossim_int32 img,
                                 const ossim_int32 iter,
                                 const ossimDpt& obs,
                                 const ossimGpt& ptEst,
                                 ossimDpt& resid,
                                 NEWMAT::Matrix& B,
                                 NEWMAT::SymmetricMatrix& W);

   NEWMAT::Matrix invert(const NEWMAT::Matrix& m) const;

};


#endif /* #ifndef ossimSensorModelTuple_HEADER */
