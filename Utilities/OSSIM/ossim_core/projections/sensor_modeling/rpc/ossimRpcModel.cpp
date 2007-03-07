//*****************************************************************************
// FILE: ossimRpcModel.cc
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
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
//  $Id: ossimRpcModel.cpp,v 1.31 2005/05/06 17:30:15 gpotts Exp $

#include <projections/sensor_modeling/rpc/ossimRpcModel.h>

RTTI_DEF1(ossimRpcModel, "ossimRpcModel", ossimSensorModel);

#include <base/data_types/ossimGpt.h>
#include <base/data_types/ossimDpt.h>
#include <base/data_types/ossimKeywordlist.h>
#include <base/common/ossimKeywordNames.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <algorithm>

//***
// Define Trace flags for use within this file:
//***
#include <base/common/ossimTrace.h>
static ossimTrace traceExec  ("ossimRpcModel:exec");
static ossimTrace traceDebug ("ossimRpcModel:debug");

const ossimFilename ossimRpcModel::INIT_RPC_GEOM_FILENAME ("rpc_init.geom");
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
   : ossimSensorModel(),
     theIntrackOffset(0),
     theCrtrackOffset(0),
     theIntrackScale(0.0),
     theCrtrackScale(0.0),
     theYawSkew   (0.0),
     theCosMapRot (1.0),
     theSinMapRot (0.0)
   
 {
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel Default Constructor: entering..." << std::endl;

   initAdjustableParameters();

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel Default Constructor: returning..." << std::endl;
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
      theYawSkew      (model.theYawSkew),
      theCosMapRot    (model.theCosMapRot),
      theSinMapRot    (model.theSinMapRot)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel Copy Constructor: entering..." << std::endl;

   for (int i=0; i<20; i++)
   {
      theLineNumCoef[i] = model.theLineNumCoef[i];
      theLineDenCoef[i] = model.theLineDenCoef[i];
      theSampNumCoef[i] = model.theSampNumCoef[i];
      theSampDenCoef[i] = model.theSampDenCoef[i];
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel Copy Constructor: returning..." << std::endl;
}

//*****************************************************************************
//  CONSTRUCTOR: ossimRpcModel(kwl)
//  
//  Constructs model from keywordlist geometry file
//  
//*****************************************************************************
ossimRpcModel::ossimRpcModel(const ossimKeywordlist& geom_kwl)
   : ossimSensorModel(),
     theIntrackOffset(0),
     theCrtrackOffset(0),
     theIntrackScale(0.0),
     theCrtrackScale(0.0),
     theYawSkew   (0.0),
     theCosMapRot (1.0),
     theSinMapRot (0.0)
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel(kwl) Constructor: entering..." << std::endl;

   if(geom_kwl.getErrorStatus() == ossimErrorCodes::OSSIM_OK)
   {
      //***
      // Parse keywordlist for geometry:
      //***
      loadState(geom_kwl);
   }
   else
   {
      theErrorStatus++;
   }
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimRpcModel()
//  
//*****************************************************************************
ossimRpcModel::~ossimRpcModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ~ossimRpcModel() Destructor: entering..." << std::endl;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNotify(ossimNotifyLevel_DEBUG): returning..." << std::endl;
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
                                  bool computeGsd)
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

   if(computeGsd)
   {
      ossimGpt centerGround;
      ossimGpt rightGround;
      ossimGpt topGround;
      lineSampleToWorld(ossimDpt(sampleOffset,
                                 lineOffset),
                        centerGround);
      lineSampleToWorld(ossimDpt(sampleOffset+1,
                                 lineOffset),
                        rightGround);
      lineSampleToWorld(ossimDpt(sampleOffset,
                                 lineOffset+1),
                        topGround);
      
      ossimEcefPoint p1 = centerGround;
      ossimEcefPoint p2 = rightGround;
      ossimEcefPoint p3 = topGround;
      
      theGSD.x = (p2-p1).magnitude();
      theGSD.y = (p3-p1).magnitude();
      theMeanGSD = (theGSD.x+theGSD.y)*.5;
   }
}

void ossimRpcModel::setMetersPerPixel(const ossimDpt& metersPerPixel)
{
   theGSD = metersPerPixel;
   theMeanGSD = (theGSD.x+theGSD.y)*.5;
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
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::worldToLineSample(): entering..." << std::endl;

   if(ground_point.isLatNan() ||
      ground_point.isLonNan() )
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

   if(ground_point.hgt == OSSIM_DBL_NAN)
   {
      nhgt = (theHgtScale - theHgtOffset) / theHgtScale;
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
//   img_pt.samp = V*(theSampScale+theCrtrackScale) + theSampOffset + theIntrackOffset - theYawSkew*img_pt.u;
   img_pt.samp = V*(theSampScale+theCrtrackScale) + theSampOffset + theCrtrackOffset - theYawSkew*img_pt.u;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::worldToLineSample(): returning..." << std::endl;
   return;
}

void  ossimRpcModel::lineSampleToWorld(const ossimDpt& imagePoint,
                                       ossimGpt&       worldPoint) const
{
   if(!imagePoint.hasNans())
   {
      lineSampleHeightToWorld(imagePoint,
                              worldPoint.height(),
                              worldPoint);
   }
   else
   {
      worldPoint.makeNan();
   }
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
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::lineSampleHeightToWorld: entering..." << std::endl;

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
   double skew = (image_point.x-theSampOffset - theCrtrackOffset)*theYawSkew;
   double U    = (image_point.y-theLineOffset - theIntrackOffset+skew) / (theLineScale+theIntrackScale);
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

   if(ellHeight == OSSIM_DBL_NAN)
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


void ossimRpcModel::updateModel()
{
   theIntrackOffset    = computeParameterOffset(INTRACK_OFFSET);
   theCrtrackOffset    = computeParameterOffset(CRTRACK_OFFSET);
   theIntrackScale     = computeParameterOffset(INTRACK_SCALE);
   theCrtrackScale     = computeParameterOffset(CRTRACK_SCALE);
//   theYawSkew          = sind(computeParameterOffset(YAW_OFFSET));
   double mapRotation  = computeParameterOffset(MAP_ROTATION);
   theCosMapRot        = cosd(mapRotation);
   theSinMapRot        = sind(mapRotation);
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


ossimDpt ossimRpcModel::extrapolate (const ossimGpt& gp) const
{
  return ossimSensorModel::extrapolate(gp);
//   ossimDpt temp;
  
//   temp.makeNan();
  
//   return temp;
  
}

ossimGpt ossimRpcModel::extrapolate (const ossimDpt& ip,
				       const double& height) const
{
  return ossimSensorModel::extrapolate(ip, height);
//  return ossimGpt(OSSIM_DBL_NAN, OSSIM_DBL_NAN, OSSIM_DBL_NAN, 0);
}

//*****************************************************************************
//  METHOD: ossimRpcModel::print()
//  
//  Formatted dump of data members.
//  
//*****************************************************************************
std::ostream& ossimRpcModel::print(std::ostream& out) const
{
   out << "\nDump of ossimRpcModel object at " << hex << this << ":\n"
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
       << HGT_OFFSET_KW  << ": " << theHgtOffset  << endl;

   for (int i=0; i<NUM_COEFFS; i++)
      out<<"  "<<LINE_NUM_COEF_KW<<"["<<i<<"]: "<<theLineNumCoef[i]<<endl;

   out << endl;
   for (int i=0; i<NUM_COEFFS; i++)
      out<<"  "<<LINE_DEN_COEF_KW<<"["<<i<<"]: "<<theLineDenCoef[i]<<endl;

   out << endl;
   for (int i=0; i<NUM_COEFFS; i++)
      out<<"  "<<SAMP_NUM_COEF_KW<<"["<<i<<"]: "<<theSampNumCoef[i]<<endl;

   out << endl;
   for (int i=0; i<NUM_COEFFS; i++)
      out<<"  "<<SAMP_DEN_COEF_KW<<"["<<i<<"]: "<<theSampDenCoef[i]<<endl;
      
   out << endl;

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

   char value[128];

   kwl.add(prefix, ossimKeywordNames::TYPE_KW, MODEL_TYPE);

   //***
   // Hand off to base class for common stuff:
   //***
   ossimSensorModel::saveState(kwl, prefix);

   //***
   // Save off offsets and scales:
   //***
   sprintf(value, "%c", (char) thePolyType);
   kwl.add(prefix, POLY_TYPE_KW, value);
   
   sprintf(value, "%22.15e", theLineScale);
   kwl.add(prefix, LINE_SCALE_KW, value);
   
   sprintf(value, "%22.15e", theSampScale);
   kwl.add(prefix, SAMP_SCALE_KW, value);
   
   sprintf(value, "%22.15e", theLatScale);
   kwl.add(prefix, LAT_SCALE_KW, value);
   
   sprintf(value, "%22.15e", theLonScale);
   kwl.add(prefix, LON_SCALE_KW, value);
   
   sprintf(value, "%22.15e", theHgtScale);
   kwl.add(prefix, HGT_SCALE_KW, value);
   
   sprintf(value, "%22.15e", theLineOffset);
   kwl.add(prefix, LINE_OFFSET_KW, value);
   
   sprintf(value, "%22.15e", theSampOffset);
   kwl.add(prefix, SAMP_OFFSET_KW, value);
   
   sprintf(value, "%22.15e", theLatOffset);
   kwl.add(prefix, LAT_OFFSET_KW, value);
   
   sprintf(value, "%22.15e", theLonOffset);
   kwl.add(prefix, LON_OFFSET_KW, value);
   
   sprintf(value, "%22.15e", theHgtOffset);
   kwl.add(prefix, HGT_OFFSET_KW, value);

   char key[128];
   for (int i=0; i<NUM_COEFFS; i++)
   {
      sprintf(key,   "%s%02d", LINE_NUM_COEF_KW, i);
      sprintf(value, "%22.15e",    theLineNumCoef[i]);
      kwl.add(prefix, key, value);
      
      sprintf(key,   "%s%02d", LINE_DEN_COEF_KW, i);
      sprintf(value, "%22.15e",    theLineDenCoef[i]);
      kwl.add(prefix, key, value);
      
      sprintf(key,   "%s%02d", SAMP_NUM_COEF_KW, i);
      sprintf(value, "%22.15e",    theSampNumCoef[i]);
      kwl.add(prefix, key, value);
      
      sprintf(key,   "%s%02d", SAMP_DEN_COEF_KW, i);
      sprintf(value, "%22.15e",    theSampDenCoef[i]);
      kwl.add(prefix, key, value);
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
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::loadState(): entering..." << std::endl;

   const char* value;
   const char* keyword;

   //***
   // Pass on to the base-class for parsing first:
   //***
   bool success = ossimSensorModel::loadState(kwl, prefix);
   if (!success)
   {
      theErrorStatus++;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::loadState(): returning with error..." << std::endl;
      return false;
   }
      
   //***
   // Continue parsing for local members:
   //***
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

   char key[128];
   keyword = key;
   for (int i=0; i<NUM_COEFFS; i++)
   {
      sprintf(key,   "%s%02d", LINE_NUM_COEF_KW, i);
      value = kwl.find(prefix, keyword);
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theLineNumCoef[i] = atof(value);
   
      sprintf(key,   "%s%02d", LINE_DEN_COEF_KW, i);
      value = kwl.find(prefix, keyword);
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theLineDenCoef[i] = atof(value);
   
      sprintf(key,   "%s%02d", SAMP_NUM_COEF_KW, i);
      value = kwl.find(prefix, keyword);
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcModel::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theSampNumCoef[i] = atof(value);
      
      sprintf(key,   "%s%02d", SAMP_DEN_COEF_KW, i);
      value = kwl.find(prefix, keyword);
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
   theYawSkew   = 0.0;
   theCosMapRot = 1.0;
   theSinMapRot = 0.0;
   
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
      << "\n"
      << "// RPC Coefficients are specified with indexes. Coefficients \n "
      << "// are specified for the four polynomials: line numerator, line \n"
      << "// denominator, sample numerator, and sample denominator:" << endl;

   for (int i=1; i<=20; i++)
      os << LINE_NUM_COEF_KW << i << ": <float>" << endl; 
   os << endl;
   for (int i=1; i<=20; i++)
      os << LINE_DEN_COEF_KW << i << ": <float>" << endl; 
   os << endl;
   for (int i=1; i<=20; i++)
      os << SAMP_NUM_COEF_KW << i << ": <float>" << endl; 
   os << endl;
   for (int i=1; i<=20; i++)
      os << SAMP_DEN_COEF_KW << i << ": <float>" << endl; 
   os << "\n" <<endl;

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcModel::writeGeomTemplate(): returning..." << std::endl;
   return;
}

