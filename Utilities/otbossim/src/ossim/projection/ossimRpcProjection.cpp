//*****************************************************************************
// FILE: ossimRpcModel.cc
//
// Copyright (C) 2004 Intelligence Data Systems.
//
// LGPL
// 
// AUTHOR: Garrett Potts
//
//*****************************************************************************
//$Id: ossimRpcProjection.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <ossim/projection/ossimProjectionFactoryRegistry.h>
#include <ossim/projection/ossimRpcProjection.h>
#include <ossim/projection/ossimRpcSolver.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimTieGptSet.h>
#include <ossim/imaging/ossimImageGeometry.h>

RTTI_DEF3(ossimRpcProjection, "ossimRpcProjection", ossimProjection, ossimOptimizableProjection,
          ossimAdjustableParameterInterface);

#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <ossim/matrix/newmatrc.h>

//***
// Define Trace flags for use within this file:
//***
#include <ossim/base/ossimTrace.h>
static ossimTrace traceExec  ("ossimRpcProjection:exec");
static ossimTrace traceDebug ("ossimRpcProjection:debug");

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

static const ossim_int32 INTRACK_OFFSET = 0;
static const ossim_int32 CRTRACK_OFFSET = 1;
static const ossim_int32 INTRACK_SCALE  = 2;
static const ossim_int32 CRTRACK_SCALE  = 3;
static const ossim_int32 MAP_ROTATION   = 4;
//static const ossim_int32 YAW_OFFSET;
static const ossim_int32 NUM_ADJUSTABLE_PARAMS = 5;

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
ossimRpcProjection::ossimRpcProjection()
   : ossimOptimizableProjection(),
     theIntrackOffset(0),
     theCrtrackOffset(0),
     theIntrackScale(0.0),
     theCrtrackScale(0.0),
     theYawSkew   (0.0),
     theCosMapRot (1.0),
     theSinMapRot (0.0)

 {
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection Default Constructor: entering..." << std::endl;

   initAdjustableParameters();
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection Default Constructor: returning..." << std::endl;
}

//*****************************************************************************
//  COPY CONSTRUCTOR: ossimRpcProjection(ossimRpcProjection)
//  
//*****************************************************************************
ossimRpcProjection::ossimRpcProjection(const ossimRpcProjection& model)
   :
      ossimOptimizableProjection(model),
      ossimAdjustableParameterInterface(model),
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
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection Copy Constructor: entering..." << std::endl;

   for (int i=0; i<20; i++)
   {
      theLineNumCoef[i] = model.theLineNumCoef[i];
      theLineDenCoef[i] = model.theLineDenCoef[i];
      theSampNumCoef[i] = model.theSampNumCoef[i];
      theSampDenCoef[i] = model.theSampDenCoef[i];
   }
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection Copy Constructor: returning..." << std::endl;
}

//*****************************************************************************
//  DESTRUCTOR: ~ossimRpcProjection()
//  
//*****************************************************************************
ossimRpcProjection::~ossimRpcProjection()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ~ossimRpcProjection() Destructor: entering..." << std::endl;
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimNotify(ossimNotifyLevel_DEBUG): returning..." << std::endl;
}

ossimObject* ossimRpcProjection::getBaseObject()
{
   return this;
}

const ossimObject* ossimRpcProjection::getBaseObject()const
{
   return this;
}

ossimRpcProjection& 
ossimRpcProjection::operator=(const ossimRpcProjection& source)
{
   if (this != &source)
   {
      ossimOptimizableProjection::operator=(source);

      thePolyType    = source.thePolyType;
      theLineScale   = source.theLineScale;
      theSampScale   = source.theSampScale;
      theLatScale    = source.theLatScale;
      theLonScale    = source.theLonScale;
      theHgtScale    = source.theHgtScale;
      theLineOffset  = source.theLineOffset;
      theSampOffset  = source.theSampOffset;
      theLatOffset   = source.theLatOffset;
      theLonOffset   = source.theLonOffset;
      theHgtOffset   = source.theHgtOffset;
      for (int i=0; i<20; i++)
      {
         theLineNumCoef[i] = source.theLineNumCoef[i];
         theLineDenCoef[i] = source.theLineDenCoef[i];
         theSampNumCoef[i] = source.theSampNumCoef[i];
         theSampDenCoef[i] = source.theSampDenCoef[i];
      }
   }
   return *this;
}

void ossimRpcProjection::setAttributes(ossim_float64 sampleOffset,
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
                                       PolynomialType polyType)
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
}


//*****************************************************************************
//  METHOD: ossimRpcProjection::worldToLineSample()
//  
//  Overrides base class implementation. Directly computes line-sample from
//  the polynomials.
//*****************************************************************************
void ossimRpcProjection::worldToLineSample(const ossimGpt& ground_point,
                                      ossimDpt&       imgPt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection::worldToLineSample(): entering..." << std::endl;

   if(ground_point.isLatNan() ||
      ground_point.isLonNan() )
     {
       imgPt.makeNan();
       return;
     }
         
   //*
   // Normalize the lat, lon, hgt:
   //*
   double nlat = (ground_point.lat - theLatOffset) / theLatScale;
   double nlon = (ground_point.lon - theLonOffset) / theLonScale;
   double nhgt;

   if(ossim::isnan(ground_point.hgt))
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
   imgPt.line = U*(theLineScale+theIntrackScale) + theLineOffset + theIntrackOffset;
   imgPt.samp = V*(theSampScale+theCrtrackScale) + theSampOffset + theCrtrackOffset;


   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection::worldToLineSample(): returning..." << std::endl;

   return;
}

void  ossimRpcProjection::lineSampleToWorld(const ossimDpt& imagePoint,
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
//  METHOD: ossimRpcProjection::lineSampleHeightToWorld()
//  
//  Performs reverse projection of image line/sample to ground point.
//  The imaging ray is intersected with a level plane at height = elev.
//
//  NOTE: U = line, V = sample -- this differs from the convention.
//
//*****************************************************************************
void ossimRpcProjection::lineSampleHeightToWorld(const ossimDpt& image_point,
                                            const double&   ellHeight,
                                            ossimGpt&       gpt) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection::lineSampleHeightToWorld: entering..." << std::endl;

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
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimRpcProjection::lineSampleHeightToWorld: \nMax number of iterations reached in ground point "
                                         << "solution. Results are inaccurate." << endl;
   }

   //***
   // Now un-normalize the ground point lat, lon and establish return quantity:
   //***
   gpt.lat = nlat*theLatScale + theLatOffset;
   gpt.lon = nlon*theLonScale + theLonOffset;
   gpt.hgt = ellHeight;
   
}

ossimGpt ossimRpcProjection::origin()const
{
   return ossimGpt(theLatOffset,
                   theLonOffset,
                   theHgtOffset);
}


ossimDpt ossimRpcProjection::getMetersPerPixel() const
{
   ossimDpt result;
   
//    ossimDpt left  = ossimDpt(theSampOffset-1,
//                              theLineOffset);
//    ossimDpt right = ossimDpt(theSampOffset+1,
//                              theLineOffset);
   ossimDpt top  = ossimDpt(theSampOffset,
                            theLineOffset-1);
   ossimDpt bottom = ossimDpt(theSampOffset,
                              theLineOffset+1);
//    ossimGpt leftG;
//    ossimGpt rightG;
   ossimGpt topG;
   ossimGpt bottomG;
   
//    lineSampleToWorld(left, leftG);
//    lineSampleToWorld(right, rightG);
   lineSampleToWorld(top, topG);
   lineSampleToWorld(bottom, bottomG);
   
//    result.x = (ossimEcefPoint(leftG) - ossimEcefPoint(rightG)).magnitude()/2.0;
   result.y = (ossimEcefPoint(topG) - ossimEcefPoint(bottomG)).magnitude()/2.0;
   result.x = result.y;

   return result;
}

bool ossimRpcProjection::operator==(const ossimProjection& projection) const
{
   if(&projection == this) return true;

   // not implemented yet
   //
   return false;
}

//*****************************************************************************
// PRIVATE METHOD: ossimRpcProjection::polynomial
//  
//  Computes polynomial.
//  
//*****************************************************************************
double ossimRpcProjection::polynomial(const double& P, const double& L,
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
// PRIVATE METHOD: ossimRpcProjection::dPoly_dLat
//  
//  Computes derivative of polynomial wrt normalized Latitude P.
//  
//*****************************************************************************
double ossimRpcProjection::dPoly_dLat(const double& P, const double& L,
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
// PRIVATE METHOD: ossimRpcProjection::dPoly_dLon
//  
//  Computes derivative of polynomial wrt normalized Longitude L.
//  
//*****************************************************************************
double ossimRpcProjection::dPoly_dLon(const double& P, const double& L,
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
//  METHOD: ossimRpcProjection::print()
//  
//  Formatted dump of data members.
//  
//*****************************************************************************
std::ostream& ossimRpcProjection::print(std::ostream& out) const
{
   out << "\nDump of ossimRpcProjection object at " << hex << this << ":\n"
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

   return ossimProjection::print(out);
}

//*****************************************************************************
//  METHOD: ossimRpcProjection::saveState()
//  
//  Saves the model state to the KWL. This KWL also serves as a geometry file.
//  
//*****************************************************************************
bool ossimRpcProjection::saveState(ossimKeywordlist& kwl,
                              const char* prefix) const
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection::saveState(): entering..." << std::endl;

   kwl.add(prefix, ossimKeywordNames::TYPE_KW, MODEL_TYPE);

   //***
   // Hand off to base class for common stuff:
   //***
   ossimProjection::saveState(kwl, prefix);

   kwl.add(prefix,
           POLY_TYPE_KW,
           (char)thePolyType,
           true);
   
   kwl.add(prefix, LINE_SCALE_KW, theLineScale);
   kwl.add(prefix, SAMP_SCALE_KW, theSampScale);
   kwl.add(prefix, LAT_SCALE_KW, theLatScale);
   kwl.add(prefix, LON_SCALE_KW, theLonScale);
   kwl.add(prefix, HGT_SCALE_KW, theHgtScale);
   kwl.add(prefix, LINE_OFFSET_KW, theLineOffset);
   kwl.add(prefix, SAMP_OFFSET_KW, theSampOffset);
   kwl.add(prefix, LAT_OFFSET_KW, theLatOffset);
   kwl.add(prefix, LON_OFFSET_KW, theLonOffset);
   
   kwl.add(prefix, HGT_OFFSET_KW, theHgtOffset);

   for (int i=0; i<NUM_COEFFS; i++)
   {
      kwl.add(prefix, (LINE_NUM_COEF_KW + ossimString::toString(i)).c_str(), theLineNumCoef[i]);
      kwl.add(prefix, (LINE_DEN_COEF_KW + ossimString::toString(i)).c_str(), theLineDenCoef[i]);
      kwl.add(prefix, (SAMP_NUM_COEF_KW + ossimString::toString(i)).c_str(), theSampNumCoef[i]);
      kwl.add(prefix, (SAMP_DEN_COEF_KW + ossimString::toString(i)).c_str(), theSampDenCoef[i]);
   }
      
   saveAdjustments(kwl, prefix);
   
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection::saveState(): returning..." << std::endl;
   return true;
}

//*****************************************************************************
//  METHOD: ossimRpcProjection::loadState()
//  
//  Restores the model's state from the KWL. This KWL also serves as a
//  geometry file.
//  
//*****************************************************************************
bool ossimRpcProjection::loadState(const ossimKeywordlist& kwl,
                              const char* prefix) 
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection::loadState(): entering..." << std::endl;

   const char* value;
   const char* keyword;

   //***
   // Pass on to the base-class for parsing first:
   //***
   bool success = ossimProjection::loadState(kwl, prefix);
   if (!success)
   {
      theErrorStatus++;
      if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection::loadState(): returning with error..." << std::endl;
      return false;
   }
      
   //***
   // Continue parsing for local members:
   //***
   keyword = POLY_TYPE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   thePolyType = (PolynomialType) value[0];
      
   keyword = LINE_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLineScale = ossimString(value).toDouble();
   
   keyword = SAMP_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theSampScale = ossimString(value).toDouble();
   
   keyword = LAT_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLatScale = ossimString(value).toDouble();
   
   keyword = LON_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLonScale = ossimString(value).toDouble();
   
   keyword = HGT_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theHgtScale = ossimString(value).toDouble();
   
   keyword = LINE_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLineOffset = ossimString(value).toDouble();
   
   keyword = SAMP_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theSampOffset = ossimString(value).toDouble();
   
   keyword = LAT_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLatOffset = ossimString(value).toDouble();
   
   keyword = LON_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLonOffset = ossimString(value).toDouble();
   
   keyword = HGT_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theHgtOffset = ossimString(value).toDouble();

   for (int i=0; i<NUM_COEFFS; i++)
   {
      value = kwl.find(prefix, (LINE_NUM_COEF_KW+ossimString::toString(i)).c_str());
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theLineNumCoef[i] = ossimString(value).toDouble();
   
      value = kwl.find(prefix, (LINE_DEN_COEF_KW+ossimString::toString(i)).c_str());
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theLineDenCoef[i] = ossimString(value).toDouble();
   
      value = kwl.find(prefix, (SAMP_NUM_COEF_KW+ossimString::toString(i)).c_str());
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theSampNumCoef[i] = ossimString(value).toDouble();
      
      value = kwl.find(prefix, (SAMP_DEN_COEF_KW+ossimString::toString(i)).c_str());
      if (!value)
      {
         ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::loadState(): Error encountered parsing the following required keyword: "
                                             << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                             << std::endl;
         return false;
      }
      theSampDenCoef[i] = ossimString(value).toDouble();
   }

   loadAdjustments(kwl, prefix);

   if(getNumberOfAdjustableParameters() < 1)
   {
      initAdjustableParameters();
   }
      
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection::loadState(): returning..." << std::endl;

   return true;
}

void ossimRpcProjection::initAdjustableParameters()
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

void ossimRpcProjection::adjustableParametersChanged()
{
   theIntrackOffset    = computeParameterOffset(INTRACK_OFFSET);
   theCrtrackOffset    = computeParameterOffset(CRTRACK_OFFSET);
   theIntrackScale     = computeParameterOffset(INTRACK_SCALE);
   theCrtrackScale     = computeParameterOffset(CRTRACK_SCALE);
   double mapRotation  = computeParameterOffset(MAP_ROTATION);
   theCosMapRot        = ossim::cosd(mapRotation);
   theSinMapRot        = ossim::sind(mapRotation);
}

bool
ossimRpcProjection::setupOptimizer(const ossimString& setup)
{
   ossimKeywordlist kwl;

   if(kwl.addFile(ossimFilename(setup)))
   {
      return loadState(kwl);
   }
   else
   {
      ossimRefPtr<ossimProjection> proj = ossimProjectionFactoryRegistry::instance()->createProjection(setup);
      if(proj.valid())
      {
         kwl.clear();
         proj->saveState(kwl);
         
         return loadState(kwl);
      }
   }
   
   return false;
}

ossim_uint32
ossimRpcProjection::degreesOfFreedom()const
{
   ossim_uint32 dof = 0;
   ossim_uint32 idx = 0;
   ossim_uint32 numAdj = getNumberOfAdjustableParameters();
   for(idx = 0; idx < numAdj; ++idx)
   {
      if(!isParameterLocked(idx))
      {
         ++dof;
      }
   }
   
   return dof;
}
//give inverse() partial derivative regarding parameter parmIdx (>=0)
ossimGpt
ossimRpcProjection::getInverseDeriv(int parmIdx, const ossimDpt& ipos, double hdelta)
{   
   double den = 0.5/hdelta;
   ossimGpt res,gd;

   double middle = getAdjustableParameter(parmIdx);
   //set parm to high value
   setAdjustableParameter(parmIdx, middle + hdelta, true);
   res = inverse(ipos);
   //set parm to low value and gte difference
   setAdjustableParameter(parmIdx, middle - hdelta, true);
   gd = inverse(ipos);

   //reset parm
   setAdjustableParameter(parmIdx, middle, true);

   res.lon = den*(res.lon - gd.lon) * 100000.0; //TBC : approx meters
   res.lat = den*(res.lat - gd.lat) * 100000.0 * cos(gd.lat / 180.0 * M_PI);
   res.hgt = den*(res.hgt - gd.hgt);

   return res;
}

//give forward() partial derivative regarding parameter parmIdx (>=0)
ossimDpt
ossimRpcProjection::getForwardDeriv(int parmIdx, const ossimGpt& gpos, double hdelta)
{   
   static double den = 0.5/hdelta;
   ossimDpt res;

   double middle = getAdjustableParameter(parmIdx);
   //set parm to high value
   setAdjustableParameter(parmIdx, middle + hdelta, true);
   res = inverse(gpos);
   //set parm to low value and gte difference
   setAdjustableParameter(parmIdx, middle - hdelta, true);
   res -= inverse(gpos);
   //get partial derivative
   res = res*den;

   //reset parm
   setAdjustableParameter(parmIdx, middle, true);

   return res;
}

double
ossimRpcProjection::optimizeFit(const ossimTieGptSet& tieSet, double* /* targetVariance */)
{
#if 1
   //NOTE : ignore targetVariance
   ossimRefPtr<ossimRpcSolver> solver = new ossimRpcSolver(false, false); //TBD : choices should be part of setupFromString

   std::vector<ossimDpt> imagePoints;
   std::vector<ossimGpt> groundPoints;
   tieSet.getSlaveMasterPoints(imagePoints, groundPoints);
   solver->solveCoefficients(imagePoints, groundPoints);

   ossimRefPtr< ossimImageGeometry > optProj = solver->createRpcProjection();
   if (!optProj)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimRpcProjection::optimizeFit(): error when optimizing the RPC with given tie points"
                                             << std::endl;
      return -1.0;
   }

   if(optProj->hasProjection())
   {
      ossimKeywordlist kwl;
      optProj->getProjection()->saveState(kwl);
      this->loadState(kwl);
   }

   return std::pow(solver->getRmsError(), 2); //variance in pixel^2
#else
   // COPIED from ossimRpcProjection
   //
   //
   //use a simple Levenberg-Marquardt non-linear optimization
   //note : please limit the number of tie points
   //
   //INPUTS: requires Jacobian matrix (partial derivatives with regards to parameters)
   //OUPUTS: will also compute parameter covariance matrix
   //
   //TBD: use targetVariance!
 
   int np = getNumberOfAdjustableParameters();
   int nobs = tieSet.size();

   //setup initail values
   int iter=0;
   int iter_max = 200;
   double minResidue = 1e-10; //TBC
   double minDelta = 1e-10; //TBC

   //build Least Squares initial normal equation
   // don't waste memory, add samples one at a time
   NEWMAT::SymmetricMatrix A;
   NEWMAT::ColumnVector residue;
   NEWMAT::ColumnVector projResidue;
   double deltap_scale = 1e-4; //step_Scale is 1.0 because we expect parameters to be between -1 and 1
   buildNormalEquation(tieSet, A, residue, projResidue, deltap_scale);
   double ki2=residue.SumSquare();

   //get current adjustment (between -1 and 1 normally) and convert to ColumnVector
   ossimAdjustmentInfo cadj;
   getAdjustment(cadj);
   std::vector< ossimAdjustableParameterInfo >& parmlist = cadj.getParameterList();
   NEWMAT::ColumnVector cparm(np), nparm(np);
   for(int n=0;n<np;++n)
   {
      cparm(n+1) = parmlist[n].getParameter();
   }

   double damping_speed = 2.0;
   //find max diag element for A
   double maxdiag=0.0;
   for(int d=1;d<=np;++d) {
      if (maxdiag < A(d,d)) maxdiag=A(d,d);
   }
   double damping = 1e-3 * maxdiag;
   double olddamping = 0.0;
   bool found = false;

//DEBUG TBR
cout<<"rms="<<sqrt(ki2/nobs)<<" ";
cout.flush();

   while ( (!found) && (iter < iter_max) ) //non linear optimization loop
   {
      bool decrease = false;

      do
      {
         //add damping update to normal matrix
         for(int d=1;d<=np;++d) A(d,d) += damping - olddamping;
         olddamping = damping;

         NEWMAT::ColumnVector deltap = solveLeastSquares(A, projResidue);

         if (deltap.NormFrobenius() <= minDelta) 
         {
            found = true;
         } else {
            //update adjustment
            nparm = cparm + deltap;
            for(int n=0;n<np;++n)
            {
               setAdjustableParameter(n, nparm(n+1), false); //do not update now, wait
            }
            adjustableParametersChanged();

            //check residue is reduced
            NEWMAT::ColumnVector newresidue = getResidue(tieSet);
            double newki2=newresidue.SumSquare();
            double res_reduction = (ki2 - newki2) / (deltap.t()*(deltap*damping + projResidue)).AsScalar();
 //DEBUG TBR
       cout<<sqrt(newki2/nobs)<<" ";
       cout.flush();

            if (res_reduction > 0)
            {
               //accept new parms
               cparm = nparm;
               ki2=newki2;

               deltap_scale = max(1e-15, deltap.NormInfinity()*1e-4);

               buildNormalEquation(tieSet, A, residue, projResidue, deltap_scale);
               olddamping = 0.0;

               found = ( projResidue.NormInfinity() <= minResidue );
               //update damping factor
               damping *= std::max( 1.0/3.0, 1.0-std::pow((2.0*res_reduction-1.0),3));
               damping_speed = 2.0;
               decrease = true;
            } else {
               //cancel parameter update
               for(int n=0;n<np;++n)
               {
                  setAdjustableParameter(n, nparm(n+1), false); //do not update right now
               }
               adjustableParametersChanged();

               damping *= damping_speed;
               damping_speed *= 2.0;
            }
         }
      } while (!decrease && !found);
      ++iter;
   }

//DEBUG TBR
cout<<endl;

   //compute parameter correlation
   // use normal matrix inverse
   //TBD

   return ki2/nobs;
#endif
}

void
ossimRpcProjection::buildNormalEquation(const ossimTieGptSet& tieSet,
                                      NEWMAT::SymmetricMatrix& A,
                                      NEWMAT::ColumnVector& residue,
                                      NEWMAT::ColumnVector& projResidue,
                                      double pstep_scale)
{
   //goal:       build Least Squares system
   //constraint: never store full Jacobian matrix in memory (can be huge)
   //            so we build the matrices incrementally
   // the system can be built using forward() or inverse() depending on the projection capabilities : useForward()
   //
   //TBD : add covariance matrix for each tie point

   //init
   int np = getNumberOfAdjustableParameters();
   int dimObs;
   bool useImageObs = useForward(); //caching
   if (useImageObs)
   {
      dimObs = 2; //image observation
   } else {
      dimObs = 3; //ground observations
   }
   int no = dimObs * tieSet.size(); //number of observations

   A.ReSize(np);
   residue.ReSize(no);
   projResidue.ReSize(np);
   //Zeroify matrices that will be accumulated
   A           = 0.0;
   projResidue = 0.0;

   const vector<ossimRefPtr<ossimTieGpt> >& theTPV = tieSet.getTiePoints();
   vector<ossimRefPtr<ossimTieGpt> >::const_iterator tit;
   unsigned long c=1;

   if (useImageObs)
   { 
     //image observations 
     ossimDpt* imDerp = new ossimDpt[np];
     ossimDpt resIm;
     // loop on tie points
      for (tit = theTPV.begin() ; tit != theTPV.end() ; ++tit)
      {
         //compute residue
         resIm = (*tit)->tie - forward(*(*tit));
         residue(c++) = resIm.x;
         residue(c++) = resIm.y;

         //compute all image derivatives regarding parametres for the tie point position
         for(int p=0;p<np;++p)
         {
            imDerp[p] = getForwardDeriv( p , *(*tit) , pstep_scale);
         }

         //compute influence of tie point on all sytem elements
         for(int p1=0;p1<np;++p1)
         {        
            //proj residue: J * residue
            projResidue.element(p1) += imDerp[p1].x * resIm.x + imDerp[p1].y * resIm.y;

            //normal matrix A = transpose(J)*J
            for(int p2=p1;p2<np;++p2)
            {
               A.element(p1,p2) += imDerp[p1].x * imDerp[p2].x + imDerp[p1].y * imDerp[p2].y;
            }
         }
      }
      delete []imDerp;
   }
   else
   {
      // ground observations
      std::vector<ossimGpt> gdDerp(np);
      ossimGpt gd, resGd;
      // loop on tie points
      for (tit = theTPV.begin() ; tit != theTPV.end() ; ++tit)
      {
         //compute residue
         gd = inverse((*tit)->tie);
         residue(c++) = resGd.lon = ((*tit)->lon - gd.lon) * 100000.0;
         residue(c++) = resGd.lat = ((*tit)->lat - gd.lat) * 100000.0 * cos(gd.lat / 180.0 * M_PI);
         residue(c++) = resGd.hgt = (*tit)->hgt - gd.hgt; //TBD : normalize to meters?

         //compute all image derivatives regarding parametres for the tie point position
         for(int p=0;p<np;++p)
         {
            gdDerp[p] = getInverseDeriv( p , (*tit)->tie, pstep_scale);
         }

         //compute influence of tie point on all sytem elements
         for(int p1=0;p1<np;++p1)
         {        
            //proj residue: J * residue
            projResidue.element(p1) += gdDerp[p1].lon * resGd.lon + gdDerp[p1].lat * resGd.lat + gdDerp[p1].hgt * resGd.hgt; //TBC

            //normal matrix A = transpose(J)*J
            for(int p2=p1;p2<np;++p2)
            {
               A.element(p1,p2) += gdDerp[p1].lon * gdDerp[p2].lon + gdDerp[p1].lat * gdDerp[p2].lat + gdDerp[p1].hgt * gdDerp[p2].hgt;
            }
         }
      }
   } //end of if (useImageObs)
}

NEWMAT::ColumnVector
ossimRpcProjection::getResidue(const ossimTieGptSet& tieSet)
{
   //init
   NEWMAT::ColumnVector residue;
   int dimObs;

   bool useImageObs = useForward(); //caching
   if (useImageObs)
   {
      dimObs = 2; //image observation
   } else {
      dimObs = 3; //ground observations
   }
   int no = dimObs * tieSet.size(); //number of observations

   residue.ReSize(no);

   const vector<ossimRefPtr<ossimTieGpt> >& theTPV = tieSet.getTiePoints();
   vector<ossimRefPtr<ossimTieGpt> >::const_iterator tit;
   unsigned long c=1;

   if (useImageObs)
   { 
     //image observations 
     ossimDpt resIm;
     // loop on tie points
      for (tit = theTPV.begin() ; tit != theTPV.end() ; ++tit)
      {
         //compute residue
         resIm = (*tit)->tie - forward(**tit);
         residue(c++) = resIm.x;
         residue(c++) = resIm.y;
      }
   } else {
      // ground observations
      ossimGpt gd;
      // loop on tie points
      for (tit = theTPV.begin() ; tit != theTPV.end() ; ++tit)
      {
         //compute residue
         gd = inverse((*tit)->tie);
         residue(c++) = ((*tit)->lon - gd.lon) * 100000.0; //approx meters //TBC TBD
         residue(c++) = ((*tit)->lat - gd.lat) * 100000.0 * cos(gd.lat / 180.0 * M_PI);
         residue(c++) = (*tit)->hgt - gd.hgt; //meters
      }
   } //end of if (useImageObs)

   return residue;
}

/*!
 * solves Ax = r , with A symmetric positive definite
 * A can be rank deficient
 * size of A is typically between 10 and 100 rows
 */
NEWMAT::ColumnVector 
ossimRpcProjection::solveLeastSquares(NEWMAT::SymmetricMatrix& A,  NEWMAT::ColumnVector& r)const
{
   NEWMAT::ColumnVector x = invert(A)*r;
   return x;
}

/** 
 * stable invert stolen from ossimRpcSolver
 */
NEWMAT::Matrix 
ossimRpcProjection::invert(const NEWMAT::Matrix& m)const
{
   ossim_uint32 idx = 0;
   NEWMAT::DiagonalMatrix d;
   NEWMAT::Matrix u;
   NEWMAT::Matrix v;

   // decompose m.t*m which is stored in Temp into the singular values and vectors.
   //
   NEWMAT::SVD(m, d, u, v, true, true);
   
   // invert the diagonal
   // this is just doing the reciprical fo all diagonal components and store back int
   // d.  ths compute d inverse.
   //
   for(idx=0; idx < (ossim_uint32)d.Ncols(); ++idx)
   {
      if(d[idx] > 1e-14) //TBC : use DBL_EPSILON ?
      {
         d[idx] = 1.0/d[idx];
      }
      else
      {
         d[idx] = 0.0;

//DEBUG TBR
cout<<"warning: singular matrix in SVD"<<endl;

      }
   }

   //compute inverse of decomposed m;
   return v*d*u.t();
}
