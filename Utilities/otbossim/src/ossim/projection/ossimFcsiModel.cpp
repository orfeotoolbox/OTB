//*****************************************************************************
// FILE: ossimFcsiModel.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// LGPL
//
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains sensor model implementation for "Frame Camera Scanned
//   Image" (FCSI, pronounced "foxy"). 
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
// $Id: ossimFcsiModel.cpp 13005 2008-06-08 20:26:36Z dburken $

#include <cstdlib>
#include <fstream>
#include <sstream>

#include <ossim/projection/ossimFcsiModel.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimEcefRay.h>

RTTI_DEF1(ossimFcsiModel, "ossimFcsiModel", ossimSensorModel);

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimFcsiModel:exec");
static ossimTrace traceDebug ("ossimFcsiModel:debug");

//***
// File-scope constants:
//***
static const int MODEL_VERSION_NUMBER  = 1;
static const ossimString PARAM_NAMES[] ={"x_pos_offset",
                                         "y_pos_offset",
                                         "z_pos_offset",
                                         "x_rot_corr",
                                         "y_rot_corr",
                                         "z_rot_corr",
                                         "foc_len_offset",
                                         "scan_skew_corr"};
static const ossimString PARAM_UNITS[] ={"meters",
                                         "meters",
                                         "meters",
                                         "degrees",
                                         "degress",
                                         "degress",
                                         "millimeters",
                                         "degrees"};

//***
// Public FCSI Keywords:
//***
const char* ossimFcsiModel::PRINCIPAL_POINT_X_KW   = "principal_point_x";
const char* ossimFcsiModel::PRINCIPAL_POINT_Y_KW   = "principal_point_y";
const char* ossimFcsiModel::SCAN_SCALE_MATRIX_00_KW = "scan_scale_matrix_00";
const char* ossimFcsiModel::SCAN_SCALE_MATRIX_01_KW = "scan_scale_matrix_01";
const char* ossimFcsiModel::SCAN_SCALE_MATRIX_10_KW = "scan_scale_matrix_10";
const char* ossimFcsiModel::SCAN_SCALE_MATRIX_11_KW = "scan_scale_matrix_11";
const char* ossimFcsiModel::SCAN_ROTATION_ANGLE_KW = "scan_rotation_angle";
const char* ossimFcsiModel::SCAN_SKEW_ANGLE_KW     = "scan_skew_angle";
const char* ossimFcsiModel::FOCAL_LENGTH_KW        = "focal_length";
const char* ossimFcsiModel::PLATFORM_POSITION_X_KW = "platform_position_x";
const char* ossimFcsiModel::PLATFORM_POSITION_Y_KW = "platform_position_y";
const char* ossimFcsiModel::PLATFORM_POSITION_Z_KW = "platform_position_z";
const char* ossimFcsiModel::CAMERA_ORIENTATION_MATRIX_ELEM_KW
                               = "camera_orientation_matrix_elem_";

//*****************************************************************************
//  DEFAULT CONSTRUCTOR: ossimFcsiModel()
//
//  Note: Using this constructor results in an uninitialized model. It is
//  expected that either a loadState with valid keywordlist is called, or that
//  the individual set-methods (including the base-class' set methods are
//  called. When set-methods are used, a final call to completeInitialization()
//  (declared in ossimSensorModel) is required to give the model a chance to
//  "digest" the values previously set.
//  
//*****************************************************************************
ossimFcsiModel::ossimFcsiModel()
   :
   ossimSensorModel       (),
   thePrincipalPoint      (0, 0),
   theScanSkew            (0.0),  
   theScanRotation        (0.0),
   theOpticalDistortion   (0),
   theLsrToEcfRot         (3, 3),
   theEcfOffset           (0, 0, 0),
   theXrotCorr            (0.0),
   theYrotCorr            (0.0),
   theZrotCorr            (0.0),
   theFocalOffset         (0.0),
   theScanSkewCorr        (0.0),
   theAdjLsrToEcfRot      (3, 3),
   theAdjEcfToLsrRot      (3, 3)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: entering..." << std::endl;
   theScanScaleMatrix[0] = 1.0;
   theScanScaleMatrix[1] = 0.0;
   theScanScaleMatrix[2] = 0.0;
   theScanScaleMatrix[3] = 1.0;
   theScanXformMatrix[0] = 1.0;
   theScanXformMatrix[1] = 0.0;
   theScanXformMatrix[2] = 0.0;
   theScanXformMatrix[3] = 1.0;
   initAdjustableParameters();
   setErrorStatus(); // indicate uninitialized
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: returning..." << std::endl;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimFcsiModel(filename)
//  
//  Constructs model from a keywordlist filename
//  
//*****************************************************************************
ossimFcsiModel::ossimFcsiModel(const ossimFilename& init_file)
   :
   ossimSensorModel       (),
   thePrincipalPoint      (0, 0),
   theScanSkew            (0.0),  
   theScanRotation        (0.0),
   theOpticalDistortion   (0),
   theLsrToEcfRot         (3, 3),
   theEcfOffset           (0, 0, 0),
   theXrotCorr            (0.0),
   theYrotCorr            (0.0),
   theZrotCorr            (0.0),
   theFocalOffset         (0.0),
   theScanSkewCorr        (0.0),
   theAdjLsrToEcfRot      (3, 3),
   theAdjEcfToLsrRot      (3, 3)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: entering..." << std::endl;

   initAdjustableParameters();
   ossimKeywordlist kwl (init_file);
   loadState(kwl);

   if (traceExec()) ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: Exited..." << std::endl;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimFcsiModel(kwl)
//  
//  Constructs model from keywordlist geometry file
//  
//*****************************************************************************
ossimFcsiModel::ossimFcsiModel(const ossimKeywordlist& geom_kwl)
   :
   ossimSensorModel       (),
   thePrincipalPoint      (0, 0),
   theScanSkew            (0.0),  
   theScanRotation        (0.0),
   theOpticalDistortion   (0),
   theLsrToEcfRot         (3, 3),
   theEcfOffset           (0, 0, 0),
   theXrotCorr            (0.0),
   theYrotCorr            (0.0),
   theZrotCorr            (0.0),
   theFocalOffset         (0.0),
   theScanSkewCorr        (0.0),
   theAdjLsrToEcfRot      (3, 3),
   theAdjEcfToLsrRot      (3, 3)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: entering..." << std::endl;

   initAdjustableParameters();
   loadState(geom_kwl);

   if (traceExec()) ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: Exited..." << std::endl;
}

//*****************************************************************************
//  COPY CONSTRUCTOR: 
//*****************************************************************************
ossimFcsiModel::ossimFcsiModel(const ossimFcsiModel& foxy)
   :
   ossimSensorModel     (foxy),
   thePrincipalPoint    (foxy.thePrincipalPoint),
   theScanSkew          (foxy.theScanSkew),  
   theScanRotation      (foxy.theScanRotation),
   theFocalLen          (foxy.theFocalLen),
   thePlatformPos       (foxy.thePlatformPos),
   theLsrToEcfRot       (foxy.theLsrToEcfRot),
   theEcfOffset         (foxy.theEcfOffset),
   theXrotCorr          (foxy.theXrotCorr),
   theYrotCorr          (foxy.theYrotCorr),
   theZrotCorr          (foxy.theZrotCorr),
   theFocalOffset       (foxy.theFocalOffset),
   theScanSkewCorr      (foxy.theScanSkewCorr)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: entering..." << std::endl;

   if(foxy.theOpticalDistortion)
   {
      //
      // Finish the copy:
      //
      theOpticalDistortion =
         new ossimRadialDecentLensDistortion (*(foxy.theOpticalDistortion));
   }

   for (int i=0; i<4; i++)
      theScanScaleMatrix[i] = foxy.theScanScaleMatrix[i];

   //***
   // Compute quantities derived from adjustables:
   //***
   updateModel();
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: returning..." << std::endl;
}

//*****************************************************************************
//  METHOD: ossimFcsiModel::lineSampleHeightToWorld()
//  
//  Performs the line/sample to groundpoint projection given an elevation.
//
//  1. Compute ECF Imaging ray (see imagingRay() method below)
//  5. Intersect imaging ray with elevation surface.
//
//*****************************************************************************
void ossimFcsiModel::lineSampleHeightToWorld(const ossimDpt& image_point,
                                             const double&   height,
                                             ossimGpt&       gpt) const
{
   bool debug = false;  // setable via interactive debugger
   if (traceExec() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: entering..." << std::endl;
   
   //***
   // Extrapolate if point is outside image:
   //***
   if (!insideImage(image_point))
   {
      gpt = extrapolate(image_point, height);
   }

   else
   {
      //***
      // First establish imaging ray from image point:
      //***
      ossimEcefRay imaging_ray;
      imagingRay(image_point, imaging_ray);
      ossimEcefPoint Pecf (imaging_ray.intersectAboveEarthEllipsoid(height));
      gpt = ossimGpt(Pecf);

      if (traceDebug() || debug)
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "Pecf = " << Pecf << std::endl;
         ossimNotify(ossimNotifyLevel_DEBUG) << "gpt = " << gpt << std::endl;
      }
   }

   if (traceExec() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::ossimFcsiModel: returning..." << std::endl;
}
   
   
//*****************************************************************************
//  METHOD: ossimFcsiModel::imagingRay()
//
//  Establishes an origin (at the platform position) and direction of the
//  imaging ray as follows:
//
//  1. Remove any pixel offset into the image (in the case where the digital
//     image is a sub-image of a larger scan).
//  2. Transform pixel coordinates to film point (with inherent lens distortion)
//  3. Remove radial and decentering lens distortion to arrive at ideal film
//     coordinates.
//  4. Establish an internal imaging ray direction vector given the focal length
//  5. Transform ray direction vector to ECF coordinates and assign origin.
//
//*****************************************************************************
void ossimFcsiModel::imagingRay(const ossimDpt& image_point,
                                ossimEcefRay&   image_ray) const
{
   bool debug = false;  // setable via interactive debugger: "set debug = true"
   if (traceExec() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::imagingRay: entering..." << std::endl;
   
   //***
   // In case this is a sub-image of a full image scan, need to add the
   // sub-image offset maintained in the base-class:
   //***
   ossimDpt p0 (image_point + theSubImageOffset);

   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "image_point = " << image_point << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "theSubImageOffset = " << theSubImageOffset << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "p0 = " << p0 << std::endl;
   }
   
   //***
   // Apply pixel to film coordinates affine transform:
   //***
   ossimDpt p1 (p0 - theRefImgPt);
   ossimDpt f1 (theScanXformMatrix[0]*p1.x + theScanXformMatrix[1]*p1.y,
                theScanXformMatrix[2]*p1.x + theScanXformMatrix[3]*p1.y);
   ossimDpt film (f1 - thePrincipalPoint);
   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "theRefImgPt = " << theRefImgPt << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "p1 = " << p1 << std::endl;
      for (int i=0; i<4; i++)
      {
         ossimNotify(ossimNotifyLevel_DEBUG) << "theScanXformMatrix["<<i<<"] = "<<theScanXformMatrix[i] << std::endl;
      }
      ossimNotify(ossimNotifyLevel_DEBUG) << "f1 = " << f1 << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "thePrincipalPoint = " << thePrincipalPoint << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "film (distorted) = " << film << std::endl;
   }

   //***
   // Correct film point for optical (radial, decentering) distortion:
   //***
   if (theOpticalDistortion)
      theOpticalDistortion->inverse(film);
   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "film (undistorted) = " << film << std::endl;
      if(theOpticalDistortion)
      {
         theOpticalDistortion->print(ossimNotify(ossimNotifyLevel_DEBUG));
      }
   }
   
   //***
   // Establish image ray in  camera-space (LSR) coordinates, then rotate to the
   // ECF. This rotation contains interior orientation, exterior orientation,
   // and attitude correction:
   //***
   ossimColumnVector3d cam_ray_dir (film.x, film.y, -theAdjFocalLen);
   ossimEcefVector ecf_ray_dir (theAdjLsrToEcfRot*cam_ray_dir);
   ecf_ray_dir = ecf_ray_dir*(1.0/ecf_ray_dir.magnitude());
   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "theAdjFocalLen = " << theAdjFocalLen << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "cam_ray_dir = " << cam_ray_dir << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "theAdjLsrToEcfRot = \n" << theAdjLsrToEcfRot << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "ecf_ray_dir = " << ecf_ray_dir << std::endl;
   }
   
   //***
   // Establish ECF Ray object to represent imaging ray:
   //***
   image_ray.setOrigin(theAdjPlatformPos);
   image_ray.setDirection(ecf_ray_dir);
   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "theAdjPlatformPos = " << theAdjPlatformPos << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "theAdjPlatformPos (ossimGpt): " << ossimGpt(theAdjPlatformPos)
                                          << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "image_ray = " << image_ray << std::endl;
   }

   if (traceExec() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::imagingRay: returning..." << std::endl;
}

//*****************************************************************************
//  METHOD ossimFcsiModel::worldToLineSample()
//
//  Rigorous inverse transform overrides base-class iterative solution. It
//  implements the exact reverse transform as imagingRay() above.
//  
//*****************************************************************************
void ossimFcsiModel::worldToLineSample(const ossimGpt& world_point,
                                       ossimDpt&       image_point) const
{
   bool debug = false;  // setable via interactive debugger: "set debug = true"
   if (traceExec() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::imagingRay: entering..." << std::endl;

   //***
   // Establish ECF ray from platform to ground point and rotate to platform LSR
   //***
   ossimEcefPoint g_ecf (world_point);
   ossimEcefVector ecf_ray_dir (g_ecf - theAdjPlatformPos);
   ossimColumnVector3d cam_ray_dir (theAdjEcfToLsrRot*ecf_ray_dir.data());

   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<<"world_point = " << world_point << endl;
      ossimNotify(ossimNotifyLevel_DEBUG)<<"g_ecf = " << g_ecf << endl;
      ossimNotify(ossimNotifyLevel_DEBUG)<<"theAdjPlatformPos = " << theAdjPlatformPos << endl;
      ossimNotify(ossimNotifyLevel_DEBUG)<<"theAdjPlatformPos (ossimGpt): "<<ossimGpt(theAdjPlatformPos)<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG)<<"ecf_ray_dir = " << ecf_ray_dir.unitVector() << endl;
      ossimNotify(ossimNotifyLevel_DEBUG)<<"theAdjEcfToLsrRot = \n" << theAdjEcfToLsrRot << endl;
      ossimNotify(ossimNotifyLevel_DEBUG)<<"cam_ray_dir = " << cam_ray_dir << endl;
   }
      
   //***
   // Scale the direction vector by the focal length to arrive at  film
   // coordinates:
   //***
   double scale = -theAdjFocalLen/cam_ray_dir[2];
   ossimDpt film (scale*cam_ray_dir[0], scale*cam_ray_dir[1]);

   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "theAdjFocalLen = " << theAdjFocalLen << endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "scale = " << scale << endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "film (undistorted) = " << film << endl;
   }
      
   //***
   // Add distortion:
   //***
   if (theOpticalDistortion)
      theOpticalDistortion->forward(film);
   
   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "film (distorted) = " << film << endl;
   }
      
   //***
   // Apply film to pixel coordinates affine transform:
   //***
   ossimDpt f1(film + thePrincipalPoint);
   ossimDpt p1(theInvScanXformMatrix[0]*f1.x+theInvScanXformMatrix[1]*f1.y,
               theInvScanXformMatrix[2]*f1.x+theInvScanXformMatrix[3]*f1.y);
   ossimDpt p0 (p1 + theRefImgPt);

   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "thePrincipalPoint = " << thePrincipalPoint << endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "f1 = " << f1 << endl;
      for (int i=0; i<4; i++)
         ossimNotify(ossimNotifyLevel_DEBUG)<<"theInvScanXformMatrix["<<i<<"] = "<<theInvScanXformMatrix[i]
             <<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "p1 = " << p1 << endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "theRefImgPt = " << theRefImgPt << endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "p0 = " << p0 << endl;
   }
      
   //***
   // Finally, apply the sub-image offset to get to digital image coordinates:
   //***
   image_point = p0 - theSubImageOffset;
   
   if (traceDebug() || debug)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "theImageOffset = " << theSubImageOffset << endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << "image_point = " << image_point << endl;
   }
   
   if (traceExec() || debug)  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::imagingRay: returning..." << std::endl;
}


//*****************************************************************************
//  METHOD: ossimFcsiModel::print()
//  
//  Formatted dump of data members.
//  
//*****************************************************************************
std::ostream& ossimFcsiModel::print(std::ostream& out) const
{
   out << "\nDump of ossimFcsiModel object at " << hex << this << ":\n"
       << "\nossimFcsiModel data members: "
       << "\n        thePrincipalPoint: " << thePrincipalPoint
       << "\n        theScanScaleMatrix: " << theScanScaleMatrix[0] << "  "
       << theScanScaleMatrix[1]
       << "\n                            " << theScanScaleMatrix[2] << "  "
       << theScanScaleMatrix[3]
       << "\n          theScanSkew: " << theScanSkew
       << "\n      theScanRotation: " << theScanRotation;

   for (int i=0; i<4; ++i)
   {
      out << "\ntheScanXformMatrix["<<i<<"]: " << theScanXformMatrix[i];
   }

   out << "\n          theFocalLen: " << theFocalLen
       << "\n       thePlatformPos: " << thePlatformPos
       << "\n       theLsrToEcfRot: \n"  << theLsrToEcfRot
       << "\n    theAdjLsrToEcfRot: \n" << theAdjLsrToEcfRot
       << "\n         theEcfOffset: " << theEcfOffset
       << "\n          theXrotCorr: " << theXrotCorr
       << "\n          theYrotCorr: " << theYrotCorr
       << "\n          theZrotCorr: " << theZrotCorr
       << "\n       theFocalOffset: " << theFocalOffset
       << "\n      theScanSkewCorr: " << theScanSkewCorr
       << endl;
   
   if (theOpticalDistortion)
   {
      out << *theOpticalDistortion << endl;
   }

   return ossimSensorModel::print(out);
}

//*****************************************************************************
//  METHOD: ossimFcsiModel::saveState()
//  
//  Saves the model state to the KWL. This KWL also serves as a geometry file.
//  
//*****************************************************************************
bool ossimFcsiModel::saveState(ossimKeywordlist& kwl,
                              const char* prefix) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::saveState: entering..." << std::endl;

   kwl.add(prefix, ossimKeywordNames::TYPE_KW, "ossimFcsiModel");

   //***
   // Hand off to base class for common stuff:
   //***
   ossimSensorModel::saveState(kwl, prefix);

   //***
   // Save off data members:
   //***
   kwl.add(prefix, PRINCIPAL_POINT_X_KW,    thePrincipalPoint.x);
   kwl.add(prefix, PRINCIPAL_POINT_Y_KW,    thePrincipalPoint.y);
   kwl.add(prefix, SCAN_SCALE_MATRIX_00_KW, theScanScaleMatrix[0]);
   kwl.add(prefix, SCAN_SCALE_MATRIX_01_KW, theScanScaleMatrix[1]);
   kwl.add(prefix, SCAN_SCALE_MATRIX_10_KW, theScanScaleMatrix[2]);
   kwl.add(prefix, SCAN_SCALE_MATRIX_11_KW, theScanScaleMatrix[3]);
   kwl.add(prefix, SCAN_SKEW_ANGLE_KW,      theScanSkew);
   kwl.add(prefix, SCAN_ROTATION_ANGLE_KW,  theScanRotation);
   kwl.add(prefix, FOCAL_LENGTH_KW,         theFocalLen);
   kwl.add(prefix, PLATFORM_POSITION_X_KW,
           thePlatformPos.x());
   kwl.add(prefix, PLATFORM_POSITION_Y_KW,
           thePlatformPos.y());
   kwl.add(prefix, PLATFORM_POSITION_Z_KW,
           thePlatformPos.z());

   //***
   // Camera orientation matrix:
   // i is row and j is column
   //***
   for (int i=0; i<3; i++)
   {
      for (int j=0; j<3; j++)
      {
         ostringstream ostr;
         ostr << CAMERA_ORIENTATION_MATRIX_ELEM_KW
              << i << "_" << j << ends;
         kwl.add(prefix, ostr.str().c_str(), theLsrToEcfRot[i][j]);
      }
   }
   
   //***
   // Optical distortion coefficients:
   //***
   if (theOpticalDistortion)
   {
      ostringstream ostr;
      ostr << prefix << "distortion." << ends;
      theOpticalDistortion->saveState(kwl, ostr.str().c_str());
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::saveState: returning..." << std::endl;
   return true;
}

//*****************************************************************************
//  METHOD: ossimFcsiModel::loadState()
//  
//  Restores the model's state from the KWL. This KWL also serves as a
//  geometry file.
//  
//*****************************************************************************
bool ossimFcsiModel::loadState(const ossimKeywordlist& kwl,
                               const char* prefix) 
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::loadState: entering..." << std::endl;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG:"
                                          << "\nInput kwl:  " << kwl
                                          << std::endl;
   }

   const char* value;
   const char* keyword;
   bool success;

   //***
   // Assure this keywordlist contains correct type info:
   //***
   keyword = ossimKeywordNames::TYPE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "Type name not found");
      return false;
      
   }
   if (strcmp(value, TYPE_NAME(this)))
   {
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::saveState: returning..." << std::endl;
      return false;
   }
      
   //***
   // Clear out any existing adjustable params:
   //***
   
   //***
   // Pass on to the base-class for parsing first:
   //***
   success = ossimSensorModel::loadState(kwl, prefix);
   if (!success)
   {
      return false;
   }
   if(!getNumberOfAdjustableParameters())
   {
      initAdjustableParameters();
   }

   //***
   // Now begin loading data members:
   //***
   keyword = PRINCIPAL_POINT_X_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "principle point x not given");
      return false;
   }
   thePrincipalPoint.x = atof(value);
 
   keyword = PRINCIPAL_POINT_Y_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "principle point y not given");
      return false;
   }
      
   thePrincipalPoint.y = atof(value);
 
   keyword = SCAN_SCALE_MATRIX_00_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "scale matrix 0,0 not given");
      return false;
   }
   theScanScaleMatrix[0] = atof(value);
 
   keyword = SCAN_SCALE_MATRIX_01_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "scale matrix 0,1 not given");
      return false;
   }
   theScanScaleMatrix[1] = atof(value);
 
   keyword = SCAN_SCALE_MATRIX_10_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "scale matrix 1,0 not given");
      return false;
   }
   theScanScaleMatrix[2] = atof(value);
 
   keyword = SCAN_SCALE_MATRIX_11_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "scale matrix 1,1 not given");
      return false;
   }
   theScanScaleMatrix[3] = atof(value);
 
   keyword = SCAN_SKEW_ANGLE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
      theScanSkew = 0.0;
   else
      theScanSkew = atof(value);

   keyword = SCAN_ROTATION_ANGLE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
      theScanRotation = 0.0;
   else
      theScanRotation = atof(value);
 
   keyword = FOCAL_LENGTH_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "Focal length not given");
      return false;
   }
   theFocalLen = atof(value);
 
   keyword = PLATFORM_POSITION_X_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "Platform position X not given");
      return false;
   }
   thePlatformPos.x() = atof(value);
 
   keyword = PLATFORM_POSITION_Y_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "Platform position Y not given");
   }
   thePlatformPos.y() = atof(value);
 
   keyword = PLATFORM_POSITION_Z_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimSetError(getClassName(),
                    ossimErrorCodes::OSSIM_ERROR,
                    "Platform position Z not given");
   }
   thePlatformPos.z() = atof(value);
   
   //***
   // Platform orientation matrix:
   //***
   for (int i=0; i<3; i++)
   {
      for (int j=0; j<3; j++)
      {
         ostringstream ostr;
         ostr << CAMERA_ORIENTATION_MATRIX_ELEM_KW
              << i << "_" << j << ends;
         ossimString s = ostr.str();
         keyword = s.c_str();
         value = kwl.find(prefix, keyword);
         if (!value)
         {
            ossimSetError(getClassName(),
                          ossimErrorCodes::OSSIM_ERROR,
                          "Camera orientation matrix not given");
         }
         theLsrToEcfRot[i][j] = ossimString(value).toDouble();
      }
   }
   
   //***
   // Reading of optical distortion coefficients is left to the service object:
   //***
   if (theOpticalDistortion)
      delete theOpticalDistortion;
   theOpticalDistortion = new ossimRadialDecentLensDistortion(kwl, prefix);

   //***
   // Initialize given parameters read:
   //***
   clearErrorStatus();
   updateModel();
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::loadState: returning..." << std::endl;
   return true;
}

//*****************************************************************************
// PROTECTED METHOD: ossimFcsiModel::updateModel()
//
// Following a parameter adjustment, this method is called to recompute all
// dependent quantities that are used by the projection methods.
// 
//*****************************************************************************
void  ossimFcsiModel::updateModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::updateModel: entering..." << std::endl;
   
   //***
   // Update the geometry...
   // Adjusted = Initial + adj_parm[i]*adj_sigma[i]
   // Some parameters have a zero initial value and that term is not included.
   //***
   theEcfOffset.x() = getAdjustableParameter(X_POS) *
                      getParameterSigma(X_POS);
   theEcfOffset.y() = getAdjustableParameter(Y_POS) *
                      getParameterSigma(Y_POS);
   theEcfOffset.z() = getAdjustableParameter(Z_POS) *
                      getParameterSigma(Z_POS);
   theXrotCorr      = getAdjustableParameter(X_ROT) *
                      getParameterSigma(X_ROT);
   theYrotCorr      = getAdjustableParameter(Y_ROT) *
                      getParameterSigma(Y_ROT);
   theZrotCorr      = getAdjustableParameter(Z_ROT) *
                      getParameterSigma(Z_ROT);
   theFocalOffset   = getAdjustableParameter(FOCAL_LEN) *
                      getParameterSigma(FOCAL_LEN);
   theScanSkewCorr  = getAdjustableParameter(SCAN_SKEW) *
                      getParameterSigma(SCAN_SKEW);

   //***
   // Compute adjusted quatities used in projection code:
   //***
   theAdjFocalLen    = theFocalLen    + theFocalOffset;
   theAdjPlatformPos = thePlatformPos + theEcfOffset;
   
   //***
   // Establish the Scanner to film transform matrix.
   //
   // NOTE FOR IMPLEMENTING PARAMETER ADJUSTMENT: while the scan skew
   // angle is an adjustable parameter, the pixel scaling is not since this
   // error can be absorbed by the focal length adjustment, though the sigma
   // for the focal length must be inflated to account for the additional error
   // source. A similar situation occurrs with the scan rotation which manifests
   // as an attitude adjustment.
   //***
   double cos_rot  = ossim::cosd(theScanRotation);
   double sin_rot  = ossim::sind(theScanRotation);
   double tan_skew = ossim::tand(theScanSkew + theScanSkewCorr);

   double a =  theScanScaleMatrix[0]*(sin_rot*tan_skew + cos_rot) +
               theScanScaleMatrix[1]*(cos_rot*tan_skew - sin_rot);
   double b =  theScanScaleMatrix[0]*sin_rot + theScanScaleMatrix[1]*cos_rot;
   double c =  theScanScaleMatrix[2]*(sin_rot*tan_skew + cos_rot) +
               theScanScaleMatrix[3]*(cos_rot*tan_skew - sin_rot);
   double d =  theScanScaleMatrix[2]*sin_rot + theScanScaleMatrix[3]*cos_rot;

   theScanXformMatrix[0] = a;
   theScanXformMatrix[1] = b;
   theScanXformMatrix[2] = c;
   theScanXformMatrix[3] = d;

   //***
   // The inverse of the scanner-to-film transform must be computed since it is
   // not an orthogonal matrix (inverse != transpose):
   //***
   theInvScanXformMatrix[0] =  d/(d*a - b*c);
   theInvScanXformMatrix[1] = -b/(d*a - b*c);
   theInvScanXformMatrix[2] =  c/(b*c - a*d);
   theInvScanXformMatrix[3] = -a/(b*c - a*d);

   //***
   // Establish the differential rotation matrix due to attitude correction:
   //***
   double cw = ossim::cosd(theXrotCorr);
   double sw = ossim::sind(theXrotCorr);
   double cp = ossim::cosd(theYrotCorr);
   double sp = ossim::sind(theYrotCorr);
   double ck = ossim::cosd(theZrotCorr);
   double sk = ossim::sind(theZrotCorr);

   NEWMAT::Matrix attitudeCorrection(3, 3);
   attitudeCorrection(1,1) =  ck*cw - sk*sp*sw;
   attitudeCorrection(1,2) =  sk*cp;
   attitudeCorrection(1,3) =  ck*sw + sk*sp*cw;
   attitudeCorrection(2,1) = -sk*cw - ck*sp*sw;
   attitudeCorrection(2,2) =  cp*ck;
   attitudeCorrection(2,3) = -sk*sw + ck*sp*cw;
   attitudeCorrection(3,1) = -cp*sw;
   attitudeCorrection(3,2) = -sp;
   attitudeCorrection(3,3) =  cp*cw;

   //***
   // Now compute the final LSR to ECF rotation:
   //***
   theAdjLsrToEcfRot = attitudeCorrection * theLsrToEcfRot;
   theAdjEcfToLsrRot = theAdjLsrToEcfRot.t();

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::updateModel: returning..." << std::endl;
}


//*****************************************************************************
// PRIVATE METHOD: ossimFcsiModel::initAdjustableParameters()
//  
//  This method initializes the base class adjustable parameter and associated
//  sigmas arrays with quantities specific to this model. These are default
//  values only. A functional implementation would assign the sigmas via a
//  keywordlist or camera specific derived class.
//  
//*****************************************************************************
void ossimFcsiModel::initAdjustableParameters()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::initAdjustableParameters: entering..." << std::endl;

   int numParams = NUM_ADJUSTABLE_PARAMS;
   resizeAdjustableParameterArray(NUM_ADJUSTABLE_PARAMS);
   
//   if(!getNumberOfAdjustableParameters())
///   {
   //***
   // Allocate storage for adjustables and assign their names and units strings
   //***
//      newAdjustment(NUM_ADJUSTABLE_PARAMS);
//   }
   //***
   // Initialize base-class adjustable parameter array:
   //***
   for (int i=0; i<numParams; i++)
   {
      setAdjustableParameter(i, 0.0);
      setParameterDescription(i, PARAM_NAMES[i]);
      setParameterUnit(i, PARAM_UNITS[i]);
   }
   
   //***
   // Initialize base-class parameter sigma array:
   //***
   setParameterSigma(X_POS, 50.0);
   setParameterSigma(Y_POS, 50.0);
   setParameterSigma(Z_POS, 50.0);  
   setParameterSigma(X_ROT, 0.01);
   setParameterSigma(Y_ROT, 0.01);
   setParameterSigma(Z_ROT, 0.01);
   setParameterSigma(FOCAL_LEN,0.005);  
   setParameterSigma(SCAN_SKEW, 0.001);

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::initAdjustableParameters: returning..." << std::endl;
}

//*****************************************************************************
// STATIC METHOD: ossimFcsiModel::writeGeomTemplate
//  
//  Writes a sample kwl to output stream. Please update this method with any
//  format and/or keyword changes. It will make life a lot easier for everyone.
//  
//*****************************************************************************
void ossimFcsiModel::writeGeomTemplate(ostream& os)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::writeGeomTemplate: entering..." << std::endl;

   os <<
      "//*****************************************************************\n"
      "// Template for Frame Camera Scanned Image (FCSI) model keywordlist\n"
      "//*****************************************************************\n"
      << ossimKeywordNames::TYPE_KW << ": " << "ossimFcsiModel" << endl;

   ossimSensorModel::writeGeomTemplate(os);
   
   os << "//***\n"
      << "// Derived-class FCSI Keywords:\n"
      << "//***\n"
      << PRINCIPAL_POINT_X_KW << ": <pixels>\n"
      << PRINCIPAL_POINT_Y_KW << ": <pixels>\n"
      << SCAN_SCALE_MATRIX_00_KW << ": <mm/pixel>\n"
      << SCAN_SCALE_MATRIX_01_KW << ": <mm/pixel>\n"
      << SCAN_SCALE_MATRIX_10_KW << ": <mm/pixel>\n"
      << SCAN_SCALE_MATRIX_11_KW << ": <mm/pixel>\n"
      << SCAN_SKEW_ANGLE_KW << ": <degrees> [OPTIONAL]\n"
      << SCAN_ROTATION_ANGLE_KW << ": <degrees> [OPTIONAL]\n"
      << FOCAL_LENGTH_KW << ": <millimeters>\n"
      << PLATFORM_POSITION_X_KW << ": <meters (ECF)>\n"
      << PLATFORM_POSITION_Y_KW << ": <meters (ECF)>\n"
      << PLATFORM_POSITION_Z_KW << ": <meters (ECF)>\n"
      << "\n"
      << "//***\n"
      << "// Camera (LSR) to ECF orientation matrix:\n"
      << "//***\n";
   
   for (int i=0; i<3; i++)
   {
      for (int j=0; j<3; j++)
      {
         ostringstream ostr;
         ostr << CAMERA_ORIENTATION_MATRIX_ELEM_KW
              << i << "_" << j << ends;
         os << ostr.str() << ": <Euler rot matrix element>\n";
      }
   }

   os << "\n"
      << "//***\n"
      << "// Optical distortion coefficients [OPTIONAL]:\n"
      << "//***\n"
      << "distortion."
      << ossimRadialDecentLensDistortion::RADIAL_DISTORTION_COEFF_KW
      << "N: <coeff N> (for N <= 5)\n"
      << "distortion."
      << ossimRadialDecentLensDistortion::DECENT_DISTORTION_COEFF_KW
      << "N: <coeff N> (for N <= 4)\n"
      << endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimFcsiModel::writeGeomTemplate: returning..." << std::endl;
   
   return;
}


