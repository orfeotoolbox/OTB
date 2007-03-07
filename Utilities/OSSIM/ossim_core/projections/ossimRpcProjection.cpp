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
//$Id: ossimRpcProjection.cpp,v 1.4 2005/09/22 11:58:12 gpotts Exp $

#include <projections/ossimRpcProjection.h>
#include <base/data_types/ossimEcefPoint.h>
#include <base/data_types/ossimEcefVector.h>

RTTI_DEF1(ossimRpcProjection, "ossimRpcProjection", ossimProjection);

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

//*****************************************************************************
//  DEFAULT CONSTRUCTOR: ossimRpcModel()
//  
//*****************************************************************************
ossimRpcProjection::ossimRpcProjection()
   : ossimProjection()
 {
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection Default Constructor: entering..." << std::endl;


   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection Default Constructor: returning..." << std::endl;
}

//*****************************************************************************
//  COPY CONSTRUCTOR: ossimRpcProjection(ossimRpcProjection)
//  
//*****************************************************************************
ossimRpcProjection::ossimRpcProjection(const ossimRpcProjection& model)
   :
      ossimProjection(model),
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
      theHgtOffset    (model.theHgtOffset)
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
   //
   // Compute the adjusted, normalized line (U) and sample (V):
   //
   imgPt.x = (polynomial(nlat, nlon, nhgt, theSampNumCoef)/
              polynomial(nlat, nlon, nhgt, theSampDenCoef))*theSampScale + theSampOffset;
   
   imgPt.y = (polynomial(nlat, nlon, nhgt, theLineNumCoef)/
              polynomial(nlat, nlon, nhgt, theLineDenCoef))*theLineScale + theLineOffset ;

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

   //
   // Constants for convergence tests:
   //
   static const int    MAX_NUM_ITERATIONS  = 10;
   static const double CONVERGENCE_EPSILON = 0.1;  // pixels
   
   //
   // The image point must be adjusted by the adjustable parameters as well
   // as the scale and offsets given as part of the RPC param normalization.
   //
   //      NOTE: U = line, V = sample
   //
   double U    = (image_point.y-theLineOffset)/(theLineScale);
   double V    = (image_point.x-theSampOffset)/(theSampScale);


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
   
   double epsilonU = CONVERGENCE_EPSILON/(theLineScale);
   double epsilonV = CONVERGENCE_EPSILON/(theSampScale);
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
   ossimDpt left  = ossimDpt(theSampOffset-1,
                             theLineOffset);
   ossimDpt right = ossimDpt(theSampOffset+1,
                             theLineOffset);
   ossimDpt top  = ossimDpt(theSampOffset,
                            theLineOffset-1);
   ossimDpt bottom = ossimDpt(theSampOffset,
                              theLineOffset+1);
   ossimGpt leftG;
   ossimGpt rightG;
   ossimGpt topG;
   ossimGpt bottomG;
   
   lineSampleToWorld(left, leftG);
   lineSampleToWorld(right, rightG);
   lineSampleToWorld(top, topG);
   lineSampleToWorld(bottom, bottomG);

   ossimDpt result;
   
   result.x = (ossimEcefPoint(leftG) - ossimEcefPoint(rightG)).magnitude()/2.0;
   result.y = (ossimEcefPoint(topG) - ossimEcefPoint(bottomG)).magnitude()/2.0;

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
      
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimRpcProjection::loadState(): returning..." << std::endl;

   return true;
}
