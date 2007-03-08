//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Height reference class.
//----------------------------------------------------------------------------
#ifndef ossimHgtRef_HEADER
#define ossimHgtRef_HEADER

#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimColumnVector3d.h>
#include <ossim/matrix/newmat.h>
#include <ossim/matrix/newmatap.h>
#include <ossim/matrix/newmatio.h>

typedef enum HeightRefType_t
{
   AT_HGT = 0,
   AT_DEM = 1
};


class OSSIM_DLL ossimHgtRef
{
public:

   /**
    * @brief constructor.
    */
   ossimHgtRef(HeightRefType_t cRefType);
   ossimHgtRef(HeightRefType_t cRefType, const ossim_float64& atHgt);

   /**
    * @brief virtual destructor.
    */
   virtual ~ossimHgtRef();
   
   /**
    * @brief Method to get height reference type (ellipsoid or DEM).
    */
   HeightRefType_t getHeightRefType() const {return theCurrentHeightRefType;}
   
   /**
    * @brief Method to get height reference.
    *
    * @param pg Reference point.
    *
    * @return Height at reference point.
    */
   virtual ossim_float64 getRefHeight(const ossimGpt& pg) const;
   
   /**
    * @brief Method to get height covariance matrix.
    *
    * @param pg Reference point.
    *
    * @param cov 3X3 covariance matrix.
    *
    * @return true on success, false on error.
    */
   virtual bool getHeightCovMatrix(const ossimGpt& pg, NEWMAT::Matrix& cov) const;
   
   /**
    * @brief Method to get local terrain normal unit vector (slope).
    *
    * @param pg Reference point.
    *
    * @return ossimColumnVector3D.
    */
   virtual ossimColumnVector3d getLocalTerrainNormal(const ossimGpt& pg) const;


protected:

private:
   HeightRefType_t theCurrentHeightRefType;
   ossim_float64 theCurrentRefHeight;

};

#endif /* #ifndef ossimHgtRef_HEADER */
