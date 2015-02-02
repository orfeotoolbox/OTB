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
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimNotifyContext.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/support_data/ossimSupportFilesList.h>
#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>
#include <otb/GMSTDateTime.h>
#include <otb/PlatformPosition.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <otb/SarSensor.h>
#include <cmath>
#include <iostream>
#include <sstream>


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
static const char PRODUCT_TYPE[] = "product_type";
static const char RADIOMETRIC_CORRECTION[] = "radiometricCorrection";
static const char AZ_START_TIME[] = "azimuth_start_time";
static const char AZ_STOP_TIME[] = "azimuth_stop_time";
static const char RG_FIRST_TIME[] = "range_first_time";
static const char RG_LAST_TIME[] = "range_last_time";
static const char GENERATION_TIME[] = "generation_time";
static const char ACQUISITION_INFO[] = "acquisitionInfo.";
static const char IMAGING_MODE[] = "imagingMode";
static const char SENSOR[] = "sensor";
static const char LOOK_DIRECTION[] = "lookDirection";
static const char POLARISATION_MODE[] = "polarisationMode";
static const char POLARISATION_LIST[] = "polarisationList";
static const char NUMBER_LAYERS[] = "numberOfLayers";
static const char CALIBRATION_CALFACTOR[] = "calibration.calibrationConstant.calFactor";
static const char RADAR_FREQUENCY[] = "radarFrequency";

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
     _productType(),
     _radiometricCorrection(),
     _imagingMode(),
     _acquisitionSensor(),
     _lookDirection(),
     _polarisationMode(),
     _polLayer("UNDEFINED"),
     _polLayerList(),
     _noise(0),
     _sceneCoord(0),
     _calFactor(0.),
     _radarFrequency(0.),
     _numberOfLayers(0),
     _azStartTime(),
     _azStopTime(),
     _rgFirstPixelTime(),
     _rgLastPixelTime(),
     _generationTime()
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
     _productType(rhs._productType),
     _radiometricCorrection(rhs._radiometricCorrection),
     _imagingMode(rhs._imagingMode),
     _acquisitionSensor(rhs._acquisitionSensor),
     _lookDirection(rhs._lookDirection),
     _polarisationMode(rhs._polarisationMode),
     _polLayer(rhs._polLayer),
     _polLayerList(rhs._polLayerList),
     _noise(rhs._noise),
     _sceneCoord( 0 ),
     _calFactor(rhs._calFactor),
     _radarFrequency(rhs._radarFrequency),
     _numberOfLayers(rhs._numberOfLayers),
     _azStartTime(rhs._azStartTime),
     _azStopTime(rhs._azStopTime),
     _rgFirstPixelTime(rhs._rgFirstPixelTime),
     _rgLastPixelTime(rhs._rgLastPixelTime),
     _generationTime(rhs._generationTime)
{
   if ( rhs._sceneCoord )
   {
      _sceneCoord = new SceneCoord( *(rhs._sceneCoord) );
   }  
}

ossimplugins::ossimTerraSarModel::~ossimTerraSarModel()
{
   _noise.clear();
   
/*
 *    if (_noise != 0)
 {
 delete _noise;
 }
*/
   if (_sceneCoord)
   {
      delete _sceneCoord;
      _sceneCoord = 0;
   }
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
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::open() -- ";
  
   _imageFilename = file.expand();
   _productXmlFile = ossimFilename::NIL;
   ossimFilename xmlfile;
   bool foundMetadataFile = findTSXLeader(file, xmlfile);
   while (foundMetadataFile) // use of while allows use of "break"
   {
      //---
      // Instantiate the XML parser:
      //---
      ossimRefPtr<ossimXmlDocument> xdoc = new ossimXmlDocument();
      if ( !xdoc->openFile(xmlfile) ) break;

      ossimTerraSarProductDoc tsDoc;
      if (!tsDoc.isTerraSarX(xdoc.get())) break;

      ossimString s;
      if ( tsDoc.getProjection(xdoc.get(), s) )
      {
         if ( s.upcase() == "MAP" ) break;
      }

      // Set the base class number of lines and samples
      // Set the base class clip rect.
      // Set the sub image offset. tmp hard coded (drb).
      if (!tsDoc.initImageSize(xdoc.get(), theImageSize)) break;
      theImageClipRect = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);
      theSubImageOffset.x = 0.0;
      theSubImageOffset.y = 0.0;

      // Set the image ID to the scene ID.
      if (!tsDoc.getSceneId(xdoc.get(), theImageID)) break;

      // Set the sensor ID to the mission ID.
      if (!tsDoc.getMission(xdoc.get(), theSensorID)) break;

      // Set the base class gsd:
      if (!tsDoc.initGsd(xdoc.get(), theGSD)) break;
      theMeanGSD = (theGSD.x + theGSD.y)/2.0;

      initSRGR(xdoc.get(), tsDoc);

      if (!initPlatformPosition(xdoc.get(), tsDoc)) break;
      if (!initSensorParams(xdoc.get(), tsDoc)) break;
      if (!initRefPoint(xdoc.get(), tsDoc)) break;
      if (!tsDoc.getProductType(xdoc.get(), _productType)) break;
      if (!tsDoc.getRadiometricCorrection(xdoc.get(), _radiometricCorrection)) break;
      if (!initAcquisitionInfo(xdoc.get(), tsDoc)) break;
      if (!initNoise(xdoc.get(), tsDoc)) break;
      if (!getPolLayerFromImageFile(xdoc.get(), file)) break;
      if (!initCalibration(xdoc.get(), tsDoc)) break;

//      replaced by OTB by the initCalibration above
//      if (!tsDoc.getCalFactor(xdoc.get(), s)) break;
//      _calFactor = s.toFloat64();

      if (!tsDoc.getRadarFrequency(xdoc.get(), s)) break;
      _radarFrequency= s.toFloat64();

      if (!tsDoc.getAzimuthStartTime(xdoc.get(), _azStartTime)) break;
      if (!tsDoc.getAzimuthStopTime(xdoc.get(), _azStopTime)) break;
      if (!tsDoc.getRangeFirstPixelTime(xdoc.get(), _rgFirstPixelTime)) break;
      if (!tsDoc.getRangeLastPixelTime(xdoc.get(), _rgLastPixelTime)) break;
      if (!tsDoc.getGenerationTime(xdoc.get(), _generationTime)) break;
//      removed by OTB
//      if (!initIncidenceAngles(xdoc.get(), tsDoc)) break;
      if (!initSceneCoord(xdoc.get(), tsDoc)) break;

      xdoc = 0;

      _productXmlFile = xmlfile;
      ossimSupportFilesList::instance()->add(_productXmlFile);

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
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_NOTICE) << MODULE << "Loaded support data file <"<<xmlfile
                                              <<"> for image <"<<file<<">."<<std::endl;

         ossimNotify(ossimNotifyLevel_DEBUG) <<MODULE
                                             << "4 corners from Projection: " << "\n"
                                             << ul << ", " << ur << ", " << lr << ", " << ll << "\n";
      }

      // OSSIM preferences specifies whether a coarse grid needs to be generated:
      if (!createReplacementOCG()) break;
      return true;
   }
   
   // If we broke out of the while, something happened...
   return false;

} // End of: bool ossimTerraSarModel::open(const ossimFilename& file)

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
      kwl.add(prefix, PRODUCT_XML_FILE_KW, _productXmlFile.c_str());   

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

   kwl.add(prefix, PRODUCT_TYPE, _productType.c_str());

   kwl.add(prefix, RADIOMETRIC_CORRECTION, _radiometricCorrection.c_str());

   ossimString kw = ACQUISITION_INFO;
   ossimString kw2 = kw + IMAGING_MODE;
   kwl.add(prefix, kw2, _imagingMode.c_str());
   kw2 = kw + SENSOR;
   kwl.add(prefix, kw2, _acquisitionSensor.c_str());
   kw2 = kw + LOOK_DIRECTION;
   kwl.add(prefix, kw2, _lookDirection.c_str());
   kw2 = kw + POLARISATION_MODE;
   kwl.add(prefix, kw2, _polarisationMode.c_str());
   kw2 = kw + POLARISATION_LIST;
   for(ossim_uint32 i = 0; i < _numberOfLayers; ++i)
   {	
      ossimString iStr = ossimString::toString(i)+"]";
      ossimString kw3 = kw2+"[";
      kw3 += iStr;  		
      kwl.add(prefix, kw3, _polLayerList[i].c_str());
   }

   if( _polLayer !="UNDEFINED")
   {
      ossim_uint32 polLayerIdx = 0;
      for(ossim_uint32 idx = 0 ; idx < _polLayerList.size(); ++idx)
      {
         if(_polLayerList[idx] == _polLayer)
         {
            polLayerIdx = idx;
         }    
      }  
      _noise[polLayerIdx].saveState(kwl,prefix);
   }
   else
   {
      for(ossim_uint32 i = 0; i < _numberOfLayers; ++i)
      {	
         _noise[i].saveState(kwl,prefix);
      }
   }		
   _sceneCoord->saveState(kwl,prefix);
   
   for(ossim_uint32 i = 0; i < _numberOfLayers; ++i)
   {	
      kwl.add(prefix, CALIBRATION_CALFACTOR, ossimString::toString(_calFactor[i]).c_str());
   }
   kwl.add(prefix, RADAR_FREQUENCY, ossimString::toString(_radarFrequency).c_str());
   kwl.add(prefix, AZ_START_TIME, _azStartTime.c_str());
   kwl.add(prefix, AZ_STOP_TIME, _azStopTime.c_str());
   kwl.add(prefix, RG_FIRST_TIME, _rgFirstPixelTime.c_str());
   kwl.add(prefix, RG_LAST_TIME, _rgLastPixelTime.c_str());

   kwl.add(prefix, GENERATION_TIME, _generationTime.c_str());

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
      _productXmlFile = lookup;

      // See if caller wants to load from xml vice keyword list.
      lookup = kwl.find(prefix, LOAD_FROM_PRODUCT_FILE_KW);
      if (lookup)
      {
         s = lookup;
         if ( s.toBool() )
         {
            // Loading from product.xml file.
            return open(_productXmlFile);
         }
      }
   }
   

   //---
   // Temp:  This must be cleared or you end up with a bounding rect of all
   // zero's.
   //---
   theBoundGndPolygon.clear();

   // Load the base class.
   bool result = ossimGeometricSarSensorModel::loadState(kwl, prefix);

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
      
      ////////////////////////////////////////////////////

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
         _productXmlFile = lookup;
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

   // Load the base class.
   /*
    * TODO correct loadState
    */
#if 0
   if ( !_noise)
   {
      _noise = new Noise();
   }
   if ( _noise->loadState(kwl, prefix) == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\n_noise->loadState failed!\n";
      }
      result = false;
   }
#endif


   // Load the base class.
   if ( !_sceneCoord)
   {
      _sceneCoord = new SceneCoord();
   }

   if ( _sceneCoord->loadState(kwl,prefix) == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\n__sceneCoord->loadState failed!\n";
      }
      result = false;
   }
     
   lookup = kwl.find(prefix, CALIBRATION_CALFACTOR);
   if (lookup)
   {
      std::istringstream in(lookup);
      ossimString tempValue;
      for(ossim_uint32 idx = 0; idx < _numberOfLayers; ++idx)
      {
         in >> tempValue;
         _calFactor[idx] = tempValue.toDouble();
      		
      }
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << CALIBRATION_CALFACTOR << "\n";
      } 
      result = false;
   }

   lookup = kwl.find(prefix, RADAR_FREQUENCY);
   if (lookup)
   {
      s = lookup;
      _radarFrequency= s.toDouble();
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << RADAR_FREQUENCY << "\n";
      } 
      result = false;
   }

   lookup = kwl.find(prefix, AZ_START_TIME);
   if (lookup)
   {
      _azStartTime = lookup;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << AZ_START_TIME << "\n";
      } 
      result = false;
   }

   lookup = kwl.find(prefix, AZ_STOP_TIME);
   if (lookup)
   {
      _azStopTime = lookup;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << AZ_STOP_TIME << "\n";
      } 
      result = false;
   }

   lookup = kwl.find(prefix, RG_FIRST_TIME);
   if (lookup)
   {
      _rgFirstPixelTime = lookup;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << RG_FIRST_TIME << "\n";
      }
      result = false;
   }

   lookup = kwl.find(prefix, RG_LAST_TIME);
   if (lookup)
   {
      _rgLastPixelTime = lookup;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << RG_LAST_TIME << "\n";
      }
      result = false;
   }


   lookup = kwl.find(prefix, GENERATION_TIME);
   if (lookup)
   {
      _generationTime = lookup;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\nRequired keyword not found: "
            << GENERATION_TIME << "\n";
      } 
      result = false;
   }

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
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::print";
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
       << PRODUCT_XML_FILE_KW << ": " << _productXmlFile.c_str() << "\n";
   
   ossimGeometricSarSensorModel::print(out);
   for(ossim_uint32 i = 0; i < _numberOfLayers; ++i)
   {
      if ( !_noise[i].print(out) )
      {
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_WARN)
               << MODULE
               << "\n_noise->print failed!\n";
         }
      }
   }

   if ( !_sceneCoord->print(out) )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_WARN)
            << MODULE
            << "\n_sceneCoord->print failed!\n";
      }
   }

   out << NUMBER_LAYERS <<  ": " << _numberOfLayers << "\n";
   for(ossim_uint32 idx = 0; idx < _numberOfLayers; ++idx)
   {
      out << CALIBRATION_CALFACTOR << "["<< idx << "]: " << _calFactor[idx] << "\n";
   }
   out << RADAR_FREQUENCY <<  ": " << _radarFrequency<< "\n";
   out << AZ_START_TIME <<  ": " << _azStartTime << "\n";
   out << AZ_STOP_TIME <<  ": " << _azStopTime << "\n";
   out << RG_FIRST_TIME <<  ": " << _rgFirstPixelTime << "\n";
   out << RG_LAST_TIME <<  ": " << _rgLastPixelTime << "\n";
   out << GENERATION_TIME <<  ": " << _generationTime << "\n";

   // Reset flags.

   ossimString kw = ACQUISITION_INFO;
   ossimString kw2 = kw + IMAGING_MODE;
   out << kw2 <<  ": " << _imagingMode.c_str()<< "\n";
   kw2 = kw + SENSOR;
   out << kw2<<  ": " <<  _acquisitionSensor.c_str()<< "\n";
   kw2 = kw + LOOK_DIRECTION;
   out << kw2<<  ": " <<  _lookDirection.c_str()<< "\n";
   kw2 = kw + POLARISATION_MODE;
   out << kw2<<  ": " <<  _polarisationMode.c_str()<< "\n";
/*   kw2 = kw + POLARISATION_LIST;
     for(ossim_uint32 i = 0; i < _numberOfLayers; ++i)
     {	
     out << kw2 <<  "["<< i <<"] : " <<  _polLayer[i].c_str()<< "\n";
     if ( _noise[i]->print(out) == false )
     {
     if (traceDebug())
     {
     ossimNotify(ossimNotifyLevel_WARN)
     << MODULE
     << "\n_noise->print failed!\n";
     }
     }
     }
*/


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
      if (! ossim::iso8601TimeStringToCivilDate(utcString, eph_civil_date)) 
        {
        delete [] ephemeris;
        return false;
        }

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
   if (! ossim::iso8601TimeStringToCivilDate(inp_sctim_string, *date)) 
     {
     delete date;
     return false ;
     }

   if(_platformPosition != NULL)
   {
      Ephemeris * ephemeris = _platformPosition->Interpolate((JSDDateTime)*date);
      if (ephemeris == NULL) 
        {
        delete date;
        return false ;
        }

      _refPoint->set_ephemeris(ephemeris);

      delete ephemeris;
   }
   else
   {
      delete date;
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
      if (! ossim::iso8601TimeStringToCivilDate(azimuthStartTime, *dateStart)) 
        {
        delete dateStart;
        return false ;
        }
      CivilDateTime * dateStop = new CivilDateTime() ;
      if (! ossim::iso8601TimeStringToCivilDate(azimuthStopTime, *dateStop)) 
        {
        delete dateStart;
        delete dateStop;
        return false ;
        }
      double acq_msec_first = (double) dateStart->get_second()+dateStart->get_decimal();
      double acq_msec_last = (double) dateStop->get_second()+dateStop->get_decimal();

      double actualPRF = theImageSize.y/(acq_msec_last-acq_msec_first) ;
      _sensor->set_nAzimuthLook(_sensor->get_prf()/actualPRF); 

      delete dateStart;
      delete dateStop;
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
      ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
   }

   if (_sensor )
   {
      delete _sensor;
   }
   _sensor =  new SensorParams();


   bool result = tsDoc.initSensorParams(xdoc, _sensor);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "result for  tsDoc.initSensorParams " << result << endl;
   }

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

bool ossimplugins::ossimTerraSarModel::initAcquisitionInfo(
   const ossimXmlDocument* xdoc, const ossimTerraSarProductDoc& tsDoc)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::initAcquisitionInfo";
   bool result;
   ossimString     s;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entering...\n";
   }   
   
   result = tsDoc.geNumberOfLayers(xdoc, s);
   if ( result == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "unable to get Number Of Layers \n";
      }   
      setErrorStatus();
      return false;
   }
   _numberOfLayers = ossimString(s).toUInt32();

   result = tsDoc.getMission(xdoc, theSensorID);
   if ( result == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "unable to get theSensorID \n";
      }   
      setErrorStatus();
      return false;
   }
   
   result = tsDoc.getImagingMode(xdoc, _imagingMode);
   if ( result == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "unable to get Imaging Mode \n";
      }   
      setErrorStatus();
      return false;
   }

   result = tsDoc.getAcquisitionSensor(xdoc, _acquisitionSensor);
   if ( result == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "unable to get Acquisition Sensor \n";
      }   
      setErrorStatus();
      return false;
   }
   result = tsDoc.getLookDirection(xdoc, _lookDirection);
   if ( result == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "unable to get Look direction \n";
      }   
      setErrorStatus();
      return false;
   }
   result = tsDoc.getPolarisationMode(xdoc, _polarisationMode);
   if ( result == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "unable to get Polarisation Mode \n";
      }   
      setErrorStatus();
      return false;
   }
   result = tsDoc.getPolLayerList(xdoc, _polLayerList);
   if ( result == false )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "unable to get Polarisation Layer list \n";
      }   
      setErrorStatus();
      return false;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " leaving...\n";
   }   

   return result;
}

bool ossimplugins::ossimTerraSarModel::initSceneCoord(
   const ossimXmlDocument* xdoc, const ossimTerraSarProductDoc& tsDoc)
{
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::initSceneCoord";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }   
   
   if (_sceneCoord)
   {
      delete _sceneCoord;
   }
   
   _sceneCoord = new SceneCoord();
   
   bool result = tsDoc.initSceneCoord(xdoc, _sceneCoord);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimplugins::ossimTerraSarModel::getNoiseAtGivenNode(
   const ossimRefPtr<ossimXmlNode> xmlDocument, ossimplugins::Noise& noise)
{
   ossimString xpath;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node;

   std::vector<ImageNoise> tabImageNoise;
   ImageNoise              ev;
   tabImageNoise.clear();
 
   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::getNoise";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }  

   if ( !xmlDocument )
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG: one of the getNoise parameter of the method is NULL" << std::endl;
      }
      return false;	
   }

   xml_nodes.clear();
   xpath = "numberOfNoiseRecords";
   xmlDocument->findChildNodes(xpath, xml_nodes);
   if(xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }
    
   noise.set_numberOfNoiseRecords( xml_nodes[0]->getText().toInt32() );


   xml_nodes.clear();
   xpath = "imageNoise";
   xmlDocument->findChildNodes(xpath, xml_nodes);
   if(xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }

   node = xml_nodes.begin();
   while (node != xml_nodes.end())
   {
      sub_nodes.clear();
      xpath = "timeUTC";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
      ev.set_timeUTC(sub_nodes[0]->getText());
    
      sub_nodes.clear();
      xpath = "noiseEstimate/validityRangeMin";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
      ev.set_validityRangeMin( sub_nodes[0]->getText().toDouble() );

      sub_nodes.clear();
      xpath = "noiseEstimate/validityRangeMax";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
      ev.set_validityRangeMax( sub_nodes[0]->getText().toDouble() );

      sub_nodes.clear();
      xpath = "noiseEstimate/referencePoint";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
      ev.set_referencePoint( sub_nodes[0]->getText().toDouble() );

      sub_nodes.clear();
      xpath = "noiseEstimate/polynomialDegree";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
      ev.set_polynomialDegree( sub_nodes[0]->getText().toInt32() );

      sub_nodes.clear();
      ossimXmlNode::ChildListType nodelist;
      (*node)->findChildNodes("noiseEstimate/coefficient",nodelist);
      ossimXmlNode::ChildListType::const_iterator child_iter = nodelist.begin();
      std::vector<double> polynomialCoefficients;
      while(child_iter != nodelist.end())
      {
         double coefficient = ((*child_iter)->getText()).toDouble() ;
         polynomialCoefficients.push_back(coefficient);
         ++child_iter;
      }                  
      ev.set_polynomialCoefficients( polynomialCoefficients );
 
      tabImageNoise.push_back(ev);

      ++node;
   }

   noise.set_imageNoise(tabImageNoise);
 
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " leaving...\n";
   }   

   return true;
}


bool ossimplugins::ossimTerraSarModel::initNoise(
   const ossimXmlDocument* xmlDocument, const ossimTerraSarProductDoc& /* tsDoc */ )
{
   ossimString xpath;
   ossimString polLayerName;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node;
   bool result;

   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::initNoise";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entering...\n";
   }   

   _noise.resize(_numberOfLayers);
      
   xml_nodes.clear();
   xpath = "/level1Product/noise";

   xmlDocument->findNodes(xpath, xml_nodes);
   if(xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }

   node = xml_nodes.begin();
   while (node != xml_nodes.end())
   {
      sub_nodes.clear();
      xpath = "polLayer";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
    
      polLayerName = sub_nodes[0]->getText();

      ossim_uint32 polLayerIdx = 0;
      bool polLayerIdxFound = false;
    
      for(ossim_uint32 idx = 0 ; idx < _polLayerList.size(); ++idx)
      {
         if(_polLayerList[idx] == polLayerName)
         {
            polLayerIdx = idx;
            polLayerIdxFound = true;
         }    
      }  

      if (!polLayerIdxFound)
      {
         setErrorStatus();
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG: Unable to find polLayer in polLayer List"
                                                << std::endl;
         }
         return false;
      }
	
      sub_nodes.clear();

      //_noise[polLayerIdx] = new Noise();
      _noise[polLayerIdx].set_imagePolarisation(polLayerName);
   
      result = getNoiseAtGivenNode( (*node),_noise[polLayerIdx]);
      if(!result)
      {
         setErrorStatus();
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " DEBUG: Unable to getNoise for the " << polLayerName
                                                << " layer image" << std::endl;
         }
         return false;
      }

      ++node;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " leaving...\n";
   }   

   return true;
}


bool ossimplugins::ossimTerraSarModel::getPolLayerFromImageFile(
   const ossimXmlDocument* xmlDocument, const ossimFilename& imageFilename)
{
   ossimString xpath;
   ossimString polLayerName;
   ossimString polLayerFileName;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node;

   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::getPolLayerFromImageFile";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entering...\n";
   }   
   
   xml_nodes.clear();
   xpath = "/level1Product/productComponents/imageData";

   xmlDocument->findNodes(xpath, xml_nodes);
   if(xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }

   node = xml_nodes.begin();
   while (node != xml_nodes.end())
   {
      sub_nodes.clear();
      xpath = "polLayer";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
      polLayerName = sub_nodes[0]->getText();
	
      sub_nodes.clear();
      xpath = "file/location/filename";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
      polLayerFileName = sub_nodes[0]->getText();

      if (polLayerFileName == imageFilename.file())
      {
         _polLayer = polLayerName;
      }
      ++node;
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " leaving...\n";
   }   
   return true;
}


bool ossimplugins::ossimTerraSarModel::initCalibration(
   const ossimXmlDocument* xmlDocument, const ossimTerraSarProductDoc& /* tsDoc */ )
{
   ossimString xpath;
   ossimString polLayerName;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node;

   static const char MODULE[] = "ossimplugins::ossimTerraSarModel::initCalibration";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entering...\n";
   }   

   _calFactor.resize(_numberOfLayers);
      
   xml_nodes.clear();
   xpath = "/level1Product/calibration/calibrationConstant";

   xmlDocument->findNodes(xpath, xml_nodes);
   if(xml_nodes.size() == 0)
   {
      setErrorStatus();
      if(traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " DEBUG:"
            << "\nCould not find: " << xpath
            << std::endl;
      }
      return false;
   }

   node = xml_nodes.begin();
   while (node != xml_nodes.end())
   {
      sub_nodes.clear();
      xpath = "polLayer";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
    
      polLayerName = sub_nodes[0]->getText();

      ossim_uint32 polLayerIdx = 0;
      bool polLayerIdxFound = false;
    
      for(ossim_uint32 idx = 0 ; idx < _polLayerList.size(); ++idx)
      {
         if(_polLayerList[idx] == polLayerName)
         {
            polLayerIdx = idx;
            polLayerIdxFound = true;
         }    
      }  

      if(!polLayerIdxFound)
      {
         setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG: Unable to found polLayer in polLayer List" << std::endl;
         }
         return false;	
      }
	
      sub_nodes.clear();
      xpath = "calFactor";
      (*node)->findChildNodes(xpath, sub_nodes);
      if (sub_nodes.size() == 0)
      {
    	 setErrorStatus();
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << " DEBUG:"
               << "\nCould not find: " << xpath
               << std::endl;
         }
         return false;
      }
      _calFactor[polLayerIdx] = sub_nodes[0]->getText().toDouble();
      ++node;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " leaving...\n";
   }   

   return true;
}


bool ossimplugins::ossimTerraSarModel::findTSXLeader(const ossimFilename& file,
                                                     ossimFilename& metadataFile)
{
   bool res = false;

   if ( file.exists() && (file.ext().downcase() == "xml") )
   {
      metadataFile = file;
      res = true;
   }
   else if (!file.exists())
   {
      res = false;
   }
   else
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimplugins::ossimTerraSarModel::findTSXLeader "
            << " directory scan turned off.  This is killing the factory open."
            << " We should never scan a directory.  Need to resolve. "
            << std::endl;
      }

//#if 0
      ossimFilename imagePath = file.path();
      if (imagePath.empty())
         imagePath = ossimEnvironmentUtility::instance()->getCurrentWorkingDir();

      ossimDirectory directory = ossimDirectory(imagePath.path());
      std::vector<ossimFilename> vectName;
      ossimString reg = ".xml";
      directory.findAllFilesThatMatch( vectName, reg, 1 );

      bool goodFileFound = false;
      unsigned int loop = 0;
      while(loop<vectName.size() && !goodFileFound)
      {
         ossimFilename curFile = vectName[loop];
         if(curFile.file().beforePos(3) == ossimString("TSX"))
            goodFileFound = true;
         else
            loop++;
      }
      if(goodFileFound)
      {
         metadataFile = vectName[loop];
         res = true;
      }
      else
      {
         if (traceDebug())
         {
            if (res)
            {
               this->print(ossimNotify(ossimNotifyLevel_DEBUG));
            }
            
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "ossimplugins::ossimTerraSarModel::findTSXLeader "
               << " exit status = " << (res?"true":"false\n")
               << std::endl;
         }
      }
//#endif
   }

   return res;
}


void ossimplugins::ossimTerraSarModel::printInfo(ostream& os) const
{
   os << "\n----------------- General Info on TSX-1 Image -------------------"
      << "\n  "
      << "\n  Sensor ID :               			" << theSensorID
      << "\n  Imaging Mode :               			" << _imagingMode
      << "\n  Acquisition Sensor :        			" << _acquisitionSensor
      << "\n  Look direction :          			" << _lookDirection
      << "\n  Polarisation Mode :          			" << _polarisationMode
      << "\n  Number of Layers :           			" << _numberOfLayers
      << "\n  Polarisation List Size :     			" << _polLayerList.size()
      << "\n"
      << "\n------------------------------------------------------------------";
   for(ossim_uint32 idx = 0 ; idx < _polLayerList.size(); ++idx)
   {
      os << "\n----------------- Info on " << _polLayerList[idx] <<" Layer Image -------------------"
         << "\n calFactor :                                  " << _calFactor[idx]
//               << "\n Image Noise  size :                          " << _noise[idx].get_imageNoise().size()
         << "\n------------------------------------------------------------";
   }  
   os << std::endl;	
}

