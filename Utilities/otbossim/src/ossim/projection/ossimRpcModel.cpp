//*****************************************************************************
// FILE: ossimRpcModel.cpp
//
// License:  See top level LICENSE.txt file.
//
// AUTHOR: Oscar Kramer
//
// DESCRIPTION: Contains implementation of class ossimRpcModel.
//   This is a replacement model utilizing the Rational Polynomial Coefficients
//   (RPC), a.k.a. Rapid Positioning Capability, and Universal Sensor Model
//   (USM).
//
// LIMITATIONS: Does not support parameter adjustment (YET)
//
//*****************************************************************************
//  $Id: ossimRpcModel.cpp 20600 2012-02-20 15:03:14Z gpotts $

#include <ossim/projection/ossimRpcModel.h>
#include <ossim/elevation/ossimElevManager.h>

RTTI_DEF1(ossimRpcModel, "ossimRpcModel", ossimSensorModel);

#include <ossim/elevation/ossimHgtRef.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ossim/projection/ossimProjectionFactoryRegistry.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimRpcModel:exec");
static ossimTrace traceDebug ("ossimRpcModel:debug");

static const int    MODEL_VERSION_NUMBER  = 1;
static const int    NUM_COEFFS        = 20;
static const char*  MODEL_TYPE        = "ossimRpcModel";
static const char*  POLY_TYPE_KW      = "polynomial_format";
static const char*  LINE_SCALE_KW     = "line_scale";
static const char*  SAMP_SCALE_KW     = "samp_scale";
static const char*  LAT_SCALE_KW      = "lat_scale";
static const char*  LON_SCALE_KW      = "long_scale";
static const char*  HGT_SCALE_KW      = "height_scale";
static const char*  LINE_OFFSET_KW    = "line_off";
static const char*  SAMP_OFFSET_KW    = "samp_off";
static const char*  LAT_OFFSET_KW     = "lat_off";
static const char*  LON_OFFSET_KW     = "long_off";
static const char*  HGT_OFFSET_KW     = "height_off";

static const char*  BIAS_ERROR_KW     = "bias_error";
static const char*  RAND_ERROR_KW     = "rand_error";

static const char*  LINE_NUM_COEF_KW  = "line_num_coeff_";
static const char*  LINE_DEN_COEF_KW  = "line_den_coeff_";
static const char*  SAMP_NUM_COEF_KW  = "samp_num_coeff_";
static const char*  SAMP_DEN_COEF_KW  = "samp_den_coeff_";

static const ossimString PARAM_NAMES[] ={"intrack_offset",
                                        "crtrack_offset",
                                        "intrack_scale",
                                        "crtrack_scale",
                                        "map_rotation",
                                        "yaw_offset"};
static const ossimString PARAM_UNITS[] ={"pixel",
                                        "pixel",
                                        "scale",
                                        "scale",
                                        "degrees",
                                        "degrees"};
      
//*****************************************************************************
//  DEFAULT CONSTRUCTOR: ossimRpcModel()
//  
//*****************************************************************************
ossimRpcModel::ossimRpcModel()
   :  ossimSensorModel(),
      thePolyType     (A),
      theLineScale    (0.0),
      theSampScale    (0.0),
      theLatScale     (0.0),
      theLonScale     (0.0),
      theHgtScale     (0.0),
      theLineOffset   (0.0),
      theSampOffset   (0.0),
      theLatOffset    (0.0),
      theLonOffset    (0.0),
      theHgtOffset    (0.0),
      theIntrackOffset(0.0),
      theCrtrackOffset(0.0),
      theIntrackScale (0.0),
      theCrtrackScale (0.0),
      theCosMapRot    (0.0),
      theSinMapRot    (0.0),
      theBiasError    (0.0),
      theRandError    (0.0)

{
   initAdjustableParameters();
}

//*****************************************************************************
//  COPY CONSTRUCTOR: ossimRpcModel(ossimRpcModel)
//  
//*****************************************************************************
ossimRpcModel::ossimRpcModel(const ossimRpcModel& model)
   :
      ossimSensorModel(model),
      thePolyType     (model.thePolyType),
      theLineScale    (model.theLineScale),
      theSampScale    (model.theSampScale),
      theLatScale     (model.theLatScale),
      theLonScale     (model.theLonScale),
      theHgtScale     (model.theHgtScale),
      theLineOffset   (model.theLineOffset),
      theSampOffset   (model.theSampOffset),
      theLatOffset    (model.theLatOffset),
      theLonOffset    (model.theLonOffset),
      theHgtOffset    (model.theHgtOffset),
      theIntrackOffset(model.theIntrackOffset),
      theCrtrackOffset(model.theCrtrackOffset),
      theIntrackScale(model.theIntrackScale),
      theCrtrackScale(model.theCrtrackScale),
      theCosMapRot    (model.theCosMapRot),
      theSinMapRot    (model.theSinMapRot),
      theBiasError    (model.theBiasError),
      theRandError    (model.theRandError)
{
   for (int i=0; i<20; ++i  )
   {
      theLineNumCoef[i] = model.theLineNumCoef[i];
      theLineDenCoef[i] = model.theLineDenCoef[i];
      theSampNumCoef[i] = model.theSampNumCoef[i];
      theSampDenCoef[i] = model.theSampDenCoef[i];
   }
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimRpcModel()
//  
//*****************************************************************************
ossimRpcModel::~ossimRpcModel()
{
}

void ossimRpcModel::setAttributes(ossim_float64 sampleOffset,
                                  ossim_float64 lineOffset,
                                  ossim_float64 sampleScale,
                                  ossim_float64 lineScale,
                                  ossim_float64 latOffset,
                                  ossim_float64 lonOffset,
                                  ossim_float64 heightOffset,
                                  ossim_float64 latScale,
                                  ossim_float64 lonScale,
                                  ossim_float64 heightScale,
                                  const std::vector<double>& xNumeratorCoeffs,
                                  const std::vector<double>& xDenominatorCoeffs,
                                  const std::vector<double>& yNumeratorCoeffs,
                                  const std::vector<double>& yDenominatorCoeffs,
                                  PolynomialType polyType,
                                  bool computeGsdFlag)
{
   thePolyType = polyType;
   
   theLineScale  = lineScale;
   theSampScale  = sampleScale;
   theLatScale   = latScale;
   theLonScale   = lonScale;
   theHgtScale   = heightScale;
   theLineOffset = lineOffset;
   theSampOffset = sampleOffset;
   theLatOffset  = latOffset;
   theLonOffset  = lonOffset;
   theHgtOffset  = heightOffset;

   if(xNumeratorCoeffs.size() == 20)
   {
      std::copy(xNumeratorCoeffs.begin(),
                xNumeratorCoeffs.end(),
                theSampNumCoef);
   }
   if(xDenominatorCoeffs.size() == 20)
   {
      std::copy(xDenominatorCoeffs.begin(),
                xDenominatorCoeffs.end(),
                theSampDenCoef);
   }
   if(yNumeratorCoeffs.size() == 20)
   {
      std::copy(yNumeratorCoeffs.begin(),
                yNumeratorCoeffs.end(),
                theLineNumCoef);
   }
   if(yDenominatorCoeffs.size() == 20)
   {
      std::copy(yDenominatorCoeffs.begin(),
                yDenominatorCoeffs.end(),
                theLineDenCoef);
   }

   if(computeGsdFlag)
   {
      try
      {
         // This will set theGSD and theMeanGSD. Method throws ossimException.
         computeGsd();
      }
      catch (const ossimException& e)
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimRpcModel::setAttributes Caught Exception:\n"
               << e.what() << std::endl;
         }
      }
   }
}

void ossimRpcModel::setMetersPerPixel(const ossimDpt& metersPerPixel)
{
   theGSD = metersPerPixel;
   theMeanGSD = (theGSD.x+theGSD.y)*.5;
}

void ossimRpcModel::setPositionError(const ossim_float64& biasError,
                                     const ossim_float64& randomError,
                                     bool initNominalPostionErrorFlag)
{
   theBiasError = biasError;
   theRandError = randomError;
   if (initNominalPostionErrorFlag)
   {
      theNominalPosError = sqrt(theBiasError*theBiasError +
                                theRandError*theRandError); // meters
   }
}

//*****************************************************************************
//  METHOD: ossimRpcModel::worldToLineSample()
//  
//  Overrides base class implementation. Directly computes line-sample from
//  the polynomials.
//*****************************************************************************
void ossimRpcModel::worldToLineSample(const ossimGpt& ground_point,
                                      ossimDpt&       img_pt) const
{
   // if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::worldToLineSample(): entering..." << std::endl;

   if(ground_point.isLatNan() || ground_point.isLonNan() )
   {
      img_pt.makeNan();
      return;
   }

   //***
   // First check if the world point is inside bounding rectangle:
   //***
//   ossimDpt wdp (ground_point);
//    if (!(theBoundGndPolygon.pointWithin(wdp)))
//    {
//      img_pt = extrapolate(ground_point);
//       if (traceExec())  CLOG << "returning..." << endl;
//       return;
//    }
         
   //***
   // Normalize the lat, lon, hgt:
   //***
   double nlat = (ground_point.lat - theLatOffset) / theLatScale;
   double nlon = (ground_point.lon - theLonOffset) / theLonScale;
   double nhgt;

   if( ground_point.isHgtNan() )
   {
      // nhgt = (theHgtScale - theHgtOffset) / theHgtScale;
      nhgt = ( - theHgtOffset) / theHgtScale;
   }
   else
   {
      nhgt = (ground_point.hgt - theHgtOffset) / theHgtScale;
   }

   //***
   // Compute the adjusted, normalized line (U) and sample (V):
   //***
   double Pu = polynomial(nlat, nlon, nhgt, theLineNumCoef);
   double Qu = polynomial(nlat, nlon, nhgt, theLineDenCoef);
   double Pv = polynomial(nlat, nlon, nhgt, theSampNumCoef);
   double Qv = polynomial(nlat, nlon, nhgt, theSampDenCoef);
   double U_rot  = Pu / Qu;
   double V_rot  = Pv / Qv;

   //***
   // U, V are normalized quantities. Need now to establish the image file
   // line and sample. First, back out the adjustable parameter effects
   // starting with rotation:
   //***
   double U = U_rot*theCosMapRot + V_rot*theSinMapRot;
   double V = V_rot*theCosMapRot - U_rot*theSinMapRot;

   //***
   // Now back out skew, scale, and offset adjustments:
   //***
   img_pt.line = U*(theLineScale+theIntrackScale) + theLineOffset + theIntrackOffset;
   
   img_pt.samp = V*(theSampScale+theCrtrackScale) + theSampOffset + theCrtrackOffset;

   // if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::worldToLineSample(): returning..." << std::endl;
   return;
}

//*****************************************************************************
//  METHOD: ossimRpcModel::lineSampleToWorld()
//  
//  Overrides base class implementation. Performs DEM intersection.
//*****************************************************************************
void  ossimRpcModel::lineSampleToWorld(const ossimDpt& imagePoint,
                                       ossimGpt&       worldPoint) const
{
   if(!imagePoint.hasNans())
   {
      ossimEcefRay ray;
      imagingRay(imagePoint, ray);
      ossimElevManager::instance()->intersectRay(ray, worldPoint);
   }
   else
   {
      worldPoint.makeNan();
   }
}

//*****************************************************************************
//  METHOD: ossimRpcModel::imagingRay()
//  
//  Constructs an RPC ray by intersecting 2 ellipsoid heights above and
//  below the RPC height offset, and then forming a vector between the two.
//
//*****************************************************************************
void ossimRpcModel::imagingRay(const ossimDpt& imagePoint,
                               ossimEcefRay&   imageRay) const
{
   //---
   // For "from point", "to point" we want the image ray to be from above the
   // ellipsoid down to Earth.
   // 
   // It appears the ray "from point" must be above the ellipsiod for the
   // ossimElevSource::intersectRay method; ultimately, the
   // ossimEllipsoid::nearestIntersection method, else it goes off in the
   // weeds...
   //---
   double vectorLength = theHgtScale ? (theHgtScale * 2.0) : 1000.0;

   ossimGpt gpt;
   
   // "from" point
   double intHgt = theHgtOffset + vectorLength;
   lineSampleHeightToWorld(imagePoint, intHgt, gpt);
   ossimEcefPoint intECFfrom(gpt);
   
   // "to" point
   lineSampleHeightToWorld(imagePoint, theHgtOffset, gpt);
   ossimEcefPoint intECFto(gpt);
   
   // Construct ray
   ossimEcefRay ray(intECFfrom, intECFto);
   
   imageRay = ray;
}


//*****************************************************************************
//  METHOD: ossimRpcModel::lineSampleHeightToWorld()
//  
//  Performs reverse projection of image line/sample to ground point.
//  The imaging ray is intersected with a level plane at height = elev.
//
//  NOTE: U = line, V = sample -- this differs from the convention.
//
//*****************************************************************************
void ossimRpcModel::lineSampleHeightToWorld(const ossimDpt& image_point,
                                            const double&   ellHeight,
                                            ossimGpt&       gpt) const
{
   // if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::lineSampleHeightToWorld: entering..." << std::endl;

   //***
   // Extrapolate if point is outside image:
   //***
//    if (!insideImage(image_point))
//    {
//       gpt = extrapolate(image_point, ellHeight);
//       if (traceExec())  CLOG << "returning..." << endl;
//       return;
//    }

   //***
   // Constants for convergence tests:
   //***
   static const int    MAX_NUM_ITERATIONS  = 10;
   static const double CONVERGENCE_EPSILON = 0.1;  // pixels
   
   //***
   // The image point must be adjusted by the adjustable parameters as well
   // as the scale and offsets given as part of the RPC param normalization.
   //
   //      NOTE: U = line, V = sample
   //***
   double U    = (image_point.y-theLineOffset - theIntrackOffset) / (theLineScale+theIntrackScale);
   double V    = (image_point.x-theSampOffset - theCrtrackOffset) / (theSampScale+theCrtrackScale);

   //***
   // Rotate the normalized U, V by the map rotation error (adjustable param):
   //***
   double U_rot = theCosMapRot*U - theSinMapRot*V;
   double V_rot = theSinMapRot*U + theCosMapRot*V;
   U = U_rot; V = V_rot;


   // now apply adjust intrack and cross track
   //***
   // Initialize quantities to be used in the iteration for ground point:
   //***
   double nlat      = 0.0;  // normalized latitude
   double nlon      = 0.0;  // normalized longitude
   
   double nhgt;

   if(ossim::isnan(ellHeight))
   {
     nhgt = (theHgtScale - theHgtOffset) / theHgtScale;  // norm height
   }
   else
   {
      nhgt = (ellHeight - theHgtOffset) / theHgtScale;  // norm height
   }
   
   double epsilonU = CONVERGENCE_EPSILON/(theLineScale+theIntrackScale);
   double epsilonV = CONVERGENCE_EPSILON/(theSampScale+theCrtrackScale);
   int    iteration = 0;

   //***
   // Declare variables only once outside the loop. These include:
   // * polynomials (numerators Pu, Pv, and denominators Qu, Qv),
   // * partial derivatives of polynomials wrt X, Y,
   // * computed normalized image point: Uc, Vc,
   // * residuals of normalized image point: deltaU, deltaV,
   // * partial derivatives of Uc and Vc wrt X, Y,
   // * corrections to normalized lat, lon: deltaLat, deltaLon.
   //***
   double Pu, Qu, Pv, Qv;
   double dPu_dLat, dQu_dLat, dPv_dLat, dQv_dLat;
   double dPu_dLon, dQu_dLon, dPv_dLon, dQv_dLon;
   double Uc, Vc;
   double deltaU, deltaV;
   double dU_dLat, dU_dLon, dV_dLat, dV_dLon, W;
   double deltaLat, deltaLon;
   
   //***
   // Now iterate until the computed Uc, Vc is within epsilon of the desired
   // image point U, V:
   //***
   do
   {
      //***
      // Calculate the normalized line and sample Uc, Vc as ratio of
      // polynomials Pu, Qu and Pv, Qv:
      //***
      Pu = polynomial(nlat, nlon, nhgt, theLineNumCoef);
      Qu = polynomial(nlat, nlon, nhgt, theLineDenCoef);
      Pv = polynomial(nlat, nlon, nhgt, theSampNumCoef);
      Qv = polynomial(nlat, nlon, nhgt, theSampDenCoef);
      Uc = Pu/Qu;
      Vc = Pv/Qv;
      
      //***
      // Compute residuals between desired and computed line, sample:
      //***
      deltaU = U - Uc;
      deltaV = V - Vc;
      
      //***
      // Check for convergence and skip re-linearization if converged:
      //***
      if ((fabs(deltaU) > epsilonU) || (fabs(deltaV) > epsilonV))
      {
         //***
         // Analytically compute the partials of each polynomial wrt lat, lon:
         //***
         dPu_dLat = dPoly_dLat(nlat, nlon, nhgt, theLineNumCoef);
         dQu_dLat = dPoly_dLat(nlat, nlon, nhgt, theLineDenCoef);
         dPv_dLat = dPoly_dLat(nlat, nlon, nhgt, theSampNumCoef);
         dQv_dLat = dPoly_dLat(nlat, nlon, nhgt, theSampDenCoef);
         dPu_dLon = dPoly_dLon(nlat, nlon, nhgt, theLineNumCoef);
         dQu_dLon = dPoly_dLon(nlat, nlon, nhgt, theLineDenCoef);
         dPv_dLon = dPoly_dLon(nlat, nlon, nhgt, theSampNumCoef);
         dQv_dLon = dPoly_dLon(nlat, nlon, nhgt, theSampDenCoef);
         
         //***
         // Analytically compute partials of quotients U and V wrt lat, lon: 
         //***
         dU_dLat = (Qu*dPu_dLat - Pu*dQu_dLat)/(Qu*Qu);
         dU_dLon = (Qu*dPu_dLon - Pu*dQu_dLon)/(Qu*Qu);
         dV_dLat = (Qv*dPv_dLat - Pv*dQv_dLat)/(Qv*Qv);
         dV_dLon = (Qv*dPv_dLon - Pv*dQv_dLon)/(Qv*Qv);
         
         W = dU_dLon*dV_dLat - dU_dLat*dV_dLon;
         
         //***
         // Now compute the corrections to normalized lat, lon:
         //***
         deltaLat = (dU_dLon*deltaV - dV_dLon*deltaU) / W;
         deltaLon = (dV_dLat*deltaU - dU_dLat*deltaV) / W;
         nlat += deltaLat;
         nlon += deltaLon;
      }
      
      iteration++;
      
   } while (((fabs(deltaU)>epsilonU) || (fabs(deltaV)>epsilonV))
            && (iteration < MAX_NUM_ITERATIONS));
      
   //***
   // Test for exceeding allowed number of iterations. Flag error if so:
   //***
   if (iteration == MAX_NUM_ITERATIONS)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimRpcModel::lineSampleHeightToWorld: \nMax number of iterations reached in ground point "
                                         << "solution. Results are inaccurate." << endl;
   }

   //***
   // Now un-normalize the ground point lat, lon and establish return quantity:
   //***
   gpt.lat = nlat*theLatScale + theLatOffset;
   gpt.lon = nlon*theLonScale + theLonOffset;
   gpt.hgt = ellHeight;
   
}

//*****************************************************************************
// PRIVATE METHOD: ossimRpcModel::polynomial
//  
//  Computes polynomial.
//  
//*****************************************************************************
double ossimRpcModel::polynomial(const double& P, const double& L,
                                 const double& H, const double* c) const
{
   double r;

   if (thePolyType == A)
   {
      r = c[ 0]       + c[ 1]*L     + c[ 2]*P     + c[ 3]*H     +
          c[ 4]*L*P   + c[ 5]*L*H   + c[ 6]*P*H   + c[ 7]*L*P*H +
          c[ 8]*L*L   + c[ 9]*P*P   + c[10]*H*H   + c[11]*L*L*L +
          c[12]*L*L*P + c[13]*L*L*H + c[14]*L*P*P + c[15]*P*P*P +
          c[16]*P*P*H + c[17]*L*H*H + c[18]*P*H*H + c[19]*H*H*H;
   }
   else
   {
      r = c[ 0]       + c[ 1]*L     + c[ 2]*P     + c[ 3]*H     +
          c[ 4]*L*P   + c[ 5]*L*H   + c[ 6]*P*H   + c[ 7]*L*L   +
          c[ 8]*P*P   + c[ 9]*H*H   + c[10]*L*P*H + c[11]*L*L*L +
          c[12]*L*P*P + c[13]*L*H*H + c[14]*L*L*P + c[15]*P*P*P +
          c[16]*P*H*H + c[17]*L*L*H + c[18]*P*P*H + c[19]*H*H*H;
   }
   
   return r;
}

//*****************************************************************************
// PRIVATE METHOD: ossimRpcModel::dPoly_dLat
//  
//  Computes derivative of polynomial wrt normalized Latitude P.
//  
//*****************************************************************************
double ossimRpcModel::dPoly_dLat(const double& P, const double& L,
                                 const double& H, const double* c) const
{
   double dr;

   if (thePolyType == A)
   {
      dr = c[2] + c[4]*L + c[6]*H + c[7]*L*H + 2*c[9]*P + c[12]*L*L +
           2*c[14]*L*P + 3*c[15]*P*P +2*c[16]*P*H + c[18]*H*H;
   }
   else
   {
      dr = c[2] + c[4]*L + c[6]*H + 2*c[8]*P + c[10]*L*H + 2*c[12]*L*P +
           c[14]*L*L + 3*c[15]*P*P + c[16]*H*H + 2*c[18]*P*H;
   }
   
   return dr;
}

//*****************************************************************************
// PRIVATE METHOD: ossimRpcModel::dPoly_dLon
//  
//  Computes derivative of polynomial wrt normalized Longitude L.
//  
//*****************************************************************************
double ossimRpcModel::dPoly_dLon(const double& P, const double& L,
                                 const double& H, const double* c) const
{
   double dr;

   if (thePolyType == A)
   {
      dr = c[1] + c[4]*P + c[5]*H + c[7]*P*H + 2*c[8]*L + 3*c[11]*L*L +
           2*c[12]*L*P + 2*c[13]*L*H + c[14]*P*P + c[17]*H*H;
   }
   else
   {
      dr = c[1] + c[4]*P + c[5]*H + 2*c[7]*L + c[10]*P*H + 3*c[11]*L*L +
           c[12]*P*P + c[13]*H*H + 2*c[14]*P*L + 2*c[17]*L*H;
   }
   return dr;
}

//*****************************************************************************
// PRIVATE METHOD: ossimRpcModel::dPoly_dHgt
//  
//  Computes derivative of polynomial wrt normalized Height H.
//  
//*****************************************************************************
double ossimRpcModel::dPoly_dHgt(const double& P, const double& L,
                                 const double& H, const double* c) const
{
   double dr;

   if (thePolyType == A)
   {
      dr = c[3] + c[5]*L + c[6]*P + c[7]*L*P + 2*c[10]*H + c[13]*L*L +
           c[16]*P*P + 2*c[17]*L*H + 2*c[18]*P*H + 3*c[19]*H*H;
   }
   else
   {
      dr = c[3] + c[5]*L + c[6]*P + 2*c[9]*H + c[10]*L*P + 2*c[13]*L*H +
           2*c[16]*P*H + c[17]*L*L + c[18]*P*P + 3*c[19]*H*H;
   }
   return dr;
}

void ossimRpcModel::updateModel()
{
   theIntrackOffset    = computeParameterOffset(INTRACK_OFFSET);
   theCrtrackOffset    = computeParameterOffset(CRTRACK_OFFSET);
   theIntrackScale     = computeParameterOffset(INTRACK_SCALE);
   theCrtrackScale     = computeParameterOffset(CRTRACK_SCALE);
   double mapRotation  = computeParameterOffset(MAP_ROTATION);
   theCosMapRot        = ossim::cosd(mapRotation);
   theSinMapRot        = ossim::sind(mapRotation);
}

void ossimRpcModel::initAdjustableParameters()
{
   resizeAdjustableParameterArray(NUM_ADJUSTABLE_PARAMS);
   int numParams = getNumberOfAdjustableParameters();
   for (int i=0; i<numParams; i++)
   {
      setAdjustableParameter(i, 0.0);
      setParameterDescription(i, PARAM_NAMES[i]);
      setParameterUnit(i,PARAM_UNITS[i]);
   }
   setParameterSigma(INTRACK_OFFSET, 50.0);
   setParameterSigma(CRTRACK_OFFSET, 50.0);
   setParameterSigma(INTRACK_SCALE, 50.0);  
   setParameterSigma(CRTRACK_SCALE, 50.0);  
   setParameterSigma(MAP_ROTATION, 0.1);
//   setParameterSigma(YAW_OFFSET, 0.001);
}

ossimObject* ossimRpcModel::dup() const
{
   return new ossimRpcModel(*this);
}

//*****************************************************************************
//  METHOD: ossimRpcModel::print()
//  
//  Formatted dump of data members.
//  
//*****************************************************************************
std::ostream& ossimRpcModel::print(std::ostream& out) const
{
   out << "\nDump of ossimRpcModel object at " << std::hex << this << std::dec << ":\n"
       << POLY_TYPE_KW   << ": " << thePolyType   << "\n"
       << LINE_SCALE_KW  << ": " << theLineScale  << "\n"
       << SAMP_SCALE_KW  << ": " << theSampScale  << "\n"
       << LAT_SCALE_KW   << ": " << theLatScale   << "\n"
       << LON_SCALE_KW   << ": " << theLonScale   << "\n"
       << HGT_SCALE_KW   << ": " << theHgtScale   << "\n"
       << LINE_OFFSET_KW << ": " << theLineOffset << "\n"
       << SAMP_OFFSET_KW << ": " << theSampOffset << "\n"
       << LAT_OFFSET_KW  << ": " << theLatOffset  << "\n"
       << LON_OFFSET_KW  << ": " << theLonOffset  << "\n"
       << HGT_OFFSET_KW  << ": " << theHgtOffset  << "\n"
       << BIAS_ERROR_KW  << ": " << theBiasError  << "\n"
       << RAND_ERROR_KW  << ": " << theRandError  << "\n"
       << std::endl;

   for (int i=0; i<NUM_COEFFS; i++)
      out<<"  "<<LINE_NUM_COEF_KW<<"["<<i<<"]: "<<theLineNumCoef[i]<<std::endl;

   out << std::endl;
   for (int i=0; i<NUM_COEFFS; i++)
      out<<"  "<<LINE_DEN_COEF_KW<<"["<<i<<"]: "<<theLineDenCoef[i]<<std::endl;

   out << std::endl;
   for (int i=0; i<NUM_COEFFS; i++)
      out<<"  "<<SAMP_NUM_COEF_KW<<"["<<i<<"]: "<<theSampNumCoef[i]<<std::endl;

   out << std::endl;
   for (int i=0; i<NUM_COEFFS; i++)
      out<<"  "<<SAMP_DEN_COEF_KW<<"["<<i<<"]: "<<theSampDenCoef[i]<<std::endl;
      
   out << std::endl;

   return ossimSensorModel::print(out);
}

//*****************************************************************************
//  METHOD: ossimRpcModel::saveState()
//  
//  Saves the model state to the KWL. This KWL also serves as a geometry file.
//  
//*****************************************************************************
bool ossimRpcModel::saveState(ossimKeywordlist& kwl,
                              const char* prefix) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::saveState(): entering..." << std::endl;

   kwl.add(prefix, ossimKeywordNames::TYPE_KW, MODEL_TYPE);

   //***
   // Hand off to base class for common stuff:
   //***
   ossimSensorModel::saveState(kwl, prefix);

   //---
   // Save off offsets and scales:
   //---
   kwl.add(prefix, POLY_TYPE_KW,   ((char)thePolyType));
   kwl.add(prefix, LINE_SCALE_KW,  theLineScale);
   kwl.add(prefix, SAMP_SCALE_KW,  theSampScale);
   kwl.add(prefix, LAT_SCALE_KW,   theLatScale);
   kwl.add(prefix, LON_SCALE_KW,   theLonScale);
   kwl.add(prefix, HGT_SCALE_KW,   theHgtScale);
   kwl.add(prefix, LINE_OFFSET_KW, theLineOffset);
   kwl.add(prefix, SAMP_OFFSET_KW, theSampOffset);
   kwl.add(prefix, LAT_OFFSET_KW,  theLatOffset);
   kwl.add(prefix, LON_OFFSET_KW,  theLonOffset);
   kwl.add(prefix, HGT_OFFSET_KW,  theHgtOffset);
   kwl.add(prefix, BIAS_ERROR_KW,  theBiasError);
   kwl.add(prefix, RAND_ERROR_KW,  theRandError);

   for (int i=0; i<NUM_COEFFS; i++)
   {
      ossimString key;
      std::ostringstream os;
      os << setw(2) << setfill('0') << right << i;
      
      key = LINE_NUM_COEF_KW;
      key += os.str();
      kwl.add(prefix, key.c_str(), theLineNumCoef[i],
              true, 15);
      
      key = LINE_DEN_COEF_KW;
      key += os.str();
      kwl.add(prefix, key.c_str(), theLineDenCoef[i],
              true, 15);

      key = SAMP_NUM_COEF_KW;
      key += os.str();
      kwl.add(prefix, key.c_str(), theSampNumCoef[i],
              true, 15);

      key = SAMP_DEN_COEF_KW;
      key += os.str();
      kwl.add(prefix, key.c_str(), theSampDenCoef[i],
              true, 15);
   }
      
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::saveState(): returning..." << std::endl;
   return true;
}

//*****************************************************************************
//  METHOD: ossimRpcModel::loadState()
//  
//  Restores the model's state from the KWL. This KWL also serves as a
//  geometry file.
//  
//*****************************************************************************
bool ossimRpcModel::loadState(const ossimKeywordlist& kwl,
                              const char* prefix) 
{
   if (traceExec())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimRpcModel::loadState(): entering..." << std::endl;
   }

   const char* value;
   const char* keyword;

   //***
   // Pass on to the base-class for parsing first:
   //***
   bool success = ossimSensorModel::loadState(kwl, prefix);
   if (!success)
   {
      theErrorStatus++;

      if (traceExec())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimRpcModel::loadState(): returning with error..."
            << std::endl;
      }
      return false;
   }
      
   //---
   // Continue parsing for local members:
   //---
   value = kwl.find(prefix, BIAS_ERROR_KW);
   if (value)
   {
      theBiasError = ossimString(value).toDouble();
   }

   value = kwl.find(prefix, RAND_ERROR_KW);
   if (value)
   {
      theRandError = ossimString(value).toDouble();
   }
        
   keyword = POLY_TYPE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   thePolyType = (PolynomialType) value[0];
      
   keyword = LINE_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLineScale = atof(value);
   
   keyword = SAMP_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theSampScale = atof(value);
   
   keyword = LAT_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLatScale = atof(value);
   
   keyword = LON_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLonScale = atof(value);
   
   keyword = HGT_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theHgtScale = atof(value);
   
   keyword = LINE_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLineOffset = atof(value);
   
   keyword = SAMP_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theSampOffset = atof(value);
   
   keyword = LAT_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLatOffset = atof(value);
   
   keyword = LON_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLonOffset = atof(value);
   
   keyword = HGT_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theHgtOffset = atof(value);

   for (int i=0; i<NUM_COEFFS; i++)
   {
      ossimString keyword;
      ostringstream os;
      os << setw(2) << setfill('0') << right << i;

      keyword = LINE_NUM_COEF_KW;
      keyword += os.str();
      value = kwl.find(prefix, keyword.c_str());
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
            << "<" << keyword << ">. Check the keywordlist for proper syntax."
            << std::endl;
         return false;
      }
      theLineNumCoef[i] = atof(value);

      keyword = LINE_DEN_COEF_KW;
      keyword += os.str();
      value = kwl.find(prefix, keyword.c_str());
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theLineDenCoef[i] = atof(value);
   
      keyword = SAMP_NUM_COEF_KW;
      keyword += os.str();
      value = kwl.find(prefix, keyword.c_str());
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theSampNumCoef[i] = atof(value);

      keyword = SAMP_DEN_COEF_KW;
      keyword += os.str();
      value = kwl.find(prefix, keyword.c_str());
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theSampDenCoef[i] = atof(value);
   }
      
   //***
   // Initialize other data members given quantities read in KWL:
   //***
   theCosMapRot = 1.0;
   theSinMapRot = 0.0;

   updateModel();
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::loadState(): returning..." << std::endl;
   return true;
}

//*****************************************************************************
// STATIC METHOD: ossimRpcModel::writeGeomTemplate
//  
//  Writes a sample kwl to output stream.
//  
//*****************************************************************************
void ossimRpcModel::writeGeomTemplate(ostream& os)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::writeGeomTemplate(): entering..." << std::endl;

   os <<
      "//**************************************************************\n"
      "// Template for RPC model keywordlist\n"
      "//**************************************************************\n"
      << ossimKeywordNames::TYPE_KW << ": " << MODEL_TYPE << endl;

   ossimSensorModel::writeGeomTemplate(os);
   
   os << "//\n"
      << "// Derived-class ossimRpcModel Keywords:\n"
      << "//\n"
      << POLY_TYPE_KW << ": A|B\n"
      << "\n"
      << "// RPC data consists of coefficients and normalization \n"
      << "// parameters. The RPC keywords used here are compatible with \n"
      << "// keywords found in Ikonos \"rpc.txt\" files.\n"
      << "// First are the normalization parameters:\n"
      << LINE_OFFSET_KW << ": <float>\n"
      << SAMP_OFFSET_KW << ": <float>\n"
      << LAT_OFFSET_KW << ": <float>\n"
      << LON_OFFSET_KW << ": <float>\n"
      << HGT_OFFSET_KW << ": <float>\n"
      << LINE_SCALE_KW << ": <float>\n"
      << SAMP_SCALE_KW << ": <float>\n"
      << LAT_SCALE_KW << ": <float>\n"
      << LON_SCALE_KW << ": <float>\n"
      << HGT_SCALE_KW << ": <float>\n"
      << BIAS_ERROR_KW << ": <float>\n"
      << RAND_ERROR_KW << ": <float>\n"
      << "\n"
      << "// RPC Coefficients are specified with indexes. Coefficients \n "
      << "// are specified for the four polynomials: line numerator, line \n"
      << "// denominator, sample numerator, and sample denominator:" << endl;

   for (int i=1; i<=20; i++)
      os << LINE_NUM_COEF_KW << setw(2) << setfill('0') << right
         << i << ": <float>" << endl; 
   os << endl;
   for (int i=1; i<=20; i++)
      os << LINE_DEN_COEF_KW << setw(2) << setfill('0') << right
         << i << ": <float>" << endl; 
   os << endl;
   for (int i=1; i<=20; i++)
      os << SAMP_NUM_COEF_KW << setw(2) << setfill('0') << right
         << i << ": <float>" << endl; 
   os << endl;
   for (int i=1; i<=20; i++)
      os << SAMP_DEN_COEF_KW << setw(2) << setfill('0') << right
         << i << ": <float>" << endl; 
   os << "\n" <<endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::writeGeomTemplate(): returning..." << std::endl;
   return;
}

bool ossimRpcModel::setupOptimizer(const ossimString& init_file)
{
   ossimKeywordlist kwl;

   if(kwl.addFile(ossimFilename(init_file)))
   {
      return loadState(kwl);
   }
   else
   {
      ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->createProjection(init_file);
      if(proj.valid())
      {
         kwl.clear();
         proj->saveState(kwl);
         
         return loadState(kwl);
      }
   }
   
   return false;
}

//*****************************************************************************
//  METHOD: ossimSarModel::getForwardDeriv()
//  
//  Compute partials of samp/line WRT to ground.
//  
//*****************************************************************************
ossimDpt ossimRpcModel::getForwardDeriv(int derivMode,
                                        const ossimGpt& pos,
                                        double h)
{
   // If derivMode (parmIdx) >= 0 call base class version
   // for "adjustable parameters"
   if (derivMode >= 0)
   {
      return ossimSensorModel::getForwardDeriv(derivMode, pos, h);
   }
   
   // Use alternative derivMode definitions
   else
   {
      ossimDpt returnData;

      //******************************************
      // OBS_INIT mode
      //    [1] 
      //    [2] 
      //  Note: In this mode, pos is used to pass
      //  in the (s,l) observations.
      //******************************************
      if (derivMode==OBS_INIT)
      {
         // Image coordinates
         ossimDpt obs;
         obs.samp = pos.latd();
         obs.line = pos.lond();
         theObs = obs;
      }

      //******************************************
      // EVALUATE mode
      //   [1] evaluate & save partials, residuals
      //   [2] return residuals
      //******************************************
      else if (derivMode==EVALUATE)
      {
         //***
         // Normalize the lat, lon, hgt:
         //***
         double nlat = (pos.lat - theLatOffset) / theLatScale;
         double nlon = (pos.lon - theLonOffset) / theLonScale;
         double nhgt;

         if( ossim::isnan(pos.hgt) )
         {
            nhgt = (theHgtScale - theHgtOffset) / theHgtScale;
         }
         else
         {
            nhgt = (pos.hgt - theHgtOffset) / theHgtScale;
         }
         
         //***
         // Compute the normalized line (Un) and sample (Vn):
         //***
         double Pu = polynomial(nlat, nlon, nhgt, theLineNumCoef);
         double Qu = polynomial(nlat, nlon, nhgt, theLineDenCoef);
         double Pv = polynomial(nlat, nlon, nhgt, theSampNumCoef);
         double Qv = polynomial(nlat, nlon, nhgt, theSampDenCoef);
         double Un  = Pu / Qu;
         double Vn  = Pv / Qv;
         
         //***
         // Compute the actual line (U) and sample (V):
         //***
         double U  = Un*theLineScale + theLineOffset;
         double V  = Vn*theSampScale + theSampOffset;

         //***
         // Compute the partials of each polynomial wrt lat, lon, hgt
         //***
         double dPu_dLat, dQu_dLat, dPv_dLat, dQv_dLat;
         double dPu_dLon, dQu_dLon, dPv_dLon, dQv_dLon;
         double dPu_dHgt, dQu_dHgt, dPv_dHgt, dQv_dHgt;
         dPu_dLat = dPoly_dLat(nlat, nlon, nhgt, theLineNumCoef);
         dQu_dLat = dPoly_dLat(nlat, nlon, nhgt, theLineDenCoef);
         dPv_dLat = dPoly_dLat(nlat, nlon, nhgt, theSampNumCoef);
         dQv_dLat = dPoly_dLat(nlat, nlon, nhgt, theSampDenCoef);
         dPu_dLon = dPoly_dLon(nlat, nlon, nhgt, theLineNumCoef);
         dQu_dLon = dPoly_dLon(nlat, nlon, nhgt, theLineDenCoef);
         dPv_dLon = dPoly_dLon(nlat, nlon, nhgt, theSampNumCoef);
         dQv_dLon = dPoly_dLon(nlat, nlon, nhgt, theSampDenCoef);
         dPu_dHgt = dPoly_dHgt(nlat, nlon, nhgt, theLineNumCoef);
         dQu_dHgt = dPoly_dHgt(nlat, nlon, nhgt, theLineDenCoef);
         dPv_dHgt = dPoly_dHgt(nlat, nlon, nhgt, theSampNumCoef);
         dQv_dHgt = dPoly_dHgt(nlat, nlon, nhgt, theSampDenCoef);
         
         //***
         // Compute partials of quotients U and V wrt lat, lon, hgt 
         //***
         double dU_dLat, dU_dLon, dU_dHgt, dV_dLat, dV_dLon, dV_dHgt;
         dU_dLat = (Qu*dPu_dLat - Pu*dQu_dLat)/(Qu*Qu);
         dU_dLon = (Qu*dPu_dLon - Pu*dQu_dLon)/(Qu*Qu);
         dU_dHgt = (Qu*dPu_dHgt - Pu*dQu_dHgt)/(Qu*Qu);
         dV_dLat = (Qv*dPv_dLat - Pv*dQv_dLat)/(Qv*Qv);
         dV_dLon = (Qv*dPv_dLon - Pv*dQv_dLon)/(Qv*Qv);
         dV_dHgt = (Qv*dPv_dHgt - Pv*dQv_dHgt)/(Qv*Qv);
         
         //***
         // Apply necessary scale factors 
         //***
        dU_dLat *= theLineScale/theLatScale;
        dU_dLon *= theLineScale/theLonScale;
        dU_dHgt *= theLineScale/theHgtScale;
        dV_dLat *= theSampScale/theLatScale;
        dV_dLon *= theSampScale/theLonScale;
        dV_dHgt *= theSampScale/theHgtScale;

        dU_dLat *= DEG_PER_RAD;
        dU_dLon *= DEG_PER_RAD;
        dV_dLat *= DEG_PER_RAD;
        dV_dLon *= DEG_PER_RAD;

         // Save the partials referenced to ECF
         ossimEcefPoint location(pos);
         NEWMAT::Matrix jMat(3,3);
         pos.datum()->ellipsoid()->jacobianWrtEcef(location, jMat);
         //  Line
         theParWRTx.u = dU_dLat*jMat(1,1)+dU_dLon*jMat(2,1)+dU_dHgt*jMat(3,1);
         theParWRTy.u = dU_dLat*jMat(1,2)+dU_dLon*jMat(2,2)+dU_dHgt*jMat(3,2);
         theParWRTz.u = dU_dLat*jMat(1,3)+dU_dLon*jMat(2,3)+dU_dHgt*jMat(3,3);
         //  Samp
         theParWRTx.v = dV_dLat*jMat(1,1)+dV_dLon*jMat(2,1)+dV_dHgt*jMat(3,1);
         theParWRTy.v = dV_dLat*jMat(1,2)+dV_dLon*jMat(2,2)+dV_dHgt*jMat(3,2);
         theParWRTz.v = dV_dLat*jMat(1,3)+dV_dLon*jMat(2,3)+dV_dHgt*jMat(3,3);

         // Residuals
         ossimDpt resid(theObs.samp-V, theObs.line-U);
         returnData = resid;
      }

      //******************************************
      // P_WRT_X, P_WRT_Y, P_WRT_Z modes
      //   [1] 3 separate calls required
      //   [2] return 3 sets of partials
      //******************************************
      else if (derivMode==P_WRT_X)
      {
         returnData = theParWRTx;
      }

      else if (derivMode==P_WRT_Y)
      {
         returnData = theParWRTy;
      }

      else
      {
         returnData = theParWRTz;
      }

      return returnData;
   }
}

double ossimRpcModel::getBiasError() const
{
   return theBiasError;
}

double ossimRpcModel::getRandError() const
{
   return theRandError;
}


//*****************************************************************************
//  METHOD: ossimSarModel::getRpcParameters)
//  
//  Accessor for RPC parameter set.
//  
//*****************************************************************************
void ossimRpcModel::getRpcParameters(ossimRpcModel::rpcModelStruct& model) const
{
   model.lineScale  = theLineScale;
   model.sampScale  = theSampScale;
   model.latScale   = theLatScale;
   model.lonScale   = theLonScale;
   model.hgtScale   = theHgtScale;
   model.lineOffset = theLineOffset;
   model.sampOffset = theSampOffset;
   model.latOffset  = theLatOffset;
   model.lonOffset  = theLonOffset;
   model.hgtOffset  = theHgtOffset;
   
   for (int i=0; i<20; ++i)
   {
      model.lineNumCoef[i] = theLineNumCoef[i];
      model.lineDenCoef[i] = theLineDenCoef[i];
      model.sampNumCoef[i] = theSampNumCoef[i];
      model.sampDenCoef[i] = theSampDenCoef[i];
   }
   
   if (thePolyType == A)
   {
      model.type= 'A';
   }
   else
   {
      model.type= 'B';
   }
}
