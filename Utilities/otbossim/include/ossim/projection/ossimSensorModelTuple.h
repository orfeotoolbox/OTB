//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Base class for tuple-based ossimSensorModel metric operations.
//----------------------------------------------------------------------------
// $Id: ossimSensorModelTuple.h 11614 2007-08-15 17:07:49Z dburken $
#ifndef ossimSensorModelTuple_HEADER
#define ossimSensorModelTuple_HEADER

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/elevation/ossimHgtRef.h>
#include <ossim/matrix/newmat.h>

// Forward class declarations:
class ossimEcefPoint;

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
    * @param obs     Vector of image point observations.
    * @param pt      Intersected ECF position of point.
    * @param covMat  3X3 ECF position covariance matrix [m].
    *
    * @return true on success, false on error.
    */
   ossimSensorModelTuple::IntersectStatus intersect(
      const DptSet_t         obs,
      ossimEcefPoint&  pt,
      NEWMAT::Matrix&  covMat) const;
   
   /**
    * @brief Single-image/DEM intersection method.
    *
    * @param img     Image set index of current image.
    * @param obs     Image point observations.
    * @param pt      Intersected ECF position of point.
    * @param covMat  3X3 ECF position covariance matrix [m].
    *
    * @return true on success, false on error.
    */
   ossimSensorModelTuple::IntersectStatus intersect(
      const ossim_int32&     img,
      const ossimDpt&        obs,
      ossimEcefPoint&  pt,
      NEWMAT::Matrix&  covMat) const;
   
   /**
    * @brief Single-image/height intersection method.
    *
    * @param img                  Image set index of current image.
    * @param obs                  Image point observations.
    * @param heightAboveEllipsoid Desired intersection height [m].
    * @param pt                   Intersected ECF position of point.
    * @param covMat               3X3 ECF position covariance matrix [m].
    *
    * @return true on success, false on error.
    */
   ossimSensorModelTuple::IntersectStatus intersect(
      const ossim_int32&     img,
      const ossimDpt&        obs,
      const ossim_float64&   heightAboveEllipsoid,
      ossimEcefPoint&  pt,
      NEWMAT::Matrix&  covMat) const;
            
   /**
    * @brief Set intersection surface accuracy method.
    *
    * @param surfCE90 90% CE [m].
    * @param surfLE90 90% LE [m].
    *
    * @return true on success, false on exception.
    * Entry of negative value(s) indicates "no DEM" error prop for RPC
    */
   bool setIntersectionSurfaceAccuracy(const ossim_float64 surfCE90,
                                       const ossim_float64 surfLE90);

protected:

private:
   std::vector<ossimSensorModel*> theImages;

   ossim_int32    theNumImages;
   
   ossim_float64  theSurfCE90;
   ossim_float64  theSurfLE90;
   bool           theSurfAccSet;
   bool           theSurfAccRepresentsNoDEM;
   
   /**
    * @brief Compute single image intersection covariance matrix.
    *
    * @param img      Image set index of current image.
    * @param obs      Image point observations.
    * @param ptG      Current ground estimate.
    * @param cRefType Current height reference type.
    * @param covMat   3X3 ECF position covariance matrix.
    *
    * @return true on success, false on error.
    */
   bool computeSingleInterCov(const ossim_int32& img,
                              const ossimDpt&    obs,
                              const ossimGpt&    ptG,
                              HeightRefType_t    cRefType,
                              NEWMAT::Matrix&    covMat) const;
      
   
   /**
    * @brief Get observation equation components.
    *
    * @param img   Image set index of current image.
    * @param iter  Current iteration.
    * @param obs   Observations.
    * @param ptEst Current ground estimate.
    * @param resid Observation residuals.
    * @param B     Matrix of partials of observations WRT X,Y,Z.
    * @param W     Weight matrix of observations.
    *
    * @param img Image set index of current image.
    */
   bool getGroundObsEqComponents(const ossim_int32 img,
                                 const ossim_int32 iter,
                                 const ossimDpt& obs,
                                 const ossimGpt& ptEst,
                                 ossimDpt& resid,
                                 NEWMAT::Matrix& B,
                                 NEWMAT::SymmetricMatrix& W) const;

   NEWMAT::Matrix invert(const NEWMAT::Matrix& m) const;

};


#endif /* #ifndef ossimSensorModelTuple_HEADER */
