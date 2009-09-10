//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossimTerraSarModel.h>
#include <ossimPluginCommon.h>
#include <ossimTerraSarProductDoc.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>
#include <otb/GMSTDateTime.h>
#include <otb/PlatformPosition.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <otb/SarSensor.h>

#include <cmath>

// Keyword constants:
static const char NUMBER_SRGR_COEFFICIENTS_KW[] = "sr_gr_coeffs_count";
static const char LOAD_FROM_PRODUCT_FILE_KW[] = "load_from_product_file_flag";
static const char PRODUCT_XML_FILE_KW[] = "product_xml_filename";
static const char SR_GR_R0_KW[] = "sr_gr_r0";
static const char SC_RT_KW[] = "sc_rt";
static const char SR_GR_SF_KW[] = "sc_gr_sf";
static const char ALT_SR_GR_COEFFICIENT0_KW[] = "alt_sr_gr_coeff0";
static const char ALT_SR_GR_COEFFICIENT1_KW[] = "alt_sr_gr_coeff1";
static const char ALT_SR_GR_COEFFICIENT2_KW[] = "alt_sr_gr_coeff2";

using ::ossimString;
using ::ossimXmlDocument;

// Static trace for debugging
static ossimTrace traceDebug("ossimTerraSarModel:debug");

namespace ossimplugins
{
RTTI_DEF1(ossimTerraSarModel,
          "ossimTerraSarModel",
          ossimGeometricSarSensorModel);
}

ossimplugins::ossimTerraSarModel::ossimTerraSarModel()
   : ossimGeometricSarSensorModel(),
     _SrToGr_R0(0.0),
     _SrToGr_exponent(),
     _SrToGr_coeffs(),
     _sceneCenterRangeTime(0.0),
     _SrToGr_scaling_factor(0.0),
     _alt_srgr_coefset(3),
     theProductXmlFile()
{
}

ossimplugins::ossimTerraSarModel::ossimTerraSarModel(
   const ossimTerraSarModel& rhs)
   : ossimGeometricSarSensorModel(rhs),
     _SrToGr_R0(rhs._SrToGr_R0),
     _SrToGr_exponent(rhs._SrToGr_exponent),
     _SrToGr_coeffs(rhs._SrToGr_coeffs),
     _sceneCenterRangeTime(rhs._sceneCenterRangeTime),
     _SrToGr_scaling_factor(rhs._SrToGr_scaling_factor),
     _alt_srgr_coefset(rhs._alt_srgr_coefset),
     theProductXmlFile(rhs.theProductXmlFile)
{
}

ossimplugins::ossimTerraSarModel::~ossimTerraSarModel()
{
}

ossimString ossimplugins::ossimTerraSarModel::getClassName() const
{
   return ossimString("ossimTerraSarModel");
}

ossimObject* ossimplugins::ossimTerraSarModel::dup() const
{
   return new ossimTerraSarModel(*this);   
}

// Note : ground range to slant range coputation could be performed in three ways : 
//    (1) Slant Range to Ground Range polynomial inversion (coefficients given at mid-scene)
//    (2) use of a parabolic model from three geolocated points
//    (3) interpolation from the geolocation grid (separate file, most precise technique) 
// In this version, (1) and (2) were implemented but (1) is imprecise on the test products. 
double ossimplugins::ossimTerraSarModel::getSlantRangeFromGeoreferenced(double col) const
{
   const double c =  2.99792458e+8;
   double tn = _alt_srgr_coefset[0] + _alt_srgr_coefset[1] * col + _alt_srgr_coefset[2] * col*col ;
   return tn * (c/2.0);
}

bool ossimplugins::ossimTerraSarModel::open(const ossimFilename& file)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::open";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n"
         << "file: " << file << "\n";
   }

   bool result = false;

   if ( file.exists() && (file.ext().downcase() == "xml") )
   {
      //---
      // Instantiate the XML parser:
      //---
      ossimXmlDocument* xdoc = new ossimXmlDocument();
      if ( xdoc->openFile(file) )
      {
         ossimTerraSarProductDoc tsDoc;

         result = tsDoc.isTerraSarX(xdoc);
        
         if (result)
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "isTerraSarX...\n";
            }

            // Set the base class number of lines and samples
            result = tsDoc.initImageSize(xdoc, theImageSize);
            
            if (result)
            {
               // Set the base class clip rect.
               theImageClipRect = ossimDrect(
                  0, 0,
                  theImageSize.x-1, theImageSize.y-1);
            }
               
            // Set the sub image offset. tmp hard coded (drb).
            theSubImageOffset.x = 0.0;
            theSubImageOffset.y = 0.0;
            
            // Set the image ID to the scene ID.
            if (result)
            {
               result = tsDoc.getSceneId(xdoc, theImageID);
            }

            // Set the sensor ID to the mission ID.
            if (result)
            {
               result = tsDoc.getMission(xdoc, theSensorID);
            }

            // Set the base class gsd:
            result = tsDoc.initGsd(xdoc, theGSD);
            if (result)
            {
               theMeanGSD = (theGSD.x + theGSD.y)/2.0;
            }

            if (result)
            {
               /*result = */initSRGR(xdoc, tsDoc);

               if (result)
               {
                  result = initPlatformPosition(xdoc, tsDoc);

                  if (result)
                  {
                     result = initSensorParams(xdoc, tsDoc);

                     if (result)
                     {
                        result = initRefPoint(xdoc, tsDoc);
                     }
                  }
               }
            }
         }
         
      } // matches: if ( xdoc->openFile(file) )
      
      delete xdoc;
      xdoc = 0;
      
   } // matches: if ( file.exists() )

   if (result)
   {
      theProductXmlFile = file;
      
      // Assign the ossimSensorModel::theBoundGndPolygon
      ossimGpt ul;
      ossimGpt ur;
      ossimGpt lr;
      ossimGpt ll;
      lineSampleToWorld(theImageClipRect.ul(), ul);
      lineSampleToWorld(theImageClipRect.ur(), ur);
      lineSampleToWorld(theImageClipRect.lr(), lr);
      lineSampleToWorld(theImageClipRect.ll(), ll);
      setGroundRect(ul, ur, lr, ll);  // ossimSensorModel method.
   }
   else
   {
      theProductXmlFile = ossimFilename::NIL;
   }

   if (traceDebug())
   {
      this->print(ossimNotify(ossimNotifyLevel_DEBUG));
            
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimplugins::ossimTerraSarModel::saveState(ossimKeywordlist& kwl,
                                                 const char* prefix) const
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::saveState";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }

   bool result = false;

   if ( (_alt_srgr_coefset.size() == 3) &&
        ( _SrToGr_exponent.size() == _SrToGr_coeffs.size()) )
   {
      // Save our state:
      kwl.add(prefix, SR_GR_R0_KW, _SrToGr_R0);
      
      ossimString kw1 = "sr_gr_exponent_";
      ossimString kw2 = "sr_gr_coeffs_";

      const ossim_uint32 COUNT = _SrToGr_exponent.size();

      kwl.add(prefix, NUMBER_SRGR_COEFFICIENTS_KW, COUNT);
      
      for(ossim_uint32 i = 0; i < COUNT; ++i)
      {
         ossimString iStr = ossimString::toString(i);
         ossimString kw = kw1;
         kw += iStr;
         kwl.add(prefix, kw, _SrToGr_exponent[i]);
         kw = kw2;
         kw += iStr;
         kwl.add(prefix, kw, _SrToGr_coeffs[i]);
      }
      kwl.add(prefix, SC_RT_KW, _sceneCenterRangeTime);
      kwl.add(prefix, SR_GR_SF_KW, _SrToGr_scaling_factor);
      kwl.add(prefix, ALT_SR_GR_COEFFICIENT0_KW,  _alt_srgr_coefset[0]);
      kwl.add(prefix, ALT_SR_GR_COEFFICIENT1_KW,  _alt_srgr_coefset[1]);
      kwl.add(prefix, ALT_SR_GR_COEFFICIENT2_KW,  _alt_srgr_coefset[2]);
      kwl.add(prefix, PRODUCT_XML_FILE_KW, theProductXmlFile.c_str());   

      // Call base save state:
      result = ossimGeometricSarSensorModel::saveState(kwl, prefix);
   }

   //---
   // Uncomment to force load from product file instead of loadState.
   //---
   if (result)
   {
      // kwl.add(prefix, LOAD_FROM_PRODUCT_FILE_KW, "true");
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }
   
   return result;
}

bool ossimplugins::ossimTerraSarModel::loadState (const ossimKeywordlist &kwl,
                                                  const char *prefix)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::loadState";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   const char* lookup = 0;
   ossimString s;

   // Check the type first.
   lookup = kwl.find(prefix, ossimKeywordNames::TYPE_KW);
   if (lookup)
   {
      s = lookup;
      if (s != getClassName())
      {
         return false;
      }
   }

   // Get the product.xml file name.
   lookup = kwl.find(prefix, PRODUCT_XML_FILE_KW);
   if (lookup)
   {
      theProductXmlFile = lookup;

      // See if caller wants to load from xml vice keyword list.
      lookup = kwl.find(prefix, LOAD_FROM_PRODUCT_FILE_KW);
      if (lookup)
      {
         s = lookup;
         if ( s.toBool() )
         {
            // Loading from product.xml file.
            return open(theProductXmlFile);
         }
      }
   }
   
   // Load the base class.
   bool result = ossimGeometricSarSensorModel::loadState(kwl, prefix);

   //---
   // Temp:  This must be cleared or you end up with a bounding rect of all
   // zero's.
   //---
   theBoundGndPolygon.clear();

   if (result)
   {
      lookup = kwl.find(prefix,SR_GR_R0_KW);
      if (lookup)
      {
         s = lookup;
         _SrToGr_R0 = s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               <<  SR_GR_R0_KW << "\n";
         } 
         result = false;
      }
      
      ossim_uint32 count = 0;      
      lookup = kwl.find(prefix, NUMBER_SRGR_COEFFICIENTS_KW);
      if (lookup)
      {
         s = lookup;
         count = s.toUInt32();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << NUMBER_SRGR_COEFFICIENTS_KW << "\n";
         } 
         result = false;
      }
      
      if (result && count)
      {
         _SrToGr_exponent.resize(count);
         _SrToGr_coeffs.resize(count);

         ossimString kw1 = "sr_gr_exponent_";
         ossimString kw2 = "sr_gr_coeffs_";
          
         for(ossim_uint32 i = 0; i < count; ++i)
         {
            ossimString kw;
            ossimString iStr = ossimString::toString(i);
            
            // sr_gr_exponents
            kw = kw1;
            kw += iStr;
            
            lookup = kwl.find(prefix, kw);
            if (lookup)
            {
               s = lookup;
               _SrToGr_exponent[i] = s.toInt();
            }
            else
            {
               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE
                     << "\nRequired keyword not found: "
                     << kw << "\n";
               } 
               result = false;
            }
            
            // _SrToGr_coeffs
            kw = kw2;
            kw += iStr;
            lookup = kwl.find(prefix, kw);
            if (lookup)
            {
               s = lookup;
               _SrToGr_coeffs[i] = s.toDouble();
            }
            else
            {
               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_WARN)
                     << MODULE
                     << "\nRequired keyword not found: "
                     << kw << "\n";
               } 
               result = false;
            }
         }

      } // matches:  if (result && count)
      else
      {
         result = false;
      }

      lookup = kwl.find(prefix, SC_RT_KW);
      if (lookup)
      {
         s = lookup;
         _sceneCenterRangeTime = s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << SC_RT_KW << "\n";
         } 
         result = false;
      }

      lookup = kwl.find(prefix, SR_GR_SF_KW);
      if (lookup)
      {
         s = lookup;
         _SrToGr_scaling_factor = s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << SR_GR_SF_KW << "\n";
         } 
         result = false;
      }
      
      lookup = kwl.find(prefix, ALT_SR_GR_COEFFICIENT0_KW);
      if (lookup)
      {
         s = lookup;
         _alt_srgr_coefset[0] = s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << ALT_SR_GR_COEFFICIENT0_KW << "\n";
         } 
         result = false;
      }
      lookup = kwl.find(prefix, ALT_SR_GR_COEFFICIENT1_KW);
      if (lookup)
      {
         s = lookup;
         _alt_srgr_coefset[1] = s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << ALT_SR_GR_COEFFICIENT1_KW << "\n";
         } 
         result = false;
      }
      lookup = kwl.find(prefix, ALT_SR_GR_COEFFICIENT2_KW);
      if (lookup)
      {
         s = lookup;
         _alt_srgr_coefset[2] = s.toDouble();
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << ALT_SR_GR_COEFFICIENT2_KW << "\n";
         } 
         result = false;
      }

      lookup = kwl.find(prefix, PRODUCT_XML_FILE_KW);
      if (lookup)
      {
         theProductXmlFile = lookup;
      }
      else
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\nRequired keyword not found: "
               << PRODUCT_XML_FILE_KW << "\n";
         } 
         result = false;
      }
      
   } // matches: if (result)

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }
   
   return result;
}

std::ostream& ossimplugins::ossimTerraSarModel::print(std::ostream& out) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();
   
   out << setprecision(15) << setiosflags(ios::fixed)
       << "\nossimTerraSarModelclass data members:\n"
       << SR_GR_R0_KW << _SrToGr_R0 << "\n";

   ossim_uint32 i;
   
   ossimString kw1 = "sr_gr_exponent_";
   for(i = 0; i < _SrToGr_exponent.size(); ++i)
   {
      ossimString iStr = ossimString::toString(i);
      ossimString kw = kw1;
      kw += iStr;
      kw += ": ";
      out << kw << _SrToGr_exponent[i] << "\n";
   }

   kw1 = "sr_gr_coeffs_";
   for(i = 0; i < _SrToGr_coeffs.size(); ++i)
   {
      ossimString iStr = ossimString::toString(i);
      ossimString kw = kw1;
      kw += iStr;
      kw += ": ";
      out << kw << _SrToGr_coeffs[i] << "\n";
   }   

   out << SC_RT_KW << ": " << _sceneCenterRangeTime << "\n"
       << SR_GR_SF_KW << ": " << _SrToGr_scaling_factor << "\n"
       << ALT_SR_GR_COEFFICIENT0_KW << ": " << _alt_srgr_coefset[0] << "\n"
       << ALT_SR_GR_COEFFICIENT1_KW << ": " <<_alt_srgr_coefset[1] << "\n"
       << ALT_SR_GR_COEFFICIENT2_KW << ": " <<_alt_srgr_coefset[2] << "\n"
       << PRODUCT_XML_FILE_KW << ": " << theProductXmlFile.c_str() << "\n";
   
   ossimGeometricSarSensorModel::print(out);
   
   // Reset flags.
   out.setf(f);

   return out;
}

/* 
//  Version (1)
double ossimplugins::ossimTerraSarModel::getSlantRangeFromGeoreferenced(double col) const
{
  // iterative polynomial inversion
  const double CLUM        = 2.99792458e+8 ;
  double EPSILON = 0.0000001 ;
  double iterError = 1.0 ; 
  int maxIter = 50, nIter=0 ;
  double estimatedGroundRange, estimatedSlantRangeTime, actualGroundRange, estimatedSlantRange ; 

  
  // actual ground range computation relative to the image near side
  // in the case of Georeferenced images, _refPoint->get_distance() contains the ground range
  actualGroundRange = _refPoint->get_distance() - _sensor->get_col_direction() * (col-_refPoint->get_pix_col()) * _SrToGr_scaling_factor ; 

  estimatedSlantRangeTime = _sceneCenterRangeTime ;
  while ((fabs(iterError)>EPSILON)&& (nIter<maxIter)) {
    // estimated ground range computation from SrToGr
    estimatedGroundRange = 0.0 ; 
    for (int i=0; i<_SrToGr_coeffs.size(); i++) {
      estimatedGroundRange += _SrToGr_coeffs[i]*pow(estimatedSlantRangeTime-_SrToGr_R0,_SrToGr_exponent[i]);
    }
    
    // comparison between the estimated ground range and the actual ground range
    iterError = actualGroundRange - estimatedGroundRange ;

    // estimated slant range update
    estimatedSlantRangeTime += iterError * 2.0 / CLUM ; 

    nIter++;
  }

  estimatedSlantRange = estimatedSlantRangeTime* CLUM / 2.0 ;

  return  estimatedSlantRange  ;
}
*/

bool ossimplugins::ossimTerraSarModel::InitSensorParams(
   const ossimKeywordlist &kwl, const char *prefix)
{
   const char* central_freq_str = kwl.find(prefix,"central_freq");
   double central_freq = atof(central_freq_str);
   const char* fr_str = kwl.find(prefix,"fr");
   double fr = atof(fr_str);
   const char* fa_str = kwl.find(prefix,"fa");
   double fa = atof(fa_str);
   
   //number of different looks 
   // const char* n_azilok_str = kwl.find(prefix,"n_azilok");
   // double n_azilok = atof(n_azilok_str);
   const char* n_rnglok_str = kwl.find(prefix,"n_rnglok");
   double n_rnglok = atof(n_rnglok_str);
   
   //ellipsoid parameters
   const char* ellip_maj_str = kwl.find(prefix,"ellip_maj");
   double ellip_maj = atof(ellip_maj_str) * 1000.0;  // km -> m
   const char* ellip_min_str = kwl.find(prefix,"ellip_min");
   double ellip_min = atof(ellip_min_str) * 1000.0;  // km -> m
   
   
   if(_sensor != NULL)
   {
      delete _sensor;
   }
   
   _sensor = new SensorParams();
   
   
   if (_isProductGeoreferenced)
   {
      const char* orbitDirection_str = kwl.find(prefix,"orbitDirection");
      std::string orbitDirection(orbitDirection_str) ;
      int orbitDirectionSign ; 
      if (orbitDirection=="DESCENDING") orbitDirectionSign = 1 ;
      else orbitDirectionSign = - 1 ;
      
      const char* imageDataStartWith_str = kwl.find(prefix,"imageDataStartWith");
      std::string imageDataStartWith(imageDataStartWith_str) ;
      if (imageDataStartWith=="EARLYAZNEARRG") {
         _sensor->set_col_direction(orbitDirectionSign); 
         _sensor->set_lin_direction(orbitDirectionSign);
      } else if (imageDataStartWith=="EARLYAZFARRG") {
         _sensor->set_col_direction(-orbitDirectionSign);
         _sensor->set_lin_direction(orbitDirectionSign);
      } else if (imageDataStartWith=="LATEAZNEARRG") {
         _sensor->set_col_direction(orbitDirectionSign);
         _sensor->set_lin_direction(-orbitDirectionSign);
      } else if (imageDataStartWith=="LATEAZFARRG") {
         _sensor->set_col_direction(-orbitDirectionSign);
         _sensor->set_lin_direction(-orbitDirectionSign);
      } else {
         // COSAR Files are stored starting with early azimuth, near range
         _sensor->set_col_direction(orbitDirectionSign);
         _sensor->set_lin_direction(orbitDirectionSign);
      }
   }
   else
   {
      _sensor->set_col_direction(1);
      _sensor->set_lin_direction(1);
   }
   
   const char* lookDirection_str = kwl.find(prefix,"lookDirection");
   std::string lookDirection(lookDirection_str) ;
   if ((lookDirection == "Right")||(lookDirection == "RIGHT")) _sensor->set_sightDirection(SensorParams::Right) ;
   else _sensor->set_sightDirection(SensorParams::Left) ;
   
   _sensor->set_sf(fr);
   const double CLUM        = 2.99792458e+8 ;
   double wave_length = CLUM / central_freq ;
   _sensor->set_rwl(wave_length);
   _sensor->set_nRangeLook(n_rnglok);
   _sensor->set_prf(fa) ; 
   // fa is the processing PRF 
   //_sensor->set_prf(fa * n_azilok); // number of looks disabled
   
   _sensor->set_semiMajorAxis(ellip_maj) ; 
   _sensor->set_semiMinorAxis(ellip_min) ; 
   
   return true;
}

bool ossimplugins::ossimTerraSarModel::InitPlatformPosition(
   const ossimKeywordlist &kwl, const char *prefix)
{
   /*
    * Retrieval of ephemerisis number
    */
   const char* neph_str = kwl.find(prefix,"neph");
   int neph = atoi(neph_str);
   
   Ephemeris** ephemeris = new Ephemeris*[neph];
   
   /* 
    * Retrieval of ephemerisis
    */
   for (int i=0;i<neph;i++)
   {
      double pos[3];
      double vit[3];
      char name[64];
      
      
      sprintf(name,"eph%i_date",i);
      const char* date_str = kwl.find(prefix,name);
      
      sprintf(name,"eph%i_posX",i);
      const char* px_str = kwl.find(prefix,name);
      pos[0] = atof(px_str);
      
      sprintf(name,"eph%i_posY",i);
      const char* py_str = kwl.find(prefix,name);
      pos[1] = atof(py_str);
      
      sprintf(name,"eph%i_posZ",i);
      const char* pz_str = kwl.find(prefix,name);
      pos[2] = atof(pz_str);
      
      
      sprintf(name,"eph%i_velX",i);
      const char* vx_str = kwl.find(prefix,name);
      vit[0] = atof(vx_str) ;
      
      sprintf(name,"eph%i_velY",i);
      const char* vy_str = kwl.find(prefix,name);
      vit[1] = atof(vy_str) ;
      
      sprintf(name,"eph%i_velZ",i);
      const char* vz_str = kwl.find(prefix,name);
      vit[2] = atof(vz_str) ;

      /*
       * Conversion to JSD Date
       */
      std::string utcString(date_str);
      CivilDateTime eph_civil_date;
      if (! ossim::iso8601TimeStringToCivilDate(utcString, eph_civil_date)) return false;

      JSDDateTime eph_jsd_date(eph_civil_date);

      GeographicEphemeris* eph = new GeographicEphemeris(eph_jsd_date,pos,vit);

      ephemeris[i] = eph;
   }
  
   /*
    * Creation of the platform position interpolator
    */
   if (_platformPosition != NULL)
   {
      delete _platformPosition;
   }
   _platformPosition = new PlatformPosition(ephemeris,neph);

   /*
    * Free of memory used by ephemerisis list : the constructor copies the ephemerisis
    */
   for (int i=0;i<neph;i++)
   {
      delete ephemeris[i];
   }
   delete[] ephemeris;

   return true;
}

bool ossimplugins::ossimTerraSarModel::InitRefPoint(const ossimKeywordlist &kwl, const char *prefix)
{
   const char* sc_lin_str = kwl.find(prefix,"sc_lin");
   double sc_lin = atof(sc_lin_str);

   const char* sc_pix_str = kwl.find(prefix,"sc_pix");
   double sc_pix = atof(sc_pix_str);

   const char* inp_sctim_str = kwl.find(prefix,"inp_sctim");
   std::string inp_sctim_string(inp_sctim_str) ;

   const char* sceneCenterRangeTime_str = kwl.find(prefix,"sc_rng");
   _sceneCenterRangeTime = atof(sceneCenterRangeTime_str);

   if(_refPoint == NULL)
   {
      _refPoint = new RefPoint();
   }

   _refPoint->set_pix_col(sc_pix);
   _refPoint->set_pix_line(sc_lin);

   CivilDateTime * date = new CivilDateTime() ;
   if (! ossim::iso8601TimeStringToCivilDate(inp_sctim_string, *date)) return false ;

   if(_platformPosition != NULL)
   {
      Ephemeris * ephemeris = _platformPosition->Interpolate((JSDDateTime)*date);
      if (ephemeris == NULL) return false ; 

      _refPoint->set_ephemeris(ephemeris);

      delete ephemeris;
   }
   else
   {
      return false;
   }

   const double CLUM        = 2.99792458e+8 ;
   double sceneCenterSlantRange = _sceneCenterRangeTime * CLUM / 2.0 ;

   _refPoint->set_distance(sceneCenterSlantRange);

   // in the case of Georeferenced images, the ground range is stored in place of the slant range
   // (used for SlantRange computation relative to reference point, necessary for optimization)
   if (_isProductGeoreferenced) {
      double estimatedGroundRange = 0.0 ; 
      for (int i=0; i<static_cast<int>(_SrToGr_coeffs.size()); i++)
      {
         estimatedGroundRange += _SrToGr_coeffs[i]*pow(_sceneCenterRangeTime-_SrToGr_R0,_SrToGr_exponent[i]);
      }
      // SrToGr update : estimatedGroundRange  = sc_pix * scaling_factor
      _SrToGr_scaling_factor = estimatedGroundRange / sc_pix ;
      _refPoint->set_distance(estimatedGroundRange);
   }

   // in order to use ossimSensorModel::lineSampleToWorld
   const char* nbCol_str = kwl.find(prefix,"nbCol");
   const char* nbLin_str = kwl.find(prefix,"nbLin");
   theImageSize.x      = atoi(nbCol_str);
   theImageSize.y      = atoi(nbLin_str);
   theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);

   if (_isProductGeoreferenced) {
      std::string azimuthStartTime(kwl.find("azimuthStartTime"));
      std::string azimuthStopTime(kwl.find("azimuthStopTime"));
      CivilDateTime * dateStart = new CivilDateTime() ;
      if (! ossim::iso8601TimeStringToCivilDate(azimuthStartTime, *dateStart)) return false ;
      CivilDateTime * dateStop = new CivilDateTime() ;
      if (! ossim::iso8601TimeStringToCivilDate(azimuthStopTime, *dateStop)) return false ;
      double acq_msec_first = (double) dateStart->get_second()+dateStart->get_decimal();
      double acq_msec_last = (double) dateStop->get_second()+dateStop->get_decimal();

      double actualPRF = theImageSize.y/(acq_msec_last-acq_msec_first) ;
      _sensor->set_nAzimuthLook(_sensor->get_prf()/actualPRF); 
   }
   else   
      _sensor->set_nAzimuthLook(1.0);

   // Ground Control Points extracted from the model : scene center and corners
   std::list<ossimGpt> groundGcpCoordinates ; 
   std::list<ossimDpt> imageGcpCoordinates ; 
   char name[64];
   for (int k=0 ; k<5 ; k++) {
      sprintf(name,"cornersCol%i",k);
      const char* i_str = kwl.find(name);
      int i = atoi(i_str);
      sprintf(name,"cornersLin%i",k);
      const char* j_str = kwl.find(name);
      int j = atoi(j_str);
      sprintf(name,"cornersLon%i",k);
      const char* lon_str = kwl.find(name);
      double lon = atof(lon_str);
      sprintf(name,"cornersLat%i",k);
      const char* lat_str = kwl.find(name);
      double lat = atof(lat_str);
      const char* height_str = kwl.find("terrain_h");
      double height = atof(height_str) ;

      ossimDpt imageGCP(i,j);
      ossimGpt groundGCP(lat ,lon , height);
      groundGcpCoordinates.push_back(groundGCP) ; 
      imageGcpCoordinates.push_back(imageGCP) ;
   }

   // Default optimization 
   optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

   return true;
}

bool ossimplugins::ossimTerraSarModel::InitSRGR(const ossimKeywordlist &kwl, const char *prefix)
{
   const char* rangeProjectionType_str = kwl.find(prefix,"rangeProjectionType");
   std::string rangeProjectionType(rangeProjectionType_str);

   _isProductGeoreferenced = (rangeProjectionType=="GROUNDRANGE") ;

   // SRtoGR polynomial reference
   const char* SrToGr_R0_str = kwl.find(prefix,"SrToGr_R0");
   _SrToGr_R0 = atof(SrToGr_R0_str);

   // number of SRtoGR coefficients
   const char* SrToGr_coeffs_number_str = kwl.find(prefix,"SrToGr_coeffs_number");
   int srToGr_coeffs_number = atoi(SrToGr_coeffs_number_str);

   // SRtoGR coefficients and exponents
   char name[64];
   double coeff ; 
   int exponent ; 
   for(int i=0;i<srToGr_coeffs_number;i++)
   {
      sprintf(name,"SrToGr_coeffs_%i",i);
      const char* coeff_str = kwl.find(prefix,name);
      coeff = atof(coeff_str);
      _SrToGr_coeffs.push_back(coeff);
      sprintf(name,"SrToGr_exps_%i",i);
      const char* exp_str = kwl.find(prefix,name);
      exponent = atoi(exp_str);
      _SrToGr_exponent.push_back(exponent);
   }

   // ALTERNATIVE to provided coefficients

   // Range time for first mid and last pixel
   std::string orbitDirection(kwl.find(prefix,"orbitDirection")) ;
   double t1, t2, t3 ; 
   if (orbitDirection=="DESCENDING") {
      t3 = atof(kwl.find("start_rng"));
      t2 = atof(kwl.find("sc_rng"));
      t1 = atof(kwl.find("end_rng"));
   }
   else {
      t1 = atof(kwl.find("start_rng"));
      t2 = atof(kwl.find("sc_rng"));
      t3 = atof(kwl.find("end_rng"));
   }

   // Range pixels numbers corresponding
   // double x1 = 0.0;
   double x2 = atof(kwl.find("sc_pix")); 
   double x3 = 2.0*(x2+1.0) -1.0 ; 

   _alt_srgr_coefset[0] = t1;
   _alt_srgr_coefset[1] = ((t2-t1)/(x2*x2)+(t1-t3)/(x3*x3))/((1.0/x2)-(1.0/x3));
   _alt_srgr_coefset[2] = ((t2-t1)/x2 + (t1-t3)/x3)/(x2-x3);
  
   return true;
}

bool ossimplugins::ossimTerraSarModel::initSRGR(const ossimXmlDocument* xdoc,
                                  const ossimTerraSarProductDoc& tsDoc)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::initSRGR";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }
   
   bool result = true;
   
   ossimString s;

   _isProductGeoreferenced = tsDoc.isProductGeoreferenced(xdoc);

   // SRtoGR polynomial reference
   if ( tsDoc.getReferencePoint(xdoc, s) )
   {
      _SrToGr_R0 = s.toDouble();
   }
   else
   {
      //---
      // This value is only used if product is geo referenced,
      // (projection == GROUNDRANGE) so we won't error out if not.
      //---
      if (_isProductGeoreferenced)
      {
         result = false;
      }
      _SrToGr_R0 = 0.0;
   }

   // number of SRtoGR coefficients
   ossimRefPtr<ossimXmlNode> nodes;
   ossimString path =
      "/level1Product/productSpecific/projectedImageInfo/slantToGroundRangeProjection/coefficient";
   std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
   const ossimString EXP = "exponent";
   xdoc->findNodes(path, xnodes);
   if ( xnodes.size() )
   {
      for (ossim_uint32 i = 0; i < xnodes.size(); ++i)
      {
         if (xnodes[i].valid())
         {
            xnodes[i]->getAttributeValue(s, EXP);
            _SrToGr_exponent.push_back(s.toInt32());
            _SrToGr_coeffs.push_back(xnodes[i]->getText().toDouble());
         }
      }
   }
   else
   {
      result = false;
   }

   // Range time for first mid and last pixel

   if ( tsDoc.getOrbitDirection(xdoc, s) )
   {
      bool desendingFlag = (s == "DESCENDING");
      double startRng = 0.0;
      double endRng   = 0.0;
      double scRng    = 0.0;
      double t1 = 0.0;
      double t2 = 0.0;
      double t3 = 0.0;
      double x2 = 0.0;

      if ( tsDoc.getRangeGateFirstPixel(xdoc, s) )
      {
         startRng = s.toDouble();
      }
      else
      {
         result = false;
      }
      
      if ( tsDoc.getRangeGateLastPixel(xdoc, s) )
      {
         endRng = s.toDouble();
      }
      else
      {
         result = false;
      }
      
      if ( tsDoc.getSceneCenterRangeTime(xdoc, s) )
      {
         scRng = s.toDouble();
      }
      else
      {
         result = false;
      }         
      
      t2 = scRng;
      
      if (desendingFlag)
      {
         t3 = startRng;
         t1 = endRng;
      }
      else
      {
         t1 = startRng;
         t3 = endRng;
      }
      
      // Range pixels numbers corresponding
      if ( tsDoc.getSceneCenterRefColumn(xdoc, s) )
      {
         //---
         // TSX image coordinates start at (1,1),
         // while OSSIM image coordinates start at (0,0)
         //---
         x2 = s.toDouble() - 1.0;
      }
      else
      {
         result = false;
      }
      
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "startRng: " << startRng
            << "\nscRng:    " << scRng
            << "\nendRng:   " << endRng
            << "\nx2:       " << x2
            << "\n";
      }
      
      double x3 = 2.0*(x2+1.0) -1.0 ;
      
      _alt_srgr_coefset[0] = t1;
      _alt_srgr_coefset[1] =
         ((t2-t1)/(x2*x2)+(t1-t3)/(x3*x3))/((1.0/x2)-(1.0/x3));
      _alt_srgr_coefset[2] = ((t2-t1)/x2 + (t1-t3)/x3)/(x2-x3);
   }
   else
   {
      result = false;
   }

   return result;
}
   
bool ossimplugins::ossimTerraSarModel::initPlatformPosition(const ossimXmlDocument* xdoc,
                                              const ossimTerraSarProductDoc& tsDoc)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::initPlatformPosition";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }   
   
   // Initialize the platform position interpolator.
   if (_platformPosition)
   {
      delete _platformPosition;
   }
   
   _platformPosition = new PlatformPosition();
   
   bool result = tsDoc.initPlatformPosition(xdoc, _platformPosition);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimplugins::ossimTerraSarModel::initSensorParams(const ossimXmlDocument* xdoc,
                                          const ossimTerraSarProductDoc& tsDoc)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::initSensorParams";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }
   
   if (_sensor )
   {
      delete _sensor;
   }
   _sensor =  new SensorParams();
   
   bool result = tsDoc.initSensorParams(xdoc, _sensor);

   if (!result)
   {
      delete _sensor;
      _sensor = 0;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimplugins::ossimTerraSarModel::initRefPoint(
   const ossimXmlDocument* xdoc, const ossimTerraSarProductDoc& tsDoc)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::initRefPoint";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }

   if ( !_platformPosition )
   {
      return false;
   }

   bool result = true;

   ossimString s; // used throughout

   //---
   // Set the base class reference points.
   // Note the "ONE BASED" image points from xml doc.
   //---
   
   // Get the center sample.
   if ( tsDoc.getSceneCenterRefColumn(xdoc, s) )
   {
      theRefImgPt.x = s.toFloat64() - 1.0;
   }
   else
   {
      result = false;
   }

   // Get the center line.
   if ( tsDoc.getSceneCenterRefRow(xdoc, s) )
   {
      theRefImgPt.y = s.toFloat64() - 1.0;
   }
   else
   {
      result = false;
   }

   // Get the center latitude.
   if ( tsDoc.getSceneCenterLat(xdoc, s) )
   {
      theRefGndPt.lat = s.toFloat64();
   }
   else
   {
      result = false;
   }

   // Get the center longitude.
   if ( tsDoc.getSceneCenterLon(xdoc, s) )
   {
      theRefGndPt.lon = s.toFloat64();
   }
   else
   {
      result = false;
   }

   // Reference point.
   if (_refPoint)
   {
      delete _refPoint;
   }
   _refPoint = new RefPoint();

   _refPoint->set_pix_col(theRefImgPt.x);
   _refPoint->set_pix_line(theRefImgPt.y);

   if ( tsDoc.getSceneCenterAzimuthTime(xdoc, s) )
   {
      CivilDateTime date;
      if ( ossim::iso8601TimeStringToCivilDate(s, date) )
      {
         JSDDateTime jdate(date);
         Ephemeris * ephemeris = _platformPosition->Interpolate(jdate);
         if (ephemeris)
         {
            _refPoint->set_ephemeris(ephemeris);
            delete ephemeris;
         }
         else
         {
            result = false;
         }
      }
      else
      {
         result = false;
      }
   }
   else
   {
      result = false;
   }

   if ( tsDoc.getSceneCenterRangeTime(xdoc, s) )
   {
      _sceneCenterRangeTime = s.toDouble();
      
      const double CLUM = 2.99792458e+8;
      double sceneCenterSlantRange = _sceneCenterRangeTime * CLUM / 2.0;
      _refPoint->set_distance(sceneCenterSlantRange);
   }
   else
   {
      result = false;
   }

   //---
   // In the case of Georeferenced images, the ground range is stored in
   // place of the slant range
   // (used for SlantRange computation relative to reference point, necessary
   // for optimization)
   //---
   if (_isProductGeoreferenced)
   {
      double estimatedGroundRange = 0.0 ; 
      for (int i=0; i<static_cast<int>(_SrToGr_coeffs.size()); i++)
      {
         estimatedGroundRange += _SrToGr_coeffs[i]*
            pow(_sceneCenterRangeTime - _SrToGr_R0, _SrToGr_exponent[i]);
      }

      // SrToGr update : estimatedGroundRange  = sc_pix * scaling_factor
      _SrToGr_scaling_factor = estimatedGroundRange / theRefImgPt.x;
      _refPoint->set_distance(estimatedGroundRange);

      CivilDateTime dateStart;
      CivilDateTime dateStop;
      
      if (tsDoc.getAzimuthStartTime(xdoc, s) )
      {
         if (! ossim::iso8601TimeStringToCivilDate(s, dateStart) )
         {
            result = false;
         }
      }
      else
      {
         result = false;
      }

      if (tsDoc.getAzimuthStopTime(xdoc, s) )
      {
         if (! ossim::iso8601TimeStringToCivilDate(s, dateStop) )
         {
            result = false;
         }
      }
      else
      {
         result = false;
      }

      double acq_msec_first = (double) dateStart.get_second() +
         dateStart.get_decimal();
      double acq_msec_last = (double) dateStop.get_second() +
         dateStop.get_decimal();
      
      double actualPRF = theImageSize.y/(acq_msec_last - acq_msec_first) ;
      _sensor->set_nAzimuthLook(_sensor->get_prf()/actualPRF); 
      
   }
   else
   {
      _sensor->set_nAzimuthLook(1.0);
   }
   
   // Ground Control Points extracted from the model.
   std::list<ossimGpt> groundGcpCoordinates; 
   std::list<ossimDpt> imageGcpCoordinates;
   if ( tsDoc.initTiePoints(xdoc,
                            groundGcpCoordinates,
                            imageGcpCoordinates) == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << "initTiePoint error! exiting\n";
      }
      return false;
   }

   if (result)
   {
      // Default optimization 
      optimizeModel(groundGcpCoordinates, imageGcpCoordinates);
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = true\n";
   } 

   return true;
}

   
   
