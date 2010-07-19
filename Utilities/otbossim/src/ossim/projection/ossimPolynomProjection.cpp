//*****************************************************************************
// FILE: ossimPolynomProjection.cpp
// AUTHOR: Frederic Claudel Meraka/CSIR, oct 2005
//*****************************************************************************
//TODO: speed up getMetersPerPixel using polynom derivatives
//

#include <ossim/projection/ossimPolynomProjection.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>

RTTI_DEF2(ossimPolynomProjection, "ossimPolynomProjection", ossimProjection, ossimOptimizableProjection);

#include <ossim/base/ossimGpt.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimTieGptSet.h>
#include <ossim/base/ossimString.h>

#include <iostream>
#include <sstream>
#include <float.h>

// keywords
static const char* MODEL_TYPE        = "ossimPolynomProjection";
static const char* LAT_SCALE_KW      = "lat_scale";
static const char* LON_SCALE_KW      = "long_scale";
static const char* HGT_SCALE_KW      = "height_scale";
static const char* LAT_OFFSET_KW     = "lat_off";
static const char* LON_OFFSET_KW     = "long_off";
static const char* HGT_OFFSET_KW     = "height_off";
static const char* POLY_LINE_KW      = "poly_line";
static const char* POLY_SAMP_KW      = "poly_samp";

ossimPolynomProjection::ossimPolynomProjection()
   :
   ossimOptimizableProjection(),
   theLonOffset(0.0),
   theLonScale(1.0),
   theLatOffset(0.0),
   theLatScale(1.0),
   theHgtOffset(0.0),
   theHgtScale(1e-5)
 {}

ossimPolynomProjection::ossimPolynomProjection(const ossimPolynomProjection& model)
   :
   ossimOptimizableProjection(model),

   thePolyLine(model.thePolySamp),
   thePolySamp(model.thePolyLine),

   thePolyLine_DLon(model.thePolyLine_DLon),
   thePolyLine_DLat(model.thePolyLine_DLat),
   thePolySamp_DLon(model.thePolySamp_DLon),
   thePolySamp_DLat(model.thePolySamp_DLat),

   theLonOffset(model.theLonOffset),
   theLonScale(model.theLonScale),
   theLatOffset(model.theLatOffset),
   theLatScale(model.theLatScale),
   theHgtOffset(model.theHgtOffset),
   theHgtScale(model.theHgtScale)
{
}

ossimPolynomProjection::~ossimPolynomProjection()
{}

//*****************************************************************************
//  METHOD: ossimPolynomProjection::worldToLineSample()
//  
//  Overrides base class implementation. Directly computes line-sample from
//  the polynomials.
//*****************************************************************************
void 
ossimPolynomProjection::worldToLineSample(const ossimGpt& ground_point,
                                          ossimDpt&       imgPt)const
{
   if(ground_point.isLatNan() || ground_point.isLonNan() )
     {
       imgPt.makeNan();
       return;
     }

   //***
   // Normalize the lat, lon, hgt:
   //***
   vector<double> gpt(3);
   gpt[0] = (ground_point.lon - theLonOffset) * theLonScale;
   gpt[1] = (ground_point.lat - theLatOffset) * theLatScale;
   
   if(ossim::isnan(ground_point.hgt)||ossim::almostEqual(theHgtOffset, 0.0))
   {
      gpt[2] = 0.0;
   }
   else
   {
      gpt[2] = (ground_point.hgt - theHgtOffset) * theHgtScale;
   }
   
   imgPt.x = thePolySamp.eval(gpt);
   imgPt.y = thePolyLine.eval(gpt);

   return;
}

void  
ossimPolynomProjection::lineSampleToWorld(const ossimDpt& imagePoint,
                                          ossimGpt&       worldPoint)const
{
   // how does this work on a DEM??
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
//  METHOD: ossimPolynomProjection::lineSampleHeightToWorld()
//  
//  Performs reverse projection of image line/sample to ground point.
//  The imaging ray is intersected with a level plane at height = elev.
//
//  NOTE: U = line, V = sample -- this differs from the convention.
//
//*****************************************************************************
void ossimPolynomProjection::lineSampleHeightToWorld(const ossimDpt& image_point,
                                            const double&   ellHeight,
                                            ossimGpt&       gpt) const
{
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
   double U    = image_point.y;
   double V    = image_point.x;


   //***
   // Initialize quantities to be used in the iteration for ground point:
   //***
   vector<double> ngpt(3);
   ngpt[0] = 0.0; //normalized longitude (center)
   ngpt[1] = 0.0; //normalized latitude
   if(ossim::isnan(ellHeight)||ossim::almostEqual(theHgtOffset, 0.0))
   {
      ngpt[2] = 0;  // norm height
   }
   else
   {
      ngpt[2] = (ellHeight - theHgtOffset) * theHgtScale;  // norm height
   }
   
   double epsilonU = CONVERGENCE_EPSILON;
   double epsilonV = CONVERGENCE_EPSILON;
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
      Uc = thePolyLine.eval(ngpt); //TBC
      Vc = thePolySamp.eval(ngpt); //TBC
      
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
         // Analytically compute partials of quotients U and V wrt lat, lon: 
         //***
         dU_dLat = thePolyLine_DLat.eval(ngpt);
         dU_dLon = thePolyLine_DLon.eval(ngpt);
         dV_dLat = thePolySamp_DLat.eval(ngpt);
         dV_dLon = thePolySamp_DLon.eval(ngpt);
         
         W = dU_dLon*dV_dLat - dU_dLat*dV_dLon;
         
         //***
         // Now compute the corrections to normalized lat, lon:
         //***
         deltaLat = (dU_dLon*deltaV - dV_dLon*deltaU) / W;
         deltaLon = (dV_dLat*deltaU - dU_dLat*deltaV) / W;
         ngpt[0] += deltaLon;
         ngpt[1] += deltaLat;
      }
      
      iteration++;
      
   } while (((fabs(deltaU)>epsilonU) || (fabs(deltaV)>epsilonV))
            && (iteration < MAX_NUM_ITERATIONS));
      
   //***
   // Test for exceeding allowed number of iterations. Flag error if so:
   //***
   if (iteration == MAX_NUM_ITERATIONS)
   {
      ossimNotify(ossimNotifyLevel_WARN) << "WARNING ossimPolynomProjection::lineSampleHeightToWorld: \nMax number of iterations reached in ground point "
                                         << "solution. Results are inaccurate." << endl;
   }

   //***
   // Now un-normalize the ground point lat, lon and establish return quantity:
   //***
   gpt.lat = ngpt[1] / theLatScale + theLatOffset;
   gpt.lon = ngpt[0] / theLonScale + theLonOffset;
   gpt.hgt = ellHeight;   
}

ossimGpt ossimPolynomProjection::origin()const
{
   //TBC should return samp = 0, line = 0, but this has no meaning here
   return ossimGpt(theLatOffset,
                   theLonOffset,
                   theHgtOffset);
}


ossimDpt ossimPolynomProjection::getMetersPerPixel() const
{
   //TBC TBD : use derivative polynoms (this is approximative anyway)

   ossimGpt centerG(theLatOffset, theLonOffset, theHgtOffset);
   ossimDpt centerI;
   worldToLineSample(centerG,centerI);

   ossimDpt left  = centerI + ossimDpt(-1,0);
   ossimDpt right = centerI + ossimDpt(1,0);
   ossimDpt top  = centerI + ossimDpt(0,-1);
   ossimDpt bottom = centerI + ossimDpt(0,1);

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

bool ossimPolynomProjection::operator==(const ossimProjection& projection) const
{
   if(&projection == this) return true;

   //TBD TBC: compare polynoms after applying affine transforms (=polynom)
   // TBD => need polynom composition
   return false;
}


//*****************************************************************************
//  METHOD: ossimPolynomProjection::print()
//  
//  Formatted dump of data members.
//  
//*****************************************************************************
std::ostream& ossimPolynomProjection::print(std::ostream& out) const
{
   out << "\nDump of ossimPolynomProjection object at " << hex << this << ":\n"
       << LAT_SCALE_KW   << ": " << theLatScale   << "\n"
       << LON_SCALE_KW   << ": " << theLonScale   << "\n"
       << HGT_SCALE_KW   << ": " << theHgtScale   << "\n"
       << LAT_OFFSET_KW  << ": " << theLatOffset  << "\n"
       << LON_OFFSET_KW  << ": " << theLonOffset  << "\n"
       << HGT_OFFSET_KW  << ": " << theHgtOffset  << "\n"
       << "PolySamp: "   <<thePolySamp << "\n"
       << "PolyLine: "   <<thePolyLine << "\n";
   out << endl;

   return ossimProjection::print(out);  
}

//*****************************************************************************
//  METHOD: ossimPolynomProjection::saveState()
//  
//  Saves the model state to the KWL. This KWL also serves as a geometry file.
//  
//*****************************************************************************
bool ossimPolynomProjection::saveState(ossimKeywordlist& kwl,
                              const char* prefix) const
{ 
   kwl.add(prefix, ossimKeywordNames::TYPE_KW, MODEL_TYPE);

   ossimProjection::saveState(kwl, prefix);
   
   kwl.add(prefix, LAT_SCALE_KW, theLatScale);
   kwl.add(prefix, LON_SCALE_KW, theLonScale);
   kwl.add(prefix, HGT_SCALE_KW, theHgtScale);
   kwl.add(prefix, LAT_OFFSET_KW, theLatOffset);
   kwl.add(prefix, LON_OFFSET_KW, theLonOffset);   
   kwl.add(prefix, HGT_OFFSET_KW, theHgtOffset);

   ostringstream polyLineS;
   thePolyLine.print(polyLineS);
   ostringstream polySampS;
   thePolySamp.print(polySampS);

   kwl.add(prefix, POLY_LINE_KW, polyLineS.str().c_str());
   kwl.add(prefix, POLY_SAMP_KW, polySampS.str().c_str());
   //note: dont't save derivatives

   return true;
}

//*****************************************************************************
//  METHOD: ossimPolynomProjection::loadState()
//  
//  Restores the model's state from the KWL. This KWL also serves as a
//  geometry file.
//  
//*****************************************************************************
bool ossimPolynomProjection::loadState(const ossimKeywordlist& kwl,
                              const char* prefix) 
{
   const char* value;
   const char* keyword;

   //***
   // Pass on to the base-class for parsing first:
   //***
   bool success = ossimProjection::loadState(kwl, prefix);
   if (!success)
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimPolynomProjection::loadState(): ossimProjection::loadState() returning with error..." << endl;
      return false;
   }
      
   //***
   // Continue parsing for local members:
   //***
   
   keyword = LAT_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLatScale = ossimString(value).toDouble();
   
   keyword = LON_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLonScale = ossimString(value).toDouble();
   
   keyword = HGT_SCALE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theHgtScale = ossimString(value).toDouble();
         
   keyword = LAT_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLatOffset = ossimString(value).toDouble();
   
   keyword = LON_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theLonOffset = ossimString(value).toDouble();
   
   keyword = HGT_OFFSET_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   theHgtOffset = ossimString(value).toDouble();

   //load polynoms using streams
   keyword = POLY_LINE_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   std::istringstream polyLineIS;
   polyLineIS.str(std::string(value));
   thePolyLine.import(polyLineIS);

   keyword = POLY_SAMP_KW;
   value = kwl.find(prefix, keyword);
   if (!value)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::loadState(): Error encountered parsing the following required keyword: "
                                          << "<" << keyword << ">. Check the keywordlist for proper syntax."
                                          << std::endl;
      return false;
   }
   std::istringstream polySampIS;
   polySampIS.str(std::string(value));
   thePolySamp.import(polySampIS);

   //init dependent members
   buildDerivatives();
   
   return true;
}

void 
ossimPolynomProjection::setPolyLine(const ossimPolynom< ossim_float64 , 3 >& poly)
{
   thePolyLine = poly;
   buildLineDerivatives();
}

void 
ossimPolynomProjection::setPolySamp(const ossimPolynom< ossim_float64 , 3 >& poly)
{
   thePolySamp = poly;
   buildSampDerivatives();
}

void
ossimPolynomProjection::buildDerivatives()
{
   buildLineDerivatives();
   buildSampDerivatives();
}

void
ossimPolynomProjection::buildLineDerivatives()
{
   thePolyLine.pdiff(0, thePolyLine_DLon);
   thePolyLine.pdiff(1, thePolyLine_DLat);
}

void
ossimPolynomProjection::buildSampDerivatives()
{
   thePolySamp.pdiff(0, thePolySamp_DLon);
   thePolySamp.pdiff(1, thePolySamp_DLat);
}

bool
ossimPolynomProjection::setupOptimizer(const ossimString& setup)
{

   return setupDesiredExponents(setup);
}

ossim_uint32
ossimPolynomProjection::degreesOfFreedom()const
{
   //is number of desired monoms * 2
   return (ossim_uint32)theExpSet.size() * 2;
}

bool
ossimPolynomProjection::setupDesiredExponents(const ossimString& monoms)
{
  bool res=false;
  theExpSet.clear();

  //setup monoms from a xyz1 string, eg: y 1 xy x2y3 x2y2z2
  std::vector< ossimString > spm =   monoms.explode(" \t,;");
    for (std::vector< ossimString >::const_iterator it = spm.begin(); it!=spm.end() ;++it)
  {
     ossimPolynom< ossim_float64 , 3 >::EXP_TUPLE et;
     res = stringToExp(*it, et);
     if (!res)
     {
        ossimNotify(ossimNotifyLevel_FATAL) << 
           "FATAL ossimPolynomProjection::setupDesiredExponents(): bad exponent tuple string: "<<*it<<std::endl;
        return false;
     }
     //add to exponents
     theExpSet.insert(et);
  }
  return true;
}

void
ossimPolynomProjection::setupDesiredExponents(int horizonal_ord, int vertical_ord)
{
   //build a classic exponent set - two passes: first horiz (two dim), then second vertically (one dim)
   theExpSet.clear();
   thePolySamp.addExpTupleRight(2, horizonal_ord, theExpSet );
   thePolySamp.addExpTupleRight(1, vertical_ord, theExpSet );
}

double
ossimPolynomProjection::optimizeFit(const ossimTieGptSet& tieSet, double* /* targetVariance */)
{
   //NOTE: ignore targetVariance

   if (tieSet.size() <=0 )
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::fitToTiePoints(): empty tie point set"<< std::endl;
      return -1.0;
   }

   // find bounds for inputs (ground points) and setup offsets and scales
   // TBD : check anti-meridian crossings + modulo 360!
   ossimGpt gmin,gmax;
   tieSet.getGroundBoundaries(gmin,gmax);
   if(gmax.isLatNan() || gmax.isLonNan()|| gmin.isLatNan()||gmin.isLonNan())
   {
      return -1.0;
   }
   //get offsets
   theLonOffset = (gmax.lon+gmin.lon)/2.0;
   theLatOffset = (gmax.lat+gmin.lat)/2.0;
   if(ossim::isnan(gmax.hgt)||ossim::isnan(gmin.hgt))
   {
      theHgtOffset = 0.0;
   }
   else if (gmax.hgt <= OSSIM_DEFAULT_MIN_PIX_DOUBLE)
   {
      //no height value found
      theHgtOffset = 0.0;
   }
   else
   {
      theHgtOffset = (gmax.hgt+gmin.hgt)/2.0;
   }

   //get scales
   ossim_float64 lonScale  = (gmax.lon-gmin.lon)/2.0;
   ossim_float64 latScale  = (gmax.lat-gmin.lat)/2.0;
   ossim_float64 hgtScale  = 1.0;

   if(!ossim::isnan(gmax.hgt)&&!ossim::isnan(gmin.hgt))
   {
      hgtScale = (gmax.hgt-gmin.hgt)/2.0;
   }
   //test ranges and correct
   if ((lonScale < DBL_EPSILON) && (latScale < DBL_EPSILON))
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::fitToTiePoints(): ground points are all on the same spot"<< std::endl;
      return -1.0;
   }
   if (lonScale < DBL_EPSILON) lonScale = theLatScale;  
   if (latScale < DBL_EPSILON) latScale = theLonScale;
   
   if (hgtScale < DBL_EPSILON) hgtScale = 1.0; //TBC : arbitrary value in meters

   //store inverted extents
   theLonScale = 1.0/lonScale;
   theLatScale = 1.0/latScale;
   theHgtScale = 1.0/hgtScale;

//    std::cout << "theLonScale = " << theLonScale << std::endl
//              << "theLatScale = " << theLatScale << std::endl
//              << "theHgtScale = " << theHgtScale << std::endl
//              << "theHgtOffset = " << theHgtOffset << std::endl;
   //normalize ground points and store in ossimPolynom input space
   // + store line and samp separately
   vector< ossimPolynom< ossim_float64 , 3 >::VAR_TUPLE > inputs(tieSet.size());
   vector< ossim_float64 > lines(tieSet.size());
   vector< ossim_float64 > samps(tieSet.size());
   const vector<ossimRefPtr<ossimTieGpt> >& theTPV = tieSet.getTiePoints();

   vector< ossimPolynom< ossim_float64 , 3 >::VAR_TUPLE >::iterator  pit;
   vector< ossim_float64 >::iterator sit;
   vector< ossim_float64 >::iterator lit;
   vector<ossimRefPtr<ossimTieGpt> >::const_iterator tit;
   for (tit = theTPV.begin(), pit=inputs.begin(), lit=lines.begin(), sit=samps.begin(); tit!=theTPV.end() ; ++tit,++lit, ++sit,++pit)
   {
      //setup input
      pit->push_back(((*tit)->lon - theLonOffset)*theLonScale);
      pit->push_back(((*tit)->lat - theLatOffset)*theLatScale);
      if (ossim::isnan((*tit)->hgt))
      {
         pit->push_back(theHgtOffset);
      }
      else
      {
         pit->push_back(((*tit)->hgt - theHgtOffset)*theHgtScale);
      }

      //setup line/samp outputs
      *sit = (*tit)->tie.x;
      *lit = (*tit)->tie.y;
   }  

   //fit polynoms to data using least mean squares
   double line_rms=0.0;
   bool resfit = thePolyLine.LMSfit(theExpSet, inputs, lines, &line_rms);
   if (!resfit)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::fitToTiePoints(): lines polynom LMS fit failed "<< std::endl;
      return -1.0;
   }
   double samp_rms=0.0;
   resfit = thePolySamp.LMSfit(theExpSet, inputs, samps, &samp_rms);
   if (!resfit)
   {
      ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::fitToTiePoints(): samples polynom LMS fit failed "<< std::endl;
      return -1.0;
   }

   //init
   buildDerivatives();

//    std::cout << "line_rms = " << line_rms << std::endl
//              << "samp_rms = " << samp_rms << std::endl; 
   return samp_rms*samp_rms + line_rms*line_rms; //variance
}

bool 
ossimPolynomProjection::stringToExp(const ossimString& s, ossimPolynom< ossim_float64 , 3 >::EXP_TUPLE& et)const
{
   et.clear();

   ossimString ts = s.trim().upcase();
   ossimString tkeys("XYZ");

   if (ts.size() == 0)
   {
      return false;
   }
   //check 1
   if (ts[static_cast<std::string::size_type>(0)] == '1')
   {
      for(int i=0;i<3;i++) et.push_back(0);
      return true;
   }

   //loop on symbols
   int ex[3]={0,0,0};

   while (ts.size()>0)
   {
      int symb = getSymbol(ts, tkeys);
      if (symb<0)
      {
            ossimNotify(ossimNotifyLevel_FATAL) << "FATAL ossimPolynomProjection::stringToExp(): cant find any symbol"<< std::endl;
            return false;
      }
      int expo = getExponent(ts);
      if (ex[symb]>0)
      {
         ossimNotify(ossimNotifyLevel_FATAL)
            << "FATAL ossimPolynomProjection::stringToExp(): symbol appears twice: "
            <<tkeys[static_cast<std::string::size_type>(symb)]
            << std::endl;
         return false;
      }
      ex[symb] = expo;
   }
   for(int i=0;i<3;i++) et.push_back(ex[i]);

   return true;
}

int
ossimPolynomProjection::getSymbol(ossimString& ts,const ossimString& symbols) const
{ //remove symbol from string ts and return symbol index, -1 = error
  for(unsigned int i=0;i<symbols.size();++i)
   {
     if (ts.operator[](0) == symbols.operator[](i))
     {
        ts = ts.afterPos(0);
        return i;
     }
   }
   return -1;
}

int
ossimPolynomProjection::getExponent(ossimString& ts) const
{ //remove exponent from string, no exponent means 1
   unsigned int pos=0;
   int expo=0;
   const ossimString& cts(ts);
   while ( (pos<ts.size()) && (cts.operator[](pos)<='9') && (cts.operator[](pos)>='0'))
   {
      expo = 10*expo + (cts.operator[](pos)-'0');
      ++pos;
   }
   //remove exp from string
   if (pos>0) ts=ts.afterPos(pos-1);

   if (expo==0) expo=1;
   return expo;
}
