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
#include <cstdio>

#include <ossimRadarSat2Model.h>
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
#include <ossim/support_data/ossimSupportFilesList.h>
#include <otb/GalileanEphemeris.h>
#include <otb/GeographicEphemeris.h>
#include <otb/GMSTDateTime.h>
#include <otb/PlatformPosition.h>
#include <otb/SensorParams.h>
#include <otb/RefPoint.h>
#include <otb/SarSensor.h>

namespace ossimplugins
{
// Keyword constants:
static const char NUMBER_SRGR_COEFFICIENTS_KW[] = "sr_gr_coeffs_count";
static const char LOAD_FROM_PRODUCT_FILE_KW[] = "load_from_product_file_flag";
static const char PRODUCT_XML_FILE_KW[] = "product_xml_filename";

// Static trace for debugging
static ossimTrace traceDebug("ossimRadarSat2Model:debug");

RTTI_DEF1(ossimRadarSat2Model, "ossimRadarSat2Model", ossimGeometricSarSensorModel);


ossimRadarSat2Model::ossimRadarSat2Model()
   :
   ossimGeometricSarSensorModel(),
   _n_srgr(0),
   _srgr_update(),
   _SrGr_R0()
{
}

ossimRadarSat2Model::ossimRadarSat2Model(const ossimRadarSat2Model& rhs)
   :
   ossimGeometricSarSensorModel(rhs),
   _n_srgr(rhs._n_srgr),
   _srgr_update(rhs._srgr_update),
   _SrGr_R0(rhs._SrGr_R0)
{
}

ossimRadarSat2Model::~ossimRadarSat2Model()
{
}

ossimString ossimRadarSat2Model::getClassName() const
{
   return ossimString("ossimRadarSat2Model");
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
   relativeGroundRange = _refPoint->get_distance() + _sensor->get_col_direction() * (col-_refPoint->get_pix_col())* theGSD.x;
   //relativeGroundRange = 1 + _sensor->get_col_direction() * (col-_refPoint->get_pix_col())* theGSD.x;
   //relativeGroundRange = (8.78400000e+03)*theGSD.x;
   
   if ( traceDebug() )
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "_refPoint->get_distance(): " << _refPoint->get_distance()
         << "\n_sensor->get_col_direction() " << _sensor->get_col_direction()
         << "\n(col-_refPoint->get_pix_col()) "
         << (col-_refPoint->get_pix_col())
         << "\n_refPoint->get_pix_col() : " << _refPoint->get_pix_col()
         << "\n relativeGroundRange : " << relativeGroundRange << endl;
   }
   
   int numSet = FindSRGRSetNumber((_refPoint->get_ephemeris())->get_date()) ;
   /**
    * @todo : could be improved (date choice)
    */
   
   for (int i=0 ; i < static_cast<int>(_SrGr_coeffs[numSet].size()); i++)
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
   _imageFilename = file.expand();

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
                     
                     	  if (result)
                        {
                        result = InitRefNoiseLevel(xdoc);
                        }
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
      _productXmlFile = xmlFile;
      ossimSupportFilesList::instance()->add(_productXmlFile);
   }
   else
   {
      _productXmlFile = ossimFilename::NIL;
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

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "theImageClipRect : " << theImageClipRect
            << "ul, ur, lr, ll " << ul << ", " << ur
            << ", " << lr << " , " << ll << endl;
      }
      
      setGroundRect(ul, ur, lr, ll);  // ossimSensorModel method.

      // OSSIM preferences specifies whether a coarse grid needs to be generated:
      if (!createReplacementOCG())
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

std::ostream& ossimRadarSat2Model::print(std::ostream& out) const
{
   // Capture the original flags.
   std::ios_base::fmtflags f = out.flags();

   out << setprecision(15) << setiosflags(ios::fixed)
       << "\nossimRadarSat2Model class data members:\n"
       << "_n_srgr: " << _n_srgr << "\n";

   ossim_uint32 idx = 0;
   std::vector<double>::const_iterator i = _srgr_update.begin();
   while ( i !=  _srgr_update.end() )
   {
      out << "sr_gr_update_" << idx << ": " << (*i) << "\n";
      ++i;
      ++idx;
   }

   idx = 0;
   i = _SrGr_R0.begin();
   while ( i !=  _SrGr_R0.end() )
   {
      out << "sr_gr_r0_" << idx << ": " << (*i) << "\n";
      ++i;
      ++idx;
   }

   idx = 0;
   std::vector< std::vector<double> >::const_iterator i2 =
      _SrGr_coeffs.begin();
   while ( i2 !=  _SrGr_coeffs.end() )
   {
      ossim_uint32 idx2 = 0;
      i = (*i2).begin();
      while ( i != (*i2).end() )
      {
         out << "sr_gr_coeffs_" << idx << "_" << idx2 << ": " << (*i) << "\n";
         ++i;
         ++idx2;
      }
      ++i2;
      ++idx;
   }

   ossimGeometricSarSensorModel::print(out);
   

   // Reset flags.
   out.setf(f);

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
   double ellip_maj = atof(ellip_maj_str) * 1000.0;  // km -> m
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

   _refPoint->set_pix_col(0);    // upper left corner
   _refPoint->set_pix_line(0);  // upper left corner

   const char* zeroDopplerTimeFirstLine_str = kwl.find(prefix,"zeroDopplerTimeFirstLine");
   std::string zeroDopplerTimeFirstLine(zeroDopplerTimeFirstLine_str);

   CivilDateTime * date = new CivilDateTime() ;
   if (! ossim::iso8601TimeStringToCivilDate(zeroDopplerTimeFirstLine, *date)) 
     {
     delete date;
     return false ;
     }

   if (_sensor->get_lin_direction() == -1) {
      double time = (double) date->get_second() + date->get_decimal() ;  // upper left corner
      time += theImageSize.y / _sensor->get_prf() ;
      date->set_second((int) floor(time)) ;
      date->set_decimal(time - floor(time)) ;
   }
  
   if(_platformPosition != 0)
   {
      Ephemeris * ephemeris = _platformPosition->Interpolate((JSDDateTime)*date);
      if (ephemeris == 0) 
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
         distance += theImageSize.x * theGSD.x;
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

   delete date;

   return true;
}

bool ossimRadarSat2Model::InitSRGR(const ossimKeywordlist &kwl,
                                   const char *prefix)
{
   const char* productType_str = kwl.find(prefix,"productType");
   ossimString productType(productType_str);

   _isProductGeoreferenced = (productType != "SLC") ;

//    // Pixel spacing
//    const char* pixel_spacing_str = kwl.find(prefix,"pixel_spacing_mean");
//    _pixel_spacing = atof(pixel_spacing_str);

//    // Number of columns
//    const char* nbCol_str = kwl.find(prefix,"nbCol");
//    _nbCol = atoi(nbCol_str);

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
      ossim::iso8601TimeStringToCivilDate(std::string(SrGr_update_str), SrGr_update_date) ;
      _srgr_update.push_back((double) SrGr_update_date.get_second()+ SrGr_update_date.get_decimal());
   }

   return true;
}

int ossimRadarSat2Model::FindSRGRSetNumber(JSDDateTime date) const
{
   if (_n_srgr==0) return(-1) ;

   double * delays = new double[_n_srgr];

   std::fill_n(delays,_n_srgr,0);

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
   delete [] delays;
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
               ossim::iso8601TimeStringToCivilDate(node->getText(), date);
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

   if (!result)
   {
      delete _platformPosition;
      _platformPosition = 0;
   }

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
      _refPoint->set_pix_line(0);  // upper left corner
   }

   ossimString s;
   CivilDateTime date;
   if ( rsDoc.getZeroDopplerTimeFirstLine(xdoc, s) )
   {
      if (! ossim::iso8601TimeStringToCivilDate(s, date))
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
      date.set_decimal(time - floor(time));
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

   double distance = 1;

   // Only set distance to 
   if (!_isProductGeoreferenced)
   {
	   if ( !rsDoc.getSlantRangeNearEdge(xdoc, s) )
	   {
		   if (traceDebug())
		   {
			   ossimNotify(ossimNotifyLevel_DEBUG)
    
				   << MODULE << "getSlantRangeNearEdge error! exiting\n";
		   }      
		   return false;
	   }
	   distance = s.toDouble();
   }

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
         distance += theImageSize.x * theGSD.x;
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


bool ossimRadarSat2Model::InitLut( const ossimXmlDocument* xmlDocument,
   			RadarSat2NoiseLevel& noise)
{
   static const char MODULE[] = "ossimRadarSat2Model::initLut";
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }

   ossimString xpath;
   ossimString incidenceAngleCorrectionName;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node;
   ossimFilename  lutXmlFile;
      
   incidenceAngleCorrectionName = noise.get_incidenceAngleCorrectionName();     

   xpath = "/product/imageAttributes/lookupTable";
   xml_nodes.clear();
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
		if( (*node)->getAttributeValue("incidenceAngleCorrection") == incidenceAngleCorrectionName )
		{
			// Get the xml file.
			lutXmlFile = _productXmlFile.expand().path().dirCat((*node)->getText());

   			if ( lutXmlFile.exists() )
   			{
      			//---
      			// Instantiate the XML parser:
      			//---
      			ossimXmlDocument* xmlLutDocument = new ossimXmlDocument();
      			if ( xmlLutDocument->openFile(lutXmlFile) )
      			{
   					std::vector<ossimRefPtr<ossimXmlNode> > xml_lutNodes;
   					ossimString s;
   					
   					xpath = "/lut/offset";
   					xml_lutNodes.clear();
   					xmlLutDocument->findNodes(xpath, xml_lutNodes);   					
   					if(xml_lutNodes.size() == 0)
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
   					ossim_float64 offset = xml_lutNodes[0]->getText().toFloat64();  
					noise.set_offset(offset);				
								
   					xpath = "/lut/gains";
   					xml_lutNodes.clear();
   					xmlLutDocument->findNodes(xpath, xml_lutNodes);   					
   					if(xml_lutNodes.size() == 0)
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
					noise.set_gain(xml_lutNodes[0]->getText());	
				}
   			}
		}
    	++node;	
   }

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " leaving...\n";
   }   

   return true;
}

bool ossimRadarSat2Model::InitRefNoiseLevel(
   const ossimXmlDocument* xmlDocument)
{
   static const char MODULE[] = "ossimRadarSat2Model::initRefNoiseLevel";

   ossimString xpath;
   std::vector<ossimRefPtr<ossimXmlNode> > xml_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> > sub_nodes;
   std::vector<ossimRefPtr<ossimXmlNode> >::iterator node;
   RadarSat2NoiseLevel ev;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }

   _noiseLevel.clear();
   
   
   xpath = "/product/sourceAttributes/radarParameters/referenceNoiseLevel";
   xml_nodes.clear();
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
   	
   	ev.set_incidenceAngleCorrectionName( (*node)->getAttributeValue("incidenceAngleCorrection") ); 
   	
    sub_nodes.clear();
    xpath = "pixelFirstNoiseValue";
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
    ev.set_pixelFirstNoiseValue(sub_nodes[0]->getText().toUInt32());

    sub_nodes.clear();
    xpath = "stepSize";
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
    ev.set_stepSize(sub_nodes[0]->getText().toUInt32());

    sub_nodes.clear();
    xpath = "numberOfNoiseLevelValues";
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
    ev.set_numberOfNoiseLevelValues(sub_nodes[0]->getText().toUInt32());

    sub_nodes.clear();
    xpath = "noiseLevelValues";
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
   	ev.set_units( sub_nodes[0]->getAttributeValue("units") ); 


    std::vector<ossimString> s2;      
    std::vector<ossim_float64> noiseLevelValues;
    s2.clear();
    noiseLevelValues.clear();
    s2 = sub_nodes[0]->getText().split(" ");
	for(ossim_uint32 i = 0; i < s2.size(); ++i)
	{
		noiseLevelValues.push_back( s2[i].toFloat64() );
	}
   	ev.set_noiseLevelValues( noiseLevelValues ); 

	InitLut(xmlDocument, ev);


    _noiseLevel.push_back(ev);

    ++node;
   }
 
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " leaving...\n";
   }   

   return true;
}




bool ossimRadarSat2Model::saveState(ossimKeywordlist& kwl,
                                    const char* prefix) const
{
   static const char MODULE[] = "ossimRadarSat2Model::saveState";

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)<< MODULE << " entered...\n";
   }

   bool result = true;

   // Save our state:
   kwl.add(prefix, PRODUCT_XML_FILE_KW, _productXmlFile.c_str());
   kwl.add(prefix, NUMBER_SRGR_COEFFICIENTS_KW, _n_srgr);

   // Make sure all the arrays are equal in size.
   const ossim_uint32 COUNT = static_cast<ossim_uint32>(_n_srgr);

   if ( (_srgr_update.size() == COUNT) &&
        (_SrGr_R0.size()     == COUNT) &&
        (_SrGr_coeffs.size() == COUNT) )
   {
      ossimString kw1 = "sr_gr_update_";
      ossimString kw2 = "sr_gr_r0_";
      ossimString kw3 = "sr_gr_coeffs_";

      for(ossim_uint32 i = 0; i < COUNT; ++i)
      {
         ossimString iStr = ossimString::toString(i);

         ossimString kw = kw1;
         kw += iStr;
         kwl.add(prefix, kw, _srgr_update[i]);

         kw = kw2;
         kw += iStr;
         kwl.add(prefix, kw, _SrGr_R0[i]);

         for (ossim_uint32 j = 0; j < _SrGr_coeffs[i].size(); ++j)
         {
            ossimString jStr = ossimString::toString(j);
            kw = kw3;
            kw += iStr;
            kw += "_";
            kw += jStr;
            kwl.add(prefix, kw,_SrGr_coeffs[i][j]);
         }
      }
   }
   else
   {
      result = false;
   }

   if (result)
   {
      // Call base save state:
      result = ossimGeometricSarSensorModel::saveState(kwl, prefix);
   }

   if (result)
   {
      	for(ossim_uint32 i = 0; i < _noiseLevel.size(); ++i)
   		{	
   				_noiseLevel[i].saveState(kwl, prefix);
   		}
       
   }

   //---
   // Uncomment to force load from product file instead of loadState.
   //---
   //if (result)
   //{
   // kwl.add(prefix, LOAD_FROM_PRODUCT_FILE_KW, "true");
   //}

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
      lookup = kwl.find(prefix, NUMBER_SRGR_COEFFICIENTS_KW);
      if (lookup)
      {
         s = lookup;
         _n_srgr = s.toInt();
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

      if (result && _n_srgr)
      {
         const ossim_uint32 COUNT = static_cast<ossim_uint32>(_n_srgr);

         _srgr_update.resize(COUNT);
         _SrGr_R0.resize(COUNT);
         _SrGr_coeffs.resize(COUNT);

         ossimString kw1 = "sr_gr_update_";
         ossimString kw2 = "sr_gr_r0_";
         ossimString kw3 = "sr_gr_coeffs_";

         for(ossim_uint32 i = 0; i < COUNT; ++i)
         {
            ossimString kw;
            ossimString iStr = ossimString::toString(i);

            // Get the _srgr_update's.
            kw = kw1;
            kw += iStr;

            lookup = kwl.find(prefix, kw);
            if (lookup)
            {
               s = lookup;
               _srgr_update[i] = s.toDouble();
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

            // Get the sr_gr_r0_'s.
            kw = kw2;
            kw += iStr;
            lookup = kwl.find(prefix, kw);
            if (lookup)
            {
               s = lookup;
               _SrGr_R0[i] = s.toDouble();
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

            //---
            // Get the _SrGr_coeffs.
            // Note we are assuming a count of 6.
            //---
            const ossim_uint32 COEFFS_COUNT = 6;
            _SrGr_coeffs[i].resize(COEFFS_COUNT);

            for (ossim_uint32 j = 0; j < COEFFS_COUNT; ++j)
            {
               ossimString jStr = ossimString::toString(j);
               kw = kw3;
               kw += iStr;
               kw += "_";
               kw += jStr;
               lookup = kwl.find(prefix, kw);
               if (lookup)
               {
                  s = lookup;
                  _SrGr_coeffs[i][j] = s.toDouble();
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

         } // matches: for(ossim_uint32 i = 0; i < COUNT; ++i)

      } // matches:  if (_n_srgr)
      else
      {
         result = false;
      }

   } // matches: if (result)

	if(result)
	{
      	for(ossim_uint32 i = 0; i < _noiseLevel.size(); ++i)
   		{	
   				_noiseLevel[i].loadState(kwl, prefix);
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
}
