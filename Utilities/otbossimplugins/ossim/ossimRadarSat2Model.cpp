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

#include <ossimRadarSat2Model.h>
#include <ossimPluginCommon.h>
#include <ossimRadarSat2ProductDoc.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
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

// Static trace for debugging
static ossimTrace traceDebug("ossimRadarSat2Model:debug");

// Keyword constants:
static std::string PRODUCT_XML_FILE_KW = "product_xml_filename";

RTTI_DEF1(ossimRadarSat2Model, "ossimRadarSat2Model", ossimGeometricSarSensorModel);


ossimRadarSat2Model::ossimRadarSat2Model():
   _nbCol(0.0),
   _pixel_spacing(0.0),
   _n_srgr(0),
   _srgr_update(),
   _SrGr_R0(),
   theProductDotXmlFile(ossimFilename::NIL)
{
}

ossimRadarSat2Model::ossimRadarSat2Model(const ossimRadarSat2Model& rhs)
   :
   ossimGeometricSarSensorModel(rhs),
   _nbCol(rhs._nbCol),
   _pixel_spacing(rhs._pixel_spacing),
   _n_srgr(rhs._n_srgr),
   _srgr_update(rhs._srgr_update),
   _SrGr_R0(rhs._SrGr_R0),
   theProductDotXmlFile(rhs.theProductDotXmlFile)
{
}

ossimRadarSat2Model::~ossimRadarSat2Model()
{
}

ossimObject* ossimRadarSat2Model::dup() const
{
   return new ossimRadarSat2Model(*this);
}

double ossimRadarSat2Model::getSlantRangeFromGeoreferenced(double col) const
{
   if (_n_srgr==0) return(-1) ;
   
   double relativeGroundRange, slantRange = 0.0 ; 
   
   // in the case of Georeferenced images, _refPoint->get_distance()
   // contains the ground range
   relativeGroundRange = _refPoint->get_distance() + _sensor->get_col_direction() * (col-_refPoint->get_pix_col())* _pixel_spacing ; 
   
   int numSet = FindSRGRSetNumber((_refPoint->get_ephemeris())->get_date()) ;
   /** 
    * @todo : could be improved (date choice)
    */
   
   for (int i=0 ; i<static_cast<int>(_SrGr_coeffs[numSet].size()); i++)
   {
      slantRange += _SrGr_coeffs[numSet][i]*pow(relativeGroundRange,i) ;
   }
   
   return  slantRange ;
}

bool ossimRadarSat2Model::open(const ossimFilename& file)
{
   static const char MODULE[] = "ossimRadarSat2Model::open";

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
         << "xmlFile: " << xmlFile << "\n";
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
            }

            // Set the base class number of lines and samples
            result = rsDoc.initImageSize(xdoc, theImageSize);

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

            // Set the gsd x direction.  tmp drb compute later.
            ossimString s;
            if (result)
            {
               result = rsDoc.getSampledPixelSpacing(xdoc, s);
               if (result)
               {
                  theGSD.x = s.toFloat64();
               }
            }

            // Set the gsd y direction.  tmp drb
            if (result)
            {
               result = rsDoc.getSampledLineSpacing(xdoc, s);
               if (result)
               {
                  theGSD.y = s.toFloat64(s);
               }
            }

            // Set the mean gsd.
            theMeanGSD = (theGSD.x + theGSD.y)/2.0;

            if (result)
            {
               result = initSRGR(xdoc, rsDoc);

               if (result)
               {
                  result = initPlatformPosition(xdoc, rsDoc);

                  if (result)
                  {
                     result = initSensorParams(xdoc, rsDoc);

                     if (result)
                     {
                        result = initRefPoint(xdoc, rsDoc);
                     }
                  }
               }
            }
         }
         
      } // matches: if ( xdoc->openFile(xmlFile) )
      
      delete xdoc;
      xdoc = 0;
      
   } // matches: if ( xmlFile.exists() )

   if (result)
   {
      theProductDotXmlFile = xmlFile;
   }
   else
   {
      theProductDotXmlFile = ossimFilename::NIL;
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "PRODUCT_XML_FILE_KW: " << theProductDotXmlFile << "\n"
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   if (result)
   {
      theProductDotXmlFile = xmlFile;
   }
   else
   {
      theProductDotXmlFile = ossimFilename::NIL;
   }
   
   return result;
}

std::ostream& ossimRadarSat2Model::print(std::ostream& out) const
{
   cout << "ossimRadarSat2Model::print entered..." << endl;
   return out;
}

bool ossimRadarSat2Model::InitSensorParams(const ossimKeywordlist &kwl,
                                           const char *prefix)
{
   
   // sensor frequencies
   const char* central_freq_str = kwl.find(prefix,"central_freq");
   double central_freq = atof(central_freq_str);
   const char* fr_str = kwl.find(prefix,"fr");
   double fr = atof(fr_str);
   const char* fa_str = kwl.find(prefix,"fa");
   double fa = atof(fa_str);

   //number of different looks 
   const char* n_azilok_str = kwl.find(prefix,"n_azilok");
   double n_azilok = atof(n_azilok_str);
   const char* n_rnglok_str = kwl.find(prefix,"n_rnglok");
   double n_rnglok = atof(n_rnglok_str);

   //ellipsoid parameters
   const char* ellip_maj_str = kwl.find(prefix,"ellip_maj");
   double ellip_maj = atof(ellip_maj_str) * 1000.0;	// km -> m
   const char* ellip_min_str = kwl.find(prefix,"ellip_min");
   double ellip_min = atof(ellip_min_str) * 1000.0;  // km -> m

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

bool ossimRadarSat2Model::InitPlatformPosition(const ossimKeywordlist &kwl, const char *prefix)
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
      if (! ossim::UtcDateTimeStringToCivilDate(utcString, eph_civil_date)) return false;

      JSDDateTime eph_jsd_date(eph_civil_date);

      GeographicEphemeris* eph = new GeographicEphemeris(eph_jsd_date,pos,vit);

      ephemeris[i] = eph;
   }
	
   /*
    * Creation of the platform position interpolator
    */
   if (_platformPosition != 0)
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

bool ossimRadarSat2Model::InitRefPoint(const ossimKeywordlist &kwl,
                                       const char *prefix)
{
   // in order to use ossimSensorModel::lineSampleToWorld
   const char* nbCol_str = kwl.find(prefix,"nbCol");
   const char* nbLin_str = kwl.find(prefix,"nbLin");
   theImageSize.x      = atoi(nbCol_str);
   theImageSize.y      = atoi(nbLin_str);
   theImageClipRect    = ossimDrect(0, 0, theImageSize.x-1, theImageSize.y-1);

   // no reference point (e.g. scene center) is given in the annotation file
   // we choose to use the upper left corner as reference point
   if(_refPoint == 0)
   {
      _refPoint = new RefPoint();
   }

   _refPoint->set_pix_col(0);		// upper left corner
   _refPoint->set_pix_line(0);	// upper left corner

   const char* zeroDopplerTimeFirstLine_str = kwl.find(prefix,"zeroDopplerTimeFirstLine");
   std::string zeroDopplerTimeFirstLine(zeroDopplerTimeFirstLine_str);
	
   CivilDateTime * date = new CivilDateTime() ;
   if (! ossim::UtcDateTimeStringToCivilDate(zeroDopplerTimeFirstLine, *date)) return false ;

   if (_sensor->get_lin_direction() == -1) {
      double time = (double) date->get_second() + date->get_decimal() ;  // upper left corner
      time += theImageSize.y / _sensor->get_prf() ;
      date->set_second((int) floor(time)) ;
      date->set_decimal(time - floor(time)) ; 
   }

   if(_platformPosition != 0)
   {
      Ephemeris * ephemeris = _platformPosition->Interpolate((JSDDateTime)*date);
      if (ephemeris == 0) return false ; 
      _refPoint->set_ephemeris(ephemeris);

      delete ephemeris;
   }
   else
   {
      return false;
   }

   const char* slantRangeNearEdge_str = kwl.find(prefix,"slantRangeNearEdge");
   double distance = atof(slantRangeNearEdge_str);

   //---
   // NOTE:  initSRGR method must be called before this method.
   // in the case of Georeferenced images, the ground range is stored in
   // place of the slant range
   // (used for SlantRange computation relative to reference point, necessary
   // for optimization)
   //---
   if (_isProductGeoreferenced)
   {
      if (_sensor->get_col_direction() == 1)
      {
         distance += 0.0; // upper left corner
      }
      else
      {
         distance += _nbCol * _pixel_spacing;
      }
   }

   _refPoint->set_distance(distance);

   // Ground Control Points extracted from the model 
   std::list<ossimGpt> groundGcpCoordinates ; 
   std::list<ossimDpt> imageGcpCoordinates ; 
   const char* nTiePoints_str = kwl.find(prefix,"nTiePoints");
   int nTiePoints = atoi(nTiePoints_str);
   char name[64];
   for (int k=0 ; k<nTiePoints ; k++) {
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
      sprintf(name,"cornersHeight%i",k);
      const char* height_str = kwl.find(name);
      double height = atof(height_str);

      ossimDpt imageGCP(i,j);
      ossimGpt groundGCP(lat ,lon , height);
      groundGcpCoordinates.push_back(groundGCP) ; 
      imageGcpCoordinates.push_back(imageGCP) ;
   }

   // Default optimization 
   optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;

   return true;
}

bool ossimRadarSat2Model::InitSRGR(const ossimKeywordlist &kwl,
                                   const char *prefix)
{
   const char* productType_str = kwl.find(prefix,"productType");
   ossimString productType(productType_str); 

   _isProductGeoreferenced = (productType != "SLC") ;

   // Pixel spacing
   const char* pixel_spacing_str = kwl.find(prefix,"pixel_spacing_mean");
   _pixel_spacing = atof(pixel_spacing_str);

   // Number of columns
   const char* nbCol_str = kwl.find(prefix,"nbCol");
   _nbCol = atoi(nbCol_str);

   // number of SRGR coefficient sets
   const char* SrGr_coeffs_number_str = kwl.find(prefix,"SrGr_coeffs_number");
   _n_srgr = atoi(SrGr_coeffs_number_str);

   // SRGR coefficients and exponents
   char name[64];
   for(int i=0;i<_n_srgr;i++)
   {
      std::vector<double> srgr_set ; 
      for(int j=0;j<4;j++)
      {
         sprintf(name,"SrGr_coeffs_%i_%i",i,j);
         const char* coeff_str = kwl.find(prefix,name);
         double coeff = atof(coeff_str);
         srgr_set.push_back(coeff);
      }
      _SrGr_coeffs.push_back(srgr_set);
      // SRGR polynomial reference
      sprintf(name,"SrGr_R0_%i",i);
      const char* SrGr_R0_str = kwl.find(prefix,name);
      _SrGr_R0.push_back(atof(SrGr_R0_str));
      // SRGR update time
      sprintf(name,"SrGr_update_%i",i);
      const char* SrGr_update_str = kwl.find(prefix,name);
      CivilDateTime SrGr_update_date ; 
      ossim::UtcDateTimeStringToCivilDate(std::string(SrGr_update_str), SrGr_update_date) ; 
      _srgr_update.push_back((double) SrGr_update_date.get_second()+ SrGr_update_date.get_decimal());
   }
	
   return true;
}

int ossimRadarSat2Model::FindSRGRSetNumber(JSDDateTime date) const
{
   if (_n_srgr==0) return(-1) ;

   double delays[20];
   for (int i=0;i<_n_srgr;i++)
   {	
      delays[i] = fabs(date.get_second()+date.get_decimal()-_srgr_update[i]) ;   
   }
	
   int setNumber = 0 ; 
   double min_delay = delays[0] ; 
   for (int i=1;i<_n_srgr;i++)
   {
      if (delays[i]<min_delay) {
         setNumber = i ;
         min_delay = delays[i] ;
      }
   }
   return setNumber ;
}

bool ossimRadarSat2Model::initSRGR(const ossimXmlDocument* xdoc,
                                   const ossimRadarSat2ProductDoc& rsDoc)
{
   static const char MODULE[] = "ossimRadarSat2Model::initSRGR";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }
   
   bool result = true;

   ossimString s;
   
   // Get the product type.
   if ( rsDoc.getProductType(xdoc, s) )
   {
      _isProductGeoreferenced = (s != "SLC");
   }
   else
   {
      result = false;
   }

   // Get the pixel spacing.
   if ( rsDoc.getSampledPixelSpacing(xdoc, s) )
   {
      _pixel_spacing = s.toDouble();
   }
   else
   {
      result = false;
   }

   // Number of columns
   if ( rsDoc.getNumberOfSamplesPerLine(xdoc, s) )
   {
      _nbCol = s.toInt32();
   }
   else
   {
      result = false;
   }

   ossimString path =
      "/product/imageGenerationParameters/slantRangeToGroundRange";
   std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
   xdoc->findNodes(path, xnodes);
   if ( xnodes.size() )
   {
      //---
      // Set the number of slant range for each ground range (srgs)
      // coefficients.
      //---
      _n_srgr = static_cast<int>(xnodes.size());
      
      for (ossim_uint32 i = 0; i < xnodes.size(); ++i)
      {
         if (xnodes[i].valid())
         {
            ossimRefPtr<ossimXmlNode> node = 0;
            node = xnodes[i]->findFirstNode(
               ossimString("zeroDopplerAzimuthTime"));
            if (node.valid())
            {
               CivilDateTime date;
               ossim::UtcDateTimeStringToCivilDate(node->getText(), date);
               _srgr_update.push_back((double) date.get_second()+
                                      date.get_decimal());
            }

            node = xnodes[i]->findFirstNode(
               ossimString("groundRangeOrigin"));
            if (node.valid())
            {
               _SrGr_R0.push_back(node->getText().toDouble());
            }

            node = xnodes[i]->findFirstNode(
               ossimString("groundToSlantRangeCoefficients"));
            if (node.valid())
            {
               std::vector<ossimString> vs;
               node->getText().split(vs, ' ');
               std::vector<double> vd;
               for (ossim_uint32 idx = 0; idx < vs.size(); ++idx)
               {
                  vd.push_back(vs[idx].toDouble());
               }
               _SrGr_coeffs.push_back(vd);
            }
         }
      }
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }  

   return result;
}

bool ossimRadarSat2Model::initPlatformPosition(
   const ossimXmlDocument* xdoc, const ossimRadarSat2ProductDoc& rsDoc)
{
   static const char MODULE[] = "ossimRadarSat2Model::initPlatformPosition";

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
   
   bool result = rsDoc.initPlatformPosition(xdoc, _platformPosition);

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimRadarSat2Model::initSensorParams(
   const ossimXmlDocument* xdoc, const ossimRadarSat2ProductDoc& rsDoc)
{
   static const char MODULE[] = "ossimRadarSat2Model::initSensorParams";

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

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}

bool ossimRadarSat2Model::initRefPoint(const ossimXmlDocument* xdoc,
                                       const ossimRadarSat2ProductDoc& rsDoc)
{
   static const char MODULE[] = "ossimRadarSat2Model::initRefPoint";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }   
   
   if ( !_sensor || !_platformPosition )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << "null pointer error! exiting\n";
      }
      return false;
   }

   // Reference point.
   if (_refPoint)
   {
      delete _refPoint;
   }
   _refPoint = new RefPoint();

   // Ground Control Points extracted from the model.
   std::list<ossimGpt> groundGcpCoordinates; 
   std::list<ossimDpt> imageGcpCoordinates;
   if ( rsDoc.initTiePoints(xdoc,
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

   if ( setModelRefPoint(groundGcpCoordinates, imageGcpCoordinates) )
   {
      _refPoint->set_pix_col(theRefImgPt.x);
      _refPoint->set_pix_line(theRefImgPt.y);
   }
   else
   {
      // no reference point (e.g. scene center) is given in the annotation file
      // we choose to use the upper left corner as reference point
      _refPoint->set_pix_col(0);   // upper left corner
      _refPoint->set_pix_line(0);	// upper left corner
   }

   ossimString s;
   CivilDateTime date;
   if ( rsDoc.getZeroDopplerTimeFirstLine(xdoc, s) )
   {
      if (! ossim::UtcDateTimeStringToCivilDate(s, date))
      { 
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << MODULE << "getZeroDopplerTimeFirstLine error! exiting\n";
         }
         return false;
      }
   }
   
   if (_sensor->get_lin_direction() == -1)
   {
      // upper left corner
      double time = (double) date.get_second() + date.get_decimal();  
      time += theImageSize.y / _sensor->get_prf() ;
      date.set_second((int) floor(time)) ;
      date.set_decimal(time - floor(time)) ; 
   }
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "date:\n"
         << date
         << "\n";
   }
   
   JSDDateTime jsdate(date);

   // Ephemeris * ephemeris = _platformPosition->Interpolate((JSDDateTime)date);
   Ephemeris * ephemeris = _platformPosition->Interpolate(jsdate);
   if (ephemeris == 0)
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << " Interpolate error! exiting\n";
      }
      return false;
   }
   _refPoint->set_ephemeris(ephemeris);
   delete ephemeris;
   ephemeris = 0;

   if ( !rsDoc.getSlantRangeNearEdge(xdoc, s) )
   {
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << MODULE << "getSlantRangeNearEdge error! exiting\n";
      }
      return false;
   }
   double distance = s.toDouble();

   //---
   // in the case of Georeferenced images, the ground range is stored in
   // place of the slant range
   // (used for SlantRange computation relative to reference point,
   // necessary for optimization)
   //---
   if (_isProductGeoreferenced)
   {
      if (_sensor->get_col_direction() == 1)
      {
         distance += 0 ; // upper left corner
      }
      else
      {
         distance += _nbCol * _pixel_spacing;
      }
   }

   _refPoint->set_distance(distance);

   // Default optimization 
   optimizeModel(groundGcpCoordinates, imageGcpCoordinates) ;


   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = true\n";
   } 

   return true;
}

bool ossimRadarSat2Model::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   static const char MODULE[] = "ossimRadarSat2Model::saveState";

   bool result = false;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }

   //---
   // Temporary until we get a good saveState.
   // drb
   //---
   if (theProductDotXmlFile != ossimFilename::NIL)
   {      
   
      // Call base:
      ossimGeometricSarSensorModel::saveState(kwl, prefix);
      // ossimSensorModel::saveState(kwl, prefix);

      kwl.add(prefix, "nbCol", _nbCol);
      kwl.add(prefix, "pixel_spacing_mean", _pixel_spacing);
      kwl.add(prefix, "SrGr_coeffs_number", _n_srgr);

      // Make sure all the arrays are equal in size.
      const ossim_uint32 COUNT = static_cast<ossim_uint32>(_n_srgr);

      cout << "srgr_update.size(): " << _srgr_update.size()
           << "\n_SrGr_R0.size(): " << _SrGr_R0.size()
           << "\n_SrGr_coeffs.size(): " << _SrGr_coeffs.size()
           << endl;
      
      if ( (_srgr_update.size() == COUNT) &&
           (_SrGr_R0.size()     == COUNT) &&
           (_SrGr_coeffs.size() == COUNT) )
      {
         ossimString kw1 = "SrGr_update_";
         ossimString kw2 = "SrGr_R0_";
         ossimString kw3 = "SrGr_coeffs_";
         
         for(ossim_uint32 i = 0; i < COUNT; ++i)
         {
            ossimString iStr = ossimString::toString(i);
            
            ossimString kw = kw1;
            kw += iStr;
            cout << "kw: " << kw << endl;
            cout << "iStr: " << iStr << endl;
            cout << "i: " << i << endl;
            
            kwl.add(prefix, kw, _srgr_update[i]);

            kw = kw2;
            kw += iStr;
            cout << "kw: " << kw << endl;
            kwl.add(prefix, kw, _SrGr_R0[i]);

            cout << "_SrGr_coeffs[i].size()" << _SrGr_coeffs[i].size() << endl;
            for (ossim_uint32 j = 0; j < _SrGr_coeffs[i].size(); ++j)
            {
               ossimString jStr = ossimString::toString(j);
               kw = kw3;
               kw += iStr;
               kw += "_";
               kw += jStr;
               cout << "kw: " << kw << endl;
               kwl.add(prefix, kw,_SrGr_coeffs[i][j]);
            }
         }
      }
      else
      {
         // do something...
      }
      
      kwl.add(prefix,
              PRODUCT_XML_FILE_KW.c_str(),
              theProductDotXmlFile.c_str());
      result = true;
   }

   cout << "saveState kwl:\n" << kwl << endl;
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }
   
   return result;
}

bool ossimRadarSat2Model::loadState (const ossimKeywordlist &kwl,
                                     const char *prefix)
{
   static const char MODULE[] = "ossimRadarSat2Model::loadState";
   


   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " entered...\n"
         << kwl << "\n"; 
   }

   bool result = true;
   
   // bool result = ossimSensorModel::loadState(kwl, prefix);
   if (result)
   {
      //---
      // Temporary until we get a good saveState.
      // drb
      //---
      const char* lookup = kwl.find(prefix, PRODUCT_XML_FILE_KW.c_str());
      if (lookup)
      {
         result = open(ossimFilename(lookup));
      }
   }
      
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }
   
   return result;
}

bool ossimRadarSat2Model::setModelRefPoint(
   const std::list<ossimGpt>& groundGcpCoordinates,
   const std::list<ossimDpt>& imageGcpCoordinates)
{
   static const char MODULE[] = "ossimRadarSat2Model::setModelRefPoint";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }
   
   bool result = false;

   if ( !theImageSize.hasNans() &&
        (groundGcpCoordinates.size() == imageGcpCoordinates.size()) )
   {
      const ossim_float64 THRESHOLD = 1.0;
      ossimDpt center;
      center.x = theImageSize.x / 2.0;
      center.y = theImageSize.y / 2.0;
      std::list<ossimGpt>::const_iterator gi = groundGcpCoordinates.begin();
      std::list<ossimDpt>::const_iterator di = imageGcpCoordinates.begin();

      while( gi != groundGcpCoordinates.end() )
      {
         // Find the image point within one pixel of center.
         ossim_float64 deltaX = std::fabs( (*di).x - center.x);
         ossim_float64 deltaY = std::fabs( (*di).y - center.y);
         
         if ( (deltaX <= THRESHOLD) && (deltaY <= THRESHOLD) )
         {
            theRefImgPt = (*di);
            theRefGndPt = (*gi);
            result = true;

            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG)
                  << "image reference point: " << theRefImgPt
                  << "\nground reference point: " << theRefGndPt
                  << "\n";
            }

            break;
         }

         ++gi;
         ++di;
         
      } // matches: while( gi != groundGcpCoordinates.end() )
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << MODULE << " exit status = " << (result?"true":"false\n")
         << std::endl;
   }

   return result;
}
