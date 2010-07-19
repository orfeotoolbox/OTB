//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  David Hicks
//
// Description: Base class for Synthetic Aperture Radar model.
//
//              This model represents a standard model using relatively
//              generic support data based on the following references:
//                [1] Modern Photogrammetry; Mikhail, Bethel, & McGlone;
//                    Sections 11.7-11.9.  Equation number references are
//                    provided where possible for added clarity.
//                [2] The Compendium of Controlled Extensions for NITFS
//                    21 Mar 2006, paragraph E.3.8, SAR MPDSR
//
//----------------------------------------------------------------------------
// $Id: ossimSarModel.cpp 17206 2010-04-25 23:20:40Z dburken $

#include <ossim/elevation/ossimHgtRef.h>
#include <ossim/projection/ossimSarModel.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>

RTTI_DEF1(ossimSarModel, "ossimSarModel", ossimSensorModel);

static ossimTrace traceExec (ossimString("ossimSarModel:exec"));
static ossimTrace traceDebug(ossimString("ossimSarModel:debug"));

#ifdef OSSIM_ID_ENABLED
static const char OSSIM_ID[] = "$Id: ossimSarModel.cpp 17206 2010-04-25 23:20:40Z dburken $";
#endif

static const char ACQ_MODE_KW[]         = "acq_mode";
static const char ORP_POS_KW[]          = "orp_pos";
static const char ORP_CENTER_KW[]       = "orp_ctr_xy";
static const char OPNORM_KW[]           = "opnorm";
static const char OP_X_AXIS_KW[]        = "opx";
static const char OIPR_KW[]             = "oipr";
static const char PIX_SIZE_KW[]         = "pixel_size";
static const char ARP_TIME_KW[]         = "arptm";
static const char ARP_POLY_COEFF_X_KW[] = "arppol_coeff_x";
static const char ARP_POLY_COEFF_Y_KW[] = "arppol_coeff_y";
static const char ARP_POLY_COEFF_Z_KW[] = "arppol_coeff_z";
static const char TIME_COEFF[]          = "time_coeff";

static const ossimString PARAM_NAMES[] ={"x_pos_offset",
                                         "y_pos_offset",
                                         "z_pos_offset"};
static const ossimString PARAM_UNITS[] ={"meters",
                                         "meters",
                                         "meters"};
 

//*****************************************************************************
//  METHOD: ossimSarModel::ossimSarModel()
//  
//  Constructor.
//  
//*****************************************************************************
ossimSarModel::ossimSarModel()
   :
   theAcquisitionMode(),
   theOrpPosition(),
   theOrpCenter(),
   theOutputPlaneNormal(),
   theOipr(0.0),
   theLsrOffset(0, 0, 0)
{
   initAdjustableParameters();
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimSarModel::ossimSarModel DEBUG:" << std::endl;
#ifdef OSSIM_ID_ENABLED
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "OSSIM_ID:  " << OSSIM_ID << std::endl;
#endif 
   }
   
}


//*****************************************************************************
//  DESTRUCTOR: ~ossimSarModel()
//  
//*****************************************************************************
ossimSarModel::~ossimSarModel()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG DESTRUCTOR: ~ossimSarModel(): entering..." << std::endl;
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG DESTRUCTOR: ~ossimSarModel(): returning..." << std::endl;
}


//*****************************************************************************
//  METHOD: ossimSarModel::initAdjustableParameters()
//  
//  This method initializes the base class adjustable parameter and associated
//  sigmas arrays with quantities specific to this model. These are default
//  values only. A functional implementation would assign the sigmas via a
//  keywordlist or camera specific derived class.
//  
//*****************************************************************************
void ossimSarModel::initAdjustableParameters()
{
   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSarModel::initAdjustableParameters: entering..." << std::endl;

   int numParams = NUM_ADJUSTABLE_PARAMS;
   resizeAdjustableParameterArray(NUM_ADJUSTABLE_PARAMS);
   
   //***
   // Initialize adjustable parameter array:
   //***
   for (int i=0; i<numParams; i++)
   {
      setAdjustableParameter(i, 0.0);
      setParameterDescription(i, PARAM_NAMES[i]);
      setParameterUnit(i, PARAM_UNITS[i]);
   }
   
   //***
   // Initialize parameter sigma array:
   //***
   setParameterSigma(X_POS, 50.0);
   setParameterSigma(Y_POS, 50.0);
   setParameterSigma(Z_POS, 50.0);  

   if (traceExec())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSarModel::initAdjustableParameters: returning..." << std::endl;
}


//*****************************************************************************
//  METHOD: ossimSarModel::loadState()
//  
//  Load the state of this object from KWL.
//  
//*****************************************************************************
bool ossimSarModel::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   // Call the base class.  If it fails, no sense going on.
   if (ossimSensorModel::loadState(kwl, prefix) == false)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "DEBUG ossimSarModel::loadState(): returning with error..."
            << std::endl;
      }
      
      return false;
   }

   const char* lookup;
   
   // Get the acquisition mode
   lookup = kwl.find(prefix, ACQ_MODE_KW);
   theAcquisitionMode = ossimSarModel::UNKNOWN;
   if (lookup)
   {
      setAcquisitionMode(ossimString(lookup));
   }
   if (theAcquisitionMode == ossimSarModel::UNKNOWN)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << ACQ_MODE_KW << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }

   // Get the ORP (aka Output/Ground Reference Point)
   theOrpPosition.makeNan();
   lookup = kwl.find(prefix, ORP_POS_KW);
   if (lookup)
   {
      theOrpPosition.toPoint(std::string(lookup));
   }

   if (theOrpPosition.hasNans())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << ORP_POS_KW << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }
   
   // Get the ORP image coordinates
   theOrpCenter.makeNan();
   lookup = kwl.find(prefix, ORP_CENTER_KW);
   if (lookup)
   {
      theOrpCenter.toPoint(std::string(lookup));
   }

   if (theOrpCenter.hasNans())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << ORP_CENTER_KW << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }

   // Get the output plane normal (slant plane orientation)
   theOutputPlaneNormal.makeNan();
   lookup = kwl.find(prefix, OPNORM_KW);
   if (lookup)
   {
      theOutputPlaneNormal.toPoint(std::string(lookup));
   }

   if (theOutputPlaneNormal.hasNans())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << OPNORM_KW << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }

   // Get the output plane x-axis (slant plane orientation)
   theOutputPlaneXaxis.makeNan();
   lookup = kwl.find(prefix, OP_X_AXIS_KW);
   if (lookup)
   {
      theOutputPlaneXaxis.toPoint(std::string(lookup));
   }

   if (theOutputPlaneXaxis.hasNans())
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << OP_X_AXIS_KW << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }

   // Get the output IPR
   theOipr = ossim::nan();
   lookup = kwl.find(prefix, OIPR_KW);
   if (lookup)
   {
      theOipr = ossimString::toFloat64(lookup);
   }
   
   if (ossim::isnan(theOipr))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << OIPR_KW << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }

   // Get the pixel size
   thePixelSize = ossim::nan();
   lookup = kwl.find(prefix, PIX_SIZE_KW);
   if (lookup)
   {
      thePixelSize = ossimString::toFloat64(lookup);
   }
   
   if (ossim::isnan(thePixelSize))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << PIX_SIZE_KW << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }

   // Get the ARP time
   theArpTime = ossim::nan();
   lookup = kwl.find(prefix, ARP_TIME_KW);
   if (lookup)
   {
      theArpTime = ossimString::toFloat64(lookup);
   }

   if (ossim::isnan(theArpTime))
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << ARP_TIME_KW << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }

   // Get the ARP position coefficients
   //  (variable degree polynomial representation of position)
   theArpXPolCoeff.clear();
   theArpYPolCoeff.clear();
   theArpZPolCoeff.clear();
   ossim_uint32 x_count = kwl.numberOf(ARP_POLY_COEFF_X_KW);
   ossim_uint32 y_count = kwl.numberOf(ARP_POLY_COEFF_Y_KW);
   ossim_uint32 z_count = kwl.numberOf(ARP_POLY_COEFF_Z_KW);

   if ( (x_count == y_count) && (x_count == z_count) )
   {
      ossim_uint32 found = 0;
      ossim_uint32 count = 0;
   
      while ( (found < x_count) &&
              (count < 100) ) // limit lookups to 100...
      {
         ossimString s = ossimString::toString(count);
         ossimString xkw = ARP_POLY_COEFF_X_KW;
         ossimString ykw = ARP_POLY_COEFF_Y_KW;
         ossimString zkw = ARP_POLY_COEFF_Z_KW;
         xkw += s;
         ykw += s;
         zkw += s;

         const char* xLookup = kwl.find(prefix, xkw.c_str());
         const char* yLookup = kwl.find(prefix, ykw.c_str());
         const char* zLookup = kwl.find(prefix, zkw.c_str());

         if (xLookup && yLookup && zLookup)
         {
            ++found;
            theArpXPolCoeff.push_back(ossimString::toFloat64(xLookup));
            theArpYPolCoeff.push_back(ossimString::toFloat64(yLookup));
            theArpZPolCoeff.push_back(ossimString::toFloat64(zLookup));
         }
         ++count;
      }
   }     
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << "arppol_coeff" << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }

   // Get the time coefficients
   //  (variable degree polynomial representation of time WRT image coordinates)
   theTimeCoeff.clear();
   ossim_uint32 timeCount = kwl.numberOf(TIME_COEFF);
   if (timeCount)
   {
      ossim_uint32 found = 0;
      ossim_uint32 count = 0;
   
      while ( (found < timeCount) &&
              (count < 100) ) // limit lookups to 100...
      {
         ossimString kw = TIME_COEFF;
         kw += ossimString::toString(count);;

         lookup = kwl.find(prefix, kw.c_str());

         if (lookup)
         {
            ++found;
            theTimeCoeff.push_back(ossimString::toFloat64(lookup));
         }
         ++count;
      }
   }     
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << "DEBUG ossimSarModel::loadState() lookup failure: "
            << TIME_COEFF << "\nreturning with error..."
            << std::endl;
      }
      return false;
   }
   
   
   updateModel();

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << "ossimSarModel::loadState() DEBUG: " << std::endl;
   }
   
   return true;
}


//*****************************************************************************
//  METHOD: ossimSarModel::saveState()
//  
//  Save the state of this object to KWL.
//  
//*****************************************************************************
bool ossimSarModel::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   kwl.add(prefix, ACQ_MODE_KW, getAcquistionModeString());
   kwl.add(prefix, ORP_POS_KW, theOrpPosition.toString(15).c_str());
   kwl.add(prefix, ORP_CENTER_KW, theOrpCenter.toString(15).c_str());
   kwl.add(prefix, OPNORM_KW, theOutputPlaneNormal.toString(15).c_str());
   kwl.add(prefix, OP_X_AXIS_KW, theOutputPlaneXaxis.toString(15).c_str());
   kwl.add(prefix, OIPR_KW, theOipr);
   kwl.add(prefix, PIX_SIZE_KW, thePixelSize);
   kwl.add(prefix, ARP_TIME_KW, theArpTime);

   ossim_uint32 i;
   for (i = 0; i < theArpXPolCoeff.size(); ++i)
   {
      ossimString kw = ARP_POLY_COEFF_X_KW;
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, theArpXPolCoeff[i]);
   }
   for (i = 0; i < theArpYPolCoeff.size(); ++i)
   {
      ossimString kw = ARP_POLY_COEFF_Y_KW;
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, theArpYPolCoeff[i]);
   }
   for (i = 0; i < theArpZPolCoeff.size(); ++i)
   {
      ossimString kw = ARP_POLY_COEFF_Z_KW;
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, theArpZPolCoeff[i]);
   }
   for (i = 0; i < theTimeCoeff.size(); ++i)
   {
      ossimString kw = TIME_COEFF;
      kw += ossimString::toString(i);
      kwl.add(prefix, kw, theTimeCoeff[i]);
   }
   
   return ossimSensorModel::saveState(kwl, prefix);
}


//*****************************************************************************
// STATIC METHOD: ossimSarModel::writeGeomTemplate
//  
//  Writes a sample kwl to output stream. Please update this method with any
//  format and/or keyword changes. It will make life a lot easier for everyone.
//  
//*****************************************************************************
void ossimSarModel::writeGeomTemplate(ostream& os)
{
   if (traceExec())
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimSarModel::writeGeomTemplate: entering..." << std::endl;

   os <<
      "//************************************\n"
      "// Template for SAR model keywordlist\n"
      "//************************************\n";
 
   ossimSensorModel::writeGeomTemplate(os);
   os << "//***\n"
      << "// Base-class SAR Keywords:\n"
      << "//***\n"
      << ACQ_MODE_KW         << ": <SCAN, SPOT>\n"
      << ORP_POS_KW          << ": <meters (ECF)>\n"
      << ORP_CENTER_KW       << ": <pixels>\n"
      << OPNORM_KW           << ": <unit vector>\n"
      << OP_X_AXIS_KW        << ": <unit vector>\n"
      << OIPR_KW             << ": <meters>\n"
      << PIX_SIZE_KW         << ": <meters>\n"
      << ARP_TIME_KW         << ": <sec>\n"
      << ARP_POLY_COEFF_X_KW << ": <meters (ECF)>\n"
      << ARP_POLY_COEFF_Y_KW << ": <meters (ECF)>\n"
      << ARP_POLY_COEFF_Z_KW << ": <meters (ECF)>\n"
      << TIME_COEFF          << ": <sec>\n";
   }


//*****************************************************************************
//  METHOD: ossimSarModel::print()
//  
//  Print the KWL.
//  
//*****************************************************************************
std::ostream& ossimSarModel::print(std::ostream& out) const
{
   out << "// ossimSarModel::print:" << std::endl;

   ossimKeywordlist kwl;
   if (saveState(kwl))
   {
      kwl.print(out);
   }
   else
   {
      out << "// ossimSarModel::saveState failed!" << std::endl;
   }
   return ossimSensorModel::print(out);
}


//*****************************************************************************
//  METHOD: ossimSarModel::getAcquisitionMode()
//  
//  Get internal acquisition mode parameter.
//  
//*****************************************************************************
ossimString ossimSarModel::getAcquistionModeString() const
{
   ossimString result;
   
   switch (theAcquisitionMode)
   {
      case ossimSarModel::SCAN:
         result = "scan";
         break;
      case ossimSarModel::SPOT:
         result = "spot";
         break;
      case ossimSarModel::UNKNOWN:
      default:
         result = "unknown";
         break;
   }
   return result;
}


//*****************************************************************************
//  METHOD: ossimSarModel::lineSampleHeightToWorld()
//  
//  Perform inverse projection of image space to ground point @ specified hgt.
//
//    Note: Special case indicated by 'heightAboveEllipsoid=OSSIM_NAN'
//          will utilize the ORP height for intersection.
//  
//*****************************************************************************
void ossimSarModel::lineSampleHeightToWorld(const ossimDpt& lineSampPt,
                                            const double&   heightAboveEllipsoid,
                                            ossimGpt&       worldPt) const
{
   // Compute OP position from image coordinates 
   ossimEcefPoint opPt;
   computeOPfromImage(lineSampPt, opPt);
   
   // Get image time
   ossim_float64 iTime = getArpTime(lineSampPt);
   
   // Get ARP state
   ossimEcefPoint  arpPos = getArpPos(iTime);
   ossimEcefVector arpVel = getArpVel(iTime);
   
   // Get range/Doppler
   ossim_float64 range;
   ossim_float64 doppler;
   computeRangeDoppler(opPt, arpPos, arpVel, range, doppler);
   
   // Set the height reference
   ossim_float64 hgtSet;
   if ( ossim::isnan(heightAboveEllipsoid) )
   {
      ossimGpt orpG(theOrpPosition);
      hgtSet = orpG.height();
   }
   else
   {
      hgtSet = heightAboveEllipsoid;
   }
      
   ossimHgtRef hgtRef(AT_HGT, hgtSet);
   
   // Project OP to DEM
   ossimEcefPoint ellPt;
   projOPtoSurface(opPt, range, doppler, arpPos, arpVel, &hgtRef, ellPt);
   ossimGpt iPt(ellPt);
   
   worldPt = iPt;
}


//*****************************************************************************
//  METHOD: ossimSarModel::worldToLineSample()
//  
//  Perform forward projection of ground point to image space.
//  
//*****************************************************************************
void ossimSarModel::worldToLineSample(const ossimGpt& world_point,
                                      ossimDpt&       image_point) const
{
   ossimEcefPoint pt(world_point);
   
   // Project to OP plane
   ossimEcefPoint opPt;
   projEllipsoidToOP(pt, opPt);
   
   // Compute image coordinates from OP position
   computeImageFromOP(opPt,image_point);
}


//*****************************************************************************
//  METHOD: ossimSarModel::lineSampleToWorld()
//  
//  Perform inverse projection of image space to ground point.
//  
//*****************************************************************************
void ossimSarModel::lineSampleToWorld(const ossimDpt& lineSampPt,
                                      ossimGpt&       worldPt) const
{
   // Compute OP position from image coordinates 
   ossimEcefPoint opPt;
   computeOPfromImage(lineSampPt, opPt);
   
   // Get image time
   ossim_float64 iTime = getArpTime(lineSampPt);
   
   // Get ARP state vector
   ossimEcefPoint  arpPos = getArpPos(iTime);
   ossimEcefVector arpVel = getArpVel(iTime);
   
   // Get range/Doppler
   ossim_float64 range;
   ossim_float64 doppler;
   computeRangeDoppler(opPt, arpPos, arpVel, range, doppler);
   
   // Set the height reference
   ossimHgtRef hgtRef(AT_DEM);
   
   // Project OP to DEM
   ossimEcefPoint ellPt;
   projOPtoSurface(opPt, range, doppler, arpPos, arpVel, &hgtRef, ellPt);
   ossimGpt iPt(ellPt);
   
   worldPt = iPt;

}


//*****************************************************************************
//  METHOD: ossimSarModel::imagingRay()
//  
//  Given an image point, returns a ray originating at the ARP position
//  and pointing towards the target's position in the Output
//  Plane.
//  This DOES NOT provide the conventional definition for an imaging ray
//  because the imaging locus for SAR is not a ray.
//
//  It DOES provide a radius vector for the range/Doppler circle.
//  
//*****************************************************************************
void ossimSarModel::imagingRay(const ossimDpt& image_point,
                               ossimEcefRay&   image_ray) const
{
   // Compute OP position from image coordinates 
   ossimEcefPoint opPt;
   computeOPfromImage(image_point, opPt);
   
   // Get image time
   ossim_float64 iTime = getArpTime(image_point);
   
   // Get ARP position
   ossimEcefPoint arpPos = getArpPos(iTime);
   
   // Construct the ray
   ossimGpt start(arpPos);
   ossimGpt end(opPt);

   image_ray = ossimEcefRay(start, end);

   return;   
}


//*****************************************************************************
//  METHOD: ossimSarModel::getObsCovMat()
//
//  Gives 2X2 covariance matrix of observations.
//
//    Note: At this base class level, the only error source currently
//          considered is mensuration error.  This is obviously optimistic,
//          but is included as a placeholder/example.
//
//*****************************************************************************
ossimSensorModel::CovMatStatus ossimSarModel::getObsCovMat(
   const ossimDpt& /* ipos */, NEWMAT::SymmetricMatrix& Cov)
{
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Mensuration error contribution
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //  Sensitivity matrix
   NEWMAT::SymmetricMatrix B(2);
   B(1,1) = thePixelSpacing;
   B(2,2) = B(1,1);
   B(1,2) = 0.0;
   //  Default pointing error = 0.5 pixels
   ossim_float64 defPointingSigma = 0.5;
   NEWMAT::SymmetricMatrix P(2);
   P(1,1) = defPointingSigma*defPointingSigma;
   P(2,2) = P(1,1);
   P(1,2) = 0.0;
   //  Propagate to rng/az
   NEWMAT::SymmetricMatrix Cm;
   Cm << B * P * B.t();
   
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Sum total covariance in rng/az
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   NEWMAT::SymmetricMatrix Ctot = Cm; //+ other contributors as identified
   
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // Propagate to rng/Dop
   // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   NEWMAT::SymmetricMatrix Bad(2);
   Bad(1,1) = 1.0;
   Bad(2,2) = theParDopWRTaz;
   Bad(1,2) = 0.0;
   
   Cov << Bad * Ctot * Bad.t();

   return ossimSensorModel::COV_PARTIAL;
}

//*****************************************************************************
//  METHOD: ossimSarModel::updateModel()
//  
//  Update computed parameters.
//  
//*****************************************************************************
void ossimSarModel::updateModel()
{
   // Set the pixel size
   thePixelSpacing = thePixelSize;
   
   // Define the output(focus) plane
   //    Ref[1], eq. 11-29 -> 11-31
   theOPZ = theOutputPlaneNormal.unitVector();
   theOPX = theOutputPlaneXaxis.unitVector();
   
   theOPY = theOPZ.cross(theOPX);
   theOPY.normalize();
   
   theOPX = theOPY.cross(theOPZ);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG updateModel:\n OP vectors...";
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "\n  OPX: "<<theOPX
      << "\n  OPY: "<<theOPY
      << "\n  OPZ: "<<theOPZ<<endl;
      ossimGpt orpg(theOrpPosition);
      ossimNotify(ossimNotifyLevel_DEBUG)
      <<"  ORPgeo: "<<orpg<<endl;
   }
   // Update geometry
   theLsrOffset.x() = getAdjustableParameter(X_POS) *
                      getParameterSigma(X_POS);
   theLsrOffset.y() = getAdjustableParameter(Y_POS) *
                      getParameterSigma(Y_POS);
   theLsrOffset.z() = getAdjustableParameter(Z_POS) *
                      getParameterSigma(Z_POS);

   // Set base class members
   theRefGndPt = ossimGpt(theOrpPosition);
   theRefImgPt = theOrpCenter;
   theGSD.samp = thePixelSpacing;
   theGSD.line = thePixelSpacing;
   theMeanGSD  = thePixelSpacing;
   
}


//*****************************************************************************
//  METHOD: ossimSarModel::getArpPos()
//  
//  Return ARP position as constant term.
//  
//*****************************************************************************
ossimEcefPoint ossimSarModel::getArpPos() const
{
   ossimEcefPoint pos(theArpXPolCoeff[0],
                      theArpYPolCoeff[0],
                      theArpZPolCoeff[0]);
                       
   // Handle the adjustable offset
   //   Define ENU space at pos
   ossimGpt posG(pos);
   ossimLsrSpace enu(posG);
   //   Rotate offset vector to ECF
   ossimColumnVector3d tpnn(theLsrOffset[0],theLsrOffset[1], theLsrOffset[2]);
   ossimEcefVector ecfOffset = enu.lsrToEcefRotMatrix()*tpnn;
   //   Add the offset   
   pos = pos + ecfOffset;
   
   return pos;
}


//*****************************************************************************
//  METHOD: ossimSarModel::getArpVel()
//  
//  Return ARP velocity as 1st order term coefficient.
//  
//*****************************************************************************
ossimEcefVector ossimSarModel::getArpVel() const
{
   ossimEcefVector vec(theArpXPolCoeff[1],
                       theArpYPolCoeff[1],
                       theArpZPolCoeff[1]);

   return vec;
}


//*****************************************************************************
//  METHOD: ossimSarModel::getArpPos()
//  
//  Compute ARP position as function of time-dependent polynomial.
//    Note: The polynomial degree/coefficients must be
//    predetermined by the data provider.
//  
//*****************************************************************************
ossimEcefPoint ossimSarModel::getArpPos(const ossim_float64& time) const
{
   ossim_int32 i;
   ossim_uint32 nTermsX = (ossim_uint32)theArpXPolCoeff.size();
   ossim_uint32 nTermsY = (ossim_uint32)theArpYPolCoeff.size();
   ossim_uint32 nTermsZ = (ossim_uint32)theArpZPolCoeff.size();

   ossim_float64 x = theArpXPolCoeff[nTermsX-1];
   for (i=nTermsX-2; i>=0; i--)
      x = x*time + theArpXPolCoeff[i];
      
   ossim_float64 y = theArpYPolCoeff[nTermsY-1];
   for (i=nTermsY-2; i>=0; i--)
      y = y*time + theArpYPolCoeff[i];
      
   ossim_float64 z = theArpZPolCoeff[nTermsZ-1];
   for (i=nTermsZ-2; i>=0; i--)
      z = z*time + theArpZPolCoeff[i];
   
   ossimEcefPoint pos(x, y, z);
                       
   // Handle the adjustable offset
   //   Define ENU space at pos
   ossimGpt posG(pos);
   ossimLsrSpace enu(posG);
   //   Rotate offset vector to ECF
   ossimColumnVector3d tpnn(theLsrOffset[0],theLsrOffset[1], theLsrOffset[2]);
   ossimEcefVector ecfOffset = enu.lsrToEcefRotMatrix()*tpnn;
   //   Add the offset
   pos = pos + ecfOffset;
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG getArpPos:"<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << " time   = "<<time<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << " pos    = "<<pos<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << " offset = "<<theLsrOffset<<endl;
   }
   
   return pos;
}


//*****************************************************************************
//  METHOD: ossimSarModel::getArpVel()
//  
//  Compute ARP velocity as 1st derivative of time-dependent polynomial.
//    Note: The polynomial degree/coefficients must be
//    predetermined by the data provider.
//  
//*****************************************************************************
ossimEcefVector ossimSarModel::getArpVel(const ossim_float64& time) const
{
   ossim_int32 i;
   ossim_uint32 nTermsX = (ossim_uint32)theArpXPolCoeff.size();
   ossim_uint32 nTermsY = (ossim_uint32)theArpYPolCoeff.size();
   ossim_uint32 nTermsZ = (ossim_uint32)theArpZPolCoeff.size();
   
   ossim_float64 x = nTermsX * theArpXPolCoeff[nTermsX-1];
   for (i=nTermsX-2; i>=1; i--)
      x = x*time + i*theArpXPolCoeff[i];
      
   ossim_float64 y = nTermsY * theArpYPolCoeff[nTermsY-1];
   for (i=nTermsY-2; i>=1; i--)
      y = y*time + i*theArpYPolCoeff[i];
      
   ossim_float64 z = nTermsZ * theArpZPolCoeff[nTermsZ-1];
   for (i=nTermsZ-2; i>=1; i--)
      z = z*time + i*theArpZPolCoeff[i];
      
   ossimEcefVector vel(x, y, z);
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG getArpVel:"<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << " time = "<<time<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << " vel  = "<<vel<<endl;
   }
   
   return vel;
}


//*****************************************************************************
//  METHOD: ossimSarModel::getArpTime()
//  
//  Return constant ARP time.
//  
//*****************************************************************************
ossim_float64 ossimSarModel::getArpTime() const
{
   return theTimeCoeff[0];
}


//*****************************************************************************
//  METHOD: ossimSarModel::getArpTime()
//  
//  Compute ARP time as function of line/sample-dependent polynomial.
//  Defaults to full 3rd order.
//    Note: The polynomial degree/coefficients must be
//    predetermined by the data provider.  Higher order terms
//    can be zeroed out if necessary.
//  
//*****************************************************************************
ossim_float64 ossimSarModel::getArpTime(const ossimDpt& imgPt) const
{
   ossim_float64 s = imgPt.samp;
   ossim_float64 l = imgPt.line;
  
   ossim_float64 s2 = s*s;
   ossim_float64 s3 = s2*s;
   ossim_float64 l2 = l*l;
   ossim_float64 l3 = l2*l;
      
   // Evaluate the time polynomial
   ossim_float64 imgTime = theTimeCoeff[0];
   imgTime += theTimeCoeff[1]  * l;
   imgTime += theTimeCoeff[2]  * l2;
   imgTime += theTimeCoeff[3]  * l3;
   imgTime += theTimeCoeff[4]  * s;
   imgTime += theTimeCoeff[5]  * s * l;
   imgTime += theTimeCoeff[6]  * s * l2;
   imgTime += theTimeCoeff[7]  * s * l3;
   imgTime += theTimeCoeff[8]  * s2;
   imgTime += theTimeCoeff[9]  * s2 * l;
   imgTime += theTimeCoeff[10] * s2 * l2;
   imgTime += theTimeCoeff[11] * s2 * l3;
   imgTime += theTimeCoeff[12] * s3;
   imgTime += theTimeCoeff[13] * s3 * l;
   imgTime += theTimeCoeff[14] * s3 * l2;
   imgTime += theTimeCoeff[15] * s3 * l3;
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG getArpTime:"<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << " imgPt   = "<<imgPt<<endl;
      ossimNotify(ossimNotifyLevel_DEBUG) << " imgTime = "<<imgTime<<endl;
   }
      

   return imgTime;
}


//*****************************************************************************
//  METHOD: ossimSarModel::computeRangeDoppler()
//  
//  Compute range/Doppler for given ARP state and ECF point.
//    Ref[1], eq. 11-37, 11-38
//  
//*****************************************************************************
bool ossimSarModel::computeRangeDoppler(const ossimEcefPoint& pt,
                                        const ossimEcefPoint& arpPos,
                                        const ossimEcefVector& arpVel,
                                        ossim_float64& range,
                                        ossim_float64& doppler) const
{
   // Compute range
   ossimEcefVector delta = pt - arpPos;
   range = delta.magnitude();
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "DEBUG computeRangeDoppler: range vector..." << endl;
      ossimNotify(ossimNotifyLevel_DEBUG)
      << "delta components: \n"<<delta<<endl;
   }
   
   if (range != 0.0)
   {
      // Compute Doppler
      doppler = arpVel.dot(delta) / range;
      return true;
   }
   
   return false;
}


//*****************************************************************************
//  METHOD: ossimSarModel::computeOPfromImage()
//  
//  Compute Output Plane coordinates from image coordinates.
//    Ref[1], eq. 11-32 -> 11-35
//  
//*****************************************************************************
bool ossimSarModel::computeOPfromImage(const ossimDpt& imgPt,
                                       ossimEcefPoint& opPt) const
{
   ossim_float64 dL = imgPt.line - theOrpCenter.line;
   ossim_float64 dS = imgPt.samp - theOrpCenter.samp;
      
   dL *= thePixelSpacing;
   dS *= thePixelSpacing;
   
   opPt = theOPX*dL + theOPY*dS + theOrpPosition;
   
   return true;
}


//*****************************************************************************
//  METHOD: ossimSarModel::computeImageFromOP()
//  
//  Compute image coordinates from Output Plane coordinates.
//    Ref[1], eq. 11-32 -> 11-35
//  
//*****************************************************************************
bool ossimSarModel::computeImageFromOP(const ossimEcefPoint& opPt, ossimDpt& imgPt) const
{
   ossimEcefVector delta(opPt - theOrpPosition);
   
   ossim_float64 dL = delta.dot(theOPX) / thePixelSpacing;
   ossim_float64 dS = delta.dot(theOPY) / thePixelSpacing;
      
   imgPt.line = theOrpCenter.line + dL;
   imgPt.samp = theOrpCenter.samp + dS;
   
   return true;
}


//*****************************************************************************
//  METHOD: ossimSarModel::projOPtoSurface()
//  
//  Project Output Plane coordinates to ellipsoid at reference height.
//  
//*****************************************************************************
bool ossimSarModel::projOPtoSurface(const ossimEcefPoint& opPt,
                                    const ossim_float64& range,
                                    const ossim_float64& doppler,
                                    const ossimEcefPoint& arpPos,
                                    const ossimEcefVector& arpVel,
                                    const ossimHgtRef* hgtRef,
                                    ossimEcefPoint& ellPt) const
{
   // Set slopes for tangent plane
   ossim_float64 sx  = 0.0;
   ossim_float64 sy  = 0.0;
   
   // Set tangent plane normal vector in ENU
   ossimColumnVector3d tpn(sx, sy, 1.0);
   ossimColumnVector3d tpnn(-sx, -sy, 1.0);

   // Initialize at OP point
   ossimEcefPoint rg(opPt);
   
   // Matrices
   NEWMAT::SymmetricMatrix BtB(3);
   NEWMAT::ColumnVector BtF(3);
   NEWMAT::ColumnVector F(3);
   NEWMAT::ColumnVector dR(3);
   
   // Initialize criteria
   F(1)=theOipr;
   
   ossim_int32 iter = 0;
   
   while ((F(1)>=theOipr || F(2)>=0.0003048 || F(3)>=0.5) && iter<5)
   {
      // Compute current latitude/longitude estimate
      ossimGpt pg(rg);

      // Set reference point @ desired elevation
      ossim_float64 atHgt = hgtRef->getRefHeight(pg);
      pg.height(atHgt);
      ossimEcefPoint rt(pg);

      // Define ENU space at reference point
      ossimLsrSpace enu(pg);

      // Rotate normal vector to ECF
      ossimEcefVector st = enu.lsrToEcefRotMatrix()*tpn;

      // Compute current range & Doppler estimate
      ossim_float64 rngComp;
      ossim_float64 dopComp;
      computeRangeDoppler(rg, arpPos, arpVel, rngComp, dopComp);
      
      // Compute current height estimate
      ossim_float64 diffHgt = st.dot(rg-rt);

      // Compute current fr, fd, ft
      F(1) = rngComp - range;
      F(2) = dopComp - doppler;
      F(3) = diffHgt;
   
      // Compute fr partials
      ossimEcefVector delta = rg - arpPos;
      ossimEcefVector deltaUv = delta.unitVector();
      ossimEcefVector p_fr = -deltaUv;

      // Compute fd partials
      ossim_float64 vDotr = arpVel.dot(deltaUv);
      ossimEcefVector p_fd = (arpVel - deltaUv*vDotr)/rngComp;

      // Compute ft partials
      ossimColumnVector3d p_ft = enu.lsrToEcefRotMatrix()*tpnn;

      // Form B-matrix
      NEWMAT::Matrix B = ossimMatrix3x3::create(p_fr[0], p_fr[1], p_fr[2],
                                                p_fd[0], p_fd[1], p_fd[2],
                                                p_ft[0], p_ft[1], p_ft[2]);

      // Form coefficient matrix & discrepancy vector
      BtF << B.t()*F;
      BtB << B.t()*B;

      // Solve system
      dR = solveLeastSquares(BtB, BtF);

      // Update estimate
      for (ossim_int32 k=0; k<3; k++)
         rg[k] -= dR(k+1);
      
      iter++;
   }
   
   // Set intersection for return
   ellPt = rg;

   return true;
}


//*****************************************************************************
//  METHOD: ossimSarModel::projEllipsoidToOP()
//  
//  Projects ellipsoid coordinates to Output Plane.
//  
//*****************************************************************************
bool ossimSarModel::projEllipsoidToOP(const ossimEcefPoint& ellPt,
                                      ossimEcefPoint& opPt) const
{
   ossimDpt currentImagePt;
   ossimEcefPoint selPt;
   
   // Initialize at ORP image point
   currentImagePt = theOrpCenter;
   
   ossim_float64 checkTime = 1.0;
   ossim_float64 lTime=10000.0;
   ossim_int32 iter = 0;
   
   
   // Iterate on imaging time
   while (checkTime > 10.0e-6 && iter<5)
   {
   
      // Get current time estimate
      ossim_float64 cTime = getArpTime(currentImagePt);
      checkTime = fabs(cTime-lTime)/cTime;

      // Get state vector at current time estimate
      ossimEcefPoint  arpPos = getArpPos(cTime);
      ossimEcefVector arpVel = getArpVel(cTime);
      ossimEcefVector arpVelunit = arpVel.unitVector();

      // Define planes
      ossim_float64 constOP = theOPZ.dot(
         ossimEcefVector(theOrpPosition[0],theOrpPosition[1],theOrpPosition[2]));
      ossim_float64 constRD = arpVelunit.dot(
         ossimEcefVector(ellPt[0],ellPt[1],ellPt[2]));

      // Compute dot product of normals
      ossim_float64 ndot = theOPZ.dot(arpVelunit);

      // Compute line of intersection of OP and RD planes
      ossim_float64 det = 1.0 - ndot*ndot;
      ossim_float64 c1 = (constOP - ndot*constRD) / det;
      ossim_float64 c2 = (constRD - ndot*constOP) / det;
      ossimEcefPoint ori = (theOPZ*c1+arpVelunit*c2)+ossimEcefPoint(0.0,0.0,0.0);
      ossimEcefVector dir = theOPZ.cross(arpVelunit);
   
      // Solve for intersection points (RD circle and OP)
      ossimEcefVector delta = ori - arpPos;
      ossim_float64 a2 = dir.length()*dir.length();
      ossim_float64 a1 = delta.dot(dir);
      ossimEcefVector rng(ellPt-arpPos);
      ossim_float64 a0 = delta.length()*delta.length()-rng.length()*rng.length();

      ossim_float64 rootSqr = a1*a1-a0*a2;
      ossim_float64 root = sqrt(rootSqr);
      ossimEcefPoint p1 = ori - dir*((a1+root)/a2);
      ossimEcefPoint p2 = ori - dir*((a1-root)/a2);

      ossimEcefVector p1Delta = p1 - ellPt;
      ossimEcefVector p2Delta = p2 - ellPt;

      // Pick correct point
      if (p1Delta.magnitude()<p2Delta.magnitude())
         selPt = p1;
      else
         selPt = p2;

      // Compute OP image coordinates
      computeImageFromOP(selPt,currentImagePt);

      lTime = cTime;
      iter++;
   }
      
   opPt = selPt;
   
   return true;
}


//*****************************************************************************
//  METHOD: ossimSarModel::getForwardDeriv()
//  
//  Compute partials of range/Doppler WRT to ground.
//  
//*****************************************************************************
ossimDpt ossimSarModel::getForwardDeriv(int derivMode,
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
      //    [1] compute r/D corresponding to (s,l)
      //    [2] compute time, ARP posvel and save 
      //  Note: In this mode, pos is used to pass
      //  in the (s,l) observations.
      //******************************************
      if (derivMode==OBS_INIT)
      {
         // Compute the r/D observations from image coordinates
         ossimDpt obs;
         obs.samp = pos.latd();
         obs.line = pos.lond();

         theObsTime = getArpTime(obs);
         theObsArpPos = getArpPos(theObsTime);
         theObsArpVel = getArpVel(theObsTime);
         theObsArpVel_U = theObsArpVel.unitVector();
         theObsArpVel_Mag = theObsArpVel.magnitude();

         computeOPfromImage(obs, theObsOP);
         computeRangeDoppler
            (theObsOP, theObsArpPos, theObsArpVel, theObsRng, theObsDop);
         ossimDpt obsRD(theObsRng, theObsDop);
         theObs = obsRD;
      }

      //******************************************
      // EVALUATE mode
      //   [1] evaluate & save partials, residuals
      //   [2] return residuals
      //******************************************
      else if (derivMode==EVALUATE)
      {

         // Compute the partials
         ossimEcefPoint gpos(pos);
         ossimEcefPoint ellObs = gpos;
         theObsPosition = gpos;

         // Partials of range WRT ground
         ossimEcefVector rng = ellObs - theObsArpPos;
         ossimEcefVector rngU = rng.unitVector();

         // Partials of azimuth WRT ground
         ossimEcefVector slantPerp = rngU.cross(theObsArpVel);
         ossimEcefVector rs(theObsArpPos[0],theObsArpPos[1],theObsArpPos[2]);
         ossim_float64 signPar = (slantPerp.dot(rs)<0.0) ? -1:1;
         ossimEcefVector slantNormal = slantPerp.unitVector() * signPar;
         ossimEcefVector azU = slantNormal.unitVector().cross(rngU);

         // Partials of Doppler WRT azimuth
         ossim_float64 dca = acos(theObsArpVel_U.dot(rngU));
         signPar = ((theObsArpVel.cross(rng)).dot(slantNormal)<0.0) ? 1:-1;
         theParDopWRTaz = signPar*sin(dca)*theObsArpVel_Mag/rng.magnitude();

         // Save the partials
         //  Range
         theParWRTx.u = rngU.x();
         theParWRTy.u = rngU.y();
         theParWRTz.u = rngU.z();
         //  Doppler
         theParWRTx.v = azU.x()*theParDopWRTaz;
         theParWRTy.v = azU.y()*theParDopWRTaz;
         theParWRTz.v = azU.z()*theParDopWRTaz;

         // Residuals
         ossim_float64 cRng;
         ossim_float64 cDop;
         ossimEcefPoint opPt;
         projEllipsoidToOP(ellObs, opPt);
         computeRangeDoppler(opPt, theObsArpPos, theObsArpVel, cRng, cDop);
         ossimDpt resid(theObsRng-cRng, theObsDop-cDop);
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


//*****************************************************************************
//  METHOD: ossimSarModel::setAcquisitionMode()
//  
//  Set internal acquisition mode parameter.
//  
//*****************************************************************************
void ossimSarModel::setAcquisitionMode(const ossimString& mode)
{
   ossimString os = mode;
   os.downcase();
   if (os == "scan")
   {
      theAcquisitionMode = ossimSarModel::SCAN;
   }
   else if (os == "spot")
   {
      theAcquisitionMode = ossimSarModel::SPOT;
   }
   else
   {
      theAcquisitionMode = ossimSarModel::UNKNOWN;
   }
}
