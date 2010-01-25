//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION: Contains sensor model class declaration for "Frame Camera
//   Scanned Image" (FCSI, pronounced "foxy"). 
//   
//   This model represents image data that was scanned from film exposed by a
//   conventional aerial frame camera. The data members defined in this class
//   represent the fundamental quantities needed for the transformation.
//   Typically, a different set of parameters will be provided in the metadata.
//   An interface (support data) class will need to be implemented to convert
//   a particular system's format to the quantities needed by this model.
// 
//   Coordinate systems in use are described here:
//
//   Pixel    -- line/sample in the digital image. The origin of this system is
//               the upper left corner of the image, and represents a LEFT-
//               HANDED coordinate system with Y (line) positive down and X
//               (sample) positive to the right.
//
//   Film     -- x, y related to pixel space by affine transform (which includes
//               mirror reflection due to left-handed system above) represented
//               by thePrincipalPoint and the theScanXformMatrix. The latter
//               contains scale, skew, and rotation/reflection. The film
//               coordinates are adjusted for radial and decentering distortion.
//
//   Camera/  -- 3D system containing film plane and optical (Z) axis. Given the
//   Platform    focal length (Z) and the film point (X,Y), a 3D ray direction
//               is established in this space. This space is also termed "LSR"
//               in the code. If there is a rotation between the film/optical
//               axes and the platform (interior orientation), then that
//               transformation must be included as part of the LSR-to-ECF
//               rotation by the metadata reader object.
//
//   ECF      -- Universal earth frame of reference. The imaging ray direction
//               is transformed from Camera space to ECF by theAdjLsrToEcfRot.
//
//   There are two rotations to go from Camera to ECF. First is the initial
//   camera orientation as derived from the metadata, combining the interior
//   orientation (relation between camera and platform) and the exterior
//   orientation dependent on the platform attitude w.r.t. the ECF system.
//   The second rotation is a differential rotation due to the attitude
//   adjustable params. These are arbitrarily taken as rotations about the ECF
//   XYZ axes, not the platform attitude axes of roll/pitch/yaw. The
//   combined rotation is stored in theAdjLsrToEcfRot, which is precomputed
//   in updateModel() once with each adjustment.
//
//   When the time comes to incorporate least-squares parameter adjustment, care
//   must be taken to propagate the a priori sigmas for position and attitude
//   (including both internal and external orientation angles) into the ECF
//   coordinates. This also applies to the affine transform parameters used in
//   transforming pixels to film coordinates. The propagated uncertainties can
//   then be used to assign the adjustable parameter sigmas array (and
//   covariance when we get to that...)
//
// SOFTWARE HISTORY:
//   16JAN2003  Oscar Kramer, ImageLinks
//              Initial coding
//
//*****************************************************************************
//  $Id: ossimFcsiModel.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimFcsiModel_HEADER
#define ossimFcsiModel_HEADER

#include <iostream>
using namespace std;

#include <ossim/projection/ossimSensorModel.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimMatrix3x3.h>
#include <ossim/projection/ossimRadialDecentLensDistortion.h>

//******************************************************************************
//
// CLASS:  ossimFcsiModel
//
//******************************************************************************
class OSSIM_DLL ossimFcsiModel : public ossimSensorModel
{
public:
   /*!
    * CONSTRUCTORS:
    */
   ossimFcsiModel();
   ossimFcsiModel(const ossimFilename& init_file);
   ossimFcsiModel(const ossimKeywordlist& geom_kwl);
   ossimFcsiModel(const ossimFcsiModel& rhs);
   

   enum AdjustParamIndex
   {
      X_POS = 0,
      Y_POS,
      Z_POS,
      X_ROT,
      Y_ROT,
      Z_ROT,
      FOCAL_LEN,
      SCAN_SKEW,
      NUM_ADJUSTABLE_PARAMS // not an index
   };

   /*!
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const { return new ossimFcsiModel(*this); }
   
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
   
   /*!
    * Overrides base class pure virtual.
    */
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
    * Rigorous inverse transform implented, overrides base-class' iterative
    * solution.
    */
   virtual void worldToLineSample(const ossimGpt& world_point,
                                  ossimDpt&       image_point) const;
      
   /*!
    * Following a change to the adjustable parameter set, this virtual
    * is called to permit instances to compute derived quantities after
    * parameter change.
    */
   virtual void updateModel();

   /*!
    * Global keywords for use by this model:
    */
   static const char* PRINCIPAL_POINT_X_KW;
   static const char* PRINCIPAL_POINT_Y_KW;
   static const char* SCAN_SCALE_MATRIX_00_KW;
   static const char* SCAN_SCALE_MATRIX_01_KW;
   static const char* SCAN_SCALE_MATRIX_10_KW;
   static const char* SCAN_SCALE_MATRIX_11_KW;
   static const char* SCAN_SKEW_ANGLE_KW;
   static const char* SCAN_ROTATION_ANGLE_KW;
   static const char* FOCAL_LENGTH_KW;
   static const char* PLATFORM_POSITION_X_KW;
   static const char* PLATFORM_POSITION_Y_KW;
   static const char* PLATFORM_POSITION_Z_KW;
   static const char* CAMERA_ORIENTATION_MATRIX_ELEM_KW;

   /*!
    * ossimOptimizableProjection
    */
   inline virtual bool useForward()const {return true;} //!ground to image faster (you don't need DEM)

protected:
   virtual ~ossimFcsiModel() { delete theOpticalDistortion; }

   /*!
    * Assigns initial default values to adjustable parameters and related
    * members.
    */
   void initAdjustableParameters();

   //***
   // Affine transform variables for scanned image to film coordinate
   // transformation, in order of application:
   //***
   ossimDpt       thePrincipalPoint; // principal point location in pixels
   double         theScanScaleMatrix[4];// converts pixel to film X, Y mm
   double         theScanSkew;       // degrees
   double         theScanRotation;   // degrees left rotation of scanned image

   //***
   // Transform variables for film coordinates to platform LSR (interior
   // orientation):
   //***
   double         theFocalLen;     // millimeters
   ossimRadialDecentLensDistortion* theOpticalDistortion;
   
   //***
   // Transform variables for platform LSR to ECF coordinates (interior/exterior
   // orientation and offset):
   //***
   ossimEcefPoint thePlatformPos;
   NEWMAT::Matrix theLsrToEcfRot;
   
   //***
   // Un-normalized adjustable parameters:
   //***
   ossimEcefVector theEcfOffset;      // meters
   double          theXrotCorr;       // degrees
   double          theYrotCorr;       // degrees
   double          theZrotCorr;       // degrees
   double          theFocalOffset;    // millimeters
   double          theScanSkewCorr;   // degrees

   //***
   // Quantities derived from adjustables and assigned in updateModel():
   //***
   ossimEcefPoint theAdjPlatformPos;
   double         theAdjFocalLen;
   NEWMAT::Matrix theAdjLsrToEcfRot;
   NEWMAT::Matrix theAdjEcfToLsrRot;
   double         theScanXformMatrix[4]; 
   double         theInvScanXformMatrix[4];

   TYPE_DATA
};

#endif
