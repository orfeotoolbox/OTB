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

#include <cmath>

#include <ossimRadarSat2RPCModel.h>
#include <ossimPluginCommon.h>
#include <ossimRadarSat2ProductDoc.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimXmlNode.h>

#include <ossim/base/ossimException.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotifyContext.h>

#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>
#include <otb/GMSTDateTime.h>

#include <otb/SensorParams.h>
#include <otb/SarSensor.h>

namespace ossimplugins
{

// Keyword constants:
static const char LOAD_FROM_PRODUCT_FILE_KW[] = "load_from_product_file_flag";
static const char PRODUCT_XML_FILE_KW[] = "product_xml_filename";

// Static trace for debugging
static ossimTrace traceDebug("ossimRadarSat2RPCModel:debug");


RTTI_DEF1(ossimRadarSat2RPCModel, "ossimRadarSat2RPCModel", ossimRpcModel);

ossimRadarSat2RPCModel::ossimRadarSat2RPCModel()
   :
   ossimRpcModel(),
   theDecimation(1.0),
   _sensor(NULL),
   theProductXmlFile(ossimFilename::NIL)
{
}
   
ossimRadarSat2RPCModel::ossimRadarSat2RPCModel(const ossimRadarSat2RPCModel& rhs)
   :
   ossimRpcModel(rhs),
   theDecimation(1.0),
   _sensor(NULL),
   theProductXmlFile(rhs.theProductXmlFile)
{
   
}


//*****************************************************************************
//  CONSTRUCTOR: ossimRadarSat2RPCModel
//  
//  Constructs given filename for RS2 product.xml file
//  
//*****************************************************************************

ossimRadarSat2RPCModel::ossimRadarSat2RPCModel(const ossimFilename& RS2File)
   :ossimRpcModel(), theDecimation(1.0)
{
   
   // Open the XML file, and read the RPC values in.
   if (!open(RS2File))
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "DEBUG ossimRadarSat2RPCModel::ossimRadarSat2RPCModel(RS2File): Unable to parse file " << RS2File
           << std::endl;
   }

}
   
ossimRadarSat2RPCModel::~ossimRadarSat2RPCModel()
{
}

ossimString ossimRadarSat2RPCModel::getClassName() const
{
   return ossimString("ossimRadarSat2RPCModel");
}
   
// ------ DUPLICATE RS2 MODEL OBJECT ----------
   
ossimObject* ossimRadarSat2RPCModel::dup() const
{
   return new ossimRadarSat2RPCModel(*this);
}

bool ossimRadarSat2RPCModel::open(const ossimFilename& file)
{
   static const char MODULE[] = "ossimRadarSat2RPCModel::open";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }
   
   bool result = false;
   
   // Get the xml file.
   ossimFilename xmlFile;
   
   if (file.ext().downcase() == "xml")
   {
      xmlFile = file;
   }
   else if (file.isFile())
   {
      xmlFile = file.expand().path().dirCat("product.xml");
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "product xml file: " << xmlFile << "\n";
   }
   
   if ( xmlFile.exists() )
   {
      //---
      // Instantiate the XML parser:
      //---
      ossimXmlDocument* xdoc = new ossimXmlDocument();
      if ( xdoc->openFile(xmlFile) )
      {
         ossimRadarSat2ProductDoc rsDoc;
         
         result = rsDoc.isRadarSat2(xdoc);
         
         if (result)
         {
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "isRadarSat2...\n";
               ossimString s;
               if ( rsDoc.getBeamModeMnemonic(xdoc, s) )
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "beam_mode_mnemonic: " << s << "\n";
               }
               if ( rsDoc.getAcquisitionType(xdoc, s) )
               {
                  ossimNotify(ossimNotifyLevel_DEBUG)
                     << "acquisition_type: " << s << "\n";
               }
            }
            
            // Set the base class number of lines and samples
            result = rsDoc.initImageSize(xdoc, theImageSize);
            
            
            //---
            // Fetch Image Size:
            //---

            theImageSize.line = theImageSize.x;
            theImageSize.samp =theImageSize.y;


            //***
                 // Assign the bounding image space rectangle:
                 //***

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

            // Set the image id.
            if (result)
            {
               result = rsDoc.getImageId(xdoc, theImageID);
            }

            // Set the sensor ID.
            if (result)
            {
               result = rsDoc.getSatellite(xdoc, theSensorID);
            }

            // Set the base class gsd:
            if(result)
              {
              result = rsDoc.initGsd(xdoc, theGSD);
              }

            if (result)
            {
               theMeanGSD = (theGSD.x + theGSD.y)/2.0;
            }


         }

         thePolyType = B;


         RPCModel model;
         model = rsDoc.getRpcData(xdoc);


         theBiasError  = model.biasError;
         theRandError  = model.randomError;
         theLineOffset = model.lineOffset;
         theSampOffset = model.pixelOffset;
         theLatOffset  = model.latitudeOffset;
         theLonOffset  = model.longitudeOffset;
         theHgtOffset  = model.heightOffset;
         theLineScale  = model.lineScale;
         theSampScale  = model.pixelScale;
         theLatScale   = model.latitudeScale;
         theLonScale   = model.longitudeScale;
         theHgtScale   = model.heightScale;


         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "All parameters RPC : "
               << theBiasError << ", "
               << theRandError << ", "
               << theLineOffset << ", "
               << theSampOffset << ", "
               << theLatOffset << ", "
               << theLonOffset << ", "
               << theHgtOffset << ", "
               << theLineScale << ", "
               << theSampScale << ", "
               << theLatScale << ", "
               << theLonScale << ", "
               << theHgtScale << ", " << std::endl;
         }

         // Parse coefficients:
         ossim_uint32 i;

         for (i=0; i<20; ++i)
         {

            theLineNumCoef[i] = model.lineNumeratorCoefficients[i];
            theLineDenCoef[i] = model.lineDenominatorCoefficients[i];
            theSampNumCoef[i] = model.pixelNumeratorCoefficients[i];
            theSampDenCoef[i] = model.pixelDenominatorCoefficients[i];
         }

         // Assign other data members to default values:
         theNominalPosError = sqrt(theBiasError*theBiasError +
                                   theRandError*theRandError); // meters




      } // matches: if ( xdoc->openFile(xmlFile) )

      delete xdoc;
      xdoc = 0;

   } // matches: if ( xmlFile.exists() )

   if (result)
   {
      theProductXmlFile = xmlFile;
   }
   else
   {
      theProductXmlFile = ossimFilename::NIL;
   }

   if (result)
   {
      theProductXmlFile = xmlFile;
   }
   else
   {
      theProductXmlFile = ossimFilename::NIL;
   }

   if (result)
   {
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

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }



   //---
   // Get the decimation if any from the header "IMAG" field.
   //
   // Look for string like:
   // "/2" = 1/2
   // "/4  = 1/4
   // ...
   // "/16 = 1/16
   // If it is full resolution it should be "1.0"
   //---

   theDecimation = 1.0;



   //***
        // Assign other data members:
        //***
             thePolyType = B; 					// This may not be true for early RPC imagery
   theRefImgPt.line = theImageSize.line/2.0;
   theRefImgPt.samp = theImageSize.samp/2.0;
   theRefGndPt.lat  = theLatOffset;
   theRefGndPt.lon  = theLonOffset;
   theRefGndPt.hgt  = theHgtOffset;


   //---
   // Assign the bounding ground polygon:
   //
   // NOTE:  We will use the base ossimRpcModel for transformation since all
   // of our calls are in full image space (not decimated).
   //---

   ossimGpt v0, v1, v2, v3;
   ossimDpt ip0 (0.0, 0.0);
   ossimRpcModel::lineSampleHeightToWorld(ip0, theHgtOffset, v0);
   ossimDpt ip1 (theImageSize.samp-1.0, 0.0);
   ossimRpcModel::lineSampleHeightToWorld(ip1, theHgtOffset, v1);
   ossimDpt ip2 (theImageSize.samp-1.0, theImageSize.line-1.0);
   ossimRpcModel::lineSampleHeightToWorld(ip2, theHgtOffset, v2);
   ossimDpt ip3 (0.0, theImageSize.line-1.0);
   ossimRpcModel::lineSampleHeightToWorld(ip3, theHgtOffset, v3);

   theBoundGndPolygon
      = ossimPolygon (ossimDpt(v0), ossimDpt(v1), ossimDpt(v2), ossimDpt(v3));

   updateModel();

   // Set the ground reference point.
   ossimRpcModel::lineSampleHeightToWorld(theRefImgPt,
                                          theHgtOffset,
                                          theRefGndPt);

   if ( theRefGndPt.isLatNan() || theRefGndPt.isLonNan() )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimNitfRpcModel::ossimNitfRpcModel DEBUG:"
            << "\nGround Reference Point not valid."
            << " Aborting with error..."
            << std::endl;
      }
      setErrorStatus();
      return false;
   }


   return true;
}

   
void ossimRadarSat2RPCModel::worldToLineSample(const ossimGpt& world_point,
                                               ossimDpt&       image_point) const
{
   // Get the full res (not decimated) point.
   ossimRpcModel::worldToLineSample(world_point, image_point);
   
   // Apply decimation.
   image_point.x = image_point.x * theDecimation;
   image_point.y = image_point.y * theDecimation;
}
   
void ossimRadarSat2RPCModel::lineSampleHeightToWorld(
   const ossimDpt& image_point,
   const double&   heightEllipsoid,
   ossimGpt&       worldPoint) const
{
   
   // Convert image point to full res (not decimated) point.
   ossimDpt pt;
   pt.x = image_point.x / theDecimation;
   pt.y = image_point.y / theDecimation;
   
   // Call base...
   ossimRpcModel::lineSampleHeightToWorld(pt, heightEllipsoid, worldPoint);
}

std::ostream& ossimRadarSat2RPCModel::print(std::ostream& out) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();
   
   ossimRpcModel::print(out);
   
   // Reset flags.
   out.setf(f);
   
   return out;
}
   
bool ossimRadarSat2RPCModel::InitSensorParams(const ossimKeywordlist &kwl,
                                              const char *prefix)
{
   
   // sensor frequencies
   const char* central_freq_str = kwl.find(prefix,"central_freq");
   double central_freq = 0.0;
   if (central_freq_str)
   {
      central_freq = ossimString::toDouble(central_freq_str);
   }
   const char* fr_str = kwl.find(prefix,"fr");
   double fr = 0.0;
   if (fr_str)
   {
      fr = ossimString::toDouble(fr_str);
   }
   const char* fa_str = kwl.find(prefix,"fa");
   double fa = 0.0;
   if (fa_str)
   {
      fa = ossimString::toDouble(fa_str);
   }
   
   //number of different looks
   const char* n_azilok_str = kwl.find(prefix,"n_azilok");
   double n_azilok = 0.0;
   if (n_azilok_str)
   {
      n_azilok = ossimString::toDouble(n_azilok_str);
   }
   const char* n_rnglok_str = kwl.find(prefix,"n_rnglok");
   double n_rnglok = 0.0;
   if (n_rnglok_str)
   {
      n_rnglok = ossimString::toDouble(n_rnglok_str);
   }
   
   //ellipsoid parameters
   const char* ellip_maj_str = kwl.find(prefix,"ellip_maj");
   double ellip_maj = 0.0;
   if (ellip_maj_str)
   {
      ellip_maj = ossimString::toDouble(ellip_maj_str) * 1000.0;// km -> m
   }
   const char* ellip_min_str = kwl.find(prefix,"ellip_min");
   double ellip_min = 0.0;
   if (ellip_min_str)
   {
      ellip_min = ossimString::toDouble(ellip_min_str) * 1000.0;// km -> m
   }
   
   if(_sensor != 0)
   {
      delete _sensor;
   }
   
   _sensor = new SensorParams();
   
   const char* lineTimeOrdering_str = kwl.find(prefix,"lineTimeOrdering");
   std::string lineTimeOrdering(lineTimeOrdering_str) ;
   const char* pixelTimeOrdering_str = kwl.find(prefix,"pixelTimeOrdering");
   std::string pixelTimeOrdering(pixelTimeOrdering_str) ;
   if (pixelTimeOrdering == "Increasing") _sensor->set_col_direction(1);
   else _sensor->set_col_direction(- 1);
   if (lineTimeOrdering == "Increasing") _sensor->set_lin_direction(1);
   else _sensor->set_lin_direction(- 1);
   
   const char* lookDirection_str = kwl.find(prefix,"lookDirection");
   std::string lookDirection(lookDirection_str) ;
   if ((lookDirection == "Right")||(lookDirection == "RIGHT")) _sensor->set_sightDirection(SensorParams::Right) ;
   else _sensor->set_sightDirection(SensorParams::Left) ;
   
   _sensor->set_sf(fr);
   const double CLUM        = 2.99792458e+8 ;
   double wave_length = CLUM / central_freq ;
   _sensor->set_rwl(wave_length);
   _sensor->set_nAzimuthLook(n_azilok);
   _sensor->set_nRangeLook(n_rnglok);
   
   // fa is the processing PRF
   _sensor->set_prf(fa * n_azilok);
   
   _sensor->set_semiMajorAxis(ellip_maj) ;
   _sensor->set_semiMinorAxis(ellip_min) ;
   
   return true;
}
   
bool ossimRadarSat2RPCModel::initSensorParams(
   const ossimXmlDocument* xdoc, const ossimRadarSat2ProductDoc& rsDoc)
{
   static const char MODULE[] = "ossimRadarSat2RPCModel::initSensorParams";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }
   
   if (_sensor )
   {
      delete _sensor;
   }
   _sensor =  new SensorParams();
   
   bool result = rsDoc.initSensorParams(xdoc, _sensor);
   
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

bool ossimRadarSat2RPCModel::saveState(ossimKeywordlist& kwl,
                                       const char* prefix) const
{
   static const char MODULE[] = "ossimRadarSat2RPCModel::saveState";
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }
   
   bool result = true;
   
   kwl.add(prefix, "decimation", theDecimation);
   
   // Save our state:
   kwl.add(prefix, PRODUCT_XML_FILE_KW, theProductXmlFile.c_str());
   
   if (result)
   {
      // Call base save state:
      result = ossimRpcModel::saveState(kwl, prefix);
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }
   
   return result;
}


bool ossimRadarSat2RPCModel::loadState (const ossimKeywordlist &kwl,
                                        const char *prefix)
{
   static const char MODULE[] = "ossimRadarSat2RPCModel::loadState";
   
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
   
   
   // Call base.
   bool result = ossimRpcModel::loadState(kwl, prefix);

   // Lookup decimation.
   const char* value = kwl.find(prefix, "decimation");
   if (value)
   {
      theDecimation = ossimString(value).toFloat64();
      if (theDecimation <= 0.0)
      {
         // Do not allow negative or "0.0"(divide by zero).
         theDecimation = 1.0;
      }
   }
   
   
   //---
   // Temp:  This must be cleared or you end up with a bounding rect of all
   // zero's.
   //---
   theBoundGndPolygon.clear();
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }
   
   return result;
}
}
