//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Oscar Kramer
//
// Description:
//
// Sensor Model for Radarsat1 SAR sensor.
// 
//*******************************************************************
//  $Id:$

#include <ossim/projection/ossimRS1SarModel.h>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <ossim/base/ossimTrace.h>
#include <ossim/matrix/newmat.h>
#include <ossim/base/ossimLsrPoint.h>
#include <ossim/elevation/ossimElevManager.h>


using namespace std;
using namespace ossim;

static ossimTrace traceDebug("ossimRS1SarModel:debug");

static const double SEC_PER_DAY              = 86400.0;
static const double NOMINAL_ORBIT_ELEVATION  = 798000.0; // m
static const double DEG_PER_SEC              = 0.004178074;
static const double INTRACK_OFFSET_SIGMA     = 1000.0;  // meters
static const double CRTRACK_OFFSET_SIGMA     = 300.0;
static const double RADIAL_OFFSET_SIGMA      = 300.0;
static const double LINE_SCALE_SIGMA         = 0.01;
static const double SKEW_SIGMA               = 0.1;  // deg
static const double ORIENTATION_SIGMA        = 0.2;  // deg
static const double EARTH_ANGULAR_VELOCITY   = 7.2921151467e-05;  // radians/sec
static const char*  IMAGING_MODE_ID[] = 
{"UNKNOWN", "SCN", "SCW", "SGC", "SGF", "SGX", "SLC", "SPG", "SSG","RAW","ERS"};

//******************************************************************************
// Default.
//******************************************************************************
ossimRS1SarModel::ossimRS1SarModel()
:  theCeosData        (0),
   theImagingMode     (UNKNOWN_MODE)
{
   setErrorStatus();
}

//******************************************************************************
// Takes a ossimFilename& directory containing support data, initializes the model,
// and writes out the initial geometry file.
//******************************************************************************
ossimRS1SarModel::ossimRS1SarModel(const ossimFilename& imageFile)
   :
      theCeosData        (0),
      theImagingMode     (UNKNOWN_MODE)
{
   static const char MODULE[] = "Constructor ossimRS1SarModel(ossimFilename)";
   if (traceDebug())  CLOG << "entering..." << endl;

   clearErrorStatus();

   // Parse the CEOS data files:
   ossimFilename dataDirName (imageFile.path());
   initFromCeos(dataDirName);
   if(getErrorStatus() != ossimErrorCodes::OSSIM_OK)
   {
      return;
   }
   if (theImagingMode == UNKNOWN_MODE) 
      return;

   // Parse the image data file for the local ORPs interpolator:
   if ((theImagingMode == SCN) || (theImagingMode == SCW))
      establishOrpGrid();
   else
      establishOrpInterp();

   // Establish the ephemeris interpolator:
   establishEphemeris();

   initAdjParms();
   establishVehicleSpace();

   if (traceDebug())  CLOG << "returning..." << endl;
   return;
}


//******************************************************************************
//  DESTRUCTOR: ~ossimRS1SarModel
//******************************************************************************
ossimRS1SarModel::~ossimRS1SarModel()
{
   deallocateMemory();
}

//******************************************************************************
//  loadSate()
//******************************************************************************
bool ossimRS1SarModel::loadState(const ossimKeywordlist& /* kwl */, const char* /* prefix */)
{
   // NOT YET IMPLEMENTED
   setErrorStatus();
   return false;
}

//******************************************************************************
//  saveState()
//******************************************************************************
bool ossimRS1SarModel::saveState(ossimKeywordlist& /* kwl */, const char* /* prefix */) const
{
   // NOT YET IMPLEMENTED
   setErrorStatus();
   return false;
}

//*************************************************************************************************
//! Given an image point, returns a ray originating at some arbitrarily high
//! point (in this model at the sensor position) and pointing towards the target.
//*************************************************************************************************
void ossimRS1SarModel::imagingRay(const ossimDpt& image_point, ossimEcefRay& /* image_ray */) const
{
   const char* MODULE = "ossimRS1SarModel::imagingRay()";

   // Apply offset, scale, skew, and rotation to image point:
   ossimDpt offset (theImageSize.line/2.0, theImageSize.samp/2.0);
   ossimDpt ip1 = image_point - offset;
   ossimDpt ip2;
   ip2.line = ip1.line*theCosOrientation*theLineScale +
      ip1.samp*(theCosOrientation*theSinSkew+theSinOrientation);
   ip2.samp = -ip1.line*theSinOrientation*theLineScale -
      ip1.samp*(theSinOrientation*theSinSkew-theCosOrientation);

   // Offset point back to origin of image:
   ip1 = ip2 + offset;

   // Given new line number, obtain the interpolated ORP:
   ossimEcefPoint localORP;
   if ((theImagingMode == SCN) || (theImagingMode == SCW))
   {
      ossimGpt gpt (theLatGrid(ip1.x, ip1.y), theLonGrid(ip1.x, ip1.y), theRefHeight);
      localORP = ossimEcefPoint(gpt);
   }
   else
   {
      NEWMAT::ColumnVector orpPos(3);
      theLocalOrpInterp->interpolate(ip1.line, orpPos);
      localORP = ossimEcefPoint(orpPos[0], orpPos[1], orpPos[2]);
   }
   
   // Establish imaging line time (zero-Doppler time):
   double arpTime = theFirstLineTime + ip1.line*theTimePerLine;

   NEWMAT::ColumnVector arpEph(3);
   theArpPosInterp->interpolate(arpTime, arpEph);
   ossimEcefPoint ecfArpPos (arpEph[0], arpEph[1], arpEph[2]);

   theArpVelInterp->interpolate(arpTime, arpEph);
   ossimEcefVector ecfArpVel (arpEph[0], arpEph[1], arpEph[2]);

   // Need to correct the velocity vector by the earth rotational velocity:
   ossimEcefVector earthVel 
      (-ecfArpPos.y()*EARTH_ANGULAR_VELOCITY, ecfArpPos.x()*EARTH_ANGULAR_VELOCITY, 0.0);
   ecfArpVel = ecfArpVel - earthVel;

   // Compute a vehicle space with Z near intrack, X normal to Z and range
   // vector to ORP (slant range normal), and Y in range direction.
   // Note the application of the position adjustable parameter to the space's
   // origin (after the range vector was established):
   ossimEcefVector rangeVector     = localORP - ecfArpPos;
   ossimEcefVector rangeNormal     = rangeVector.cross(ecfArpVel);
   // ossimEcefVector zDirection      = rangeNormal.cross(rangeVector);
   ossimLsrSpace localVehicleSpace (ossimEcefPoint(ecfArpPos + thePosCorrection), // origin
                                    rangeNormal,  // X-direction
                                    rangeVector, 0);   // Y-direction (Z not specified
   double local_orp_range = rangeVector.length();

   // Compute slant range distance to pixel in question:
   double slant_range = local_orp_range;
   switch (theImagingMode)
   {
   case SLC:
      if (theDirectionFlag == ASCENDING)
         slant_range += thePixelSpacing.samp*ip1.samp;
      else
         slant_range += (theImageSize.samp-1.0-ip1.samp) * thePixelSpacing.samp;
      break;

   case SGC:
   case SGF:
   case SGX:
   case ERS:
      {
         // Compute the slant range as a polynomial expansion given ground range
         double ground_range;
         if (theDirectionFlag == ASCENDING)
            ground_range = ip1.samp * thePixelSpacing.samp;
         else
            ground_range = (theImageSize.samp - 1.0 - ip1.samp) *
            thePixelSpacing.samp;
         //         double delta_g_i = 1.0;
         //         for (int 0=1; i<6; i++)
         double delta_g_i = ground_range;
         for (int i=1; i<6; i++)  // NOTE: not using offset (i=0) term
         {
            slant_range += theSrGrCoeff[i] * delta_g_i;
            delta_g_i *= ground_range;
         }
      }
      break;

   case SCN:
   case SCW:
      slant_range = local_orp_range;
      break;

   default:
      CLOG << "ERROR: Invalid imaging mode encountered." << endl;
      setErrorStatus();
   }
}

//*************************************************************************************************
// PUBLIC METHOD: ossimRS1SarModel::lineSampleHeightToWorld(image_point, height, is_inside_image)
//
//  Performs image to ground projection. 
//*************************************************************************************************
void ossimRS1SarModel::lineSampleHeightToWorld(const ossimDpt& image_point,
                                               const double&   height_ellip,
                                               ossimGpt&       worldPt) const
{
   static const char MODULE[] = "ossimRS1SarModel::lineSampleHeightToWorld()";
   if (traceDebug())  CLOG << "entering..." << endl;

   // static const int    MAX_NUM_ITERS = 50;
   // static const double MAX_ELEV_DIFF = 0.001;

   ossimEcefRay imaging_ray;
   imagingRay(image_point, imaging_ray);
   worldPt = imaging_ray.intersectAboveEarthEllipsoid(height_ellip);
         
   if (traceDebug())  CLOG << "returning..." << endl;
}

//******************************************************************************
//  This method sets theImagingMode data member given a character string
//  abbreviation of the mode.
//******************************************************************************
void ossimRS1SarModel::setImagingMode(char* modeStr)
{
   static const char MODULE[] = "ossimRS1SarModel::setImagingMode(modeStr)";
   if (traceDebug())  CLOG << "entering..." << endl;

   bool illegal_mode = false;
   
   if (strcmp(modeStr, "SCN") == 0)
   {
      theImagingMode = SCN;
   }
   else if (strcmp(modeStr, "SCW") == 0)
   {
      theImagingMode = SCW;
   }
   else if (strcmp(modeStr, "SGC") == 0)
   {
      theImagingMode = SGC;
   }
   else if (strcmp(modeStr, "SGF") == 0)
   {
      theImagingMode = SGF;
   }
   else if (strcmp(modeStr, "SGX") == 0)
   {
      theImagingMode = SGX;
   }
   else if (strcmp(modeStr, "SLC") == 0)
   {
      theImagingMode = SLC;
   }
   else if (strcmp(modeStr, "SPG") == 0)
   {
      theImagingMode = SPG;
      illegal_mode = true;
   }
   else if (strcmp(modeStr, "SSG") == 0)
   {
      theImagingMode = SSG;
      illegal_mode = true;
   }
   else if (strcmp(modeStr, "RAW") == 0)
   {
      theImagingMode = RAW;
      illegal_mode = true;
   }
   else if (strcmp(modeStr, "ERS") == 0)
   {
      theImagingMode = ERS;
   }
   else 
   {
      theImagingMode = UNKNOWN_MODE;
      illegal_mode = true;
   }

   // Test for ERS product:
   if ((theImagingMode == UNKNOWN_MODE) && theCeosData.valid())
   {
      char buf[] = "xxx";
      strncpy(buf, &(theCeosData->textRec()->product_type[8]), 3);
      if (strcmp(buf, "ERS") == 0)
      {
         theImagingMode = ERS;
         illegal_mode = false;
      }
   }
      
   // Filter the imaging modes that are not handled:
   if (illegal_mode)
   {
      CLOG << "\n\t ERROR: The imaging mode <"
           << IMAGING_MODE_ID[(int) theImagingMode]
           << "> is currently not supported. \n" << endl;
      theImagingMode = UNKNOWN_MODE;
   }
   
   if (traceDebug())  CLOG << "returning..." << endl;
}


//******************************************************************************
//  Initialize various data members from the CEOS records.
//******************************************************************************
void ossimRS1SarModel::initFromCeos(const ossimFilename& fname)
{
   static const char MODULE[] = "ossimRS1SarModel::initFromCeos()";
   if (traceDebug())  CLOG << "entering..." << endl;

   // Instantiate a CeosData object:
   theCeosData = new ossimCeosData(fname);
   
   if(theCeosData->errorStatus() != ossimErrorCodes::OSSIM_OK) return;
   const dataset_sum_rec* dsr = theCeosData->dataSetSumRec();
   const proc_parm_rec*   ppr = theCeosData->procParmRec();
   char buf[1024];
   
   // Set imaging mode:
   strncpy(buf, &(theCeosData->volDescRec()->logvol_id[11]), 3);
   buf[3] = '\0';
   setImagingMode(buf);
   
   // Determine whether ascending or descending path acquisition. This flag
   // dictates whether the first or last pixel of a line is of shorter range:
   if (theImagingMode == ERS)
   {
      theDirectionFlag = DESCENDING;
   }
   else
   {
      if (dsr->asc_des[0] == 'A')
         theDirectionFlag = ASCENDING;
      else if (dsr->asc_des[0] == 'D')
         theDirectionFlag = DESCENDING;
      else
      {
         CLOG << "ERROR: Direction Flag: " << dsr->asc_des << " not supported"
              << endl;
         theDirectionFlag = UNKNOWN_DIRECTION;
         setErrorStatus();
         if (traceDebug())  CLOG << "returning with error..." << endl;
         return;
      }
   }

   // Image (Product) ID:
   buf[8] = '\0';
   strncpy(buf, theCeosData->volDescRec()->product_id, 8);
   theImageID = buf;
   
   // pixel spacing in range:
   buf[16] = '\0';
   strncpy(buf, dsr->pix_spacing, 16);
   thePixelSpacing.samp = atof(buf);
   
   // pixel spacing in azimuth:
   strncpy(buf, dsr->line_spacing, 16);
   buf[16] = '\0';
   thePixelSpacing.line = atof(buf);

   // The ground reference point (ORP) latitude:
   ossimGpt grp;
   strncpy(buf, dsr->pro_lat, 16);
   buf[16] = '\0';
   grp.lat = atof(buf);
      
   // The ground reference point (ORP) longitude:
   strncpy(buf, dsr->pro_long, 16);
   buf[16] = '\0';
   grp.lon = atof(buf);
      
   // The ground reference point (ORP) elevation:
   strncpy(buf, dsr->terrain_h, 16);
   buf[16] = '\0';
   grp.hgt = atof(buf);
   theORP = ossimEcefPoint(grp);
   theRefHeight = grp.hgt;
   
   // Illumination elevation:
   strncpy(buf, dsr->incident_ang, 8);
   buf[8] = '\0';
   double incidence = atof(buf);
   theIllumElevation = 90.0 - incidence;

   // Illumination azimuth -- need to determine whether sensor is in normal
   // right-looking mode or "antarctic" (left-looking) mode:
   strncpy(buf, dsr->plat_head, 8);
   theIllumAzimuth = atof(buf);
   char sensor_orientation[] = "123456789";
   strncpy(sensor_orientation, ppr->sens_orient, 9);
   if (strcmp(sensor_orientation, "ANTARCTIC") == 0)
   {
      theIllumAzimuth -= 90.0;
      if (theIllumAzimuth < 0.0) theIllumAzimuth += 360.0;
   }
   else
   {
      theIllumAzimuth += 90.0;
      if (theIllumAzimuth >= 360.0) theIllumAzimuth -= 360.0;
   }

   // Determine the number of lines per image and pixels per line. NOTE: if
   // image is scan mode, the number of lines must be computed indirectly. 
   if ((theImagingMode == SCN) || (theImagingMode == SCW))
   {
      FILE* fptr = fopen(theCeosData->imageFile().chars(), "r");
      if (!fptr)
      {
         CLOG << " ERROR:\n\tCannot open CEOS image file: "
              << theCeosData->imageFile() << endl;
         setErrorStatus();
         return;
      }
      fseek(fptr, 0, SEEK_END);
      long byte_count = ftell(fptr);
      buf[6] = '\0';
      strncpy(buf, theCeosData->imopDescRec()->l_dataset, 6);
      int lengthOfRec = atoi(buf);
      theImageSize.line = (byte_count - sizeof(*theCeosData->imopDescRec()))
                           /lengthOfRec;
      fclose(fptr);
   }
   else
   {
      strncpy(buf, theCeosData->imopDescRec()->n_dataset, 6);
      buf[6] = '\0';
      theImageSize.line = atoi(buf);
   }
   strncpy(buf, theCeosData->imopDescRec()->nleft, 4);
   buf[4] = '\0';
   int left_pixels = atoi(buf);
   strncpy(buf, theCeosData->imopDescRec()->ngrp, 8);
   buf[8] = '\0';
   int data_pixels = atoi(buf);
   strncpy(buf, theCeosData->imopDescRec()->nright, 4);
   buf[4] = '\0';
   int right_pixels  = atoi(buf);
   theImageSize.samp = left_pixels + data_pixels + right_pixels;

   // If ground range product, need to read the ground-to-slant range
   // conversion coefficients from the proc_parm_rec. NOTE: only single-look
   // products handled properly. ScanSAR requires reading multiple coefficient
   // sets. See MDA Detailed Processing Parameter Record Description sec.3.84.
   if ((theImagingMode==SGC) ||
       (theImagingMode==SGF) ||
       (theImagingMode==SGX) ||
       (theImagingMode==ERS))
   {
      buf[16] = '\0';
      for (int i=0; i<6; i++)
      {
         strncpy(buf, ppr->srgr_coefset[0].srgr_coef[i], 16);
         theSrGrCoeff[i] = atof(buf);
      }
   }
   else
   {
      theSrGrCoeff[0] = 0.0;
      theSrGrCoeff[1] = 1.0;  // note 1.0 here (should never be accessed)
      theSrGrCoeff[2] = 0.0;
      theSrGrCoeff[3] = 0.0;
      theSrGrCoeff[4] = 0.0;
      theSrGrCoeff[5] = 0.0;
   }
   
   if (traceDebug())
   {
      CLOG << "DEBUG -- "
           << "\n\t theDirectionFlag = " << theDirectionFlag
           << "\n\t thePixelSpacing = " << thePixelSpacing
           << "\n\t theORP = " << theORP
           << "\n\t theRefHeight = " << theRefHeight
           << "\n\t theImageSize = " << theImageSize
           << "\n\t sensor_orientation = " << sensor_orientation
           << "\n\t theIllumElevation = " << theIllumElevation
           << "\n\t theIllumAzimuth = " << theIllumAzimuth
           << endl;
   }

   if (traceDebug())  CLOG << "returning..." << endl;
}


//******************************************************************************
//  This method sets the rigorous model adjustable parameters to
//  their initial values.
//******************************************************************************
void ossimRS1SarModel::initAdjParms()
{
   // Adjustable model not yet implemented
#if 0

   static const char* MODULE = "ossimRS1SarModel::initAdjParms()";
   if (traceDebug())  CLOG << "entering..." << endl;

   theInTrackOffset = (init_adj_parm[INTRACK_OFFSET]+adj_parm[INTRACK_OFFSET])
                      * adj_sigma[INTRACK_OFFSET];
   
   theCrTrackOffset = (init_adj_parm[CRTRACK_OFFSET]+adj_parm[CRTRACK_OFFSET])
                      * adj_sigma[CRTRACK_OFFSET];
   
   theRadialOffset = (init_adj_parm[RADIAL_OFFSET]+adj_parm[RADIAL_OFFSET])
                     * adj_sigma[RADIAL_OFFSET];
   
   theLineScale   = (init_adj_parm[LINE_SCALE]+adj_parm[LINE_SCALE])
                    * adj_sigma[LINE_SCALE];
   
   theSkew        = (init_adj_parm[SKEW]+adj_parm[SKEW])
                    * adj_sigma[SKEW];
   
   theOrientation = (init_adj_parm[ORIENTATION]+adj_parm[ORIENTATION]);

   // Initialize base-class initial adjustable parameter array:
   init_adj_parm[INTRACK_OFFSET]  = theInTrackOffset/adj_sigma[INTRACK_OFFSET];
   init_adj_parm[CRTRACK_OFFSET]  = theCrTrackOffset/adj_sigma[CRTRACK_OFFSET];
   init_adj_parm[RADIAL_OFFSET]   = theRadialOffset/adj_sigma[RADIAL_OFFSET];  
   init_adj_parm[LINE_SCALE]      = theLineScale/adj_sigma[LINE_SCALE];  
   init_adj_parm[SKEW]            = theSkew/adj_sigma[SKEW];  
   init_adj_parm[ORIENTATION]     = theOrientation/adj_sigma[ORIENTATION];  
   
   //  Initialize sensor adjustable parameter description strings:
   strncpy (adj_desc[INTRACK_OFFSET], "intrack_offset", MAX_DESC_CHARS);
   strncpy (adj_desc[CRTRACK_OFFSET], "crtrack_offset", MAX_DESC_CHARS);
   strncpy (adj_desc[RADIAL_OFFSET],  "radial_offset",  MAX_DESC_CHARS);
//   strncpy (adj_desc[SAMP_SCALE],  "samp_scale",      MAX_DESC_CHARS);
   strncpy (adj_desc[LINE_SCALE],   "line_scale",       MAX_DESC_CHARS);
   strncpy (adj_desc[SKEW],         "image_skew",       MAX_DESC_CHARS);
   strncpy (adj_desc[ORIENTATION],  "image_orientation",MAX_DESC_CHARS);

   // Initialize the adj_parms (parameter corrections) to 0:
   for (int i=0; i<num_adj_parm; i++)
      adj_parm[i] = 0.0;
   
   if (traceDebug())  CLOG << "returning..." << endl;
#endif
}


//******************************************************************************
//  This method parses the SAR header information associated with the
//  ephemeris state vectors, and instantiates a Lagrange interpolator object
//  for interpolating intermediate ehemeris states.
//******************************************************************************
void ossimRS1SarModel::establishEphemeris()
{
   static const char MODULE[] = "ossimRS1SarModel::establishEphemeris()";
   if (traceDebug())  CLOG << "entering..." << endl;

   char buf[23];
   
   // Establish pointer to platform position data record:
   const pos_data_rec* pdr = theCeosData->posDataRec();
   
   // Establish the Greenwich mean hour angle at time of first sample:
   char gha_str [23];
   strncpy(gha_str, pdr->hr_angle, 22);
   gha_str[22] = '\0';
   theGHA = atof(gha_str);

   // Fetch number of points in record and allocate memory:
   strncpy(buf, pdr->ndata, 4);
   buf[4] = '\0';
   int numPoints = atoi(buf);

   // Fetch sampling period:
   strncpy(buf, pdr->data_int, 22);
   buf[22] = '\0';
   double sampling_period = atof(buf);

   // Establish the first sample time's day:
   strncpy(buf, pdr->gmt_day, 4);
   buf[4] = '\0';
   int first_day =  atoi(buf);
   double time_offset = (double) (first_day - theFirstLineDay)*SEC_PER_DAY;

   // Establish the first sample time in seconds from beginning of day:
   strncpy(buf, pdr->gmt_sec, 22);
   buf[22] = '\0';
   theEphFirstSampTime =  atof(buf) + time_offset;

   int i, j;
   double sample_time = theEphFirstSampTime;
   buf[22] = '\0';
   NEWMAT::ColumnVector eciArpPos(3);
   NEWMAT::ColumnVector eciArpVel(3);
   NEWMAT::ColumnVector ecfArpPos(3);
   NEWMAT::ColumnVector ecfArpVel(3);
   NEWMAT::ColumnVector earthVel(3);
   NEWMAT::Matrix xform;
   theArpPosInterp = new ossimLagrangeInterpolator;
   theArpVelInterp = new ossimLagrangeInterpolator;

   // Loop over each point, converting them to vectors:
   for (i=0; i<numPoints; i++)
   {
      for (j=0; j<3; j++)
      {
         strncpy(buf, pdr->pos_vect[i].pos[j], 22);
         eciArpPos[j] = atof(buf);
         strncpy(buf, pdr->pos_vect[i].vel[j], 22);
         eciArpVel[j] = atof(buf)/1000.0;
      }

      // Convert pos and vel vectors from ECI to ECF:
      eciToEcfXform(sample_time, xform);
      ecfArpPos = xform*eciArpPos;
      ecfArpVel = xform*eciArpVel;
      theArpPosInterp->addData(sample_time, ecfArpPos);

      // Correct velocity for earth rotation:
      earthVel[0] = -ecfArpPos[1]*EARTH_ANGULAR_VELOCITY;
      earthVel[1] =  ecfArpPos[0]*EARTH_ANGULAR_VELOCITY;
      earthVel[2] =  0.0;
      ecfArpVel = ecfArpVel - earthVel;
      theArpVelInterp->addData(sample_time, ecfArpVel);

      sample_time += sampling_period;
   }

   if (traceDebug())
   {
      CLOG << "DEBUG -- "
           << "\n\t theGHA:              " << theGHA 
           << "\n\t numPoints:           " << numPoints 
           << "\n\t sampling_period:     " << sampling_period 
           << "\n\t theEphFirstSampTime: " << theEphFirstSampTime << endl;
   }

   if (traceDebug())  CLOG << "returning..." << endl;
}


//******************************************************************************
//  Returns 3x3 transform to rotate ECI into ECF. The argument is
// in seconds of the day.
//******************************************************************************
void ossimRS1SarModel::eciToEcfXform(const double& acq_time, NEWMAT::Matrix& xform) const
{
   static const char MODULE[] = "ossimRS1SarModel::eciToEcfXform(acq_time)";
   if (traceDebug())  CLOG << "entering..." << endl;

   // Determine time elapsed since ephemeris first sample time:
   double delta_t = acq_time - theEphFirstSampTime;

   // Compute GHA at time of this image line (in degrees):
   double gha = theGHA + delta_t*DEG_PER_SEC;
   if (gha >= 360.0)
      gha -=360.0;
   else if (gha < 0.0)
      gha += 360.0;
   
   // Establish rotation:
   double cos_gha = ossim::cosd(gha);
   double sin_gha = ossim::sind(gha);

   // Establish rotation matrix:
   xform = ossimMatrix3x3::create(cos_gha,  sin_gha,  0.0,
                                 -sin_gha,  cos_gha,  0.0,
                                      0.0,      0.0,  1.0);
   if (traceDebug())
   {
      CLOG << "DEBUG -- "
           << "\n\t acq_time = " << acq_time
           << "\n\t delta_t  = " << delta_t
           << "\n\t gha      = " << gha << endl;
   }

   if (traceDebug())  CLOG << "returning..." << endl;
}
   

//******************************************************************************
// PRIVATE METHOD: ossimRS1SarModel::establishOrpInterp()
//  
//  This method reads the image data file for obtaining the local ORPs (taken
//  to be the lat-lon of the nearest pixel for each line), and the zero-doppler
//  time for each line. These ORPs are used for computing SPNs.
//
//  The processed data records correspond to lines of imagery. Accompanying
//  each data line is the lat-lon (at the ellipsoid) of the first, middle, and
//  last sample of the line, the time of zero-doppler (broadside) imaging for
//  the line.
//
//  A Lagrange interpolator is established with N data points representing the
//  local ORPs and their corresponding image line numbers
//  uniformly distributed over the image.
//
//  If the image is a ground range product, need to also read the ground-to-
//  slant range conversion coefficients.
//  
//******************************************************************************
void ossimRS1SarModel::establishOrpInterp()
{
   static const char MODULE[] = "ossimRS1SarModel::establishOrpInterp()";
   if (traceDebug())  CLOG << "entering..." << endl;
   
   if (traceDebug())
      CLOG << "DEBUG -- " << endl;

   static const int      NUM_DATA_POINTS  = 11;

   desc_rec        descRec;
   pdr_prefix_rec  prefix;
   int             sizeOfDescRec = sizeof(descRec);
   int             sizeOfPrefRec = sizeof(prefix);
   int             headerSize    = sizeOfDescRec + sizeOfPrefRec;
   char            buf[] = "123456";
   ossimGpt localOrp (0.0, 0.0, theRefHeight);
   std::vector<double> line_numbers_list;
   std::vector<NEWMAT::ColumnVector> orpVectorList; // X, Y, Z

   if (traceDebug())
   {
      CLOG << "DEBUG:"
           << "\nsizeOfDescRec:  " << sizeOfDescRec
           << "\nsizeOfPrefRec:  " << sizeOfPrefRec << endl;
   }
   
   // Open the image data file given the image directory name and seek to first
   // SAR data record:
   FILE* fptr = fopen(theCeosData->imageFile().chars(), "r");
   if (!fptr)
   {
      CLOG << "ERROR: Could not open data file <" << theCeosData->imageFile()
           << ">" << endl;
      setErrorStatus();
      return;
   }
   fseek(fptr, theCeosData->imopDescRec()->desc.length, SEEK_SET);

   // Declare some constants and variables used in loop:
   int last_line    = (int) theImageSize.line - 1;
   int delta_line   = (int)ceil(last_line/((double)NUM_DATA_POINTS-1.0));
   int line_number  = 0;

   // Compute the number of SAR data bytes to skip to reach prefix of next
   // record of interest:
   strncpy(buf, theCeosData->imopDescRec()->l_dataset, 6);
   int record_size = atoi(buf);
   
   // Loop over each imaging line:
   int index;
   theLocalOrpInterp = new ossimLagrangeInterpolator;
   for (index=0; index<NUM_DATA_POINTS; index++)
   {
      if (traceDebug())
         clog << "\t Processing ORP for line number " << line_number << endl;

      // Read the image line prefix info:
      fread(&descRec, sizeOfDescRec, 1, fptr);
      fread(&prefix,  sizeOfPrefRec,  1, fptr);

      // Verify correct line number being read:
      if ((prefix.line_num-1) != line_number)
      {
         CLOG << "\n\tERROR: Synchronization error reading image file. "
              << "Expected line number " << line_number+1
              << " but read line number " << prefix.line_num << "." << endl;
         setErrorStatus();
         return;
      }
      
      // Latch the imaging time for the first line collected:
      if (index == 0)
      {
         theFirstLineDay  = prefix.acq_date.acq_day;
         theFirstLineTime = (double) prefix.acq_date.acq_msec/1000.0;
      }
      
      // Read the local ORP for this line and convert to ECF:
      if (theDirectionFlag == ASCENDING)
      {
         localOrp.lat    = (double) prefix.lat_first  / 1.0e6;
         localOrp.lon = (double) prefix.long_first / 1.0e6;
      }
      else
      {
         localOrp.lat    = (double) prefix.lat_last  / 1.0e6;
         localOrp.lon = (double) prefix.long_last / 1.0e6;
      }

      theLocalOrpInterp->addData(line_number, ossimEcefPoint(localOrp).toVector());
      
      // Update the line number for next record, insuring we don't go passed
      // the last line:
      line_number += delta_line;
      if (line_number > last_line)
      {
         delta_line -= line_number - last_line;  // adjust for fseek below
         line_number = last_line;
      }

      // Advance to read the next prefix record of interest:
      fseek(fptr, (delta_line)*record_size-headerSize, SEEK_CUR);
   }

   // Compute time interval between each line, handle possible day rollover:
   double lastLineTime = (double) prefix.acq_date.acq_msec/1000.0;
   if ((lastLineTime-theFirstLineTime) > (SEC_PER_DAY/2.0))
      lastLineTime -= SEC_PER_DAY;
   else if ((theFirstLineTime-lastLineTime) > (SEC_PER_DAY/2.0))
      lastLineTime += SEC_PER_DAY;
   theTimePerLine = (lastLineTime - theFirstLineTime)/line_number;

   if (traceDebug())
   {
      clog << "\n\t line_number (last)   = " << line_number
           << "\n\t last_line (in image) = " << last_line
           << "\n\t theFirstLineTime     = " << theFirstLineTime
           << "\n\t lastLineTime         = " << lastLineTime
           << "\n\t theTimePerLine       = " << theTimePerLine
           << endl;
   }
   
   if (traceDebug())  CLOG << "returning..." << endl;
}

//******************************************************************************
//  This method establishes a common mean intrack-crosstrack-radial LSR
//  space at the ARP.
//******************************************************************************
void ossimRS1SarModel::establishVehicleSpace()
{
   static const char MODULE[] = "ossimRS1SarModel::establishVehicleSpace()";
   if (traceDebug())  CLOG << "entering..." << endl;

   // Obtain the imaging time when the vehicle is at the ARP:
   double line = theImageSize.line/2.0;
   double arpTime = theFirstLineTime + line*theTimePerLine;

   // Obtain the ephemeris at this time:
   NEWMAT::ColumnVector arpPos(3);
   theArpPosInterp->interpolate(arpTime, arpPos);
   NEWMAT::ColumnVector arpVel(3);
   theArpVelInterp->interpolate(arpTime, arpVel);

   // Convert ephemeris to ECF:
   NEWMAT::Matrix xform;
   eciToEcfXform(arpTime, xform);
   ossimEcefPoint ecfArpPos  (xform * arpPos);
   ossimEcefVector ecfArpVel (xform * arpVel);

   // Need to correct the velocity vector by the earth rotational velocity:
   ossimEcefVector earthVel(-ecfArpPos.y()*EARTH_ANGULAR_VELOCITY,
                             ecfArpPos.x()*EARTH_ANGULAR_VELOCITY,
                             0.0);
   ecfArpVel = ecfArpVel - earthVel;

   // Establish LSR space for intrack-crosstrack-radial at vehicle:
   ossimEcefVector intrackDir (ecfArpVel);
   ossimEcefVector crtrackDir (ecfArpPos.data().cross(intrackDir.data())); 
   theVehicleSpace = ossimLsrSpace (ecfArpPos, intrackDir, crtrackDir, 0);

   if (traceDebug())
   {
      CLOG << "DEBUG -- "
           << "\n\t theVehicleSpace: " << theVehicleSpace << endl;
   }

   if (traceDebug())  CLOG << "returning..." << endl;
}
   

//******************************************************************************
//  For scan-mode imagery, this method interpolates the lat/lon given the image pixel.
//******************************************************************************
void ossimRS1SarModel::interpolatedScanORP(const ossimDpt& orp, ossimEcefPoint& orp_ecf) const
{
   static const char MODULE[] = "ossimRS1SarModel::interpolatedScanORP(gDblPoint)";
   if (traceDebug())  CLOG << "entering..." << endl;

   ossimGpt gpt;
   gpt.lat = theLatGrid(orp);
   gpt.lon = theLonGrid(orp);

   // Convert to ECF:
   orp_ecf = ossimEcefPoint(gpt);
   
   if (traceDebug())  CLOG << "returning..." << endl;
}


//******************************************************************************
// PRIVATE METHOD: ossimRS1SarModel::establishOrpGrid()
//
//  This method establishes a grid of lat-lon across the image for interpolating
//  the geographic point on the ellipsoid given a pixel value. This is used for
//  scan-mode imagery for obtaining a local ORP.
//
//******************************************************************************
void ossimRS1SarModel::establishOrpGrid()
{
   static const char MODULE[] = "ossimRS1SarModel::establishOrpGrid()";
   if (traceDebug())  CLOG << "entering..." << endl;
   
   if (traceDebug())
   {
      CLOG << "DEBUG -- " << endl;
   }

   static const int      NUM_GRID_POINTS_U  = 11;

   desc_rec        descRec;
   pdr_prefix_rec  prefix;
   int             sizeOfDescRec = sizeof(descRec);
   int             sizeOfPrefRec = sizeof(prefix);
   int             headerSize    = sizeOfDescRec + sizeOfPrefRec;
   char            buf[] = "123456";
   ossimDpt gridSize ((double) NUM_GRID_POINTS_U, 3.0);
   ossimDpt cellSize (theImageSize.line/gridSize.u, theImageSize.samp/gridSize.v);

   // Because we are doing integer arithmetic, it will be necessary to
   // consider the last grid cells as being of a slightly different size:
   //theLastGridCellSize.line = theImageSize.line - 1.0 -
   //                           (theGridSize.u-2.0)*theGridCellSize.u;
   //theLastGridCellSize.samp = theImageSize.samp - 1.0 -
   //                           (theGridSize.v-2.0)*theGridCellSize.v;
   
   // Open the image data file given the image directory name and seek to first
   // SAR data record:
   FILE* fptr = fopen(theCeosData->imageFile().chars(), "r");
   if (!fptr)
   {
      CLOG << "ERROR: Could not open data file <" << theCeosData->imageFile()
           << ">" << endl;
      setErrorStatus();
      return;
   }
   fseek(fptr, theCeosData->imopDescRec()->desc.length, SEEK_SET);

   // Declare some constants and variables used in loop:
   int last_line    = (int) theImageSize.line - 1;
   int delta_line   = (int) cellSize.u;
   int line_number  = 0;

   // Allocate memory for coarse grid:
   theLatGrid.initialize(gridSize, ossimDpt(0,0), ossimDpt(1,1));
   theLonGrid.initialize(gridSize, ossimDpt(0,0), ossimDpt(1,1));

   // Compute the number of SAR data bytes to skip to reach prefix of next
   // record of interest:
   strncpy(buf, theCeosData->imopDescRec()->l_dataset, 6);
   int record_size = atoi(buf);
   int test_line = (int) gridSize.u - 2;
   
   // Loop over each imaging line:
   for (int u=0; u<(int)gridSize.u; u++)
   {
      if (traceDebug())
      {
         clog << "\t Processing grid line number " << u << endl;
      }

      // Read the image line prefix info:
      fread(&descRec, sizeOfDescRec, 1, fptr);
      fread(&prefix,  sizeOfPrefRec,  1, fptr);

      // Verify correct line number being read:
      if ((prefix.line_num-1) != line_number)
      {
         CLOG << "\n\tERROR: Synchronization error reading image file. "
              << "Expected line number " << line_number+1
              << " but read line number " << prefix.line_num << "." << endl;
         setErrorStatus();
         return;
      }
      
      // Latch the imaging time for the first line collected:
      if (u == 0)
      {
         theFirstLineDay  = prefix.acq_date.acq_day;
         theFirstLineTime = (double) prefix.acq_date.acq_msec/1000.0;
      }
      
      // Read the ground points for this line:
      theLatGrid.setNode(u, 0, (double) prefix.lat_first /1.0e6);
      theLatGrid.setNode(u, 1, (double) prefix.lat_mid   /1.0e6);
      theLatGrid.setNode(u, 2, (double) prefix.lat_last  /1.0e6);
      theLonGrid.setNode(u, 0, (double) prefix.long_first/1.0e6);
      theLonGrid.setNode(u, 1, (double) prefix.long_mid  /1.0e6);
      theLonGrid.setNode(u, 2, (double) prefix.long_last /1.0e6);
      
      // Update the line number for next record, insuring we don't go passed
      // the last line:
      if (u == test_line)
      {
         delta_line  = last_line - line_number;
         line_number = last_line;
      }
      else
      {
         line_number += delta_line;
      }

      //***
      // Advance to read the next prefix record of interest:
      //***
      fseek(fptr, (delta_line)*record_size-headerSize, SEEK_CUR);
   }

   // Compute time interval between each line, handle possible day rollover:
   double lastLineTime = (double) prefix.acq_date.acq_msec/1000.0;
   if ((lastLineTime-theFirstLineTime) > (SEC_PER_DAY/2.0))
   {
      lastLineTime -= SEC_PER_DAY;
   }
   else if ((theFirstLineTime-lastLineTime) > (SEC_PER_DAY/2.0))
   {
      lastLineTime += SEC_PER_DAY;
   }
   theTimePerLine = (lastLineTime - theFirstLineTime)/line_number;

   if (traceDebug())
   {
      clog << "\n\t line_number (last)   = " << line_number
           << "\n\t last_line (in image) = " << last_line
           << "\n\t theFirstLineTime     = " << theFirstLineTime
           << "\n\t lastLineTime         = " << lastLineTime
           << "\n\t theTimePerLine       = " << theTimePerLine
           << endl;
   }
   
   if (traceDebug())  CLOG << "returning..." << endl;
}

//******************************************************************************
// PRIVATE METHOD: deallocateMemory()
//  
//  This method permits the partial destruction of the object so that it may
//  be reconstructed without invoking the destructor.
//  
//******************************************************************************
void ossimRS1SarModel::deallocateMemory()
{
   static const char MODULE[] = "ossimRS1SarModel::deallocateMemory()";
   if (traceDebug())  CLOG << "entering..." << endl;

   theArpPosInterp = 0;
   theArpVelInterp = 0;
   theLocalOrpInterp = 0;
   theCeosData        = 0;
   theLatGrid.clear();
   theLonGrid.clear();
      
   if (traceDebug())  CLOG << "returning..." << endl;
}


